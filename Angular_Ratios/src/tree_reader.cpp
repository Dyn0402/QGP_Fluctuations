/*
 * tree_reader.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

#include <TTree.h>
#include <TLeaf.h>

#include "tree_reader.h"
#include "ratio_methods.h"
#include "config.h"

using namespace std;


tree_data read_tree(TTree* tree) {
	tree_data data;
	data.good_protons = {};
	data.ratios = vector<vector<double>> (config::divs.size(), vector<double> {});

	event_leaves event = get_event_leaves(tree);
	proton_leaves proton = get_proton_leaves(tree);

	int n_events = tree->GetEntries();
	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		if(check_event_good(event) && check_enough_protons(proton)) {
			int n_good_protons = 0;
			vector<double> good_proton_angles = {};
			for(int proton_index = 0; proton_index<proton.phi->GetLen(); proton_index++) {
				if(check_proton_good(proton, proton_index)) {
					n_good_protons++;
					good_proton_angles.push_back(proton.phi->GetValue(proton_index));
				}
			}
			data.good_protons.push_back(n_good_protons);
			for(unsigned i=0; i<config::divs.size(); i++) {
				vector<double> event_ratios = get_Rs(good_proton_angles, config::divs[i]);
				data.ratios[i].insert(data.ratios[i].end(), event_ratios.begin(), event_ratios.end());
			}
		}
		if( !(event_index % (n_events / 10)) ) {
			cout << (int)(((double)event_index) / n_events * 100 + 0.5) << "% complete. " << event_index << " of " << n_events << endl;
		}
		event_index++;
	}


	return(data);
}


//Get event leaves and return them in an event_leaves struct.
event_leaves get_event_leaves(TTree* tree) {
	event_leaves event;
	event.run = tree->GetLeaf("run");
	event.ref_mult = tree->GetLeaf("Nprim");
	event.ref_mult2 = tree->GetLeaf("ref2");

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

	return(proton);
}



vector<double> get_tree_ratios(TTree *tree, int div) {
	vector<double> ratios;
	TLeaf *proton_psi = tree->GetLeaf("Proton.phi");

	int events = tree->GetEntries();
	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		vector<double> event_ratios = get_event_ratios(proton_psi, div);
		ratios.insert(ratios.end(), event_ratios.begin(), event_ratios.end());
		if( !(event_index % (events / 10)) ) {
			cout << (int)(((double)event_index) / events * 100 + 0.5) << "% complete. " << event_index << " of " << events << endl;
		}
		event_index++;
	}

	return(ratios);
}


vector<double> get_event_ratios(TLeaf *proton_psi, int div) {
	vector<double> angles;
	for(int i = 0; i<proton_psi->GetLen(); i++) {
		angles.push_back(proton_psi->GetValue(i));
	}
	vector<double> ratios = get_Rs(angles, div);
	return(ratios);
}


vector<int> get_tree_nprotons(TTree *tree) {
	vector<int> nprotons;
	TLeaf *proton_psi = tree->GetLeaf("Proton.phi");

	int events = tree->GetEntries();
	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		nprotons.push_back(proton_psi->GetLen());
		if( !(event_index % (events / 10)) ) {
			cout << (int)(((double)event_index) / events * 100 + 0.5) << "% complete. " << event_index << " of " << events << endl;
		}
		event_index++;
	}

	return(nprotons);
}


//Returns true if event is good, false if it is bad.
bool check_event_good(event_leaves event) {
	bool good_event = false;
	if(check_good_run((int)event.run->GetValue())) {
		if(check_ref2(event.ref_mult2->GetValue())) {
			good_event = true;
		}
	}

	return(good_event);
}

//Check the list of bad runs for input run.
//If input run is contained in bad run list, return false (bad run) else return true (good run).
bool check_good_run(int run) {
	bool good_run = find(config::bad_runs.begin(), config::bad_runs.end(), run) == config::bad_runs.end();

	return(good_run);
}


//Check if ref2 is less than ref2_min. If so return false (bad event), else return true (good event).
bool check_ref2(double ref2) {
	bool good_event = true;
	if(ref2 < config::ref2_min) {
		good_event = false;
	}

	return(good_event);
}


//Checks if there are enough protons in the event.
//If more protons than minumum, return true, else false.
bool check_enough_protons(proton_leaves protons) {
	bool enough_protons = false;
	if(protons.phi->GetLen() >= config::proton_min_multi) {
		enough_protons = true;
	}

	return(enough_protons);
}


//Returns true if proton is good and false if proton is bad.
bool check_proton_good(proton_leaves protons, int proton_index) {
	bool good_proton = false;
	if(protons.pt->GetValue(proton_index) < config::proton_max_pt) {
		good_proton = true;
	} else {
		double beta = protons.beta->GetValue(proton_index);
		double p = protons.p->GetValue(proton_index);
		if(beta > config::proton_min_beta) {
			double m = pow(p, 2) * (pow(beta, -2) - 1.);
			if(m > config::proton_min_m && m < config::proton_max_m) {
				good_proton = true;
			}
		}
	}

	return(good_proton);
}
