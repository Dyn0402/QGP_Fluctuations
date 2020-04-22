/*
 * AmptCentralityMaker.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */


#include "AmptCentralityMaker.h"



// Structors

AmptCentralityMaker::AmptCentralityMaker() {
	in_path = "";
	set_mult_quantity("ref3");
}

AmptCentralityMaker::AmptCentralityMaker(string path) {
	in_path = path;
	set_mult_quantity("ref3");
}

AmptCentralityMaker::AmptCentralityMaker(string path, string mult_quantity) {
	in_path = path;
	set_mult_quantity(mult_quantity);
}

AmptCentralityMaker::~AmptCentralityMaker() {
	//Nothing
}


// Getters

string AmptCentralityMaker::get_in_path() {
	return in_path;
}

string AmptCentralityMaker::get_mult_quantity() {
	return mult_quantity;
}

vector<int> AmptCentralityMaker::get_ref_bin20_edges() {
	return ref_bin_edges;
}

vector<int> AmptCentralityMaker::get_ref_bin9_edges() {
	vector<int> ref_bin9_edges;
	if((int)ref_bin_edges.size() == 0) { make_centrality(); }
	for(int bin = 0; bin <= 8; bin++) {
		if(bin <= 6) { ref_bin9_edges.push_back(ref_bin_edges[3 + bin * 2]); }
		else if(bin == 7) { ref_bin9_edges.push_back(ref_bin_edges[17]); }
		else if(bin == 8) { ref_bin9_edges.push_back(ref_bin_edges[18]); }
	}
	return ref_bin9_edges;
}

vector<int> AmptCentralityMaker::get_ref_bin16_edges() {
	vector<int> ref_bin16_edges;
	if((int)ref_bin_edges.size() == 0) { make_centrality(); }
	for(int bin = 0; bin <= 15; bin++) {
		ref_bin16_edges.push_back(ref_bin_edges[3 + bin]);
	}
	return ref_bin16_edges;
}

int AmptCentralityMaker::get_cent_bin20(int mult) {
	int cent_bin = -1;

	if(in_string(mult_quantity, "ref")) {

		if((int)ref_bin_edges.size() == 0) { make_centrality(); }

		for(int edge:ref_bin_edges) {
			cent_bin++;
			if(mult < edge) break;
		}
		if(mult > ref_bin_edges.back()) { cent_bin++; }
	} else {
		cout << "Impact parameter binning not implemented" << endl;
	}

	return cent_bin;
}

int AmptCentralityMaker::get_cent_bin9(int mult) {
	int cent20_bin = get_cent_bin20(mult);

	int cent_bin = 0;
	switch (cent20_bin) {
	case 19 : cent_bin = 8; break;
	case 18 : cent_bin = 7; break;
	case 1 : cent_bin = -1; break;
	case 0 : cent_bin = -1; break;
	case -1 : cent_bin = -1; break;
	default : cent_bin = cent20_bin / 2 - 2; break;
	}

	return cent_bin;
}


// Setters

void AmptCentralityMaker::set_in_path(string path) {
	in_path = path;
}

void AmptCentralityMaker::set_mult_quantity(string quantity) {
	if(quantity == "ref2") {
		append_dist_val = bind(&AmptCentralityMaker::get_ref2, this);
		sort_and_bin = bind(&AmptCentralityMaker::sort_bin_ref, this);
	} else if(quantity == "ref3") {
		append_dist_val = bind(&AmptCentralityMaker::get_ref3, this);
		sort_and_bin = bind(&AmptCentralityMaker::sort_bin_ref, this);
	} else if(quantity == "b") {
		append_dist_val = bind(&AmptCentralityMaker::get_b, this);
		sort_and_bin = bind(&AmptCentralityMaker::sort_bin_b, this);
	} else {
		cout << "Invalid multiplicty quantity to define centrality. Options are: ref2, ref3, b" << endl;
		return;
	}
	mult_quantity = quantity;
}

void AmptCentralityMaker::set_max_b(float b) {
	max_b = b;
}


// Doers

void AmptCentralityMaker::make_centrality() {
	get_distribution();
	sort_and_bin();
}

void AmptCentralityMaker::get_distribution() {
	ref_dist.clear();
	b_dist.clear();
//	cout << "AmptCentMaker in_path: " << in_path << endl;
	vector<string> in_files = get_files_in_dir(in_path, "root", "path");
	for(string path:in_files) {
		TFile *file = new TFile(path.data(), "READ");
		TTree *tree = (TTree*)file->Get(tree_name.data());
//		cout << path << "  " << tree << endl;
		leaves = get_ampt_tree_leaves(tree);

		int event_index = 0;
		while(tree->GetEvent(event_index)) {
//			cout << leaves.ref3->GetValue() << endl;
			append_dist_val();
			event_index++;
		}
		file->Close();
		delete file;
	}
}

void AmptCentralityMaker::get_ref2() {
	if(leaves.imp->GetValue() <= max_b) {
		ref_dist.push_back(leaves.ref2->GetValue());
	}
}

void AmptCentralityMaker::get_ref3() {
//	cout << "here" << endl;
	if(leaves.imp->GetValue() <= max_b) {
//		cout << "here2" << endl;
		ref_dist.push_back(leaves.ref3->GetValue());
//		cout << "here3" << endl;
	}
//	cout << "here4" << endl;
}

void AmptCentralityMaker::get_b() {
	b_dist.push_back(leaves.imp->GetValue());
}

void AmptCentralityMaker::sort_bin_ref() {
	sort(ref_dist.begin(), ref_dist.end());
	float step = (float)ref_dist.size() / bins;
	ref_bin_edges.clear();
	for(int bin_edge = 1; bin_edge < bins ; bin_edge++) {
		int index = (int)(bin_edge * step + 0.5);
		ref_bin_edges.push_back(ref_dist[index]);
	}
//	int num_last_bin = 0;
//	for(int entry:ref_dist) { if(entry > ref_bin_edges.back()) num_last_bin++; }
//	cout << "Total events: " << ref_dist.size() << "  |  Events greater than last bin edge: " << num_last_bin << endl;
	ref_dist.clear();
}

void AmptCentralityMaker::sort_bin_b() {
	sort(b_dist.begin(), b_dist.end());
	float step = (float)b_dist.size() / bins;
	b_bin_edges.clear();
	for(int bin_edge = 1; bin_edge < bins ; bin_edge++) {
		int index = (int)(bin_edge * step + 0.5);
		b_bin_edges.push_back(b_dist[index]);
	}
	b_dist.clear();
}

