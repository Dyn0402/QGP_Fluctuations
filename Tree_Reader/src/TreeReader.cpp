/*
 * TreeReader.cpp
 *
 *  Created on: Jul 18, 2019
 *      Author: dylan
 */


#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
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
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed_sets = false;
	mixed = false;
	rand_data = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	this->ref_num = ref_num;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());

	this->energy = energy;

	mix = Mixer(energy, single_ratio, rotate_random);
//	sim = Simulator();

	if(energy == 27) { cut.min_nsigma = -1.0; cut.max_nsigma = 1.0; }
}

TreeReader::TreeReader(int energy) {
	start_sys = chrono::system_clock::now();
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed_sets = false;
	mixed = false;
	rand_data = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	ref_num = 2;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());

	this->energy = energy;

	mix = Mixer(energy, single_ratio, rotate_random);
//	sim = Simulator();

	if(energy == 27) { cut.min_nsigma = -1.0; cut.max_nsigma = 1.0; }
}

TreeReader::TreeReader() {
	start_sys = chrono::system_clock::now();
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed_sets = false;
	mixed = false;
	rand_data = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	ref_num = 2;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());

	this->energy = 7;

	mix = Mixer(energy, single_ratio, rotate_random);
//	sim = Simulator();

	if(energy == 27) { cut.min_nsigma = -1.0; cut.max_nsigma = 1.0; }
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

string TreeReader::get_qa_path() {
	return(qa_path);
}

string TreeReader::get_set_name() {
	return(set_name);
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

bool TreeReader::get_cbwc() {
	return(cbwc);
}

bool TreeReader::get_rotate_random() {
	return(rotate_random);
}

bool TreeReader::get_event_plane() {
	return(event_plane);
}

bool TreeReader::get_mixed_sets() {
	return(mixed_sets);
}

bool TreeReader::get_mixed() {
	return(mixed);
}

bool TreeReader::get_rand_data() {
	return(rand_data);
}

bool TreeReader::get_pile_up() {
	return(pile_up);
}

bool TreeReader::get_efficiency() {
	return(efficiency);
}

bool TreeReader::get_single_ratio() {
	return(single_ratio);
}

double TreeReader::get_pile_up_prob() {
	return(pile_up_prob);
}

double TreeReader::get_efficiency_prob() {
	return(efficiency_prob);
}

int TreeReader::get_cent_binning() {
	return(cent_binning);
}

int TreeReader::get_ref_num() {
	return(ref_num);
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

void TreeReader::set_qa_name(string name) {
	qa_name = name;
}

void TreeReader::set_set_name(string set_name) {
	this->set_name = set_name;
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

void TreeReader::set_divs(vector<int> list) {
	divs = list;
}

void TreeReader::set_cbwc(bool cbwc) {
	this->cbwc = cbwc;
}

void TreeReader::set_rotate_random(bool rotate_random) {
	this->rotate_random = rotate_random;
	mix.set_rand_rotate(rotate_random);
}

void TreeReader::set_event_plane(bool event_plane) {
	this->event_plane = event_plane;
}

void TreeReader::set_mixed_sets(bool mixed) {
	this->mixed_sets = mixed;
}

void TreeReader::set_mixed(bool mixed_roli) {
	this->mixed = mixed_roli;
}

void TreeReader::set_rand_data(bool rand_data) {
	this->rand_data = rand_data;
}

void TreeReader::set_pile_up(bool pile_up) {
	this->pile_up = pile_up;
}

void TreeReader::set_efficiency(bool efficiency) {
	this->efficiency = efficiency;
}

void TreeReader::set_single_ratio(bool single_ratio) {
	this->single_ratio = single_ratio;
	mix.set_single_ratio(single_ratio);
}

void TreeReader::set_n1_ratios(bool n1_ratios) {
	this->n1_ratios = n1_ratios;
	mix.set_n1_ratios(n1_ratios);
}

void TreeReader::set_check_charge(bool check) {
	this->check_charge = check;
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

void TreeReader::set_efficiency_prob(double efficiency_prob) {
	this->efficiency_prob = efficiency_prob;
}

void TreeReader::set_cent_binning(int cent_binning) {
	this->cent_binning = cent_binning;
}

void TreeReader::set_ref_num(int ref_num) {
	this->ref_num = ref_num;
	if(refmultCorrUtil) { delete refmultCorrUtil; }
	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());
}

void TreeReader::set_particle(string particle) {
	this->particle = particle;
}

void TreeReader::set_particle_dist_hist_max(int max) {
	particle_dist_hist_max = max;
}

void TreeReader::set_ampt_particle_pid(vector<int> pid) {
	ampt_particle_pid = pid;
}

// Doers

// Read files for single energy and write results to text files.
void TreeReader::read_trees() {
	define_qa();

	cout << "Reading " + set_name + " " + to_string(energy) + "GeV trees." << endl << endl;
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV/", "root", "path");

	unsigned num_files = in_files.size();
	unsigned file_index = 1;

	for(string path:in_files) {

		// Display progress and time while running.
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
			cout << " " << set_name << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
		}

		TFile *file = new TFile(path.data(), "READ");
		add_cut_hists(file);
		TTree *tree = (TTree*)file->Get(tree_name.data());
		read_tree(tree);  // Read tree from file into data
		file->Close();
		delete file;
		file_index++;
	}

	reset_out_dir();
	write_info_file();
	write_qa();
	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << "Writing " + set_name + " " + to_string(energy) + "GeV trees. 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
	write_tree_data("local", data, out_path+to_string(energy)+"GeV/");
	if(mixed) { mix.write_mixed_data(); }
	if(mixed_sets) { mix_sets.write_mixed_data(); }
	if(rand_data) {random.write_random_data(); }
}


// Read files for single energy and write results to text files.
void TreeReader::read_trees_chain() {
	define_qa();

	cout << "Reading " + set_name + " " + to_string(energy) + "GeV trees." << endl << endl;
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV/", "root", "path");
	TChain *chain = new TChain(tree_name.data());

	for(string path:in_files) {
		TFile *file = new TFile(path.data(), "READ");
		add_cut_hists(file);
		file->Close();
		delete file;
		chain->AddFile(path.data());
	}

	set_branches(chain);

	int num_events = chain->GetEntries();
	for(int event_index = 0; event_index < num_events; event_index++) {

		tree_leaves leaves = get_tree_leaves(chain, particle, ref_num);

		// Display progress and time while running.
		if(!(event_index % (unsigned)(num_events/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
			cout << " " << set_name << " " << energy << "GeV " << (int)(100.0*event_index/num_events+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
		}

		chain->GetEntry(event_index);
		Event event(leaves);

		if(pile_up) {
			if(trand->Rndm() < pile_up_prob) {  // Pile up next two events
				event_index++;
				if(chain->GetEntry(event_index)) {
					Event event2(leaves);
					event.pile_up(event2);
				}
			}
		}

		process_event(event);
	}

	reset_out_dir();
	write_info_file();
	write_qa();
	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << "Writing " + set_name + " " + to_string(energy) + "GeV trees. 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
	write_tree_data("local", data, out_path+to_string(energy)+"GeV/");
	if(mixed) { mix.write_mixed_data(); }
	if(mixed_sets) { mix_sets.write_mixed_data(); }
	if(rand_data) {random.write_random_data(); }
}


// Read files for single energy and write results to text files.
void TreeReader::read_ampt_trees() {
	define_qa();

	ampt_cent = AmptCentralityMaker("/media/dylan/SSD_Storage/Research/Trees_Ampt/" + to_string(energy) + "GeV_Cent/", "ref" + to_string(ref_num));
	map<int, float> opt_bmax {{7, 14.75}, {11, 14.5312}, {19, 14.3438}, {27, 14.2969}, {39, 13.5312}, {62, 13.6094}};
	ampt_cent.set_max_b(opt_bmax[energy]);
	ampt_cent.set_mult_quantity("ref3");

	cout << "Reading " + set_name + " " + to_string(energy) + "GeV trees." << endl << endl;
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV_All/", "root", "path");
	cout << endl;

	unsigned num_files = in_files.size();
	unsigned file_index = 1;

	for(string path:in_files) {
		// Display progress and time while running.
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
			cout << " " << set_name << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
		}

		TFile *file = new TFile(path.data(), "READ");
//		add_cut_hists(file);
		TTree *tree = (TTree*)file->Get(tree_name.data());
		read_ampt_tree(tree);  // Read tree from file into data
		file->Close();
		delete file;
		file_index++;
	}

	reset_out_dir();
	write_info_file();
	write_qa();
	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << "Writing " + set_name + " " + to_string(energy) + "GeV trees. 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
	write_tree_data("local", data, out_path+to_string(energy)+"GeV/");
	if(mixed) { mix.write_mixed_data(); }
	if(mixed_sets) { mix_sets.write_mixed_data(); }
	if(rand_data) {random.write_random_data(); }
}


// Read files for single energy and write results to text files.
void TreeReader::read_nsm_ampt_trees() {
	define_qa();

	cout << "Reading " + set_name + " " + to_string(energy) + "GeV trees." << endl << endl;
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV/", "root", "path");

	unsigned num_files = in_files.size();
	unsigned file_index = 1;

	for(string path:in_files) {

		// Display progress and time while running.
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
			cout << " " << set_name << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
		}

		TFile *file = new TFile(path.data(), "READ");
//		add_cut_hists(file);
		TTree *tree = (TTree*)file->Get(tree_name.data());
		read_nsm_ampt_tree(tree);  // Read tree from file into data
		file->Close();
		delete file;
		file_index++;
	}

	reset_out_dir();
	write_info_file();
	write_qa();
	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << "Writing " + set_name + " " + to_string(energy) + "GeV trees. 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
	write_tree_data("local", data, out_path+to_string(energy)+"GeV/");
	if(mixed) { mix.write_mixed_data(); }
	if(mixed_sets) { mix_sets.write_mixed_data(); }
	if(rand_data) {random.write_random_data(); }
}


void TreeReader::sim_events(map<int, int> cent_num_events) {
	define_qa();

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
		if(sim.get_proton_dist_type() == "hist") { sim.set_proton_dist_hist(get_sim_proton_dist(cent.first)); }
		for(int i=0; i<cent.second; i++) {
			if(!(i % (int)(cent.second/10.0+0.5))) {
				chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
				auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
				vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
				cout << " " << set_name << " " << energy << "GeV Centrality " << cent.first << " " << (int)(100.0*i/cent.second+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
			}
			Event event(event_defs, energy, ref_num, cent.first);
			sim.simulate_event(event);

			process_event(event);
		}
	}

	reset_out_dir();
	write_info_file();
	write_qa();
	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << "Writing " + set_name + " " + to_string(energy) + "GeV " << total_events << " simulated events. 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
	write_tree_data("local", data, out_path+to_string(energy)+"GeV/");
	if(mixed) { mix.write_mixed_data(); }
	if(mixed_sets) { mix_sets.write_mixed_data(); }
	if(rand_data) {random.write_random_data(); }
}


// Read individual tree. Read out and then process each event.
void TreeReader::read_tree(TTree* tree) {
	tree_leaves leaves = get_tree_leaves(tree, particle, ref_num);
	set_branches(tree);

	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		Event event(leaves);

		if(pile_up) {
			if(trand->Rndm() < pile_up_prob) {  // Pile up next two events
				event_index++;
				if(tree->GetEntry(event_index)) {
					Event event2(leaves);
					event.pile_up(event2);
				}
			}
		}

		process_event(event);
		event_index++;
	}
}


void TreeReader::set_branches(TTree* tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("run", 1);
	tree->SetBranchStatus("event_id", 1);
	tree->SetBranchStatus("Nprim", 1);
	tree->SetBranchStatus(("ref"+to_string(ref_num)).data(), 1);
	tree->SetBranchStatus("btof", 1);
	tree->SetBranchStatus("vtx_z", 1);
	tree->SetBranchStatus("qx", 1);
	tree->SetBranchStatus("qy", 1);
	tree->SetBranchStatus("dca_xy_avg", 1);
	tree->SetBranchStatus("dca_xy_sd", 1);
	tree->SetBranchStatus((particle+"*").data(), 1);
}

void TreeReader::set_branches(TChain* chain) {
	chain->SetBranchStatus("*", 0);
	chain->SetBranchStatus("run", 1);
	chain->SetBranchStatus("Nprim", 1);
	chain->SetBranchStatus(("ref"+to_string(ref_num)).data(), 1);
	chain->SetBranchStatus("btof", 1);
	chain->SetBranchStatus("vtx_z", 1);
	chain->SetBranchStatus(("event_plane_ref"+to_string(ref_num)).data(), 1);
	chain->SetBranchStatus((particle+"*").data(), 1);
}


void TreeReader::read_ampt_tree(TTree* tree) {
	ampt_tree_leaves leaves = get_ampt_tree_leaves(tree, ref_num);

	int cent_9bin, particle_mult;
	int event_index = 0;
	while(tree->GetEvent(event_index)) {
		cent_9bin = ampt_cent.get_cent_bin9(leaves.refn->GetValue());
		Event event(event_defs, energy, ref_num, cent_9bin);
//		event.set_event_plane(leaves.event_plane->GetValue()); fix
		particle_mult = leaves.pmult->GetValue();

		vector<Track> particles;
		for(int i=0; i<particle_mult; i++) {
			if(!count(ampt_particle_pid.begin(), ampt_particle_pid.end(), leaves.pid->GetValue(i))) { continue; }
			TVector3 p(leaves.px->GetValue(i), leaves.py->GetValue(i), leaves.pz->GetValue(i));
			Track particle(track_defs);
			particle.set_p(p.Mag());
			particle.set_pt(p.Perp());
			particle.set_phi(p.Phi() + M_PI);
			particle.set_eta(p.PseudoRapidity());
			particle.set_charge(leaves.pid->GetValue(i) / fabs(leaves.pid->GetValue(i)));
			particles.push_back(particle);
		}
		event.set_particles(particles);

		if(pile_up) {
			if(trand->Rndm() < pile_up_prob) {  // Pile up next two events
				event_index++;
				if(tree->GetEntry(event_index)) {
					cent_9bin = ampt_cent.get_cent_bin9(leaves.refn->GetValue());
					Event event2(event_defs, energy, ref_num, cent_9bin);
//					event2.set_event_plane(leaves.event_plane->GetValue()); fix
					particle_mult = leaves.pmult->GetValue();

					particles.clear();
					for(int i=0; i<particle_mult; i++) {
						TVector3 p(leaves.px->GetValue(i), leaves.py->GetValue(i), leaves.pz->GetValue(i));
						Track particle(track_defs);
						particle.set_p(p.Mag());
						particle.set_pt(p.Perp());
						particle.set_phi(p.Phi() + M_PI);
						particle.set_eta(p.PseudoRapidity());
						particle.set_charge(leaves.pid->GetValue(i) / fabs(leaves.pid->GetValue(i)));
						particles.push_back(particle);
					}
					event2.set_particles(particles);

					event.pile_up(event2);
				}
			}
		}

		process_event(event);
		event_index++;
	}
}


void TreeReader::read_nsm_ampt_tree(TTree* tree) {
	Event *event_pointer = new Event;
	TBranch *event_branch = tree->GetBranch("event");
	event_branch->SetAddress(&event_pointer);

	int event_index = 0;
	while(tree->GetEvent(event_index)) {
		Event event = *event_pointer;

		if(pile_up) {
			if(trand->Rndm() < pile_up_prob) {  // Pile up next two events
				event_index++;
				if(tree->GetEntry(event_index)) {
					Event event2 = *event_pointer;
					event.pile_up(event2);
				}
			}
		}

		process_event(event);
		event_index++;
	}
}

// Get proton distribution for simulation from real data QA plots.
TH1D* TreeReader::get_sim_proton_dist(int cent) {
	string file_path = sim_proton_dist_dataset + to_string(energy) + "GeV/" + qa_name + to_string(energy) + "GeV.root";
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

	return(proton_dist);
}


// Get efficiency distribution for simulation from real data QA plots.
TH1D* TreeReader::get_sim_efficiency_dist() {
	string file_path = sim_proton_dist_dataset + to_string(energy) + "GeV/" + qa_name + to_string(energy) + "GeV.root";
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

	return(efficiency_dist);
}


//  For good events/tracks, azimuthally bin protons and save to data
void TreeReader::process_event(Event& event) {

	// Check if each event is good. Analyze if so, continue if not.
	if(check_event(event)) {
		vector<double> good_particle_angles = {};

		// Iterate over protons in event and add corresponding phi to good_proton_angles if proton good.
		for(Track& particle:event.get_particles()) {
			if(check_particle_good(particle)) {
				if(efficiency) {  // Skip good particle with chance efficiency_prob
					if(trand->Rndm() < efficiency_prob) { continue; }
				}
				good_particle_angles.push_back(particle.get_phi());
			}
		}

		// Get centrality bin for event from ref_multn value
		refmultCorrUtil->init(event.get_run());
		refmultCorrUtil->initEvent((int)event.get_refn(), (double)event.get_vz());
		int cent16_corr = refmultCorrUtil->getCentralityBin16();
		int cent9_corr = refmultCorrUtil->getCentralityBin9();

		// If there are enough good particles, calculate ratios for each division and save to data.
		if(good_particle_angles.size() >= (unsigned)cut.min_multi) {
			cent16_events.Fill(cent16_corr);
			cent9_events.Fill(cent9_corr);
			event_cut_hist.Fill("Enough Good Protons", 1);

			int cent;
			if(cent_binning == 16) {
				cent = cent16_corr;
			} else {
				cent = cent9_corr;
			}
			post_n_particles[cent].Fill((int)good_particle_angles.size());

			TVector2 q(event.get_qx(), event.get_qy());
			for(double &angle:good_particle_angles) {  // Subtract contribution of particle of interest to mitigate autocorrelation.
				TVector2 q_new(cos(2*angle), sin(2*angle));
				q -= q_new;
			}
			event.set_event_plane(0.5 * q.Phi());

			// If mixed/rand flagged append event to mix/rand object.
			if(mixed) { mix.append_event(good_particle_angles, cent, event.get_event_plane(), event.get_vz()); }
			if(mixed_sets) { mix_sets.append_event(good_particle_angles, event.get_refn()); }
			if(rand_data) { random.append_event((int)good_particle_angles.size(), event.get_refn(), trand); }


			if(event_plane) { // If event_plane flag then rotate all angles by -event_plane.
				good_particle_angles = rotate_angles(good_particle_angles, -event.get_event_plane());
			} else if(rotate_random) { // If rotate_random flag then rotate all angles by random angle between 0 and 2pi
				double rand_angle = trand->Rndm() * 2 * M_PI;
				good_particle_angles = rotate_angles(good_particle_angles, rand_angle);
//				q.Rotate(rand_angle);  // Untested
//				event.set_event_plane(rotate_angle(event_plane, rand_angle));  // Need to rotate event plane if it's used after this point. Currently is not.
			}


			for(auto &div:divs) {
				int bin_num = (int) 360 / div;
				if(single_ratio) { bin_num = 1; }
				else if(n1_ratios) { bin_num -= 1; }  // Ambiguous if case should change if div divides 360 or not.
				vector<int> event_ratios = get_Rs(good_particle_angles, div, trand, bin_num);  // Convert particle angles in event to ratio values.

				// Save ratio values to data
				for(int protons_in_bin:event_ratios) {
					if(cbwc) { // If centrality bin width correction flagged, save refmult2 value in place of centrality bin
						data[div][event.get_refn()][good_particle_angles.size()][protons_in_bin]++;
					} else {
						data[div][cent][good_particle_angles.size()][protons_in_bin]++;
					}
				}
			}
		}
	}
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


// Write file to outpath containing information about TreeReader variables used.
void TreeReader::write_info_file() {
	ofstream out(out_path+to_string(energy)+"GeV/"+info_file_name);
	if(!out) { cerr << "Couldn't open info_file path" << endl; }
	else {
		out << "Information file for TreeReader run." << endl;
		out << "in_path: " << in_path << endl;
		out << "out_path: " << out_path << endl;
		out << "qa_path: " << qa_path << endl;
		out << "qa_name: " << qa_name << endl;

		out << "energy: " << to_string(energy) << endl;
		out << "divs: { ";
		for(auto &div:divs) { out << to_string(div) << " "; }
		out << "}" << endl;

		out << "particle: " << particle << endl;

		out << "cbwc: " << boolalpha << cbwc << endl;
		out << "rotate_random: " << boolalpha << rotate_random << endl;
		out << "event_plane: " << boolalpha << event_plane << endl;
		out << "mixed_sets: " << boolalpha << mixed_sets << endl;
		out << "mixed: " << boolalpha << mixed << endl;
		out << "rand_data: " << boolalpha << rand_data << endl;
		out << "pile_up: " << boolalpha << pile_up << endl;
		out << "efficiency: " << boolalpha << efficiency << endl;
		out << "single_ratio: " << boolalpha << single_ratio << endl;
		out << "n1_ratios: " << boolalpha << n1_ratios << endl;
		out << "check_charge: " << boolalpha << check_charge << endl;
		out << "pile_up_prob: " << pile_up_prob << endl;
		out << "efficiency_prob: " << efficiency_prob << endl;
		out << "cent_binning: " << cent_binning << endl;
		out << "ref_num: " << ref_num << endl;

		out << "min_p: " << to_string(cut.min_p) << endl;
		out << "max_p: " << to_string(cut.max_p) << endl;
		out << "min_pt: " << to_string(cut.min_pt) << endl;
		out << "max_pt: " << to_string(cut.max_pt) << endl;
		out << "min_beta: " << to_string(cut.min_beta) << endl;
		out << "max_beta: " << to_string(cut.max_beta) << endl;
		out << "charge: " << to_string(cut.charge) << endl;
		out << "min_eta: " << to_string(cut.min_eta) << endl;
		out << "max_eta: " << to_string(cut.max_eta) << endl;
		out << "min_nsigma: " << to_string(cut.min_nsigma) << endl;
		out << "max_nsigma: " << to_string(cut.max_nsigma) << endl;
		out << "min_dca: " << to_string(cut.min_dca) << endl;
		out << "max_dca: " << to_string(cut.max_dca) << endl;
		out << "min_m2: " << to_string(cut.min_m2) << endl;
		out << "max_m2: " << to_string(cut.max_m2) << endl;
		out << "min_pt_for_m: " << to_string(cut.min_pt_for_m) << endl;
		out << "max_pt_for_m: " << to_string(cut.max_pt_for_m) << endl;

		out << "min_multi: " << to_string(cut.min_multi) << endl;

		out << "sim p_group: " << to_string(sim.get_p_group()) << endl;
		out << "sim spread_sigma: " << to_string(sim.get_spread_sigma()) << endl;

		out << "Ampt Particle PIDs: ";
		for(auto &pid:ampt_particle_pid) { out << to_string(pid) << " "; }
		out << endl;
	}
}


// Add file's event/track cut hists to corresponding collected histogram for energy.
void TreeReader::add_cut_hists(TFile *file) {
	TH1D *event_hist = (TH1D*)file->Get(event_cut_hist_name.data());
	TH1D *track_hist = (TH1D*)file->Get(track_cut_hist_name.data());
	event_cut_tree_maker.Add(event_hist);
	track_cut_tree_maker.Add(track_hist);
}


//Returns true if event is good, false if it is bad.
bool TreeReader::check_event(Event& event) {
	bool good_event = false;
	fill_pre_event_qa(event);
	event_cut_hist.Fill("Original", 1);
	if(check_good_run((int)event.get_run())) {
		event_cut_hist.Fill("Good Run", 1);
		if(check_enough_particles(event)) {
			event_cut_hist.Fill("Enough Protons", 1);
			if(check_slope(event.get_btof(), event.get_ref())) {
				event_cut_hist.Fill("Pile Up Rejected", 1);
				fill_post_event_qa(event);
				good_event = true;
			}
		}
	}

	return(good_event);
}


//Check the list of bad runs for input run.
//If input run is contained in bad run list, return false (bad run) else return true (good run).
bool TreeReader::check_good_run(int run) {
	bool list_good_run = find(cut.bad_runs.begin(), cut.bad_runs.end(), run) == cut.bad_runs.end();
	bool ref_good_run = !refmultCorrUtil->isBadRun(run);

	return(list_good_run && ref_good_run);
}


//Checks if there are enough protons in the event.
//If more protons than minimum, return true, else false.
bool TreeReader::check_enough_particles(Event& event) {
	if(event.get_num_particles() >=  cut.min_multi) { return(true);	}
	else { return(false); }
}


// Check slope of event. If within cuts, return true for good event, else false.
bool TreeReader::check_slope(int btof, int ref_mult) {
	bool good_event = true;
	double slope = (double)btof / ref_mult;
	if(slope > cut.max_slope[energy] || slope < cut.min_slope[energy]) {
		good_event = false;
	}
	btof_ref_hist.Fill(btof, ref_mult);

	return(good_event);
}


// Returns true if proton is good and false if proton is bad.
bool TreeReader::check_particle_good(Track& proton) {
	bool good_proton = false;
	track_cut_hist.Fill("Original", 1);

	fill_pre_track_qa(proton);

	double p = proton.get_p();
	if(p < cut.min_p) { return(good_proton); }
	track_cut_hist.Fill("Good p", 1);

	double pt = proton.get_pt();
	if(!(pt >= cut.min_pt && pt <= cut.max_pt)) { return(good_proton); }
	track_cut_hist.Fill("Good pt", 1);

	if(check_charge && !(proton.get_charge() == cut.charge)) { return(good_proton); }
	track_cut_hist.Fill("Good charge", 1);

	double eta = proton.get_eta();
	if(!(eta >= cut.min_eta && eta <= cut.max_eta)) { return(good_proton); }
	track_cut_hist.Fill("Good eta", 1);

	double nsigma = proton.get_nsigma();
	if(!(nsigma >= cut.min_nsigma && nsigma <= cut.max_nsigma)) { return(good_proton); }
	track_cut_hist.Fill("Good nsigma", 1);

	double dca = proton.get_dca();
	if(!(dca >= cut.min_dca && dca <= cut.max_dca)) { return(good_proton); }
	track_cut_hist.Fill("Good dca", 1);

	if(pt >= cut.min_pt_for_m && pt <= cut.max_pt_for_m) {
		double beta = proton.get_beta();
		if(beta > cut.min_beta) {
			double m2 = pow(p, 2) * (pow(beta, -2) - 1.);
			pre_m2_hist.Fill(m2);
			if(m2 > cut.min_m2 && m2 < cut.max_m2) {
				good_proton = true;
				post_m2_hist.Fill(m2);
			}
		} else {
			good_proton = true;
		}
	} else {
		good_proton = true;
	}
	if(good_proton) { track_cut_hist.Fill("Good mass*", 1); }

	fill_post_track_qa(proton);

	return(good_proton);
}


// Define all histograms collected for qa.
void TreeReader::define_qa() {
	cent_hist = TH2I(("cent_comp"+set_name+"_"+to_string(energy)).data(), "Centrality Comparison", 19, -2.5, 16.5, 19, -2.5, 16.5);
	btof_ref_hist = TH2I(("btof_ref"+set_name+"_"+to_string(energy)).data(), "BTof vs Ref", 3001, -0.5, 3000.5, 601, -0.5, 600.5);

	event_cut_tree_maker = TH1D(("event_cut_tree_maker"+set_name+"_"+to_string(energy)).data(), "Event Cuts", 8, -0.5, 7.5);
	event_cut_tree_maker.GetXaxis()->SetBinLabel(1, "Original");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(2, "Is muEvent");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(3, "Good Trigger");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(4, "Good Run");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(5, "Good Vz");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(6, "Good Vr");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(7, "Vertex Non-Zero");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(8, "Good VPD Vz");

	track_cut_tree_maker = TH1D(("track_cut_tree_maker"+set_name+"_"+to_string(energy)).data(), "Track Cuts", 13, -0.5, 12.5);
	track_cut_tree_maker.GetXaxis()->SetBinLabel(1, "Original");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(2, "Charge");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(3, "p_low");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(4, "ratio_low");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(5, "ratio_high");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(6, "eta");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(7, "nHitsFit");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(8, "nHitsDedx");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(9, "dca");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(10, "pt_low");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(11, "pt_high");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(12, "nsigma");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(13, "m");
	event_cut_hist = TH1D(("event_cut"+set_name+"_"+to_string(energy)).data(), "Event Cuts", 5, -0.5, 4.5);
	event_cut_hist.GetXaxis()->SetBinLabel(1, "Original");
	event_cut_hist.GetXaxis()->SetBinLabel(2, "Good Run");
	event_cut_hist.GetXaxis()->SetBinLabel(3, "Enough Protons");
	event_cut_hist.GetXaxis()->SetBinLabel(4, "Pile Up Rejected");
	event_cut_hist.GetXaxis()->SetBinLabel(5, "Enough Good Protons");
	track_cut_hist = TH1D(("track_cut"+set_name+"_"+to_string(energy)).data(), "Track Cuts", 8, -0.5, 7.5);
	track_cut_hist.GetXaxis()->SetBinLabel(1, "Original");
	track_cut_hist.GetXaxis()->SetBinLabel(2, "Good p");
	track_cut_hist.GetXaxis()->SetBinLabel(3, "Good pt");
	track_cut_hist.GetXaxis()->SetBinLabel(4, "Good charge");
	track_cut_hist.GetXaxis()->SetBinLabel(5, "Good eta");
	track_cut_hist.GetXaxis()->SetBinLabel(6, "Good nsigma");
	track_cut_hist.GetXaxis()->SetBinLabel(7, "Good dca");
	track_cut_hist.GetXaxis()->SetBinLabel(8, "Good mass*");


	cent16_events = TH1I(("cent16_events_"+set_name+"_"+to_string(energy)).data(), "Cent16 Events", 18, -1.5, 16.5);
	cent9_events = TH1I(("cent9_events_"+set_name+"_"+to_string(energy)).data(), "Cent9 Events", 11, -1.5, 9.5);

	pre_run_hist = TH1I(("pre_run_"+set_name+"_"+to_string(energy)).data(), "pre_run", 1000, 1000000, 100000000);
	pre_vx_hist = TH1I(("pre_vx_"+set_name+"_"+to_string(energy)).data(), "pre_vx", 100, -2.5, 2.5);
	pre_vy_hist = TH1I(("pre_vy_"+set_name+"_"+to_string(energy)).data(), "pre_vy", 100, -2.5, 2.5);
	pre_vz_hist = TH1I(("pre_vz_"+set_name+"_"+to_string(energy)).data(), "pre_vz", 100, -55, 55);
	pre_ref_hist = TH1I(("pre_ref_"+set_name+"_"+to_string(energy)).data(), "pre_ref", 801, -0.5, 800.5);
	pre_refn_hist = TH1I(("pre_refn_"+set_name+"_"+to_string(energy)).data(), "pre_refn", 801, -0.5, 800.5);
	pre_btof_hist = TH1I(("pre_btof_"+set_name+"_"+to_string(energy)).data(), "pre_btof", 2001, -0.5, 2000.5);
//	pre_ep_hist = TH1I(("pre_ep_"+set_name+"_"+to_string(energy)).data(), "pre_ep", 100, -0.5, 3.5);

	post_run_hist = TH1I(("post_run_"+set_name+"_"+to_string(energy)).data(), "post_run", 1000, 1000000, 100000000);
	post_vx_hist = TH1I(("post_vx_"+set_name+"_"+to_string(energy)).data(), "post_vx", 100, -2.5, 2.5);
	post_vy_hist = TH1I(("post_vy_"+set_name+"_"+to_string(energy)).data(), "post_vy", 100, -2.5, 2.5);
	post_vz_hist = TH1I(("post_vz_"+set_name+"_"+to_string(energy)).data(), "post_vz", 100, -55, 55);
	post_ref_hist = TH1I(("post_ref_"+set_name+"_"+to_string(energy)).data(), "post_ref", 801, -0.5, 800.5);
	post_refn_hist = TH1I(("post_refn_"+set_name+"_"+to_string(energy)).data(), "post_refn", 801, -0.5, 800.5);
	post_btof_hist = TH1I(("post_btof_"+set_name+"_"+to_string(energy)).data(), "post_btof", 2001, -0.5, 2000.5);
	post_ep_hist = TH1I(("post_ep_"+set_name+"_"+to_string(energy)).data(), "post_ep", 100, -0.5, 3.5);

	pre_phi_hist = TH1D(("pre_phi_"+set_name+"_"+to_string(energy)).data(), "pre_phi", 1000, 0.0, 2*TMath::Pi());
	pre_p_hist = TH1I(("pre_p_"+set_name+"_"+to_string(energy)).data(), "pre_p", 100, 0.0, 3.5);
	pre_pt_hist = TH1I(("pre_pt_"+set_name+"_"+to_string(energy)).data(), "pre_pt", 100, 0.0, 3.0);
	pre_beta_hist = TH1I(("pre_beta_"+set_name+"_"+to_string(energy)).data(), "pre_beta", 100, -0.5, 2.0);
	pre_charge_hist = TH1I(("pre_charge_"+set_name+"_"+to_string(energy)).data(), "pre_charge", 100, -2.5, 2.5);
	pre_eta_hist = TH1I(("pre_eta_"+set_name+"_"+to_string(energy)).data(), "pre_eta", 100, -1.0, 1.0);
	pre_nsigma_hist = TH1I(("pre_nsigma_"+set_name+"_"+to_string(energy)).data(), "pre_nsigma", 100, -2.5, 2.5);
	pre_dca_hist = TH1I(("pre_dca_"+set_name+"_"+to_string(energy)).data(), "pre_dca", 100, 0.0, 2.5);

	post_phi_hist = TH1D(("post_phi_"+set_name+"_"+to_string(energy)).data(), "post_phi", 1000, 0.0, 2*TMath::Pi());
	post_p_hist = TH1I(("post_p_"+set_name+"_"+to_string(energy)).data(), "post_p", 100, 0.0, 3.5);
	post_pt_hist = TH1I(("post_pt_"+set_name+"_"+to_string(energy)).data(), "post_pt", 100, 0.0, 3.0);
	post_beta_hist = TH1I(("post_beta_"+set_name+"_"+to_string(energy)).data(), "post_beta", 100, -0.5, 2.0);
	post_charge_hist = TH1I(("post_charge_"+set_name+"_"+to_string(energy)).data(), "post_charge", 100, -2.5, 2.5);
	post_eta_hist = TH1I(("post_eta_"+set_name+"_"+to_string(energy)).data(), "post_eta", 100, -1.0, 1.0);
	post_nsigma_hist = TH1I(("post_nsigma_"+set_name+"_"+to_string(energy)).data(), "post_nsigma", 100, -2.5, 2.5);
	post_dca_hist = TH1I(("post_dca_"+set_name+"_"+to_string(energy)).data(), "post_dca", 100, 0.0, 2.5);

	pre_m2_hist = TH1I(("pre_m2_"+set_name+"_"+to_string(energy)).data(), "pre_m^2", 100, -0.5, 2.0);
	post_m2_hist = TH1I(("post_m2_"+set_name+"_"+to_string(energy)).data(), "post_m^2", 100, -0.5, 2.0);

	for(int i=-1; i<cent_binning; i++) {
		post_n_particles[i] = TH1D(("Particle_Dist_"+set_name+"_"+to_string(energy)+"_"+to_string(i)).data(), "Particle Distribution", particle_dist_hist_max+1, -0.5, particle_dist_hist_max+0.5);
	}

}


// Fill histograms for pre-qa for tracks
void TreeReader::fill_pre_track_qa(Track&proton) {
	pre_phi_hist.Fill(proton.get_phi());
	pre_p_hist.Fill(proton.get_p());
	pre_pt_hist.Fill(proton.get_pt());
	pre_beta_hist.Fill(proton.get_beta());
	pre_charge_hist.Fill(proton.get_charge());
	pre_eta_hist.Fill(proton.get_eta());
	pre_nsigma_hist.Fill(proton.get_nsigma());
	pre_dca_hist.Fill(proton.get_dca());
}


// Fill histograms for post-qa for tracks
void TreeReader::fill_post_track_qa(Track& proton) {
	post_phi_hist.Fill(proton.get_phi());
	post_p_hist.Fill(proton.get_p());
	post_pt_hist.Fill(proton.get_pt());
	post_beta_hist.Fill(proton.get_beta());
	post_charge_hist.Fill(proton.get_charge());
	post_eta_hist.Fill(proton.get_eta());
	post_nsigma_hist.Fill(proton.get_nsigma());
	post_dca_hist.Fill(proton.get_dca());
}


// Fill histograms for pre-qa for event
void TreeReader::fill_pre_event_qa(Event& event) {
	pre_run_hist.Fill(event.get_run());
	pre_vx_hist.Fill(event.get_vx());
	pre_vy_hist.Fill(event.get_vy());
	pre_vz_hist.Fill(event.get_vz());
	pre_ref_hist.Fill(event.get_ref());
	pre_refn_hist.Fill(event.get_refn());
	pre_btof_hist.Fill(event.get_btof());
//	pre_ep_hist.Fill(event.get_event_plane());
}

// Fill histograms for post-qa for event
void TreeReader::fill_post_event_qa(Event& event) {
	post_run_hist.Fill(event.get_run());
	post_vx_hist.Fill(event.get_vx());
	post_vy_hist.Fill(event.get_vy());
	post_vz_hist.Fill(event.get_vz());
	post_ref_hist.Fill(event.get_ref());
	post_refn_hist.Fill(event.get_refn());
	post_btof_hist.Fill(event.get_btof());
	post_ep_hist.Fill(event.get_event_plane());
}


// Write all qa plots into TFile.
void TreeReader::write_qa() {
	TFile qa((qa_path + qa_name + to_string(energy) + "GeV.root").data(), "RECREATE");

	TCanvas cent_can("cent_can");
	cent_hist.Draw("COLZ");
	cent_can.Write();
	cent_hist.Write();
	TCanvas pile_can("pile_can");
	btof_ref_hist.Draw("COLZ");
	pile_can.Write();

	btof_ref_hist.Write();
	event_cut_tree_maker.Write();
	track_cut_tree_maker.Write();
	event_cut_hist.Write();
	track_cut_hist.Write();
	cent16_events.Write();
	cent9_events.Write();

	pre_run_hist.Write();
	pre_vx_hist.Write();
	pre_vy_hist.Write();
	pre_vz_hist.Write();
	pre_ref_hist.Write();
	pre_refn_hist.Write();
	pre_btof_hist.Write();
//	pre_ep_hist.Write();

	post_run_hist.Write();
	post_vx_hist.Write();
	post_vy_hist.Write();
	post_vz_hist.Write();
	post_ref_hist.Write();
	post_refn_hist.Write();
	post_btof_hist.Write();
	post_ep_hist.Write();

	pre_phi_hist.Write();
	pre_p_hist.Write();
	pre_pt_hist.Write();
	pre_beta_hist.Write();
	pre_charge_hist.Write();
	pre_eta_hist.Write();
	pre_nsigma_hist.Write();
	pre_dca_hist.Write();
	post_phi_hist.Write();
	post_p_hist.Write();
	post_pt_hist.Write();
	post_beta_hist.Write();
	post_charge_hist.Write();
	post_eta_hist.Write();
	post_nsigma_hist.Write();
	post_dca_hist.Write();

	pre_m2_hist.Write();
	post_m2_hist.Write();

	for(auto hist:post_n_particles) {
		hist.second.Write();
	}


	// Make bar plots for event/track cut histograms
	TCanvas event_cut_maker_can("event_cut_maker_can");
	gPad->SetLeftMargin(0.15);
	event_cut_tree_maker.SetFillColor(45);
	event_cut_maker_can.SetFillColor(42);
	gPad->SetFrameFillColor(33);
	event_cut_tree_maker.Draw("hbar2");
	event_cut_maker_can.Write();
	TCanvas track_cut_maker_can("track_cut_maker_can");
	gPad->SetLeftMargin(0.15);
	track_cut_tree_maker.SetFillColor(45);
	track_cut_maker_can.SetFillColor(42);
	gPad->SetFrameFillColor(33);
	track_cut_tree_maker.Draw("hbar2");
	track_cut_maker_can.Write();
	TCanvas event_cut_hist_can("event_cut_hist_can");
	gPad->SetLeftMargin(0.15);
	event_cut_hist.SetFillColor(45);
	event_cut_hist_can.SetFillColor(42);
	gPad->SetFrameFillColor(33);
	event_cut_hist.Draw("hbar2");
	event_cut_hist_can.Write();
	TCanvas track_cut_hist_can("track_cut_hist_can");
	gPad->SetLeftMargin(0.15);
	track_cut_hist.SetFillColor(45);
	track_cut_hist_can.SetFillColor(42);
	gPad->SetFrameFillColor(33);
	track_cut_hist.Draw("hbar2");
	track_cut_hist_can.Write();

	// Make before/after canvases for each variable
	TCanvas run_can("run_can");
	pre_run_hist.GetYaxis()->SetRangeUser(0, pre_run_hist.GetMaximum()*1.05);
	pre_run_hist.Draw();
	post_run_hist.SetLineColor(kRed);
	post_run_hist.Draw("sames");
	run_can.Write();
	TCanvas vx_can("vx_can");
	pre_vx_hist.GetYaxis()->SetRangeUser(0, pre_vx_hist.GetMaximum()*1.05);
	pre_vx_hist.Draw();
	post_vx_hist.SetLineColor(kRed);
	post_vx_hist.Draw("sames");
	vx_can.Write();
	TCanvas vy_can("vy_can");
	pre_vy_hist.GetYaxis()->SetRangeUser(0, pre_vy_hist.GetMaximum()*1.05);
	pre_vy_hist.Draw();
	post_vy_hist.SetLineColor(kRed);
	post_vy_hist.Draw("sames");
	vy_can.Write();
	TCanvas vz_can("vz_can");
	pre_vz_hist.GetYaxis()->SetRangeUser(0, pre_vz_hist.GetMaximum()*1.05);
	pre_vz_hist.Draw();
	post_vz_hist.SetLineColor(kRed);
	post_vz_hist.Draw("sames");
	vz_can.Write();
	TCanvas ref_can("ref_can");
	pre_ref_hist.GetYaxis()->SetRangeUser(0, pre_ref_hist.GetMaximum()*1.05);
	pre_ref_hist.Draw();
	post_ref_hist.SetLineColor(kRed);
	post_ref_hist.Draw("sames");
	ref_can.Write();
	TCanvas refn_can("refn_can");
	pre_run_hist.GetYaxis()->SetRangeUser(0, pre_run_hist.GetMaximum()*1.05);
	pre_refn_hist.Draw();
	post_refn_hist.SetLineColor(kRed);
	post_refn_hist.Draw("sames");
	refn_can.Write();
	TCanvas btof_can("btof_can");
	pre_run_hist.GetYaxis()->SetRangeUser(0, pre_run_hist.GetMaximum()*1.05);
	pre_btof_hist.Draw();
	post_btof_hist.SetLineColor(kRed);
	post_btof_hist.Draw("sames");
	btof_can.Write();
	TCanvas ep_can("ep_can");
//	pre_run_hist.GetYaxis()->SetRangeUser(0, pre_run_hist.GetMaximum()*1.05);
//	pre_ep_hist.Draw();
	post_ep_hist.SetLineColor(kRed);
	post_ep_hist.Draw("sames");
	ep_can.Write();
	TCanvas phi_can("phi_can");
	pre_phi_hist.GetYaxis()->SetRangeUser(0, pre_phi_hist.GetMaximum()*1.05);
	pre_phi_hist.Draw();
	post_phi_hist.SetLineColor(kRed);
	post_phi_hist.Draw("sames");
	phi_can.Write();
	TCanvas p_can("p_can");
	pre_p_hist.GetYaxis()->SetRangeUser(0, pre_p_hist.GetMaximum()*1.05);
	pre_p_hist.Draw();
	post_p_hist.SetLineColor(kRed);
	post_p_hist.Draw("sames");
	p_can.Write();
	TCanvas pt_can("pt_can");
	pre_pt_hist.GetYaxis()->SetRangeUser(0, pre_pt_hist.GetMaximum()*1.05);
	pre_pt_hist.Draw();
	post_pt_hist.SetLineColor(kRed);
	post_pt_hist.Draw("sames");
	pt_can.Write();
	TCanvas beta_can("beta_can");
	pre_beta_hist.GetYaxis()->SetRangeUser(0, pre_beta_hist.GetMaximum()*1.05);
	pre_beta_hist.Draw();
	post_beta_hist.SetLineColor(kRed);
	post_beta_hist.Draw("sames");
	beta_can.Write();
	TCanvas charge_can("charge_can");
	pre_charge_hist.GetYaxis()->SetRangeUser(0, pre_charge_hist.GetMaximum()*1.05);
	pre_charge_hist.Draw();
	post_charge_hist.SetLineColor(kRed);
	post_charge_hist.Draw("sames");
	charge_can.Write();
	TCanvas eta_can("eta_can");
	pre_eta_hist.GetYaxis()->SetRangeUser(0, pre_eta_hist.GetMaximum()*1.05);
	pre_eta_hist.Draw();
	post_eta_hist.SetLineColor(kRed);
	post_eta_hist.Draw("sames");
	eta_can.Write();
	TCanvas nsigma_can("nsigma_can");
	pre_nsigma_hist.GetYaxis()->SetRangeUser(0, pre_nsigma_hist.GetMaximum()*1.05);
	pre_nsigma_hist.Draw();
	post_nsigma_hist.SetLineColor(kRed);
	post_nsigma_hist.Draw("sames");
	nsigma_can.Write();
	TCanvas dca_can("dca_can");
	pre_dca_hist.GetYaxis()->SetRangeUser(0, pre_dca_hist.GetMaximum()*1.05);
	pre_dca_hist.Draw();
	post_dca_hist.SetLineColor(kRed);
	post_dca_hist.Draw("sames");
	dca_can.Write();
	TCanvas m2_can("m2_can");
	pre_m2_hist.GetYaxis()->SetRangeUser(0, pre_m2_hist.GetMaximum()*1.05);
	pre_m2_hist.Draw();
	post_m2_hist.SetLineColor(kRed);
	post_m2_hist.Draw("sames");
	m2_can.Write();

	qa.Close();
}


// Remove out_path directory for energy if it exists and recreate it.
// Create out_path directory if it does not exist.
void TreeReader::reset_out_dir() {
	if(system(("test -d "+out_path).data())) {
		if(system(("mkdir " + out_path).data())) { cout << "Could not create output directory " + out_path << endl; }
	}

	string energy_path = out_path+to_string(energy)+"GeV/";
	if(!system(("test -d "+energy_path).data())) { int i = system(("rm -r " + energy_path).data()); if(i) {}}
	if(system(("mkdir " + energy_path).data())) { cout << "Could not create output directory " + energy_path << endl; }
}
