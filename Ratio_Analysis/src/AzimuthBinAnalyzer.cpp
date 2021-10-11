/*
 * AzimuthBinAnalyzer.cpp
 *
 *  Created on: Jan 23, 2020
 *      Author: Dylan Neff
 */


#include <vector>
#include <sstream>
#include <iomanip>

#include "AzimuthBinAnalyzer.h"
#include "plotting.h"
#include "ratio_methods.h"
#include "file_io.h"
#include "Stats.h"
#include "Measure.h"
#include "AzimuthBinData.h"
#include "ThreadPool.h"

using namespace std;


// Structors
AzimuthBinAnalyzer::AzimuthBinAnalyzer() {
	out_root = NULL;
}

AzimuthBinAnalyzer::~AzimuthBinAnalyzer() {
	if (out_root != NULL) {
		out_root->Close();
	}
}


// Getters

string AzimuthBinAnalyzer::get_bes_in_path() {
	return this->bes_in_path;
}

string AzimuthBinAnalyzer::get_ampt_in_path() {
	return this->ampt_in_path;
}

string AzimuthBinAnalyzer::get_cf_in_path() {
	return this->cf_in_path;
}

string AzimuthBinAnalyzer::get_sim_in_path() {
	return this->sim_in_path;
}

string AzimuthBinAnalyzer::get_out_path() {
	return this->out_path;
}


// Setters

void AzimuthBinAnalyzer::set_bes_in_path(string bes_in_path) {
	this->bes_in_path = bes_in_path;
}

void AzimuthBinAnalyzer::set_bes_in_mix_path(string bes_in_mix_path) {
	this->bes_in_mix_path = bes_in_mix_path;
}

void AzimuthBinAnalyzer::set_ampt_in_path(string ampt_in_path) {
	this->ampt_in_path = ampt_in_path;
}

void AzimuthBinAnalyzer::set_ampt_in_mix_path(string ampt_in_mix_path) {
	this->ampt_in_mix_path = ampt_in_mix_path;
}

void AzimuthBinAnalyzer::set_cf_in_path(string cf_in_path) {
	this->cf_in_path = cf_in_path;
}

void AzimuthBinAnalyzer::set_cf_in_mix_path(string cf_in_mix_path) {
	this->cf_in_mix_path = cf_in_mix_path;
}

void AzimuthBinAnalyzer::set_sim_in_path(string sim_in_path) {
	this->sim_in_path = sim_in_path;
}

void AzimuthBinAnalyzer::set_sim_in_mix_path(string sim_in_mix_path) {
	this->sim_in_mix_path = sim_in_mix_path;
}

void AzimuthBinAnalyzer::set_out_path(string path) {
	out_path = path;
}

void AzimuthBinAnalyzer::set_out_root_name(string name) {
	out_root_name = name;
}

void AzimuthBinAnalyzer::set_out_sys_name(string name) {
	out_sys_name = name;
}

//void AzimuthBinAnalyzer::set_def_set(string set) {
//	this->def_set = set;
//}

void AzimuthBinAnalyzer::set_sets(map<string, vector<int>> sets) {
	this->sets = sets;
}

void AzimuthBinAnalyzer::set_energies(vector<int> energies) {
	energy_list = energies;
}

void AzimuthBinAnalyzer::set_all_centralities(vector<int> centralities) {
	all_centralities = centralities;
}

void AzimuthBinAnalyzer::set_centralities(vector<int> centralities) {
	this->centralities = centralities;
}

void AzimuthBinAnalyzer::set_plot_cents(vector<int> centralities) {
	plot_cents = centralities;
}

void AzimuthBinAnalyzer::set_stat_names(vector<string> names) {
	stat_names = names;
}

void AzimuthBinAnalyzer::set_cumulant_names(vector<string> names) {
	cumulant_names = names;
}

void AzimuthBinAnalyzer::set_central_moment_names(vector<string> names) {
	central_moment_names = names;
}

void AzimuthBinAnalyzer::set_raw_moment_names(vector<string> names) {
	raw_moment_names = names;
}

void AzimuthBinAnalyzer::set_can_wh(int can_width, int can_height) {
	plot::canvas_width = can_width;
	plot::canvas_height = can_height;
}

void AzimuthBinAnalyzer::set_set_combos(map<string, vector<string>> set_combos) {
	this->set_combos = set_combos;
}

void AzimuthBinAnalyzer::set_sys_combos(map<string, pair<string, vector<string>>> sys_combos) {
	this->sys_combos = sys_combos;
}

void AzimuthBinAnalyzer::set_free_threads(int free_threads) {
	this->free_threads = free_threads;
}

void AzimuthBinAnalyzer::set_plot_dists(bool plot) {
	this->plot_dists = plot;
}

void AzimuthBinAnalyzer::set_plot_dist_canvases(bool plot) {
	this->plot_dist_canvases = plot;
}

void AzimuthBinAnalyzer::set_sys_calc(bool calc) {
	this->sys_calc = calc;
}

void AzimuthBinAnalyzer::set_plot_sys(bool plot) {
	this->plot_sys = plot;
}

void AzimuthBinAnalyzer::set_write_sys(bool write) {
	this->write_sys = write;
}

void AzimuthBinAnalyzer::set_write_append(bool app) {
	this->write_append = app;
}

void AzimuthBinAnalyzer::set_divs(vector<int> divisions) {
	this->divs = divisions;
}


// Doers

void AzimuthBinAnalyzer::analyze() {
	analysis::energy_list = energy_list;
	names = {{"stat",stat_names}, {"cumulant",cumulant_names}, {"raw_moment",raw_moment_names}, {"central_moment",central_moment_names}};
	out_root = new TFile((out_path+out_root_name).data(), "RECREATE");
	analyze_sets();
}


void AzimuthBinAnalyzer::analyze_sets() {
//	ROOT::EnableThreadSafety();
//	{
//		ThreadPool pool(thread::hardware_concurrency());
//		for(pair<string, vector<int>> set:sets) {
//			pool.enqueue(&AzimuthBinAnalyzer::analyze_set, this, set.first, set.second);
//			analyze_set(set.first, set.second);
//		}
//	}

	for (pair<string, vector<int>> set : sets) {
		analyze_set(set.first, set.second);
	}

	combine_sets();
}


void AzimuthBinAnalyzer::analyze_set(string set_name, vector<int> set_nums) {
	string set_name_file = split_string_by_char(set_name, '/').back();
	TDirectory *set_dir = out_root->mkdir(set_name_file.data());
	for(int set_num = set_nums.front(); set_num <= set_nums.back(); set_num++) {
		analyze_subset(set_name, set_num, set_dir);
	}

	combine_set(set_name, set_dir);
}


void AzimuthBinAnalyzer::analyze_lite() {
	names = {{"stat",stat_names}, {"cumulant",cumulant_names}, {"raw_moment",raw_moment_names}, {"central_moment",central_moment_names}};
	out_root = new TFile((out_path+out_root_name).data(), "RECREATE");
	analyze_sets_lite();
}


void AzimuthBinAnalyzer::analyze_sets_lite() {
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency() - free_threads);
		mutex *mtx = new mutex;
		for (pair<string, vector<int>> set : sets) {
			for (int set_num = set.second.front(); set_num <= set.second.back(); set_num++) {
				pool.enqueue(&AzimuthBinAnalyzer::analyze_subset_lite, this, set.first, set_num, mtx);
			}
		}
	}

	{
		ThreadPool pool(thread::hardware_concurrency() - free_threads);
		for(pair<string, vector<int>> set:sets) {
			combine_set_lite(set.first);
		}
	}

	combine_systematics();
	if (plot_sys) { plot_systematics(); }
	if (write_sys) { write_systematics(); }
}


void AzimuthBinAnalyzer::analyze_set_lite(string set_name, vector<int> set_nums, mutex *mtx) {  // Not used, all done in analyze_sets_lite.
	for(int set_num = set_nums[0]; set_num <= set_nums[1]; set_num++) {
		analyze_subset_lite(set_name, set_num, mtx);
	}

	combine_set_lite(set_name);
}


void AzimuthBinAnalyzer::analyze_subset_lite(string set_name, int set_num, mutex *mtx) {

	cout << "Starting Set " + set_name + to_string(set_num) << endl;

	string path, path_mix, set_dir_ext;
	if (in_string(set_name, "Ampt")) {
		path = ampt_in_path + set_name + to_string(set_num) + "/";
		path_mix = ampt_in_mix_path + set_name + to_string(set_num) + "/";
	} else if (in_string(set_name, "CF")) {
		path = cf_in_path + set_name + to_string(set_num) + "/";
		path_mix = cf_in_mix_path + set_name + to_string(set_num) + "/";
	} else if (in_string(set_name, "Sim")) {
		path = sim_in_path + set_name + to_string(set_num) + "/";
		path_mix = sim_in_mix_path + set_name + to_string(set_num) + "/";
	} else {
		path = bes_in_path + set_name + to_string(set_num) + "/";
		path_mix = bes_in_mix_path + set_name + to_string(set_num) + "/";
	}

	map<int, map<int, map<int, AzimuthBinData>>> data = get_data(path);
	map<int, map<int, map<int, AzimuthBinData>>> data_mix = get_data(path_mix);

	auto ratio_stats = calculate_stats(data, "ratio");
	auto ratio_stats_mix = calculate_stats(data_mix, "ratio");

	auto diff_stats = calculate_stats(data, "diff");
	auto diff_stats_mix = calculate_stats(data_mix, "diff");

	auto diff_slice_stats_mix = calculate_mix_diff_sds(data_mix);


	for(auto &energy:data) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &particles:cent.second.get_bin_data()) {
					float p = (float)div.first / 360;
					cent.second.set_diff_slice_divisor(particles.first, sqrt(particles.first * p * (1 - p)));
				}
			}
		}
	}

	for(auto &energy:data_mix) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &particles:cent.second.get_bin_data()) {
					float p = (float)div.first / 360;
					cent.second.set_diff_slice_divisor(particles.first, sqrt(particles.first * p * (1 - p)));
				}
			}
		}
	}

	auto pull_stats = calculate_stats(data, "diff_slice_proj");
	auto pull_mix_stats = calculate_stats(data_mix, "diff_slice_proj");

	map<int, map<int, map<int, map<string, Measure>>>> stats_mix_divide;

	if(mtx) { mtx->lock(); }
	for(auto &energy:ratio_stats) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &stat:cent.second) {
					if(stat.first == "particle_dist_mean") { stats_mix_divide[energy.first][div.first][cent.first][stat.first] = stat.second; }
					else { stats_mix_divide[energy.first][div.first][cent.first][stat.first] = stat.second / ratio_stats_mix[energy.first][div.first][cent.first][stat.first]; }

					raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stat.second);
					mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(ratio_stats_mix[energy.first][div.first][cent.first][stat.first]);
					divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stats_mix_divide[energy.first][div.first][cent.first][stat.first]);
					diff_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(diff_stats[energy.first][div.first][cent.first][stat.first]);
					pull_raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(pull_stats[energy.first][div.first][cent.first][stat.first]);
					pull_mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(pull_mix_stats[energy.first][div.first][cent.first][stat.first]);
					pull_divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(pull_stats[energy.first][div.first][cent.first][stat.first] / pull_mix_stats[energy.first][div.first][cent.first][stat.first]);
				}
			}
		}
	}

	//for(auto &energy:pull_stats) {
	//	for(auto &div:energy.second) {
	//		for(auto &cent:div.second) {
	//			for(auto &stat:cent.second) {
	//				pull_raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stat.second);
	//				pull_mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(pull_mix_stats[energy.first][div.first][cent.first][stat.first]);
	//				pull_divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stat.second / pull_mix_stats[energy.first][div.first][cent.first][stat.first]);
	//			}
	//		}
	//	}
	//}
	if(mtx) { mtx->unlock(); }
}


void AzimuthBinAnalyzer::combine_sets() {
	cout << "Combining all sets" << endl;

	TDirectory *sets_dir = out_root->mkdir("Sets_Comp");

	map<string, map<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>>> set_pairs;
	map<string, map<string, map<string, map<int, map<int, map<int, map<string, double>>>>>>> set_pairs_sd;

	for(pair<string, vector<string>> combo:set_combos) {
		for(string set:combo.second) {
			set_pairs[combo.first]["raw"][set] = raw_stats_median[set];
			set_pairs[combo.first]["mix"][set] = mix_stats_median[set];
			set_pairs[combo.first]["divide"][set] = divide_stats_median[set];
			set_pairs[combo.first]["pull_raw"][set] = pull_raw_stats_median[set];
			set_pairs[combo.first]["pull_mix"][set] = pull_mix_stats_median[set];
			set_pairs[combo.first]["diff"][set] = diff_stats_median[set];
			set_pairs[combo.first]["pull_divide"][set] = pull_divide_stats_median[set];

			set_pairs_sd[combo.first]["raw"][set] = raw_stats_sd[set];
			set_pairs_sd[combo.first]["mix"][set] = mix_stats_sd[set];
			set_pairs_sd[combo.first]["divide"][set] = divide_stats_sd[set];
			set_pairs_sd[combo.first]["pull_raw"][set] = pull_raw_stats_sd[set];
			set_pairs_sd[combo.first]["pull_mix"][set] = pull_mix_stats_sd[set];
			set_pairs_sd[combo.first]["diff"][set] = diff_stats_sd[set];
			set_pairs_sd[combo.first]["pull_divide"][set] = pull_divide_stats_sd[set];
		}
	}
	set_pairs["All"] = {{"raw", raw_stats_median}, {"mix", mix_stats_median}, {"divide", divide_stats_median}, {"pull_raw", pull_raw_stats_median}, {"pull_mix", pull_mix_stats_median}, {"diff", diff_stats_median}, {"pull_divide", pull_divide_stats_median}};
	set_pairs_sd["All"] = {{"raw", raw_stats_sd}, {"mix", mix_stats_sd}, {"divide", divide_stats_sd}, {"pull_raw", pull_raw_stats_sd}, {"pull_mix", pull_mix_stats_sd}, {"diff", diff_stats_sd}, {"pull_divide", pull_divide_stats_sd}};

	map<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>> set_type;
	map<string, map<string, map<int, map<int, map<int, map<string, double>>>>>> set_type_sd;
	for(pair<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>> data_type:set_pairs["All"]) {
		for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:data_type.second) {
			set_type[data_set.first][data_type.first] = data_set.second;
			set_type_sd[data_set.first][data_type.first] = set_pairs_sd["All"][data_type.first][data_set.first];
		}
	}

	TDirectory *type_rows_dir = sets_dir->mkdir("Type_Rows");
	type_rows_dir->cd();
	for(pair<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>> data_set:set_type) {
		type_per_row(data_set.second, set_type_sd[data_set.first], row_types, stat_names_rows, plot_cents, plot_divs, data_set.first);
	}


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
					roli_thesis_stats(set_pair.second["pull_raw"], set_pairs_sd[set_pair.first]["pull_raw"], name.second, centralities, {div}, "roli_thesis_pull_raw_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = data_dir->mkdir("centralities");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = centralities_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						centralities_stat(set_pair.second["raw"], set_pairs_sd[set_pair.first]["raw"], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
						centralities_stat(set_pair.second["pull_raw"], set_pairs_sd[set_pair.first]["pull_raw"], name, all_centralities, {div}, "centralities_pull_raw_"+name+to_string(div));
//						centralities_stat(set_pair.second["pull_raw"], set_pairs_sd[set_pair.first]["pull_raw"], name, all_centralities, {div}, "centralities_pull_cor_"+name+to_string(div));
						centralities_stat(set_pair.second["diff"], set_pairs_sd[set_pair.first]["diff"], name, all_centralities, {div}, "centralities_diff_"+name+to_string(div));
					}
					centralities_stat(set_pair.second["raw"], set_pairs_sd[set_pair.first]["raw"], name, all_centralities, divs, "centralities_raw_"+name);
					centralities_stat(set_pair.second["pull_raw"], set_pairs_sd[set_pair.first]["pull_raw"], name, all_centralities, divs, "centralities_pull_raw_"+name);
				}
			}
			TDirectory *dist_mean_dir = data_dir->mkdir("dist_means");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = dist_mean_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						stat_vs_mult_mean(set_pair.second["raw"], set_pairs_sd[set_pair.first]["raw"], name, all_centralities, {div}, "stat_vs_mult_mean_raw_"+name+to_string(div));
						stat_vs_mult_mean(set_pair.second["pull_raw"], set_pairs_sd[set_pair.first]["pull_raw"], name, all_centralities, {div}, "stat_vs_mult_mean_pull_raw_"+name+to_string(div));
						stat_vs_mult_mean(set_pair.second["diff"], set_pairs_sd[set_pair.first]["diff"], name, all_centralities, {div}, "stat_vs_mult_mean_diff_"+name+to_string(div));
//						stat_vs_mult_mean_cor(set_pair.second["pull"], set_pairs_sd[set_pair.first]["pull"], name, all_centralities, {div}, "stat_vs_mult_mean_cor_pull_"+name+to_string(div));
					}
					stat_vs_mult_mean(set_pair.second["raw"], set_pairs_sd[set_pair.first]["raw"], name, all_centralities, divs, "stat_vs_mult_mean_raw_"+name);
					stat_vs_mult_mean(set_pair.second["pull_raw"], set_pairs_sd[set_pair.first]["pull_raw"], name, all_centralities, divs, "stat_vs_mult_mean_pull_raw_"+name);
				}
			}
			TDirectory *divs_dir = data_dir->mkdir("divs");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = divs_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int energy:energy_list) {
						division_stat(set_pair.second["raw"], set_pairs_sd[set_pair.first]["raw"], name, all_centralities, divs, {energy}, "divs_raw_"+name+to_string(energy)+"GeV");
						division_stat(set_pair.second["pull_raw"], set_pairs_sd[set_pair.first]["pull_raw"], name, all_centralities, divs, {energy}, "divs_pull_raw_"+name+to_string(energy)+"GeV");
					}
					division_stat(set_pair.second["raw"], set_pairs_sd[set_pair.first]["raw"], name, all_centralities, divs, energy_list, "divs_raw_"+name);
					division_stat(set_pair.second["pull_raw"], set_pairs_sd[set_pair.first]["pull_raw"], name, all_centralities, divs, energy_list, "divs_pull_raw_"+name);
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
					roli_thesis_stats(set_pair.second["pull_mix"], set_pairs_sd[set_pair.first]["pull_mix"], name.second, centralities, {div}, "roli_thesis_pull_mix_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = mix_dir->mkdir("centralities");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = centralities_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						centralities_stat(set_pair.second["mix"], set_pairs_sd[set_pair.first]["mix"], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
						centralities_stat(set_pair.second["pull_mix"], set_pairs_sd[set_pair.first]["pull_mix"], name, all_centralities, {div}, "centralities_pull_mix_"+name+to_string(div));
					}
					centralities_stat(set_pair.second["mix"], set_pairs_sd[set_pair.first]["mix"], name, all_centralities, divs, "centralities_mix_"+name);
					centralities_stat(set_pair.second["pull_mix"], set_pairs_sd[set_pair.first]["pull_mix"], name, all_centralities, divs, "centralities_pull_mix_"+name);
				}
			}
			TDirectory *dist_mean_dir = mix_dir->mkdir("dist_means");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = dist_mean_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						stat_vs_mult_mean(set_pair.second["mix"], set_pairs_sd[set_pair.first]["mix"], name, all_centralities, {div}, "stat_vs_mult_mean_mix_"+name+to_string(div));
						stat_vs_mult_mean(set_pair.second["pull_mix"], set_pairs_sd[set_pair.first]["pull_mix"], name, all_centralities, {div}, "stat_vs_mult_mean_pull_mix_"+name+to_string(div));
					}
					stat_vs_mult_mean(set_pair.second["mix"], set_pairs_sd[set_pair.first]["mix"], name, all_centralities, divs, "stat_vs_mult_mean_mix_"+name);
					stat_vs_mult_mean(set_pair.second["pull_mix"], set_pairs_sd[set_pair.first]["pull_mix"], name, all_centralities, divs, "stat_vs_mult_mean_pull_mix_"+name);
				}
			}
			TDirectory *divs_dir = mix_dir->mkdir("divs");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = divs_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int energy:energy_list) {
						division_stat(set_pair.second["mix"], set_pairs_sd[set_pair.first]["mix"], name, all_centralities, divs, {energy}, "divs_mix_"+name+to_string(energy)+"GeV");
						division_stat(set_pair.second["pull_mix"], set_pairs_sd[set_pair.first]["pull_mix"], name, all_centralities, divs, {energy}, "divs_pull_mix_"+name+to_string(energy)+"GeV");
					}
					division_stat(set_pair.second["mix"], set_pairs_sd[set_pair.first]["mix"], name, all_centralities, divs, energy_list, "divs_mix_"+name);
					division_stat(set_pair.second["pull_mix"], set_pairs_sd[set_pair.first]["pull_mix"], name, all_centralities, divs, energy_list, "divs_pull_mix_"+name);
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
					roli_thesis_stats(set_pair.second["pull_divide"], set_pairs_sd[set_pair.first]["pull_divide"], name.second, centralities, {div}, "roli_thesis_pull_divide_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = mix_div_dir->mkdir("centralities");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = centralities_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						centralities_stat(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
//						centralities_stat(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, {div}, "centralities_divide_cor_"+name+to_string(div));
						centralities_stat(set_pair.second["pull_divide"], set_pairs_sd[set_pair.first]["pull_divide"], name, all_centralities, {div}, "centralities_pull_divide_"+name+to_string(div));
					}
					centralities_stat(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, divs, "centralities_divide_"+name);
					centralities_stat(set_pair.second["pull_divide"], set_pairs_sd[set_pair.first]["pull_divide"], name, all_centralities, divs, "centralities_pull_divide_"+name);
				}
			}
			TDirectory *dist_means_dir = mix_div_dir->mkdir("dist_means");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = dist_means_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						stat_vs_mult_mean(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, {div}, "stat_vs_mult_mean_divide_"+name+to_string(div));
						stat_vs_mult_mean(set_pair.second["pull_divide"], set_pairs_sd[set_pair.first]["pull_divide"], name, all_centralities, {div}, "stat_vs_mult_mean_pull_divide_"+name+to_string(div));
//						stat_vs_mult_mean_cor(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, {div}, "stat_vs_mult_mean_cor_divide_"+name+to_string(div));
					}
					stat_vs_mult_mean(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, divs, "stat_vs_mult_mean_divide_"+name);
					stat_vs_mult_mean(set_pair.second["pull_divide"], set_pairs_sd[set_pair.first]["pull_divide"], name, all_centralities, divs, "stat_vs_mult_mean_pull_divide_"+name);
				}
			}
			TDirectory *divs_dir = mix_div_dir->mkdir("divs");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = divs_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int energy:energy_list) {
						division_stat(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, divs, {energy}, "divs_divide_"+name+to_string(energy)+"GeV");
						division_stat(set_pair.second["pull_divide"], set_pairs_sd[set_pair.first]["pull_divide"], name, all_centralities, divs, {energy}, "divs_pull_divide_"+name+to_string(energy)+"GeV");
					}
					division_stat(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, divs, energy_list, "divs_divide_"+name);
					division_stat(set_pair.second["pull_divide"], set_pairs_sd[set_pair.first]["pull_divide"], name, all_centralities, divs, energy_list, "divs_pull_divide_"+name);
				}
			}
		}
	}

//	TDirectory *sets_dir = out_root->mkdir("Systematics");

	// Calculate standard deviations for systematics
//	for(pair<int, map<int, map<int, map<string, Measure>>>> energy:raw_stats_median["Rand_Rotate"]) {
//		for(pair<int, map<int, map<string, Measure>>> div:energy.second) {
//			for(pair<int, map<string, Measure>> cent:div.second) {
//				for(pair<string, Measure> stat:cent.second) {
//					double raw_sd = raw_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
//					Measure raw_med = raw_stats_median["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
//					Measure raw_eff_med = raw_stats_median["Efficiency_05_"][energy.first][div.first][cent.first][stat.first];
//					Measure raw_pile_med = raw_stats_median["Pile_Up_0005_"][energy.first][div.first][cent.first][stat.first];
//					raw_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first] = pow( pow(raw_sd, 2) + pow(raw_med.get_val() - raw_eff_med.get_val(), 2) + pow(raw_med.get_val() - raw_pile_med.get_val(), 2), 0.5);
//
//					double mix_sd = mix_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
//					Measure mix_med = mix_stats_median["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
//					Measure mix_eff_med = mix_stats_median["Efficiency_05_"][energy.first][div.first][cent.first][stat.first];
//					Measure mix_pile_med = mix_stats_median["Pile_Up_0005_"][energy.first][div.first][cent.first][stat.first];
//					mix_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first] = pow( pow(mix_sd, 2) + pow(mix_med.get_val() - mix_eff_med.get_val(), 2) + pow(mix_med.get_val() - mix_pile_med.get_val(), 2), 0.5);
//
//					double divide_sd = divide_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
//					Measure divide_med = divide_stats_median["Rand_Rotate"][energy.first][div.first][cent.first][stat.first];
//					Measure divide_eff_med = divide_stats_median["Efficiency_05_"][energy.first][div.first][cent.first][stat.first];
//					Measure divide_pile_med = divide_stats_median["Pile_Up_0005_"][energy.first][div.first][cent.first][stat.first];
//					divide_stats_sd["Rand_Rotate"][energy.first][div.first][cent.first][stat.first] = pow( pow(divide_sd, 2) + pow(divide_med.get_val() - divide_eff_med.get_val(), 2) + pow(divide_med.get_val() - divide_pile_med.get_val(), 2), 0.5);
//				}
//			}
//		}
//	}
//
//
//	TDirectory *finals_dir = sets_dir->mkdir("Final_Systematics");
//
//	TDirectory *comp_dir = finals_dir->mkdir("Comparison");
//	comp_dir->cd();
//	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mixed = {{"raw",raw_stats_median["Rand_Rotate"]}, {"mixed",mix_stats_median["Rand_Rotate"]}};
//	map<string, map<int, map<int, map<int, map<string, double>>>>> raw_mixed_sd = {{"raw",raw_stats_sd["Rand_Rotate"]}, {"mixed",mix_stats_sd["Rand_Rotate"]}};
//
//	{
//		TDirectory *roli_thesis_dir = comp_dir->mkdir("roli_thesis");
//		for(pair<string, vector<string>> name:names) {
//			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
//			name_dir->cd();
//			for(int div:divs) {
//				roli_thesis_stats(raw_mixed, raw_mixed_sd, name.second, centralities, {div}, "roli_thesis_raw_mix_comp_"+name.first+to_string(div));
//			}
//		}
//		TDirectory *centralities_dir = comp_dir->mkdir("centralities");
//		for(string name:names["stat"]) {
//			TDirectory *name_dir = centralities_dir->mkdir(name.data());
//			name_dir->cd();
//			for(int div:divs) {
//				centralities_stat(raw_mixed, raw_mixed_sd, name, all_centralities, {div}, "centralities_raw_mix_comp_"+name+to_string(div));
//			}
//		}
//	}
//
//	TDirectory *data_dir = finals_dir->mkdir("Raw_Data");
//	data_dir->cd();
//
//	{
//		TDirectory *roli_thesis_dir = data_dir->mkdir("roli_thesis");
//		for(pair<string, vector<string>> name:names) {
//			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
//			name_dir->cd();
//			for(int div:divs) {
//				roli_thesis_stats(raw_stats_median["Rand_Rotate"], raw_stats_sd["Rand_Rotate"], name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
//			}
//		}
//		TDirectory *centralities_dir = data_dir->mkdir("centralities");
//		for(string name:names["stat"]) {
//			TDirectory *name_dir = centralities_dir->mkdir(name.data());
//			name_dir->cd();
//			for(int div:divs) {
//				centralities_stat(raw_stats_median["Rand_Rotate"], raw_stats_sd["Rand_Rotate"], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
//			}
//		}
//	}
//
//	TDirectory *mix_dir = finals_dir->mkdir("Mix_Data");
//	mix_dir->cd();
//
//	{
//		TDirectory *roli_thesis_dir = mix_dir->mkdir("roli_thesis");
//		for(pair<string, vector<string>> name:names) {
//			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
//			name_dir->cd();
//			for(int div:divs) {
//				roli_thesis_stats(mix_stats_median["Rand_Rotate"], mix_stats_sd["Rand_Rotate"], name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
//			}
//		}
//		TDirectory *centralities_dir = mix_dir->mkdir("centralities");
//		for(string name:names["stat"]) {
//			TDirectory *name_dir = centralities_dir->mkdir(name.data());
//			name_dir->cd();
//			for(int div:divs) {
//				centralities_stat(mix_stats_median["Rand_Rotate"], mix_stats_sd["Rand_Rotate"], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
//			}
//		}
//	}
//
//
//	TDirectory *mix_div_dir = finals_dir->mkdir("Mix_Divded_Data");
//	mix_div_dir->cd();
//
//	{
//		TDirectory *roli_thesis_dir = mix_div_dir->mkdir("roli_thesis");
//		for(pair<string, vector<string>> name:names) {
//			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
//			name_dir->cd();
//			for(int div:divs) {
//				roli_thesis_stats(divide_stats_median["Rand_Rotate"], divide_stats_sd["Rand_Rotate"], name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
//			}
//		}
//		TDirectory *centralities_dir = mix_div_dir->mkdir("centralities");
//		for(string name:names["stat"]) {
//			TDirectory *name_dir = centralities_dir->mkdir(name.data());
//			name_dir->cd();
//			for(int div:divs) {
//				centralities_stat(divide_stats_median["Rand_Rotate"], divide_stats_sd["Rand_Rotate"], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
//			}
//		}
//	}
}


void AzimuthBinAnalyzer::combine_systematics() {
	cout << endl << "Getting systematics" << endl;

	for(pair<string, pair<string, vector<string>>> sys_combo:sys_combos) {
		def_medians[sys_combo.first]["raw"] = raw_stats_median[sys_combo.second.first];
		def_medians[sys_combo.first]["mix"] = mix_stats_median[sys_combo.second.first];
		def_medians[sys_combo.first]["divide"] = divide_stats_median[sys_combo.second.first];
		def_medians[sys_combo.first]["pull_raw"] = pull_raw_stats_median[sys_combo.second.first];
		def_medians[sys_combo.first]["pull_mix"] = pull_mix_stats_median[sys_combo.second.first];
		def_medians[sys_combo.first]["diff"] = diff_stats_median[sys_combo.second.first];
		def_medians[sys_combo.first]["pull_divide"] = pull_divide_stats_median[sys_combo.second.first];

		map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> sys_sets;

//		cout << "sys_sets start" << endl;
		for(string set:sys_combo.second.second) {
//			cout << "sys_sets set: " << set << endl;
			for(pair<int, map<int, map<int, map<string, vector<Measure>>>>> energy:raw_stats_sets[set]) {
//				cout << "sys_sets energy: " << energy.first << endl;
				for(pair<int, map<int, map<string, vector<Measure>>>> div:energy.second) {
//					cout << "sys_sets div: " << div.first << endl;
					for(pair<int, map<string, vector<Measure>>> cent:div.second) {
//						cout << "sys_sets cent: " << cent.first << endl;
						for(pair<string, vector<Measure>> stat:cent.second) {
//							cout << "sys_sets stat: " << stat.first << endl;
							sys_sets["raw"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["raw"][energy.first][div.first][cent.first][stat.first].end(), raw_stats_sets[set][energy.first][div.first][cent.first][stat.first].begin(), raw_stats_sets[set][energy.first][div.first][cent.first][stat.first].end());
							sys_sets["mix"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["mix"][energy.first][div.first][cent.first][stat.first].end(), mix_stats_sets[set][energy.first][div.first][cent.first][stat.first].begin(), mix_stats_sets[set][energy.first][div.first][cent.first][stat.first].end());
							sys_sets["divide"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["divide"][energy.first][div.first][cent.first][stat.first].end(), divide_stats_sets[set][energy.first][div.first][cent.first][stat.first].begin(), divide_stats_sets[set][energy.first][div.first][cent.first][stat.first].end());
							sys_sets["pull_raw"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["pull_raw"][energy.first][div.first][cent.first][stat.first].end(), pull_raw_stats_sets[set][energy.first][div.first][cent.first][stat.first].begin(), pull_raw_stats_sets[set][energy.first][div.first][cent.first][stat.first].end());
							sys_sets["pull_mix"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["pull_mix"][energy.first][div.first][cent.first][stat.first].end(), pull_mix_stats_sets[set][energy.first][div.first][cent.first][stat.first].begin(), pull_mix_stats_sets[set][energy.first][div.first][cent.first][stat.first].end());
							sys_sets["diff"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["diff"][energy.first][div.first][cent.first][stat.first].end(), diff_stats_sets[set][energy.first][div.first][cent.first][stat.first].begin(), diff_stats_sets[set][energy.first][div.first][cent.first][stat.first].end());
							sys_sets["pull_divide"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["pull_divide"][energy.first][div.first][cent.first][stat.first].end(), pull_divide_stats_sets[set][energy.first][div.first][cent.first][stat.first].begin(), pull_divide_stats_sets[set][energy.first][div.first][cent.first][stat.first].end());
						}
					}
				}
			}
		}
//		cout << "Calc sys errors" << endl;
		for(pair<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> data_type:sys_sets) {
//			cout << "Sys " << data_type.first << endl;
			for(pair<int, map<int, map<int, map<string, vector<Measure>>>>> energy:data_type.second) {
//				cout << "Sys " << energy.first << endl;
				for(pair<int, map<int, map<string, vector<Measure>>>> div:energy.second) {
//					cout << "Sys " << div.first << endl;
					for(pair<int, map<string, vector<Measure>>> cent:div.second) {
//						cout << "Sys " << cent.first << endl;
						for(pair<string, vector<Measure>> stat:cent.second) {
//							cout << "Sys " << stat.first << endl;
							def_systematics[sys_combo.first][data_type.first][energy.first][div.first][cent.first][stat.first] = sample_sd_errweight(stat.second);
						}
					}
				}
			}
		}
	}
}

//	map<string, map<int, map<int, map<int, map<string, Measure>>>>> def;
//	map<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> sys_sets;
//	map<string, map<int, map<int, map<int, map<string, double>>>>> systematics;
//
//	def["raw"] = raw_stats_median[def_set];
//	def["mix"] = mix_stats_median[def_set];
//	def["divide"] = divide_stats_median[def_set];
//	def["pull_raw"] = pull_raw_stats_median[def_set];
//	def["pull_mix"] = pull_mix_stats_median[def_set];
//	def["diff"] = diff_stats_median[def_set];
//	def["pull_divide"] = pull_divide_stats_median[def_set];
//
//	for(pair<string, vector<int>> set:sets) {
//		if(set.first != def_set){
//			for(pair<int, map<int, map<int, map<string, vector<Measure>>>>> energy:raw_stats_sets[set.first]) {
//				for(pair<int, map<int, map<string, vector<Measure>>>> div:energy.second) {
//					for(pair<int, map<string, vector<Measure>>> cent:div.second) {
//						for(pair<string, vector<Measure>> stat:cent.second) {
//							sys_sets["raw"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["raw"][energy.first][div.first][cent.first][stat.first].end(), raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].begin(), raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].end());
//							sys_sets["mix"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["mix"][energy.first][div.first][cent.first][stat.first].end(), mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].begin(), mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].end());
//							sys_sets["divide"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["divide"][energy.first][div.first][cent.first][stat.first].end(), divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].begin(), divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].end());
//							sys_sets["pull_raw"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["pull_raw"][energy.first][div.first][cent.first][stat.first].end(), pull_raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].begin(), pull_raw_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].end());
//							sys_sets["pull_mix"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["pull_mix"][energy.first][div.first][cent.first][stat.first].end(), pull_mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].begin(), pull_mix_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].end());
//							sys_sets["diff"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["diff"][energy.first][div.first][cent.first][stat.first].end(), diff_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].begin(), diff_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].end());
//							sys_sets["pull_divide"][energy.first][div.first][cent.first][stat.first].insert(sys_sets["pull_divide"][energy.first][div.first][cent.first][stat.first].end(), pull_divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].begin(), pull_divide_stats_sets[set.first][energy.first][div.first][cent.first][stat.first].end());
//						}
//					}
//				}
//			}
//		}
//	}
//	for(pair<string, map<int, map<int, map<int, map<string, vector<Measure>>>>>> data_type:sys_sets) {
//		for(pair<int, map<int, map<int, map<string, vector<Measure>>>>> energy:data_type.second) {
//			for(pair<int, map<int, map<string, vector<Measure>>>> div:energy.second) {
//				for(pair<int, map<string, vector<Measure>>> cent:div.second) {
//					for(pair<string, vector<Measure>> stat:cent.second) {
//						systematics[data_type.first][energy.first][div.first][cent.first][stat.first] = sample_sd(stat.second);
//					}
//				}
//			}
//		}
//	}



void AzimuthBinAnalyzer::plot_systematics() {
	cout << endl << "Plotting systematics" << endl;

	TDirectory *sys_dir = out_root->mkdir("Systematics");

	map<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>> def_comps;
	map<string, map<string, map<int, map<int, map<int, map<string, double>>>>>> sys_comps;

	for(pair<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>> sys_combo:def_medians) {

		map<string, map<int, map<int, map<int, map<string, Measure>>>>> &def = sys_combo.second;
		map<string, map<int, map<int, map<int, map<string, double>>>>> &systematics = def_systematics[sys_combo.first];

		for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_type:def) {  // Set comparison maps, inverting data_type and systematic name.
			def_comps[data_type.first][sys_combo.first] = data_type.second;
			sys_comps[data_type.first][sys_combo.first] = systematics[data_type.first];
		}

		TDirectory *ind_sys_dir = sys_dir->mkdir(sys_combo.first.data());
		TDirectory *comp_dir = ind_sys_dir->mkdir("Comparison");
		comp_dir->cd();
		map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mixed = {{"raw",def["raw"]}, {"mixed",def["mix"]}};
		map<string, map<int, map<int, map<int, map<string, double>>>>> raw_mixed_sd = {{"raw",systematics["raw"]}, {"mixed",systematics["mix"]}};
		map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mixed_pull = {{"raw",def["pull_raw"]}, {"mixed",def["pull_mix"]}};
		map<string, map<int, map<int, map<int, map<string, double>>>>> raw_mixed_pull_sd = {{"raw",systematics["pull_raw"]}, {"mixed",systematics["pull_mix"]}};

		{
			TDirectory *roli_thesis_dir = comp_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(raw_mixed, raw_mixed_sd, name.second, centralities, {div}, "roli_thesis_raw_mix_comp_"+name.first+to_string(div));
					roli_thesis_stats(raw_mixed_pull, raw_mixed_pull_sd, name.second, centralities, {div}, "roli_thesis_raw_mix_pull_comp_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = comp_dir->mkdir("centralities");
			for(string name:names["stat"]) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat2(raw_mixed, raw_mixed_sd, name, all_centralities, {div}, "centralities_raw_mix_comp_"+name+to_string(div));
					centralities_stat2(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, {div}, "centralities_raw_mix_pull_comp_"+name+to_string(div));
				}
				centralities_stat(raw_mixed, raw_mixed_sd, name, all_centralities, divs, "centralities_raw_mix_comp_"+name);
				centralities_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, "centralities_raw_mix_pull_comp_"+name);
			}
			TDirectory *dist_means_dir = comp_dir->mkdir("dist_means");
			for(string name:names["stat"]) {
				TDirectory *name_dir = dist_means_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(raw_mixed, raw_mixed_sd, name, all_centralities, {div}, "stat_vs_mult_mean_raw_mix_comp_"+name+to_string(div));
					stat_vs_mult_mean(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, {div}, "stat_vs_mult_mean_raw_mix_pull_comp_"+name+to_string(div));
				}
				stat_vs_mult_mean(raw_mixed, raw_mixed_sd, name, all_centralities, divs, "stat_vs_mult_mean_raw_mix_comp_"+name);
				stat_vs_mult_mean(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, "stat_vs_mult_mean_raw_mix_pull_comp_"+name);
			}
			TDirectory *div_dir = comp_dir->mkdir("divs");
			for(string name:names["stat"]) {
				TDirectory *name_dir = div_dir->mkdir(name.data());
				name_dir->cd();
				for(int energy:energy_list) {
					division_stat(raw_mixed, raw_mixed_sd, name, all_centralities, divs, {energy}, "divs_raw_mix_comp_"+name+to_string(energy)+"GeV");
					division_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, {energy}, "divs_raw_mix_pull_comp_"+name+to_string(energy)+"GeV");
				}
				division_stat(raw_mixed, raw_mixed_sd, name, all_centralities, divs, energy_list, "divs_raw_mix_comp_"+name);
				division_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, energy_list, "divs_raw_mix_pull_comp_"+name);
			}
		}


		TDirectory *data_dir = ind_sys_dir->mkdir("Raw_Data");
		data_dir->cd();

		{
			TDirectory *roli_thesis_dir = data_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(def["raw"], systematics["raw"], name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
					roli_thesis_stats(def["pull_raw"], systematics["pull_raw"], name.second, centralities, {div}, "roli_thesis_pull_raw_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = data_dir->mkdir("centralities");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = centralities_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						centralities_stat(def["raw"], systematics["raw"], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
						centralities_stat(def["pull_raw"], systematics["pull_raw"], name, all_centralities, {div}, "centralities_pull_raw_"+name+to_string(div));
	//						centralities_stat2(def["pull_raw"], systematics["pull_raw"], name, all_centralities, {div}, "centralities_pull_cor_"+name+to_string(div));
						centralities_stat(def["diff"], systematics["diff"], name, all_centralities, {div}, "centralities_diff_"+name+to_string(div));
					}
					centralities_stat(def["raw"], systematics["raw"], name, all_centralities, divs, "centralities_raw_"+name);
					centralities_stat(def["pull_raw"], systematics["pull_raw"], name, all_centralities, divs, "centralities_pull_raw_"+name);
				}
			}
			TDirectory *dist_mean_dir = data_dir->mkdir("dist_means");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = dist_mean_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						stat_vs_mult_mean(def["raw"], systematics["raw"], name, all_centralities, {div}, "stat_vs_mult_mean_raw_"+name+to_string(div));
						stat_vs_mult_mean(def["pull_raw"], systematics["pull_raw"], name, all_centralities, {div}, "stat_vs_mult_mean_pull_raw_"+name+to_string(div));
						stat_vs_mult_mean(def["diff"], systematics["diff"], name, all_centralities, {div}, "stat_vs_mult_mean_diff_"+name+to_string(div));
	//						stat_vs_mult_mean_cor(def["pull"], systematics["pull"], name, all_centralities, {div}, "stat_vs_mult_mean_cor_pull_"+name+to_string(div));
					}
					stat_vs_mult_mean(def["raw"], systematics["raw"], name, all_centralities, divs, "stat_vs_mult_mean_raw_"+name);
					stat_vs_mult_mean(def["pull_raw"], systematics["pull_raw"], name, all_centralities, divs, "stat_vs_mult_mean_pull_raw_"+name);
				}
			}
			TDirectory *divs_dir = data_dir->mkdir("divs");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = divs_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int energy:energy_list) {
						division_stat(def["raw"], systematics["raw"], name, all_centralities, divs, {energy}, "divs_raw_"+name+to_string(energy)+"GeV");
						division_stat(def["pull_raw"], systematics["pull_raw"], name, all_centralities, divs, {energy}, "divs_pull_raw_"+name+to_string(energy)+"GeV");
					}
					division_stat(def["raw"], systematics["raw"], name, all_centralities, divs, energy_list, "divs_raw_"+name);
					division_stat(def["pull_raw"], systematics["pull_raw"], name, all_centralities, divs, energy_list, "divs_pull_raw_"+name);
				}
			}
		}

		TDirectory *mix_dir = ind_sys_dir->mkdir("Mix_Data");
		mix_dir->cd();

		{
			TDirectory *roli_thesis_dir = mix_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(def["mix"], systematics["mix"], name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
					roli_thesis_stats(def["pull_mix"], systematics["pull_mix"], name.second, centralities, {div}, "roli_thesis_pull_mix_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = mix_dir->mkdir("centralities");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = centralities_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						centralities_stat(def["mix"], systematics["mix"], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
						centralities_stat(def["pull_mix"], systematics["pull_mix"], name, all_centralities, {div}, "centralities_pull_mix_"+name+to_string(div));
					}
					centralities_stat(def["mix"], systematics["mix"], name, all_centralities, divs, "centralities_mix_"+name);
					centralities_stat(def["pull_mix"], systematics["pull_mix"], name, all_centralities, divs, "centralities_pull_mix_"+name);
				}
			}
			TDirectory *dist_mean_dir = mix_dir->mkdir("dist_means");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = dist_mean_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						stat_vs_mult_mean(def["mix"], systematics["mix"], name, all_centralities, {div}, "stat_vs_mult_mean_mix_"+name+to_string(div));
						stat_vs_mult_mean(def["pull_mix"], systematics["pull_mix"], name, all_centralities, {div}, "stat_vs_mult_mean_pull_mix_"+name+to_string(div));
					}
					stat_vs_mult_mean(def["mix"], systematics["mix"], name, all_centralities, divs, "stat_vs_mult_mean_mix_"+name);
					stat_vs_mult_mean(def["pull_mix"], systematics["pull_mix"], name, all_centralities, divs, "stat_vs_mult_mean_pull_mix_"+name);
				}
			}
			TDirectory *divs_dir = mix_dir->mkdir("divs");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = divs_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int energy:energy_list) {
						division_stat(def["mix"], systematics["mix"], name, all_centralities, divs, {energy}, "divs_mix_"+name+to_string(energy)+"GeV");
						division_stat(def["pull_mix"], systematics["pull_mix"], name, all_centralities, divs, {energy}, "divs_pull_mix_"+name+to_string(energy)+"GeV");
					}
					division_stat(def["mix"], systematics["mix"], name, all_centralities, divs, energy_list, "divs_mix_"+name);
					division_stat(def["pull_mix"], systematics["pull_mix"], name, all_centralities, divs, energy_list, "divs_pull_mix_"+name);
				}
			}
		}


		TDirectory *mix_div_dir = ind_sys_dir->mkdir("Mix_Divded_Data");
		mix_div_dir->cd();

		{
			TDirectory *roli_thesis_dir = mix_div_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(def["divide"], systematics["divide"], name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
					roli_thesis_stats(def["pull_divide"], systematics["pull_divide"], name.second, centralities, {div}, "roli_thesis_pull_divide_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = mix_div_dir->mkdir("centralities");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = centralities_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						centralities_stat(def["divide"], systematics["divide"], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
	//						centralities_stat2(def["divide"], systematics["divide"], name, all_centralities, {div}, "centralities_divide_cor_"+name+to_string(div));
						centralities_stat(def["pull_divide"], systematics["pull_divide"], name, all_centralities, {div}, "centralities_pull_divide_"+name+to_string(div));
					}
					centralities_stat(def["divide"], systematics["divide"], name, all_centralities, divs, "centralities_divide_"+name);
					centralities_stat(def["pull_divide"], systematics["pull_divide"], name, all_centralities, divs, "centralities_pull_divide_"+name);
				}
			}
			TDirectory *dist_means_dir = mix_div_dir->mkdir("dist_means");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = dist_means_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int div:divs) {
						stat_vs_mult_mean(def["divide"], systematics["divide"], name, all_centralities, {div}, "stat_vs_mult_mean_divide_"+name+to_string(div));
						stat_vs_mult_mean(def["pull_divide"], systematics["pull_divide"], name, all_centralities, {div}, "stat_vs_mult_mean_pull_divide_"+name+to_string(div));
	//						stat_vs_mult_mean_cor(def["divide"], systematics["divide"], name, all_centralities, {div}, "stat_vs_mult_mean_cor_divide_"+name+to_string(div));
					}
					stat_vs_mult_mean(def["divide"], systematics["divide"], name, all_centralities, divs, "stat_vs_mult_mean_divide_"+name);
					stat_vs_mult_mean(def["pull_divide"], systematics["pull_divide"], name, all_centralities, divs, "stat_vs_mult_mean_pull_divide_"+name);
				}
			}
			TDirectory *divs_dir = mix_div_dir->mkdir("divs");
			for(pair<string, vector<string>> stat_type:names) {
				TDirectory *name_dir = divs_dir->mkdir(stat_type.first.data());
				name_dir->cd();
				for(string name:stat_type.second) {
					TDirectory *stat_name_dir = name_dir->mkdir(name.data());
					stat_name_dir->cd();
					for(int energy:energy_list) {
						division_stat(def["divide"], systematics["divide"], name, all_centralities, divs, {energy}, "divs_divide_"+name+to_string(energy)+"GeV");
						division_stat(def["pull_divide"], systematics["pull_divide"], name, all_centralities, divs, {energy}, "divs_pull_divide_"+name+to_string(energy)+"GeV");
					}
					division_stat(def["divide"], systematics["divide"], name, all_centralities, divs, energy_list, "divs_divide_"+name);
					division_stat(def["pull_divide"], systematics["pull_divide"], name, all_centralities, divs, energy_list, "divs_pull_divide_"+name);
				}
			}
		}
	}

//
//	map<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>> &def_comps;
//	map<string, map<string, map<int, map<int, map<int, map<string, double>>>>>> &sys_comps;
//
//
//	map<string, map<int, map<int, map<int, map<string, Measure>>>>> &def = sys_combo.second;
//	map<string, map<int, map<int, map<int, map<string, double>>>>> &systematics = def_systematics[sys_combo.first];

	TDirectory *comb_dir = sys_dir->mkdir("Combination");
	TDirectory *comp_dir = comb_dir->mkdir("Comparison");
	comp_dir->cd();
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mixed;  // = {{"raw",def_comps["raw"]}, {"mixed",def_comps["mix"]}};
	map<string, map<int, map<int, map<int, map<string, double>>>>> raw_mixed_sd;  // = {{"raw",sys_comps["raw"]}, {"mixed",sys_comps["mix"]}};
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mixed_pull;  // = {{"raw",def_comps["pull_raw"]}, {"mixed",def_comps["pull_mix"]}};
	map<string, map<int, map<int, map<int, map<string, double>>>>> raw_mixed_pull_sd;  // = {{"raw",sys_comps["pull_raw"]}, {"mixed",sys_comps["pull_mix"]}};

	for(pair<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>> data_type:def_comps) {
		for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> sys_name:data_type.second) {
			if(data_type.first == "raw") {
				raw_mixed["raw_"+sys_name.first] = sys_name.second;
				raw_mixed_sd["raw_"+sys_name.first] = sys_comps[data_type.first][sys_name.first];
			} else if(data_type.first == "pull_raw") {
				raw_mixed_pull["raw_"+sys_name.first] = sys_name.second;
				raw_mixed_pull_sd["raw_"+sys_name.first] = sys_comps[data_type.first][sys_name.first];
			} else if(data_type.first == "mix") {
				raw_mixed["mix_"+sys_name.first] = sys_name.second;
				raw_mixed_sd["mix_"+sys_name.first] = sys_comps[data_type.first][sys_name.first];
			} else if(data_type.first == "pull_mix") {
				raw_mixed_pull["mix_"+sys_name.first] = sys_name.second;
				raw_mixed_pull_sd["mix_"+sys_name.first] = sys_comps[data_type.first][sys_name.first];
			}
		}
	}

	{
		TDirectory *roli_thesis_dir = comp_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(raw_mixed, raw_mixed_sd, name.second, centralities, {div}, "roli_thesis_raw_mix_comp_"+name.first+to_string(div));
				roli_thesis_stats(raw_mixed_pull, raw_mixed_pull_sd, name.second, centralities, {div}, "roli_thesis_raw_mix_pull_comp_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = comp_dir->mkdir("centralities");
		for(string name:names["stat"]) {
			TDirectory *name_dir = centralities_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				centralities_stat2(raw_mixed, raw_mixed_sd, name, all_centralities, {div}, "centralities_raw_mix_comp_"+name+to_string(div));
				centralities_stat2(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, {div}, "centralities_raw_mix_pull_comp_"+name+to_string(div));
			}
			centralities_stat(raw_mixed, raw_mixed_sd, name, all_centralities, divs, "centralities_raw_mix_comp_"+name);
			centralities_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, "centralities_raw_mix_pull_comp_"+name);
		}
		TDirectory *dist_means_dir = comp_dir->mkdir("dist_means");
		for(string name:names["stat"]) {
			TDirectory *name_dir = dist_means_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				stat_vs_mult_mean(raw_mixed, raw_mixed_sd, name, all_centralities, {div}, "stat_vs_mult_mean_raw_mix_comp_"+name+to_string(div));
				stat_vs_mult_mean(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, {div}, "stat_vs_mult_mean_raw_mix_pull_comp_"+name+to_string(div));
			}
			stat_vs_mult_mean(raw_mixed, raw_mixed_sd, name, all_centralities, divs, "stat_vs_mult_mean_raw_mix_comp_"+name);
			stat_vs_mult_mean(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, "stat_vs_mult_mean_raw_mix_pull_comp_"+name);
		}
		TDirectory *div_dir = comp_dir->mkdir("divs");
		for(string name:names["stat"]) {
			TDirectory *name_dir = div_dir->mkdir(name.data());
			name_dir->cd();
			for(int energy:energy_list) {
				division_stat(raw_mixed, raw_mixed_sd, name, all_centralities, divs, {energy}, "divs_raw_mix_comp_"+name+to_string(energy)+"GeV");
				division_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, {energy}, "divs_raw_mix_pull_comp_"+name+to_string(energy)+"GeV");
			}
			division_stat(raw_mixed, raw_mixed_sd, name, all_centralities, divs, energy_list, "divs_raw_mix_comp_"+name);
			division_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, energy_list, "divs_raw_mix_pull_comp_"+name);
		}
	}


	TDirectory *data_dir = comb_dir->mkdir("Raw_Data");
	data_dir->cd();

	{
		TDirectory *roli_thesis_dir = data_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(def_comps["raw"], sys_comps["raw"], name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
				roli_thesis_stats(def_comps["pull_raw"], sys_comps["pull_raw"], name.second, centralities, {div}, "roli_thesis_pull_raw_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = data_dir->mkdir("centralities");
		for(pair<string, vector<string>> stat_type:names) {
			TDirectory *name_dir = centralities_dir->mkdir(stat_type.first.data());
			name_dir->cd();
			for(string name:stat_type.second) {
				TDirectory *stat_name_dir = name_dir->mkdir(name.data());
				stat_name_dir->cd();
				for(int div:divs) {
					centralities_stat2(def_comps["raw"], sys_comps["raw"], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
					centralities_stat2(def_comps["pull_raw"], sys_comps["pull_raw"], name, all_centralities, {div}, "centralities_pull_raw_"+name+to_string(div));
//						centralities_stat2(def_comps["pull_raw"], sys_comps["pull_raw"], name, all_centralities, {div}, "centralities_pull_cor_"+name+to_string(div));
					centralities_stat2(def_comps["diff"], sys_comps["diff"], name, all_centralities, {div}, "centralities_diff_"+name+to_string(div));
				}
				centralities_stat(def_comps["raw"], sys_comps["raw"], name, all_centralities, divs, "centralities_raw_"+name);
				centralities_stat(def_comps["pull_raw"], sys_comps["pull_raw"], name, all_centralities, divs, "centralities_pull_raw_"+name);
			}
		}
		TDirectory *dist_mean_dir = data_dir->mkdir("dist_means");
		for(pair<string, vector<string>> stat_type:names) {
			TDirectory *name_dir = dist_mean_dir->mkdir(stat_type.first.data());
			name_dir->cd();
			for(string name:stat_type.second) {
				TDirectory *stat_name_dir = name_dir->mkdir(name.data());
				stat_name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(def_comps["raw"], sys_comps["raw"], name, all_centralities, {div}, "stat_vs_mult_mean_raw_"+name+to_string(div));
					stat_vs_mult_mean(def_comps["pull_raw"], sys_comps["pull_raw"], name, all_centralities, {div}, "stat_vs_mult_mean_pull_raw_"+name+to_string(div));
					stat_vs_mult_mean(def_comps["diff"], sys_comps["diff"], name, all_centralities, {div}, "stat_vs_mult_mean_diff_"+name+to_string(div));
//						stat_vs_mult_mean_cor(def_comps["pull"], sys_comps["pull"], name, all_centralities, {div}, "stat_vs_mult_mean_cor_pull_"+name+to_string(div));
				}
				stat_vs_mult_mean(def_comps["raw"], sys_comps["raw"], name, all_centralities, divs, "stat_vs_mult_mean_raw_"+name);
				stat_vs_mult_mean(def_comps["pull_raw"], sys_comps["pull_raw"], name, all_centralities, divs, "stat_vs_mult_mean_pull_raw_"+name);
			}
		}
		TDirectory *divs_dir = data_dir->mkdir("divs");
		for(pair<string, vector<string>> stat_type:names) {
			TDirectory *name_dir = divs_dir->mkdir(stat_type.first.data());
			name_dir->cd();
			for(string name:stat_type.second) {
				TDirectory *stat_name_dir = name_dir->mkdir(name.data());
				stat_name_dir->cd();
				for(int energy:energy_list) {
					division_stat(def_comps["raw"], sys_comps["raw"], name, all_centralities, divs, {energy}, "divs_raw_"+name+to_string(energy)+"GeV");
					division_stat(def_comps["pull_raw"], sys_comps["pull_raw"], name, all_centralities, divs, {energy}, "divs_pull_raw_"+name+to_string(energy)+"GeV");
				}
				division_stat(def_comps["raw"], sys_comps["raw"], name, all_centralities, divs, energy_list, "divs_raw_"+name);
				division_stat(def_comps["pull_raw"], sys_comps["pull_raw"], name, all_centralities, divs, energy_list, "divs_pull_raw_"+name);
			}
		}
	}

	TDirectory *mix_dir = comb_dir->mkdir("Mix_Data");
	mix_dir->cd();

	{
		TDirectory *roli_thesis_dir = mix_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(def_comps["mix"], sys_comps["mix"], name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
				roli_thesis_stats(def_comps["pull_mix"], sys_comps["pull_mix"], name.second, centralities, {div}, "roli_thesis_pull_mix_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = mix_dir->mkdir("centralities");
		for(pair<string, vector<string>> stat_type:names) {
			TDirectory *name_dir = centralities_dir->mkdir(stat_type.first.data());
			name_dir->cd();
			for(string name:stat_type.second) {
				TDirectory *stat_name_dir = name_dir->mkdir(name.data());
				stat_name_dir->cd();
				for(int div:divs) {
					centralities_stat2(def_comps["mix"], sys_comps["mix"], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
					centralities_stat2(def_comps["pull_mix"], sys_comps["pull_mix"], name, all_centralities, {div}, "centralities_pull_mix_"+name+to_string(div));
				}
				centralities_stat(def_comps["mix"], sys_comps["mix"], name, all_centralities, divs, "centralities_mix_"+name);
				centralities_stat(def_comps["pull_mix"], sys_comps["pull_mix"], name, all_centralities, divs, "centralities_pull_mix_"+name);
			}
		}
		TDirectory *dist_mean_dir = mix_dir->mkdir("dist_means");
		for(pair<string, vector<string>> stat_type:names) {
			TDirectory *name_dir = dist_mean_dir->mkdir(stat_type.first.data());
			name_dir->cd();
			for(string name:stat_type.second) {
				TDirectory *stat_name_dir = name_dir->mkdir(name.data());
				stat_name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(def_comps["mix"], sys_comps["mix"], name, all_centralities, {div}, "stat_vs_mult_mean_mix_"+name+to_string(div));
					stat_vs_mult_mean(def_comps["pull_mix"], sys_comps["pull_mix"], name, all_centralities, {div}, "stat_vs_mult_mean_pull_mix_"+name+to_string(div));
				}
				stat_vs_mult_mean(def_comps["mix"], sys_comps["mix"], name, all_centralities, divs, "stat_vs_mult_mean_mix_"+name);
				stat_vs_mult_mean(def_comps["pull_mix"], sys_comps["pull_mix"], name, all_centralities, divs, "stat_vs_mult_mean_pull_mix_"+name);
			}
		}
		TDirectory *divs_dir = mix_dir->mkdir("divs");
		for(pair<string, vector<string>> stat_type:names) {
			TDirectory *name_dir = divs_dir->mkdir(stat_type.first.data());
			name_dir->cd();
			for(string name:stat_type.second) {
				TDirectory *stat_name_dir = name_dir->mkdir(name.data());
				stat_name_dir->cd();
				for(int energy:energy_list) {
					division_stat(def_comps["mix"], sys_comps["mix"], name, all_centralities, divs, {energy}, "divs_mix_"+name+to_string(energy)+"GeV");
					division_stat(def_comps["pull_mix"], sys_comps["pull_mix"], name, all_centralities, divs, {energy}, "divs_pull_mix_"+name+to_string(energy)+"GeV");
				}
				division_stat(def_comps["mix"], sys_comps["mix"], name, all_centralities, divs, energy_list, "divs_mix_"+name);
				division_stat(def_comps["pull_mix"], sys_comps["pull_mix"], name, all_centralities, divs, energy_list, "divs_pull_mix_"+name);
			}
		}
	}


	TDirectory *mix_div_dir = comb_dir->mkdir("Mix_Divded_Data");
	mix_div_dir->cd();

	{
		TDirectory *roli_thesis_dir = mix_div_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(def_comps["divide"], sys_comps["divide"], name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
				roli_thesis_stats(def_comps["pull_divide"], sys_comps["pull_divide"], name.second, centralities, {div}, "roli_thesis_pull_divide_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = mix_div_dir->mkdir("centralities");
		for(pair<string, vector<string>> stat_type:names) {
			TDirectory *name_dir = centralities_dir->mkdir(stat_type.first.data());
			name_dir->cd();
			for(string name:stat_type.second) {
				TDirectory *stat_name_dir = name_dir->mkdir(name.data());
				stat_name_dir->cd();
				for(int div:divs) {
					centralities_stat2(def_comps["divide"], sys_comps["divide"], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
//						centralities_stat2(def_comps["divide"], sys_comps["divide"], name, all_centralities, {div}, "centralities_divide_cor_"+name+to_string(div));
					centralities_stat2(def_comps["pull_divide"], sys_comps["pull_divide"], name, all_centralities, {div}, "centralities_pull_divide_"+name+to_string(div));
				}
				centralities_stat(def_comps["divide"], sys_comps["divide"], name, all_centralities, divs, "centralities_divide_"+name);
				centralities_stat(def_comps["pull_divide"], sys_comps["pull_divide"], name, all_centralities, divs, "centralities_pull_divide_"+name);
			}
		}
		TDirectory *dist_means_dir = mix_div_dir->mkdir("dist_means");
		for(pair<string, vector<string>> stat_type:names) {
			TDirectory *name_dir = dist_means_dir->mkdir(stat_type.first.data());
			name_dir->cd();
			for(string name:stat_type.second) {
				TDirectory *stat_name_dir = name_dir->mkdir(name.data());
				stat_name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(def_comps["divide"], sys_comps["divide"], name, all_centralities, {div}, "stat_vs_mult_mean_divide_"+name+to_string(div));
					stat_vs_mult_mean(def_comps["pull_divide"], sys_comps["pull_divide"], name, all_centralities, {div}, "stat_vs_mult_mean_pull_divide_"+name+to_string(div));
//						stat_vs_mult_mean_cor(def_comps["divide"], sys_comps["divide"], name, all_centralities, {div}, "stat_vs_mult_mean_cor_divide_"+name+to_string(div));
				}
				stat_vs_mult_mean(def_comps["divide"], sys_comps["divide"], name, all_centralities, divs, "stat_vs_mult_mean_divide_"+name);
				stat_vs_mult_mean(def_comps["pull_divide"], sys_comps["pull_divide"], name, all_centralities, divs, "stat_vs_mult_mean_pull_divide_"+name);
			}
		}
		TDirectory *divs_dir = mix_div_dir->mkdir("divs");
		for(pair<string, vector<string>> stat_type:names) {
			TDirectory *name_dir = divs_dir->mkdir(stat_type.first.data());
			name_dir->cd();
			for(string name:stat_type.second) {
				TDirectory *stat_name_dir = name_dir->mkdir(name.data());
				stat_name_dir->cd();
				for(int energy:energy_list) {
					division_stat(def_comps["divide"], sys_comps["divide"], name, all_centralities, divs, {energy}, "divs_divide_"+name+to_string(energy)+"GeV");
					division_stat(def_comps["pull_divide"], sys_comps["pull_divide"], name, all_centralities, divs, {energy}, "divs_pull_divide_"+name+to_string(energy)+"GeV");
				}
				division_stat(def_comps["divide"], sys_comps["divide"], name, all_centralities, divs, energy_list, "divs_divide_"+name);
				division_stat(def_comps["pull_divide"], sys_comps["pull_divide"], name, all_centralities, divs, energy_list, "divs_pull_divide_"+name);
			}
		}
	}

}



void AzimuthBinAnalyzer::combine_set(string set_name, TDirectory *set_dir) {
	cout << "Combining set_nums" << endl;

	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_stats_all;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> mix_stats_all;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> divide_stats_all;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> pull_raw_stats_all;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> pull_mix_stats_all;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> diff_stats_all;
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> pull_divide_stats_all;

	// Calculate standard deviations for systematics
	for(pair<int, map<int, map<int, map<string, vector<Measure>>>>> energy:raw_stats_sets[set_name]) {
		for(pair<int, map<int, map<string, vector<Measure>>>> div:energy.second) {
			for(pair<int, map<string, vector<Measure>>> cent:div.second) {
				for(pair<string, vector<Measure>> stat:cent.second) {
					raw_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					mix_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					divide_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_raw_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(pull_raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_mix_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(pull_mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					diff_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(diff_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_divide_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(pull_divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);

					raw_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					mix_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					divide_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_raw_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(pull_raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_mix_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(pull_mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					diff_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(diff_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_divide_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(pull_divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);

					for(int i=0; i<(int)raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].size(); i++) {
						raw_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
						mix_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
						divide_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
						pull_raw_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = pull_raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
						pull_mix_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = pull_mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
						diff_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = diff_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
						pull_divide_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = pull_divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
					}
				}
			}
		}
	}


	TDirectory *comb_dir = set_dir->mkdir("Combined");

	TDirectory *comp_dir = comb_dir->mkdir("Comparison");
	comp_dir->cd();
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mixed = {{"raw",raw_stats_median[set_name]}, {"mixed",mix_stats_median[set_name]}};
	map<string, map<int, map<int, map<int, map<string, double>>>>> raw_mixed_sd = {{"raw",raw_stats_sd[set_name]}, {"mixed",mix_stats_sd[set_name]}};
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mixed_pull = {{"raw",pull_raw_stats_median[set_name]}, {"mixed",pull_mix_stats_median[set_name]}};
	map<string, map<int, map<int, map<int, map<string, double>>>>> raw_mixed_pull_sd = {{"raw",pull_raw_stats_sd[set_name]}, {"mixed",pull_mix_stats_sd[set_name]}};

	{
		TDirectory *roli_thesis_dir = comp_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(raw_mixed, raw_mixed_sd, name.second, centralities, {div}, "roli_thesis_raw_mix_comp_"+name.first+to_string(div));
				roli_thesis_stats(raw_mixed_pull, raw_mixed_pull_sd, name.second, centralities, {div}, "roli_thesis_raw_mix_pull_comp_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = comp_dir->mkdir("centralities");
		for(string name:names["stat"]) {
			TDirectory *name_dir = centralities_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				centralities_stat(raw_mixed, raw_mixed_sd, name, all_centralities, {div}, "centralities_raw_mix_comp_"+name+to_string(div));
				centralities_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, {div}, "centralities_raw_mix_pull_comp_"+name+to_string(div));
			}
			centralities_stat(raw_mixed, raw_mixed_sd, name, all_centralities, divs, "centralities_raw_mix_comp_"+name);
			centralities_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, "centralities_raw_mix_pull_comp_"+name);
		}
		TDirectory *dist_means_dir = comp_dir->mkdir("dist_means");
		for(string name:names["stat"]) {
			TDirectory *name_dir = dist_means_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				stat_vs_mult_mean(raw_mixed, raw_mixed_sd, name, all_centralities, {div}, "stat_vs_mult_mean_raw_mix_comp_"+name+to_string(div));
				stat_vs_mult_mean(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, {div}, "stat_vs_mult_mean_raw_mix_pull_comp_"+name+to_string(div));
			}
			stat_vs_mult_mean(raw_mixed, raw_mixed_sd, name, all_centralities, divs, "stat_vs_mult_mean_raw_mix_comp_"+name);
			stat_vs_mult_mean(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, "stat_vs_mult_mean_raw_mix_pull_comp_"+name);
		}
		TDirectory *div_dir = comp_dir->mkdir("divs");
		for(string name:names["stat"]) {
			TDirectory *name_dir = div_dir->mkdir(name.data());
			name_dir->cd();
			for(int energy:energy_list) {
				division_stat(raw_mixed, raw_mixed_sd, name, all_centralities, divs, {energy}, "divs_raw_mix_comp_"+name+to_string(energy)+"GeV");
				division_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, {energy}, "divs_raw_mix_pull_comp_"+name+to_string(energy)+"GeV");
			}
			division_stat(raw_mixed, raw_mixed_sd, name, all_centralities, divs, energy_list, "divs_raw_mix_comp_"+name);
			division_stat(raw_mixed_pull, raw_mixed_pull_sd, name, all_centralities, divs, energy_list, "divs_raw_mix_pull_comp_"+name);
		}
	}


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
					roli_thesis_stats(raw_stats_median[set_name], raw_stats_sd[set_name], name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
					roli_thesis_stats(pull_raw_stats_median[set_name], pull_raw_stats_sd[set_name], name.second, centralities, {div}, "roli_thesis_pull_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = median_dir->mkdir("centralities");
			for(string name:names["stat"]) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(raw_stats_median[set_name], raw_stats_sd[set_name], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
					centralities_stat(pull_raw_stats_median[set_name], pull_raw_stats_sd[set_name], name, all_centralities, {div}, "centralities_pull_"+name+to_string(div));
					centralities_stat(diff_stats_median[set_name], diff_stats_sd[set_name], name, all_centralities, {div}, "centralities_diff_"+name+to_string(div));
				}
				centralities_stat(raw_stats_median[set_name], raw_stats_sd[set_name], name, all_centralities, divs, "centralities_raw_"+name);
				centralities_stat(pull_raw_stats_median[set_name], pull_raw_stats_sd[set_name], name, all_centralities, divs, "centralities_pull_"+name);
			}
			TDirectory *dist_means_dir = median_dir->mkdir("dist_means");
			for(string name:names["stat"]) {
				TDirectory *name_dir = dist_means_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(raw_stats_median[set_name], raw_stats_sd[set_name], name, all_centralities, {div}, "stat_vs_mult_mean_raw_"+name+to_string(div));
					stat_vs_mult_mean(pull_raw_stats_median[set_name], pull_raw_stats_sd[set_name], name, all_centralities, {div}, "stat_vs_mult_mean_pull_"+name+to_string(div));
					stat_vs_mult_mean(diff_stats_median[set_name], diff_stats_sd[set_name], name, all_centralities, {div}, "stat_vs_mult_mean_diff_"+name+to_string(div));
				}
				stat_vs_mult_mean(raw_stats_median[set_name], raw_stats_sd[set_name], name, all_centralities, divs, "stat_vs_mult_mean_raw_"+name);
				stat_vs_mult_mean(pull_raw_stats_median[set_name], pull_raw_stats_sd[set_name], name, all_centralities, divs, "stat_vs_mult_mean_pull_"+name);
			}
			TDirectory *div_dir = median_dir->mkdir("divs");
			for(string name:names["stat"]) {
				TDirectory *name_dir = div_dir->mkdir(name.data());
				name_dir->cd();
				for(int energy:energy_list) {
					division_stat(raw_stats_median[set_name], raw_stats_sd[set_name], name, all_centralities, divs, {energy}, "divs_raw_"+name+to_string(energy)+"GeV");
					division_stat(pull_raw_stats_median[set_name], pull_raw_stats_sd[set_name], name, all_centralities, divs, {energy}, "divs_raw_pull_"+name+to_string(energy)+"GeV");
					division_stat(diff_stats_median[set_name], diff_stats_sd[set_name], name, all_centralities, divs, {energy}, "divs_raw_diff_"+name+to_string(energy)+"GeV");
				}
				division_stat(raw_stats_median[set_name], raw_stats_sd[set_name], name, all_centralities, divs, energy_list, "divs_raw_"+name);
				division_stat(pull_raw_stats_median[set_name], pull_raw_stats_sd[set_name], name, all_centralities, divs, energy_list, "divs_raw_pull_"+name);
				division_stat(diff_stats_median[set_name], diff_stats_sd[set_name], name, all_centralities, divs, energy_list, "divs_raw_diff_"+name);
			}
		}

		TDirectory *all_dir = data_dir->mkdir("All");

		{
			TDirectory *roli_thesis_dir = all_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(raw_stats_all, name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
					roli_thesis_stats(pull_raw_stats_all, name.second, centralities, {div}, "roli_thesis_raw_pull_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = all_dir->mkdir("centralities");
			for(string name:names["stat"]) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(raw_stats_all, name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
					centralities_stat(pull_raw_stats_all, name, all_centralities, {div}, "centralities_pull_"+name+to_string(div));
					centralities_stat(diff_stats_all, name, all_centralities, {div}, "centralities_diff_"+name+to_string(div));
				}
				centralities_stat(raw_stats_all, name, all_centralities, divs, "centralities_raw_"+name);
				centralities_stat(pull_raw_stats_all, name, all_centralities, divs, "centralities_pull_"+name);
				centralities_stat(diff_stats_all, name, all_centralities, divs, "centralities_diff_"+name);
			}
			TDirectory *dist_means_dir = all_dir->mkdir("dist_means");
			for(string name:names["stat"]) {
				TDirectory *name_dir = dist_means_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(raw_stats_all, name, all_centralities, {div}, "stat_vs_mult_mean_raw_"+name+to_string(div));
					stat_vs_mult_mean(pull_raw_stats_all, name, all_centralities, {div}, "stat_vs_mult_mean_pull_"+name+to_string(div));
					stat_vs_mult_mean(diff_stats_all, name, all_centralities, {div}, "stat_vs_mult_mean_diff_"+name+to_string(div));
				}
				stat_vs_mult_mean(raw_stats_all, name, all_centralities, divs, "stat_vs_mult_mean_raw_"+name);
				stat_vs_mult_mean(pull_raw_stats_all, name, all_centralities, divs, "stat_vs_mult_mean_pull_"+name);
				stat_vs_mult_mean(diff_stats_all, name, all_centralities, divs, "stat_vs_mult_mean_diff_"+name);
			}
			TDirectory *div_dir = all_dir->mkdir("divs");
			for(string name:names["stat"]) {
				TDirectory *name_dir = div_dir->mkdir(name.data());
				name_dir->cd();
				for(int energy:energy_list) {
					division_stat(raw_stats_all, name, all_centralities, divs, {energy}, "divs_raw_"+name+to_string(energy)+"GeV");
					division_stat(pull_raw_stats_all, name, all_centralities, divs, {energy}, "divs_raw_pull_"+name+to_string(energy)+"GeV");
					division_stat(diff_stats_all, name, all_centralities, divs, {energy}, "divs_raw_diff_"+name+to_string(energy)+"GeV");
				}
				division_stat(raw_stats_all, name, all_centralities, divs, energy_list, "divs_raw_"+name);
				division_stat(pull_raw_stats_all, name, all_centralities, divs, energy_list, "divs_raw_pull_"+name);
				division_stat(diff_stats_all, name, all_centralities, divs, energy_list, "divs_raw_diff_"+name);
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
					roli_thesis_stats(mix_stats_median[set_name], mix_stats_sd[set_name], name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
					roli_thesis_stats(pull_mix_stats_median[set_name], pull_mix_stats_sd[set_name], name.second, centralities, {div}, "roli_thesis_pull_mix_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = median_dir->mkdir("centralities");
			for(string name:names["stat"]) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(mix_stats_median[set_name], mix_stats_sd[set_name], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
					centralities_stat(pull_mix_stats_median[set_name], pull_mix_stats_sd[set_name], name, all_centralities, {div}, "centralities_pull_mix_"+name+to_string(div));
				}
				centralities_stat(mix_stats_median[set_name], mix_stats_sd[set_name], name, all_centralities, divs, "centralities_mix_"+name);
				centralities_stat(pull_mix_stats_median[set_name], pull_mix_stats_sd[set_name], name, all_centralities, divs, "centralities_pull_mix_"+name);
			}
			TDirectory *dist_mean_dir = median_dir->mkdir("dist_means");
			for(string name:names["stat"]) {
				TDirectory *name_dir = dist_mean_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(mix_stats_median[set_name], mix_stats_sd[set_name], name, all_centralities, {div}, "stat_vs_mult_mean_mix_"+name+to_string(div));
					stat_vs_mult_mean(pull_mix_stats_median[set_name], pull_mix_stats_sd[set_name], name, all_centralities, {div}, "stat_vs_mult_mean_pull_mix_"+name+to_string(div));
				}
				stat_vs_mult_mean(mix_stats_median[set_name], mix_stats_sd[set_name], name, all_centralities, divs, "stat_vs_mult_mean_mix_"+name);
				stat_vs_mult_mean(pull_mix_stats_median[set_name], pull_mix_stats_sd[set_name], name, all_centralities, divs, "stat_vs_mult_mean_pull_mix_"+name);
			}
			TDirectory *div_dir = median_dir->mkdir("divs");
			for(string name:names["stat"]) {
				TDirectory *name_dir = div_dir->mkdir(name.data());
				name_dir->cd();
				for(int energy:energy_list) {
					division_stat(mix_stats_median[set_name], mix_stats_sd[set_name], name, all_centralities, divs, {energy}, "divs_mix_"+name+to_string(energy)+"GeV");
					division_stat(pull_mix_stats_median[set_name], pull_mix_stats_sd[set_name], name, all_centralities, divs, {energy}, "divs_mix_pull_"+name+to_string(energy)+"GeV");
				}
				division_stat(mix_stats_median[set_name], mix_stats_sd[set_name], name, all_centralities, divs, energy_list, "divs_mix_"+name);
				division_stat(pull_mix_stats_median[set_name], pull_mix_stats_sd[set_name], name, all_centralities, divs, energy_list, "divs_mix_pull_"+name);
			}
		}

		TDirectory *all_dir = mix_dir->mkdir("All");

		{
			TDirectory *roli_thesis_dir = all_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(mix_stats_all, name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
					roli_thesis_stats(pull_mix_stats_all, name.second, centralities, {div}, "roli_thesis_pull_mix_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = all_dir->mkdir("centralities");
			for(string name:names["stat"]) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(mix_stats_all, name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
					centralities_stat(pull_mix_stats_all, name, all_centralities, {div}, "centralities_pull_mix_"+name+to_string(div));
				}
				centralities_stat(mix_stats_all, name, all_centralities, divs, "centralities_mix_"+name);
				centralities_stat(pull_mix_stats_all, name, all_centralities, divs, "centralities_pull_mix_"+name);
			}
			TDirectory *dist_means_dir = all_dir->mkdir("dist_means");
			for(string name:names["stat"]) {
				TDirectory *name_dir = dist_means_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(mix_stats_all, name, all_centralities, {div}, "stat_vs_mult_mean_mix_"+name+to_string(div));
					stat_vs_mult_mean(pull_mix_stats_all, name, all_centralities, {div}, "stat_vs_mult_mean_pull_mix_"+name+to_string(div));
				}
				stat_vs_mult_mean(mix_stats_all, name, all_centralities, divs, "stat_vs_mult_mean_mix_"+name);
				stat_vs_mult_mean(pull_mix_stats_all, name, all_centralities, divs, "stat_vs_mult_mean_pull_mix_"+name);
			}
			TDirectory *div_dir = all_dir->mkdir("divs");
			for(string name:names["stat"]) {
				TDirectory *name_dir = div_dir->mkdir(name.data());
				name_dir->cd();
				for(int energy:energy_list) {
					division_stat(mix_stats_all, name, all_centralities, divs, {energy}, "divs_mix_"+name+to_string(energy)+"GeV");
					division_stat(pull_mix_stats_all, name, all_centralities, divs, {energy}, "divs_mix_pull_"+name+to_string(energy)+"GeV");
				}
				division_stat(mix_stats_all, name, all_centralities, divs, energy_list, "divs_mix_"+name);
				division_stat(pull_mix_stats_all, name, all_centralities, divs, energy_list, "divs_mix_pull_"+name);
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
					roli_thesis_stats(divide_stats_median[set_name], divide_stats_sd[set_name], name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
					roli_thesis_stats(pull_divide_stats_median[set_name], pull_divide_stats_sd[set_name], name.second, centralities, {div}, "roli_thesis_pull_divide_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = median_dir->mkdir("centralities");
			for(string name:names["stat"]) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(divide_stats_median[set_name], divide_stats_sd[set_name], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
					centralities_stat(pull_divide_stats_median[set_name], pull_divide_stats_sd[set_name], name, all_centralities, {div}, "centralities_pull_divide_"+name+to_string(div));
				}
				centralities_stat(divide_stats_median[set_name], divide_stats_sd[set_name], name, all_centralities, divs, "centralities_divide_"+name);
				centralities_stat(pull_divide_stats_median[set_name], pull_divide_stats_sd[set_name], name, all_centralities, divs, "centralities_pull_divide_"+name);
			}
			TDirectory *dist_means_dir = median_dir->mkdir("dist_means");
			for(string name:names["stat"]) {
				TDirectory *name_dir = dist_means_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(divide_stats_median[set_name], divide_stats_sd[set_name], name, all_centralities, {div}, "stat_vs_mult_mean_divide_"+name+to_string(div));
					stat_vs_mult_mean(pull_divide_stats_median[set_name], pull_divide_stats_sd[set_name], name, all_centralities, {div}, "stat_vs_mult_mean_pull_divide_"+name+to_string(div));
				}
				stat_vs_mult_mean(divide_stats_median[set_name], divide_stats_sd[set_name], name, all_centralities, divs, "stat_vs_mult_mean_divide_"+name);
				stat_vs_mult_mean(pull_divide_stats_median[set_name], pull_divide_stats_sd[set_name], name, all_centralities, divs, "stat_vs_mult_mean_pull_divide_"+name);
			}
			TDirectory *divs_dir = median_dir->mkdir("divs");
			for(string name:names["stat"]) {
				TDirectory *name_dir = divs_dir->mkdir(name.data());
				name_dir->cd();
				for(int energy:energy_list) {
					division_stat(divide_stats_median[set_name], divide_stats_sd[set_name], name, all_centralities, divs, {energy}, "divs_divide_"+name+to_string(energy)+"GeV");
					division_stat(pull_divide_stats_median[set_name], pull_divide_stats_sd[set_name], name, all_centralities, divs, {energy}, "divs_pull_divide_"+name+to_string(energy)+"GeV");
				}
				division_stat(divide_stats_median[set_name], divide_stats_sd[set_name], name, all_centralities, divs, energy_list, "divs_divide_"+name);
				division_stat(pull_divide_stats_median[set_name], pull_divide_stats_sd[set_name], name, all_centralities, divs, energy_list, "divs_pull_divide_"+name);
			}
		}

		TDirectory *all_dir = mix_div_dir->mkdir("All");

		{
			TDirectory *roli_thesis_dir = all_dir->mkdir("roli_thesis");
			for(pair<string, vector<string>> name:names) {
				TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
				name_dir->cd();
				for(int div:divs) {
					roli_thesis_stats(divide_stats_all, name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
					roli_thesis_stats(pull_divide_stats_all, name.second, centralities, {div}, "roli_thesis_pull_divide_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = all_dir->mkdir("centralities");
			for(string name:names["stat"]) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(divide_stats_all, name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
					centralities_stat(pull_divide_stats_all, name, all_centralities, {div}, "centralities_pull_divide_"+name+to_string(div));
				}
				centralities_stat(divide_stats_all, name, all_centralities, divs, "centralities_divide_"+name);
				centralities_stat(pull_divide_stats_all, name, all_centralities, divs, "centralities_pull_divide_"+name);
			}
			TDirectory *dist_means_dir = all_dir->mkdir("dist_means");
			for(string name:names["stat"]) {
				TDirectory *name_dir = dist_means_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					stat_vs_mult_mean(divide_stats_all, name, all_centralities, {div}, "stat_vs_mult_mean_divide_"+name+to_string(div));
					stat_vs_mult_mean(pull_divide_stats_all, name, all_centralities, {div}, "stat_vs_mult_mean_pull_divide_"+name+to_string(div));
				}
				stat_vs_mult_mean(divide_stats_all, name, all_centralities, divs, "stat_vs_mult_mean_divide_"+name);
				stat_vs_mult_mean(pull_divide_stats_all, name, all_centralities, divs, "stat_vs_mult_mean_pull_divide_"+name);
			}
			TDirectory *divs_dir = all_dir->mkdir("divs");
			for(string name:names["stat"]) {
				TDirectory *name_dir = divs_dir->mkdir(name.data());
				name_dir->cd();
				for(int energy:energy_list) {
					division_stat(divide_stats_all, name, all_centralities, divs, {energy}, "divs_divide_"+name+to_string(energy)+"GeV");
					division_stat(pull_divide_stats_all, name, all_centralities, divs, {energy}, "divs_pull_divide_"+name+to_string(energy)+"GeV");
				}
				division_stat(divide_stats_all, name, all_centralities, divs, energy_list, "divs_divide_"+name);
				division_stat(pull_divide_stats_all, name, all_centralities, divs, energy_list, "divs_pull_divide_"+name);
			}
		}
	}
}


void AzimuthBinAnalyzer::combine_set_lite(string set_name) {
	cout << "Combining " << set_name << endl;

	//map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_stats_all;
	//map<string, map<int, map<int, map<int, map<string, Measure>>>>> mix_stats_all;
	//map<string, map<int, map<int, map<int, map<string, Measure>>>>> divide_stats_all;
	//map<string, map<int, map<int, map<int, map<string, Measure>>>>> pull_raw_stats_all;
	//map<string, map<int, map<int, map<int, map<string, Measure>>>>> pull_mix_stats_all;
	//map<string, map<int, map<int, map<int, map<string, Measure>>>>> diff_stats_all;
	//map<string, map<int, map<int, map<int, map<string, Measure>>>>> pull_divide_stats_all;

	// Calculate standard deviations for systematics
	bool print = true;
	for(pair<int, map<int, map<int, map<string, vector<Measure>>>>> energy:raw_stats_sets[set_name]) {
		for(pair<int, map<int, map<string, vector<Measure>>>> div:energy.second) {
			for(pair<int, map<string, vector<Measure>>> cent:div.second) {
				for(pair<string, vector<Measure>> stat:cent.second) {
					if (print) {
						cout << set_name << " number in set: " << raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].size() << endl;
						print = false;
					}
					raw_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					mix_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					divide_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_raw_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(pull_raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_mix_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(pull_mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					diff_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(diff_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_divide_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(pull_divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);

					raw_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					mix_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					divide_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_raw_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(pull_raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_mix_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(pull_mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					diff_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(diff_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					pull_divide_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(pull_divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);

					// Delete individual sets once combined into median and sd.
					// NEEDED FOR COMBINE SYSTEMATICS SYSTEMATICS!!!!
//					raw_stats_sets[set_name][energy.first][div.first][cent.first].erase(stat.first);
//					mix_stats_sets[set_name][energy.first][div.first][cent.first].erase(stat.first);
//					divide_stats_sets[set_name][energy.first][div.first][cent.first].erase(stat.first);
//					pull_raw_stats_sets[set_name][energy.first][div.first][cent.first].erase(stat.first);
//					pull_mix_stats_sets[set_name][energy.first][div.first][cent.first].erase(stat.first);
//					diff_stats_sets[set_name][energy.first][div.first][cent.first].erase(stat.first);
//					pull_divide_stats_sets[set_name][energy.first][div.first][cent.first].erase(stat.first);
				}
			}
		}
	}

	// Delete individual sets once combined into median and sd.
	// NEEDED FOR COMBINE SYSTEMATICS SYSTEMATICS!!!!
//	raw_stats_sets.erase(set_name);
//	mix_stats_sets.erase(set_name);
//	divide_stats_sets.erase(set_name);
//	pull_raw_stats_sets.erase(set_name);
//	pull_mix_stats_sets.erase(set_name);
//	diff_stats_sets.erase(set_name);
//	pull_divide_stats_sets.erase(set_name);
}


void AzimuthBinAnalyzer::analyze_subset(string set_name, int set_num, TDirectory *set_dir) {

	cout << "Starting Set " + set_name + to_string(set_num) << endl << endl;

	string set_name_file = split_string_by_char(set_name, '/').back();

	string path, path_mix;
	if (in_string(set_name, "Ampt")) {
		path = ampt_in_path + set_name + to_string(set_num) + "/";
		path_mix = ampt_in_mix_path + set_name + to_string(set_num) + "/";
	} else if (in_string(set_name, "CF")) {
		path = cf_in_path + set_name + to_string(set_num) + "/";
		path_mix = cf_in_mix_path + set_name + to_string(set_num) + "/";
	} else if (in_string(set_name, "Sim")) {
		path = sim_in_path + set_name + to_string(set_num) + "/";
		path_mix = sim_in_mix_path + set_name + to_string(set_num) + "/";
	} else {
		path = bes_in_path + set_name + to_string(set_num) + "/";
		path_mix = bes_in_mix_path + set_name + to_string(set_num) + "/";
	}

	map<int, map<int, map<int, AzimuthBinData>>> data = get_data(path);
	map<int, map<int, map<int, AzimuthBinData>>> data_mix = get_data(path_mix);

	auto ratio_stats = calculate_stats(data, "ratio");
	auto ratio_stats_mix = calculate_stats(data_mix, "ratio");

	auto diff_stats = calculate_stats(data, "diff");
	auto diff_stats_mix = calculate_stats(data_mix, "diff");

	auto diff_slice_stats_mix = calculate_mix_diff_sds(data_mix);

	map<int, map<int, map<int, map<string, Measure>>>> stats_mix_divide;

	for(auto &energy:ratio_stats) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &stat:cent.second) {
					if(stat.first == "particle_dist_mean") { stats_mix_divide[energy.first][div.first][cent.first][stat.first] = stat.second; }
					else { stats_mix_divide[energy.first][div.first][cent.first][stat.first] = stat.second / ratio_stats_mix[energy.first][div.first][cent.first][stat.first]; }

					raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stat.second);
					mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(ratio_stats_mix[energy.first][div.first][cent.first][stat.first]);
					divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stats_mix_divide[energy.first][div.first][cent.first][stat.first]);
					diff_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(diff_stats[energy.first][div.first][cent.first][stat.first]);
				}
			}
		}
	}

	for(auto &energy:data) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &particles:cent.second.get_bin_data()) {
					float p = (float)div.first / 360;
					cent.second.set_diff_slice_divisor(particles.first, sqrt(particles.first * p * (1 - p)));
				}
			}
		}
	}

	for(auto &energy:data_mix) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &particles:cent.second.get_bin_data()) {
					float p = (float)div.first / 360;
					cent.second.set_diff_slice_divisor(particles.first, sqrt(particles.first * p * (1 - p)));
				}
			}
		}
	}

	auto pull_stats = calculate_stats(data, "diff_slice_proj");
	auto pull_mix_stats = calculate_stats(data_mix, "diff_slice_proj");
	map<int, map<int, map<int, map<string, Measure>>>> pull_divide_stats;

	for(auto &energy:pull_stats) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &stat:cent.second) {
					pull_raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stat.second);
					pull_mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(pull_mix_stats[energy.first][div.first][cent.first][stat.first]);
					pull_divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stat.second / pull_mix_stats[energy.first][div.first][cent.first][stat.first]);
					pull_divide_stats[energy.first][div.first][cent.first][stat.first] = stat.second / pull_mix_stats[energy.first][div.first][cent.first][stat.first];
				}
			}
		}
	}

	TDirectory *set_num_dir = set_dir->mkdir((set_name_file + to_string(set_num)).data());

	TDirectory *data_dir = set_num_dir->mkdir("Raw_Data");
	data_dir->cd();
	if(plot_dists) {
		make_ratio_dist_plots(data_dir, data);
		make_diff_dist_plots(data_dir, data);
		make_2d_dist_plots(data_dir, data);
		make_proton_dist_plots(data_dir, data);
	}
	if(plot_dist_canvases) {
		make_canvas_plots(data_dir, data, all_centralities, divs);
		canvas_ratio_dists(data, plot_divs, plot_cents, "Ratio Distribution Canvas");
	}
	data_dir->cd();

	{
		TDirectory *roli_thesis_dir = data_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(ratio_stats, name.second, centralities, {div}, "roli_thesis_raw_ratio_"+name.first+to_string(div));
				roli_thesis_stats(diff_stats, name.second, centralities, {div}, "roli_thesis_raw_diff_"+name.first+to_string(div));
				roli_thesis_stats(pull_stats, name.second, centralities, {div}, "roli_thesis_raw_pull_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = data_dir->mkdir("centralities");
		for(string name:names["stat"]) {
			TDirectory *name_dir = centralities_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				centralities_stat(ratio_stats, name, all_centralities, {div}, "centralities_raw_ratio_"+name+to_string(div));
				centralities_stat(diff_stats, name, all_centralities, {div}, "centralities_raw_diff_"+name+to_string(div));
				centralities_stat(pull_stats, name, all_centralities, {div}, "centralities_raw_pull_"+name+to_string(div));
			}
			centralities_stat(ratio_stats, name, all_centralities, divs, "centralities_raw_ratio_"+name);
			centralities_stat(diff_stats, name, all_centralities, divs, "centralities_raw_diff_"+name);
			centralities_stat(pull_stats, name, all_centralities, divs, "centralities_raw_pull_"+name);
		}
		TDirectory *dist_means_dir = data_dir->mkdir("dist_means");
		for(string name:names["stat"]) {
			TDirectory *name_dir = dist_means_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				stat_vs_mult_mean(ratio_stats, name, all_centralities, {div}, "stat_vs_mult_mean_raw_ratio_"+name+to_string(div));
				stat_vs_mult_mean(diff_stats, name, all_centralities, {div}, "stat_vs_mult_mean_raw_diff_"+name+to_string(div));
				stat_vs_mult_mean(pull_stats, name, all_centralities, {div}, "stat_vs_mult_mean_raw_pull_"+name+to_string(div));
			}
			stat_vs_mult_mean(ratio_stats, name, all_centralities, divs, "stat_vs_mult_mean_raw_ratio_"+name);
			stat_vs_mult_mean(diff_stats, name, all_centralities, divs, "stat_vs_mult_mean_raw_diff_"+name);
			stat_vs_mult_mean(pull_stats, name, all_centralities, divs, "stat_vs_mult_mean_raw_pull_"+name);
		}
		TDirectory *div_dir = data_dir->mkdir("divs");
		for(string name:names["stat"]) {
			TDirectory *name_dir = div_dir->mkdir(name.data());
			name_dir->cd();
			for(int energy:energy_list) {
				division_stat(ratio_stats, name, all_centralities, divs, {energy}, "divs_raw_ratio_"+name+to_string(energy)+"GeV");
				division_stat(diff_stats, name, all_centralities, divs, {energy}, "divs_raw_diff_"+name+to_string(energy)+"GeV");
				division_stat(pull_stats, name, all_centralities, divs, {energy}, "divs_raw_pull_"+name+to_string(energy)+"GeV");
			}
			division_stat(ratio_stats, name, all_centralities, divs, energy_list, "divs_raw_ratio_"+name);
			division_stat(diff_stats, name, all_centralities, divs, energy_list, "divs_raw_diff_"+name);
			division_stat(pull_stats, name, all_centralities, divs, energy_list, "divs_raw_pull_"+name);
		}
	}


	TDirectory *mix_dir = set_num_dir->mkdir("Mix_Data");
	mix_dir->cd();
	if(plot_dists) {
		make_ratio_dist_plots(mix_dir, data_mix);
		make_diff_dist_plots(mix_dir, data_mix);
		make_2d_dist_plots(mix_dir, data_mix);
		make_proton_dist_plots(mix_dir, data_mix);
	}
	if(plot_dist_canvases) {
		make_canvas_plots(mix_dir, data_mix, all_centralities, divs);
		canvas_ratio_dists(data_mix, plot_divs, plot_cents, "Ratio Distribution Canvas");
	}
	mix_dir->cd();

	{
		TDirectory *roli_thesis_dir = mix_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(ratio_stats_mix, name.second, centralities, {div}, "roli_thesis_mix_ratio_"+name.first+to_string(div));
				roli_thesis_stats(diff_stats_mix, name.second, centralities, {div}, "roli_thesis_mix_diff_"+name.first+to_string(div));
				roli_thesis_stats(pull_mix_stats, name.second, centralities, {div}, "roli_thesis_mix_pull_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = mix_dir->mkdir("centralities");
		for(string name:names["stat"]) {
			TDirectory *name_dir = centralities_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				centralities_stat(ratio_stats_mix, name, all_centralities, {div}, "centralities_mix_ratio_"+name+to_string(div));
				centralities_stat(diff_stats_mix, name, all_centralities, {div}, "centralities_mix_diff_"+name+to_string(div));
				centralities_stat(pull_mix_stats, name, all_centralities, {div}, "centralities_mix_pull_"+name+to_string(div));
			}
			centralities_stat(ratio_stats_mix, name, all_centralities, divs, "centralities_mix_ratio_"+name);
			centralities_stat(diff_stats_mix, name, all_centralities, divs, "centralities_mix_diff_"+name);
			centralities_stat(pull_mix_stats, name, all_centralities, divs, "centralities_mix_pull_"+name);
		}
		TDirectory *dist_means_dir = mix_dir->mkdir("dist_means");
		for(string name:names["stat"]) {
			TDirectory *name_dir = dist_means_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				stat_vs_mult_mean(ratio_stats_mix, name, all_centralities, {div}, "stat_vs_mult_mean_mix_ratio_"+name+to_string(div));
				stat_vs_mult_mean(diff_stats_mix, name, all_centralities, {div}, "stat_vs_mult_mean_mix_diff_"+name+to_string(div));
				stat_vs_mult_mean(pull_mix_stats, name, all_centralities, {div}, "stat_vs_mult_mean_mix_diff_"+name+to_string(div));
			}
			stat_vs_mult_mean(ratio_stats_mix, name, all_centralities, divs, "stat_vs_mult_mean_mix_ratio_"+name);
			stat_vs_mult_mean(diff_stats_mix, name, all_centralities, divs, "stat_vs_mult_mean_mix_diff_"+name);
			stat_vs_mult_mean(pull_mix_stats, name, all_centralities, divs, "stat_vs_mult_mean_mix_pull_"+name);
		}
		TDirectory *div_dir = mix_dir->mkdir("divs");
		for(string name:names["stat"]) {  // This may be wrong, think accidentally edited
			TDirectory *name_dir = div_dir->mkdir(name.data());
			name_dir->cd();
			for(int energy:energy_list) {
				division_stat(ratio_stats_mix, name, all_centralities, divs, {energy}, "divs_mix_ratio_"+name+to_string(energy)+"GeV");
				division_stat(diff_stats_mix, name, all_centralities, divs, {energy}, "divs_mix_diff_"+name+to_string(energy)+"GeV");
				division_stat(pull_mix_stats, name, all_centralities, divs, {energy}, "divs_mix_pull_"+name+to_string(energy)+"GeV");
			}
			division_stat(ratio_stats_mix, name, all_centralities, divs, energy_list, "divs_mix_ratio_"+name);
			division_stat(diff_stats_mix, name, all_centralities, divs, energy_list, "divs_mix_diff_"+name);
			division_stat(pull_mix_stats, name, all_centralities, divs, energy_list, "divs_mix_pull_"+name);
		}
	}

	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mix_comp {{"raw", ratio_stats}, {"mixed", ratio_stats_mix}};
	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mix_comp_pull {{"raw", pull_stats}, {"mixed", pull_mix_stats}};

	TDirectory *mix_div_dir = set_num_dir->mkdir("Mix_Divded_Data");
	mix_div_dir->cd();

	{
		TDirectory *roli_thesis_dir = mix_div_dir->mkdir("roli_thesis");
		for(pair<string, vector<string>> name:names) {
			TDirectory *name_dir = roli_thesis_dir->mkdir(name.first.data());
			name_dir->cd();
			for(int div:divs) {
				roli_thesis_stats(stats_mix_divide, name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
				roli_thesis_stats(pull_divide_stats, name.second, centralities, {div}, "roli_thesis_divide_pull_"+name.first+to_string(div));
			}
		}
		TDirectory *centralities_dir = mix_div_dir->mkdir("centralities");
		for(string name:names["stat"]) {
			TDirectory *name_dir = centralities_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				centralities_stat(stats_mix_divide, name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
				centralities_stat(raw_mix_comp, name, all_centralities, {div}, "centralities_comp_"+name+to_string(div));
				centralities_stat(pull_divide_stats, name, all_centralities, {div}, "centralities_divide_pull_"+name+to_string(div));
				centralities_stat(raw_mix_comp_pull, name, all_centralities, {div}, "centralities_comp_pull_"+name+to_string(div));
			}
			centralities_stat(stats_mix_divide, name, all_centralities, divs, "centralities_divide_"+name);
			centralities_stat(raw_mix_comp, name, all_centralities, divs, "centralities_comp_"+name);
			centralities_stat(pull_divide_stats, name, all_centralities, divs, "centralities_divide_pull_"+name);
			centralities_stat(raw_mix_comp_pull, name, all_centralities, divs, "centralities_comp_pull_"+name);
		}
		TDirectory *dist_means_dir = mix_div_dir->mkdir("dist_means");
		for(string name:names["stat"]) {
			TDirectory *name_dir = dist_means_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				stat_vs_mult_mean(stats_mix_divide, name, all_centralities, {div}, "stat_vs_mult_mean_divide_"+name+to_string(div));
				stat_vs_mult_mean(raw_mix_comp, name, all_centralities, {div}, "stat_vs_mult_mean_comp_"+name+to_string(div));
				stat_vs_mult_mean(pull_divide_stats, name, all_centralities, {div}, "stat_vs_mult_mean_divide_pull_"+name+to_string(div));
				stat_vs_mult_mean(raw_mix_comp_pull, name, all_centralities, {div}, "stat_vs_mult_mean_comp_pull_"+name+to_string(div));
			}
			stat_vs_mult_mean(stats_mix_divide, name, all_centralities, divs, "stat_vs_mult_mean_divide_"+name);
			stat_vs_mult_mean(raw_mix_comp, name, all_centralities, divs, "stat_vs_mult_mean_comp_"+name);
			stat_vs_mult_mean(pull_divide_stats, name, all_centralities, divs, "stat_vs_mult_mean_divide_pull_"+name);
			stat_vs_mult_mean(raw_mix_comp_pull, name, all_centralities, divs, "stat_vs_mult_mean_comp_pull_"+name);
		}
		TDirectory *div_dir = mix_div_dir->mkdir("divs");
		for(string name:names["stat"]) {
			TDirectory *name_dir = div_dir->mkdir(name.data());
			name_dir->cd();
			for(int energy:energy_list) {
				division_stat(stats_mix_divide, name, all_centralities, divs, {energy}, "divs_divide_"+name+to_string(energy)+"GeV");
				division_stat(raw_mix_comp, name, all_centralities, divs, {energy}, "divs_comp_"+name+to_string(energy)+"GeV");
				division_stat(pull_divide_stats, name, all_centralities, divs, {energy}, "divs_divide_pull_"+name+to_string(energy)+"GeV");
				division_stat(raw_mix_comp_pull, name, all_centralities, divs, {energy}, "divs_comp_pull_"+name+to_string(energy)+"GeV");
			}
			division_stat(stats_mix_divide, name, all_centralities, divs, energy_list, "divs_divide_"+name);
			division_stat(raw_mix_comp, name, all_centralities, divs, energy_list, "divs_comp_"+name);
			division_stat(pull_divide_stats, name, all_centralities, divs, energy_list, "divs_divide_pull_"+name);
			division_stat(raw_mix_comp_pull, name, all_centralities, divs, energy_list, "divs_comp_pull_"+name);
		}
	}
}


map<int, map<int, map<int, AzimuthBinData>>> AzimuthBinAnalyzer::get_data(string in_path, int min_num_events) {
	map<int, map<int, map<int, AzimuthBinData>>> data;

	for(int energy:energy_list) {

		string path = in_path + to_string(energy) + "GeV/";

		for(int div:divs) {

			// Get centralities found in file names of given directory path.
			for(int cent:get_centrals(path, div)) {
				if(find(all_centralities.begin(), all_centralities.end(), cent) == all_centralities.end()) { continue; }
				AzimuthBinData az_data(div);
				az_data.read_data_from_dir(path, div, cent);  // Read azimuthal bin data from file path

				if(az_data.get_num_bins() <= min_num_events) {
					cout << "Too few entries " << az_data.get_num_bins() << " | " << to_string(az_data.get_num_bins() <= min_num_events) << " | " << path << endl;
					if(div == divs[0]) {
						cout << "Centrality " << cent << " with only " << az_data.get_num_bins() << " entries. Skipping." << endl;
					}
				} else {
					data[energy][div][cent] = az_data;  // Store azimuthal bin data in data under corresponding centrality (refmult2) value
				}

			}
		}
	}

	return data;
}


vector<string> AzimuthBinAnalyzer::get_sets(string set_dir) {
	string path;
	if (in_string(set_dir, "Ampt")) {
		path = ampt_in_path + set_dir + "/";
	}
	else if (in_string(set_dir, "CF")) {
		path = cf_in_path + set_dir + "/";
	}
	else if (in_string(set_dir, "Sim")) {
		path = sim_in_path + set_dir + "/";
	}
	else {
		path = bes_in_path + set_dir + "/";
	}
	return get_files_in_dir(path, "", "name", true);
}


void AzimuthBinAnalyzer::write_systematics() {
	string out_sys_path = out_path + out_sys_name;  // + "_" + split(out_root_name, ".")[0] + ".txt";
	cout << "Write systematic values to output file: " << out_sys_path << endl;

	ofstream file;
	if (write_append) {
		file.open(out_sys_path, ios_base::app);  // Append new sys values to existing file. app works but ate does not?
	} else {
		file.open(out_sys_path, ios_base::trunc);  // Overwrite existing file with new sys values
	}

	for (auto set_name : def_medians) {  // systematic data set name [string] (BES1, AMPT, etc)
		for (auto data_type : set_name.second) {  // data type [string] (raw, mix, divide, pull_raw, pull_mix, pull_divide, diff)
			for (auto energy : data_type.second) {  // energy [int] (7, 11, 19, ...)
				for (auto div : energy.second) {  // div [int] {60, 72, 90, ...)
					for (auto cent : div.second) {  // cent [int] {0, 1, ..., 8)
						for (auto stat : cent.second) {  // stat [string] (mean, standard_deviation, ...)
							ostringstream val, err, sys;
							val << fixed; err << fixed; sys << fixed;
							val << setprecision(20); err << setprecision(20); sys << setprecision(20);
							val << stat.second.get_val(); err << stat.second.get_err();
							sys << def_systematics[set_name.first][data_type.first][energy.first][div.first][cent.first][stat.first];
							string common = set_name.first + "," + data_type.first + "," + to_string(energy.first) + "," + to_string(div.first) + "," + to_string(cent.first) + "," + stat.first + ",";
							string med_out = "medians," + common + val.str() + "±" + err.str();
							string sys_out = "systematics," + common + sys.str();
							file << med_out << endl;
							file << sys_out << endl;
						}
					}
				}
			}
		}
	}

	file.close();
}


void AzimuthBinAnalyzer::read_systematics(string path) {
	def_medians.clear(); def_systematics.clear();  // Delete before reading in from file

	cout << "Reading systematics (def_medians and def_systematics) from " << path << endl;

	ifstream file(path);
	string line;
	int line_num = 0;

	while (getline(file, line)) {
		++line_num;
		vector<string> split_line = split(line, ',');
		if (split_line.size() != 8) {
			cout << "Wrong size of split line for line " << line_num << " of " << path << endl;
		} else {
			string set = split_line[1];
			string data_type = split_line[2];
			int energy = stoi(split_line[3]);
			int div = stoi(split_line[4]);
			int cent = stoi(split_line[5]);
			string stat = split_line[6];

			if (split_line[0] == "medians") {
				vector<string> val_err = split(split_line[7], "±");
				if (val_err.size() != 2) {
					cout << "Wrong size of split val/err for line " << line_num << " of " << path << endl;
				} else {
					def_medians[set][data_type][energy][div][cent][stat] = Measure(stod(val_err[0]), stod(val_err[1]));
				}
			} else if (split_line[0] == "systematics") {
				def_systematics[set][data_type][energy][div][cent][stat] = stod(split_line[7]);
			} else {
				cout << "Can't read first entry in line " << line_num << " of " << path << endl;
			}
		}
	}

	file.close();
}


//Calculate stats for each cumulant_order for each centrality for each number of divisions for each energy.
map<int, map<int, map<int, map<string, Measure>>>> AzimuthBinAnalyzer::calculate_stats(map<int, map<int, map<int, AzimuthBinData>>> data, string type) {
	map<int, map<int, map<int, map<string, Measure>>>> stats;
//	ROOT::EnableThreadSafety();
//	{
//	ThreadPool pool(1);//thread::hardware_concurrency());
	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		for(pair<int, map<int, AzimuthBinData>> div:energy.second) {
			for(pair<int, AzimuthBinData> cent:div.second) {
				calc_stat(&(data[energy.first][div.first][cent.first]), type, energy.first, div.first, cent.first, &stats);
//				pool.enqueue(&AzimuthBinAnalyzer::calc_stat, this, &(data[energy.first][div.first][cent.first]), type, energy.first, div.first, cent.first, orders, &stats);
			}
		}
	}
//	}

	return(stats);
}


//Calculate stats for each cumulant_order for each centrality for each number of divisions for each energy.
map<int, map<int, map<int, map<int, map<string, Measure>>>>> AzimuthBinAnalyzer::calculate_mix_diff_sds(map<int, map<int, map<int, AzimuthBinData>>> data) {
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> sds;

	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		for(pair<int, map<int, AzimuthBinData>> div:energy.second) {
			for(pair<int, AzimuthBinData> cent:div.second) {
				calc_mix_diff_sd(&(data[energy.first][div.first][cent.first]), energy.first, div.first, cent.first, &sds);
			}
		}
	}

	return(sds);
}


//void AzimuthBinAnalyzer::calc_stat(AzimuthBinData *data, string type, int energy, int div, int cent, vector<int> orders, map<int, map<int, map<int, map<string, Measure>>>> *stats) {
//	Stats stat;
//	if(type == "ratio") { stat.set_distribution(data->get_ratio_hist()); }
//	if(type == "diff") { stat.set_distribution(data->get_diff_hist()); }
//	if(type == "diff_slice_proj") { stat.set_distribution(data->get_diff_slice_proj()); }
//	(*stats)[energy][div][cent]["mean"] = stat.get_mean();
//	(*stats)[energy][div][cent]["standard_deviation"] = stat.get_standard_deviation();
//	(*stats)[energy][div][cent]["skewness"] = stat.get_skewness();
//	(*stats)[energy][div][cent]["kurtosis"] = stat.get_kurtosis();
//	(*stats)[energy][div][cent]["non_excess_kurtosis"] = stat.get_non_excess_kurtosis();
//	(*stats)[energy][div][cent]["kurtosis*variance"] = stat.get_kurt_var();
//	Stats dist_mean_stat;
//	dist_mean_stat.set_distribution(data->get_proton_dist());
//	(*stats)[energy][div][cent]["particle_dist_mean"] = dist_mean_stat.get_mean();
//
//	for(int order:orders) {
//		(*stats)[energy][div][cent]["cumulant "+to_string(order)] = stat.get_cumulant(order);
//		(*stats)[energy][div][cent]["raw moment "+to_string(order)] = stat.get_raw_moment(order);
//		(*stats)[energy][div][cent]["central moment "+to_string(order)] = stat.get_central_moment(order);
//	}
//	if(isnan(stat.get_standard_deviation().get_val()) || isnan(stat.get_standard_deviation().get_err())) {
//		cout << type << " " << energy << "GeV,  div: " << div << ", cent: " << cent << ", sd: " << (string)stat.get_standard_deviation() << endl;
//		//this_thread::sleep_for(chrono::seconds(1));
//	}
//}


void AzimuthBinAnalyzer::calc_stat(AzimuthBinData* data, string type, int energy, int div, int cent, map<int, map<int, map<int, map<string, Measure>>>>* stats) {
	Stats stat;
	if (type == "ratio") { stat.set_distribution(data->get_ratio_hist()); }
	if (type == "diff") { stat.set_distribution(data->get_diff_hist()); }
	if (type == "diff_slice_proj") { stat.set_distribution(data->get_diff_slice_proj()); }
	Stats dist_mean_stat;
	dist_mean_stat.set_distribution(data->get_proton_dist());
	(*stats)[energy][div][cent]["particle_dist_mean"] = dist_mean_stat.get_mean();
	stat.get_cumulant(4);  // Get 4th order cumulant to calculate relevant moments in one go.

	for (pair<string, vector<string>> stat_type : names) {
		for (string stat_name : stat_type.second) {
			if (stat_type.first == "cumulant" || stat_type.first == "raw_moment" || stat_type.first == "central_moment") {
				int order;
				try {
					order = stoi(split_string_by_char(stat_name, ' ').back());
				}
				catch (...) {
					cout << "calc_stat can't get order integer from back of " << stat_name << endl;
				}
				if (stat_type.first == "cumulant") { (*stats)[energy][div][cent]["cumulant " + to_string(order)] = stat.get_cumulant(order); }
				else if (stat_type.first == "raw_moment") { (*stats)[energy][div][cent]["raw moment " + to_string(order)] = stat.get_raw_moment(order); }
				else if (stat_type.first == "central_moment") { (*stats)[energy][div][cent]["central moment " + to_string(order)] = stat.get_central_moment(order); }
			}
			if (stat_name == "mean") { (*stats)[energy][div][cent]["mean"] = stat.get_mean(); }
			else if (stat_name == "standard_deviation") { (*stats)[energy][div][cent]["standard_deviation"] = stat.get_standard_deviation(); }
			else if (stat_name == "skewness") { (*stats)[energy][div][cent]["skewness"] = stat.get_skewness(); }
			else if (stat_name == "kurtosis") { (*stats)[energy][div][cent]["kurtosis"] = stat.get_kurtosis(); }
			else if (stat_name == "non_excess_kurtosis") { (*stats)[energy][div][cent]["non_excess_kurtosis"] = stat.get_non_excess_kurtosis(); }
			else if (stat_name == "kurtosis*variance") { (*stats)[energy][div][cent]["kurtosis*variance"] = stat.get_kurt_var(); }
		}
	}

	if (isnan(stat.get_standard_deviation().get_val()) || isnan(stat.get_standard_deviation().get_err())) {
		cout << type << " " << energy << "GeV,  div: " << div << ", cent: " << cent << ", sd: " << (string)stat.get_standard_deviation() << endl;
	}
}


void AzimuthBinAnalyzer::calc_mix_diff_sd(AzimuthBinData *data, int energy, int div, int cent, map<int, map<int, map<int, map<int, map<string, Measure>>>>> *stats) {
	auto dist = data->get_diff_slice_hist("no");
	for(auto particles:dist) {
		Stats stat;
		stat.set_distribution(particles.second);
		(*stats)[energy][div][cent][particles.first]["standard_deviation"] = stat.get_standard_deviation();
	}
}


double AzimuthBinAnalyzer::sample_sd(vector<double> data) {
	double mean = 0.0, sd = 0.0;
	for(double element:data) { mean += element; }
	mean /= (int)data.size();
	for(double element:data) { sd += pow(element - mean, 2); }
	sd = pow(sd / ((int)data.size() - 1), 0.5);

	return(sd);
}


double AzimuthBinAnalyzer::sample_sd(vector<Measure> data) {
	vector<double> val_vec;
	for(Measure element:data) {
		val_vec.push_back(element.get_val());
	}

	double sd = sample_sd(val_vec);

	return(sd);
}


double AzimuthBinAnalyzer::sample_sd_weight(vector<double> data, vector<double> weight) {
	if(data.size() != weight.size()) { cout << "sample_sd_weight arguments not same size! Returning 0 for sd." << endl; return 0;}
	double mean = 0.0, weights = 0.0, sd = 0.0;
	int nonzero_weights = 0;
	for(unsigned index=0; index < data.size(); index++) {
		mean += data[index] * weight[index];
		weights += weight[index];
		if(weight[index] > 0) {nonzero_weights++;}
	}
	mean /= weights;
	for(unsigned index=0; index < data.size(); index++) { sd += weight[index] * pow(data[index] - mean, 2); }
	sd = pow(sd / ((nonzero_weights - 1) * weights / nonzero_weights), 0.5);

	return(sd);
}


double AzimuthBinAnalyzer::sample_sd_errweight(vector<Measure> data) {
	vector<double> val_vec;
	vector<double> weight_vec;
	for(Measure element:data) {
		val_vec.push_back(element.get_val());
		weight_vec.push_back(1.0 / pow(element.get_err(), 2));
	}

	double sd = sample_sd_weight(val_vec, weight_vec);

	return(sd);
}

// Get median Measure of vector by value. If even number of Measures takes lower of two median options.
Measure AzimuthBinAnalyzer::median(vector<Measure> data) {
	sort(data.begin(), data.end(), [] (Measure a, Measure b) { return(a.get_val() < b.get_val()); } );
	return(data[(int)(data.size()-1) / 2]);
}


pair<string, int> get_set_name_num(string set) {
	unsigned index = set.rfind("_");
	if (index == string::npos) {
		cout << "No _ found in " << set << endl;
		return make_pair(set, 0);
		set = set.substr(0, set.rfind('_')) + "_";
	}
	else {
		string set_name = set.substr(0, index + 1);
		int set_num = stoi(set.substr(index + 1));
		return make_pair(set_name, set_num);
	}
}
