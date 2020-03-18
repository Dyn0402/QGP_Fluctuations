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

void AzimuthBinAnalyzer::set_in_path(string path) {
	in_path = path;
}

void AzimuthBinAnalyzer::set_in_mix_path(string path) {
	in_mix_path = path;
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

// Doers

void AzimuthBinAnalyzer::analyze() {
	out_root = new TFile((out_path+out_root_name).data(), "RECREATE");
	analyze_sets();
}


void AzimuthBinAnalyzer::analyze_sets() {
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

	map<string, map<string, map<string, map<int, map<int, map<int, map<string, Measure>>>>>>> set_pairs = {
//			{"Rand_Pile_Large", {
//					{"raw" , {{"Rand_Rotate", raw_stats_median["Rand_Rotate"]}, {"Pile_Up 0.1%", raw_stats_median["Pile_Up_001_"]}, {"Pile_Up 1%", raw_stats_median["Pile_Up_01_"]}, {"Pile_Up 0.2%", raw_stats_median["Pile_Up_002_"]}, {"Pile_Up 0.5%", raw_stats_median["Pile_Up_005_"]}, {"Pile_Up 0.8%", raw_stats_median["Pile_Up_008_"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_median["Rand_Rotate"]}, {"Pile_Up 0.1%", mix_stats_median["Pile_Up_001_"]}, {"Pile_Up 1%", mix_stats_median["Pile_Up_01_"]}, {"Pile_Up 0.2%", mix_stats_median["Pile_Up_002_"]}, {"Pile_Up 0.5%", mix_stats_median["Pile_Up_005_"]}, {"Pile_Up 0.8%", mix_stats_median["Pile_Up_008_"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_median["Rand_Rotate"]}, {"Pile_Up 0.1%", divide_stats_median["Pile_Up_001_"]}, {"Pile_Up 1%", divide_stats_median["Pile_Up_01_"]}, {"Pile_Up 0.2%", divide_stats_median["Pile_Up_002_"]}, {"Pile_Up 0.5%", divide_stats_median["Pile_Up_005_"]}, {"Pile_Up 0.8%", divide_stats_median["Pile_Up_008_"]}}}}
//			},
//			{"Rand_Pile_Small", {
//					{"raw" , {{"Rand_Rotate", raw_stats_median["Rand_Rotate"]}, {"Pile_Up 0.1%", raw_stats_median["Pile_Up_001_"]}, {"Pile_Up 0.08%", raw_stats_median["Pile_Up_0008_"]}, {"Pile_Up 0.05%", raw_stats_median["Pile_Up_0005_"]}, {"Pile_Up 0.02%", raw_stats_median["Pile_Up_0002_"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_median["Rand_Rotate"]}, {"Pile_Up 0.1%", mix_stats_median["Pile_Up_001_"]}, {"Pile_Up 0.08%", mix_stats_median["Pile_Up_0008_"]}, {"Pile_Up 0.05%", mix_stats_median["Pile_Up_0005_"]}, {"Pile_Up 0.02%", mix_stats_median["Pile_Up_0002_"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_median["Rand_Rotate"]}, {"Pile_Up 0.1%", divide_stats_median["Pile_Up_001_"]}, {"Pile_Up 0.08%", divide_stats_median["Pile_Up_0008_"]}, {"Pile_Up 0.05%", divide_stats_median["Pile_Up_0005_"]}, {"Pile_Up 0.02%", divide_stats_median["Pile_Up_0002_"]}}}}
//			},
//			{"Rand_Pile_RejCut", {
//					{"raw" , {{"Rand_Rotate", raw_stats_median["Rand_Rotate"]}, {"No BTof_Ref Cut", raw_stats_median["No_BTof_Rej"]}, {"Pile_Up 0.02%", raw_stats_median["Pile_Up_0002_"]}, {"Pile_Up 0.05%", raw_stats_median["Pile_Up_0005_"]}, {"Pile_Up 0.1%", raw_stats_median["Pile_Up_001_"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_median["Rand_Rotate"]}, {"No BTof_Ref Cut", mix_stats_median["No_BTof_Rej"]}, {"Pile_Up 0.02%", mix_stats_median["Pile_Up_0002_"]}, {"Pile_Up 0.05%", mix_stats_median["Pile_Up_0005_"]}, {"Pile_Up 0.1%", mix_stats_median["Pile_Up_001_"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_median["Rand_Rotate"]}, {"No BTof_Ref Cut", divide_stats_median["No_BTof_Rej"]}, {"Pile_Up 0.02%", divide_stats_median["Pile_Up_0002_"]}, {"Pile_Up 0.05%", divide_stats_median["Pile_Up_0005_"]}, {"Pile_Up 0.1%", divide_stats_median["Pile_Up_001_"]}}}}
//			},
//			{"Rand_Efficiency", {
//					{"raw", {{"Rand_Rotate", raw_stats_median["Rand_Rotate"]}, {"Efficiency 1%", raw_stats_median["Efficiency_01_"]}, {"Efficiency 8%", raw_stats_median["Efficiency_08_"]}, {"Efficiency 2.5%", raw_stats_median["Efficiency_025_"]}, {"Efficiency 5%", raw_stats_median["Efficiency_05_"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_median["Rand_Rotate"]}, {"Efficiency 1%", mix_stats_median["Efficiency_01_"]}, {"Efficiency 8%", mix_stats_median["Efficiency_08_"]}, {"Efficiency 2.5%", mix_stats_median["Efficiency_025_"]}, {"Efficiency 5%", mix_stats_median["Efficiency_05_"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_median["Rand_Rotate"]}, {"Efficiency 1%", divide_stats_median["Efficiency_01_"]}, {"Efficiency 8%", divide_stats_median["Efficiency_08_"]}, {"Efficiency 2.5%", divide_stats_median["Efficiency_025_"]}, {"Efficiency 5%", divide_stats_median["Efficiency_05_"]}}}}
//			},
//			{"Rotates", {
//					{"raw", {{"Rand_Rotate", raw_stats_median["Rand_Rotate"]}, {"No_Rotate", raw_stats_median["No_Rotate"]}, {"EP_Rotate", raw_stats_median["EP_Rotate"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_median["Rand_Rotate"]}, {"No_Rotate", mix_stats_median["No_Rotate"]}, {"EP_Rotate", mix_stats_median["EP_Rotate"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_median["Rand_Rotate"]}, {"No_Rotate", divide_stats_median["No_Rotate"]}, {"EP_Rotate", divide_stats_median["EP_Rotate"]}}}}
//			},
			{"All", {{"raw", raw_stats_median}, {"mix", mix_stats_median}, {"divide", divide_stats_median}}}
	};

	map<string, map<string, map<string, map<int, map<int, map<int, map<string, double>>>>>>> set_pairs_sd = {
//			{"Rand_Pile_Large", {
//					{"raw" , {{"Rand_Rotate", raw_stats_sd["Rand_Rotate"]}, {"Pile_Up 0.1%", raw_stats_sd["Pile_Up_001_"]}, {"Pile_Up 1%", raw_stats_sd["Pile_Up_01_"]}, {"Pile_Up 0.2%", raw_stats_sd["Pile_Up_002_"]}, {"Pile_Up 0.5%", raw_stats_sd["Pile_Up_005_"]}, {"Pile_Up 0.8%", raw_stats_sd["Pile_Up_008_"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_sd["Rand_Rotate"]}, {"Pile_Up 0.1%", mix_stats_sd["Pile_Up_001_"]}, {"Pile_Up 1%", mix_stats_sd["Pile_Up_01_"]}, {"Pile_Up 0.2%", mix_stats_sd["Pile_Up_002_"]}, {"Pile_Up 0.5%", mix_stats_sd["Pile_Up_005_"]}, {"Pile_Up 0.8%", mix_stats_sd["Pile_Up_008_"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_sd["Rand_Rotate"]}, {"Pile_Up 0.1%", divide_stats_sd["Pile_Up_001_"]}, {"Pile_Up 1%", divide_stats_sd["Pile_Up_01_"]}, {"Pile_Up 0.2%", divide_stats_sd["Pile_Up_002_"]}, {"Pile_Up 0.5%", divide_stats_sd["Pile_Up_005_"]}, {"Pile_Up 0.8%", divide_stats_sd["Pile_Up_008_"]}}}}
//			},
//			{"Rand_Pile_Small", {
//					{"raw" , {{"Rand_Rotate", raw_stats_sd["Rand_Rotate"]}, {"Pile_Up 0.1%", raw_stats_sd["Pile_Up_001_"]}, {"Pile_Up 0.08%", raw_stats_sd["Pile_Up_0008_"]}, {"Pile_Up 0.05%", raw_stats_sd["Pile_Up_0005_"]}, {"Pile_Up 0.02%", raw_stats_sd["Pile_Up_0002_"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_sd["Rand_Rotate"]}, {"Pile_Up 0.1%", mix_stats_sd["Pile_Up_001_"]}, {"Pile_Up 0.08%", mix_stats_sd["Pile_Up_0008_"]}, {"Pile_Up 0.05%", mix_stats_sd["Pile_Up_0005_"]}, {"Pile_Up 0.02%", mix_stats_sd["Pile_Up_0002_"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_sd["Rand_Rotate"]}, {"Pile_Up 0.1%", divide_stats_sd["Pile_Up_001_"]}, {"Pile_Up 0.08%", divide_stats_sd["Pile_Up_0008_"]}, {"Pile_Up 0.05%", divide_stats_sd["Pile_Up_0005_"]}, {"Pile_Up 0.02%", divide_stats_sd["Pile_Up_0002_"]}}}}
//			},
//			{"Rand_Pile_RejCut", {
//					{"raw" , {{"Rand_Rotate", raw_stats_sd["Rand_Rotate"]}, {"No BTof_Ref Cut", raw_stats_sd["No_BTof_Rej"]}, {"Pile_Up 0.02%", raw_stats_sd["Pile_Up_0002_"]}, {"Pile_Up 0.05%", raw_stats_sd["Pile_Up_0005_"]}, {"Pile_Up 0.1%", raw_stats_sd["Pile_Up_001_"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_sd["Rand_Rotate"]}, {"No BTof_Ref Cut", mix_stats_sd["No_BTof_Rej"]}, {"Pile_Up 0.02%", mix_stats_sd["Pile_Up_0002_"]}, {"Pile_Up 0.05%", mix_stats_sd["Pile_Up_0005_"]}, {"Pile_Up 0.1%", mix_stats_sd["Pile_Up_001_"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_sd["Rand_Rotate"]}, {"No BTof_Ref Cut", divide_stats_sd["No_BTof_Rej"]}, {"Pile_Up 0.02%", divide_stats_sd["Pile_Up_0002_"]}, {"Pile_Up 0.05%", divide_stats_sd["Pile_Up_0005_"]}, {"Pile_Up 0.1%", divide_stats_sd["Pile_Up_001_"]}}}}
//			},
//			{"Rand_Efficiency", {
//					{"raw", {{"Rand_Rotate", raw_stats_sd["Rand_Rotate"]}, {"Efficiency 1%", raw_stats_sd["Efficiency_01_"]}, {"Efficiency 8%", raw_stats_sd["Efficiency_08_"]}, {"Efficiency 2.5%", raw_stats_sd["Efficiency_025_"]}, {"Efficiency 5%", raw_stats_sd["Efficiency_05_"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_sd["Rand_Rotate"]}, {"Efficiency 1%", mix_stats_sd["Efficiency_01_"]}, {"Efficiency 8%", mix_stats_sd["Efficiency_08_"]}, {"Efficiency 2.5%", mix_stats_sd["Efficiency_025_"]}, {"Efficiency 5%", mix_stats_sd["Efficiency_05_"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_sd["Rand_Rotate"]}, {"Efficiency 1%", divide_stats_sd["Efficiency_01_"]}, {"Efficiency 8%", divide_stats_sd["Efficiency_08_"]}, {"Efficiency 2.5%", divide_stats_sd["Efficiency_025_"]}, {"Efficiency 5%", divide_stats_sd["Efficiency_05_"]}}}}
//			},
//			{"Rotates", {
//					{"raw", {{"Rand_Rotate", raw_stats_sd["Rand_Rotate"]}, {"No_Rotate", raw_stats_sd["No_Rotate"]}, {"EP_Rotate", raw_stats_sd["EP_Rotate"]}}},
//					{"mix", {{"Rand_Rotate", mix_stats_sd["Rand_Rotate"]}, {"No_Rotate", mix_stats_sd["No_Rotate"]}, {"EP_Rotate", mix_stats_sd["EP_Rotate"]}}},
//					{"divide", {{"Rand_Rotate", divide_stats_sd["Rand_Rotate"]}, {"No_Rotate", divide_stats_sd["No_Rotate"]}, {"EP_Rotate", divide_stats_sd["EP_Rotate"]}}}}
//			},
			{"All", {{"raw", raw_stats_sd}, {"mix", mix_stats_sd}, {"divide", divide_stats_sd}}}
	};

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

	// Calculate standard deviations for systematics
	for(pair<int, map<int, map<int, map<string, vector<Measure>>>>> energy:raw_stats_sets[set_name]) {
		for(pair<int, map<int, map<string, vector<Measure>>>> div:energy.second) {
			for(pair<int, map<string, vector<Measure>>> cent:div.second) {
				for(pair<string, vector<Measure>> stat:cent.second) {
					raw_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					mix_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					divide_stats_sd[set_name][energy.first][div.first][cent.first][stat.first] = sample_sd(divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);

					raw_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					mix_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);
					divide_stats_median[set_name][energy.first][div.first][cent.first][stat.first] = median(divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first]);

					for(int i=0; i<(int)raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].size(); i++) {
						raw_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
						mix_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
						divide_stats_all[set_name+to_string(i)][energy.first][div.first][cent.first][stat.first] = divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first][i];
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
				}
			}
			TDirectory *centralities_dir = median_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(raw_stats_median[set_name], raw_stats_sd[set_name], name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
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
					roli_thesis_stats(raw_stats_all, name.second, centralities, {div}, "roli_thesis_raw_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = all_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(raw_stats_all, name, all_centralities, {div}, "centralities_raw_"+name+to_string(div));
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
					roli_thesis_stats(mix_stats_median[set_name], mix_stats_sd[set_name], name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = median_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(mix_stats_median[set_name], mix_stats_sd[set_name], name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
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
					roli_thesis_stats(mix_stats_all, name.second, centralities, {div}, "roli_thesis_mix_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = all_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(mix_stats_all, name, all_centralities, {div}, "centralities_mix_"+name+to_string(div));
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
					roli_thesis_stats(divide_stats_median[set_name], divide_stats_sd[set_name], name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = median_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(divide_stats_median[set_name], divide_stats_sd[set_name], name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
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
					roli_thesis_stats(divide_stats_all, name.second, centralities, {div}, "roli_thesis_divide_"+name.first+to_string(div));
				}
			}
			TDirectory *centralities_dir = all_dir->mkdir("centralities");
			for(string name:stat_names) {
				TDirectory *name_dir = centralities_dir->mkdir(name.data());
				name_dir->cd();
				for(int div:divs) {
					centralities_stat(divide_stats_all, name, all_centralities, {div}, "centralities_divide_"+name+to_string(div));
				}
			}
		}
	}
}


void AzimuthBinAnalyzer::analyze_subset(string set_name, int set_num, TDirectory *set_dir) {

	cout << "Starting Set " + set_name + to_string(set_num) << endl << endl;

	string path = in_path + set_name + to_string(set_num) + "/";
	string path_mix = in_mix_path + set_name + to_string(set_num) + "/";

	map<int, map<int, map<int, AzimuthBinData>>> data = get_data(path);
	map<int, map<int, map<int, AzimuthBinData>>> data_mix = get_data(path_mix);

	auto ratio_stats = calculate_stats(data, "ratio", orders);
	auto ratio_stats_mix = calculate_stats(data_mix, "ratio", orders);

	auto diff_stats = calculate_stats(data, "diff", orders);
	auto diff_stats_mix = calculate_stats(data_mix, "diff", orders);

	map<int, map<int, map<int, map<string, Measure>>>> stats_mix_divide;

	for(pair<int, map<int, map<int, map<string, Measure>>>> energy:ratio_stats) {
		for(pair<int, map<int, map<string, Measure>>> div:energy.second) {
			for(pair<int, map<string, Measure>> cent:div.second) {
				cout << "Energy: " + to_string(energy.first) + "GeV " + "Cent: " + to_string(cent.first) + " Div: " + to_string(div.first) + " Mix_Diff_Sd: " + to_string(diff_stats_mix[energy.first][div.first][cent.first]["standard_deviation"].get_val()) << endl;
				data[energy.first][div.first][cent.first].set_diff_divisor(diff_stats_mix[energy.first][div.first][cent.first]["standard_deviation"].get_val());
				for(pair<string, Measure> stat:cent.second) {
					stats_mix_divide[energy.first][div.first][cent.first][stat.first] = stat.second / ratio_stats_mix[energy.first][div.first][cent.first][stat.first];

					raw_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stat.second);
					mix_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(ratio_stats_mix[energy.first][div.first][cent.first][stat.first]);
					divide_stats_sets[set_name][energy.first][div.first][cent.first][stat.first].push_back(stats_mix_divide[energy.first][div.first][cent.first][stat.first]);
				}
			}
		}
	}

	auto pull_stats = calculate_stats(data, "diff", orders);

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
		make_canvas_plots(data_dir, data);
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
		make_canvas_plots(mix_dir, data_mix);
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
			}
		}
		TDirectory *centralities_dir = mix_dir->mkdir("centralities");
		for(string name:stat_names) {
			TDirectory *name_dir = centralities_dir->mkdir(name.data());
			name_dir->cd();
			for(int div:divs) {
				centralities_stat(ratio_stats_mix, name, all_centralities, {div}, "centralities_mix_ratio_"+name+to_string(div));
				centralities_stat(diff_stats_mix, name, all_centralities, {div}, "centralities_mix_diff_"+name+to_string(div));
			}
		}
	}

	map<string, map<int, map<int, map<int, map<string, Measure>>>>> raw_mix_comp {{"raw", ratio_stats}, {"mixed", ratio_stats_mix}};

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
				centralities_stat(raw_mix_comp, name, all_centralities, {div}, "centralities_comp_"+name+to_string(div));
			}
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
				AzimuthBinData az_data(div);
				az_data.read_data_from_dir(path, div, cent);  // Read azimuthal bin data from file path

				if(az_data.get_num_bins() / div <= min_num_events) {
					if(div == divs[0]) {
						cout << "Centrality " << cent << " with only " << az_data.get_num_bins() / div << " events. Skipping." << endl;
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
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(1);//thread::hardware_concurrency());
		for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
			for(pair<int, map<int, AzimuthBinData>> div:energy.second) {
				for(pair<int, AzimuthBinData> cent:div.second) {
					pool.enqueue(&AzimuthBinAnalyzer::calc_stat, this, &(data[energy.first][div.first][cent.first]), type, energy.first, div.first, cent.first, orders, &stats);
				}
			}
		}
	}

	return(stats);
}


void AzimuthBinAnalyzer::calc_stat(AzimuthBinData *data, string type, int energy, int div, int cent, vector<int> orders, map<int, map<int, map<int, map<string, Measure>>>> *stats) {
	Stats stat;
	if(type == "ratio") { stat.set_distribution(data->get_ratio_hist()); }
	if(type == "diff") { stat.set_distribution(data->get_diff_hist()); }
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
