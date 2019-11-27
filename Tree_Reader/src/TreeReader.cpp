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

#include "../StRoot/StRefMultCorr/CentralityMaker.h"
#include "../StRoot/StRefMultCorr/StRefMultCorr.h"

#include "TreeReader.h"
#include "file_io.h"
#include "ratio_methods.h"

#include "Mixer.h"
#include "MixerSets.h"

using namespace std;


// Structors

TreeReader::TreeReader(int energy) {
	start_sys = chrono::system_clock::now();
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed_sets = false;
	mixed = false;
	rand_data = false;
	this->energy = energy;
	mix.set_energy(energy);

	cent_binning = 9;

	define_qa();

	if(energy == 27) { cut.min_nsigma = -1.0; cut.max_nsigma = 1.0; }
}

TreeReader::~TreeReader() {
}


// Getters

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

int TreeReader::get_cent_binning() {
	return(cent_binning);
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

void TreeReader::set_cent_binning(int cent_binning) {
	this->cent_binning = cent_binning;
}


// Doers

// Read files for single energy and write results to text files.
void TreeReader::read_trees() {
	cout << "Reading " + to_string(energy) + "GeV trees." << endl;
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV/", "root", "path");

	unsigned num_files = in_files.size();
	unsigned file_index = 1;

	for(string path:in_files) {

		// Display progress and time while running.
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			cout << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s" << endl;
		}

		TFile *file = new TFile(path.data(), "READ");
		TTree *tree = (TTree*)file->Get(tree_name.data());
		read_tree(tree);  // Read tree from file into data
		file->Close();
		delete file;
		file_index++;
	}

	reset_out_dir();
	write_info_file();
	write_qa();
	cout << " Writing " + to_string(energy) + "GeV trees." << endl;
	write_tree_data("local", data, out_path+to_string(energy)+"GeV/");
	if(mixed) { mix.write_mixed_data(); }
	if(mixed_sets) { mix_sets.write_mixed_data(); }
	if(rand_data) {random.write_random_data(); }
	cout << endl;
}


// Read individual tree. Read each event and for good events/tracks, calculate ratio values and save to data.
void TreeReader::read_tree(TTree* tree) {
	event_leaves event = get_event_leaves(tree);
	proton_leaves proton = get_proton_leaves(tree);

	int event_index = 0;
	while(tree->GetEntry(event_index)) {

		// Check if each event is good. Analyze if so, continue if not.
		if(check_event_good(event, proton, energy)) {
			vector<double> good_proton_angles = {};

			// Iterate over protons in event and add corresponding phi to good_proton_angles if proton good.
			for(int proton_index = 0; proton_index<proton.phi->GetLen(); proton_index++) {
				if(check_proton_good(proton, proton_index)) {
					good_proton_angles.push_back(proton.phi->GetValue(proton_index));
				}
			}

			// Get centrality bin for event from ref_mult2 value
			refmult2CorrUtil->init(event.run->GetValue());
			refmult2CorrUtil->initEvent((int)event.ref_mult2->GetValue(), (double)event.vz->GetValue());
			int cent16_corr = refmult2CorrUtil->getCentralityBin16();
			int cent9_corr = refmult2CorrUtil->getCentralityBin9();

			// If there are enough good protons, calculate ratios for each division and save to data.
			if(good_proton_angles.size() >= (unsigned)cut.min_multi) {

				cent16_events.Fill(cent16_corr);
				cent9_events.Fill(cent9_corr);
				event_cut_hist.Fill(4);

				int cent;
				if(cent_binning == 16) {
					cent = cent16_corr;
				} else {
					cent = cent9_corr;
				}

				if(event_plane) { // If event_plane flag then rotate all angles by -event_plane.
					good_proton_angles = rotate_angles(good_proton_angles, -event.event_plane->GetValue());
				} else if(rotate_random) { // If rotate_random flag then rotate all angles by random angle between 0 and 2pi
					good_proton_angles = rotate_angles(good_proton_angles, trand->Rndm() * 2 * M_PI);
				}

				// If mixed/rand flagged append event to mix/rand object.
				if(mixed) {
					if(cbwc) {
						mix.append_event_CBWC(good_proton_angles, event.ref_mult2->GetValue(), event.event_plane->GetValue(), event.vz->GetValue());
					} else {
						mix.append_event(good_proton_angles, cent, event.event_plane->GetValue(), event.vz->GetValue());
					}
				}
				if(mixed_sets) { mix_sets.append_event(good_proton_angles, event.ref_mult2->GetValue()); }
				if(rand_data) { random.append_event((int)good_proton_angles.size(), event.ref_mult2->GetValue(), trand); }

				for(int div:divs) {
					vector<int> event_ratios = get_Rs(good_proton_angles, div);  // Convert proton angles in event to ratio values.

					// Save ratio values to data
					for(int protons_in_bin:event_ratios) {
						if(cbwc) { // If centrality bin width correction flagged, save refmult2 value in place of centrality bin
							data[div][event.ref_mult2->GetValue()][good_proton_angles.size()][protons_in_bin]++;
						} else {
							data[div][cent][good_proton_angles.size()][protons_in_bin]++;
						}
					}

				}
			}
		}
		event_index++;
	}
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
		for(int div:divs) { out << to_string(div) << " "; }
		out << "}" << endl;

		out << "cbwc: " << boolalpha << cbwc << endl;
		out << "rotate_random: " << boolalpha << rotate_random << endl;
		out << "event_plane: " << boolalpha << event_plane << endl;
		out << "mixed_sets: " << boolalpha << mixed_sets << endl;
		out << "mixed: " << boolalpha << mixed << endl;
		out << "rand_data: " << boolalpha << rand_data << endl;
		out << "cent_binning: " << cent_binning << endl;

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
	}
}


//Get event leaves and return them in an event_leaves struct.
event_leaves TreeReader::get_event_leaves(TTree* tree) {
	event_leaves event;
	event.run = tree->GetLeaf("run");
	event.ref_mult = tree->GetLeaf("Nprim");
	event.ref_mult2 = tree->GetLeaf("ref2");
	event.btof_mult = tree->GetLeaf("btof");
	event.vz = tree->GetLeaf("vtx_z");
	event.run = tree->GetLeaf("run");
	event.event_plane = tree->GetLeaf("event_plane");

	return(event);
}


//Get proton leaves and return them in an protons_leaves struct.
proton_leaves TreeReader::get_proton_leaves(TTree* tree) {
	proton_leaves proton;
	proton.pt = tree->GetLeaf("Proton.pt");
	proton.p = tree->GetLeaf("Proton.p");
	proton.phi = tree->GetLeaf("Proton.phi");
	proton.beta = tree->GetLeaf("Proton.beta");
	proton.dedx = tree->GetLeaf("Proton.dedx");
	proton.charge = tree->GetLeaf("Proton.charge");
	proton.dca = tree->GetLeaf("Proton.dca");
	proton.nsigma = tree->GetLeaf("Proton.nsigma");
	proton.eta = tree->GetLeaf("Proton.eta");

	return(proton);
}


//Returns true if event is good, false if it is bad.
bool TreeReader::check_event_good(event_leaves event, proton_leaves proton, int energy) {
	bool good_event = false;
	event_cut_hist.Fill(0);
	pre_run_hist.Fill(event.run->GetValue());
	if(check_good_run((int)event.run->GetValue())) {
		event_cut_hist.Fill(1);
		if(check_enough_protons(proton)) {
			event_cut_hist.Fill(2);
			if(check_slope(event.btof_mult->GetValue(), event.ref_mult->GetValue(), energy)) {
				event_cut_hist.Fill(3);
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
	bool ref_good_run = !refmult2CorrUtil->isBadRun(run);

	return(list_good_run && ref_good_run);
}


//Checks if there are enough protons in the event.
//If more protons than minimum, return true, else false.
bool TreeReader::check_enough_protons(proton_leaves protons) {
	bool enough_protons = false;
	if(protons.phi->GetLen() >=  cut.min_multi) {
		enough_protons = true;
	}

	return(enough_protons);
}


// Check slope of event. If within cuts, return true for good event, else false.
bool TreeReader::check_slope(int btof_mult, int ref_mult, int energy) {
	bool good_event = true;
	double slope = (double)btof_mult / ref_mult;
	if(slope > cut.max_slope[energy] || slope < cut.min_slope[energy]) {
		good_event = false;
	}
	btof_ref_hist.Fill(btof_mult, ref_mult);

	return(good_event);
}


// Returns true if proton is good and false if proton is bad.
bool TreeReader::check_proton_good(proton_leaves protons, int proton_index) {
	bool good_proton = false;
	track_cut_hist.Fill(0);

	fill_pre_track_qa(protons, proton_index);

	double p = protons.p->GetValue(proton_index);
	if(p < cut.min_p) { return(good_proton); }
	track_cut_hist.Fill(1);

	double pt = protons.pt->GetValue(proton_index);
	if(!(pt >= cut.min_pt && pt <= cut.max_pt)) { return(good_proton); }
	track_cut_hist.Fill(2);

	if(!(protons.charge->GetValue(proton_index) == cut.charge)) { return(good_proton); }
	track_cut_hist.Fill(3);

	double eta = protons.eta->GetValue(proton_index);
	if(!(eta >= cut.min_eta && eta <= cut.max_eta)) { return(good_proton); }
	track_cut_hist.Fill(4);

	double nsigma = protons.nsigma->GetValue(proton_index);
	if(!(nsigma >= cut.min_nsigma && nsigma <= cut.max_nsigma)) { return(good_proton); }
	track_cut_hist.Fill(5);

	double dca = protons.dca->GetValue(proton_index);
	if(!(dca >= cut.min_dca && dca <= cut.max_dca)) { return(good_proton); }
	track_cut_hist.Fill(6);

	if(pt >= cut.min_pt_for_m && pt <= cut.max_pt_for_m) {
		double beta = protons.beta->GetValue(proton_index);
		if(beta > cut.min_beta) {
			double m2 = pow(p, 2) * (pow(beta, -2) - 1.);
			pre_m2_hist.Fill(m2);
			if(m2 > cut.min_m2 && m2 < cut.max_m2) {
				good_proton = true;
				post_m2_hist.Fill(m2);
			}
		}
	} else {
		good_proton = true;
	}
	if(good_proton) { track_cut_hist.Fill(7); }

	fill_post_track_qa(protons, proton_index);

	return(good_proton);
}


// Define all histograms collected for qa.
void TreeReader::define_qa() {
	cent_hist = TH2I(("cent_comp"+to_string(energy)).data(), "Centrality Comparison", 19, -2.5, 16.5, 19, -2.5, 16.5);
	btof_ref_hist = TH2I(("btof_ref"+to_string(energy)).data(), "BTof vs Ref", 3001, -0.5, 3000.5, 601, -0.5, 600.5);

	event_cut_hist = TH1I(("event_cut"+to_string(energy)).data(), "Event Cuts", 5, -0.5, 4.5);
	track_cut_hist = TH1I(("track_cut"+to_string(energy)).data(), "Track Cuts", 8, -0.5, 7.5);
	cent16_events = TH1I(("cent16_events"+to_string(energy)).data(), "Cent16 Events", 18, -1.5, 16.5);
	cent9_events = TH1I(("cent9_events"+to_string(energy)).data(), "Cent9 Events", 11, -1.5, 9.5);

	pre_m2_hist = TH1I(("pre_m2"+to_string(energy)).data(), "pre_m^2", 100, -0.5, 2.0);
	post_m2_hist = TH1I(("post_m2"+to_string(energy)).data(), "post_m^2", 100, -0.5, 2.0);

	pre_run_hist = TH1I(("pre_run"+to_string(energy)).data(), "pre_run", 1000, 1000000, 100000000);
	pre_phi_hist = TH1I(("pre_phi"+to_string(energy)).data(), "pre_phi", 100, 0.0, 7.0);
	pre_p_hist = TH1I(("pre_p"+to_string(energy)).data(), "pre_p", 100, 0.0, 3.5);
	pre_pt_hist = TH1I(("pre_pt"+to_string(energy)).data(), "pre_pt", 100, 0.0, 3.0);
	pre_beta_hist = TH1I(("pre_beta"+to_string(energy)).data(), "pre_beta", 100, -0.5, 2.0);
	pre_charge_hist = TH1I(("pre_charge"+to_string(energy)).data(), "pre_charge", 100, -2.5, 2.5);
	pre_eta_hist = TH1I(("pre_eta"+to_string(energy)).data(), "pre_eta", 100, -1.0, 1.0);
	pre_nsigma_hist = TH1I(("pre_nsigma"+to_string(energy)).data(), "pre_nsigma", 100, -2.5, 2.5);
	pre_dca_hist = TH1I(("pre_dca"+to_string(energy)).data(), "pre_dca", 100, 0.0, 2.5);

	post_run_hist = TH1I(("post_run"+to_string(energy)).data(), "post_run", 1000, 1000000, 100000000);
	post_phi_hist = TH1I(("post_phi"+to_string(energy)).data(), "post_phi", 100, 0.0, 7.0);
	post_p_hist = TH1I(("post_p"+to_string(energy)).data(), "post_p", 100, 0.0, 3.5);
	post_pt_hist = TH1I(("post_pt"+to_string(energy)).data(), "post_pt", 100, 0.0, 3.0);
	post_beta_hist = TH1I(("post_beta"+to_string(energy)).data(), "post_beta", 100, -0.5, 2.0);
	post_charge_hist = TH1I(("post_charge"+to_string(energy)).data(), "post_charge", 100, -2.5, 2.5);
	post_eta_hist = TH1I(("post_eta"+to_string(energy)).data(), "post_eta", 100, -1.0, 1.0);
	post_nsigma_hist = TH1I(("post_nsigma"+to_string(energy)).data(), "post_nsigma", 100, -2.5, 2.5);
	post_dca_hist = TH1I(("post_dca"+to_string(energy)).data(), "post_dca", 100, 0.0, 2.5);

}


// Fill histograms for pre-qa
void TreeReader:: fill_pre_track_qa(proton_leaves proton, int proton_index) {
	pre_phi_hist.Fill(proton.phi->GetValue(proton_index));
	pre_p_hist.Fill(proton.p->GetValue(proton_index));
	pre_pt_hist.Fill(proton.pt->GetValue(proton_index));
	pre_beta_hist.Fill(proton.beta->GetValue(proton_index));
	pre_charge_hist.Fill(proton.charge->GetValue(proton_index));
	pre_eta_hist.Fill(proton.eta->GetValue(proton_index));
	pre_nsigma_hist.Fill(proton.nsigma->GetValue(proton_index));
	pre_dca_hist.Fill(proton.dca->GetValue(proton_index));
}


// Fill histograms for post-qa
void TreeReader:: fill_post_track_qa(proton_leaves proton, int proton_index) {
	post_phi_hist.Fill(proton.phi->GetValue(proton_index));
	post_p_hist.Fill(proton.p->GetValue(proton_index));
	post_pt_hist.Fill(proton.pt->GetValue(proton_index));
	post_beta_hist.Fill(proton.beta->GetValue(proton_index));
	post_charge_hist.Fill(proton.charge->GetValue(proton_index));
	post_eta_hist.Fill(proton.eta->GetValue(proton_index));
	post_nsigma_hist.Fill(proton.nsigma->GetValue(proton_index));
	post_dca_hist.Fill(proton.dca->GetValue(proton_index));
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
	event_cut_hist.Write();
	track_cut_hist.Write();
	cent16_events.Write();
	cent9_events.Write();
	pre_m2_hist.Write();
	post_m2_hist.Write();
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

	// Make before/after canvases for each variable
	TCanvas phi_can("phi_can");
	pre_phi_hist.Draw();
	post_phi_hist.SetLineColor(kRed);
	post_phi_hist.Draw("sames");
	phi_can.Write();
	TCanvas p_can("p_can");
	pre_p_hist.Draw();
	post_p_hist.SetLineColor(kRed);
	post_p_hist.Draw("sames");
	p_can.Write();
	TCanvas pt_can("pt_can");
	pre_pt_hist.Draw();
	post_pt_hist.SetLineColor(kRed);
	post_pt_hist.Draw("sames");
	pt_can.Write();
	TCanvas beta_can("beta_can");
	pre_beta_hist.Draw();
	post_beta_hist.SetLineColor(kRed);
	post_beta_hist.Draw("sames");
	beta_can.Write();
	TCanvas charge_can("charge_can");
	pre_charge_hist.Draw();
	post_charge_hist.SetLineColor(kRed);
	post_charge_hist.Draw("sames");
	charge_can.Write();
	TCanvas eta_can("eta_can");
	pre_eta_hist.Draw();
	post_eta_hist.SetLineColor(kRed);
	post_eta_hist.Draw("sames");
	eta_can.Write();
	TCanvas nsigma_can("nsigma_can");
	pre_nsigma_hist.Draw();
	post_nsigma_hist.SetLineColor(kRed);
	post_nsigma_hist.Draw("sames");
	nsigma_can.Write();
	TCanvas dca_can("dca_can");
	pre_dca_hist.Draw();
	post_dca_hist.SetLineColor(kRed);
	post_dca_hist.Draw("sames");
	dca_can.Write();
	TCanvas m2_can("m2_can");
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
	if(!system(("test -d "+energy_path).data())) { system(("rm -r " + energy_path).data()); }
	if(system(("mkdir " + energy_path).data())) { cout << "Could not create output directory " + energy_path << endl; }
}
