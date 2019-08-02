/*
 * main.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <thread>

#include <TROOT.h>

#include "config_analysis.h"
#include "plotting.h"
#include "ratio_methods.h"
#include "file_io.h"
#include "Stats.h"
#include "Measure.h"
#include "ThreadPool.h"

using namespace std;


void analyze();
void cumulant_test();
pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> calculate_stats(map<int, tree_data> trees); //cumulants[energy][divisions][centrality][cumulant_order]
void calc_stat(map<int, map<int, int>> ratios_map, int energy, int div, int cent, map<int, map<int, map<int, map<string, Measure>>>> *stats, map<int, map<int, map<int, map<int, Measure>>>> *cumulants);


int main() {
	analyze();

	cout << "donzo" << endl;
	return(0);
}


void cumulant_test() {
	int n = 4;
	vector<double> test_data = {-1,0,1,2,3,4,5,6,7,8,9,10,10};
	cout << get_raw_moment(test_data, n) << endl;
	cout << get_central_moment(test_data, n) << endl;
	double val, err;
	tie(val, err) = get_cumulant(test_data, n);
	cout << val << "+-" << err << endl;
}


void analyze() {
	TFile *out_root = new TFile((plot::out_path+plot::out_root_name).data(), "RECREATE");

	map<int, tree_data> trees;

	int tree_num = 1;
	int num_trees = analysis::energy_list.size();
	for(int energy:analysis::energy_list) {
		cout << "Working on " << energy << "GeV. " << tree_num << " of " << num_trees << endl;
		string path = analysis::in_path + to_string(energy) + "GeV/";
		for(int cent:analysis::centrals) {
			map<int, int> nprotons = read_nprotons(path, cent);
			trees[energy].good_protons[cent] = nprotons;
			for(int div:analysis::divs) {
				map<int, map<int, int>> ratios = read_ratios(path, div, cent);
				trees[energy].ratios[div][cent] = ratios;
			}
		}
		tree_num++;
	}

	cout << endl << "Calculating Cumulants..." << endl;
	auto stats = calculate_stats(trees);

	out_root->cd();
	cout << endl << "Making ratio distribution plots..." << endl;
	make_ratio_dist_plots(out_root, trees);
	cout << endl << "Making 2d distribution plots..." << endl;
	make_2d_dist_plots(out_root, trees);
	cout << endl << "Making proton distribution plots..." << endl;
	make_proton_dist_plots(out_root, trees);
	cout << endl << "Making cumulant plots..." << endl;
//	make_cumulant_plots(out_root, stats.second);
//	cout << endl << "Making stat plots..." << endl;
//	make_stat_plots(out_root, stats.first);
//	cout << endl << "Making canvases..." << endl;
//	make_canvas_plots(out_root, trees, stats.second, stats.first);


	out_root->Close();
}



//Calculate cumulants for each cumulant_order for each centrality for each number of divisions for each energy.
map<int, map<int, map<int, map<int, tuple<double, double>>>>> calculate_cumulants(map<int, tree_data> trees) {
	map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants;
	for(int energy:analysis::energy_list) {
		cout << "Calculating " << to_string(energy) << "GeV cumulants" << endl;
		for(int div:analysis::divs) {
			cout << "Calculating " << to_string(div) << " division cumulants" << endl;
			for(int cent:analysis::centrals) {
				for(int order:analysis::cumulant_orders) {
					vector<double> ratios = ratios_map_to_vec(trees[energy].ratios[div][cent]);
					cumulants[energy][div][cent][order] = get_cumulant(ratios, order);
//					cout << " mean: " << get_raw_moment(ratios, 1) <<  " len: " << ratios.size() << " divs: " << div << " cent: " << cent << " order: " << order << " cumulant: " <<  cumulants[energy][div][cent][order] << endl;
				}
			}
		}
	}

	return(cumulants);
}


//Calculate stats for each cumulant_order for each centrality for each number of divisions for each energy.
pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> calculate_stats(map<int, tree_data> trees) {
	map<int, map<int, map<int, map<string, Measure>>>> stats;
	map<int, map<int, map<int, map<int, Measure>>>> cumulants;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(int energy:analysis::energy_list) {
			for(int div:analysis::divs) {
				for(int cent:analysis::centrals) {
					map<int, map<int, int>> ratios_map = trees[energy].ratios[div][cent];
					pool.enqueue(calc_stat, ratios_map, energy, div, cent, &stats, &cumulants);
				}
			}
		}
	}

	return(make_pair(stats, cumulants));
}


void calc_stat(map<int, map<int, int>> ratios_map, int energy, int div, int cent, map<int, map<int, map<int, map<string, Measure>>>> *stats, map<int, map<int, map<int, map<int, Measure>>>> *cumulants) {
	cout << "Starting " << energy << "GeV \t\t" << div << " divisions \t\t" << cent << " centrality" << endl;
	vector<double> ratios = ratios_map_to_vec(ratios_map);
	Stats stat(ratios);
	(*stats)[energy][div][cent]["mean"] = stat.get_mean();
	(*stats)[energy][div][cent]["standard_deviation"] = stat.get_standard_deviation();
	(*stats)[energy][div][cent]["skewness"] = stat.get_skewness();
	(*stats)[energy][div][cent]["kurtosis"] = stat.get_kurtosis();
	for(int order:analysis::cumulant_orders) {
		(*cumulants)[energy][div][cent][order] = stat.get_cumulant(order);
	}
	cout << "Finishing " << energy << "GeV \t" << div << " divisions \t\t" << cent << " centrality" << endl;
}
