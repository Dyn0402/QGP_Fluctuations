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
#include "ratio_methods.h"
#include "file_io.h"

#include "../StRoot/StRefMultCorr/CentralityMaker.h"
#include "../StRoot/StRefMultCorr/StRefMultCorr.h"

using namespace std;


void read_class();

clock_t start = clock();
auto start_sys = chrono::system_clock::now();


int main(int argc, char** argv) {
	read_class();

	cout << endl << "donzo" << endl;
	return(0);

}


void read_class() {
	string out_dir = "/home/dylan/Research/Data/";
	string mix_sets_out_dir = "/home/dylan/Research/Data_Mix_Sets/";
	string mix_out_dir = "/home/dylan/Research/Data_Mix/";
	string random_out_dir = "/home/dylan/Research/Data_Random/";

	map<string, int> set_pairs = {{"No_Rotate",10}, {"Rand_Rotate",25}, {"EP_Rotate",10}, {"Pile_Up_05_",10}, {"Efficiency_05_",10}};

	int sleep = 60;

	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(pair<string, int> set_pair:set_pairs) {
			for(int set_num = 0; set_num < set_pair.second; set_num++) {
				string set_dir = set_pair.first + to_string(set_num) + "/";
				cout << endl << endl << "Queueing " + set_dir <<  "  set_num: " << set_num << endl << endl;
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

					if(set_pair.first == "EP_Rotate")  { reader.set_event_plane(true); }
					else { reader.set_event_plane(true); }

					if(set_pair.first == "No_Rotate" || set_pair.first == "EP_Rotate") { reader.set_rotate_random(true); }
					else{ reader.set_rotate_random(false); }

					if(set_pair.first == "Pile_Up_05_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.05); }
					else { reader.set_pile_up(false); reader.set_pile_up_prob(0); }

					if(set_pair.first == "Efficiency_05_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.05); }
					else { reader.set_efficiency(false); reader.set_efficiency_prob(0); }

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
			}
			this_thread::sleep_for(chrono::seconds(sleep));
		}
	}
}
