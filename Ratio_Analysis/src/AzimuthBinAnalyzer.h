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


class AzimuthBinAnalyzer {
public:
	// Structors
	AzimuthBinAnalyzer();
	~AzimuthBinAnalyzer();

	// Getters

	// Setters
	void set_bes_in_path(string bes_in_path);
	void set_bes_in_mix_path(string bes_in_mix_path);
	void set_ampt_in_path(string ampt_in_path);
	void set_ampt_in_mix_path(string ampt_in_mix_path);
	void set_sim_in_path(string sim_in_path);
	void set_sim_in_mix_path(string sim_in_mix_path);
	void set_out_path(string path);
	void set_out_root_name(string name);
	void set_sets(map<string, vector<int>> sets);
	void set_energies(vector<int> energies);
	void set_all_centralities(vector<int> centralities);
	void set_centralities(vector<int> centralities);
	void set_plot_cents(vector<int> centralities);
	void set_can_wh(int can_width, int can_height);
	void set_set_combos(map<string, vector<string>> set_combos);
	void set_plot_dists(bool plot);
	void set_plot_dist_canvases(bool plot);
	void set_divs(vector<int> divisions);

	// Doers
	void analyze();

private:
	// Parameters
	vector<string> stat_names = {"standard_deviation", "skewness", "kurtosis", "non_excess_kurtosis", "kurtosis*variance"};
	vector<string> stat_names_rows = {"mean", "standard_deviation", "skewness", "kurtosis", "non_excess_kurtosis", "kurtosis*variance"};
	vector<string> cumulant_names = {"cumulant 2", "cumulant 3", "cumulant 4"};
	vector<string> raw_moment_names = {"raw moment 2", "raw moment 3", "raw moment 4"};
	vector<string> central_moment_names = {"central moment 2", "central moment 3", "central moment 4"};
	vector<string> row_types = {"raw", "mix", "divide"};
	map<string, vector<string>> names = {{"stat",stat_names}, {"cumulant",cumulant_names}, {"raw_moment",raw_moment_names}, {"central_moment",central_moment_names}};
	vector<int> centralities = {8, 7, 4, 1};
	vector<int> all_centralities = {8,7,6,5,4,3,2,1,0};
	string bes_in_path = "/home/dylan/Research/Data/";
	string bes_in_mix_path = "/home/dylan/Research/Data_Mix/";
	string ampt_in_path = "/home/dylan/Research/Data_Ampt/";
	string ampt_in_mix_path = "/home/dylan/Research/Data_Ampt_Mix/";
	string sim_in_path = "/home/dylan/Research/Data_Sim/";
	string sim_in_mix_path = "/home/dylan/Research/Data_Sim_Mix/";
	vector<int> energy_list {7,11,19,27,39,62};
	vector<int> divs {180, 120, 90, 72, 60};
	vector<int> orders {1,2,3,4};
	vector<int> plot_divs {60};
	vector<int> plot_cents {8,5,1};//{0,1,2,3,4,5,6,7,8,9};
	string out_path = "/home/dylan/Research/Results/";
	string out_root_name = "1-25-20_pull_test.root";

	bool plot_dists = true;
	bool plot_dist_canvases = true;
//	map<string, vector<int>> sets {{"Rand_Rotate", {0, 29}}, {"No_Rotate", {0, 9}}, {"EP_Rotate", {0, 9}}, {"Efficiency_01_", {0, 6}}, {"Efficiency_08_", {0, 6}}, {"Efficiency_025_", {0, 6}}, {"Efficiency_05_", {0, 6}}, {"Pile_Up_001_", {0, 6}}, {"Pile_Up_01_", {0, 6}}, {"Pile_Up_002_", {0, 6}}, {"Pile_Up_005_", {0, 6}}, {"Pile_Up_008_", {0, 6}}};
//	map<string, vector<int>> sets {{"Rand_Rotate", {0, 29}}, {"No_Rotate", {0, 9}}, {"EP_Rotate", {0, 9}}, {"Efficiency_01_", {0, 6}}, {"Efficiency_08_", {0, 6}}, {"Efficiency_025_", {0, 6}}, {"Efficiency_05_", {0, 6}}, {"Pile_Up_001_", {0, 6}}, {"Pile_Up_01_", {0, 6}}, {"Pile_Up_002_", {0, 6}}, {"Pile_Up_005_", {0, 6}}, {"Pile_Up_008_", {0, 6}}};
//	map<string, vector<int>> sets {{"Rand_Rotate", {0, 50}}, {"No_Rotate", {0, 8}}, {"EP_Rotate", {0, 8}}, {"No_BTof_Rej", {0, 8}}, {"Efficiency_01_", {0, 8}}, {"Efficiency_025_", {0, 8}}, {"Efficiency_05_", {0, 8}}, {"Efficiency_08_", {0, 8}}, {"Pile_Up_0002_", {0, 8}}, {"Pile_Up_0005_", {0, 8}}, {"Pile_Up_0008_", {0, 8}}, {"Pile_Up_001_", {0, 8}}, {"Pile_Up_002_", {0, 8}}, {"Pile_Up_005_", {0, 8}}, {"Pile_Up_008_", {0, 8}}, {"Pile_Up_01_", {0, 8}}};
//	map<string, vector<int>> sets {{"Rand_Rotate", {0,1}}, {"Single_Ratio", {0,1}}};
	map<string, vector<int>> sets {{"Single_Ratio", {0,0}}};
	map<string, vector<string>> set_combos;


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


	// Doers
	void analyze_sets();
	void analyze_set(string set_name, vector<int> set_nums);
	void analyze_subset(string set_name, int set_num, TDirectory *set_dir);
	map<int, map<int, map<int, AzimuthBinData>>> get_data(string path, int min_num_events = 1);

	void combine_sets();
	void combine_set(string set_name, TDirectory *set_dir);

	map<int, map<int, map<int, map<string, Measure>>>> calculate_stats(map<int, map<int, map<int, AzimuthBinData>>> data, string type, vector<int> orders);
	void calc_stat(AzimuthBinData *data, string type, int energy, int div, int cent, vector<int> orders, map<int, map<int, map<int, map<string, Measure>>>> *stats);
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> calculate_mix_diff_sds(map<int, map<int, map<int, AzimuthBinData>>> data);
	void calc_mix_diff_sd(AzimuthBinData *data, int energy, int div, int cent, map<int, map<int, map<int, map<int, map<string, Measure>>>>> *stats);
	double sample_sd(vector<double> data);
	double sample_sd(vector<Measure> data);
	Measure median(vector<Measure> data);
};


#endif /* AZIMUTHBINANALYZER_H_ */
