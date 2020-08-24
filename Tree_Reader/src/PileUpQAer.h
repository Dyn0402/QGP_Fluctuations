/*
 * PileUpQAer.h
 *
 *  Created on: Aug 7, 2020
 *      Author: Dylan Neff
 */

#ifndef PILEUPQAER_H_
#define PILEUPQAER_H_


#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <mutex>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"

#include "file_io.h"
#include "TreeBranches.h"

using namespace std;


pair<float, float> rotate_xy(float x, float y, float angle);


class PileUpQAer {
public:
	// Structors
	PileUpQAer(int energy);
	PileUpQAer(int energy, mutex *mtx);
	~PileUpQAer();

	// Getters
	pair<float, float> get_low_cut();
	pair<float, float> get_high_cut();

	// Setters
	void set_energy(int energy);
	void set_in_path(string path);

	// Doers
	void run_qa();

private:
	// Attributes
	mutex *mtx = NULL;  // To lock non-thread-safe graphics (TCanvas) processes

	int energy;
	string in_path = "/media/ucla/Research/BES1_Trees/";
	string out_path = "/home/dylan/Research/Pile_Up_QA/";
	string out_file_suf = "_Pile_Up_Cuts.txt";
	TFile *out_file = NULL;
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;
	string tree_name = "tree";

	string orig_btof_ref_name = "btof_ref_original_";
	string orig_btof_ref_title = "BTof Mutiplicity vs Reference Multiplicity ";

	float sigmas = 3.0;
	int min_points = 100;

	float rot_slice_height = 5.0;

	int orig_btof_bins = 301;
	float orig_btof_low = -0.5;
	float orig_btof_high = orig_btof_low + orig_btof_bins;

	int orig_ref_bins = 601;
	float orig_ref_low = -0.5;
	float orig_ref_high = orig_ref_low + orig_ref_bins;

	tree_branches branches;

	// Data Containers
	vector<pair<short, short>> ref_btof_pairs;  //{refmult, btof}
	pair<float, float> low_cut;
	pair<float, float> high_cut;


	// Doers
	void read_trees();
	void rotate_dist();
	void get_cuts();

	void read_tree(TTree *tree);
	void set_branches(TTree *tree);

	void write_cut_file();
	void read_cut_file();
};



#endif /* PILEUPQAER_H_ */


