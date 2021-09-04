/*
 * AzimuthAnalyzer.h
 *
 *  Created on: Jan 23, 2020
 *      Author: Dylan Neff
 */

#ifndef AZIMUTHBINANALYZER_H_
#define AZIMUTHBINANALYZER_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <map>

#include <TROOT.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TCanvas.h>

#include "config_analysis.h"
#include "plotting.h"
#include "ratio_methods.h"
#include "file_io.h"
#include "Stats.h"
#include "Measure.h"
#include "AzimuthBinData.h"
#include "ThreadPool.h"

using namespace std;

pair<string, int> get_set_name_num(string set);


class AzimuthBinAnalyzer {
public:
	// Structors
	AzimuthBinAnalyzer();
	~AzimuthBinAnalyzer();

	// Getters
	string get_bes_in_path();
	string get_ampt_in_path();
	string get_cf_in_path();
	string get_out_path();

	// Setters
	void set_bes_in_path(string bes_in_path);
	void set_bes_in_mix_path(string bes_in_mix_path);
	void set_ampt_in_path(string ampt_in_path);
	void set_ampt_in_mix_path(string ampt_in_mix_path);
	void set_cf_in_path(string cf_in_path);
	void set_cf_in_mix_path(string cf_in_mix_path);
	void set_sim_in_path(string sim_in_path);
	void set_sim_in_mix_path(string sim_in_mix_path);
	void set_out_path(string path);
	void set_out_root_name(string name);
//	void set_def_set(string set);
	void set_sets(map<string, vector<int>> sets);
	void set_energies(vector<int> energies);
	void set_all_centralities(vector<int> centralities);
	void set_centralities(vector<int> centralities);
	void set_plot_cents(vector<int> centralities);
	void set_stat_names(vector<string> names);
	void set_cumulant_names(vector<string> names);
	void set_central_moment_names(vector<string> names);
	void set_raw_moment_names(vector<string> names);
	void set_can_wh(int can_width, int can_height);
	void set_set_combos(map<string, vector<string>> set_combos);
	void set_sys_combos(map<string, pair<string, vector<string>>> sys_combos);
	void set_free_threads(int free_threads);
	void set_plot_dists(bool plot);
	void set_plot_dist_canvases(bool plot);
	void set_sys_calc(bool calc);
	void set_plot_sys(bool plot);
	void set_write_sys(bool write);
	void set_write_append(bool app);
	void set_divs(vector<int> divisions);

	// Doers
	void analyze();
	void analyze_lite();
	void read_systematics(string path);  // Read in def_medians and def_systematics from file.

protected:
	// Attributes
	vector<string> stat_names = {"standard_deviation", "skewness", "kurtosis", "non_excess_kurtosis", "kurtosis*variance"};
	vector<string> stat_names_rows = {"mean", "standard_deviation", "skewness", "kurtosis", "non_excess_kurtosis", "kurtosis*variance"};
	vector<string> cumulant_names = {"cumulant 2", "cumulant 3", "cumulant 4"};
	vector<string> raw_moment_names = {"raw moment 2", "raw moment 3", "raw moment 4"};
	vector<string> central_moment_names = {"central moment 2", "central moment 3", "central moment 4"};
	vector<string> row_types = {"raw", "mix", "divide"};
	map<string, vector<string>> names;
	vector<int> centralities = {8, 7, 4, 1};
	vector<int> all_centralities = {8,7,6,5,4,3,2,1,0};
	string bes_in_path = "/home/dylan/Research/Data/";
	string bes_in_mix_path = "/home/dylan/Research/Data_Mix/";
	string ampt_in_path = "/home/dylan/Research/Data_Ampt/";
	string ampt_in_mix_path = "/home/dylan/Research/Data_Ampt_Mix/";
	string cf_in_path = "/home/dylan/Research/Data_CF/";
	string cf_in_mix_path = "/home/dylan/Research/Data_CF_Mix/";
	string sim_in_path = "/home/dylan/Research/Data_Sim/";
	string sim_in_mix_path = "/home/dylan/Research/Data_Sim_Mix/";
	vector<int> energy_list {7,11,19,27,39,62};
	vector<int> divs {180, 120, 90, 72, 60};
	vector<int> orders {1,2,3,4};
	vector<int> plot_divs {60};
	vector<int> plot_cents {8,5,1};//{0,1,2,3,4,5,6,7,8,9};
	string out_path = "/home/dylan/Research/Results/";
	string out_root_name = "1-25-20_pull_test.root";
	string out_sys_name = "sys_vals";

	bool plot_dists = true;
	bool plot_dist_canvases = true;
	bool sys_calc = false;
	bool plot_sys = true;  // Plot systematics
	bool write_sys = true;  // Write systmatics values to csv file if true
	bool write_append = false;  // If true and write_sys, append to current file instead of overwriting

	int free_threads = 0;
	map<string, vector<int>> sets {{"Single_Ratio", {0}}};
	map<string, vector<string>> set_combos;
	map<string, pair<string, vector<string>>> sys_combos;  // sys_combos[combo_name]{first.default.set_name, second.systematics[set_names]}


	// Data Containers
	TFile *out_root;

	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> raw_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> mix_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> divide_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> diff_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> pull_raw_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> pull_mix_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> pull_divide_stats_sets;

	map<string, map<int, map<int, map<int, map<string, double>>>>> raw_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> mix_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> divide_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> diff_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> pull_raw_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> pull_mix_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> pull_divide_stats_sd;

	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_stats_median;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> mix_stats_median;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> divide_stats_median;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> diff_stats_median;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> pull_raw_stats_median;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> pull_mix_stats_median;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> pull_divide_stats_median;

	map<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>> def_medians;  // [setgroup_name][data_type][energy][div][cent][stat_name]
	map<string, map<string, map<int, map<int, map<int, map<string, double>>>>>> def_systematics;


	// Doers
	void analyze_sets();
	void analyze_set(string set_name, vector<int> set_nums);
	void analyze_subset(string set_name, int set_num, TDirectory *set_dir);
	void analyze_sets_lite();
	void analyze_set_lite(string set_name, vector<int> set_nums, mutex *mtx);
	void analyze_subset_lite(string set_name, int set_num, mutex *mtx);
	map<int, map<int, map<int, AzimuthBinData>>> get_data(string path, int min_num_events = 1);

	void combine_sets();
	void combine_systematics();
	void combine_set(string set_name, TDirectory *set_dir);
	void combine_set_lite(string set_name);

	void plot_systematics();
	void write_systematics();

	vector<string> get_sets(string set_dir);

	//map<int, map<int, map<int, map<string, Measure>>>> calculate_stats(map<int, map<int, map<int, AzimuthBinData>>> data, string type, vector<int> orders);
	//void calc_stat(AzimuthBinData *data, string type, int energy, int div, int cent, vector<int> orders, map<int, map<int, map<int, map<string, Measure>>>> *stats);
	map<int, map<int, map<int, map<string, Measure>>>> calculate_stats(map<int, map<int, map<int, AzimuthBinData>>> data, string type);
	void calc_stat(AzimuthBinData* data, string type, int energy, int div, int cent, map<int, map<int, map<int, map<string, Measure>>>>* stats);
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> calculate_mix_diff_sds(map<int, map<int, map<int, AzimuthBinData>>> data);
	void calc_mix_diff_sd(AzimuthBinData *data, int energy, int div, int cent, map<int, map<int, map<int, map<int, map<string, Measure>>>>> *stats);
	double sample_sd(vector<double> data);
	double sample_sd(vector<Measure> data);
	double sample_sd_weight(vector<double> data, vector<double> weight);
	double sample_sd_errweight(vector<Measure> data);
	Measure median(vector<Measure> data);
};


#endif /* AZIMUTHBINANALYZER_H_ */
