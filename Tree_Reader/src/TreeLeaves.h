/*
 * TreeLeaves.h
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */

#ifndef TREELEAVES_H_
#define TREELEAVES_H_


#include "TTree.h"
#include "TLeaf.h"

struct tree_leaves {
	TLeaf *run;
	TLeaf *ref_mult;
	TLeaf *ref_mult2;
	TLeaf *btof;
	TLeaf *vx;
	TLeaf *vy;
	TLeaf *vz;
	TLeaf *event_plane;

	TLeaf *phi;
	TLeaf *pt;
	TLeaf *p;
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
	TLeaf *pid;
	TLeaf *px;
	TLeaf *py;
	TLeaf *pz;
};


tree_leaves get_tree_leaves(TTree* tree);
tree_leaves get_tree_leaves_new(TTree* tree);
ampt_tree_leaves get_ampt_tree_leaves(TTree* tree);



#endif /* TREELEAVES_H_ */
