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

#include "config_analysis.h"
#include "plotting.h"
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
	void set_out_root_name(string name);

	// Doers
	void analyze();

private:
	// Parameters
	string in_path = "/home/dylan/Research/Data/";
	string in_mix_path = "/home/dylan/Research/Data_Mix/";
	string out_path = "/home/dylan/Research/Results/";
	string out_root_name = "1-25-20_pull_test.root";

	vector<int> energy_list = {7,11,19,27,39,62};
	vector<int> divs = {2,3,4,5,6};
	vector<int> centralities = {8,7,6,5,4,3,2,1,0};

	int stat_threads = 1;

	// Temporary Parameters
	string set_name = "Single_Ratio";
	int set_num = 0;

	// Data Containers
	TFile *out_root;

	// Doers
	void analyze_subset(string set_name, int set_num, TDirectory *set_dir);
	map<int, map<int, map<int, AzimuthBinData>>> get_data(string path, int min_num_events = 1);  // [energy][div][cent]

	map<int, map<int, map<int, map<int, map<string, Measure>>>>> get_slice_stats(map<int, map<int, map<int, AzimuthBinData>>> data);
	void calc_stat(map<int, int> slice_data, int energy, int div, int cent, map<int, map<int, map<int, map<int, map<string, Measure>>>>> *stats);

	void draw_proton_bin_plots(map<int, map<int, map<int, AzimuthBinData>>> &data);

	// Plotters
	void slice_stats_plot(map<int, map<int, map<int, map<int, map<string, Measure>>>>> slice_stats)
};


#endif /* BINOMIALANALYZER_H_ */
