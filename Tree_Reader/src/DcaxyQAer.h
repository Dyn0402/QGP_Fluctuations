/*
 * Dca_xy_QAer.h
 *
 *  Created on: Jul 2, 2020
 *      Author: Dylan Neff
 */

#ifndef DCAXYQAER_H_
#define DCAXYQAER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <limits>
#include <mutex>

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
#include <TStyle.h>
#include <TLegend.h>
#include <Math/ProbFuncMathCore.h>
#include <Math/QuantFuncMathCore.h>

#include "file_io.h"
#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"
#include "DcaxyQABase.h"
#include "TreeBranches.h"
#include "Event.h"
#include "Track.h"
#include "TreeCuts.h"
#include "PileUpQAer.h"

using namespace std;


pair<vector<float>, vector<float>> moving_average(const vector<float> &x, const vector<float> &y, int n_point);
float convert_sigmas(float old_sigma, int new_points, int old_points=1);

class DcaxyQAer : public DcaxyQABase {
public:
	// Structors
	DcaxyQAer(int energy);
	DcaxyQAer(int energy, mutex *mtx);
	~DcaxyQAer();

	// Getters

	// Setters
	void set_in_path(string path);

	// Doers
	void run_qa();

private:
	// Attributes
	mutex *mtx = NULL;  // To lock non-thread-safe graphics (TCanvas) processes
	TreeCuts cut;

	string in_path = "/media/ucla/Research/BES1_Trees/";
	TFile *out_file;
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;
	string tree_name = "tree";

	string particle = "proton";
	int ref_num = 3;

	StRefMultCorr *refmultCorrUtil;
	tree_branches branches;

	map<int, int> mv_avg_stats;

	pair<float, float> y_range = {-1.5, 0.3};
	int n_plot_points = 5000;

	// Data Containers
	map<int, map<int, pair<float, float>>> dca_event;  // [run][event_id]{dca_xy_event_avg, dca_xy_event_err}
	map<int, vector<double>> dca_run;  // [run]{dca_xy_run_count, dca_xy_run_sum, dca_xy_run_2nd_raw_moment}  std::vector<> default value initialized to 0
	map<int, map<int, int>> protons;  // [run][event_id]{number of protons}
	map<int, map<int, int>> centralities;  // [run][event_id]{event centrality}

	double set_avg = 0;
	double set_err = 0;
	int set_count = 0;

	// Doers
	void read_trees();
	void read_tree(TTree *tree);
	void set_branches(TTree *tree);
	void analyze_runs();
	void make_proton_plots();

	bool check_bad_run(float run_avg);

	vector<pair<int, int>> get_npt_bad_ranges(const pair<vector<float>, vector<float>> &mv_avg, TF1 &lin_fit, float sigma, float sigma_thresh, float sigma_relax);
	vector<pair<int, int>> combine_ranges(const vector<vector<pair<int, int>>> &ranges);

	void plot_run(vector<float> &run_num, vector<float> &dca_xy_run_avg, vector<float> &dca_xy_run_err, vector<float> &run_indexes);
	void plot_mv_avg(const vector<float> &event_ids, const vector<float> &dca_xy_val, const vector<float> &dca_xy_err, const pair<vector<float>, vector<float>> &mv_avg, const vector<pair<int, int>> &bad_ranges, TF1 &lin_fit, float run_avg, float sigmas_thresh, int mv_avg_num, int run_num);
	void plot_final(const vector<float> &event_ids, const vector<float> &dca_xy_val, const vector<float> &dca_xy_err, const vector<pair<int, int>> &bad_ranges, TF1 &lin_fit, float run_avg, int run_num);

	// Doers ripped from TreeReader
	bool check_event(Event& event);
	bool check_enough_particles(Event& event);
	bool check_good_run(int run);
	bool check_pile_up(int btof_multi, int btof_match, int ref_mult);
	bool check_particle_good(Track& particle);
};


#endif /* DCAXYQAER_H_ */
