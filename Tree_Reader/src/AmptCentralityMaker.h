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
#include "TreeLeaves.h"
#include "file_io.h"

#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TH1.h"

using namespace std;

class AmptCentralityMaker {
public:
	// Structors
	AmptCentralityMaker();
	AmptCentralityMaker(string path);
	AmptCentralityMaker(string path, string mult_quantity);
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
	void set_in_path(string path);
	void set_mult_quantity(string quantity);
	void set_max_b(float b);

	// Doers

private:
	// Attributes
	int bins = 20;
	float max_b = 20.0;

	vector<int> ref_bin_edges;
	vector<float> b_bin_edges;

	string in_path;
	string mult_quantity;

	string tree_name = "tree";

	vector<int> ref_dist;
	vector<float> b_dist;

	ampt_tree_leaves leaves;

	// Doers
	void make_centrality();
	void get_distribution();

	function<void()> append_dist_val;
	void get_ref2();
	void get_ref3();
	void get_b();

	function<void()> sort_and_bin;
	void sort_bin_ref();
	void sort_bin_b();
};


#endif /* AMPTCENTRALITYMAKER_H_ */
