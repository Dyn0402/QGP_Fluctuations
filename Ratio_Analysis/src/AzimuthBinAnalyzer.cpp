/*
 * AzimuthBinAnalyzer.cpp
 *
 *  Created on: Jan 23, 2020
 *      Author: Dylan Neff
 */


#include <vector>

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
	out_root->Close();
}


// Getters


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

void AzimuthBinAnalyzer::set_can_wh(int can_width, int can_height) {
	plot::canvas_width = can_width;
	plot::canvas_height = can_height;
}

void AzimuthBinAnalyzer::set_set_combos(map<string, vector<string>> set_combos) {
	this->set_combos = set_combos;
}

void AzimuthBinAnalyzer::set_plot_dists(bool plot) {
	this->plot_dists = plot;
}

void AzimuthBinAnalyzer::set_plot_dist_canvases(bool plot) {
	this->plot_dist_canvases = plot;
}

void AzimuthBinAnalyzer::set_divs(vector<int> divisions) {
	this->divs = divisions;
}

// Doers

void AzimuthBinAnalyzer::analyze() {
	analysis::energy_list = energy_list;
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

	for(pair<string, vector<int>> set:sets) {
		analyze_set(set.first, set.second);
	}

	combine_sets();
}


void AzimuthBinAnalyzer::analyze_set(string set_name, vector<int> set_nums) {
	TDirectory *set_dir = out_root->mkdir(set_name.data());
	for(int set_num = set_nums[0]; set_num <= set_nums[1]; set_num++) {
		analyze_subset(set_name, set_num, set_dir);
	}

	combine_set(set_name, set_dir);
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
						division_stat(set_pair.second["raw"], set_pairs_sd[set_pair.first]["raw"], name, all_centralities, divs, {energy}, "divs_raw_"+name);
						division_stat(set_pair.second["pull_raw"], set_pairs_sd[set_pair.first]["pull_raw"], name, all_centralities, divs, {energy}, "divs_pull_raw_"+name);
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
						division_stat(set_pair.second["mix"], set_pairs_sd[set_pair.first]["mix"], name, all_centralities, divs, {energy}, "divs_mix_"+name);
						division_stat(set_pair.second["pull_mix"], set_pairs_sd[set_pair.first]["pull_mix"], name, all_centralities, divs, {energy}, "divs_pull_mix_"+name);
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
						division_stat(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, divs, {energy}, "divs_divide_"+name);
						division_stat(set_pair.second["pull_divide"], set_pairs_sd[set_pair.first]["pull_divide"], name, all_centralities, divs, {energy}, "divs_pull_divide_"+name);
					}
					division_stat(set_pair.second["divide"], set_pairs_sd[set_pair.first]["divide"], name, all_centralities, divs, energy_list, "divs_divide_"+name);
					division_stat(set_pair.second["pull_divide"], set_pairs_sd[set_pair.first]["pull_divide"], name, all_centralities, divs, energy_list, "divs_pull_divide_"+name);
				}
			}
		}
	}

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
//		for(string name:stat_names) {
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
//		for(string name:stat_names) {
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
//		for(string name:stat_names) {
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
//		for(string name:stat_names) {
//			TDirectory *name_dir = centralities_dir->mkdir(name.data());
//			name_dir->cd();
//			for(int div:divs) {
//				centralities_stat(divide_stats_median["Rand_Rotate"], divide_stats_sd["Rand_Rotate"], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
//			}
//		}
//	}
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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
				TDirectory *name_dir = divs_dir->mkdir(name.data());
				name_dir->cd();
				for(int energy:energy_list) {
					division_stat(divide_stats_median[set_name], divide_stats_sd[set_name], name, all_centralities, divs, {energy}, "divs_divide_"+name);
					division_stat(pull_divide_stats_median[set_name], pull_divide_stats_sd[set_name], name, all_centralities, divs, {energy}, "divs_pull_divide_"+name);
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
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
			for(string name:stat_names) {
				TDirectory *name_dir = divs_dir->mkdir(name.data());
				name_dir->cd();
				for(int energy:energy_list) {
					division_stat(divide_stats_all, name, all_centralities, divs, {energy}, "divs_divide_"+name);
					division_stat(pull_divide_stats_all, name, all_centralities, divs, {energy}, "divs_pull_divide_"+name);
				}
				division_stat(divide_stats_all, name, all_centralities, divs, energy_list, "divs_divide_"+name);
				division_stat(pull_divide_stats_all, name, all_centralities, divs, energy_list, "divs_pull_divide_"+name);
			}
		}
	}
}


void AzimuthBinAnalyzer::analyze_subset(string set_name, int set_num, TDirectory *set_dir) {

	cout << "Starting Set " + set_name + to_string(set_num) << endl << endl;

	string path, path_mix;
	if(in_string(set_name, "Ampt")) {
		path = ampt_in_path + set_name + to_string(set_num) + "/";
		path_mix = ampt_in_mix_path + set_name + to_string(set_num) + "/";
	} else if(in_string(set_name, "Sim")) {
		path = sim_in_path + set_name + to_string(set_num) + "/";
		path_mix = sim_in_mix_path + set_name + to_string(set_num) + "/";
	} else {
		path = bes_in_path + set_name + to_string(set_num) + "/";
		path_mix = bes_in_mix_path + set_name + to_string(set_num) + "/";
	}

	map<int, map<int, map<int, AzimuthBinData>>> data = get_data(path);
	map<int, map<int, map<int, AzimuthBinData>>> data_mix = get_data(path_mix);

	auto ratio_stats = calculate_stats(data, "ratio", orders);
	auto ratio_stats_mix = calculate_stats(data_mix, "ratio", orders);

	auto diff_stats = calculate_stats(data, "diff", orders);
	auto diff_stats_mix = calculate_stats(data_mix, "diff", orders);

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
					float p = 1 / (float)div.first;  // FIX!
					cent.second.set_diff_slice_divisor(particles.first, sqrt(particles.first * p * (1 - p)));
				}
			}
		}
	}

	for(auto &energy:data_mix) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &particles:cent.second.get_bin_data()) {
					float p = 1 / (float)div.first;  // FIX!
					cent.second.set_diff_slice_divisor(particles.first, sqrt(particles.first * p * (1 - p)));
				}
			}
		}
	}

	auto pull_stats = calculate_stats(data, "diff_slice_proj", orders);
	auto pull_mix_stats = calculate_stats(data_mix, "diff_slice_proj", orders);
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

	TDirectory *set_num_dir = set_dir->mkdir((set_name + to_string(set_num)).data());

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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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
		for(string name:stat_names) {
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


//Calculate stats for each cumulant_order for each centrality for each number of divisions for each energy.
map<int, map<int, map<int, map<string, Measure>>>> AzimuthBinAnalyzer::calculate_stats(map<int, map<int, map<int, AzimuthBinData>>> data, string type, vector<int> orders) {
	map<int, map<int, map<int, map<string, Measure>>>> stats;
//	ROOT::EnableThreadSafety();
//	{
//	ThreadPool pool(1);//thread::hardware_concurrency());
	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		for(pair<int, map<int, AzimuthBinData>> div:energy.second) {
			for(pair<int, AzimuthBinData> cent:div.second) {
				calc_stat(&(data[energy.first][div.first][cent.first]), type, energy.first, div.first, cent.first, orders, &stats);
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


void AzimuthBinAnalyzer::calc_stat(AzimuthBinData *data, string type, int energy, int div, int cent, vector<int> orders, map<int, map<int, map<int, map<string, Measure>>>> *stats) {
	Stats stat;
	if(type == "ratio") { stat.set_distribution(data->get_ratio_hist()); }
	if(type == "diff") { stat.set_distribution(data->get_diff_hist()); }
	if(type == "diff_slice_proj") { stat.set_distribution(data->get_diff_slice_proj()); }
	(*stats)[energy][div][cent]["mean"] = stat.get_mean();
	(*stats)[energy][div][cent]["standard_deviation"] = stat.get_standard_deviation();
	(*stats)[energy][div][cent]["skewness"] = stat.get_skewness();
	(*stats)[energy][div][cent]["kurtosis"] = stat.get_kurtosis();
	(*stats)[energy][div][cent]["non_excess_kurtosis"] = stat.get_non_excess_kurtosis();
	(*stats)[energy][div][cent]["kurtosis*variance"] = stat.get_kurt_var();
	Stats dist_mean_stat;
	dist_mean_stat.set_distribution(data->get_proton_dist());
	(*stats)[energy][div][cent]["particle_dist_mean"] = dist_mean_stat.get_mean();

	for(int order:orders) {
		(*stats)[energy][div][cent]["cumulant "+to_string(order)] = stat.get_cumulant(order);
		(*stats)[energy][div][cent]["raw moment "+to_string(order)] = stat.get_raw_moment(order);
		(*stats)[energy][div][cent]["central moment "+to_string(order)] = stat.get_central_moment(order);
	}
	if(isnan(stat.get_standard_deviation().get_val()) || isnan(stat.get_standard_deviation().get_err())) {
		cout << type << endl;
		this_thread::sleep_for(chrono::seconds(1));
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


// Get median Measure of vector by value. If even number of Measures takes lower of two median options.
Measure AzimuthBinAnalyzer::median(vector<Measure> data) {
	sort(data.begin(), data.end(), [] (Measure a, Measure b) { return(a.get_val() < b.get_val()); } );
	return(data[(int)(data.size()-1) / 2]);
}
