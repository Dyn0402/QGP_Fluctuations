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
#include <thread>
#include <limits>

#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TF2.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLine.h>
#include <TSystem.h>
#include <TLegend.h>
#include <Math/ProbFuncMathCore.h>
#include <Math/QuantFuncMathCore.h>

#include "file_io.h"
#include "TreeLeaves.h"

#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"

using namespace std;

pair<vector<float>, vector<float>> moving_average(const vector<float> &x, const vector<float> &y, int n_point);
float convert_sigmas(float old_sigma, int new_points, int old_points=1);

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
	string in_path = "/media/ucla/Research/BES1_Trees/";
	string out_path = "/home/dylan/Research/Dca_xy_QA/";
	TFile *out_file;
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;
	string tree_name = "tree";
	StRefMultCorr *refmultCorrUtil;

	map<int, pair<float, float>> mv_avg_pars {{1, {5.0, 0.2}}, {5, {5.5, 2.0}}, {10, {8.0, 4.0}}};//, {20, {9.5, 1.0}}, {30, {12.0, 1.5}}};
	map<int, int> mv_avg_stats;
	float n_sigma_fit = 2.0;
	int cent_min = 6;
	int range_comb_tol = 5000;
	int min_points_fit = 4;
	pair<float, float> y_range {-1.5, 0.3};

	// Data Containers
	map<int, map<int, pair<float, float>>> dca_event;  // [run][event_id]{dca_xy_event_avg, dca_xy_event_sd}
	map<int, vector<double>> dca_run;  // [run]{dca_xy_run_count, dca_xy_run_sum, dca_xy_run_2nd_raw_moment}  std::vector<> default value initialized to 0
	map<int, map<int, int>> protons;  // [run][event_id]{number of protons}

	// Doers
	void read_trees();
	void read_tree(TTree *tree);
	void set_branches(TTree *tree);
	void analyze_runs();
	vector<pair<int, int>> get_npt_bad_ranges(const pair<vector<float>, vector<float>> &mv_avg, TF1 &lin_fit, float sigma, float sigma_thresh, float sigma_relax);
	vector<pair<int, int>> combine_ranges(const vector<vector<pair<int, int>>> &ranges);
	void plot_mv_avg(const vector<float> &event_ids, const vector<float> &dca_xy_val, const vector<float> &dca_xy_err, const pair<vector<float>, vector<float>> &mv_avg, const vector<pair<int, int>> &bad_ranges, TF1 &lin_fit, float run_avg, float set_avg, float set_err, float sigmas_thresh, int mv_avg_num, int run_num);
	void plot_final(const vector<float> &event_ids, const vector<float> &dca_xy_val, const vector<float> &dca_xy_err, const vector<pair<int, int>> &bad_ranges, TF1 &lin_fit, float run_avg, float set_avg, float set_err, int run_num);
};


#endif /* DCAXYQAER_H_ */
