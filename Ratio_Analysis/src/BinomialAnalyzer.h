/*
 * BinomialAnalyzer.h
 *
 *  Created on: Jan 30, 2020
 *      Author: Dylan Neff
 */

#ifndef BINOMIALANALYZER_H_
#define BINOMIALANALYZER_H_


#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include <TROOT.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>

#include "ratio_methods.h"
#include "file_io.h"
#include "Stats.h"
#include "Measure.h"
#include "AzimuthBinData.h"
#include "ThreadPool.h"

using namespace std;


class BinomialAnalyzer {
public:
	// Structors
	BinomialAnalyzer();
	~BinomialAnalyzer();

	// Getters
	string get_out_root_name();

	// Setters
	void set_bes_in_path(string bes_in_path);
	void set_bes_in_mix_path(string bes_in_mix_path);
	void set_ampt_in_path(string ampt_in_path);
	void set_ampt_in_mix_path(string ampt_in_mix_path);
	void set_sim_in_path(string sim_in_path);
	void set_sim_in_mix_path(string sim_in_mix_path);
	void set_out_path(string out_path);
	void set_out_root_name(string name);
	void set_energies(vector<int> energies);
	void set_divs(vector<int> divs);
	void set_sets(map<string, vector<int>> sets);
	void set_set_combos(map<string, vector<string>> combos);
	void set_centralities(vector<int> centralities);
	void set_can_wh(int can_width, int can_height);

	// Doers
	void analyze();
	void analyze_set(string set_name, vector<int> set_nums);
	void analyze_subset(string set_name, int set_num, TDirectory *set_dir);

private:
	// Parameters
	string bes_in_path = "/home/dylan/Research/Data/";
	string bes_in_mix_path = "/home/dylan/Research/Data_Mix/";
	string ampt_in_path = "/home/dylan/Research/Data_Ampt/";
	string ampt_in_mix_path = "/home/dylan/Research/Data_Ampt_Mix/";
	string sim_in_path = "/home/dylan/Research/Data_Sim/";
	string sim_in_mix_path = "/home/dylan/Research/Data_Sim_Mix/";
	string out_path = "/home/dylan/Research/Results/Azimuth_Analysis/";
	string out_root_name = "3-05-20_Binomial_Ref3_partial.root";

	map<string, vector<int>> sets {{"Single_Ratio", {0,0}}};
	map<string, vector<string>> set_combos;

	vector<int> energy_list {7,11,19,27,39,62};
	vector<int> divs {180, 120, 90, 72, 60};
	vector<int> centralities {8,7,6,5,4,3,2,1,0};
	vector<string> stats {"mean", "standard_deviation", "skewness", "skewness_sd", "kurtosis", "kurtosis_variance"};
	vector<string> stat_ratios {"variance/mean", "sd/mean"};

	int stat_threads = 1;

	int canvas_width = 2070;
	int canvas_height = 1080;
	double legend_border_width = 0;
	double legend_text_size = 0.2;
	double left_margin = 0.15;

	map<int, int> div_marker_style {{0, 8}, {360, 8}, {180, 34}, {120, 20}, {90, 21}, {72, 22}, {60, 33}};
	map<int, int> div_marker_color {{0, 28}, {360, 9}, {180, 6}, {120, 1}, {90, 2}, {72, 8}, {60, 4}};
	map<int, double> div_marker_size {{0, 1.3}, {360, 1.3}, {180, 1.3}, {120, 1.3}, {90, 1.3}, {72, 1.3}, {60, 1.3}};

	map<int, int> energy_marker_style {{7,24}, {11,25}, {19,26}, {27,27}, {39,28}, {62,30}};
	map<int, int> energy_marker_color {{7,1}, {11,2}, {19,8}, {27,4}, {39,6}, {62,9}};
	map<int, int> energy_marker_size {{7,1}, {11,1}, {19,1}, {27,1}, {39,1}, {62,1}};;

	map<int, int> cent_marker_style {{9,24}, {8,25}, {7,26}, {6,27}, {5,28}, {4,30}, {3,30}, {2,30}, {1,30}, {0,30}};
	map<int, int> cent_marker_color {{9,1}, {8,2}, {7,11}, {6,4}, {5,46}, {4,6}, {3,7}, {2,8}, {1,9}, {0,28}};
	map<int, int> cent_marker_size {{9,1}, {8,1}, {7,1}, {6,1}, {5,1}, {4,1}, {3,1}, {2,1}, {1,1}, {0,1}};

	map<int, double> energy_match {{7,7.7}, {11,11.5}, {19,19.6}, {27,27.0}, {39,39.0}, {62,62.4}};

	map<string, int> marker_style {{"raw", 20}, {"mix", 22}, {"raw/mix", 20}, {"eta05_n1ratios_raw", 20}, {"eta05_n1ratios_mix", 20}, {"eta05_n1ratios_raw/mix", 20}, {"Ampt_p+_n1ratios_raw", 20}, {"Ampt_p+_n1ratios_mix", 20}, {"Ampt_p+_n1ratios_raw/mix", 20}};
	map<string, int> marker_color {{"raw", kBlue}, {"mix", kGreen+3}, {"raw/mix", kRed+1}, {"eta05_n1ratios_raw", kBlue}, {"eta05_n1ratios_mix", kGreen+3}, {"eta05_n1ratios_raw/mix", 9}, {"Ampt_p+_n1ratios_raw", kRed+2}, {"Ampt_p+_n1ratios_mix", kViolet}, {"Ampt_p+_n1ratios_raw/mix", 28}};
	map<string, double> marker_size {{"raw", 1.3}, {"mix", 1.3}, {"raw/mix", 1.3}, {"eta05_n1ratios_raw", 1.3}, {"eta05_n1ratios_mix", 1.3}, {"eta05_n1ratios_raw/mix", 1.3}, {"Ampt_p+_n1ratios_raw", 1.3}, {"Ampt_p+_n1ratios_mix", 1.3}, {"Ampt_p+_n1ratios_raw/mix", 1.3}};

	vector<int> marker_style_def {20, 20, 20, 20, 20, 20, 20, 20};
	vector<double> marker_size_def {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	vector<int> marker_color_def {6, 1, 2, kGreen+2, 4, kViolet}; //9,22

	pair<int, int> plot_x_range {0, 40};  // Max val is obselete
	int min_events = 250;

	// Temporary Parameters
//	string set_name = "Single_Ratio";  // "Rand_Rotate";
//	int set_num = 0;


	// Data Containers
	TFile *out_root;
	map<string, map<string, map<string, map<int, map<int, map<int, map<int, map<string, vector<Measure>>>>>>>>> combined_stats;
	map<string, map<string, map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>>>> sys_stats;

	// Doers
	void analyze_sets();
	void combine_sets();
	void combine_subsets(string set_name);
	void plot_set_combo(string set_name, TDirectory *dir);
	void plot_combo_sets(vector<string> set_names, TDirectory *dir);
	map<int, map<int, map<int, AzimuthBinData>>> get_data(string path, int min_num_events = 1);  // [energy][div][cent]

	map<int, map<int, map<int, map<int, map<string, Measure>>>>> get_slice_stats(map<int, map<int, map<int, AzimuthBinData>>> &data);
	void calc_stat(map<int, int> &slice_data, int protons, int energy, int div, int cent, map<int, map<int, map<int, map<int, map<string, Measure>>>>> *stats);

	map<int, map<int, map<int, map<int, map<string, Measure>>>>> divide_binomial(map<int, map<int, map<int, map<int, map<string, Measure>>>>> &slice_stats);
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> divide_mix(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats);
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> get_stat_ratios(map<int, map<int, map<int, map<int, map<string, Measure>>>>> &slice_stats);

	// Plotters
	void draw_proton_bin_plots(map<int, map<int, map<int, AzimuthBinData>>> &data, TDirectory *dir);

	void plot_slices(map<int, map<int, map<int, AzimuthBinData>>> &data, TDirectory *dir);
	void plot_all_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, TDirectory *dir);
	void plot_all_stats(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, TDirectory *dir);
	void plot_all_stat_ratios(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stat_ratios, TDirectory *dir);
	void plot_all_stat_ratios(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stat_ratios, TDirectory *dir);
	void plot_all_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stat_ratios, TDirectory *dir);
	void plot_all_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stat_ratios, TDirectory *dir);
	void plot_mix_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, TDirectory *dir);
	void plot_mix_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, TDirectory *dir);

	void slice_dist_plot(map<int, int> &slice_data, int total_protons, int div, string name);
	void slice_diff_dist_plot(map<double, int> &slice_data, int total_protons, int div, string name);
	void slice_stats_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name);
	void slice_stats_plot(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name);
	void slice_stat_ratios_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stat_ratios, string stat_name, vector<int> &energies, int cent, int div, string name);
	void slice_stat_ratios_plot(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stat_ratios, string stat_name, vector<int> &energies, int cent, int div, string name);
	map<string, map<int, TF1*>> slice_stats_divided_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name);
	map<string, map<int, TF1*>> slice_stats_divided_plot(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name);
	void plot_fit(map<string, map<int, TF1*>> &fits, int cent, int div, string name);
	pair<int, int> get_canvas_div(int plots);

	// Helpers
	int get_marker_style(string key, int alt_key=3);
	double get_marker_size(string key, int alt_key=3);
	int get_marker_color(string key, int alt_key=3);
};


#endif /* BINOMIALANALYZER_H_ */
