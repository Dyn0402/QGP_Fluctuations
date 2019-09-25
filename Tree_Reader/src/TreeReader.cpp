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
#include "file_io.h"
#include "ratio_methods.h"

using namespace std;


TreeReader::TreeReader() {
	start_sys = chrono::system_clock::now();
	cbwc = false;
}


void TreeReader::read_trees() {
	ROOT::EnableThreadSafety();
	vector<thread> threads;
	for(int energy:energy_list) {
		threads.push_back(thread(&TreeReader::read_energy, this, energy));
	}
	for(thread & th : threads) {
		if(th.joinable()) {
			th.join();
		}
	}
}


// Read files for single energy and write results to text files.
void TreeReader::read_energy(int energy) {
	cout << "Reading " + to_string(energy) + "GeV trees." << endl;
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV/", "root", "path");
	unsigned num_files = in_files.size();
	unsigned file_index = 1;
	tree_data data;
	auto *cent_hist = new TH2I(("cent_comp"+to_string(energy)).data(), "Centrality Comparison", 19, -2.5, 16.5, 19, -2.5, 16.5);
	StRefMultCorr *refmult2CorrUtil = new StRefMultCorr("refmult2");
	for(string path:in_files) {
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			cout << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s" << endl;
		}
		TFile *file = new TFile(path.data(), "READ");
		TTree *tree = (TTree*)file->Get(tree_name.data());
		if(cbwc) {
			read_tree_cbwc(tree, &data, energy, refmult2CorrUtil, cent_hist);
		} else {
			read_tree(tree, &data, energy, refmult2CorrUtil, cent_hist);
		}
		file->Close();
		delete file;
		file_index++;
	}
	TFile *qa = new TFile((qa_path + qa_name + to_string(energy) + "GeV.root").data(), "RECREATE");
	TCanvas *can = new TCanvas();
	cent_hist->Draw("COLZ");
	can->Write();
	cent_hist->Write();
	delete cent_hist;
	delete can;
	qa->Close();
	delete qa;
	cout << " Writing " + to_string(energy) + "GeV trees." << endl;
	write_tree_data("local", data, out_path+to_string(energy)+"GeV/");
	cout << endl;
}


void TreeReader::read_tree(TTree* tree, tree_data *data, int energy, StRefMultCorr *refmult2CorrUtil, TH2I *cent_hist) {
	event_leaves event = get_event_leaves(tree);
	proton_leaves proton = get_proton_leaves(tree);

	TRandom3 *rand = new TRandom3(0);

	int event_index = 1;
	while(tree->GetEntry(event_index)) {
		if(check_event_good(event, proton, energy)) {
			vector<double> good_proton_angles = {};
			for(int proton_index = 0; proton_index<proton.phi->GetLen(); proton_index++) {
				if(check_proton_good(proton, proton_index)) {
					good_proton_angles.push_back(proton.phi->GetValue(proton_index));
				}
			}
			int cent = get_centrality(event.ref_mult2->GetValue(), energy);
			int cent2 = -2;
			if(!refmult2CorrUtil->isBadRun(event.run->GetValue()) || true) { //Used || true as hack to include bad runs. Fix.
				refmult2CorrUtil->init(event.run->GetValue());
				refmult2CorrUtil->initEvent((int)event.ref_mult2->GetValue(), (double)event.vz->GetValue());
				cent2 = refmult2CorrUtil->getCentralityBin16();
			} //else { cout << "Refmult said was a bad run" << endl; }
			cent_hist->Fill(cent, cent2);
			if(good_proton_angles.size() >= (unsigned)cut.min_multi) {
				data->good_protons[cent][(int)good_proton_angles.size()]++;
				for(int div:divs) {
					good_proton_angles = rotate_angles(good_proton_angles, rand->Rndm() * 2 * M_PI);
					vector<int> event_ratios = get_Rs(good_proton_angles, div);
					for(int protons_in_bin:event_ratios) {
						data->ratios[div][cent][good_proton_angles.size()][protons_in_bin]++;
					}
				}
			}
		}
		event_index++;
	}
}



void TreeReader::read_tree_cbwc(TTree* tree, tree_data *data, int energy, StRefMultCorr *refmult2CorrUtil, TH2I *cent_hist) {
	event_leaves event = get_event_leaves(tree);
	proton_leaves proton = get_proton_leaves(tree);

	TRandom3 *rand = new TRandom3(0);

	int event_index = 1;
	while(tree->GetEntry(event_index)) {
		if(check_event_good(event, proton, energy)) {
			vector<double> good_proton_angles = {};
			for(int proton_index = 0; proton_index<proton.phi->GetLen(); proton_index++) {
				if(check_proton_good(proton, proton_index)) {
					good_proton_angles.push_back(proton.phi->GetValue(proton_index));
				}
			}
			int cent = get_centrality(event.ref_mult2->GetValue(), energy);
			int cent2 = -2;
			if(!refmult2CorrUtil->isBadRun(event.run->GetValue()) || true) { //Used || true as hack to include bad runs. Fix.
				refmult2CorrUtil->init(event.run->GetValue());
				refmult2CorrUtil->initEvent((int)event.ref_mult2->GetValue(), (double)event.vz->GetValue());
				cent2 = refmult2CorrUtil->getCentralityBin16();
			} //else { cout << "Refmult said was a bad run" << endl; }
			cent_hist->Fill(cent, cent2);
			if(good_proton_angles.size() >= (unsigned)cut.min_multi) {
				data->good_protons[cent][(int)good_proton_angles.size()]++;
				for(int div:divs) {
					good_proton_angles = rotate_angles(good_proton_angles, rand->Rndm() * 2 * M_PI);
					vector<int> event_ratios = get_Rs(good_proton_angles, div);
					for(int protons_in_bin:event_ratios) {
						data->ratios[div][event.ref_mult2->GetValue()][good_proton_angles.size()][protons_in_bin]++;
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
	event.btof_mult = tree->GetLeaf("btof_multi");
	event.vz = tree->GetLeaf("vtx_z");
	event.run = tree->GetLeaf("run");

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
	if(check_good_run((int)event.run->GetValue())) {
		if(check_enough_protons(proton)) {
			if(check_slope(event.btof_mult->GetValue(), event.ref_mult->GetValue(), energy)) {
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


// Returns true if proton is good and false if proton is bad.
bool TreeReader::check_proton_good(proton_leaves protons, int proton_index) {
	bool good_proton = false;

	double p = protons.p->GetValue(proton_index);
//	if(!(p > cut.min_p && p < cut.max_p)) { return(good_proton); }

	double pt = protons.pt->GetValue(proton_index);
	if(!(pt > cut.min_pt && pt < cut.max_pt)) { return(good_proton); }

	double beta = protons.beta->GetValue(proton_index);
//	if(!(beta > cut.min_beta && beta < cut.max_beta)) { return(good_proton); }

	if(!(protons.charge->GetValue() == cut.charge)) { return(good_proton); }

	double eta = protons.eta->GetValue(proton_index);
	if(!(eta > cut.min_eta && eta < cut.max_eta)) { return(good_proton); }

//	double nsigma = protons.nsigma->GetValue(proton_index);
//	if(!(nsigma > cut.min_nsigma && nsigma < cut.max_nsigma)) { return(good_proton); }

	double dca = protons.dca->GetValue(proton_index);
	if(!(dca > cut.min_dca && dca < cut.max_dca)) { return(good_proton); }

	if(pt > cut.min_pt_for_m && pt < cut.max_pt_for_m) {
		if(beta > cut.min_beta) {
			double m2 = pow(pow(p, 2) * (pow(beta, -2) - 1.), 0.5);
			if(m2 > cut.min_m2 && m2 < cut.max_m2) {
				good_proton = true;
			}
		}
	} else {
		double nsigma = protons.nsigma->GetValue(proton_index);
		if(!(nsigma > cut.min_nsigma && nsigma < cut.max_nsigma)) { return(good_proton); }
		good_proton = true;
	}

//		double p = protons.p->GetValue(proton_index);
//	//	if(p >  cut.min_p && p <  cut.max_p) {
//			double pt = protons.pt->GetValue(proton_index);
//			if(pt <  cut.max_pt) {
//				double beta = protons.beta->GetValue(proton_index);
//	//			if(beta >  cut.min_beta && beta <  cut.max_beta) {
//					if(protons.charge->GetValue() ==  cut.charge) {
//						double eta = protons.eta->GetValue(proton_index);
//						if(eta >  cut.min_eta && eta <  cut.max_eta) {
//							double nsigma = protons.nsigma->GetValue(proton_index);
//							if(nsigma >  cut.min_nsigma && nsigma <  cut.max_nsigma) {
//								double dca = protons.dca->GetValue(proton_index);
//								if(dca >  cut.min_dca && dca <  cut.max_dca) {
//									if(pt >  cut.min_pt_for_m && pt < cut.max_pt_for_m) {
//										if(beta > cut.min_beta) {
//											double m2 = pow(pow(p, 2) * (pow(beta, -2) - 1.), 0.5);
//											if(m2 >  cut.min_m2 && m2 <  cut.max_m2) {
//												good_proton = true;
//											}
//										}
//									} else {
//										good_proton = true;
//									}
//								}
//							}
//						}
//					}
//	//			}
//			}
//	//	}


	return(good_proton);
}


//Taken directly from Roli.
//Given energy and refmult2, will return centrality of event.
int TreeReader::get_centrality(double refmult2, int energy){

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


// Check slope of event. If within cuts, return true for good event, else false.
bool TreeReader::check_slope(double btof_mult, double ref_mult, int energy) {
	bool good_event = true;
	double slope = btof_mult / ref_mult;
	if(slope > cut.max_slope[energy] || slope < cut.min_slope[energy]) {
		good_event = false;
	}

	return(good_event);
}


bool TreeReader::get_cbwc() {
	return(cbwc);
}


void TreeReader::set_cbwc(bool cbwc) {
	this->cbwc = cbwc;
}
