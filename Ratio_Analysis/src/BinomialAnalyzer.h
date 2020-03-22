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
	void set_in_path(string in_path);
	void set_in_mix_path(string in_mix_path);
	void set_out_path(string out_path);
	void set_out_root_name(string name);
	void set_energies(vector<int> energies);
	void set_sets(map<string, vector<int>> sets);

	// Doers
	void analyze();
	void analyze_set(string set_name, vector<int> set_nums);
	void analyze_subset(string set_name, int set_num, TDirectory *set_dir);

private:
	// Parameters
	string in_path = "/media/dylan/SSD_Storage/Research/Data_Old_Ref3/";  // "/home/dylan/Research/Data/";
	string in_mix_path = "/media/dylan/SSD_Storage/Research/Data_Old_Ref3_Mix/";  // "/home/dylan/Research/Data_Mix/";
	string out_path = "/home/dylan/Research/Results/";
	string out_root_name = "3-05-20_Binomial_Ref3_partial.root";

	map<string, vector<int>> sets {{"Single_Ratio", {0,0}}};

	vector<int> energy_list = {7,11,19,27,39,62};
	vector<int> divs = {2,3,4,5,6};
	vector<int> centralities = {8,7,6,5,4,3,2,1,0};
	vector<string> stats = {"mean", "standard_deviation"};//, "skewness", "kurtosis", "non_excess_kurtosis"};

	int stat_threads = 1;

	int canvas_width = 2070;
	int canvas_height = 1080;
	double legend_border_width = 0;

	vector<int> div_marker_style = {8, 8, 34, 20, 21, 22, 33};
	vector<int> div_marker_color = {28, 9, 6, 1, 2, 8, 4};
	vector<double> div_marker_size = {1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2};

	map<int, int> energy_marker_style = {{7,24}, {11,25}, {19,26}, {27,27}, {39,28}, {62,30}};
	map<int, int> energy_marker_color = {{7,1}, {11,2}, {19,8}, {27,4}, {39,6}, {62,9}};
	map<int, int> energy_marker_size = {{7,1}, {11,1}, {19,1}, {27,1}, {39,1}, {62,1}};;

	map<int, int> cent_marker_style = {{9,24}, {8,25}, {7,26}, {6,27}, {5,28}, {4,30}, {3,30}, {2,30}, {1,30}, {0,30}};
	map<int, int> cent_marker_color = {{9,1}, {8,2}, {7,11}, {6,4}, {5,46}, {4,6}, {3,7}, {2,8}, {1,9}, {0,28}};
	map<int, int> cent_marker_size = {{9,1}, {8,1}, {7,1}, {6,1}, {5,1}, {4,1}, {3,1}, {2,1}, {1,1}, {0,1}};

	map<int, double> energy_match = {{7,7.7}, {11,11.5}, {19,19.6}, {27,27.0}, {39,39.0}, {62,62.4}};

	map<string, int> raw_mix_marker_style = {{"raw", 20}, {"mix", 20}};
	map<string, int> raw_mix_marker_color = {{"raw", kBlue}, {"mix", kGreen+3}};
	map<string, int> raw_mix_marker_size = {{"raw", 1.2}, {"mix", 1.2}};

	pair<int, int> plot_x_range = {0, 40};
	int min_events = 250;

	// Temporary Parameters
//	string set_name = "Single_Ratio";  // "Rand_Rotate";
//	int set_num = 0;


	// Data Containers
	TFile *out_root;

	// Doers
	void analyze_sets();
	map<int, map<int, map<int, AzimuthBinData>>> get_data(string path, int min_num_events = 1);  // [energy][div][cent]

	map<int, map<int, map<int, map<int, map<string, Measure>>>>> get_slice_stats(map<int, map<int, map<int, AzimuthBinData>>> &data);
	void calc_stat(map<int, int> &slice_data, int protons, int energy, int div, int cent, map<int, map<int, map<int, map<int, map<string, Measure>>>>> *stats);

	map<int, map<int, map<int, map<int, map<string, Measure>>>>> divide_binomial(map<int, map<int, map<int, map<int, map<string, Measure>>>>> &slice_stats);

	// Plotters
	void draw_proton_bin_plots(map<int, map<int, map<int, AzimuthBinData>>> &data, TDirectory *dir);

	void plot_slices(map<int, map<int, map<int, AzimuthBinData>>> &data, TDirectory *dir);
	void plot_all_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, TDirectory *dir);
	void plot_all_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, TDirectory *dir);

	void slice_dist_plot(map<int, int> &slice_data, int total_protons, string name);
	void slice_stats_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name);
	map<string, map<int, TF1*>> slice_stats_divided_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name);
	void plot_fit(map<string, map<int, TF1*>> &fits, int cent, int div, string name);
	pair<int, int> get_canvas_div(int plots);
};


#endif /* BINOMIALANALYZER_H_ */
