/*
 * TreeLeaves.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */


#include "TreeBranches.h"


// Get tree leaves and return them in a tree_leaves struct.
tree_leaves get_tree_leaves(TTree* tree, string particle, int ref) {
	tree_leaves leaves;
	leaves.run = tree->GetLeaf("run_num");
	leaves.event_id = tree->GetLeaf("event_id");
	leaves.ref_mult = tree->GetLeaf("refmult");
	leaves.ref_multn = tree->GetLeaf(("refmult"+to_string(ref)).data());
	leaves.btof = tree->GetLeaf("btof");
	leaves.vx = tree->GetLeaf("vx");
	leaves.vy = tree->GetLeaf("vy");
	leaves.vz = tree->GetLeaf("vz");
	leaves.qx = tree->GetLeaf("qx");
	leaves.qy = tree->GetLeaf("qy");
	leaves.dca_xy_avg = tree->GetLeaf("dca_xy_avg");
	leaves.dca_xy_err = tree->GetLeaf("dca_xy_err");

	leaves.pt = tree->GetLeaf((particle+".pt").data());
	leaves.phi = tree->GetLeaf((particle+".phi").data());
	leaves.beta = tree->GetLeaf((particle+".beta").data());
	leaves.charge = tree->GetLeaf((particle+".charge").data());
	leaves.dca = tree->GetLeaf((particle+".dca").data());
	leaves.nsigma = tree->GetLeaf((particle+".nsigma").data());
	leaves.eta = tree->GetLeaf((particle+".eta").data());
	leaves.nhits_fit = tree->GetLeaf((particle + ".nhits_fit").data());

	return leaves;
}


// Set tree branch addresses and return them in a tree_branches struct.
void set_tree_branches(TTree* tree, tree_branches &branches, string particle, int ref) {
	tree->SetBranchAddress("run_num", &branches.run_num);
	tree->SetBranchAddress("event_id", &branches.event_id);

	tree->SetBranchAddress("refmult", &branches.refmult);
	tree->SetBranchAddress(("refmult"+to_string(ref)).data(), &branches.refmultn);
	tree->SetBranchAddress("btof_multi", &branches.btof_multi);
	tree->SetBranchAddress("btof_match", &branches.btof_match);

	tree->SetBranchAddress("vx", &branches.vx);
	tree->SetBranchAddress("vy", &branches.vy);
	tree->SetBranchAddress("vz", &branches.vz);

	tree->SetBranchAddress("qx", &branches.qx);
	tree->SetBranchAddress("qy", &branches.qy);

	tree->SetBranchAddress("dca_xy_avg", &branches.dca_xy_avg);
	tree->SetBranchAddress("dca_xy_err", &branches.dca_xy_err);

	tree->SetBranchAddress((particle+".phi").data(), &branches.phi, &branches.branch_phi);
	tree->SetBranchAddress((particle+".pt").data(), &branches.pt, &branches.branch_pt);
	tree->SetBranchAddress((particle+".beta").data(), &branches.beta, &branches.branch_beta);
	tree->SetBranchAddress((particle+".charge").data(), &branches.charge, &branches.branch_charge);
	tree->SetBranchAddress((particle+".dca").data(), &branches.dca, &branches.branch_dca);
	tree->SetBranchAddress((particle+".dca_z").data(), &branches.dca_z, &branches.branch_dca_z);
	tree->SetBranchAddress((particle+".nsigma").data(), &branches.nsigma, &branches.branch_nsigma);
	tree->SetBranchAddress((particle+".eta").data(), &branches.eta, &branches.branch_eta);
	tree->SetBranchAddress((particle + ".nhits_fit").data(), &branches.nhits_fit, &branches.branch_nhits_fit);
}

// Set tree branch addresses and return them in a tree_branches struct.
void set_tree_branches(TChain* chain, tree_branches &branches, string particle, int ref) {
	chain->SetBranchAddress("run_num", &branches.run_num);
	chain->SetBranchAddress("event_id", &branches.event_id);

	chain->SetBranchAddress("refmult", &branches.refmult);
	chain->SetBranchAddress(("refmult"+to_string(ref)).data(), &branches.refmultn);
	chain->SetBranchAddress("btof_multi", &branches.btof_multi);
	chain->SetBranchAddress("btof_match", &branches.btof_match);

	chain->SetBranchAddress("vx", &branches.vx);
	chain->SetBranchAddress("vy", &branches.vy);
	chain->SetBranchAddress("vz", &branches.vz);

	chain->SetBranchAddress("qx", &branches.qx);
	chain->SetBranchAddress("qy", &branches.qy);

	chain->SetBranchAddress("dca_xy_avg", &branches.dca_xy_avg);
	chain->SetBranchAddress("dca_xy_err", &branches.dca_xy_err);

	chain->SetBranchAddress((particle+".phi").data(), &branches.phi, &branches.branch_phi);
	chain->SetBranchAddress((particle+".pt").data(), &branches.pt, &branches.branch_pt);
	chain->SetBranchAddress((particle+".beta").data(), &branches.beta, &branches.branch_beta);
	chain->SetBranchAddress((particle+".charge").data(), &branches.charge, &branches.branch_charge);
	chain->SetBranchAddress((particle+".dca").data(), &branches.dca, &branches.branch_dca);
	chain->SetBranchAddress((particle+".dca_z").data(), &branches.dca_z, &branches.branch_dca_z);
	chain->SetBranchAddress((particle+".nsigma").data(), &branches.nsigma, &branches.branch_nsigma);
	chain->SetBranchAddress((particle+".eta").data(), &branches.eta, &branches.branch_eta);
	chain->SetBranchAddress((particle + ".nhits_fit").data(), &branches.nhits_fit, &branches.branch_nhits_fit);
}


// Set tree particle branch addresses only return them in a tree_branches struct.
void set_particle_branches(TTree* tree, tree_branches &branches, string particle) {
	tree->SetBranchAddress((particle+".phi").data(), &branches.phi, &branches.branch_phi);
	tree->SetBranchAddress((particle+".pt").data(), &branches.pt, &branches.branch_pt);
	tree->SetBranchAddress((particle+".beta").data(), &branches.beta, &branches.branch_beta);
	tree->SetBranchAddress((particle+".charge").data(), &branches.charge, &branches.branch_charge);
	tree->SetBranchAddress((particle+".dca").data(), &branches.dca, &branches.branch_dca);
	tree->SetBranchAddress((particle+".dca_z").data(), &branches.dca_z, &branches.branch_dca_z);
	tree->SetBranchAddress((particle+".nsigma").data(), &branches.nsigma, &branches.branch_nsigma);
	tree->SetBranchAddress((particle+".eta").data(), &branches.eta, &branches.branch_eta);
	tree->SetBranchAddress((particle + ".nhits_fit").data(), &branches.nhits_fit, &branches.branch_nhits_fit);
}


// Set tree branch addresses and return them in a tree_branches struct.
void set_ampt_tree_branches(TTree* tree, ampt_tree_branches &branches) {
	tree->SetBranchAddress("event", &branches.event);

	tree->SetBranchAddress("refmult", &branches.refmult);
	tree->SetBranchAddress("refmult2", &branches.refmult2);
	tree->SetBranchAddress("refmult3", &branches.refmult3);

	tree->SetBranchAddress("imp", &branches.imp);
	tree->SetBranchAddress("qx", &branches.qx);
	tree->SetBranchAddress("qy", &branches.qy);

	tree->SetBranchAddress("npp", &branches.npp);
	tree->SetBranchAddress("npt", &branches.npt);
	tree->SetBranchAddress("nesp", &branches.nesp);
	tree->SetBranchAddress("ninesp", &branches.ninesp);
	tree->SetBranchAddress("nest", &branches.nest);
	tree->SetBranchAddress("ninest", &branches.ninest);

	tree->SetBranchAddress("pid", &branches.pid, &branches.branch_pid);
	tree->SetBranchAddress("px", &branches.px, &branches.branch_px);
	tree->SetBranchAddress("py", &branches.py, &branches.branch_py);
	tree->SetBranchAddress("pz", &branches.pz, &branches.branch_pz);
}


// Set tree branch addresses and return them in a tree_branches struct.
void set_cooper_frye_tree_branches(TTree* tree, cooper_frye_tree_branches &branches) {
	tree->SetBranchAddress("event", &branches.event);

	tree->SetBranchAddress("refmult", &branches.refmult);
	tree->SetBranchAddress("refmult2", &branches.refmult2);
	tree->SetBranchAddress("refmult3", &branches.refmult3);

	tree->SetBranchAddress("qx", &branches.qx);
	tree->SetBranchAddress("qy", &branches.qy);

	tree->SetBranchAddress("pid", &branches.pid, &branches.branch_pid);
	tree->SetBranchAddress("px", &branches.px, &branches.branch_px);
	tree->SetBranchAddress("py", &branches.py, &branches.branch_py);
	tree->SetBranchAddress("pz", &branches.pz, &branches.branch_pz);
}


// Get ampt tree leaves and return them in a tree_leaves struct.
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

	return leaves;
}
