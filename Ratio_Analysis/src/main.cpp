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
Measure median(vector<Measure> data);


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
	map<string, vector<string>> names = {{"stat",stat_names}, {"cumulant",cumulant_names}, {"raw_moment",raw_moment_names}, {"central_moment",central_moment_names}};
	vector<int> centralities = {8, 7, 4, 1};
	vector<int> all_centralities = {8,7,6,5,4,3,2,1,0};
	string in_path = "/home/dylan/Research/Data/";
	string in_mix_path = "/home/dylan/Research/Data_Mix/";
	vector<int> energy_list = {7,11,19,27,39,62};
	vector<int> divs = {2,3,4,5,6};
	vector<int> orders = {1,2,3,4};
	string out_path = "/home/dylan/Research/Results/";
	string out_root_name = "12-11-19_pu001.root";
	bool plot_dists = false;

//	map<string, vector<int>> sets = {{"Rand_Rotate", {0, 29}}, {"No_Rotate", {0, 9}}, {"EP_Rotate", {0, 9}}, {"Efficiency_01_", {0, 6}}, {"Efficiency_08_", {0, 6}}, {"Efficiency_025_", {0, 6}}, {"Efficiency_05_", {0, 6}}, {"Pile_Up_001_", {0, 6}}, {"Pile_Up_01_", {0, 6}}, {"Pile_Up_002_", {0, 6}}, {"Pile_Up_005_", {0, 6}}, {"Pile_Up_008_", {0, 6}}};
//	map<string, vector<int>> sets = {{"Rand_Rotate", {0, 29}}, {"No_Rotate", {0, 9}}, {"EP_Rotate", {0, 9}}, {"Efficiency_01_", {0, 6}}, {"Efficiency_08_", {0, 6}}, {"Efficiency_025_", {0, 6}}, {"Efficiency_05_", {0, 6}}, {"Pile_Up_001_", {0, 6}}, {"Pile_Up_01_", {0, 6}}, {"Pile_Up_002_", {0, 6}}, {"Pile_Up_005_", {0, 6}}, {"Pile_Up_008_", {0, 6}}};
	map<string, vector<int>> sets = {{"Rand_Rotate", {0, 50}}, {"No_Rotate", {0, 8}}, {"EP_Rotate", {0, 8}}, {"Efficiency_01_", {0, 8}}, {"Efficiency_08_", {0, 8}}, {"Efficiency_025_", {0, 8}}, {"Efficiency_05_", {0, 8}}, {"Pile_Up_001_", {0, 8}}, {"Pile_Up_01_", {0, 8}}, {"Pile_Up_002_", {0, 8}}, {"Pile_Up_005_", {0, 8}}, {"Pile_Up_008_", {0, 8}}};
//	map<string, vector<int>> sets = {{"Rand_Rotate", {0,50}}};


	TFile *out_root = new TFile((out_path+out_root_name).data(), "RECREATE");

	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> raw_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> mix_stats_sets;
	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> divide_stats_sets;

	map<string, map<int, map<int, map<int, map<string, double>>>>> raw_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> mix_stats_sd;
	map<string, map<int, map<int, map<int, map<string, double>>>>> divide_stats_sd;

	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_stats_median;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> mix_stats_median;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> divide_stats_median;

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
			if(plot_dists) {
				make_ratio_dist_plots(data_dir, data);
				make_2d_dist_plots(data_dir, data);
				make_proton_dist_plots(data_dir, data);
			}
			data_dir->cd();

			{
				TDirectory *roli_thesis_dir = data_dir->mkdir("roli_thesis");
				for(pair<string, vector<string>> name:names) {
					TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
					name_dir->cd();
					for(int div:divs) {
						roli_thesis_stats(stats, name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
					}
				}
				TDirectory *centralities_dir = data_dir->mkdir("centralities");
				for(string name:stat_names) {
					TDirectory *name_dir = centralities_dir->mkdir(name.data());
					name_dir->cd();
					for(int div:divs) {
						centralities_stat(stats, name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
					}
				}
			}


			TDirectory *mix_dir = set_num_dir->mkdir("Mix_Data");
			mix_dir->cd();
			if(plot_dists) {
				make_ratio_dist_plots(mix_dir, data_mix);
				make_2d_dist_plots(mix_dir, data_mix);
				make_proton_dist_plots(mix_dir, data_mix);
			}
			mix_dir->cd();

			{
				TDirectory *roli_thesis_dir = mix_dir->mkdir("roli_thesis");
				for(pair<string, vector<string>> name:names) {
					TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
					name_dir->cd();
					for(int div:divs) {
						roli_thesis_stats(stats_mix, name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
					}
				}
				TDirectory *centralities_dir = mix_dir->mkdir("centralities");
				for(string name:stat_names) {
					TDirectory *name_dir = centralities_dir->mkdir(name.data());
					name_dir->cd();
					for(int div:divs) {
						centralities_stat(stats_mix, name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
					}
				}
			}

			TDirectory *mix_div_dir = set_num_dir->mkdir("Mix_Divded_Data");
			mix_div_dir->cd();

			{
				TDirectory *roli_thesis_dir = mix_div_dir->mkdir("roli_thesis");
				for(pair<string, vector<string>> name:names) {
					TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
					name_dir->cd();
					for(int div:divs) {
						roli_thesis_stats(stats_mix_divide, name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
					}
				}
				TDirectory *centralities_dir = mix_div_dir->mkdir("centralities");
				for(string name:stat_names) {
					TDirectory *name_dir = centralities_dir->mkdir(name.data());
					name_dir->cd();
					for(int div:divs) {
						centralities_stat(stats_mix_divide, name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
					}
				}
			}

		}

		cout << "Combining set_nums" << endl;

		map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_stats_all;
		map<string, map<int, map<int, map<int, map<string, Measure>>>>> mix_stats_all;
		map<string, map<int, map<int, map<int, map<string, Measure>>>>> divide_stats_all;

		// Calculate standard deviations for systematics
		for(pair<int, map<int, map<int, map<string, vector<Measure>>>>> energy:raw_stats_sets[set.first]) {
			for(pair<int, map<int, map<string, vector<Measure>>>> div:energy.second) {
				for(pair<int, map<string, vector<Measure>>> cent:div.second) {
					for(pair<string, vector<Measure>> stat:cent.second) {
						raw_stats_sd[set.first][energy.first][div.first][cent.first][stat.first] = sample_sd(raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first]);
						mix_stats_sd[set.first][energy.first][div.first][cent.first][stat.first] = sample_sd(mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first]);
						divide_stats_sd[set.first][energy.first][div.first][cent.first][stat.first] = sample_sd(divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first]);

						raw_stats_median[set.first][energy.first][div.first][cent.first][stat.first] = median(raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first]);
						mix_stats_median[set.first][energy.first][div.first][cent.first][stat.first] = median(mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first]);
						divide_stats_median[set.first][energy.first][div.first][cent.first][stat.first] = median(divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first]);

						for(int i=0; i<(int)raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].size(); i++) {
							raw_stats_all[set.first+to_string(i)][energy.first][div.first][cent.first][stat.first] = raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first][i];
							mix_stats_all[set.first+to_string(i)][energy.first][div.first][cent.first][stat.first] = mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first][i];
							divide_stats_all[set.first+to_string(i)][energy.first][div.first][cent.first][stat.first] = divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first][i];
						}
					}
				}
			}
		}


		TDirectory *comb_dir = set_dir->mkdir("Combined");

		TDirectory *comp_dir = comb_dir->mkdir("Comparison");
		comp_dir->cd();
		map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mixed = {{"raw",raw_stats_median[set.first]}, {"mixed",mix_stats_median[set.first]}};
		map<string, map<int, map<int, map<int, map<string, double>>>>> raw_mixed_sd = {{"raw",raw_stats_sd[set.first]}, {"mixed",mix_stats_sd[set.first]}};

		{
			TDirectory *roli_thesis_dir = comp_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(raw_mixed, raw_mixed_sd, name.second, centralities, {div}, "roli_thesis_raw_mix_comp_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = comp_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(raw_mixed, raw_mixed_sd, name, all_centralities, {div}, "centralities_raw_mix_comp_"+name+to_string(div));
				}
			}
		}


		// All plotting does not work.
		TDirectory *data_dir = comb_dir->mkdir("Raw_Data");
		data_dir->cd();

		{
			TDirectory *median_dir = data_dir->mkdir("Median");

			{
				TDirectory *roli_thesis_dir = median_dir->mkdir("roli_thesis");
				for(pair<string, vector<string>> name:names) {
					TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
					name_dir->cd();
					for(int div:divs) {
						roli_thesis_stats(raw_stats_median[set.first], raw_stats_sd[set.first], name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
					}
				}
				TDirectory *centralities_dir = median_dir->mkdir("centralities");
				for(string name:stat_names) {
					TDirectory *name_dir = centralities_dir->mkdir(name.data());
					name_dir->cd();
					for(int div:divs) {
						centralities_stat(raw_stats_median[set.first], raw_stats_sd[set.first], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
					}
				}
			}

			TDirectory *all_dir = data_dir->mkdir("All");

			{
				TDirectory *roli_thesis_dir = all_dir->mkdir("roli_thesis");
				for(pair<string, vector<string>> name:names) {
					TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
					name_dir->cd();
					for(int div:divs) {
						roli_thesis_stats(raw_stats_all[set.first], name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
					}
				}
				TDirectory *centralities_dir = all_dir->mkdir("centralities");
				for(string name:stat_names) {
					TDirectory *name_dir = centralities_dir->mkdir(name.data());
					name_dir->cd();
					for(int div:divs) {
						centralities_stat(raw_stats_all[set.first], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
					}
				}
			}
		}


		TDirectory *mix_dir = comb_dir->mkdir("Mix_Data");
		mix_dir->cd();

		{
			TDirectory *median_dir = mix_dir->mkdir("Median");

			{
				TDirectory *roli_thesis_dir = median_dir->mkdir("roli_thesis");
				for(pair<string, vector<string>> name:names) {
					TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
					name_dir->cd();
					for(int div:divs) {
						roli_thesis_stats(mix_stats_median[set.first], mix_stats_sd[set.first], name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
					}
				}
				TDirectory *centralities_dir = median_dir->mkdir("centralities");
				for(string name:stat_names) {
					TDirectory *name_dir = centralities_dir->mkdir(name.data());
					name_dir->cd();
					for(int div:divs) {
						centralities_stat(mix_stats_median[set.first], mix_stats_sd[set.first], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
					}
				}
			}

			TDirectory *all_dir = mix_dir->mkdir("All");

			{
				TDirectory *roli_thesis_dir = all_dir->mkdir("roli_thesis");
				for(pair<string, vector<string>> name:names) {
					TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
					name_dir->cd();
					for(int div:divs) {
						roli_thesis_stats(mix_stats_all[set.first], name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
					}
				}
				TDirectory *centralities_dir = all_dir->mkdir("centralities");
				for(string name:stat_names) {
					TDirectory *name_dir = centralities_dir->mkdir(name.data());
					name_dir->cd();
					for(int div:divs) {
						centralities_stat(mix_stats_all[set.first], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
					}
				}
			}
		}


		TDirectory *mix_div_dir = comb_dir->mkdir("Mix_Divded_Data");
		mix_div_dir->cd();

		{
			TDirectory *median_dir = mix_div_dir->mkdir("Median");

			{
				TDirectory *roli_thesis_dir = median_dir->mkdir("roli_thesis");
				for(pair<string, vector<string>> name:names) {
					TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
					name_dir->cd();
					for(int div:divs) {
						roli_thesis_stats(divide_stats_median[set.first], divide_stats_sd[set.first], name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
					}
				}
				TDirectory *centralities_dir = median_dir->mkdir("centralities");
				for(string name:stat_names) {
					TDirectory *name_dir = centralities_dir->mkdir(name.data());
					name_dir->cd();
					for(int div:divs) {
						centralities_stat(divide_stats_median[set.first], divide_stats_sd[set.first], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
					}
				}
			}

			TDirectory *all_dir = mix_div_dir->mkdir("All");

			{
				TDirectory *roli_thesis_dir = all_dir->mkdir("roli_thesis");
				for(pair<string, vector<string>> name:names) {
					TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
					name_dir->cd();
					for(int div:divs) {
						roli_thesis_stats(divide_stats_all[set.first], name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
					}
				}
				TDirectory *centralities_dir = all_dir->mkdir("centralities");
				for(string name:stat_names) {
					TDirectory *name_dir = centralities_dir->mkdir(name.data());
					name_dir->cd();
					for(int div:divs) {
						centralities_stat(divide_stats_all[set.first], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
					}
				}
			}
		}

	}

	cout << "Combining all sets" << endl;

	TDirectory *sets_dir = out_root->mkdir("Sets_Comp");

	map<string, map<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>>> set_pairs = {
			{"Rand_Pile", {
					{"raw" , {{"Rand_Rotate", raw_stats_median["Rand_Rotate"]}, {"Pile_Up 0.1%", raw_stats_median["Pile_Up_001_"]}, {"Pile_Up 1%", raw_stats_median["Pile_Up_01_"]}, {"Pile_Up 0.2%", raw_stats_median["Pile_Up_002_"]}, {"Pile_Up 0.5%", raw_stats_median["Pile_Up_005_"]}, {"Pile_Up 0.8%", raw_stats_median["Pile_Up_008_"]}}},
					{"mix", {{"Rand_Rotate", mix_stats_median["Rand_Rotate"]}, {"Pile_Up 0.1%", mix_stats_median["Pile_Up_001_"]}, {"Pile_Up 1%", mix_stats_median["Pile_Up_01_"]}, {"Pile_Up 0.2%", mix_stats_median["Pile_Up_002_"]}, {"Pile_Up 0.5%", mix_stats_median["Pile_Up_005_"]}, {"Pile_Up 0.8%", mix_stats_median["Pile_Up_008_"]}}},
					{"divide", {{"Rand_Rotate", divide_stats_median["Rand_Rotate"]}, {"Pile_Up 0.1%", divide_stats_median["Pile_Up_001_"]}, {"Pile_Up 1%", divide_stats_median["Pile_Up_01_"]}, {"Pile_Up 0.2%", divide_stats_median["Pile_Up_002_"]}, {"Pile_Up 0.5%", divide_stats_median["Pile_Up_005_"]}, {"Pile_Up 0.8%", divide_stats_median["Pile_Up_008_"]}}}}
			},
			{"Rand_Efficiency", {
					{"raw", {{"Rand_Rotate", raw_stats_median["Rand_Rotate"]}, {"Efficiency 1%", raw_stats_median["Efficiency_01_"]}, {"Efficiency 8%", raw_stats_median["Efficiency_08_"]}, {"Efficiency 2.5%", raw_stats_median["Efficiency_025_"]}, {"Efficiency 5%", raw_stats_median["Efficiency_05_"]}}},
					{"mix", {{"Rand_Rotate", mix_stats_median["Rand_Rotate"]}, {"Efficiency 1%", mix_stats_median["Efficiency_01_"]}, {"Efficiency 8%", mix_stats_median["Efficiency_08_"]}, {"Efficiency 2.5%", mix_stats_median["Efficiency_025_"]}, {"Efficiency 5%", mix_stats_median["Efficiency_05_"]}}},
					{"divide", {{"Rand_Rotate", divide_stats_median["Rand_Rotate"]}, {"Efficiency 1%", divide_stats_median["Efficiency_01_"]}, {"Efficiency 8%", divide_stats_median["Efficiency_08_"]}, {"Efficiency 2.5%", divide_stats_median["Efficiency_025_"]}, {"Efficiency 5%", divide_stats_median["Efficiency_05_"]}}}}
			},
			{"Rotates", {
					{"raw", {{"Rand_Rotate", raw_stats_median["Rand_Rotate"]}, {"No_Rotate", raw_stats_median["No_Rotate"]}, {"EP_Rotate", raw_stats_median["EP_Rotate"]}}},
					{"mix", {{"Rand_Rotate", mix_stats_median["Rand_Rotate"]}, {"No_Rotate", mix_stats_median["No_Rotate"]}, {"EP_Rotate", mix_stats_median["EP_Rotate"]}}},
					{"divide", {{"Rand_Rotate", divide_stats_median["Rand_Rotate"]}, {"No_Rotate", divide_stats_median["No_Rotate"]}, {"EP_Rotate", divide_stats_median["EP_Rotate"]}}}}
			},
			{"All", {{"raw", raw_stats_median}, {"mix", mix_stats_median}, {"divide", divide_stats_median}}}
	};

	map<string, map<string, map<string, map<int, map<int, map<int, map<string, double>>>>>>> set_pairs_sd = {
			{"Rand_Pile", {
					{"raw" , {{"Rand_Rotate", raw_stats_sd["Rand_Rotate"]}, {"Pile_Up 0.1%", raw_stats_sd["Pile_Up_001_"]}, {"Pile_Up 1%", raw_stats_sd["Pile_Up_01_"]}, {"Pile_Up 0.2%", raw_stats_sd["Pile_Up_002_"]}, {"Pile_Up 0.5%", raw_stats_sd["Pile_Up_005_"]}, {"Pile_Up 0.8%", raw_stats_sd["Pile_Up_008_"]}}},
					{"mix", {{"Rand_Rotate", mix_stats_sd["Rand_Rotate"]}, {"Pile_Up 0.1%", mix_stats_sd["Pile_Up_001_"]}, {"Pile_Up 1%", mix_stats_sd["Pile_Up_01_"]}, {"Pile_Up 0.2%", mix_stats_sd["Pile_Up_002_"]}, {"Pile_Up 0.5%", mix_stats_sd["Pile_Up_005_"]}, {"Pile_Up 0.8%", mix_stats_sd["Pile_Up_008_"]}}},
					{"divide", {{"Rand_Rotate", divide_stats_sd["Rand_Rotate"]}, {"Pile_Up 0.1%", divide_stats_sd["Pile_Up_001_"]}, {"Pile_Up 1%", divide_stats_sd["Pile_Up_01_"]}, {"Pile_Up 0.2%", divide_stats_sd["Pile_Up_002_"]}, {"Pile_Up 0.5%", divide_stats_sd["Pile_Up_005_"]}, {"Pile_Up 0.8%", divide_stats_sd["Pile_Up_008_"]}}}}
			},
			{"Rand_Efficiency", {
					{"raw", {{"Rand_Rotate", raw_stats_sd["Rand_Rotate"]}, {"Efficiency 1%", raw_stats_sd["Efficiency_01_"]}, {"Efficiency 8%", raw_stats_sd["Efficiency_08_"]}, {"Efficiency 2.5%", raw_stats_sd["Efficiency_025_"]}, {"Efficiency 5%", raw_stats_sd["Efficiency_05_"]}}},
					{"mix", {{"Rand_Rotate", mix_stats_sd["Rand_Rotate"]}, {"Efficiency 1%", mix_stats_sd["Efficiency_01_"]}, {"Efficiency 8%", mix_stats_sd["Efficiency_08_"]}, {"Efficiency 2.5%", mix_stats_sd["Efficiency_025_"]}, {"Efficiency 5%", mix_stats_sd["Efficiency_05_"]}}},
					{"divide", {{"Rand_Rotate", divide_stats_sd["Rand_Rotate"]}, {"Efficiency 1%", divide_stats_sd["Efficiency_01_"]}, {"Efficiency 8%", divide_stats_sd["Efficiency_08_"]}, {"Efficiency 2.5%", divide_stats_sd["Efficiency_025_"]}, {"Efficiency 5%", divide_stats_sd["Efficiency_05_"]}}}}
			},
			{"Rotates", {
					{"raw", {{"Rand_Rotate", raw_stats_sd["Rand_Rotate"]}, {"No_Rotate", raw_stats_sd["No_Rotate"]}, {"EP_Rotate", raw_stats_sd["EP_Rotate"]}}},
					{"mix", {{"Rand_Rotate", mix_stats_sd["Rand_Rotate"]}, {"No_Rotate", mix_stats_sd["No_Rotate"]}, {"EP_Rotate", mix_stats_sd["EP_Rotate"]}}},
					{"divide", {{"Rand_Rotate", divide_stats_sd["Rand_Rotate"]}, {"No_Rotate", divide_stats_sd["No_Rotate"]}, {"EP_Rotate", divide_stats_sd["EP_Rotate"]}}}}
			},
			{"All", {{"raw", raw_stats_sd}, {"mix", mix_stats_sd}, {"divide", divide_stats_sd}}}
	};

	for(pair<string, map<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>>> set_pair:set_pairs) {
		TDirectory *set_pair_dir = sets_dir->mkdir(set_pair.first.data());

		TDirectory *data_dir = set_pair_dir->mkdir("Raw_Data");
		data_dir->cd();

		{
			TDirectory *roli_thesis_dir = data_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(set_pair.second["raw"], set_pairs_sd[set_pair.first]["raw"], name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = data_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(set_pair.second["raw"], set_pairs_sd[set_pair.first]["raw"], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
				}
			}
		}

		TDirectory *mix_dir = set_pair_dir->mkdir("Mix_Data");
		mix_dir->cd();

		{
			TDirectory *roli_thesis_dir = mix_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(set_pair.second["mix"], set_pairs_sd[set_pair.first]["mix"], name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = mix_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(set_pair.second["mix"], set_pairs_sd[set_pair.first]["mix"], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
				}
			}
		}


		TDirectory *mix_div_dir = set_pair_dir->mkdir("Mix_Divded_Data");
		mix_div_dir->cd();

		{
			TDirectory *roli_thesis_dir = mix_div_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = mix_div_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
				}
			}
		}
	}

//	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_stats_median;
	// Calculate standard deviations for systematics
	for(pair<int, map<int, map<int, map<string, Measure>>>> energy:raw_stats_median["Rand_Rotate"]) {
		for(pair<int, map<int, map<string, Measure>>> div:energy.second) {
			for(pair<int, map<string, Measure>> cent:div.second) {
				for(pair<string, Measure> stat:cent.second) {
					double raw_sd = raw_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
					Measure raw_med = raw_stats_median["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
					Measure raw_eff_med = raw_stats_median["Efficiency_05_"][energy.first][div.first][cent.first][stat.first];
					Measure raw_pile_med = raw_stats_median["Pile_Up_001_"][energy.first][div.first][cent.first][stat.first];
					raw_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first] = pow( pow(raw_sd, 2) + pow(raw_med.get_val() - raw_eff_med.get_val(), 2) + pow(raw_med.get_val() - raw_pile_med.get_val(), 2), 0.5);

					double mix_sd = mix_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
					Measure mix_med = mix_stats_median["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
					Measure mix_eff_med = mix_stats_median["Efficiency_05_"][energy.first][div.first][cent.first][stat.first];
					Measure mix_pile_med = mix_stats_median["Pile_Up_001_"][energy.first][div.first][cent.first][stat.first];
					mix_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first] = pow( pow(mix_sd, 2) + pow(mix_med.get_val() - mix_eff_med.get_val(), 2) + pow(mix_med.get_val() - mix_pile_med.get_val(), 2), 0.5);

					double divide_sd = divide_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
					Measure divide_med = divide_stats_median["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
					Measure divide_eff_med = divide_stats_median["Efficiency_05_"][energy.first][div.first][cent.first][stat.first];
					Measure divide_pile_med = divide_stats_median["Pile_Up_001_"][energy.first][div.first][cent.first][stat.first];
					divide_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first] = pow( pow(divide_sd, 2) + pow(divide_med.get_val() - divide_eff_med.get_val(), 2) + pow(divide_med.get_val() - divide_pile_med.get_val(), 2), 0.5);
				}
			}
		}
	}


	TDirectory *finals_dir = sets_dir->mkdir("Final_Systematics");

	TDirectory *data_dir = finals_dir->mkdir("Raw_Data");
	data_dir->cd();

	{
		TDirectory *roli_thesis_dir = data_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(raw_stats_median["Rand_Rotate"], raw_stats_sd["Rand_Rotate"], name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = data_dir->mkdir("centralities");
		for(string name:stat_names) {
			TDirectory *name_dir = centralities_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				centralities_stat(raw_stats_median["Rand_Rotate"], raw_stats_sd["Rand_Rotate"], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
			}
		}
	}

	TDirectory *mix_dir = finals_dir->mkdir("Mix_Data");
	mix_dir->cd();

	{
		TDirectory *roli_thesis_dir = mix_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(mix_stats_median["Rand_Rotate"], mix_stats_sd["Rand_Rotate"], name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = mix_dir->mkdir("centralities");
		for(string name:stat_names) {
			TDirectory *name_dir = centralities_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				centralities_stat(mix_stats_median["Rand_Rotate"], mix_stats_sd["Rand_Rotate"], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
			}
		}
	}


	TDirectory *mix_div_dir = finals_dir->mkdir("Mix_Divded_Data");
	mix_div_dir->cd();

	{
		TDirectory *roli_thesis_dir = mix_div_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(divide_stats_median["Rand_Rotate"], divide_stats_sd["Rand_Rotate"], name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = mix_div_dir->mkdir("centralities");
		for(string name:stat_names) {
			TDirectory *name_dir = centralities_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				centralities_stat(divide_stats_median["Rand_Rotate"], divide_stats_sd["Rand_Rotate"], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
			}
		}
	}



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


// Get median Measure of vector by value. If even number of Measures takes lower of two median options.
Measure median(vector<Measure> data) {
	sort(data.begin(), data.end(), [] (Measure a, Measure b) { return(a.get_val() < b.get_val()); } );
	return(data[(int)(data.size()-1) / 2]);
}

