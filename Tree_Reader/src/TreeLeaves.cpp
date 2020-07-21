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
	leaves.event_id = tree->GetLeaf("event_id");
	leaves.dca_xy_avg = tree->GetLeaf("dca_xy_avg");
	leaves.dca_xy_sd = tree->GetLeaf("dca_xy_err");
	leaves.ref_mult = tree->GetLeaf("Nprim");
	leaves.ref_multn = tree->GetLeaf(("ref"+to_string(ref)).data());
	leaves.btof = tree->GetLeaf("btof");
	leaves.vx = tree->GetLeaf("vtx_x");
	leaves.vy = tree->GetLeaf("vtx_y");
	leaves.vz = tree->GetLeaf("vtx_z");
	leaves.qx = tree->GetLeaf("qx");
	leaves.qy = tree->GetLeaf("qy");

	leaves.pt = tree->GetLeaf((particle+".pt").data());
	leaves.phi = tree->GetLeaf((particle+".phi").data());
	leaves.beta = tree->GetLeaf((particle+".beta").data());
	leaves.charge = tree->GetLeaf((particle+".charge").data());
	leaves.dca = tree->GetLeaf((particle+".dca").data());
	leaves.nsigma = tree->GetLeaf((particle+".nsigma").data());
	leaves.eta = tree->GetLeaf((particle+".eta").data());

	return(leaves);
}

//Get tree leaves and return them in a tree_leaves struct.
tree_branches get_chain_branches(TChain *chain, string particle, int ref) {
	tree_branches branches;
	chain->SetBranchAddress("run", &branches.run);
	chain->SetBranchAddress("Nprim", &branches.ref_mult);
	chain->SetBranchAddress(("ref"+to_string(ref)).data(), &branches.ref_multn);
	chain->SetBranchAddress("btof", &branches.btof);
	chain->SetBranchAddress("vtx_x", &branches.vx);
	chain->SetBranchAddress("vtx_y", &branches.vy);
	chain->SetBranchAddress("vtx_z", &branches.vz);
	chain->SetBranchAddress(("event_plane_ref"+to_string(ref)).data(), &branches.event_plane);

	chain->SetBranchAddress((particle+".pt").data(), &branches.pt);
	chain->SetBranchAddress((particle+".phi").data(), &branches.phi);
	chain->SetBranchAddress((particle+".beta").data(), &branches.beta);
	chain->SetBranchAddress((particle+".charge").data(), &branches.charge);
	chain->SetBranchAddress((particle+".dca").data(), &branches.dca);
	chain->SetBranchAddress((particle+".nsigma").data(), &branches.nsigma);
	chain->SetBranchAddress((particle+".eta").data(), &branches.eta);

	return(branches);
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
