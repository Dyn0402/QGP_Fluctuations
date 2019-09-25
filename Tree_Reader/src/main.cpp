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

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>
#include <TROOT.h>
#include <TCanvas.h>

#include "tree_reader.h"
#include "TreeReader.h"
#include "ratio_methods.h"
#include "file_io.h"

#include "../StRoot/StRefMultCorr/CentralityMaker.h"
#include "../StRoot/StRefMultCorr/StRefMultCorr.h"
#include "config_reader.h"

using namespace std;


void read_class();
void read_files(int argc, char** argv);
void read_files_local();
void read_energy(int energy);
void io_test();
void sum_tree_data_test();
void ref_mult_test();
void vec_tree_test();
void obj_tree_test();
void real_tree_test();

clock_t start = clock();
auto start_sys = chrono::system_clock::now();


int main(int argc, char** argv) {
//	config::centrals = {atoi(argv[1])};
//	string cent(argv[1]);
//	config::out_root_name = "cent_" + cent + ".root";
//	read_files(argc, argv);
//	read_files_local();
//	ref_mult_test();
	read_class();
//	vec_tree_test();
//	obj_tree_test();
//	real_tree_test();

	cout << endl << "donzo" << endl;
	return(0);

}


void read_class() {
	TreeReader reader;
	reader.set_cbwc(false);
	reader.set_in_path("/home/dylan/Research/Trees/");
	reader.set_out_path("/home/dylan/local_server/dyn0402/Research/Data/");
	reader.set_qa_name("QA_");
	reader.set_divs({2,3,4,5,6});
	reader.set_energy_list({27,19,7,11});
	reader.read_trees();
}


void read_files(int argc, char** argv) {
	string in_list = argv[1];
	string job_id = argv[2];
	int energy = stoi(argv[3]);

	ifstream list_file(in_list);
//	cout << in_list << " " << job_id << endl;
	if(list_file.is_open()) {
		vector<string> in_files;
		string line;
		while(getline(list_file, line)) {
			in_files.push_back(line);
		}
		list_file.close();

		tree_data data;
		for(string path:in_files) {
//			cout << path << endl;
			TFile *file = new TFile(path.data(), "READ");
			TTree *tree = (TTree*)file->Get(pars::tree_name.data());
//			data = read_tree(tree, data, energy);
			file->Close();
			delete file;
		}
		write_tree_data(job_id, data);
	} else {
		cout << "Couldn't open the list_file. \n";
	}
}


// Read files locally via paths from config rather than input file lists from job submission.
void read_files_local() {
	ROOT::EnableThreadSafety();
	vector<thread> threads;
	for(int energy:local::energy_list) {
		threads.push_back(thread(read_energy, energy));
//		usleep(20000000);
	}
	for(thread & th : threads) {
		if(th.joinable()) {
			th.join();
		}
	}
}


// Read files for single energy and write results to text files.
void read_energy(int energy) {
	cout << "Reading " + to_string(energy) + "GeV trees." << endl;
	vector<string> in_files = get_files_in_dir(local::in_path+to_string(energy)+"GeV/", "root", "path");
	unsigned num_files = in_files.size();
	unsigned file_index = 1;
	tree_data data;
	auto *cent_hist = new TH2I(("cent_comp"+to_string(energy)).data(), "Centrality Comparison", 19, -2.5, 16.5, 19, -2.5, 16.5);
	StRefMultCorr *refmult2CorrUtil = new StRefMultCorr("refmult2");
	for(string path:in_files) {
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) {
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			cout << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s" << endl;
		}
		TFile *file = new TFile(path.data(), "READ");
		TTree *tree = (TTree*)file->Get(pars::tree_name.data());
		read_tree(tree, &data, energy, refmult2CorrUtil, cent_hist);
		file->Close();
		delete file;
		file_index++;
	}
	TFile *qa = new TFile((local::qa_path + local::qa_name + to_string(energy) + "GeV.root").data(), "RECREATE");
	TCanvas *can = new TCanvas();
	cent_hist->Draw("COLZ");
	can->Write();
	cent_hist->Write();
	delete cent_hist;
	delete can;
	qa->Close();
	delete qa;
	cout << " Writing " + to_string(energy) + "GeV trees." << endl;
	write_tree_data("local", data, local::out_path+to_string(energy)+"GeV/");
	cout << endl;
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
		double reweight = refmult2CorrUtil->getWeight();
		double refmult2Cor = refmult2CorrUtil->getRefMultCorr();
		cout << cent16 << endl;
	}
}

