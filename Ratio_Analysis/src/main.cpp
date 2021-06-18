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

#ifdef _WIN32
#include "dirent_win.h"
#include "Windows.h"
static const std::string platform = "win";
#else
#include <dirent.h>
static const std::string platform = "lin";
#endif

#include <cstdlib>
#include <ctime>
#include <chrono>

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
#include "AzimuthBinData.h"
#include "AzimuthBinAnalyzer.h"
#include "AzFigurePlotter.h"
#include "BinomialAnalyzer.h"

using namespace std;

void azimuth_bin_analyze();
void azimuth_bin_analyze_fig();
void binomial_analyze();
void mem_test();
void stat_sd_nan_test();

map<string, vector<int>> get_sets(string dir_path, string val_flag, pair<float, float> val_range, int dec_pos, vector<string> other_flags={});
map<string, vector<int>> get_sets(string dir_path);
vector<string> get_set_names(map<string, vector<int>> sets);
map<string, vector<int>> combine_sets(vector<map<string, vector<int>>> sets_set);


int main() {
//	for(pair<string, vector<int>> set:get_sets("/home/dylan/Research/Data/", "dca", make_pair(0.6, 1.4), 1, {"rapid05_"})) {
//		cout << endl << set.first << endl;
//		for(int set_num:set.second) {
//			cout << set_num << " " << flush;
//		}
//	}
	cout << "Running AzimuthBinAnalyzer" << endl << endl;
	//azimuth_bin_analyze();
	azimuth_bin_analyze_fig();
	//cout << endl << endl << "Running BinomialAnalyzer" << endl << endl;
	//binomial_analyze();

	//mem_test();
	//stat_sd_nan_test();

	cout << "donzo" << endl;
	return(0);
}


void azimuth_bin_analyze_fig() {
	AzFigurePlotter analyzer;
	if (platform == "lin") {
		analyzer.set_bes_in_path("/home/dylan/Research/Data/");
		analyzer.set_bes_in_mix_path("/home/dylan/Research/Data_Mix/");
		analyzer.set_ampt_in_path("/home/dylan/Research/Data_Ampt/");
		analyzer.set_ampt_in_mix_path("/home/dylan/Research/Data_Ampt_Mix/");
		analyzer.set_sim_in_path("/home/dylan/Research/Data_Sim/");
		analyzer.set_sim_in_mix_path("/home/dylan/Research/Data_Sim_Mix/");
		analyzer.set_out_path("/home/dylan/Research/Results/Azimuth_Analysis/");
	}
	else if (platform == "win") {
		analyzer.set_bes_in_path("C:/Users/Dylan/Research/Data/");
		analyzer.set_bes_in_mix_path("C:/Users/Dylan/Research/Data_Mix/");
		analyzer.set_ampt_in_path("C:/Users/Dylan/Research/Data_Ampt/");
		analyzer.set_ampt_in_mix_path("C:/Users/Dylan/Research/Data_Ampt_Mix/");
		analyzer.set_sim_in_path("C:/Users/Dylan/Research/Data_Sim/");
		analyzer.set_sim_in_mix_path("C:/Users/Dylan/Research/Data_Sim_Mix/");
		analyzer.set_out_path("C:/Users/Dylan/Research/Results/Azimuth_Analysis/");
	}
	else {
		cout << "What platform? " << platform << endl;
	}
	//	analyzer.set_out_root_name("10-2-20_BES1_eta_05_1_dca_1_3.root");
	//	analyzer.set_def_set("rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_");
	analyzer.set_out_root_name("6-18-21_paper_figs.root");
	analyzer.set_energies({ 7, 11, 19, 27, 39, 62 });
	//	analyzer.set_energies({7});
	analyzer.set_all_centralities({ 8, 7, 6, 5, 4, 3, 2, 1 });
	analyzer.set_centralities({ 8, 7, 6, 5, 4, 3, 2, 1 });
	analyzer.set_divs({ 300, 270, 240, 180, 120, 90, 72, 60 });
	analyzer.set_stat_names({"mean", "standard_deviation", "skewness", "kurtosis", "non_excess_kurtosis", "kurtosis*variance"});
	//	analyzer.set_divs({60});
	analyzer.set_plot_cents({ 8 });
	analyzer.set_plot_dists(true);
	analyzer.set_plot_dist_canvases(true);
	analyzer.set_plot_sys(true);
	//	analyzer.set_can_wh(625, 550);
	//	analyzer.set_can_wh(955, 900);
	analyzer.set_can_wh(955, 805);
	//	analyzer.set_sets({{"rapid05_n1ratios_dca1", {0, 4}}, {"Ampt_rapid05_n1ratios", {0, 4}},
	//		{"rapid05_n1ratios_dca09", {0, 4}}, {"rapid05_n1ratios_dca095", {0, 4}}, {"rapid05_n1ratios_dca105", {0, 4}}, {"rapid05_n1ratios_dca11", {0, 4}}});
	//	analyzer.set_set_combos({{"rapid05_dca1_Ampt", {"rapid05_n1ratios_dca1", "Ampt_rapid05_n1ratios"}},
	//		{"rapid05_dca_var", {"rapid05_n1ratios_dca09", "rapid05_n1ratios_dca095", "rapid05_n1ratios_dca1", "rapid05_n1ratios_dca105", "rapid05_n1ratios_dca11"}}});
	//	analyzer.set_sets({{"rapid05_n1ratios_dca1", {0, 4}}, {"Ampt_rapid05_n1ratios", {0, 4}},
	//		{"rapid05_n1ratios_dca1_Efficiency1", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency2", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency3", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency4", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency5", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency6", {0, 4}}});
	//	analyzer.set_set_combos({{"rapid05_dca1_Ampt", {"rapid05_n1ratios_dca1", "Ampt_rapid05_n1ratios"}},
	//		{"rapid05_dca1_Eff", {"rapid05_n1ratios_dca1", "rapid05_n1ratios_dca1_Efficiency1", "rapid05_n1ratios_dca1_Efficiency2", "rapid05_n1ratios_dca1_Efficiency3", "rapid05_n1ratios_dca1_Efficiency4", "rapid05_n1ratios_dca1_Efficiency5", "rapid05_n1ratios_dca1_Efficiency6"}}});
	//	analyzer.set_sets({{"rapid05_n1ratios_dca1", {0, 4}}, {"rapid05_n1ratios_dca1_seed_test", {0, 4}}, {"rapid05_n1ratios_dca1_stref_seed_test", {0, 4}}});

	//	map<string, vector<int>> bes1_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
	//	map<string, vector<int>> bes1_sys = get_sets("/home/dylan/Research/Data/", "sys", make_pair(2, 2), 1);
	//	map<string, vector<int>> ampt_def {{"Ampt_rapid05_n1ratios", {0, 54}}};
	//	map<string, vector<int>> ampt_sys = ampt_def;
	//	analyzer.set_sets(combine_sets({bes1_sys, bes1_def, ampt_def}));
	//	analyzer.set_sys_combos({{"BES1", {get_set_names(bes1_def)[0], get_set_names(bes1_sys)}}, {"AMPT", {get_set_names(ampt_def)[0], get_set_names(ampt_sys)}}});

		//map<string, vector<int>> bes1_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
		//map<string, vector<int>> bes1_sys = get_sets("/home/dylan/Research/Data/", "sys", make_pair(2, 2), 1);
		//map<string, vector<int>> ampt_def {{"Ampt_rapid05_n1ratios", {0, 54}}};
		//map<string, vector<int>> ampt_sys = ampt_def;
		//map<string, vector<int>> ampt_rp_def {{"Ampt_rapid05_n1ratios_dca1_ReactionPlane_", {0, 16}}};
		//map<string, vector<int>> ampt_rp_sys = ampt_def;
		//analyzer.set_sets(combine_sets({bes1_def, bes1_sys, ampt_rp_def, ampt_def}));
		//analyzer.set_sys_combos({{"BES1", {get_set_names(bes1_def)[0], get_set_names(bes1_sys)}}, {"AMPT_RP", {get_set_names(ampt_rp_def)[0], get_set_names(ampt_rp_sys)}}, {"AMPT", {get_set_names(ampt_def)[0], get_set_names(ampt_sys)}}});


	map<string, vector<int>> bes1_def = get_sets(analyzer.get_bes_in_path() + "default/");
	map<string, vector<int>> bes1_sys = get_sets(analyzer.get_bes_in_path() + "default_sys/");
	map<string, vector<int>> bes1_def1, bes1_sys1;
	for (pair<string, vector<int>> def : bes1_def) { bes1_def1["default/" + def.first] = def.second; }
	for (pair<string, vector<int>> def : bes1_sys) { bes1_sys1["default_sys/" + def.first] = def.second; }
	//	cout << analyzer.get_ampt_in_path() << endl;
	//	int x;
	//	cin >> x;
	map<string, vector<int>> ampt_def = get_sets(analyzer.get_ampt_in_path() + "default/");
	map<string, vector<int>> ampt_sys = ampt_def;
	map<string, vector<int>> ampt_def1, ampt_sys1;
	for (pair<string, vector<int>> def : ampt_def) { ampt_def1["default/" + def.first] = def.second; }
	for (pair<string, vector<int>> def : ampt_sys) { ampt_sys1["default/" + def.first] = def.second; }

	analyzer.set_sets(combine_sets({ bes1_def1, bes1_sys1, ampt_def1 }));
	analyzer.set_sys_combos({ {"BES1", {get_set_names(bes1_def1)[0], get_set_names(bes1_sys1)}},
		{"AMPT", {get_set_names(ampt_def1)[0], get_set_names(ampt_sys1)}} });

	//map<string, vector<int>> bes1_sys = get_sets(analyzer.get_bes_in_path() + "default_sys_test/");
	//map<string, vector<int>> bes1_sys1;
	//for (pair<string, vector<int>> def : bes1_sys) { bes1_sys1["default_sys_test/" + def.first] = def.second; }
	//analyzer.set_sets(bes1_sys1);


	//	map<string, vector<int>> full_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
	//	map<string, vector<int>> full_sys = get_sets("/home/dylan/Research/Data/", "sys", make_pair(2, 2), 1);
	//	map<string, vector<int>> rand_def = full_def;
	//	map<string, vector<int>> rand_sys = full_def;
	//	map<string, vector<int>> mix_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_mixnoseed_", {0, 20}}};
	//	map<string, vector<int>> mix_sys = mix_def;
	//	analyzer.set_sets(combine_sets({full_def, full_sys, mix_def}));
	//	analyzer.set_sys_combos({{"Full_Sys", {get_set_names(full_def)[0], get_set_names(full_sys)}},
	//		{"Rand_Sys", {get_set_names(rand_def)[0], get_set_names(rand_sys)}},
	//		{"ZMix_Sys", {get_set_names(mix_def)[0], get_set_names(mix_sys)}}});

	//	map<string, vector<int>> def_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
	//	map<string, vector<int>> def_sys = def_def;
	//	map<string, vector<int>> mixseed_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_mixseed_", {0, 20}}};
	//	map<string, vector<int>> mixseed_sys = mixseed_def;
	//	map<string, vector<int>> mixnoseed_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_mixnoseed_", {0, 20}}};
	//	map<string, vector<int>> mixnoseed_sys = mixnoseed_def;
	//	analyzer.set_sets(combine_sets({def_def, def_sys, mixseed_sys, mixseed_def, mixnoseed_sys, mixnoseed_def}));
	//	analyzer.set_sys_combos({{"def", {get_set_names(def_def)[0], get_set_names(def_sys)}},
	//		{"mixseed", {get_set_names(mixseed_def)[0], get_set_names(mixseed_sys)}},
	//		{"mixnoseed", {get_set_names(mixnoseed_def)[0], get_set_names(mixnoseed_sys)}}});

	//	map<string, vector<int>> def_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
	//	map<string, vector<int>> def_sys = def_def;
	//	map<string, vector<int>> ef1_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency1_", {0, 16}}};
	//	map<string, vector<int>> ef1_sys = ef1_def;
	//	map<string, vector<int>> ef2_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency2_", {0, 16}}};
	//	map<string, vector<int>> ef2_sys = ef2_def;
	//	map<string, vector<int>> ef3_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency3_", {0, 16}}};
	//	map<string, vector<int>> ef3_sys = ef3_def;
	////	map<string, vector<int>> ef4_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency4_", {0, 16}}};
	////	map<string, vector<int>> ef4_sys = ef4_def;
	//	analyzer.set_sets(combine_sets({def_def, ef1_def, ef2_def, ef3_def}));
	//	analyzer.set_sys_combos({{"Efficiency 0", {get_set_names(def_def)[0], get_set_names(def_sys)}},
	//		{"Efficiency 1", {get_set_names(ef1_def)[0], get_set_names(ef1_sys)}},
	//		{"Efficiency 2", {get_set_names(ef2_def)[0], get_set_names(ef2_sys)}},
	//		{"Efficiency 3", {get_set_names(ef3_def)[0], get_set_names(ef3_sys)}}});
	////		{"Efficiency 4", {get_set_names(ef4_def)[0], get_set_names(ef4_sys)}}});

	//	analyzer.set_set_combos({{"rapid05_dcavar_seed", get_set_names(get_sets("/home/dylan/Research/Data/", "dca", make_pair(0.85, 1.2), 1, {"rapid05_"}))},
	//		{"rapidvar_dca1_seed", get_set_names(get_sets("/home/dylan/Research/Data/", "rapid", make_pair(0.35, 0.7), 1, {"_dca1_"}))}});
	//analyzer.analyze();
	analyzer.analyze_lite();
	analyzer.plot_paper_figs();
}

void azimuth_bin_analyze() {
	AzimuthBinAnalyzer analyzer;
	if (platform == "lin") {
		analyzer.set_bes_in_path("/home/dylan/Research/Data/");
		analyzer.set_bes_in_mix_path("/home/dylan/Research/Data_Mix/");
		analyzer.set_ampt_in_path("/home/dylan/Research/Data_Ampt/");
		analyzer.set_ampt_in_mix_path("/home/dylan/Research/Data_Ampt_Mix/");
		analyzer.set_sim_in_path("/home/dylan/Research/Data_Sim/");
		analyzer.set_sim_in_mix_path("/home/dylan/Research/Data_Sim_Mix/");
		analyzer.set_out_path("/home/dylan/Research/Results/Azimuth_Analysis/");
	}
	else if (platform == "win") {
		analyzer.set_bes_in_path("C:/Users/Dylan/Research/Data/");
		analyzer.set_bes_in_mix_path("C:/Users/Dylan/Research/Data_Mix/");
		analyzer.set_ampt_in_path("C:/Users/Dylan/Research/Data_Ampt/");
		analyzer.set_ampt_in_mix_path("C:/Users/Dylan/Research/Data_Ampt_Mix/");
		analyzer.set_sim_in_path("C:/Users/Dylan/Research/Data_Sim/");
		analyzer.set_sim_in_mix_path("C:/Users/Dylan/Research/Data_Sim_Mix/");
		analyzer.set_out_path("C:/Users/Dylan/Research/Results/Azimuth_Analysis/");
	}
	else {
		cout << "What platform? " << platform << endl;
	}
//	analyzer.set_out_root_name("10-2-20_BES1_eta_05_1_dca_1_3.root");
//	analyzer.set_def_set("rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_");
	analyzer.set_out_root_name("5-7-21_bes_ampt_new_structure.root");
	analyzer.set_energies({7, 11, 19, 27, 39, 62});
//	analyzer.set_energies({7});
	analyzer.set_all_centralities({8}); // ,7,6,5,4,3,2,1
	analyzer.set_centralities({8});
	analyzer.set_divs({300, 270, 240, 180, 120, 90, 72, 60});
//	analyzer.set_divs({60});
	analyzer.set_plot_cents({8});
	analyzer.set_plot_dists(true);
	analyzer.set_plot_dist_canvases(true);
	analyzer.set_plot_sys(true);
//	analyzer.set_can_wh(625, 550);
//	analyzer.set_can_wh(955, 900);
	analyzer.set_can_wh(955, 805);
//	analyzer.set_sets({{"rapid05_n1ratios_dca1", {0, 4}}, {"Ampt_rapid05_n1ratios", {0, 4}},
//		{"rapid05_n1ratios_dca09", {0, 4}}, {"rapid05_n1ratios_dca095", {0, 4}}, {"rapid05_n1ratios_dca105", {0, 4}}, {"rapid05_n1ratios_dca11", {0, 4}}});
//	analyzer.set_set_combos({{"rapid05_dca1_Ampt", {"rapid05_n1ratios_dca1", "Ampt_rapid05_n1ratios"}},
//		{"rapid05_dca_var", {"rapid05_n1ratios_dca09", "rapid05_n1ratios_dca095", "rapid05_n1ratios_dca1", "rapid05_n1ratios_dca105", "rapid05_n1ratios_dca11"}}});
//	analyzer.set_sets({{"rapid05_n1ratios_dca1", {0, 4}}, {"Ampt_rapid05_n1ratios", {0, 4}},
//		{"rapid05_n1ratios_dca1_Efficiency1", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency2", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency3", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency4", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency5", {0, 4}}, {"rapid05_n1ratios_dca1_Efficiency6", {0, 4}}});
//	analyzer.set_set_combos({{"rapid05_dca1_Ampt", {"rapid05_n1ratios_dca1", "Ampt_rapid05_n1ratios"}},
//		{"rapid05_dca1_Eff", {"rapid05_n1ratios_dca1", "rapid05_n1ratios_dca1_Efficiency1", "rapid05_n1ratios_dca1_Efficiency2", "rapid05_n1ratios_dca1_Efficiency3", "rapid05_n1ratios_dca1_Efficiency4", "rapid05_n1ratios_dca1_Efficiency5", "rapid05_n1ratios_dca1_Efficiency6"}}});
//	analyzer.set_sets({{"rapid05_n1ratios_dca1", {0, 4}}, {"rapid05_n1ratios_dca1_seed_test", {0, 4}}, {"rapid05_n1ratios_dca1_stref_seed_test", {0, 4}}});

//	map<string, vector<int>> bes1_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
//	map<string, vector<int>> bes1_sys = get_sets("/home/dylan/Research/Data/", "sys", make_pair(2, 2), 1);
//	map<string, vector<int>> ampt_def {{"Ampt_rapid05_n1ratios", {0, 54}}};
//	map<string, vector<int>> ampt_sys = ampt_def;
//	analyzer.set_sets(combine_sets({bes1_sys, bes1_def, ampt_def}));
//	analyzer.set_sys_combos({{"BES1", {get_set_names(bes1_def)[0], get_set_names(bes1_sys)}}, {"AMPT", {get_set_names(ampt_def)[0], get_set_names(ampt_sys)}}});

	//map<string, vector<int>> bes1_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
	//map<string, vector<int>> bes1_sys = get_sets("/home/dylan/Research/Data/", "sys", make_pair(2, 2), 1);
	//map<string, vector<int>> ampt_def {{"Ampt_rapid05_n1ratios", {0, 54}}};
	//map<string, vector<int>> ampt_sys = ampt_def;
	//map<string, vector<int>> ampt_rp_def {{"Ampt_rapid05_n1ratios_dca1_ReactionPlane_", {0, 16}}};
	//map<string, vector<int>> ampt_rp_sys = ampt_def;
	//analyzer.set_sets(combine_sets({bes1_def, bes1_sys, ampt_rp_def, ampt_def}));
	//analyzer.set_sys_combos({{"BES1", {get_set_names(bes1_def)[0], get_set_names(bes1_sys)}}, {"AMPT_RP", {get_set_names(ampt_rp_def)[0], get_set_names(ampt_rp_sys)}}, {"AMPT", {get_set_names(ampt_def)[0], get_set_names(ampt_sys)}}});


	map<string, vector<int>> bes1_def = get_sets(analyzer.get_bes_in_path() + "default/");
	map<string, vector<int>> bes1_sys = get_sets(analyzer.get_bes_in_path() + "default_sys/");
	map<string, vector<int>> bes1_def1, bes1_sys1;
	for (pair<string, vector<int>> def : bes1_def) { bes1_def1["default/" + def.first] = def.second;  }
	for (pair<string, vector<int>> def : bes1_sys) { bes1_sys1["default_sys/" + def.first] = def.second; }
//	cout << analyzer.get_ampt_in_path() << endl;
//	int x;
//	cin >> x;
	map<string, vector<int>> ampt_def = get_sets(analyzer.get_ampt_in_path() + "default/");
	map<string, vector<int>> ampt_sys = ampt_def;
	map<string, vector<int>> ampt_def1, ampt_sys1;
	for (pair<string, vector<int>> def : ampt_def) { ampt_def1["default/" + def.first] = def.second; }
	for (pair<string, vector<int>> def : ampt_sys) { ampt_sys1["default_sys/" + def.first] = def.second; }

	analyzer.set_sets(combine_sets({ bes1_def1, bes1_sys1, ampt_def1}));
	analyzer.set_sys_combos({ {"BES1", {get_set_names(bes1_def1)[0], get_set_names(bes1_sys1)}},
		{"AMPT", {get_set_names(ampt_def1)[0], get_set_names(ampt_sys1)}}});


//	map<string, vector<int>> full_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
//	map<string, vector<int>> full_sys = get_sets("/home/dylan/Research/Data/", "sys", make_pair(2, 2), 1);
//	map<string, vector<int>> rand_def = full_def;
//	map<string, vector<int>> rand_sys = full_def;
//	map<string, vector<int>> mix_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_mixnoseed_", {0, 20}}};
//	map<string, vector<int>> mix_sys = mix_def;
//	analyzer.set_sets(combine_sets({full_def, full_sys, mix_def}));
//	analyzer.set_sys_combos({{"Full_Sys", {get_set_names(full_def)[0], get_set_names(full_sys)}},
//		{"Rand_Sys", {get_set_names(rand_def)[0], get_set_names(rand_sys)}},
//		{"ZMix_Sys", {get_set_names(mix_def)[0], get_set_names(mix_sys)}}});

//	map<string, vector<int>> def_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
//	map<string, vector<int>> def_sys = def_def;
//	map<string, vector<int>> mixseed_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_mixseed_", {0, 20}}};
//	map<string, vector<int>> mixseed_sys = mixseed_def;
//	map<string, vector<int>> mixnoseed_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_mixnoseed_", {0, 20}}};
//	map<string, vector<int>> mixnoseed_sys = mixnoseed_def;
//	analyzer.set_sets(combine_sets({def_def, def_sys, mixseed_sys, mixseed_def, mixnoseed_sys, mixnoseed_def}));
//	analyzer.set_sys_combos({{"def", {get_set_names(def_def)[0], get_set_names(def_sys)}},
//		{"mixseed", {get_set_names(mixseed_def)[0], get_set_names(mixseed_sys)}},
//		{"mixnoseed", {get_set_names(mixnoseed_def)[0], get_set_names(mixnoseed_sys)}}});

//	map<string, vector<int>> def_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}};
//	map<string, vector<int>> def_sys = def_def;
//	map<string, vector<int>> ef1_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency1_", {0, 16}}};
//	map<string, vector<int>> ef1_sys = ef1_def;
//	map<string, vector<int>> ef2_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency2_", {0, 16}}};
//	map<string, vector<int>> ef2_sys = ef2_def;
//	map<string, vector<int>> ef3_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency3_", {0, 16}}};
//	map<string, vector<int>> ef3_sys = ef3_def;
////	map<string, vector<int>> ef4_def {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency4_", {0, 16}}};
////	map<string, vector<int>> ef4_sys = ef4_def;
//	analyzer.set_sets(combine_sets({def_def, ef1_def, ef2_def, ef3_def}));
//	analyzer.set_sys_combos({{"Efficiency 0", {get_set_names(def_def)[0], get_set_names(def_sys)}},
//		{"Efficiency 1", {get_set_names(ef1_def)[0], get_set_names(ef1_sys)}},
//		{"Efficiency 2", {get_set_names(ef2_def)[0], get_set_names(ef2_sys)}},
//		{"Efficiency 3", {get_set_names(ef3_def)[0], get_set_names(ef3_sys)}}});
////		{"Efficiency 4", {get_set_names(ef4_def)[0], get_set_names(ef4_sys)}}});

//	analyzer.set_set_combos({{"rapid05_dcavar_seed", get_set_names(get_sets("/home/dylan/Research/Data/", "dca", make_pair(0.85, 1.2), 1, {"rapid05_"}))},
//		{"rapidvar_dca1_seed", get_set_names(get_sets("/home/dylan/Research/Data/", "rapid", make_pair(0.35, 0.7), 1, {"_dca1_"}))}});
	analyzer.analyze_lite();
}


void binomial_analyze() {
	BinomialAnalyzer analyzer;
	analyzer.set_bes_in_path("/home/dylan/Research/Data/");
	analyzer.set_bes_in_mix_path("/home/dylan/Research/Data_Mix/");
	analyzer.set_ampt_in_path("/home/dylan/Research/Data_Ampt/");
	analyzer.set_ampt_in_mix_path("/home/dylan/Research/Data_Ampt_Mix/");
	analyzer.set_out_path("/home/dylan/Research/Results/Azimuth_Analysis/");
	analyzer.set_sim_in_path("/home/dylan/Research/Data_Sim/");
	analyzer.set_sim_in_mix_path("/home/dylan/Research/Data_Sim_Mix/");
//	analyzer.set_out_root_name("10-2-20_binom_BES1_eta_05_1_dca_1_3.root");
	analyzer.set_out_root_name("4-23-21_binom_bes1_ampt.root");
	analyzer.set_energies({7, 11, 19, 27, 39, 62});
	analyzer.set_divs({300, 240, 180, 120, 90, 72, 60});
	analyzer.set_centralities({8});
	analyzer.set_can_wh(1920, 900);
//	analyzer.set_can_wh(1700, 800);
//	analyzer.set_sets({{"eta05_n1ratios_dca1", {0, 2}}, {"eta05_n1ratios_dca3", {0, 2}}});
//	analyzer.set_set_combos({{"Eta05_dca1_3", {"eta05_n1ratios_dca1", "eta05_n1ratios_dca3"}}});
//	analyzer.set_sets({{"eta05_n1ratios_dca1", {0, 2}}, {"eta05_n1ratios_dca3", {0, 2}}, {"eta1_n1ratios_dca1", {0, 2}}, {"eta1_n1ratios_dca3", {0, 2}}});
//	analyzer.set_set_combos({{"Eta1_05_dca1", {"eta05_n1ratios_dca1", "eta1_n1ratios_dca1"}}, {"Eta1_dca1", {"eta1_n1ratios_dca1", "eta1_n1ratios_dca1"}},
//		{"Eta1_05_dca3", {"eta05_n1ratios_dca3", "eta1_n1ratios_dca3"}}, {"Eta1_dca3", {"eta1_n1ratios_dca3", "eta1_n1ratios_dca3"}}});
//	analyzer.set_sets({{"eta05_n1ratios", {0, 4}}, {"Ampt_p+_n1ratios", {0, 4}}});
//	analyzer.set_set_combos({{"Bes1_Ampt", {"eta05_n1ratios", "Ampt_p+_n1ratios"}}});
//	analyzer.set_sets({{"pion-_n1ratios", {0, 2}}, {"pion+_n1ratios", {0, 2}}, {"piontotal_n1ratios", {0, 2}}, {"Ampt_pion-_n1ratios", {0, 2}}, {"Ampt_pion+_n1ratios", {0, 2}}, {"Ampt_piontotal_n1ratios", {0, 2}}});
//	analyzer.set_set_combos({{"Bes1_Ampt_Pions", {"pion-_n1ratios", "pion+_n1ratios", "piontotal_n1ratios", "Ampt_pion-_n1ratios", "Ampt_pion+_n1ratios", "Ampt_piontotal_n1ratios"}}, {"Bes1_Ampt_Pion-", {"pion-_n1ratios", "Ampt_pion-_n1ratios"}}, {"Bes1_Ampt_Pion+", {"pion+_n1ratios", "Ampt_pion+_n1ratios"}},{"Bes1_Ampt_PionTotal", {"piontotal_n1ratios", "Ampt_piontotal_n1ratios"}}, {"Bes1_Pions", {"pion-_n1ratios", "pion+_n1ratios", "piontotal_n1ratios"}}, {"Ampt_Pions", {"Ampt_pion-_n1ratios", "Ampt_pion+_n1ratios", "Ampt_piontotal_n1ratios"}}});
//	analyzer.set_sets({{"eta05_n1ratios_Efficiency8", {0, 2}}, {"eta05_n1ratios_Efficiency5", {0, 2}}, {"eta05_n1ratios_Efficiency3", {0, 2}}, {"eta05_n1ratios_Efficiency1", {0, 2}}, {"eta05_n1ratios", {0, 4}}, {"Ampt_p+_n1ratios_Efficiency8", {0, 2}}, {"Ampt_p+_n1ratios_Efficiency5", {0, 2}}, {"Ampt_p+_n1ratios_Efficiency3", {0, 2}}, {"Ampt_p+_n1ratios_Efficiency1", {0, 2}}, {"Ampt_p+_n1ratios", {0, 4}}, {"Ampt_p+_eta1_n1ratios", {0, 4}}});
//	analyzer.set_set_combos({{"Bes1_Ampt_eta05", {"eta05_n1ratios", "Ampt_p+_n1ratios"}}, {"Bes1_Ampt_eta_both", {"eta05_n1ratios", "Ampt_p+_n1ratios", "Ampt_p+_eta1_n1ratios"}}, {"BES1_p+_eff", {"eta05_n1ratios", "eta05_n1ratios_Efficiency1", "eta05_n1ratios_Efficiency3", "eta05_n1ratios_Efficiency5", "eta05_n1ratios_Efficiency8"}}, {"AMPT_p+_eff", {"Ampt_p+_n1ratios", "Ampt_p+_n1ratios_Efficiency1", "Ampt_p+_n1ratios_Efficiency3", "Ampt_p+_n1ratios_Efficiency5", "Ampt_p+_n1ratios_Efficiency8"}}});
//	analyzer.set_sets({{"eta05", {0, 4}}});
//	analyzer.set_sets({{"Ampt_mesontotal", {0, 0}}, {"Ampt_ptotal_Efficiency0", {0,0}}});
//	analyzer.set_sets({{"Ampt_mesontotal_Efficiency8", {0, 0}}, {"Ampt_mesontotal_Efficiency5", {0, 0}}, {"Ampt_mesontotal_Efficiency3", {0, 0}}, {"Ampt_mesontotal_Efficiency1", {0, 0}}, {"Ampt_mesontotal", {0, 0}}, {"Ampt_ptotal_Efficiency8", {0,0}}, {"Ampt_ptotal_Efficiency5", {0,0}}, {"Ampt_ptotal_Efficiency3", {0,0}}, {"Ampt_ptotal_Efficiency1", {0,0}}, {"Ampt_ptotal_Efficiency0", {0,0}}});
//	analyzer.set_sets({{"Ampt_baryontotal", {0, 0}}, {"Ampt_mesontotal", {0, 0}}, {"Ampt_hadrontotal", {0, 0}}, {"Ampt_hadronobs", {0, 0}}});
//	analyzer.set_sets({{"Ampt_ptotal_Efficiency8", {0,0}}, {"Ampt_ptotal_Efficiency5", {0,0}}, {"Ampt_ptotal_Efficiency3", {0,0}}, {"Ampt_ptotal_Efficiency1", {0,0}}, {"Ampt_ptotal_Efficiency0", {0,0}}});
//	analyzer.set_sets({{"p+", {0,4}}, {"p-", {0,4}}, {"ptotal", {0,4}}});
//	analyzer.set_sets({{"Ampt_p-", {0,0}}, {"Ampt_p+", {0,0}}, {"Ampt_ptotal", {0,0}}, {"Ampt_pion-", {0,0}}, {"Ampt_pion+", {0,0}}, {"Ampt_piontotal", {0,0}}, {"Ampt_gamma", {0,0}}, {"Ampt_neutron", {0,0}}, {"Ampt_neutronbar", {0,0}}, {"Ampt_neutrontotal", {0,0}}, {"Ampt_pion0", {0, 0}}, {"Ampt_pionall", {0, 0}}});
//	analyzer.set_sets({{"Sim_0p0s", {0,0}}, {"Sim_0p0s_No_Rotate", {0,0}},
//				{"Sim_0p0s_Eff_Hole3-4", {0,0}}, {"Sim_0p0s_Eff_Hole3-4_No_Rotate", {0,0}},
//				{"Sim_0p0s_Eff", {0,0}}, {"Sim_0p0s_Eff_No_Rotate", {0,0}},
//				{"Sim_0p0s_Flow_08res_05v2", {0,0}}, {"Sim_0p0s_Flow_08res_05v2_No_Rotate", {0,0}},
//				{"Sim_0p0s_Flow_05res_05v2", {0,0}}, {"Sim_0p0s_Flow_05res_05v2_No_Rotate", {0,0}},
//				{"Sim_0p0s_Flow_099res_05v2", {0,0}}, {"Sim_0p0s_Flow_099res_05v2_No_Rotate", {0,0}},
//				{"Sim_05p05s", {0,0}}, {"Sim_05p05s_No_Rotate", {0,0}},
//				{"Sim_05p05s_Eff_Hole3-4", {0,0}}, {"Sim_05p05s_Eff_Hole3-4_No_Rotate", {0,0}},
//				{"Sim_05p05s_Eff", {0,0}}, {"Sim_05p05s_Eff_No_Rotate", {0,0}},
//				{"Sim_05p05s_Flow_08res_05v2", {0,0}}, {"Sim_05p05s_Flow_08res_05v2_No_Rotate", {0,0}},
//				{"Sim_05p05s_Flow_05res_05v2", {0,0}}, {"Sim_05p05s_Flow_05res_05v2_No_Rotate", {0,0}},
//				{"Sim_05p05s_Flow_099res_05v2", {0,0}}, {"Sim_05p05s_Flow_099res_05v2_No_Rotate", {0,0}},
//				{"eta05", {0,0}}});
//	analyzer.set_sets({{"eta05", {0,0}}, {"eta05_No_Rotate", {0,0}}, {"eta1", {0,0}}, {"eta1_No_Rotate", {0,0}}});
//	analyzer.set_sets({{"Sim_Flow_08res_05v2", {0,0}}, {"Sim_Flow_08res_05v2_No_Rotate", {0,0}}, {"Sim_Flow_05res_05v2", {0,0}}, {"Sim_Flow_05res_05v2_No_Rotate", {0,0}}, {"Sim_Flow_05v2_Reaction_Plane", {0,0}}, {"Sim_Flow_05v2_Reaction_Plane_No_Rotate", {0,0}}});
//	analyzer.set_sets({{"Sim_Eff_Flow", {0,0}}, {"Sim_Eff_Flow_No_Rotate", {0,0}}, {"Sim_Flow", {0,0}}, {"Sim_Flow_No_Rotate", {0,0}}, {"Sim_Eff_Hole3-4_Flow", {0,0}}, {"Sim_Eff_Hole3-4_Flow_No_Rotate", {0,0}}, {"Sim_Flow_Reaction_Plane", {0,0}}});
//	analyzer.set_sets({{"Sim_0p0s", {0,0}}, {"Sim_0p0s_No_Rotate", {0,0}}, {"Sim_05p002s", {0,0}}, {"Sim_05p002s_No_Rotate", {0,0}}, {"Sim_15p002s", {0,0}}, {"Sim_0p0s_Eff", {0,0}}, {"Sim_0p0s_No_Rotate_Eff", {0,0}}, {"Sim_05p002s_Eff", {0,0}}, {"Sim_05p002s_No_Rotate_Eff", {0,0}},{"Sim_0p0s_Eff_Hole3-4", {0,0}}, {"Sim_0p0s_No_Rotate_Eff_Hole3-4", {0,0}}, {"Sim_05p002s_Eff_Hole3-4", {0,0}}, {"Sim_05p002s_No_Rotate_Eff_Hole3-4", {0,0}}});
//	analyzer.set_sets({{"eta05_n1ratios_dca1", {0, 4}}, {"eta05_n1ratios_dca3", {0, 4}}, {"eta1_n1ratios_dca1", {0, 4}}, {"eta1_n1ratios_dca3", {0, 4}},
//		{"Ampt_eta05_n1ratios", {0, 4}}, {"Ampt_eta1_n1ratios", {0, 4}}, {"Sim_n1ratios_0p0s_eta05_dca3", {0,4}}});
//	analyzer.set_set_combos({{"Eta1_05_dca1", {"eta05_n1ratios_dca1", "eta1_n1ratios_dca1"}}, {"Eta05_dca1_3", {"eta05_n1ratios_dca1", "eta05_n1ratios_dca3"}},
//		{"Eta1_05_dca3", {"eta05_n1ratios_dca3", "eta1_n1ratios_dca3"}}, {"Eta1_dca1_3", {"eta1_n1ratios_dca1", "eta1_n1ratios_dca3"}},
//		{"Ampt_Eta1_dca1", {"Ampt_eta1_n1ratios", "eta1_n1ratios_dca1"}}, {"Ampt_Eta05_dca1", {"Ampt_eta05_n1ratios", "eta05_n1ratios_dca1"}},
//		{"Ampt_Eta1_dca3", {"Ampt_eta1_n1ratios", "eta1_n1ratios_dca3"}}, {"Ampt_Eta05_dca3", {"Ampt_eta05_n1ratios", "eta05_n1ratios_dca3"}},
//		{"Sim_Ampt_Eta1_dca3", {"Ampt_eta1_n1ratios", "Sim_n1ratios_0p0s_eta05_dca3"}}});
//	analyzer.set_sets({{{"Sim_n1ratios_0p0s_No_Rotate_test", {0,0}}}});
//	analyzer.set_sets({{"eta05_n1ratios_dca1", {0, 4}}, {"eta05_n1ratios_dca3", {0, 4}}, {"eta1_n1ratios_dca1", {0, 4}}, {"eta1_n1ratios_dca3", {0, 4}},
//		{"Ampt_eta05_n1ratios", {0, 4}}, {"Ampt_eta1_n1ratios", {0, 4}}, {"Sim_n1ratios_0p0s_eta05_dca3", {0,4}}});
//	analyzer.set_set_combos({{"Eta1_05_dca1", {"eta05_n1ratios_dca1", "eta1_n1ratios_dca1"}}, {"Eta05_dca1_3", {"eta05_n1ratios_dca1", "eta05_n1ratios_dca3"}},
//		{"Eta1_05_dca3", {"eta05_n1ratios_dca3", "eta1_n1ratios_dca3"}}, {"Eta1_dca1_3", {"eta1_n1ratios_dca1", "eta1_n1ratios_dca3"}},
//		{"Ampt_Eta1_dca1", {"Ampt_eta1_n1ratios", "eta1_n1ratios_dca1"}}, {"Ampt_Eta05_dca1", {"Ampt_eta05_n1ratios", "eta05_n1ratios_dca1"}},
//		{"Ampt_Eta1_dca3", {"Ampt_eta1_n1ratios", "eta1_n1ratios_dca3"}}, {"Ampt_Eta05_dca3", {"Ampt_eta05_n1ratios", "eta05_n1ratios_dca3"}},
//		{"Sim_Ampt_Eta05_dca3", {"Ampt_eta05_n1ratios", "Sim_n1ratios_0p0s_eta05_dca3"}}, {"Sim_BES1_Eta05_dca3", {"eta05_n1ratios_dca3", "Sim_n1ratios_0p0s_eta05_dca3"}}});
//	analyzer.set_sets({{"eta05_n1ratios_dca3", {0, 4}}, {"Ampt_eta05_n1ratios", {0, 4}}});
//	analyzer.set_set_combos({{"Bes_Ampt", {"eta05_n1ratios_dca3", "Ampt_eta05_n1ratios"}}});
//	analyzer.set_sets({{"eta05_n1ratios_dca1", {0, 4}}, {"eta05_n1ratios_dca3", {0, 4}}, {"eta1_n1ratios_dca1", {0, 4}}, {"eta1_n1ratios_dca3", {0, 4}},
//		{"Ampt_eta05_n1ratios", {0, 4}}, {"Ampt_eta1_n1ratios", {0, 4}}, {"Sim_n1ratios_0p0s_eta05_dca3", {0,4}}});
//	analyzer.set_set_combos({{"Eta1_05_dca1", {"eta05_n1ratios_dca1", "eta1_n1ratios_dca1"}}, {"Eta05_dca1_3", {"eta05_n1ratios_dca1", "eta05_n1ratios_dca3"}},
//		{"Eta1_05_dca3", {"eta05_n1ratios_dca3", "eta1_n1ratios_dca3"}}, {"Eta1_dca1_3", {"eta1_n1ratios_dca1", "eta1_n1ratios_dca3"}},
//		{"Ampt_Eta1_dca1", {"Ampt_eta1_n1ratios", "eta1_n1ratios_dca1"}}, {"Ampt_Eta05_dca1", {"Ampt_eta05_n1ratios", "eta05_n1ratios_dca1"}},
//		{"Ampt_Eta1_dca3", {"Ampt_eta1_n1ratios", "eta1_n1ratios_dca3"}}, {"Ampt_Eta05_dca3", {"Ampt_eta05_n1ratios", "eta05_n1ratios_dca3"}},
//		{"Sim_Ampt_Eta05_dca3", {"Ampt_eta05_n1ratios", "Sim_n1ratios_0p0s_eta05_dca3"}}, {"Sim_BES1_Eta05_dca3", {"eta05_n1ratios_dca3", "Sim_n1ratios_0p0s_eta05_dca3"}}});
//	analyzer.set_sets({{"rapid05_n1ratios_dca1", {0, 4}}, {"rapid05_n1ratios_dca3", {0, 4}}, {"rapid1_n1ratios_dca1", {0, 4}}, {"rapid1_n1ratios_dca3", {0, 4}},
//		{"Ampt_rapid05_n1ratios", {0, 4}}, {"Ampt_rapid1_n1ratios", {0, 4}}});
//	analyzer.set_set_combos({{"rapid1_05_dca1", {"rapid05_n1ratios_dca1", "rapid1_n1ratios_dca1"}}, {"rapid05_dca1_3", {"rapid05_n1ratios_dca1", "rapid05_n1ratios_dca3"}},
//		{"rapid1_05_dca3", {"rapid05_n1ratios_dca3", "rapid1_n1ratios_dca3"}}, {"rapid1_dca1_3", {"rapid1_n1ratios_dca1", "rapid1_n1ratios_dca3"}},
//		{"Ampt_rapid1_dca1", {"Ampt_rapid1_n1ratios", "rapid1_n1ratios_dca1"}}, {"Ampt_rapid05_dca1", {"Ampt_rapid05_n1ratios", "rapid05_n1ratios_dca1"}},
//		{"Ampt_rapid1_dca3", {"Ampt_rapid1_n1ratios", "rapid1_n1ratios_dca3"}}, {"Ampt_rapid05_dca3", {"Ampt_rapid05_n1ratios", "rapid05_n1ratios_dca3"}},
//		{"Ampt_rapid05_1", {"Ampt_rapid05_n1ratios", "Ampt_rapid1_n1ratios"}}});
	analyzer.set_sets({{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0, 54}}, {"Ampt_rapid05_n1ratios", {0, 54}}, {"Ampt_rapid05_n1ratios_dca1_ReactionPlane_", {0, 16}}});
	analyzer.set_set_combos({{"bes_ampt", {"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", "Ampt_rapid05_n1ratios"}}, {"ampt_rp", {"Ampt_rapid05_n1ratios", "Ampt_rapid05_n1ratios_dca1_ReactionPlane_"}}});

	analyzer.analyze();
}


map<string, vector<int>> get_sets(string dir_path) {
	map<string, vector<int>> out_vec;
	vector<string> dir_names = get_files_in_dir(dir_path, "txt", "name", true);
	for (string dir_name : dir_names) {
		if (dir_name != "." && dir_name != "..") {
			cout << dir_name << endl;
			string set_name = strip_flag(dir_name, "_", true, false);
			int set_num = stoi(strip_flag(dir_name, "_", false, true));
			out_vec[set_name].push_back(set_num);
		}
	}

	for (pair<string, vector<int>> set : out_vec) {
		int high_num = 0;
		while (count(set.second.begin(), set.second.end(), high_num) > 0) { high_num++; }
		if (high_num == 0) { cout << "No zero dir num for " << set.first << endl; }
		out_vec[set.first] = { 0, high_num - 1 };
	}

	return out_vec;
}


map<string, vector<int>> get_sets(string dir_path, string val_flag, pair<float, float> val_range, int dec_pos, vector<string> other_flags) {
	map<string, vector<int>> out_vec;
	vector<string> dir_names = get_files_in_dir(dir_path, "txt", "name", true);
	for(string dir_name:dir_names) {
		if(in_string(dir_name, val_flag) && in_string(dir_name, other_flags, true)) {
			float val_flag_num = str_num_dec(get_flag_trail(dir_name, val_flag, "_")[0], dec_pos);
			if(val_flag_num <= val_range.second && val_flag_num >= val_range.first) {
				string set_name = strip_flag(dir_name, "_", true, false);
				int set_num = stoi(strip_flag(dir_name, "_", false, true));
				out_vec[set_name].push_back(set_num);
			}
		}
	}

	for(pair<string, vector<int>> set:out_vec) {
		int high_num = 0;
		while(count(set.second.begin(), set.second.end(), high_num) > 0) { high_num++; }
		if(high_num == 0) { cout << "No zero dir num for " << set.first << endl; }
		out_vec[set.first] = {0, high_num - 1};
	}

	return out_vec;
}


vector<string> get_set_names(map<string, vector<int>> sets) {
	vector<string> set_names;
	for(pair<string, vector<int>> set:sets) {
		set_names.push_back(set.first);
	}

	return(set_names);
}


map<string, vector<int>> combine_sets(vector<map<string, vector<int>>> sets_set) {
	map<string, vector<int>> combo_sets;
	for(map<string, vector<int>> sets:sets_set) {
		combo_sets.insert(sets.begin(), sets.end());
	}

	return combo_sets;
}


class mem_class {
public:
	//

private:
	int hold;
};

void mem_test() {
	map<int, vector<double>> a;  // Only 2GB until failure, should look into this later.
	for (int i = 0; i < 2000000000; i++) {
		for (int j = 0; j < 20; j++) {
			a[i].push_back((double)i * j / 40);
		}
		if (i % 1000000 == 0) { cout << "i: " << i << endl; }// this_thread::sleep_for(chrono::seconds(1)); }
	}

	int x;
	cout << "Enter to quit" << endl;
	cin >> x;
}


void stat_sd_nan_test() {
	int energy = 39;
	string path = "C:/Users/Dylan/Research/Data_Mix/default_sys_test/rapid05_n1ratios_dca097_m2r487_nhfit15_nsprx103_m2s0_sys_0/" + to_string(energy) + "GeV/";
	int div = 60;
	int cent = 5;

	//int a = 5908521001;
	//long int b = 5908521001;
	//long long int c = 5908521001;
	//cout << "int a: " << a << " long int b: " << b << " long long int c: " << c << endl;
	AzimuthBinData az_data(div);
	az_data.read_data_from_dir(path, div, cent);
	Stats stat;
	stat.set_distribution(az_data.get_ratio_hist());
	cout << "Dist num: " << stat.get_dist_num() << endl;
	cout << "Mean: " << (string)stat.get_mean() << endl;
	cout << "SD: " << (string)stat.get_standard_deviation() << endl;
	cout << "Skew: " << (string)stat.get_skewness() << endl;
	cout << "Kurt: " << (string)stat.get_kurtosis() << endl;
	cout << "CM 2: " << (string)stat.get_central_moment(2) << endl;
	cout << "CM 4: " << (string)stat.get_central_moment(4) << endl;

	//TFile test("C:/Users/Dylan/Research/Results/Azimuth_Analysis/az_data_mix_debug_6-6-21.root", "RECREATE");
	//az_data.canvas_ratio_dist(("debug_mix_ratio_dist_" + to_string(energy) + "GeV_" + to_string(cent) + "cent_" + to_string(div) + "divs").data());
	//az_data.canvas_ratio_dist_slow(("debug_mix_ratio_dist_slow_" + to_string(energy) + "GeV_" + to_string(cent) + "cent_" + to_string(div) + "divs").data());
	//az_data.canvas_ratio_dist_slow(("debug_mix_ratio_dist_slow_10_" + to_string(energy) + "GeV_" + to_string(cent) + "cent_" + to_string(div) + "divs").data(), 10);
	//test.Close();

}
