/*
 * TreeReader.cpp
 *
 *  Created on: Jul 18, 2019
 *      Author: dylan
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <TVector3.h>

#include "TreeReader.h"
#include "file_io.h"
#include "ratio_methods.h"

#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"
#include "Event.h"
#include "Track.h"

#include "Mixer.h"
#include "MixerSets.h"

using namespace std;


// Structors

TreeReader::TreeReader(int energy, int ref_num) {
	start_sys = chrono::system_clock::now();

	ampt = false;
	cooper_frye = false;
	file_list = NULL;

	sim_eff = false;
	sim_flow = false;

	pile_up = false;
	pile_up_prob = 0;

	this->ref_num = ref_num;
	this->energy = energy;
}

TreeReader::TreeReader(int energy, int ref_num, mutex *mtx) {
	start_sys = chrono::system_clock::now();

	this->mtx = mtx;

	ampt = false;
	cooper_frye = false;
	file_list = NULL;

	sim_eff = false;
	sim_flow = false;

	pile_up = false;
	pile_up_prob = 0;
	
	this->ref_num = ref_num;
	this->energy = energy;
}

TreeReader::TreeReader(int energy) {
	start_sys = chrono::system_clock::now();

	ampt = false;
	cooper_frye = false;
	file_list = NULL;

	sim_eff = false;
	sim_flow = false;

	pile_up = false;
	pile_up_prob = 0;
	
	ref_num = 2;
	this->energy = energy;
}

TreeReader::TreeReader(int energy, mutex *mtx) {
	start_sys = chrono::system_clock::now();

	this->mtx = mtx;

	ampt = false;
	cooper_frye = false;
	file_list = NULL;

	sim_eff = false;
	sim_flow = false;

	pile_up = false;
	pile_up_prob = 0;

	ref_num = 2;
	this->energy = energy;
}

TreeReader::TreeReader() {
	start_sys = chrono::system_clock::now();

	ampt = false;
	cooper_frye = false;
	file_list = NULL;

	sim_eff = false;
	sim_flow = false;

	pile_up = false;
	pile_up_prob = 0;
	
	ref_num = 2;
	energy = 7;
}


TreeReader::~TreeReader() {
//	delete refmultCorrUtil;
}


// Getters

string TreeReader::get_in_path() {
	return(in_path);
}

string TreeReader::get_out_path() {
	return(out_path);
}

string TreeReader::get_tree_name() {
	return(tree_name);
}

string TreeReader::get_event_cut_hist_name() {
	return(event_cut_hist_name);
}

string TreeReader::get_track_cut_hist_name() {
	return(track_cut_hist_name);
}

string TreeReader::get_particle() {
	return particle;
}


// Setters

void TreeReader::set_in_path(string path) {
	in_path = path;
}

void TreeReader::set_out_path(string path) {
	out_path = path;
}

void TreeReader::set_qa_path(string path) {
	qa_path = path;
}

void TreeReader::set_ampt_cent_path(string path) {
	ampt_cent_path = path;
}

void TreeReader::set_ampt_type(string type) {
	ampt_type = type;
}

void TreeReader::set_tree_name(string tree_name) {
	this->tree_name = tree_name;
}

void TreeReader::set_sim_proton_dist_dataset(string path) {
	sim_proton_dist_dataset = path;
}

void TreeReader::set_sim_eff_dist_path(string root_path, string hist_name) {
	if((int)sim_eff_dist_path.size() > 0) { sim_eff_dist_path.clear(); }
	sim_eff_dist_path.push_back(root_path);
	sim_eff_dist_path.push_back(hist_name);
}

void TreeReader::set_energy(int energy) {
	this->energy = energy;
}

void TreeReader::set_ampt(bool ampt) {
	this->ampt = ampt;
}

void TreeReader::set_cooper_frye(bool cf) {
	this->cooper_frye = cf;
}

void TreeReader::set_pile_up(bool pile_up) {
	this->pile_up = pile_up;
}

void TreeReader::set_sim_eff(bool sim_eff) {
	this->sim_eff = sim_eff;
}

void TreeReader::set_sim_flow(bool sim_flow) {
	this->sim_flow = sim_flow;
}

void TreeReader::set_pile_up_prob(double pile_up_prob) {
	this->pile_up_prob = pile_up_prob;
}

void TreeReader::set_particle(string particle) {
	this->particle = particle;
}

//void TreeReader::set_particle_dist_hist_max(int max) {
//	particle_dist_hist_max = max;
//}

void TreeReader::set_ampt_particle_pid(vector<int> pid) {
	pdg_particle_pid = pid;
}

void TreeReader::set_file_list(vector<string> *file_list) {
	this->file_list = file_list;
}

void TreeReader::set_tree_reader_rand_seed(int seed) {
	tree_reader_seed = seed;
	trand = new TRandom3(seed);
}

void TreeReader::set_file_shuffle_rand_seed(int seed) {
	file_shuffle_seed = seed;
	srand(seed);
}

// Create new binner with TreeReader energy and ref_num and return reference to new AzBinner
AzBinner& TreeReader::add_binner() {
	binners.emplace_back(AzBinner(energy, ref_num));
	return binners.back();
}



// Doers


// Read files for single energy and write results to text files.
void TreeReader::read_trees() {
	for (AzBinner& binner : binners) {
		binner.prep_read();
		binner.define_qa();
	}

	cout << "Reading " + to_string(energy) + "GeV trees." << endl << endl;

	vector<string> in_files;
	if (ampt) {
		ampt_cent = AmptCentralityMaker(energy, in_path + "min_bias/" + ampt_type + "/", ampt_cent_path + ampt_type + "/", "ref" + to_string(ref_num));
		ampt_cent.make_centrality(false);  // Usually just reads from file unless it can't find it.

		in_files = get_files_in_dir(in_path + "most_central/" + ampt_type + "/" + to_string(energy) + "GeV/", "root", "path");
		vector<string> min_bias_files = get_files_in_dir(in_path + "min_bias/" + ampt_type + "/" + to_string(energy) + "GeV/", "root", "path");
		in_files.insert(in_files.end(), min_bias_files.begin(), min_bias_files.end());
	}
//	else if (cooper_frye) {
//		in_files = get_files_in_dir(in_path + to_string(energy) + "GeV/", "root", "path");
//	}
	else {
		in_files = get_files_in_dir(in_path + to_string(energy) + "GeV/", "root", "path");
	}
	
	random_shuffle(in_files.begin(), in_files.end());

	unsigned num_files = in_files.size();
	unsigned file_index = 1;

	queue<string> in_files_queue;
	for (const string& path : in_files) { in_files_queue.push(path); }

	while (!in_files_queue.empty()) {
		string path = in_files_queue.front();

		if(file_list != NULL) {  // If file_list passed, check if path is being read on another thread before continuing
			bool wait = true;
			bool skip = false;
			while(wait) {
				if(mtx) { mtx->lock(); }
				if(find(file_list->begin(), file_list->end(), path) == file_list->end()) {
					in_files_queue.pop();
					file_list->push_back(path);
					wait = false;
				}
				if(mtx) { mtx->unlock(); }
				if(wait) {
					if (in_files_queue.size() == 1) {  // Last file in queue, just have to wait for it to open up
						cout << "Waiting for path: " << path << endl;
						this_thread::sleep_for(chrono::seconds(file_wait_sleep));
					}
					else {  // Put current file at end of queue and move on to next file
						cout << "Pushing to back of queue path: " << path << endl;
						in_files_queue.push(path);
						in_files_queue.pop();
						skip = true;
						break;
					}
				}
			}
			if (skip) { continue; }  // Skip to next file and return to current (busy one) at the end.
		}

		// Display progress and time while running.
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
			cout << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
		}

		TFile *file = new TFile(path.data(), "READ");
		if (!ampt && !cooper_frye) { for (AzBinner& binner : binners) { binner.add_cut_hists(file); } }
		TTree *tree = (TTree*)file->Get(tree_name.data());
		if (ampt) { read_ampt_tree(tree); }  // Read tree from file into data
		else if (cooper_frye) { read_cooper_frye_tree(tree); }
		else { read_tree(tree); }  // Read tree from file into data
		tree->ResetBranchAddresses();
		file->Close();
		//delete file;
		file_index++;

		if(file_list != NULL) {  // If file_list passed, remove path before going to next
			if(mtx) { mtx->lock(); }
			vector<string>::iterator it = find(file_list->begin(), file_list->end(), path);
			if(it == file_list->end()) {
				cout << "Current file not found in list! Not removing: " << path << endl;
			} else {
				file_list->erase(it);
			}
			if(mtx) { mtx->unlock(); }
		}
	}

	for (AzBinner& binner : binners) {
		binner.reset_out_dir();
		binner.write_info_file();
		if (mtx) mtx->lock();
		binner.write_qa();
		if (mtx) mtx->unlock();
		binner.write_binner_data();
	}

	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << "Writing " + to_string(energy) + "GeV trees. 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
	
}


void TreeReader::sim_events(map<int, int> cent_num_events) {
	for (AzBinner& binner : binners) {
		binner.prep_read();
		binner.define_qa();
	}

	if(sim_eff) {
		if((int)sim_eff_dist_path.size() == 0)  {
			sim.set_efficiency_dist_hist(get_sim_efficiency_dist());
		} else {
			sim.set_efficiency_dist_hist(sim_eff_dist_path[0], sim_eff_dist_path[1]);
		}
	}

	if(sim_eff) {
		if(sim_flow) { sim.set_eff_flow(); }
		else { sim.set_eff(); }
	} else if(sim_flow) { sim.set_flow(); }

	sim.track_defs = track_defs;

	int total_events = 0;
	for(auto cent:cent_num_events) {
		total_events += cent.second;
		if(sim.get_proton_dist_type() == "hist") { cout << "hist" << endl; sim.set_proton_dist_hist(get_sim_proton_dist(cent.first)); }
		for(int i=0; i<cent.second; i++) {
			if(!(i % (int)(cent.second/10.0+0.5))) {
				chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
				auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
				vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
				cout << " " << energy << "GeV Centrality " << cent.first << " " << (int)(100.0*i/cent.second+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
			}
			Event event(event_defs, energy, ref_num, cent.first);
			sim.simulate_event(event);

			for (AzBinner& binner : binners) {
				binner.process_event(event);
			}
		}
	}

	for (AzBinner& binner : binners) {
		binner.set_sim_pars(sim.get_sim_pars());  // Only needed in order to write values to info file
		binner.reset_out_dir();
		binner.write_info_file();
		if (mtx) mtx->lock();
		binner.write_qa();
		if (mtx) mtx->unlock();
		binner.write_binner_data();
	}

	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << "Writing " + to_string(energy) + "GeV " << total_events << " simulated events. 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
}


// Read individual tree. Read out and then process each event.
void TreeReader::read_tree(TTree* tree) {
	set_tree_branches(tree, branches, particle, ref_num);
	set_branches(tree);

	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		Event event(branches);

		if(pile_up) {
			if(trand->Rndm() < pile_up_prob) {  // Pile up next two events
				event_index++;
				if(tree->GetEntry(event_index)) {
					Event event2(branches);
					event.pile_up(event2);
				}
			}
		}

		for (AzBinner& binner : binners) {
			binner.process_event(event);
		}

		event_index++;
	}
}


void TreeReader::read_ampt_tree(TTree* tree) {
	set_ampt_tree_branches(tree, ampt_branches);
	set_ampt_branches(tree);

	int* ref_n = NULL;
	switch (ref_num) {
	case 1:
		ref_n = &ampt_branches.refmult; break;
	case 2:
		ref_n = &ampt_branches.refmult2; break;
	case 3:
		ref_n = &ampt_branches.refmult3; break;
	default:
		cout << "Unknown ref_num value!" << endl;
	}

	int cent_9bin;
	int event_index = 0;
	while (tree->GetEvent(event_index)) {

		cent_9bin = ampt_cent.get_cent_bin9(*ref_n);
		Event event(event_defs, energy, ref_num, cent_9bin);
		event.set_qx(ampt_branches.qx); event.set_qy(ampt_branches.qy);

		vector<Track> particles;
		for (int particle_index = 0; particle_index < (int)ampt_branches.pid->size(); particle_index++) {
			if (!count(pdg_particle_pid.begin(), pdg_particle_pid.end(), ampt_branches.pid->at(particle_index))) { continue; }
			TVector3 p(ampt_branches.px->at(particle_index), ampt_branches.py->at(particle_index), ampt_branches.pz->at(particle_index));
			Track new_particle(track_defs);
			new_particle.set_p(p.Mag());
			new_particle.set_pt(p.Perp());
			new_particle.set_phi(p.Phi() + M_PI);
			new_particle.set_eta(p.PseudoRapidity());
			new_particle.set_charge(ampt_branches.pid->at(particle_index) / fabs(ampt_branches.pid->at(particle_index)));
			new_particle.set_beta(pow((cut.max_m2 + cut.min_m2) / (2 * pow(p.Mag(), 2)) + 1, -0.5));  // Pass m^2 cut
			particles.push_back(new_particle);
		}
		event.set_particles(particles);

		if (pile_up) {
			if (trand->Rndm() < pile_up_prob) {  // Pile up next two events
				event_index++;
				if (tree->GetEntry(event_index)) {
					cent_9bin = ampt_cent.get_cent_bin9(*ref_n);
					Event event2(event_defs, energy, ref_num, cent_9bin);
					event2.set_qx(ampt_branches.qx); event.set_qy(ampt_branches.qy);

					particles.clear();
					for (int particle_index = 0; particle_index < (int)ampt_branches.pid->size(); particle_index++) {
						if (!count(pdg_particle_pid.begin(), pdg_particle_pid.end(), ampt_branches.pid->at(particle_index))) { continue; }
						TVector3 p(ampt_branches.px->at(particle_index), ampt_branches.py->at(particle_index), ampt_branches.pz->at(particle_index));
						Track new_particle(track_defs);
						new_particle.set_p(p.Mag());
						new_particle.set_pt(p.Perp());
						new_particle.set_phi(p.Phi() + M_PI);
						new_particle.set_eta(p.PseudoRapidity());
						new_particle.set_charge(ampt_branches.pid->at(particle_index) / fabs(ampt_branches.pid->at(particle_index)));
						new_particle.set_beta(pow((cut.max_m2 + cut.min_m2) / (2 * pow(p.Mag(), 2)) + 1, -0.5));  // Pass m^2 cut
						particles.push_back(new_particle);
					}
					event2.set_particles(particles);

					event.pile_up(event2);
				}
			}
		}

		for (AzBinner& binner : binners) {
			binner.process_event(event);
		}

		event_index++;
	}
}



void TreeReader::read_cooper_frye_tree(TTree* tree) {
	set_cooper_frye_tree_branches(tree, cooper_frye_branches);
	set_cooper_frye_branches(tree);

	int* ref_n = NULL;
	switch (ref_num) {
	case 1:
		ref_n = &cooper_frye_branches.refmult; break;
	case 2:
		ref_n = &cooper_frye_branches.refmult2; break;
	case 3:
		ref_n = &cooper_frye_branches.refmult3; break;
	default:
		cout << "Unknown ref_num value!" << endl;
	}

	int cent_9bin = 8;
	int event_index = 0;
	while (tree->GetEvent(event_index)) {

		Event event(event_defs, energy, ref_num, cent_9bin);
		event.set_qx(cooper_frye_branches.qx); event.set_qy(cooper_frye_branches.qy);

		vector<Track> particles;
		for (int particle_index = 0; particle_index < (int)cooper_frye_branches.pid->size(); particle_index++) {
			if (!count(pdg_particle_pid.begin(), pdg_particle_pid.end(), cooper_frye_branches.pid->at(particle_index))) { continue; }
			TVector3 p(cooper_frye_branches.px->at(particle_index), cooper_frye_branches.py->at(particle_index), cooper_frye_branches.pz->at(particle_index));
			Track new_particle(track_defs);
			new_particle.set_p(p.Mag());
			new_particle.set_pt(p.Perp());
			new_particle.set_phi(p.Phi() + M_PI);
			new_particle.set_eta(p.PseudoRapidity());
			new_particle.set_charge(cooper_frye_branches.pid->at(particle_index) / fabs(cooper_frye_branches.pid->at(particle_index)));
			new_particle.set_beta(pow((cut.max_m2 + cut.min_m2) / (2 * pow(p.Mag(), 2)) + 1, -0.5));  // Pass m^2 cut
			particles.push_back(new_particle);
		}
		event.set_particles(particles);

		if (pile_up) {
			if (trand->Rndm() < pile_up_prob) {  // Pile up next two events
				event_index++;
				if (tree->GetEntry(event_index)) {
					cent_9bin = ampt_cent.get_cent_bin9(*ref_n);
					Event event2(event_defs, energy, ref_num, cent_9bin);
					event2.set_qx(cooper_frye_branches.qx); event.set_qy(cooper_frye_branches.qy);

					particles.clear();
					for (int particle_index = 0; particle_index < (int)cooper_frye_branches.pid->size(); particle_index++) {
						if (!count(pdg_particle_pid.begin(), pdg_particle_pid.end(), cooper_frye_branches.pid->at(particle_index))) { continue; }
						TVector3 p(cooper_frye_branches.px->at(particle_index), cooper_frye_branches.py->at(particle_index), cooper_frye_branches.pz->at(particle_index));
						Track new_particle(track_defs);
						new_particle.set_p(p.Mag());
						new_particle.set_pt(p.Perp());
						new_particle.set_phi(p.Phi() + M_PI);
						new_particle.set_eta(p.PseudoRapidity());
						new_particle.set_charge(cooper_frye_branches.pid->at(particle_index) / fabs(cooper_frye_branches.pid->at(particle_index)));
						new_particle.set_beta(pow((cut.max_m2 + cut.min_m2) / (2 * pow(p.Mag(), 2)) + 1, -0.5));  // Pass m^2 cut
						particles.push_back(new_particle);
					}
					event2.set_particles(particles);

					event.pile_up(event2);
				}
			}
		}

		for (AzBinner& binner : binners) {
			binner.process_event(event);
		}

		event_index++;
	}
}


void TreeReader::set_branches(TTree* tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("run_num", 1);
	tree->SetBranchStatus("event_id", 1);
	tree->SetBranchStatus("refmult", 1);
	tree->SetBranchStatus(("refmult"+to_string(ref_num)).data(), 1);
	tree->SetBranchStatus("btof_multi", 1);
	tree->SetBranchStatus("btof_match", 1);
	tree->SetBranchStatus("vz", 1);
	tree->SetBranchStatus("qx", 1);
	tree->SetBranchStatus("qy", 1);
	tree->SetBranchStatus((particle+".pt").data(), 1);
	tree->SetBranchStatus((particle+".phi").data(), 1);
	tree->SetBranchStatus((particle+".beta").data(), 1);
	tree->SetBranchStatus((particle+".charge").data(), 1);
	tree->SetBranchStatus((particle+".dca").data(), 1);
	tree->SetBranchStatus((particle+".nsigma").data(), 1);
	tree->SetBranchStatus((particle+".eta").data(), 1);
	tree->SetBranchStatus((particle + ".nhits_fit").data(), 1);
}


void TreeReader::set_ampt_branches(TTree* tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("qx", 1);
	tree->SetBranchStatus("qy", 1);
	tree->SetBranchStatus("pid", 1);
	tree->SetBranchStatus("px", 1);
	tree->SetBranchStatus("py", 1);
	tree->SetBranchStatus("pz", 1);

	switch(ref_num) {
	case 1 :
		tree->SetBranchStatus("refmult", 1); break;
	case 2 :
		tree->SetBranchStatus("refmult2", 1); break;
	case 3 :
		tree->SetBranchStatus("refmult3", 1); break;
	default :
		cout << "Unknown ref_num value!" << endl;
	}
}


void TreeReader::set_cooper_frye_branches(TTree* tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("qx", 1);
	tree->SetBranchStatus("qy", 1);
	tree->SetBranchStatus("pid", 1);
	tree->SetBranchStatus("px", 1);
	tree->SetBranchStatus("py", 1);
	tree->SetBranchStatus("pz", 1);

	switch(ref_num) {
	case 1 :
		tree->SetBranchStatus("refmult", 1); break;
	case 2 :
		tree->SetBranchStatus("refmult2", 1); break;
	case 3 :
		tree->SetBranchStatus("refmult3", 1); break;
	default :
		cout << "Unknown ref_num value!" << endl;
	}
}


// Pass TreeReader attributes down to AzBinner, if for no other reason than to write to info file.
void TreeReader::set_binner(AzBinner& binner) {
	binner.set_energy(energy);
	binner.set_ref_num(ref_num);
	binner.set_particle(particle);
	binner.set_ampt_particle_pid(pdg_particle_pid);
	binner.set_ampt(ampt);
	binner.set_pile_up(pile_up);
	binner.set_pile_up_prob(pile_up_prob);

}

// Get proton distribution for simulation from real data QA plots.
TH1D* TreeReader::get_sim_proton_dist(int cent) {
	string file_path = sim_proton_dist_dataset + to_string(energy) + "GeV/" + sim_dist_qa_name + to_string(energy) + "GeV.root";
	TFile *file = new TFile(file_path.data(), "READ");
	string dataset_path = sim_proton_dist_dataset;
	dataset_path.pop_back();
	string dataset = get_name_from_path(dataset_path);
	string hist_name = ("Proton_Dist_"+dataset+"_"+to_string(energy)+"_"+to_string(cent)).data();

	TH1D *hist = (TH1D*)file->Get(hist_name.data());
	TH1D *proton_dist = (TH1D*)hist->Clone();
	proton_dist->SetDirectory(0);

	file->Close();
	delete file;

	return proton_dist;
}


// Get efficiency distribution for simulation from real data QA plots.
TH1D* TreeReader::get_sim_efficiency_dist() {
	string file_path = sim_proton_dist_dataset + to_string(energy) + "GeV/" + sim_dist_qa_name + to_string(energy) + "GeV.root";
	TFile *file = new TFile(file_path.data(), "READ");
	string dataset_path = sim_proton_dist_dataset;
	dataset_path.pop_back();
	string dataset = get_name_from_path(dataset_path);
	string hist_name = ("post_phi_"+dataset+"_"+to_string(energy)).data();

	TH1D *hist = (TH1D*)file->Get(hist_name.data());
	TH1D *efficiency_dist = (TH1D*)hist->Clone();
	efficiency_dist->SetDirectory(0);

	file->Close();
	delete file;

	return efficiency_dist;
}


// Read individual tree. Read each event and for good events/tracks, calculate ratio values and save to data.
void TreeReader::read_tree_debug(TTree* tree) {
//	tree_leaves leaves = get_tree_leaves(tree);
//	Event *event_pointer = new Event;
//	TBranch *event_branch = tree->GetBranch("Event");
//	event_branch->SetAddress(&event_pointer);

	int event_index = 0;
	while(tree->GetEntry(event_index)) {
//		Event event(event_pointer);
//		Event event(leaves);
//		Event event = *event_pointer;

//		cout << "Got Event address" << endl;

//		if(pile_up) {
//			if(trand->Rndm() < pile_up_prob) {  // Pile up next two events
//				event_index++;
//				if(tree->GetEntry(event_index)) {
////					Event event2 = *event_pointer;
////					event.pile_up(event2);
//				}
//			}
//		}

//		cout << "Checked pile-up" << endl;

//		// Check if each event is good. Analyze if so, continue if not.
//		if(check_event_good(event)) {
//			vector<double> good_proton_angles = {};
//
////			cout << "Get protons" << endl;
//			// Iterate over protons in event and add corresponding phi to good_proton_angles if proton good.
//			for(Track proton:event.get_protons()) {
//				if(check_proton_good(proton)) {
//					if(efficiency) {  // Skip good proton with chance efficiency_prob
//						if(trand->Rndm() < efficiency_prob) { continue; }
//					}
//					good_proton_angles.push_back(proton.get_phi());
//				}
//			}
//
////			cout << "Do refmultCorr Stuff" << endl;
//			// Get centrality bin for event from ref_multn value
//			refmultCorrUtil->init(event.get_run());
//			refmultCorrUtil->initEvent((int)event.get_refn(), (double)event.get_vz());
//			int cent16_corr = refmultCorrUtil->getCentralityBin16();
//			int cent9_corr = refmultCorrUtil->getCentralityBin9();
//
//			// If there are enough good protons, calculate ratios for each division and save to data.
//			if(good_proton_angles.size() >= (unsigned)cut.min_multi) {
////				cout << "Enough protons" << endl;
//				cent16_events.Fill(cent16_corr);
//				cent9_events.Fill(cent9_corr);
//				event_cut_hist.Fill("Enough Good Protons", 1);
//
//				int cent;
//				if(cent_binning == 16) {
//					cent = cent16_corr;
//				} else {
//					cent = cent9_corr;
//				}
//
////				cout << "Rotate protons" << endl;
//				if(event_plane) { // If event_plane flag then rotate all angles by -event_plane.
//					good_proton_angles = rotate_angles(good_proton_angles, -event.get_event_plane());
//				} else if(rotate_random) { // If rotate_random flag then rotate all angles by random angle between 0 and 2pi
//					double rand_angle = trand->Rndm() * 2 * M_PI;
//					good_proton_angles = rotate_angles(good_proton_angles, rand_angle);
//					event.set_event_plane(rotate_angle(event.get_event_plane(), rand_angle));
//				}
//
////				cout << "Mix" << endl;
//				// If mixed/rand flagged append event to mix/rand object.
//				if(mixed) {
//					if(cbwc) {
//						mix.append_event_CBWC(good_proton_angles, event.get_refn(), event.get_event_plane(), event.get_vz());
//					} else {
//						mix.append_event(good_proton_angles, cent, event.get_event_plane(), event.get_vz());
//					}
//				}
//				if(mixed_sets) { mix_sets.append_event(good_proton_angles, event.get_refn()); }
//				if(rand_data) { random.append_event((int)good_proton_angles.size(), event.get_refn(), trand); }
//
////				cout << "Get/Store Ratios" << endl;
//				for(int div:divs) {
//					vector<int> event_ratios = get_Rs(good_proton_angles, div);  // Convert proton angles in event to ratio values.
//
//					// Save ratio values to data
//					if(single_ratio) { // Only save a single ratio per event at random.
//						if(cbwc) {
//							data[div][event.get_refn()][good_proton_angles.size()][event_ratios[((int)trand->Rndm()*event_ratios.size())]]++;
//						} else {
//							data[div][cent][good_proton_angles.size()][event_ratios[((int)trand->Rndm()*event_ratios.size())]]++;
//						}
//					} else { // Save all ratios from event.
//						for(int protons_in_bin:event_ratios) {
//							if(cbwc) { // If centrality bin width correction flagged, save refmult2 value in place of centrality bin
//								data[div][event.get_refn()][good_proton_angles.size()][protons_in_bin]++;
//							} else {
//								data[div][cent][good_proton_angles.size()][protons_in_bin]++;
//							}
//						}
//					}
//
//				}
//			}
//		}
		event_index++;
	}
//	delete event_pointer;
}
