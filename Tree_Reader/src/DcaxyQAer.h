/*
 * Dca_xy_QAer.h
 *
 *  Created on: Jul 2, 2020
 *      Author: Dylan Neff
 */

#ifndef DCAXYQAER_H_
#define DCAXYQAER_H_

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>

#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TF2.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLine.h>

#include "file_io.h"
#include "TreeLeaves.h"

#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"

using namespace std;

class DcaxyQAer {
public:
	// Structors
	DcaxyQAer(int energy);
	~DcaxyQAer();

	// Getters

	// Setters
	void set_energy(int energy);
	void set_in_path(string path);

	// Doers
	void run_qa();

private:
	// Attributes
	int energy;
	string in_path = "/media/dylan/SSD_Storage/Research/Trees/";
	string out_path = "/home/dylan/Research/Dca_xy_QA/";
	TFile *out_file;
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;
	string tree_name = "tree";
	StRefMultCorr *refmultCorrUtil;

	// Data Containers
	map<int, map<int, pair<float, float>>> dca_event;  // [run][event_id]{dca_xy_event_avg, dca_xy_event_sd}
	map<int, vector<float>> dca_run;  // [run]{dca_xy_run_count, dca_xy_run_sum, dca_xy_run_2nd_raw_moment}  std::vector<> default value initialized to 0

	// Doers
	void read_trees();
	void read_tree(TTree *tree);
	void set_branches(TTree *tree);
	void analyze_runs();
};


#endif /* DCAXYQAER_H_ */
