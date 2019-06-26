/*
 * tree_reader.h
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */

#ifndef TREE_READER_H_
#define TREE_READER_H_

#include <vector>

#include <TChain.h>
#include <TTree.h>
#include <TLeaf.h>

using namespace std;


struct tree_data {
	map<int, map<int, map<int, map<int, int>>>> ratios; //ratios[divisions][centrality][num protons in event][num protons in bin]
	map<int, map<int, int>> good_protons; //good_protons[centrality][num protons]
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

tree_data read_tree(TChain* tree, int energy);

tree_data init_tree_data();
event_leaves get_event_leaves(TChain* tree);
proton_leaves get_proton_leaves(TChain* tree);

bool check_event_good(event_leaves event, proton_leaves, int energy);
bool check_enough_protons(proton_leaves protons);
bool check_good_run(int run);

bool check_proton_good(proton_leaves protons, int proton_index);

int get_centrality(double refmult2, int energy);


#endif /* TREE_READER_H_ */
