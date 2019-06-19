/*
 * tree_reader.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <vector>

#include <TTree.h>
#include <TLeaf.h>

#include "tree_reader.h"
#include "ratio_methods.h"

using namespace std;


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
