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

#include "ratio_methods.h"

using namespace std;


struct event_leaves {
	TLeaf *run;
	TLeaf *ref_mult;
	TLeaf *ref_mult2;
	TLeaf *btof_mult;
};

struct proton_leaves {
	TLeaf *phi;
	TLeaf *pt;
	TLeaf *p;
	TLeaf *beta;
	TLeaf *dedx;
	TLeaf *charge;
	TLeaf *dca;
	TLeaf *nsigma;
	TLeaf *eta;
};

tree_data read_tree(TTree* tree, tree_data data, int energy);

event_leaves get_event_leaves(TTree* tree);
proton_leaves get_proton_leaves(TTree* tree);

bool check_event_good(event_leaves event, proton_leaves, int energy);
bool check_enough_protons(proton_leaves protons);
bool check_good_run(int run);

bool check_proton_good(proton_leaves protons, int proton_index);

int get_centrality(double refmult2, int energy);
bool check_slope(double btof_mult, double ref_mult, int energy);


#endif /* TREE_READER_H_ */
