/*
 * TreeLeaves.h
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */

#ifndef TREEBRANCHES_H_
#define TREEBRANCHES_H_

#include <string>

#include "TTree.h"
#include "TLeaf.h"
#include "TChain.h"

using namespace std;

struct tree_leaves {
	TLeaf *run;
	TLeaf *event_id;

	TLeaf *ref_mult;
	TLeaf *ref_multn;
	TLeaf *btof;

	TLeaf *vx;
	TLeaf *vy;
	TLeaf *vz;

	TLeaf *qx;
	TLeaf *qy;

	TLeaf *dca_xy_avg;
	TLeaf *dca_xy_err;

	TLeaf *phi;
	TLeaf *pt;
	TLeaf *beta;
	TLeaf *charge;
	TLeaf *dca;
	TLeaf *nsigma;
	TLeaf *eta;
};


struct ampt_tree_leaves {
	TLeaf *event;
	TLeaf *pmult;
	TLeaf *imp;
	TLeaf *refn;
	TLeaf *event_plane;
	TLeaf *pid;
	TLeaf *px;
	TLeaf *py;
	TLeaf *pz;
};


struct tree_branches {
	int run_num;
	int event_id;

	short refmult;
	short refmultn;
	short btof;

	float vx;
	float vy;
	float vz;

	float qx;
	float qy;

	float dca_xy_avg;
	float dca_xy_err;

	vector<float> *phi = 0;
	vector<float> *pt = 0;
	vector<float> *beta = 0;
	vector<float> *dca = 0;
	vector<float> *nsigma = 0;
	vector<float> *eta = 0;
	vector<short> *charge = 0;

	TBranch *branch_pt = 0;
	TBranch *branch_phi = 0;
	TBranch *branch_beta = 0;
	TBranch *branch_dca = 0;
	TBranch *branch_nsigma = 0;
	TBranch *branch_eta = 0;
	TBranch *branch_charge = 0;
};


tree_leaves get_tree_leaves(TTree* tree, string particle = "proton", int ref = 3);
ampt_tree_leaves get_ampt_tree_leaves(TTree* tree, int ref = 3);
void set_tree_branches(TTree* tree, tree_branches &branches, string particle = "proton", int ref = 3);
void set_tree_branches(TChain* chain, tree_branches &branches, string particle = "proton", int ref = 3);



#endif /* TREEBRANCHES_H_ */
