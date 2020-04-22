/*
 * TreeLeaves.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */


#include "TreeLeaves.h"


//Get tree leaves and return them in a tree_leaves struct.
tree_leaves get_tree_leaves(TTree* tree, string particle, int ref) {
	tree_leaves leaves;
	leaves.run = tree->GetLeaf("run");
	leaves.ref_mult = tree->GetLeaf("Nprim");
	leaves.ref_multn = tree->GetLeaf(("ref"+to_string(ref)).data());
	leaves.btof = tree->GetLeaf("btof");
	leaves.vx = tree->GetLeaf("vtx_x");
	leaves.vy = tree->GetLeaf("vtx_y");
	leaves.vz = tree->GetLeaf("vtx_z");
	leaves.event_plane = tree->GetLeaf(("event_plane_ref"+to_string(ref)).data());

	leaves.pt = tree->GetLeaf((particle+".pt").data());
	leaves.phi = tree->GetLeaf((particle+".phi").data());
	leaves.beta = tree->GetLeaf((particle+".beta").data());
	leaves.charge = tree->GetLeaf((particle+".charge").data());
	leaves.dca = tree->GetLeaf((particle+".dca").data());
	leaves.nsigma = tree->GetLeaf((particle+".nsigma").data());
	leaves.eta = tree->GetLeaf((particle+".eta").data());

	return(leaves);
}


//Get ampt tree leaves and return them in a tree_leaves struct.
ampt_tree_leaves get_ampt_tree_leaves(TTree* tree, int ref) {
	ampt_tree_leaves leaves;
	leaves.event = tree->GetLeaf("event");
	leaves.pmult = tree->GetLeaf("pmult");
	leaves.imp = tree->GetLeaf("imp");
	leaves.refn = tree->GetLeaf(("refmult"+to_string(ref)).data());
	leaves.event_plane = tree->GetLeaf(("event_plane_ref"+to_string(ref)).data());
	leaves.pid = tree->GetLeaf("pid");
	leaves.px = tree->GetLeaf("px");
	leaves.py = tree->GetLeaf("py");
	leaves.pz = tree->GetLeaf("pz");

	return(leaves);
}
