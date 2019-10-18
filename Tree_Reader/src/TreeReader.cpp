/*
 * TreeReader.cpp
 *
 *  Created on: Jul 18, 2019
 *      Author: dylan
 */


#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <thread>
#include <ctime>
#include <chrono>

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
#include "Mixer.h"
#include "file_io.h"
#include "ratio_methods.h"

using namespace std;


// Structors

TreeReader::TreeReader(int energy) {
	start_sys = chrono::system_clock::now();
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed = false;
	this->energy = energy;

	define_qa();

	if(energy == 27) { cut.min_nsigma = -1.0; cut.max_nsigma = 1.0; }
}

TreeReader::~TreeReader() {
//	delete refmult2CorrUtil;
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

bool TreeReader::get_mixed() {
	return(mixed);
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

void TreeReader::set_mixed(bool mixed) {
	this->mixed = mixed;
}


// Doers

// Read files for single energy and write results to text files.
void TreeReader::read_trees() {
	cout << "Reading " + to_string(energy) + "GeV trees." << endl;
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV/", "root", "path");

	unsigned num_files = in_files.size();
	unsigned file_index = 1;

	if(event_plane) { // If event_plane flagged, need to flatten event plane before using raw values.
		for(string path:in_files) {
			cout << "Need to implement event plane flattening" << endl;  // Need to implement
		}
	}

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

	write_qa();
	cout << " Writing " + to_string(energy) + "GeV trees." << endl;
	write_tree_data("local", data, out_path+to_string(energy)+"GeV/");
	mix.write_mixed_data();
	cout << endl;
}


// Read individual tree. Read each event and for good events/tracks, calculate ratio values and save to data.
void TreeReader::read_tree(TTree* tree) {
	event_leaves event = get_event_leaves(tree);
	proton_leaves proton = get_proton_leaves(tree);

	TRandom3 *rand = new TRandom3(0);

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
			int cent = get_centrality(event.ref_mult2->GetValue(), energy);
			int cent9 = get_centrality9(event.ref_mult2->GetValue(), energy);
			int cent2 = -2;
			if(!refmult2CorrUtil->isBadRun(event.run->GetValue()) || true) { //Used || true as hack to include bad runs. Fix.
				refmult2CorrUtil->init(event.run->GetValue());
				refmult2CorrUtil->initEvent((int)event.ref_mult2->GetValue(), (double)event.vz->GetValue());
				cent2 = refmult2CorrUtil->getCentralityBin16();
			} //else { cout << "Refmult said was a bad run" << endl; }
			cent_hist.Fill(cent, cent2);

			// If there are enough good protons, calculate ratios for each division and save to data.
			if(good_proton_angles.size() >= (unsigned)cut.min_multi) {

				cent16_events.Fill(cent);
				cent9_events.Fill(cent9);
				event_cut_hist.Fill(4);

				if(event_plane) { // If event_plane flag then rotate all angles by -event_plane.
					double event_plane = event.event_plane->GetValue();
					cout << "Need to flatten event plane angles before using. Need to implement" << endl;  // Need to implement.
					good_proton_angles = rotate_angles(good_proton_angles, -event_plane);
				} else if(rotate_random) { // If rotate_random flag then rotate all angles by random angle between 0 and 2pi
					good_proton_angles = rotate_angles(good_proton_angles, rand->Rndm() * 2 * M_PI);
				}

				// If mixed flagged append event to mix object.
				if(mixed) { mix.append_event(good_proton_angles, event.ref_mult2->GetValue()); }

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
	bool good_run = find(cut.bad_runs.begin(), cut.bad_runs.end(), run) == cut.bad_runs.end();

	return(good_run);
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

	double beta = protons.beta->GetValue(proton_index);
//	if(!(beta > cut.min_beta && beta < cut.max_beta)) { return(good_proton); }

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


//Taken directly from Roli.
//Given energy and refmult2, will return centrality of event.
int TreeReader::get_centrality(int refmult2, int energy){

    int cent = -1;

    if(energy == 7){

        if(refmult2 >= 165) cent = 15;
        else if(refmult2 >= 137 && refmult2 < 165) cent = 14;
        else if(refmult2 >= 114 && refmult2 < 137) cent = 13;
        else if(refmult2 >= 95 && refmult2 < 114) cent = 12;
        else if(refmult2 >= 78 && refmult2 < 95) cent = 11;
        else if(refmult2 >= 64 && refmult2 < 78) cent = 10;
        else if(refmult2 >= 51 && refmult2 < 64) cent = 9;
        else if(refmult2 >= 41 && refmult2 < 51) cent = 8;
        else if(refmult2 >= 32 && refmult2 < 41) cent = 7;
        else if(refmult2 >= 25 && refmult2 < 32) cent = 6;
        else if(refmult2 >= 19 && refmult2 < 25) cent = 5;
        else if(refmult2 >= 14 && refmult2 < 19) cent = 4;
        else if(refmult2 >= 10 && refmult2 < 14) cent = 3;
        else if(refmult2 >= 7 && refmult2 < 10) cent = 2;
        else if(refmult2 >= 5 && refmult2 < 7) cent = 1;
        else if(refmult2 >= 3 && refmult2 < 5) cent = 0;

    }

    else if(energy == 11){

        if(refmult2 >= 206) cent = 15;
        else if(refmult2 >= 172 && refmult2 < 206) cent = 14;
        else if(refmult2 >= 143 && refmult2 < 172) cent = 13;
        else if(refmult2 >= 118 && refmult2 < 143) cent = 12;
        else if(refmult2 >= 98 && refmult2 < 118) cent = 11;
        else if(refmult2 >= 80 && refmult2 < 98) cent = 10;
        else if(refmult2 >= 65 && refmult2 < 80) cent = 9;
        else if(refmult2 >= 52 && refmult2 < 65) cent = 8;
        else if(refmult2 >= 41 && refmult2 < 52) cent = 7;
        else if(refmult2 >= 32 && refmult2 < 41) cent = 6;
        else if(refmult2 >= 24 && refmult2 < 32) cent = 5;
        else if(refmult2 >= 18 && refmult2 < 24) cent = 4;
        else if(refmult2 >= 13 && refmult2 < 18) cent = 3;
        else if(refmult2 >= 9 && refmult2 < 13) cent = 2;
        else if(refmult2 >= 6 && refmult2 < 9) cent = 1;
        else if(refmult2 >= 4 && refmult2 < 6) cent = 0;

    }

    else if(energy == 14){ ///// we take refmult here

        if(refmult2 >= 239) cent = 15;
        else if(refmult2 >= 200 && refmult2 < 239) cent = 14;
        else if(refmult2 >= 167 && refmult2 < 200) cent = 13;
        else if(refmult2 >= 139 && refmult2 < 167) cent = 12;
        else if(refmult2 >= 115 && refmult2 < 139) cent = 11;
        else if(refmult2 >= 94 && refmult2 < 115) cent = 10;
        else if(refmult2 >= 76 && refmult2 < 94) cent = 9;
        else if(refmult2 >= 61 && refmult2 < 76) cent = 8;
        else if(refmult2 >= 48 && refmult2 < 61) cent = 7;
        else if(refmult2 >= 37 && refmult2 < 48) cent = 6;
        else if(refmult2 >= 28 && refmult2 < 37) cent = 5;
        else if(refmult2 >= 21 && refmult2 < 28) cent = 4;
        else if(refmult2 >= 16 && refmult2 < 21) cent = 3;
        else if(refmult2 >= 11 && refmult2 < 16) cent = 2;
        else if(refmult2 >= 8 && refmult2 < 11) cent = 1;
        else if(refmult2 >= 5 && refmult2 < 8) cent = 0;

    }

    else if(energy == 19){

        if(refmult2 >= 258) cent = 15;
        else if(refmult2 >= 215 && refmult2 < 258) cent = 14;
        else if(refmult2 >= 180 && refmult2 < 215) cent = 13;
        else if(refmult2 >= 149 && refmult2 < 180) cent = 12;
        else if(refmult2 >= 123 && refmult2 < 149) cent = 11;
        else if(refmult2 >= 100 && refmult2 < 123) cent = 10;
        else if(refmult2 >= 81 && refmult2 < 100) cent = 9;
        else if(refmult2 >= 65 && refmult2 < 81) cent = 8;
        else if(refmult2 >= 51 && refmult2 < 65) cent = 7;
        else if(refmult2 >= 40 && refmult2 < 51) cent = 6;
        else if(refmult2 >= 30 && refmult2 < 40) cent = 5;
        else if(refmult2 >= 22 && refmult2 < 30) cent = 4;
        else if(refmult2 >= 16 && refmult2 < 22) cent = 3;
        else if(refmult2 >= 12 && refmult2 < 16) cent = 2;
        else if(refmult2 >= 8 && refmult2 < 12) cent = 1;
        else if(refmult2 >= 5 && refmult2 < 8) cent = 0;

    }

    else if(energy == 27){

        if(refmult2 >= 284) cent = 15;
        else if(refmult2 >= 237 && refmult2 < 284) cent = 14;
        else if(refmult2 >= 198 && refmult2 < 237) cent = 13;
        else if(refmult2 >= 164 && refmult2 < 198) cent = 12;
        else if(refmult2 >= 135 && refmult2 < 164) cent = 11;
        else if(refmult2 >= 111 && refmult2 < 135) cent = 10;
        else if(refmult2 >= 90 && refmult2 < 111) cent = 9;
        else if(refmult2 >= 71 && refmult2 < 90) cent = 8;
        else if(refmult2 >= 56 && refmult2 < 71) cent = 7;
        else if(refmult2 >= 43 && refmult2 < 56) cent = 6;
        else if(refmult2 >= 33 && refmult2 < 43) cent = 5;
        else if(refmult2 >= 25 && refmult2 < 33) cent = 4;
        else if(refmult2 >= 18 && refmult2 < 25) cent = 3;
        else if(refmult2 >= 13 && refmult2 < 18) cent = 2;
        else if(refmult2 >= 9 && refmult2 < 13) cent = 1;
        else if(refmult2 >= 6 && refmult2 < 9) cent = 0;

    }

    else if(energy == 39){

        if(refmult2 >= 307) cent = 15;
        else if(refmult2 >= 257 && refmult2 < 307) cent = 14;
        else if(refmult2 >= 215 && refmult2 < 257) cent = 13;
        else if(refmult2 >= 179 && refmult2 < 215) cent = 12;
        else if(refmult2 >= 147 && refmult2 < 179) cent = 11;
        else if(refmult2 >= 121 && refmult2 < 147) cent = 10;
        else if(refmult2 >= 97 && refmult2 < 121) cent = 9;
        else if(refmult2 >= 78 && refmult2 < 97) cent = 8;
        else if(refmult2 >= 61 && refmult2 < 78) cent = 7;
        else if(refmult2 >= 47 && refmult2 < 61) cent = 6;
        else if(refmult2 >= 36 && refmult2 < 47) cent = 5;
        else if(refmult2 >= 27 && refmult2 < 36) cent = 4;
        else if(refmult2 >= 20 && refmult2 < 27) cent = 3;
        else if(refmult2 >= 14 && refmult2 < 20) cent = 2;
        else if(refmult2 >= 10 && refmult2 < 14) cent = 1;
        else if(refmult2 >= 7 && refmult2 < 10) cent = 0;

    }

    else if(energy == 62){

        if(refmult2 >= 334) cent = 15;
        else if(refmult2 >= 279 && refmult2 < 334) cent = 14;
        else if(refmult2 >= 233 && refmult2 < 279) cent = 13;
        else if(refmult2 >= 194 && refmult2 < 233) cent = 12;
        else if(refmult2 >= 160 && refmult2 < 194) cent = 11;
        else if(refmult2 >= 131 && refmult2 < 160) cent = 10;
        else if(refmult2 >= 106 && refmult2 < 131) cent = 9;
        else if(refmult2 >= 84 && refmult2 < 106) cent = 8;
        else if(refmult2 >= 66 && refmult2 < 84) cent = 7;
        else if(refmult2 >= 51 && refmult2 < 66) cent = 6;
        else if(refmult2 >= 39 && refmult2 < 51) cent = 5;
        else if(refmult2 >= 29 && refmult2 < 39) cent = 4;
        else if(refmult2 >= 21 && refmult2 < 29) cent = 3;
        else if(refmult2 >= 15 && refmult2 < 21) cent = 2;
        else if(refmult2 >= 10 && refmult2 < 15) cent = 1;
        else if(refmult2 >= 7 && refmult2 < 10) cent = 0;

    }

    else if(energy == 200){

        if(refmult2 >= 421) cent = 15;
        else if(refmult2 >= 355 && refmult2 < 421) cent = 14;
        else if(refmult2 >= 297 && refmult2 < 355) cent = 13;
        else if(refmult2 >= 247 && refmult2 < 297) cent = 12;
        else if(refmult2 >= 204 && refmult2 < 247) cent = 11;
        else if(refmult2 >= 167 && refmult2 < 204) cent = 10;
        else if(refmult2 >= 135 && refmult2 < 167) cent = 9;
        else if(refmult2 >= 108 && refmult2 < 135) cent = 8;
        else if(refmult2 >= 85 && refmult2 < 108) cent = 7;
        else if(refmult2 >= 65 && refmult2 < 85) cent = 6;
        else if(refmult2 >= 50 && refmult2 < 65) cent = 5;
        else if(refmult2 >= 37 && refmult2 < 50) cent = 4;
        else if(refmult2 >= 27 && refmult2 < 37) cent = 3;
        else if(refmult2 >= 19 && refmult2 < 27) cent = 2;
        else if(refmult2 >= 13 && refmult2 < 19) cent = 1;
        else if(refmult2 >= 9 && refmult2 < 13) cent = 0;

    }

    else {
    	cout << "Unimplemented energy " << energy << " returned centrality " << cent << endl;
    }


    return cent;

}


//Taken directly from Roli.
//Given energy and refmult2, will return centrality of event.
int TreeReader::get_centrality9(int mult, int energy){

    int central = -1;

    if(energy == 7){

        float centFull[9] = {32,41,51,64,78,95,114,137,165};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 11){

        float centFull[9] = {41,52,65,80,98,118,143,172,206};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 19){

        float centFull[9] = {51,65,81,100,123,149,180,215,258};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 27){

        float centFull[9] = {56,71,90,111,135,164,198,237,284};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 39){

        float centFull[9] = {61,78,97,121,147,179,215,257,307};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 62){

        float centFull[9] = {66,84,106,131,160,194,233,279,334};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 200){

        float centFull[9] = {85,108,135,167,204,247,297,355,421};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else {
		cout << "Unimplemented energy " << energy << " returned centrality " << central << endl;
	}

    return central;

}
