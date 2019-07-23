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

#include <TTree.h>
#include <TLeaf.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom3.h>
#include <TCanvas.h>

#include "../StRoot/StRefMultCorr/CentralityMaker.h"
#include "../StRoot/StRefMultCorr/StRefMultCorr.h"

#include "TreeReader.h"
#include "tree_reader.h"
#include "ratio_methods.h"

#include "config_reader.h"

using namespace std;


void read_tree(TTree* tree, tree_data *data, int energy, StRefMultCorr *refmult2CorrUtil, TH2I *cent_hist) {
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
			if(good_proton_angles.size() >= (unsigned)cuts::min_multi) {
				data->good_protons[cent][(int)good_proton_angles.size()]++;
				for(int div:pars::divs) {
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


//Get event leaves and return them in an event_leaves struct.
event_leaves get_event_leaves(TTree* tree) {
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
proton_leaves get_proton_leaves(TTree* tree) {
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
bool check_event_good(event_leaves event, proton_leaves proton, int energy) {
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
bool check_good_run(int run) {
	bool good_run = find(cuts::bad_runs.begin(), cuts::bad_runs.end(), run) == cuts::bad_runs.end();

	return(good_run);
}


//Checks if there are enough protons in the event.
//If more protons than minimum, return true, else false.
bool check_enough_protons(proton_leaves protons) {
	bool enough_protons = false;
	if(protons.phi->GetLen() >=  cuts::min_multi) {
		enough_protons = true;
	}

	return(enough_protons);
}


// Returns true if proton is good and false if proton is bad.
bool check_proton_good(proton_leaves protons, int proton_index) {
	bool good_proton = false;

	double p = protons.p->GetValue(proton_index);
	if(!(p > cuts::min_p && p < cuts::max_p)) { return(good_proton); }

	double pt = protons.pt->GetValue(proton_index);
	if(!(pt < cuts::max_pt)) { return(good_proton); }

	double beta = protons.beta->GetValue(proton_index);
//	if(!(beta > cuts::min_beta && beta < cuts::max_beta)) { return(good_proton); }

	if(!(protons.charge->GetValue() == cuts::charge)) { return(good_proton); }

	double eta = protons.eta->GetValue(proton_index);
	if(!(eta > cuts::min_eta && eta < cuts::max_eta)) { return(good_proton); }

	double nsigma = protons.nsigma->GetValue(proton_index);
	if(!(nsigma > cuts::min_nsigma && nsigma < cuts::max_nsigma)) { return(good_proton); }

	double dca = protons.dca->GetValue(proton_index);
	if(!(dca > cuts::min_dca && dca < cuts::max_dca)) { return(good_proton); }

	if(pt > cuts::min_pt_for_m && pt < cuts::max_pt_for_m) {
		if(beta > cuts::min_beta) {
			double m2 = pow(pow(p, 2) * (pow(beta, -2) - 1.), 0.5);
			if(m2 > cuts::min_m2 && m2 < cuts::max_m2) {
				good_proton = true;
			}
		}
	} else {
		good_proton = true;
	}

	return(good_proton);
}
