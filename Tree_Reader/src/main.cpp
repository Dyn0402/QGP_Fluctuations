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

#include "ThreadPool.h"
#include "TreeReader.h"
#include "AmptConverter.h"
#include "ratio_methods.h"
#include "file_io.h"

#include "../StRoot/StRefMultCorr/CentralityMaker.h"
#include "../StRoot/StRefMultCorr/StRefMultCorr.h"

using namespace std;


void read_class();
void read_comb_sys();
//void real_event_tree_test();
//void speed_test();
void speed_test_class();

clock_t start = clock();
auto start_sys = chrono::system_clock::now();


int main(int argc, char** argv) {
	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Track.h");
	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Event.h");
//	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Track.cpp");
//	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Event.cpp");
//	AmptConverter converter39("/media/dylan/SSD_Storage/Research/ampt/AuAu_nt150_3mb_39gev/", "/media/dylan/SSD_Storage/Research/Trees_Ampt/39GeV/");
//	converter39.convert_trees();
//	AmptConverter converter11("/media/dylan/SSD_Storage/Research/ampt/AuAu_nt150_3mb_11gev/", "/media/dylan/SSD_Storage/Research/Trees_Ampt/11GeV/");
//	converter11.convert_trees();
	read_class();
//	real_event_tree_test();
//	speed_test();
//	speed_test_class();

	cout << endl << "donzo" << endl;
	return(0);

}


void read_class() {
	int ref = 2;
//	string in_path = "/media/dylan/SSD_Storage/Research/Trees_Old_Ref" + to_string(ref) + "/";
//	string out_dir = "/media/dylan/SSD_Storage/Research/Data_Old_Ref" + to_string(ref) + "/";  // "/media/dylan/SSD_Storage/Research/Data_Ref3/"; //"/home/dylan/Research/Data/";
////	string mix_sets_out_dir = "/home/dylan/Research/Data_Mix_Sets/";
//	string mix_out_dir = "/media/dylan/SSD_Storage/Research/Data_Old_Ref" + to_string(ref) + "_Mix/";  // "/media/dylan/SSD_Storage/Research/Data_Ref3_Mix/";
//	string random_out_dir = "/home/dylan/Research/Data_Random/";

	string in_path = "/media/dylan/SSD_Storage/Research/Trees_Old_Ref2/";
	string out_dir = "/media/dylan/SSD_Storage/Research/Data_Sim/";
	string mix_out_dir = "/media/dylan/SSD_Storage/Research/Data_Sim_Mix/";

//	map<string, pair<int, int>> set_pairs = {{"No_Rotate",{0,4}}, {"Rand_Rotate",{0,4}}, {"EP_Rotate",{0,4}}, {"Pile_Up_01_",{0,4}}, {"Pile_Up_008_",{0,4}}, {"Pile_Up_005_",{0,4}}, {"Pile_Up_002_",{0,4}}, {"Pile_Up_001_",{0,4}}, {"Efficiency_08_",{0,4}}, {"Efficiency_05_",{0,4}}, {"Efficiency_025_",{0,4}}, {"Efficiency_01_",{0,4}}};
//	map<string, pair<int, int>> set_pairs = {{"Rand_Rotate",{0,4}}, {"Pile_Up_01_",{0,4}}, {"Pile_Up_008_",{0,4}}, {"Pile_Up_005_",{0,4}}, {"Pile_Up_002_",{0,4}}, {"Pile_Up_001_",{3,4}}};
//	map<string, pair<int, int>> set_pairs = {{"No_BTof_Rej",{0,8}}, {"Pile_Up_0008_",{0,8}}, {"Pile_Up_0005_",{0,8}}, {"Pile_Up_0002_",{0,8}}};
	map<string, pair<int, int>> set_pairs = {{"Sim_0p0s",{0,0}}, {"Sim_05p002s",{0,0}}, {"Sim_05p002s_No_Rotate",{0,0}}, {"Sim_15p002s",{0,0}}};
	int sleep = 60;

	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(pair<string, pair<int, int>> set_pair:set_pairs) {
			for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
				string set_dir = set_pair.first + to_string(set_num) + "/";
				cout << endl << "Queueing " + set_dir <<  "  set_num: " << set_num << endl << endl;
				vector<int> energy_list = {7, 39, 27, 62, 19, 11};
				vector<int> divs = {2, 3, 4, 5, 6};
				for(int energy:energy_list) {
					TreeReader reader(energy, ref);
					reader.set_divs(divs);
					reader.set_cbwc(false);
					reader.set_cent_binning(9);

					reader.set_in_path(in_path);
					reader.set_out_path(out_dir+set_dir);
					reader.set_qa_path(out_dir+set_dir+to_string(energy)+"GeV/");
					reader.set_qa_name("QA_");
					reader.set_set_name(set_pair.first + to_string(set_num));
					reader.set_tree_name("nsmTree");

					if(set_pair.first == "EP_Rotate")  { reader.set_event_plane(true); }
					else { reader.set_event_plane(false); }

					if(set_pair.first == "No_Rotate" || set_pair.first == "No_Rotate_Single"  || set_pair.first == "EP_Rotate" || set_pair.first == "Sim_05p002s_No_Rotate") { reader.set_rotate_random(false); }
					else{ reader.set_rotate_random(true); }

					if(set_pair.first == "Pile_Up_01_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.01); }
					else if(set_pair.first == "Pile_Up_008_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.008); }
					else if(set_pair.first == "Pile_Up_005_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.005); }
					else if(set_pair.first == "Pile_Up_002_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.002); }
					else if(set_pair.first == "Pile_Up_001_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.001); }
					else if(set_pair.first == "Pile_Up_0008_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.0008); }
					else if(set_pair.first == "Pile_Up_0005_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.0005); }
					else if(set_pair.first == "Pile_Up_0002_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.0002); }
					else { reader.set_pile_up(false); reader.set_pile_up_prob(0); }

					if(set_pair.first == "No_BTof_Rej") {
						reader.cut.max_slope = {{7, 999}, {11, 999}, {14, 999}, {19, 999}, {27, 999}, {39, 999}, {62, 999}, {200, 999}};
						reader.cut.min_slope = {{7, -999}, {11, -999}, {14, -999}, {19, -999}, {27, -999}, {39, -999}, {62, -999}, {200, -999}};
					}

//					if(set_pair.first == "Single_Ratio" || set_pair.first == "No_Rotate_Single" ) { reader.set_single_ratio(true); reader.mix.set_single_ratio(true); }
					reader.set_single_ratio(true); reader.mix.set_single_ratio(true);

					if(set_pair.first == "Efficiency_08_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.08); }
					else if(set_pair.first == "Efficiency_05_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.05); }
					else if(set_pair.first == "Efficiency_025_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.025); }
					else if(set_pair.first == "Efficiency_01_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.01); }
					else { reader.set_efficiency(false); reader.set_efficiency_prob(0); }

					if(set_pair.first == "Sim_0p0s") { reader.sim.set_p_group(0.0); reader.sim.set_spread_sigma(0.0); }
					else if(set_pair.first == "Sim_05p002s" || set_pair.first == "Sim_05p002s_No_Rotate") { reader.sim.set_p_group(0.05); reader.sim.set_spread_sigma(0.002); }
					else if(set_pair.first == "Sim_15p002s") { reader.sim.set_p_group(0.15); reader.sim.set_spread_sigma(0.002); }

					reader.set_mixed_sets(false);
					reader.set_rand_data(false);

					reader.set_mixed(true);
					reader.mix.set_divs(divs);
					reader.mix.set_out_path(mix_out_dir+set_dir);
					reader.mix.set_max_events(250);
					reader.mix.set_min_events(150);
					if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
					else { reader.mix.set_mixes_per_event(10); }
					pool.enqueue(&TreeReader::sim_events, reader, 100000000, 8);
//					pool.enqueue(&TreeReader::read_trees, reader);
				}
				this_thread::sleep_for(chrono::seconds(sleep));
			}
		}
	}
}


void speed_test_class() {
	string out_dir = "/media/dylan/SSD_Storage/Research/Data_Ref2/";  // "/media/dylan/SSD_Storage/Research/Data_Ref3/"; //"/home/dylan/Research/Data/";
	int energy = 11;
	int ref = 2;

	ROOT::EnableThreadSafety();

	TreeReader reader(energy, ref);
	reader.set_in_path("/media/dylan/SSD_Storage/Research/Trees_Ref2/");
	reader.set_out_path(out_dir+"Single_Ratio0/");
	reader.read_trees();

////	map<string, pair<int, int>> set_pairs = {{"No_Rotate",{0,4}}, {"Rand_Rotate",{0,4}}, {"EP_Rotate",{0,4}}, {"Pile_Up_01_",{0,4}}, {"Pile_Up_008_",{0,4}}, {"Pile_Up_005_",{0,4}}, {"Pile_Up_002_",{0,4}}, {"Pile_Up_001_",{0,4}}, {"Efficiency_08_",{0,4}}, {"Efficiency_05_",{0,4}}, {"Efficiency_025_",{0,4}}, {"Efficiency_01_",{0,4}}};
////	map<string, pair<int, int>> set_pairs = {{"Rand_Rotate",{0,4}}, {"Pile_Up_01_",{0,4}}, {"Pile_Up_008_",{0,4}}, {"Pile_Up_005_",{0,4}}, {"Pile_Up_002_",{0,4}}, {"Pile_Up_001_",{3,4}}};
////	map<string, pair<int, int>> set_pairs = {{"No_BTof_Rej",{0,8}}, {"Pile_Up_0008_",{0,8}}, {"Pile_Up_0005_",{0,8}}, {"Pile_Up_0002_",{0,8}}};
//	map<string, pair<int, int>> set_pairs = {{"Single_Ratio",{0,0}}};
//	int sleep = 60;
//
//	ROOT::EnableThreadSafety();
//	{
//		ThreadPool pool(1); //thread::hardware_concurrency()
//		for(pair<string, pair<int, int>> set_pair:set_pairs) {
//			for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
//				string set_dir = set_pair.first + to_string(set_num) + "/";
//				cout << endl << "Queueing " + set_dir <<  "  set_num: " << set_num << endl << endl;
//				vector<int> energy_list = {11, 7}; //{7, 39, 27, 62, 19, 11};
//				vector<int> divs = {2, 3, 4, 5, 6};
//				for(int energy:energy_list) {
//					TreeReader reader(energy, 2);
//					reader.set_divs(divs);
//					reader.set_cbwc(false);
//					reader.set_cent_binning(9);
////					reader.set_ref_num(2);
//
//					reader.set_in_path("/media/dylan/SSD_Storage/Research/Trees_Ref2/");
//					reader.set_out_path(out_dir+set_dir);
//					reader.set_qa_path(out_dir+set_dir+to_string(energy)+"GeV/");
//					reader.set_qa_name("QA_");
//					reader.set_set_name(set_pair.first + to_string(set_num));
//
//					if(set_pair.first == "EP_Rotate")  { reader.set_event_plane(true); }
//					else { reader.set_event_plane(false); }
//
//					if(set_pair.first == "No_Rotate" || set_pair.first == "EP_Rotate") { reader.set_rotate_random(false); }
//					else{ reader.set_rotate_random(true); }
//
//					if(set_pair.first == "Pile_Up_01_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.01); }
//					else if(set_pair.first == "Pile_Up_008_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.008); }
//					else if(set_pair.first == "Pile_Up_005_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.005); }
//					else if(set_pair.first == "Pile_Up_002_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.002); }
//					else if(set_pair.first == "Pile_Up_001_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.001); }
//					else if(set_pair.first == "Pile_Up_0008_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.0008); }
//					else if(set_pair.first == "Pile_Up_0005_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.0005); }
//					else if(set_pair.first == "Pile_Up_0002_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.0002); }
//					else { reader.set_pile_up(false); reader.set_pile_up_prob(0); }
//
//					if(set_pair.first == "No_BTof_Rej") {
//						reader.cut.max_slope = {{7, 999}, {11, 999}, {14, 999}, {19, 999}, {27, 999}, {39, 999}, {62, 999}, {200, 999}};
//						reader.cut.min_slope = {{7, -999}, {11, -999}, {14, -999}, {19, -999}, {27, -999}, {39, -999}, {62, -999}, {200, -999}};
//
//					}
//
//					if(set_pair.first == "Single_Ratio") { reader.set_single_ratio(true); reader.mix.set_single_ratio(true); }
//
//					if(set_pair.first == "Efficiency_08_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.08); }
//					else if(set_pair.first == "Efficiency_05_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.05); }
//					else if(set_pair.first == "Efficiency_025_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.025); }
//					else if(set_pair.first == "Efficiency_01_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.01); }
//					else { reader.set_efficiency(false); reader.set_efficiency_prob(0); }
//
//					reader.set_mixed_sets(false);
//					reader.set_rand_data(false);
//
//					reader.set_mixed(false);
//					reader.mix.set_divs(divs);
//					reader.mix.set_out_path(mix_out_dir+set_dir);
//					reader.mix.set_max_events(250);
//					reader.mix.set_min_events(150);
//					if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
//					else { reader.mix.set_mixes_per_event(10); }
//					reader.read_trees();
////					pool.enqueue(&TreeReader::read_trees, reader);
//				}
//				this_thread::sleep_for(chrono::seconds(sleep));
//			}
//		}
//	}
}


void read_comb_sys() {
	string out_dir = "/home/dylan/Research/Data/";
	string mix_sets_out_dir = "/home/dylan/Research/Data_Mix_Sets/";
	string mix_out_dir = "/home/dylan/Research/Data_Mix/";
	string random_out_dir = "/home/dylan/Research/Data_Random/";

	map<string, pair<int, int>> set_pairs = {{"Final_Set",{0,100}}};
	pair<float, float> pile_up_range = {0.0, 0.01};
	pair<float, float> efficiency_range = {0.0, 0.05};
	TRandom3 sys_rand(0);
	int sleep = 60;

	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(pair<string, pair<int, int>> set_pair:set_pairs) {
			for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
				string set_dir = set_pair.first + to_string(set_num) + "/";
				cout << endl << "Queueing " + set_dir <<  "  set_num: " << set_num << endl << endl;
				vector<int> energy_list = {39, 27, 62, 19, 11, 7};
				vector<int> divs = {2, 3, 4, 5, 6};
				for(int energy:energy_list) {
					TreeReader reader(energy);
					reader.set_divs(divs);
					reader.set_cbwc(false);
					reader.set_cent_binning(9);

					reader.set_in_path("/home/dylan/Research/Trees/");
					reader.set_out_path(out_dir+set_dir);
					reader.set_qa_path(out_dir+set_dir+to_string(energy)+"GeV/");
					reader.set_qa_name("QA_");
					reader.set_set_name(set_pair.first + to_string(set_num));

					reader.set_event_plane(false);
					reader.set_rotate_random(true);

					reader.set_pile_up(true);
					reader.set_pile_up_prob(sys_rand.Rndm() * (pile_up_range.second - pile_up_range.first) + pile_up_range.second);

					reader.set_efficiency(true);
					reader.set_efficiency_prob(sys_rand.Rndm() * (efficiency_range.second - efficiency_range.first) + efficiency_range.second);

					reader.set_mixed_sets(false);
					reader.set_rand_data(false);

					reader.set_mixed(true);
					reader.mix.set_divs(divs);
					reader.mix.set_out_path(mix_out_dir+set_dir);
					reader.mix.set_max_events(250);
					reader.mix.set_min_events(150);
					if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
					else { reader.mix.set_mixes_per_event(10); }
					pool.enqueue(&TreeReader::read_trees, reader);
				}
				this_thread::sleep_for(chrono::seconds(sleep));
			}
		}
	}
}
