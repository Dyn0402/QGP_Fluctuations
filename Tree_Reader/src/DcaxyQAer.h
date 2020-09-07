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
#include "TreeBranches.h"
#include "Event.h"
#include "Track.h"
#include "TreeCutsBase.h"
#include "PileUpQAer.h"

using namespace std;


// Not thread safe. Crashes when two threads run analyze_runs() method simultaneously.
// Think it's probably to do with non-unique ROOT object names. Annoying.


pair<vector<float>, vector<float>> moving_average(const vector<float> &x, const vector<float> &y, int n_point);
float convert_sigmas(float old_sigma, int new_points, int old_points=1);

class DcaxyQAer {
public:
	// Structors
	DcaxyQAer(int energy);
	DcaxyQAer(int energy, mutex *mtx);
	~DcaxyQAer();

	// Getters
	vector<int> get_bad_runs();
	map<int, vector<pair<int, int>>> get_bad_ranges();

	// Setters
	void set_energy(int energy);
	void set_in_path(string path);

	// Doers
	void run_qa();

private:
	// Attributes
	mutex *mtx = NULL;  // To lock non-thread-safe graphics (TCanvas) processes
	TreeCutsBase cut;

	int energy;
	string in_path = "/media/ucla/Research/BES1_Trees/";
	string out_path = "/home/dylan/Research/Dca_xy_QA/";
	string out_file_suf = "_Bad_DCA_Events_Ranges.txt";
	TFile *out_file;
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;
	string tree_name = "tree";

	string particle = "proton";
	int ref_num = 3;

	StRefMultCorr *refmultCorrUtil;
	tree_branches branches;

	map<int, pair<float, float>> mv_avg_pars {{1, {5.0, 0.2}}, {5, {5.5, 2.0}}, {10, {8.0, 4.0}}};//, {20, {9.5, 1.0}}, {30, {12.0, 1.5}}};
	map<int, int> mv_avg_stats;
	float n_sigma_fit = 2.0;
	int cent_min = 6;
	int range_comb_tol = 1000;
	int min_points_fit = 4;
	pair<float, float> y_range {-1.5, 0.3};
	map<int, float> bad_run_sigmas {{7, 3.0}, {11, 3.0}, {14, 3.0}, {19, 4.5}, {27, 3.0}, {39, 3.0}, {54, 3.0}, {62, 4.0}, {200, 3.0}};

	int min_fit_entries = 100;
	int min_fit_count = 3;

	int n_plot_points = 5000;

	// Data Containers
	map<int, map<int, pair<float, float>>> dca_event;  // [run][event_id]{dca_xy_event_avg, dca_xy_event_err}
	map<int, vector<double>> dca_run;  // [run]{dca_xy_run_count, dca_xy_run_sum, dca_xy_run_2nd_raw_moment}  std::vector<> default value initialized to 0
	map<int, map<int, int>> protons;  // [run][event_id]{number of protons}
	map<int, map<int, int>> centralities;  // [run][event_id]{event centrality}
	vector<int> bad_runs;
	map<int, vector<pair<int, int>>> bad_ranges;
	double set_avg = 0;
	double set_err = 0;
	int set_count = 0;

	// Doers
	void read_trees();
	void read_tree(TTree *tree);
	void set_branches(TTree *tree);
	void analyze_runs();
	void make_proton_plots();

	void write_bad_dca_file();
	void read_bad_dca_file();

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
