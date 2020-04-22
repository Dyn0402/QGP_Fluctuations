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

using namespace std;

struct tree_leaves {
	TLeaf *run;
	TLeaf *ref_mult;
	TLeaf *ref_multn;
	TLeaf *btof;
	TLeaf *vx;
	TLeaf *vy;
	TLeaf *vz;
	TLeaf *event_plane;

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


tree_leaves get_tree_leaves(TTree* tree, string particle = "Proton", int ref = 3);
ampt_tree_leaves get_ampt_tree_leaves(TTree* tree, int ref = 3);



#endif /* TREELEAVES_H_ */
