/*
 * AmptCentralityMaker.h
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */

#ifndef AMPTCENTRALITYMAKER_H_
#define AMPTCENTRALITYMAKER_H_

#include <iostream>
#include <vector>
#include <functional>

#include "Event.h"
#include "Track.h"
#include "file_io.h"

#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TH1.h"
#include "TreeBranches.h"
#include "TMultiGraph.h"
#include "TLegend.h"

using namespace std;

class AmptCentralityMaker {
public:
	// Structors
	AmptCentralityMaker();
	AmptCentralityMaker(string in_path);
	AmptCentralityMaker(string in_path, string mult_quantity);
	AmptCentralityMaker(string in_path, string out_path, string mult_quantity);
	~AmptCentralityMaker();

	// Getters
	string get_in_path();
	string get_mult_quantity();
	vector<int> get_ref_bin20_edges();
	vector<int> get_ref_bin16_edges();
	vector<int> get_ref_bin9_edges();

	int get_cent_bin20(int mult);
	int get_cent_bin9(int mult);

	// Setters
	void set_in_path(string in_path);
	void set_out_path(string out_path);
	void set_min_bias_path(string minb_bias_path);
	void set_mult_quantity(string quantity);
	void set_max_b(float b);
	void set_ref_num(int ref);

	// Doers
	void run_b_opt(vector<int> energy_vec, string out_path);
	float get_chi_diff(float b, int energy);

private:
	// Attributes
	int bins = 20;
	float max_b = 20.0;
	int ref_num = 3;

	vector<int> ref_bin_edges;
	vector<float> b_bin_edges;

	string in_path;
	string out_path;
	string min_bias_path;
	string mult_quantity;

	string tree_name = "tree";

	vector<int> ref_dist;
	vector<float> b_dist;

	ampt_tree_branches branches;

	// Doers
	void set_branches(TTree *tree);

	void make_centrality();
	void get_distribution();

	function<void()> append_dist_val;
	void get_ref2();
	void get_ref3();
	void get_b();

	function<void()> sort_and_bin;
	void sort_bin_ref();
	void sort_bin_b();

	void write_b_opt();
	void read_b_opt();

	void write_bin_edges();
	void read_bin_edges();
};


#endif /* AMPTCENTRALITYMAKER_H_ */
