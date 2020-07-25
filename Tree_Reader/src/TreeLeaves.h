/*
 * TreeLeaves.h
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */

#ifndef TREELEAVES_H_
#define TREELEAVES_H_

#include <string>

#include "TTree.h"
#include "TLeaf.h"
#include "TChain.h"

using namespace std;

struct tree_leaves {
	TLeaf *run;
	TLeaf *event_id;

	TLeaf *dca_xy_avg;
	TLeaf *dca_xy_err;

	TLeaf *ref_mult;
	TLeaf *ref_multn;
	TLeaf *btof;

	TLeaf *vx;
	TLeaf *vy;
	TLeaf *vz;

	TLeaf *qx;
	TLeaf *qy;

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
	int run;
	int ref_mult;
	int ref_multn;
	int btof;
	float vx;
	float vy;
	float vz;
	float event_plane;

	float phi;
	float pt;
	float beta;
	int charge;
	float dca;
	float nsigma;
	float eta;
};


tree_leaves get_tree_leaves(TTree* tree, string particle = "proton", int ref = 3);
tree_branches get_chain_branches(TChain* chain, string particle = "proton", int ref = 3);
ampt_tree_leaves get_ampt_tree_leaves(TTree* tree, int ref = 3);



#endif /* TREELEAVES_H_ */
