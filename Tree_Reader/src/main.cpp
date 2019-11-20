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
void io_test();
void sum_tree_data_test();
void ref_mult_test();
//void vec_tree_test();
//void obj_tree_test();
//void real_tree_test();

clock_t start = clock();
auto start_sys = chrono::system_clock::now();


int main(int argc, char** argv) {
	read_class();
//	vec_tree_test();
//	obj_tree_test();
//	real_tree_test();

	cout << endl << "donzo" << endl;
	return(0);

}


void read_class() {
//	string out_dir = "/home/dylan/local_server/dyn0402/Research/Data/";
//	string mix_out_dir = "/home/dylan/local_server/dyn0402/Research/Data_Mix/";
//	string mix_roli_out_dir = "/home/dylan/local_server/dyn0402/Research/Data_Mix_Roli/";
//	string random_out_dir = "/home/dylan/local_server/dyn0402/Research/Data_Random/";
	string out_dir = "/home/dylan/Research/Data/";
	string mix_out_dir = "/home/dylan/Research/Data_Mix/";
	string mix_roli_out_dir = "/home/dylan/Research/Data_Mix_Roli/";
	string random_out_dir = "/home/dylan/Research/Data_Random/";
	vector<string> set_dirs;
	for(int i = 0; i < 10; i++) {
		set_dirs.push_back("Set" + to_string(i) + "/");
		if(system(("test -d " + out_dir + set_dirs.back()).data())) { system(("mkdir " + out_dir + set_dirs.back()).data()); }
		if(system(("test -d " + mix_out_dir + set_dirs.back()).data())) { system(("mkdir " + mix_out_dir + set_dirs.back()).data()); }
		if(system(("test -d " + mix_roli_out_dir + set_dirs.back()).data())) { system(("mkdir " + mix_roli_out_dir + set_dirs.back()).data()); }
		if(system(("test -d " + random_out_dir + set_dirs.back()).data())) { system(("mkdir " + random_out_dir + set_dirs.back()).data()); }
	}
	for(string set:set_dirs) {
		cout << endl << endl << "Starting " + set << endl << endl;
		vector<int> energy_list = {27, 39, 62, 19, 11, 7};
		vector<int> divs = {2, 3, 4, 5, 6};
		ROOT::EnableThreadSafety();
		vector<thread> threads;
		for(int energy:energy_list) {
			TreeReader reader(energy);
			reader.set_cbwc(false);
			reader.set_rotate_random(false);
			reader.set_in_path("/home/dylan/Research/Trees/");
			reader.set_out_path(out_dir+set);
			reader.set_qa_path(out_dir+set+to_string(energy)+"GeV/");
			reader.set_qa_name("QA_");
			reader.set_divs(divs);
			reader.set_event_plane(false);
			reader.set_mixed(false);
//			reader.mix.set_divs(divs);
//			reader.mix.set_out_path(mix_out_dir+set+to_string(energy)+"GeV/");
//			reader.mix.set_max_events(250);
//			reader.mix.set_use_leftover(true);
			reader.set_mixed_roli(true);
			reader.mix_roli.set_divs(divs);
			reader.mix_roli.set_out_path(mix_roli_out_dir+set+to_string(energy)+"GeV/");
			reader.mix_roli.set_max_events(250);
			reader.mix_roli.set_mixes_per_event(10);
			reader.set_rand_data(true);
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


void io_test() {
	string testc = "hello_thie\tresse_reand\t11_.txt";
	vector<string> split = split_string_by_char(testc, '\t');
	for(string str:split) { cout << str << endl; }

	map<int, map<int, map<int, map<int, int>>>> test;
	test[2][15][0][0] = 152;
	test[2][15][0][1] = 456;
	test[2][15][0][2] = 1452;
	test[1][15][1][3] = 343;
	test[2][15][1][11] = 1;

	write_ratios("Dafd4009", test);
	write_ratios("Cafd4009", test);
	write_ratios("Bafd4009", test);
	map<int, map<int, int>> test_in = read_ratios("/home/dylan/git/Research/QGP_Fluctuations/Angular_Ratios/", 2, 15);
	for(pair<int, map<int, int>> entry:test_in) {
		cout << entry.first << "\t";
		for(pair<int, int> entrya:entry.second) {
			cout << entrya.first << ":" << entrya.second << " ";
		}
		cout << endl;
	}

	map<int, map<int, int>> testb;
	testb[15][0] = 156;
	testb[15][1] = 1584;
	testb[15][2] = 657;


	write_nprotons("Dafd4009", testb);
	map<int, int> test_inb = read_nprotons("/home/dylan/git/Research/QGP_Fluctuations/Angular_Ratios/", 15);
	for(pair<int, int> entry:test_inb) {
		cout << entry.first << "\t" << entry.second << endl;
	}
}



void sum_tree_data_test() {
	tree_data data;
	data.good_protons[0][10] += 10;
	data.ratios[6][0][10][2] += 14;
	tree_data new_data;
	new_data.good_protons[0][10] += 5;
	new_data.ratios[6][0][10][2] += 3;
	sum_tree_data(&data, new_data);

	cout << data.good_protons[0][10] << endl;
	cout << data.ratios[6][0][10][2] << endl;

}


void ref_mult_test() {
	StRefMultCorr *refmult2CorrUtil = CentralityMaker::instance()->getRefMult2Corr();
	//StRefMultCorr *refmult2CorrUtil = new StRefMultCorr("refmult2");
	refmult2CorrUtil->init(11078000);
	if(!refmult2CorrUtil->isBadRun(11078000)) {
		int refmult2 = 100;
		double vz = 20.0;
		double bbc = 20000.0;
		refmult2CorrUtil->initEvent(refmult2, vz, bbc);
		int cent16 = refmult2CorrUtil->getCentralityBin16();
//		double reweight = refmult2CorrUtil->getWeight();
//		double refmult2Cor = refmult2CorrUtil->getRefMultCorr();
		cout << cent16 << endl;
	}
}

