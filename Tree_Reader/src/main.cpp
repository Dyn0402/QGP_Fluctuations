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
//	string out_dir = "/home/dylan/local_server/dyn0402/Research/Data/";
//	string mix_out_dir = "/home/dylan/local_server/dyn0402/Research/Data_Mix/";
//	string mix_roli_out_dir = "/home/dylan/local_server/dyn0402/Research/Data_Mix_Roli/";
//	string random_out_dir = "/home/dylan/local_server/dyn0402/Research/Data_Random/";
	string out_dir = "/home/dylan/Research/Data/";
	string mix_sets_out_dir = "/home/dylan/Research/Data_Mix/";
	string mix_roli_out_dir = "/home/dylan/Research/Data_Mix_Roli/";
	string random_out_dir = "/home/dylan/Research/Data_Random/";
	vector<string> set_dirs;
	for(int set = 0; set <= 5; set++) {
		set_dirs.push_back("Set" + to_string(set) + "/");
		if(system(("test -d " + out_dir + set_dirs.back()).data())) { system(("mkdir " + out_dir + set_dirs.back()).data()); }
		if(system(("test -d " + mix_sets_out_dir + set_dirs.back()).data())) { system(("mkdir " + mix_sets_out_dir + set_dirs.back()).data()); }
		if(system(("test -d " + mix_roli_out_dir + set_dirs.back()).data())) { system(("mkdir " + mix_roli_out_dir + set_dirs.back()).data()); }
		if(system(("test -d " + random_out_dir + set_dirs.back()).data())) { system(("mkdir " + random_out_dir + set_dirs.back()).data()); }
	}
	for(string set:set_dirs) {
		cout << endl << endl << "Starting " + set << endl << endl;
		int set_num = stoi(set.substr(set.find('t')+1, set.find('/')-set.find('t')-1));
		vector<int> energy_list = {27, 39, 62, 19, 11, 7};
		vector<int> divs = {2, 3, 4, 5, 6};
		ROOT::EnableThreadSafety();
		vector<thread> threads;
		for(int energy:energy_list) {
			TreeReader reader(energy);
			reader.set_cbwc(false);
			reader.set_cent_binning(9);
			reader.set_in_path("/home/dylan/Research/Trees/");
			reader.set_out_path(out_dir+set);
			reader.set_qa_path(out_dir+set+to_string(energy)+"GeV/");
			reader.set_qa_name("QA_");
			reader.set_divs(divs);
			if(set_num < 6) { reader.set_event_plane(true); }
			else { reader.set_event_plane(false); }
			if(set_num >= 6 && set_num < 12) { reader.set_rotate_random(true); }
			else { reader.set_rotate_random(false); }
			reader.set_mixed_sets(false);
//			reader.mix.set_divs(divs);
//			reader.mix.set_out_path(mix_out_dir+set+to_string(energy)+"GeV/");
//			reader.mix.set_max_events(250);
//			reader.mix.set_use_leftover(true);

//			reader.set_event_plane(true);
//			reader.mix_rotate = true;

			reader.set_mixed(true);
			reader.mix.set_divs(divs);
			reader.mix.set_out_path(mix_roli_out_dir+set+to_string(energy)+"GeV/");
			reader.mix.set_max_events(250);
			reader.mix.set_min_events(150);
			reader.mix.set_mixes_per_event(10);
			if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
			reader.set_rand_data(false);
			reader.random.set_divs(divs);
			reader.random.set_out_path(random_out_dir+set+to_string(energy)+"GeV/");
			threads.push_back(thread(&TreeReader::read_trees, reader));
		}
		for(thread & th : threads) {
			if(th.joinable()) {
				th.join();
			}
		}
	}
}
