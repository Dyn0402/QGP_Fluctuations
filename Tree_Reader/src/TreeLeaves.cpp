/*
 * TreeLeaves.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */


#include <iostream>

#include "TreeLeaves.h"

using namespace std;


//Get tree leaves and return them in a tree_leaves struct.
tree_leaves get_tree_leaves(TTree* tree) {
	tree_leaves leaves;
	leaves.run = tree->GetLeaf("run");
	leaves.ref_mult = tree->GetLeaf("Nprim");
	leaves.ref_mult2 = tree->GetLeaf("ref2");
	leaves.ref_mult3 = tree->GetLeaf("ref3");
	leaves.btof = tree->GetLeaf("btof");
	leaves.vx = tree->GetLeaf("vtx_x");
	leaves.vy = tree->GetLeaf("vtx_y");
	leaves.vz = tree->GetLeaf("vtx_z");
	leaves.event_plane_ref2 = tree->GetLeaf("event_plane_ref2");
	leaves.event_plane_ref3 = tree->GetLeaf("event_plane_ref3");

	leaves.pt = tree->GetLeaf("Track.pt");
	leaves.p = tree->GetLeaf("Track.p");
	leaves.phi = tree->GetLeaf("Track.phi");
	leaves.beta = tree->GetLeaf("Track.beta");
	leaves.charge = tree->GetLeaf("Track.charge");
	leaves.dca = tree->GetLeaf("Track.dca");
	leaves.nsigmapr = tree->GetLeaf("Track.nsigmapr");
	leaves.nsigmapi = tree->GetLeaf("Track.nsigmapi");
	leaves.eta = tree->GetLeaf("Track.eta");

	return(leaves);
}


//Get tree leaves and return them in a tree_leaves struct. From new event trees.
tree_leaves get_tree_leaves_new(TTree* tree) {
	tree_leaves leaves;
	leaves.run = tree->GetLeaf("run");
	leaves.ref_mult = tree->GetLeaf("ref");
	leaves.ref_mult2 = tree->GetLeaf("refn");
	leaves.btof = tree->GetLeaf("btof");
	leaves.vx = tree->GetLeaf("vx");
	leaves.vy = tree->GetLeaf("vy");
	leaves.vz = tree->GetLeaf("vz");
	leaves.event_plane = tree->GetLeaf("event_plane");

	leaves.pt = tree->GetLeaf("protons.pt");
	leaves.p = tree->GetLeaf("protons.p");
	leaves.phi = tree->GetLeaf("protons.phi");
	leaves.beta = tree->GetLeaf("protons.beta");
	leaves.charge = tree->GetLeaf("protons.charge");
	leaves.dca = tree->GetLeaf("protons.dca");
	leaves.nsigma = tree->GetLeaf("protons.nsigma");
	leaves.eta = tree->GetLeaf("protons.eta");

	return(leaves);
}

//Get ampt tree leaves and return them in a tree_leaves struct.
ampt_tree_leaves get_ampt_tree_leaves(TTree* tree) {
	ampt_tree_leaves leaves;
	leaves.event = tree->GetLeaf("event");
	leaves.pmult = tree->GetLeaf("pmult");
	leaves.imp = tree->GetLeaf("imp");
	leaves.ref2 = tree->GetLeaf("refmult2");
	leaves.ref3 = tree->GetLeaf("refmult3");
	leaves.event_plane_ref2 = tree->GetLeaf("event_plane_ref2");
	leaves.event_plane_ref3 = tree->GetLeaf("event_plane_ref3");
	leaves.pid = tree->GetLeaf("pid");
	leaves.px = tree->GetLeaf("px");
	leaves.py = tree->GetLeaf("py");
	leaves.pz = tree->GetLeaf("pz");

	return(leaves);
}
