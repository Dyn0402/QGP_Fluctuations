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

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>

#include "tree_reader.h"
#include "ratio_methods.h"

#include "file_io.h"
#include "config_reader.h"

using namespace std;


void read_files(int argc, char** argv);
void read_files_local();
void io_test();


int main(int argc, char** argv) {
//	config::centrals = {atoi(argv[1])};
//	string cent(argv[1]);
//	config::out_root_name = "cent_" + cent + ".root";
//	read_files(argc, argv);
	read_files_local();

	cout << endl << "donzo" << endl;
	return(0);

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
			data = read_tree(tree, data, energy);
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
	for(int energy:local::energy_list) {
		cout << "Reading " + to_string(energy) + "GeV trees." << endl;
		vector<string> in_files = get_files_in_dir(local::in_path+to_string(energy)+"GeV/", "root", "path");
		unsigned num_files = in_files.size();
		unsigned file_index = 1;
		tree_data data;
		for(string path:in_files) {
			if(!(file_index % (num_files/10))) {
				cout << "  " << (int)(100.0*file_index/num_files+0.5) << "% complete" << endl;
			}
			TFile *file = new TFile(path.data(), "READ");
			TTree *tree = (TTree*)file->Get(pars::tree_name.data());
			data = read_tree(tree, data, energy);
			file->Close();
			delete file;
			file_index++;
		}
		cout << " Writing " + to_string(energy) + "GeV trees." << endl;
		write_tree_data("local", data, local::out_path+to_string(energy)+"GeV/");
		cout << endl;
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
