/*
 * main.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */


#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <cstdlib>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLine.h>
#include <TLegend.h>

#include "ThreadPool.h"
#include "TreeReader.h"
#include "AmptCentralityMaker.h"
#include "AmptConverter.h"
#include "TreeLeaves.h"
#include "ratio_methods.h"
#include "file_io.h"

#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"

using namespace std;


void read_class();

void run_set(int energy, int set_num, string set_name);

clock_t start = clock();
auto start_sys = chrono::system_clock::now();


int main(int argc, char** argv) {
	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Track.h");
	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Event.h");
//	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Track.cpp");
//	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Event.cpp");
	read_class();


	cout << endl << "donzo" << endl;
	return(0);

}




void read_class() {
//	map<string, pair<int, int>> set_pairs = {{"Sim_Flow", {0,0}}, {"Sim_Flow_No_Rotate", {0,0}}, {"Sim_Eff_Hole3-4_Flow", {0,0}}, {"Sim_Eff_Hole3-4_Flow_No_Rotate", {0,0}}};
//	map<string, pair<int, int>> set_pairs = {{"Sim_Flow_08res_05v2", {0,0}}, {"Sim_Flow_08res_05v2_No_Rotate", {0,0}}, {"Sim_Flow_05res_05v2", {0,0}}, {"Sim_Flow_05res_05v2_No_Rotate", {0,0}}};
//	map<string, pair<int, int>> set_pairs {{"Sim_Flow_05v2_08res_Test", {0,0}}, {"Sim_Flow_05v2_05res_Test", {0,0}}};
//	map<string, pair<int, int>> set_pairs = {{"Sim_0p0s", {0,0}}, {"Sim_05p002s", {0,0}}, {"Sim_0p0s_No_Rotate", {0,0}}, {"Sim_05p002s_No_Rotate", {0,0}},
//				{"Sim_0p0s_Eff_Hole3-4", {0,0}}, {"Sim_05p002s_Eff_Hole3-4", {0,0}}, {"Sim_0p0s_Eff_Hole3-4_No_Rotate", {0,0}}, {"Sim_05p002s_Eff_Hole3-4_No_Rotate", {0,0}},
//				{"Sim_0p0s_Eff", {0,0}}, {"Sim_05p002s_Eff", {0,0}}, {"Sim_0p0s_Eff_No_Rotate", {0,0}}, {"Sim_05p002s_Eff_No_Rotate", {0,0}},
//				{"Sim_0p0s_Flow_08res_05v2", {0,0}}, {"Sim_05p002s_Flow_08res_05v2", {0,0}}, {"Sim_0p0s_Flow_08res_05v2_No_Rotate", {0,0}}, {"Sim_05p002s_Flow_08res_05v2_No_Rotate", {0,0}},
//				{"Sim_0p0s_Flow_05res_05v2", {0,0}}, {"Sim_05p002s_Flow_05res_05v2", {0,0}}, {"Sim_0p0s_Flow_05res_05v2_No_Rotate", {0,0}}, {"Sim_05p002s_Flow_05res_05v2_No_Rotate", {0,0}},
//				{"Sim_0p0s_Flow_099res_05v2", {0,0}}, {"Sim_05p002s_Flow_099res_05v2", {0,0}}, {"Sim_0p0s_Flow_099res_05v2_No_Rotate", {0,0}}, {"Sim_05p002s_Flow_099res_05v2_No_Rotate", {0,0}}
//				};
//map<string, pair<int, int>> set_pairs = {{"Sim_05p05s", {0,0}}, {"Sim_05p05s_No_Rotate", {0,0}},
//				{"Sim_05p05s_Eff_Hole3-4", {0,0}}, {"Sim_05p05s_Eff_Hole3-4_No_Rotate", {0,0}},
//				{"Sim_05p05s_Eff", {0,0}}, {"Sim_05p05s_Eff_No_Rotate", {0,0}},
//				{"Sim_05p05s_Flow_08res_05v2", {0,0}}, {"Sim_05p05s_Flow_08res_05v2_No_Rotate", {0,0}},
//				{"Sim_05p05s_Flow_05res_05v2", {0,0}}, {"Sim_05p05s_Flow_05res_05v2_No_Rotate", {0,0}},
//				{"Sim_05p05s_Flow_099res_05v2", {0,0}}, {"Sim_05p05s_Flow_099res_05v2_No_Rotate", {0,0}}
//				};
//	map<string, pair<int, int>> set_pairs = {{"Sim_05p002s", {0,0}}, {"Sim_05p002s_Eff_Hole3-4", {0,0}}, {"Sim_05p002s_Eff", {0,0}}, {"Sim_05p002s_Flow_08res_05v2", {0,0}}, {"Sim_05p002s_Flow_05res_05v2", {0,0}}, {"Sim_05p002s_Flow_099res_05v2", {0,0}}};
//	map<string, pair<int, int>> set_pairs = {{"eta05", {0,0}}, {"eta1", {0,0}}, {"eta05_No_Rotate", {0,0}}, {"eta1_No_Rotate", {0,0}}};
//	map<string, pair<int, int>> set_pairs = {{"eta05_old", {5,9}}};
	map<string, pair<int, int>> set_pairs = {{"Ampt_p+", {0, 0}}, {"Ampt_ptotal", {0, 0}}};
//	map<string, pair<int, int>> set_pairs = {{"Sim_01p002s_Flat100_", {0,0}}, {"Sim_01p05s_Flat100_", {0,0}}, {"Sim_002p002s_Flat100_", {0,0}}, {"Sim_002p05s_Flat100_", {0,0}}};
//	map<string, pair<int, int>> set_pairs = {{"Sim_05p05s_Flat1000", {0,2}}};

	int set_sleep = 100;
	int energy_sleep = 1;

	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(pair<string, pair<int, int>> set_pair:set_pairs) {
			for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
				string set_dir = set_pair.first + to_string(set_num) + "/";
				cout << endl << "Queueing " + set_dir <<  "  set_num: " << set_num << endl << endl;
				vector<int> energy_list {7, 11, 39, 27, 62, 19};
				for(int energy:energy_list) {
					pool.enqueue(run_set, energy, set_num, set_pair.first);
					this_thread::sleep_for(chrono::seconds(energy_sleep));
				}
				this_thread::sleep_for(chrono::seconds(set_sleep));
			}
		}
	}
}


void run_set(int energy, int set_num, string set_name) {
	string base_path = "/home/dylan/Research/";
	string in_base_path = "/media/dylan/SSD_Storage/Research/";//base_path;
	string out_base_path = base_path;
	int ref = 3;

	string in_path = in_base_path + "Trees_Ampt/";
	string out_dir = out_base_path + "Twopcorr_Ampt/";

//	map<int, int> sim_cent_events = {{0, 500000}, {1, 500000}, {2, 500000}, {3, 500000}, {4, 500000}, {5, 500000}, {6, 500000}, {7, 500000}, {8, 20000000}};
	map<int, int> sim_cent_events = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 20000000}};

	string set_dir = set_name + to_string(set_num) + "/";

	TreeReader reader(energy, ref);
	reader.set_cbwc(false);
	reader.set_cent_binning(9);

	reader.set_in_path(in_path);
	reader.set_out_path(out_dir+set_dir);
	reader.set_qa_path(out_dir+set_dir+to_string(energy)+"GeV/");
	reader.set_qa_name("QA_");
	reader.set_set_name(set_name + to_string(set_num));
	reader.set_tree_name("tree");

	if(in_string(set_name, "EP_Rotate"))  { reader.set_event_plane(true); }
	else { reader.set_event_plane(false); }

	if(in_string(set_name, {"No_Rotate", "EP_Rotate"}, false)) { reader.set_rotate_random(false); }
	else{ reader.set_rotate_random(true); }

	if(in_string(set_name, "Pile_Up_01_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.01); }
	else if(in_string(set_name, "Pile_Up_008_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.008); }
	else if(in_string(set_name, "Pile_Up_005_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.005); }
	else if(in_string(set_name, "Pile_Up_002_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.002); }
	else if(in_string(set_name, "Pile_Up_001_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.001); }
	else if(in_string(set_name, "Pile_Up_0008_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.0008); }
	else if(in_string(set_name, "Pile_Up_0005_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.0005); }
	else if(in_string(set_name, "Pile_Up_0002_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.0002); }
	else { reader.set_pile_up(false); reader.set_pile_up_prob(0); }

	if(in_string(set_name,  "No_BTof_Rej")) {
		reader.cut.max_slope = {{7, 999}, {11, 999}, {14, 999}, {19, 999}, {27, 999}, {39, 999}, {62, 999}, {200, 999}};
		reader.cut.min_slope = {{7, -999}, {11, -999}, {14, -999}, {19, -999}, {27, -999}, {39, -999}, {62, -999}, {200, -999}};
	}

	if(in_string(set_name, "eta1")) { reader.cut.min_eta = -1.0, reader.cut.max_eta = 1.0; }
	else if(in_string(set_name, "eta05")) { reader.cut.min_eta = -0.5, reader.cut.max_eta = 0.5; }

	if(in_string(set_name,  "Efficiency8")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.8); }
	else if(in_string(set_name,  "Efficiency7")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.7); }
	else if(in_string(set_name,  "Efficiency6")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.6); }
	else if(in_string(set_name,  "Efficiency5")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.5); }
	else if(in_string(set_name,  "Efficiency4")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.4); }
	else if(in_string(set_name,  "Efficiency3")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.3); }
	else if(in_string(set_name,  "Efficiency2")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.2); }
	else if(in_string(set_name,  "Efficiency1")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.1); }
	else if(in_string(set_name,  "Efficiency08")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.08); }
	else if(in_string(set_name,  "Efficiency05")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.05); }
	else if(in_string(set_name,  "Efficiency025")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.025); }
	else if(in_string(set_name,  "Efficiency01")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.01); }
	else { reader.set_efficiency(false); reader.set_efficiency_prob(0); }

	if(in_string(set_name, {"Sim", "0p0s"}, true)) { reader.sim.set_p_group(0.0); reader.sim.set_spread_sigma(0.0); }
	else if(in_string(set_name, {"Sim", "05p002s"}, true)) { reader.sim.set_p_group(0.05); reader.sim.set_spread_sigma(0.002); }
	else if(in_string(set_name, {"Sim", "05p05s"}, true)) { reader.sim.set_p_group(0.05); reader.sim.set_spread_sigma(0.5); }
	else if(in_string(set_name, {"Sim", "01p002s"}, true)) { reader.sim.set_p_group(0.01); reader.sim.set_spread_sigma(0.002); }
	else if(in_string(set_name, {"Sim", "01p05s"}, true)) { reader.sim.set_p_group(0.01); reader.sim.set_spread_sigma(0.5); }
	else if(in_string(set_name, {"Sim", "002p002s"}, true)) { reader.sim.set_p_group(0.002); reader.sim.set_spread_sigma(0.002); }
	else if(in_string(set_name, {"Sim", "002p05s"}, true)) { reader.sim.set_p_group(0.002); reader.sim.set_spread_sigma(0.5); }
	else if(in_string(set_name, {"Sim", "15p002s"}, true)) { reader.sim.set_p_group(0.15); reader.sim.set_spread_sigma(0.002); }
	else { reader.sim.set_p_group(0.0); reader.sim.set_spread_sigma(0.0); }

	if(in_string(set_name, {"Sim", "Eff"}, true)) {
		reader.set_sim_eff(true);
	} else {
		reader.set_sim_eff(false);
	}
	if(in_string(set_name, {"Sim", "Eff", "Hole3-4"}, true)) {
		reader.set_sim_eff_dist_path(in_base_path + "Sim_Efficiency_Hists.root", "Hole_3to4");
	}
	if(in_string(set_name, "Sim")) {
		reader.sim.set_hom_eff(1.0);
		reader.set_sim_proton_dist_dataset(in_base_path + "Data_Old_Ref3/eta050/");
	}
	if(in_string(set_name, {"Sim", "Flow"}, true)) {
		reader.set_sim_flow(true);
		if(in_string(set_name, {"08res", "05v2"}, true)) { reader.sim.set_flow(0.05, 0.8, 0.0001); }
		else if(in_string(set_name, {"05res", "05v2"}, true)) { reader.sim.set_flow(0.05, 0.5, 0.0001); }
		else if(in_string(set_name, {"099res", "05v2"}, true)) { reader.sim.set_flow(0.05, 0.99, 0.0001); }
		else if(in_string(set_name, "05v2")) { reader.sim.set_flow(0.05, 0.0, 0.0001); }
	} else {
		reader.set_sim_flow(false);
	}
	if(in_string(set_name, {"Sim", "Flat1000"}, true)) { reader.sim.set_flat_dist(1000); reader.set_particle_dist_hist_max(1000); }
	else if(in_string(set_name, {"Sim", "Flat500"}, true)) { reader.sim.set_flat_dist(500); reader.set_particle_dist_hist_max(500); }
	else if(in_string(set_name, {"Sim", "Flat100"}, true)) { reader.sim.set_flat_dist(100); reader.set_particle_dist_hist_max(100); }

	if(in_string(set_name, "Ampt")) {
//		reader.ampt_cent = AmptCentralityMaker("/media/dylan/SSD_Storage/Research/Trees_Ampt/" + to_string(energy) + "GeV_Cent/", "ref" + to_string(ref));
//		map<int, float> opt_bmax {{7, 14.75}, {11, 14.5312}, {19, 14.3438}, {27, 14.2969}, {39, 13.5312}, {62, 13.6094}};
//		reader.ampt_cent.set_max_b(opt_bmax[energy]);
	}

	if(in_string(set_name, "p+")) { reader.cut.charge = +1; reader.set_ampt_particle_pid({2212}); }
	if(in_string(set_name, "p-")) { reader.cut.charge = -1; reader.set_ampt_particle_pid({-2212}); }
	if(in_string(set_name, "ptotal")) { reader.set_check_charge(false); }

	if(!in_string(set_name, "Ampt") && in_string(set_name, "pion")) {
		reader.cut.min_m2 = -0.15;
		reader.cut.max_m2 = 0.15;
		reader.set_particle("Pion");
	}
	if(in_string(set_name, "pion+")) { reader.cut.charge = 1; reader.set_ampt_particle_pid({211}); }
	if(in_string(set_name, "pion-")) { reader.cut.charge = -1; reader.set_ampt_particle_pid({-211}); }
	if(in_string(set_name, "pion0")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({111}); }
	if(in_string(set_name, "piontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({211, -211}); }
	if(in_string(set_name, "pionall")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({211, 111, -211}); }

	if(in_string(set_name, "e+")) { reader.cut.charge = +1; reader.set_ampt_particle_pid({-11}); }
	else if(in_string(set_name, "e-")) { reader.cut.charge = -1; reader.set_ampt_particle_pid({11}); }
	else if(in_string(set_name, "etotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({11, -11}); }

	if(in_string(set_name, "neutron")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2112}); }
	if(in_string(set_name, "neutronbar")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({-2112}); }
	if(in_string(set_name, "neutrontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2112, -2112}); }

	if(in_string(set_name, "gamma")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({22}); }

	if(in_string(set_name, "baryontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2212, 2112, -2212, -2112}); }
	if(in_string(set_name, "mesontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({211, 111, -211}); }
	if(in_string(set_name, "hadrontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2212, 2112, 211, 111, -211, -2212, -2112}); }
	if(in_string(set_name, "hadronobs")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2212, 211, -211, -2212}); }

	reader.set_mixed_sets(false);
	reader.set_rand_data(false);

	reader.set_mixed(true);
	reader.mix.set_max_events(250);
	reader.mix.set_min_events(150);
	if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
	else { reader.mix.set_mixes_per_event(10); }
	if(in_string(set_name, "Sim") || in_string(set_name, "Ampt")) { reader.mix.set_mixes_per_event(10); }

	if(in_string(set_name, "Sim")) {
		reader.sim_events(sim_cent_events);
	} else if(in_string(set_name, "Ampt")) {
		reader.read_ampt_trees();
	} else{
		reader.read_trees();
	}
}
