/*
 * tree_reader.cpp
 *
 *  Created on: Jun 4, 2019
 *      Author: Dylan Neff
 */


#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>

#include "flatten.h"
#include "file_io.h"
#include "config.h"



TH1D* get_dist(TTree *tree) {
	TLeaf *l_psi = tree->GetLeaf("psi");

	TH1D *dist = new TH1D("event_angle_dist", "Event Angle Distribution", config::bins, config::lBound, config::rBound);

	double angle;
	for(int i=0; i<tree->GetEntries(); i++) {
		tree->GetEntry(i);
		angle = l_psi->GetValue(i);
		dist->Fill(angle);
	}

	return(dist);
}


TH1D* get_flat_dist(TTree *tree, string coef_path, string entry) {
	TLeaf *l_psi = tree->GetLeaf("psi");
	vector<double> angles;

	double angle;
	for(int i=0; i<tree->GetEntries(); i++) {
		tree->GetEntry(i);
		angle = l_psi->GetValue(i);
		angles.push_back(angle);
	}

	vector<double> A,B;
	tie(A,B) = get_coefs(coef_path, entry);

	TH1D *flat_dist = flatten_dist(angles, A, B, config::n_flatten_min, config::n_flatten_max, config::lBound, config::rBound, "flat"+entry);

	return(flat_dist);
}


void flatten_tree(TTree *tree, string entry, string coef_path, TFile *out_file) {
	TH1D *dist = get_dist(tree);
	vector<double> A, B;
	tie(A, B) = calcCoef(dist, config::n_flatten_min, config::n_flatten_max, config::lBound, config::rBound);
	write_coefs(A, B, entry, coef_path);
	TH1D *flat_dist = get_flat_dist(tree, coef_path, entry);
	out_file->cd();
	dist->Write(entry.data());
	flat_dist->Write(("flat"+entry).data());
}
