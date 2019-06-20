/*
 * tree_reader.h
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */

#ifndef TREE_READER_H_
#define TREE_READER_H_

#include <vector>

#include <TTree.h>
#include <TLeaf.h>

using namespace std;


struct tree_data {
	map<int, vector<int>> good_protons; //good_protons[centrality][event_index]
	map<int, map<int, vector<double>>> ratios; //ratios[divisions][centrality][proton_index]
};

struct event_leaves {
	TLeaf *run;
	TLeaf *ref_mult;
	TLeaf *ref_mult2;
};

struct proton_leaves {
	TLeaf *phi;
	TLeaf *pt;
	TLeaf *p;
	TLeaf *beta;
	TLeaf *dedx;
};

tree_data read_tree(TTree* tree, int energy);

tree_data init_tree_data();
event_leaves get_event_leaves(TTree* tree);
proton_leaves get_proton_leaves(TTree* tree);

vector<double> get_tree_ratios(TTree *tree, int div);
vector<double> get_event_ratios(TLeaf *proton_psi, int div);
vector<int> get_tree_nprotons(TTree *tree);

bool check_event_good(event_leaves event, int energy);
bool check_enough_protons(proton_leaves protons);
bool check_good_run(int run);
bool check_ref2(double ref2, int energy);

bool check_proton_good(proton_leaves protons, int proton_index);

int get_centrality(double refmult2, int energy);


#endif /* TREE_READER_H_ */
