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
#include "AzimuthBinData.h"
#include "AzimuthBinAnalyzer.h"
#include "BinomialAnalyzer.h"

using namespace std;

void azimuth_bin_analyze();
void binomial_analyze();


int main() {
	cout << "Running AzimuthBinAnalyzer" << endl << endl;
	azimuth_bin_analyze();
	cout << endl << endl << "Running BinomialAnalyzer" << endl << endl;
	binomial_analyze();

	cout << "donzo" << endl;
	return(0);
}


void azimuth_bin_analyze() {
	AzimuthBinAnalyzer analyzer;
	analyzer.set_in_path("/home/dylan/Research/Data_Sim/");
	analyzer.set_in_mix_path("/home/dylan/Research/Data_Sim_Mix/");
	analyzer.set_out_path("/home/dylan/Research/Results/");
	analyzer.set_out_root_name("4-29-20_sim_flat_cent8.root");
//	analyzer.set_energies({7, 11, 19, 27, 39, 62});
	analyzer.set_energies({7});
	analyzer.set_all_centralities({8});
	analyzer.set_plot_dists(true);
	analyzer.set_can_wh(936, 780);
//	analyzer.set_sets({{"Sim_0p0s", {0,0}}, {"Sim_0p0s_No_Rotate", {0,0}}, {"Sim_05p002s", {0,0}}, {"Sim_05p002s_No_Rotate", {0,0}}, {"Sim_15p002s", {0,0}}, {"Sim_0p0s_Eff", {0,0}}, {"Sim_0p0s_No_Rotate_Eff", {0,0}}, {"Sim_05p002s_Eff", {0,0}}, {"Sim_05p002s_No_Rotate_Eff", {0,0}},{"Sim_0p0s_Eff_Hole3-4", {0,0}}, {"Sim_0p0s_No_Rotate_Eff_Hole3-4", {0,0}}, {"Sim_05p002s_Eff_Hole3-4", {0,0}}, {"Sim_05p002s_No_Rotate_Eff_Hole3-4", {0,0}}});
//	analyzer.set_sets({{"Sim_Eff_Flow", {0,0}}, {"Sim_Eff_Flow_No_Rotate", {0,0}}, {"Sim_Flow", {0,0}}, {"Sim_Flow_No_Rotate", {0,0}}, {"Sim_Eff_Hole3-4_Flow", {0,0}}, {"Sim_Eff_Hole3-4_Flow_No_Rotate", {0,0}}, {"Sim_Flow_Reaction_Plane", {0,0}}});
//	analyzer.set_sets({{"Sim_Flow_08res_05v2", {0,0}}, {"Sim_Flow_08res_05v2_No_Rotate", {0,0}}, {"Sim_Flow_05res_05v2", {0,0}}, {"Sim_Flow_05res_05v2_No_Rotate", {0,0}}, {"Sim_Flow_05v2_Reaction_Plane", {0,0}}, {"Sim_Flow_05v2_Reaction_Plane_No_Rotate", {0,0}}});
//	analyzer.set_sets({{"eta05", {0,0}}, {"eta05_No_Rotate", {0,0}}, {"eta1", {0,0}}, {"eta1_No_Rotate", {0,0}}});
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
//	analyzer.set_set_combos({{"Basic_Effect_With_BES1", {"Sim_0p0s", "Sim_05p05s", "eta05"}},
//		{"Basic_Effect", {"Sim_0p0s", "Sim_05p05s"}},
//		{"Hole_Efficiency", {"Sim_0p0s_Eff_Hole3-4", "Sim_05p05s_Eff_Hole3-4"}},
//		{"Hole_Efficiency_BES1", {"Sim_0p0s_Eff_Hole3-4", "Sim_05p05s_Eff_Hole3-4", "eta05"}},
//		{"BES1_Efficiency_BES1", {"Sim_0p0s_Eff", "Sim_05p05s_Eff", "eta05"}},
//		{"BES1_Efficiency", {"Sim_0p0s_Eff", "Sim_05p05s_Eff"}},
//		{"Flow_Res", {"Sim_0p0s_Flow_05res_05v2", "Sim_0p0s_Flow_099res_05v2"}},
//		{"Flow_Res_BES1", {"Sim_0p0s_Flow_05res_05v2", "Sim_0p0s_Flow_099res_05v2", "eta05"}},
//	});
//	analyzer.set_sets({{"Sim_0p0s", {0,0}}, {"Sim_05p002s", {0,0}}});
//	analyzer.set_sets({{"p+", {0,4}}, {"p-", {0,4}}, {"ptotal", {0,4}}, {"pion+", {0,0}}, {"pion-", {0,0}}, {"piontotal", {0,0}}});
//	analyzer.set_sets({{"Ampt_p+", {0,4}}, {"Ampt_p-", {0,4}}, {"Ampt_ptotal", {0,4}}, {"Ampt_ptotal_Efficiency8", {0,2}}, {"Ampt_ptotal_Efficiency5", {0,2}}, {"Ampt_ptotal_Efficiency3", {0,2}}, {"Ampt_ptotal_Efficiency1", {0,2}}, {"Ampt_ptotal_Efficiency0", {0,2}}});
//	analyzer.set_set_combos({{"Proton_AntiProton_TotalProton", {"Ampt_p+", "Ampt_p-", "Ampt_ptotal"}}, {"TotalProton_Efficiency", {"Ampt_ptotal_Efficiency8", "Ampt_ptotal_Efficiency5", "Ampt_ptotal_Efficiency3", "Ampt_ptotal_Efficiency1", "Ampt_ptotal_Efficiency0"}}});
//	analyzer.set_set_combos({{"Proton_AntiProton_TotalProton", {"p+", "p-", "ptotal"}}, {"Pion_AntiPion_TotalPion", {"pion+", "pion-", "piontotal"}}});
	analyzer.set_sets({{"Sim_0p0s_Flat100_", {0,0}}, {"Sim_05p002s_Flat100_", {0,0}}, {"Sim_05p05s_Flat100_", {0,0}}});
	analyzer.analyze();
}


void binomial_analyze() {
	BinomialAnalyzer analyzer;
	analyzer.set_in_path("/home/dylan/Research/Data_Sim/");
	analyzer.set_in_mix_path("/home/dylan/Research/Data_Sim_Mix/");
	analyzer.set_out_path("/home/dylan/Research/Results/");
	analyzer.set_out_root_name("4-29-20_binom_sim_flat_cent8.root");
	analyzer.set_energies({7});
	analyzer.set_centralities({8});
	analyzer.set_can_wh(950, 828);
//	analyzer.set_sets({{"p+", {0,4}}, {"p-", {0,4}}, {"ptotal", {0,4}}});
	analyzer.set_sets({{"Sim_0p0s_Flat100_", {0,0}}, {"Sim_05p002s_Flat100_", {0,0}}, {"Sim_05p05s_Flat100_", {0,0}}});
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
	analyzer.analyze();
}

