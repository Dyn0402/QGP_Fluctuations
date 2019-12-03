/*
 * main.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: Dylan Neff
 */


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
#include "ThreadPool.h"
#include "RatioData.h"

using namespace std;


void analyze_no_CBWC();
map<int, map<int, map<int, map<string, Measure>>>> calculate_stats(map<int, map<int, map<int, RatioData>>> data, vector<int> orders);
void calc_stat(RatioData *data, int energy, int div, int cent, vector<int> orders, map<int, map<int, map<int, map<string, Measure>>>> *stats);
double sample_sd(vector<double> data);
double sample_sd(vector<Measure> data);


int main() {
	analyze_no_CBWC();

	cout << "donzo" << endl;
	return(0);
}



void analyze_no_CBWC() {
	vector<string> stat_names = {"standard_deviation", "skewness", "non_excess_kurtosis"};
	vector<string> cumulant_names = {"cumulant 2", "cumulant 3", "cumulant 4"};
	vector<string> raw_moment_names = {"raw moment 2", "raw moment 3", "raw moment 4"};
	vector<string> central_moment_names = {"central moment 2", "central moment 3", "central moment 4"};
	vector<int> centralities = {8, 7, 4, 1};
	string in_path = "/home/dylan/Research/Data/";
	string in_mix_path = "/home/dylan/Research/Data_Mix/";
	vector<int> energy_list = {7,11,19,27,39,62};
	vector<int> divs = {2,3,4,5,6};
	vector<int> orders = {1,2,3,4};
	string out_path = "/home/dylan/Research/Results/";
	string out_root_name = "12-02-19_sets_comp2.root";

	map<string, vector<int>> sets = {{"Rand_Rotate", {0, 24, 0}}, {"No_Rotate", {0, 9, 0}}, {"EP_Rotate", {0, 9, 0}}, {"Efficiency_05_", {0, 9, 0}}, {"Pile_Up_05_", {0, 9, 0}}};

	TFile *out_root = new TFile((out_path+out_root_name).data(), "RECREATE");

	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> raw_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> mix_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> divide_stats_sets;

	map<string, map<int, map<int, map<int, map<string, double>>>>> raw_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> mix_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> divide_stats_sd;

	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_stats_single;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> mix_stats_single;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> divide_stats_single;

	for(pair<string, vector<int>> set:sets) {

		TDirectory *set_dir = out_root->mkdir(set.first.data());

		for(int set_num = set.second[0]; set_num <= set.second[1]; set_num++) {
			cout << "Starting Set " + set.first + to_string(set_num) << endl << endl;
			map<int, map<int, map<int, RatioData>>> data;
			map<int, map<int, map<int, RatioData>>> data_mix;

			int min_num_events = 1;

			int energy_num = 1;
			for(int energy:energy_list) {

				string path = in_path + set.first + to_string(set_num) + "/" + to_string(energy) + "GeV/";
				string path_mix = in_mix_path + set.first + to_string(set_num) + "/" + to_string(energy) + "GeV/";

				for(int div:divs) {

					// Get centralities found in file names of given directory path.
					for(int cent:get_centrals(path, div)) {
						RatioData ratios(div);
						ratios.read_ratios_from_dir(path, div, cent);  // Read ratio data from file path

						if(ratios.get_num_ratios() / div <= min_num_events) {
							if(div == divs[0]) {
								cout << "Raw centrality " << cent << " with only " << ratios.get_num_ratios() / div << " events. Skipping." << endl;
							}
						} else {
							data[energy][div][cent] = ratios;  // Store ratio data in data under corresponding centrality (refmult2) value
						}

					}

					// Get centralities found in file names of given directory path.
					for(int cent:get_centrals(path_mix, div)) {
						RatioData ratios(div);
						ratios.read_ratios_from_dir(path_mix, div, cent);  // Read ratio data from file path

						if(ratios.get_num_ratios() / div <= min_num_events) {
							cout << "Mixed centrality " << cent << " with only " << ratios.get_num_ratios() / div << " events. Skipping." << endl;
						} else {
							data_mix[energy][div][cent] = ratios;  // Store ratio data in data_mix under corresponding centrality (refmult2) value
						}

					}
				}
				energy_num++;
			}

			auto stats = calculate_stats(data, orders);
			auto stats_mix = calculate_stats(data_mix, orders);

			map<int, map<int, map<int, map<string, Measure>>>> stats_mix_divide;
			map<int, map<int, map<int, map<string, Measure>>>> stats_mix_reldiff;

			for(pair<int, map<int, map<int, map<string, Measure>>>> energy:stats) {
				for(pair<int, map<int, map<string, Measure>>> div:energy.second) {
					for(pair<int, map<string, Measure>> cent:div.second) {
						for(pair<string, Measure> stat:cent.second) {
							stats_mix_divide[energy.first][div.first][cent.first][stat.first] = stat.second / stats_mix[energy.first][div.first][cent.first][stat.first];

							raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].push_back(stat.second);
							mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].push_back(stats_mix[energy.first][div.first][cent.first][stat.first]);
							divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].push_back(stats_mix_divide[energy.first][div.first][cent.first][stat.first]);
						}
					}
				}
			}

			TDirectory *set_num_dir = set_dir->mkdir((set.first + to_string(set_num)).data());

			TDirectory *data_dir = set_num_dir->mkdir("Raw_Data");
			data_dir->cd();
			make_ratio_dist_plots(data_dir, data);
			make_2d_dist_plots(data_dir, data);
			make_proton_dist_plots(data_dir, data);
			data_dir->cd();
			roli_thesis_stats(stats, stat_names, centralities, {2,3,4,5,6}, "roli_thesis_stats");
			roli_thesis_stats(stats, cumulant_names, centralities, {2,3,4,5,6}, "roli_thesis_cumulants");
			roli_thesis_stats(stats, raw_moment_names, centralities, {2,3,4,5,6}, "roli_thesis_raw_moments");
			roli_thesis_stats(stats, central_moment_names, centralities, {2,3,4,5,6}, "roli_thesis_central_moments");


			TDirectory *mix_dir = set_num_dir->mkdir("Mix_Data");
			mix_dir->cd();
			make_ratio_dist_plots(mix_dir, data_mix);
			make_2d_dist_plots(mix_dir, data_mix);
			make_proton_dist_plots(mix_dir, data_mix);
			mix_dir->cd();
			roli_thesis_stats(stats_mix, stat_names, centralities, {2,3,4,5,6}, "roli_thesis_stats");
			roli_thesis_stats(stats_mix, cumulant_names, centralities, {2,3,4,5,6}, "roli_thesis_cumulants");
			roli_thesis_stats(stats_mix, raw_moment_names, centralities, {2,3,4,5,6}, "roli_thesis_raw_moments");
			roli_thesis_stats(stats_mix, central_moment_names, centralities, {2,3,4,5,6}, "roli_thesis_central_moments");



			TDirectory *mix_div_dir = set_num_dir->mkdir("Mix_Divded_Data");
			mix_div_dir->cd();
			roli_thesis_stats(stats_mix_divide, stat_names, centralities, {3,4,5,6}, "roli_thesis_stats_mix_divide");
			roli_thesis_stats(stats_mix_divide, stat_names, centralities, {2}, "roli_thesis_stats_mix_divide2");
			roli_thesis_stats(stats_mix_divide, stat_names, centralities, {3}, "roli_thesis_stats_mix_divide3");
			roli_thesis_stats(stats_mix_divide, stat_names, centralities, {4}, "roli_thesis_stats_mix_divide4");
			roli_thesis_stats(stats_mix_divide, stat_names, centralities, {5}, "roli_thesis_stats_mix_divide5");
			roli_thesis_stats(stats_mix_divide, stat_names, centralities, {6}, "roli_thesis_stats_mix_divide6");
			roli_thesis_stats(stats_mix_divide, cumulant_names, centralities, {3,4,5,6}, "roli_thesis_cumulants_mix_divide");
			roli_thesis_stats(stats_mix_divide, cumulant_names, centralities, {2}, "roli_thesis_cumulants_mix_divide2");
			roli_thesis_stats(stats_mix_divide, cumulant_names, centralities, {3}, "roli_thesis_cumulants_mix_divide3");
			roli_thesis_stats(stats_mix_divide, cumulant_names, centralities, {4}, "roli_thesis_cumulants_mix_divide4");
			roli_thesis_stats(stats_mix_divide, cumulant_names, centralities, {5}, "roli_thesis_cumulants_mix_divide5");
			roli_thesis_stats(stats_mix_divide, cumulant_names, centralities, {6}, "roli_thesis_cumulants_mix_divide6");
			roli_thesis_stats(stats_mix_divide, raw_moment_names, centralities, {3,4,5,6}, "roli_thesis_raw_moments_mix_divide");
			roli_thesis_stats(stats_mix_divide, raw_moment_names, centralities, {2}, "roli_thesis_raw_moments_mix_divide2");
			roli_thesis_stats(stats_mix_divide, raw_moment_names, centralities, {3}, "roli_thesis_raw_moments_mix_divide3");
			roli_thesis_stats(stats_mix_divide, raw_moment_names, centralities, {4}, "roli_thesis_raw_moments_mix_divide4");
			roli_thesis_stats(stats_mix_divide, raw_moment_names, centralities, {5}, "roli_thesis_raw_moments_mix_divide5");
			roli_thesis_stats(stats_mix_divide, raw_moment_names, centralities, {6}, "roli_thesis_raw_moments_mix_divide6");
			roli_thesis_stats(stats_mix_divide, central_moment_names, centralities, {3,4,5,6}, "roli_thesis_central_moments_mix_divide");
			roli_thesis_stats(stats_mix_divide, central_moment_names, centralities, {2}, "roli_thesis_central_moments_mix_divide2");
			roli_thesis_stats(stats_mix_divide, central_moment_names, centralities, {3}, "roli_thesis_central_moments_mix_divide3");
			roli_thesis_stats(stats_mix_divide, central_moment_names, centralities, {4}, "roli_thesis_central_moments_mix_divide4");
			roli_thesis_stats(stats_mix_divide, central_moment_names, centralities, {5}, "roli_thesis_central_moments_mix_divide5");
			roli_thesis_stats(stats_mix_divide, central_moment_names, centralities, {6}, "roli_thesis_central_moments_mix_divide6");
		}

		cout << "Combining set_nums" << endl;

		// Calculate standard deviations for systematics
		for(pair<int, map<int, map<int, map<string, vector<Measure>>>>> energy:raw_stats_sets[set.first]) {
			for(pair<int, map<int, map<string, vector<Measure>>>> div:energy.second) {
				for(pair<int, map<string, vector<Measure>>> cent:div.second) {
					for(pair<string, vector<Measure>> stat:cent.second) {
						raw_stats_sd[set.first][energy.first][div.first][cent.first][stat.first] = sample_sd(raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first]);
						mix_stats_sd[set.first][energy.first][div.first][cent.first][stat.first] = sample_sd(mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first]);
						divide_stats_sd[set.first][energy.first][div.first][cent.first][stat.first] = sample_sd(divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first]);
						raw_stats_single[set.first][energy.first][div.first][cent.first][stat.first] = raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first][set.second[2]];
						mix_stats_single[set.first][energy.first][div.first][cent.first][stat.first] = mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first][set.second[2]];
						divide_stats_single[set.first][energy.first][div.first][cent.first][stat.first] = divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first][set.second[2]];
					}
				}
			}
		}


		TDirectory *comb_dir = set_dir->mkdir("Combined");

		TDirectory *comp_dir = comb_dir->mkdir("Comparison");
		comp_dir->cd();
		map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mixed = {{"raw",raw_stats_single[set.first]}, {"mixed",mix_stats_single[set.first]}};
		map<string, map<int, map<int, map<int, map<string, double>>>>> raw_mixed_sys = {{"raw",raw_stats_sd[set.first]}, {"mixed",mix_stats_sd[set.first]}};
		roli_thesis_stats(raw_mixed, raw_mixed_sys, stat_names, centralities, {2}, "roli_thesis_raw_mix_stat_comp2");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, stat_names, centralities, {3}, "roli_thesis_raw_mix_stat_comp3");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, stat_names, centralities, {4}, "roli_thesis_raw_mix_stat_comp4");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, stat_names, centralities, {5}, "roli_thesis_raw_mix_stat_comp5");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, stat_names, centralities, {6}, "roli_thesis_raw_mix_stat_comp6");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, cumulant_names, centralities, {2}, "roli_thesis_raw_mix_cumulant_comp2");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, cumulant_names, centralities, {3}, "roli_thesis_raw_mix_cumulant_comp3");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, cumulant_names, centralities, {4}, "roli_thesis_raw_mix_cumulant_comp4");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, cumulant_names, centralities, {5}, "roli_thesis_raw_mix_cumulant_comp5");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, cumulant_names, centralities, {6}, "roli_thesis_raw_mix_cumulant_comp6");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, raw_moment_names, centralities, {2}, "roli_thesis_raw_mix_raw_moment_comp2");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, raw_moment_names, centralities, {3}, "roli_thesis_raw_mix_raw_moment_comp3");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, raw_moment_names, centralities, {4}, "roli_thesis_raw_mix_raw_moment_comp4");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, raw_moment_names, centralities, {5}, "roli_thesis_raw_mix_raw_moment_comp5");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, raw_moment_names, centralities, {6}, "roli_thesis_raw_mix_raw_moment_comp6");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, central_moment_names, centralities, {2}, "roli_thesis_raw_mix_central_moment_comp2");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, central_moment_names, centralities, {3}, "roli_thesis_raw_mix_central_moment_comp3");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, central_moment_names, centralities, {4}, "roli_thesis_raw_mix_central_moment_comp4");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, central_moment_names, centralities, {5}, "roli_thesis_raw_mix_central_moment_comp5");
		roli_thesis_stats(raw_mixed, raw_mixed_sys, central_moment_names, centralities, {6}, "roli_thesis_raw_mix_central_moment_comp6");
		centralities_stat(raw_mixed, raw_mixed_sys, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_non_excess_kurtosis6");
		centralities_stat(raw_mixed, raw_mixed_sys, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_non_excess_kurtosis5");
		centralities_stat(raw_mixed, raw_mixed_sys, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_non_excess_kurtosis4");
		centralities_stat(raw_mixed, raw_mixed_sys, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_non_excess_kurtosis3");
		centralities_stat(raw_mixed, raw_mixed_sys, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_non_excess_kurtosis2");
		centralities_stat(raw_mixed, raw_mixed_sys, "skewness", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_skewness6");
		centralities_stat(raw_mixed, raw_mixed_sys, "skewness", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_skewness5");
		centralities_stat(raw_mixed, raw_mixed_sys, "skewness", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_skewness4");
		centralities_stat(raw_mixed, raw_mixed_sys, "skewness", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_skewness3");
		centralities_stat(raw_mixed, raw_mixed_sys, "skewness", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_skewness2");
		centralities_stat(raw_mixed, raw_mixed_sys, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_standard_deviation6");
		centralities_stat(raw_mixed, raw_mixed_sys, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_standard_deviation5");
		centralities_stat(raw_mixed, raw_mixed_sys, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_standard_deviation4");
		centralities_stat(raw_mixed, raw_mixed_sys, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_standard_deviation3");
		centralities_stat(raw_mixed, raw_mixed_sys, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_standard_deviation2");

		TDirectory *data_dir = comb_dir->mkdir("Raw_Data");
		data_dir->cd();
		roli_thesis_stats(raw_stats_single[set.first], raw_stats_sd[set.first], stat_names, centralities, {2,3,4,5,6}, "roli_thesis_stats");
		roli_thesis_stats(raw_stats_single[set.first], raw_stats_sd[set.first], cumulant_names, centralities, {2,3,4,5,6}, "roli_thesis_cumulants");


		TDirectory *mix_dir = comb_dir->mkdir("Mix_Data");
		mix_dir->cd();
		roli_thesis_stats(mix_stats_single[set.first], mix_stats_sd[set.first], stat_names, centralities, {2,3,4,5,6}, "roli_thesis_stats_mix");
		roli_thesis_stats(mix_stats_single[set.first], mix_stats_sd[set.first], cumulant_names, centralities, {2,3,4,5,6}, "roli_thesis_cumulants_mix");


		TDirectory *mix_div_dir = comb_dir->mkdir("Mix_Divded_Data");
		mix_div_dir->cd();
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], stat_names, centralities, {2}, "roli_thesis_raw_mix_stat_divide2");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], stat_names, centralities, {3}, "roli_thesis_raw_mix_stat_divide3");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], stat_names, centralities, {4}, "roli_thesis_raw_mix_stat_divide4");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], stat_names, centralities, {5}, "roli_thesis_raw_mix_stat_divide5");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], stat_names, centralities, {6}, "roli_thesis_raw_mix_stat_divide6");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], cumulant_names, centralities, {2}, "roli_thesis_raw_mix_cumulant_divide2");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], cumulant_names, centralities, {3}, "roli_thesis_raw_mix_cumulant_divide3");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], cumulant_names, centralities, {4}, "roli_thesis_raw_mix_cumulant_divide4");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], cumulant_names, centralities, {5}, "roli_thesis_raw_mix_cumulant_divide5");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], cumulant_names, centralities, {6}, "roli_thesis_raw_mix_cumulant_divide6");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], raw_moment_names, centralities, {2}, "roli_thesis_raw_mix_raw_moment_divide2");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], raw_moment_names, centralities, {3}, "roli_thesis_raw_mix_raw_moment_divide3");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], raw_moment_names, centralities, {4}, "roli_thesis_raw_mix_raw_moment_divide4");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], raw_moment_names, centralities, {5}, "roli_thesis_raw_mix_raw_moment_divide5");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], raw_moment_names, centralities, {6}, "roli_thesis_raw_mix_raw_moment_divide6");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], central_moment_names, centralities, {2}, "roli_thesis_raw_mix_central_moment_divide2");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], central_moment_names, centralities, {3}, "roli_thesis_raw_mix_central_moment_divide3");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], central_moment_names, centralities, {4}, "roli_thesis_raw_mix_central_moment_divide4");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], central_moment_names, centralities, {5}, "roli_thesis_raw_mix_central_moment_divide5");
		roli_thesis_stats(divide_stats_single[set.first], divide_stats_sd[set.first], central_moment_names, centralities, {6}, "roli_thesis_raw_mix_central_moment_divide6");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_non_excess_kurtosis6");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_non_excess_kurtosis5");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_non_excess_kurtosis4");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_non_excess_kurtosis3");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_non_excess_kurtosis2");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "skewness", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_skewness6");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "skewness", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_skewness5");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "skewness", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_skewness4");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "skewness", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_skewness3");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "skewness", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_skewness2");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "standard_deviation", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_standard_deviation6");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "standard_deviation", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_standard_deviation5");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "standard_deviation", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_standard_deviation4");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "standard_deviation", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_standard_deviation3");
		centralities_stat(divide_stats_single[set.first], divide_stats_sd[set.first], "standard_deviation", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_standard_deviation2");
	}

	cout << "Combining all sets" << endl;

	TDirectory *sets_dir = out_root->mkdir("Sets_Comp");

	TDirectory *data_dir = sets_dir->mkdir("Raw_Data");
	data_dir->cd();
	roli_thesis_stats(raw_stats_single, raw_stats_sd, stat_names, centralities, {2,3,4,5,6}, "roli_thesis_stats");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, cumulant_names, centralities, {2,3,4,5,6}, "roli_thesis_cumulants");

	roli_thesis_stats(raw_stats_single, raw_stats_sd, stat_names, centralities, {2}, "roli_thesis_raw_mix_stat_divide2");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, stat_names, centralities, {3}, "roli_thesis_raw_mix_stat_divide3");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, stat_names, centralities, {4}, "roli_thesis_raw_mix_stat_divide4");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, stat_names, centralities, {5}, "roli_thesis_raw_mix_stat_divide5");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, stat_names, centralities, {6}, "roli_thesis_raw_mix_stat_divide6");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, cumulant_names, centralities, {2}, "roli_thesis_raw_mix_cumulant_divide2");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, cumulant_names, centralities, {3}, "roli_thesis_raw_mix_cumulant_divide3");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, cumulant_names, centralities, {4}, "roli_thesis_raw_mix_cumulant_divide4");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, cumulant_names, centralities, {5}, "roli_thesis_raw_mix_cumulant_divide5");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, cumulant_names, centralities, {6}, "roli_thesis_raw_mix_cumulant_divide6");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, raw_moment_names, centralities, {2}, "roli_thesis_raw_mix_raw_moment_divide2");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, raw_moment_names, centralities, {3}, "roli_thesis_raw_mix_raw_moment_divide3");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, raw_moment_names, centralities, {4}, "roli_thesis_raw_mix_raw_moment_divide4");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, raw_moment_names, centralities, {5}, "roli_thesis_raw_mix_raw_moment_divide5");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, raw_moment_names, centralities, {6}, "roli_thesis_raw_mix_raw_moment_divide6");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, central_moment_names, centralities, {2}, "roli_thesis_raw_mix_central_moment_divide2");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, central_moment_names, centralities, {3}, "roli_thesis_raw_mix_central_moment_divide3");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, central_moment_names, centralities, {4}, "roli_thesis_raw_mix_central_moment_divide4");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, central_moment_names, centralities, {5}, "roli_thesis_raw_mix_central_moment_divide5");
	roli_thesis_stats(raw_stats_single, raw_stats_sd, central_moment_names, centralities, {6}, "roli_thesis_raw_mix_central_moment_divide6");
	centralities_stat(raw_stats_single, raw_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_non_excess_kurtosis6");
	centralities_stat(raw_stats_single, raw_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_non_excess_kurtosis5");
	centralities_stat(raw_stats_single, raw_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_non_excess_kurtosis4");
	centralities_stat(raw_stats_single, raw_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_non_excess_kurtosis3");
	centralities_stat(raw_stats_single, raw_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_non_excess_kurtosis2");
	centralities_stat(raw_stats_single, raw_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_skewness6");
	centralities_stat(raw_stats_single, raw_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_skewness5");
	centralities_stat(raw_stats_single, raw_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_skewness4");
	centralities_stat(raw_stats_single, raw_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_skewness3");
	centralities_stat(raw_stats_single, raw_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_skewness2");
	centralities_stat(raw_stats_single, raw_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_standard_deviation6");
	centralities_stat(raw_stats_single, raw_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_standard_deviation5");
	centralities_stat(raw_stats_single, raw_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_standard_deviation4");
	centralities_stat(raw_stats_single, raw_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_standard_deviation3");
	centralities_stat(raw_stats_single, raw_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_standard_deviation2");


	TDirectory *mix_dir = sets_dir->mkdir("Mix_Data");
	mix_dir->cd();
	roli_thesis_stats(mix_stats_single, mix_stats_sd, stat_names, centralities, {2,3,4,5,6}, "roli_thesis_stats_mix");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, cumulant_names, centralities, {2,3,4,5,6}, "roli_thesis_cumulants_mix");

	roli_thesis_stats(mix_stats_single, mix_stats_sd, stat_names, centralities, {2}, "roli_thesis_raw_mix_stat_divide2");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, stat_names, centralities, {3}, "roli_thesis_raw_mix_stat_divide3");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, stat_names, centralities, {4}, "roli_thesis_raw_mix_stat_divide4");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, stat_names, centralities, {5}, "roli_thesis_raw_mix_stat_divide5");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, stat_names, centralities, {6}, "roli_thesis_raw_mix_stat_divide6");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, cumulant_names, centralities, {2}, "roli_thesis_raw_mix_cumulant_divide2");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, cumulant_names, centralities, {3}, "roli_thesis_raw_mix_cumulant_divide3");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, cumulant_names, centralities, {4}, "roli_thesis_raw_mix_cumulant_divide4");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, cumulant_names, centralities, {5}, "roli_thesis_raw_mix_cumulant_divide5");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, cumulant_names, centralities, {6}, "roli_thesis_raw_mix_cumulant_divide6");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, raw_moment_names, centralities, {2}, "roli_thesis_raw_mix_raw_moment_divide2");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, raw_moment_names, centralities, {3}, "roli_thesis_raw_mix_raw_moment_divide3");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, raw_moment_names, centralities, {4}, "roli_thesis_raw_mix_raw_moment_divide4");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, raw_moment_names, centralities, {5}, "roli_thesis_raw_mix_raw_moment_divide5");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, raw_moment_names, centralities, {6}, "roli_thesis_raw_mix_raw_moment_divide6");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, central_moment_names, centralities, {2}, "roli_thesis_raw_mix_central_moment_divide2");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, central_moment_names, centralities, {3}, "roli_thesis_raw_mix_central_moment_divide3");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, central_moment_names, centralities, {4}, "roli_thesis_raw_mix_central_moment_divide4");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, central_moment_names, centralities, {5}, "roli_thesis_raw_mix_central_moment_divide5");
	roli_thesis_stats(mix_stats_single, mix_stats_sd, central_moment_names, centralities, {6}, "roli_thesis_raw_mix_central_moment_divide6");
	centralities_stat(mix_stats_single, mix_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_non_excess_kurtosis6");
	centralities_stat(mix_stats_single, mix_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_non_excess_kurtosis5");
	centralities_stat(mix_stats_single, mix_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_non_excess_kurtosis4");
	centralities_stat(mix_stats_single, mix_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_non_excess_kurtosis3");
	centralities_stat(mix_stats_single, mix_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_non_excess_kurtosis2");
	centralities_stat(mix_stats_single, mix_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_skewness6");
	centralities_stat(mix_stats_single, mix_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_skewness5");
	centralities_stat(mix_stats_single, mix_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_skewness4");
	centralities_stat(mix_stats_single, mix_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_skewness3");
	centralities_stat(mix_stats_single, mix_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_skewness2");
	centralities_stat(mix_stats_single, mix_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_standard_deviation6");
	centralities_stat(mix_stats_single, mix_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_standard_deviation5");
	centralities_stat(mix_stats_single, mix_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_standard_deviation4");
	centralities_stat(mix_stats_single, mix_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_standard_deviation3");
	centralities_stat(mix_stats_single, mix_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_standard_deviation2");


	TDirectory *mix_div_dir = sets_dir->mkdir("Mix_Divded_Data");
	mix_div_dir->cd();
	roli_thesis_stats(divide_stats_single, divide_stats_sd, stat_names, centralities, {2}, "roli_thesis_raw_mix_stat_divide2");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, stat_names, centralities, {3}, "roli_thesis_raw_mix_stat_divide3");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, stat_names, centralities, {4}, "roli_thesis_raw_mix_stat_divide4");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, stat_names, centralities, {5}, "roli_thesis_raw_mix_stat_divide5");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, stat_names, centralities, {6}, "roli_thesis_raw_mix_stat_divide6");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, cumulant_names, centralities, {2}, "roli_thesis_raw_mix_cumulant_divide2");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, cumulant_names, centralities, {3}, "roli_thesis_raw_mix_cumulant_divide3");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, cumulant_names, centralities, {4}, "roli_thesis_raw_mix_cumulant_divide4");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, cumulant_names, centralities, {5}, "roli_thesis_raw_mix_cumulant_divide5");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, cumulant_names, centralities, {6}, "roli_thesis_raw_mix_cumulant_divide6");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, raw_moment_names, centralities, {2}, "roli_thesis_raw_mix_raw_moment_divide2");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, raw_moment_names, centralities, {3}, "roli_thesis_raw_mix_raw_moment_divide3");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, raw_moment_names, centralities, {4}, "roli_thesis_raw_mix_raw_moment_divide4");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, raw_moment_names, centralities, {5}, "roli_thesis_raw_mix_raw_moment_divide5");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, raw_moment_names, centralities, {6}, "roli_thesis_raw_mix_raw_moment_divide6");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, central_moment_names, centralities, {2}, "roli_thesis_raw_mix_central_moment_divide2");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, central_moment_names, centralities, {3}, "roli_thesis_raw_mix_central_moment_divide3");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, central_moment_names, centralities, {4}, "roli_thesis_raw_mix_central_moment_divide4");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, central_moment_names, centralities, {5}, "roli_thesis_raw_mix_central_moment_divide5");
	roli_thesis_stats(divide_stats_single, divide_stats_sd, central_moment_names, centralities, {6}, "roli_thesis_raw_mix_central_moment_divide6");
	centralities_stat(divide_stats_single, divide_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_non_excess_kurtosis6");
	centralities_stat(divide_stats_single, divide_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_non_excess_kurtosis5");
	centralities_stat(divide_stats_single, divide_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_non_excess_kurtosis4");
	centralities_stat(divide_stats_single, divide_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_non_excess_kurtosis3");
	centralities_stat(divide_stats_single, divide_stats_sd, "non_excess_kurtosis", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_non_excess_kurtosis2");
	centralities_stat(divide_stats_single, divide_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_skewness6");
	centralities_stat(divide_stats_single, divide_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_skewness5");
	centralities_stat(divide_stats_single, divide_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_skewness4");
	centralities_stat(divide_stats_single, divide_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_skewness3");
	centralities_stat(divide_stats_single, divide_stats_sd, "skewness", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_skewness2");
	centralities_stat(divide_stats_single, divide_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {6}, "centralities_standard_deviation6");
	centralities_stat(divide_stats_single, divide_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {5}, "centralities_standard_deviation5");
	centralities_stat(divide_stats_single, divide_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {4}, "centralities_standard_deviation4");
	centralities_stat(divide_stats_single, divide_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {3}, "centralities_standard_deviation3");
	centralities_stat(divide_stats_single, divide_stats_sd, "standard_deviation", {8,7,6,5,4,3,2,1,0}, {2}, "centralities_standard_deviation2");

	out_root->Close();
}


//Calculate stats for each cumulant_order for each centrality for each number of divisions for each energy.
map<int, map<int, map<int, map<string, Measure>>>> calculate_stats(map<int, map<int, map<int, RatioData>>> data, vector<int> orders) {
	map<int, map<int, map<int, map<string, Measure>>>> stats;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(1);//thread::hardware_concurrency());
		for(pair<int, map<int, map<int, RatioData>>> energy:data) {
			for(pair<int, map<int, RatioData>> div:energy.second) {
				for(pair<int, RatioData> cent:div.second) {
					pool.enqueue(calc_stat, &(data[energy.first][div.first][cent.first]), energy.first, div.first, cent.first, orders, &stats);
				}
			}
		}
	}

	return(stats);
}


void calc_stat(RatioData *data, int energy, int div, int cent, vector<int> orders, map<int, map<int, map<int, map<string, Measure>>>> *stats) {
	Stats stat(data->get_ratio_hist());
	(*stats)[energy][div][cent]["mean"] = stat.get_mean();
	(*stats)[energy][div][cent]["standard_deviation"] = stat.get_standard_deviation();
	(*stats)[energy][div][cent]["skewness"] = stat.get_skewness();
	(*stats)[energy][div][cent]["kurtosis"] = stat.get_kurtosis();
	(*stats)[energy][div][cent]["non_excess_kurtosis"] = stat.get_non_excess_kurtosis();

	for(int order:orders) {
		(*stats)[energy][div][cent]["cumulant "+to_string(order)] = stat.get_cumulant(order);
		(*stats)[energy][div][cent]["raw moment "+to_string(order)] = stat.get_raw_moment(order);
		(*stats)[energy][div][cent]["central moment "+to_string(order)] = stat.get_central_moment(order);
	}
}


double sample_sd(vector<double> data) {
	double mean = 0.0, sd = 0.0;
	for(double element:data) { mean += element; }
	mean /= (int)data.size();
	for(double element:data) { sd += pow(element - mean, 2); }
	sd = pow(sd / ((int)data.size() - 1), 0.5);

	return(sd);
}


double sample_sd(vector<Measure> data) {
	vector<double> val_vec;
	for(Measure element:data) {
		val_vec.push_back(element.get_val());
	}

	double sd = sample_sd(val_vec);

	return(sd);
}

