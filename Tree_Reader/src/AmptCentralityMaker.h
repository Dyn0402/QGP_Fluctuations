/*
 * AmptCentralityMaker.h
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */

#ifndef AMPTCENTRALITYMAKER_H_
#define AMPTCENTRALITYMAKER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <functional>

#include "Event.h"
#include "Track.h"
#include "file_io.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TH1.h"
#include "TreeBranches.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TLine.h"

using namespace std;


struct event_imp_ref {
	short ref;
	float b;
};


class AmptCentralityMaker {
public:
	// Structors
	AmptCentralityMaker();
	AmptCentralityMaker(string min_bias_path);
	AmptCentralityMaker(string min_bias_path, string mult_quantity);
	AmptCentralityMaker(string min_bias_path, string qa_path, string mult_quantity);
	AmptCentralityMaker(int energy, string min_bias_path, string qa_path, string mult_quantity);
	~AmptCentralityMaker();

	// Getters
	string get_min_bias_path();
	string get_mult_quantity();
	vector<int> get_ref_bin20_edges();
	vector<int> get_ref_bin16_edges();
	vector<int> get_ref_bin9_edges();

	int get_cent_bin20(int mult);
	int get_cent_bin9(int mult);

	// Setters
	void set_energy(int energy);
	void set_qa_path(string qa_path);
	void set_min_bias_path(string min_bias_path);
	void set_star_data_path(string star_data_path);
	void set_mult_quantity(string quantity);
	void set_ref_num(int ref);

	// Doers
	void run_b_opt(vector<int> energy_vec);
	void make_centrality(bool make_new = false);
	void plot_ref_vs_b(vector<int> energy_vec);

private:
	// Attributes
	int bins = 20;
	float max_b = 20.0;
	int ref_num = 3;
	bool b_check = false;  // No longer set or check maximum impact parameter, not correct!

	float acc = 0.0001;
	map<int, int> energy_color {{7, 1}, {11, 2}, {19, 4}, {27, 6}, {39, 8}, {62, 9}};
	map<int, float> b_min {{7, 14}, {11, 14}, {19, 13.5}, {27, 13.5}, {39, 13}, {62, 13}};
	float b_cut_step_init = 0.5;

	float ref_min = -0.5;
	float ref_max = 1000.5;
	int ref_bins = (int)(ref_max - ref_min);

	int energy;

	string qa_path;
	string min_bias_path;
	string star_data_path;
	string mult_quantity;
	string qa_file_sufx = "Ampt_opt_b.txt";
	string bin_file_sufx = "Ampt_ref_bin_edge.txt";

	string tree_name = "tree";

	// Data Containers
	vector<event_imp_ref> events;
	TH1D star_ref_dist;

	map<int, float> opt_b_max;

	vector<int> ref_bin_edges;
	vector<float> b_bin_edges;

	vector<int> ref_dist;
	vector<float> b_dist;

	ampt_tree_branches branches;

	// Doers
	void get_distribution();

	function<void()> append_dist_val;
	void get_ref2();
	void get_ref3();
	void get_b();

	function<void()> sort_and_bin;
	void sort_bin_ref();
	void sort_bin_b();

	float get_chi_dof_diff(float b, int energy);
	void plot_ref_dist_comp(float b, int energy);
	void get_imp_ref(int energy);
	void get_star_ref(int energy);

	void set_ampt_branches(TTree *tree);
	void set_star_branches(TTree *tree);

	void write_b_opt();
	void read_b_opt();

	void write_bin_edges();
	void read_bin_edges();
};


#endif /* AMPTCENTRALITYMAKER_H_ */
