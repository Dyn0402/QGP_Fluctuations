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

#include "config_tr.h"
#include "plotting.h"
#include "file_io.h"

using namespace std;


void read_files(int argc, char** argv);
void analysis();
void io_test();
void cumulant_test();
map<int, map<int, map<int, map<int, double>>>> calculate_cumulants(map<int, tree_data> trees); //cumulants[energy][divisions][centrality][cumulant_order]


int main(int argc, char** argv) {
//	cumulant_test();
//	config::centrals = {atoi(argv[1])};
//	string cent(argv[1]);
//	config::out_root_name = "cent_" + cent + ".root";
//	analysis();
//	analysis_mem_check();
	read_files(argc, argv);

	cout << endl << "donzo" << endl;
	return(0);

}


void read_files(int argc, char** argv) {
	string in_list = argv[1];
	string job_id = argv[2];
	int energy = stoi(argv[3]);

	ifstream list_file(in_list);
	cout << in_list << " " << job_id << endl;
	if(list_file.is_open()) {
		vector<string> read_files;
		string line;
		while(getline(list_file, line)) {
			read_files.push_back(line);
		}
		list_file.close();

		int i = 0;
		tree_data data;
		for(string path:read_files) {
			cout << path << endl;
			if(i <= 100) {
				TFile *file = new TFile(path.data(), "READ");
				TTree *tree = (TTree*)file->Get(config::tree_name.data());
				data = read_tree(tree, data, energy);
				file->Close();
				delete file;
			}
			i++;
		}
		write_tree_data(job_id, data);
	} else {
		cout << "Couldn't open the list_file. \n";
	}
}


void cumulant_test() {
	int n = 4;
	vector<double> test_data = {-1,0,1,2,3,4,5,6,7,8,9,10,10};
	cout << get_raw_moment(test_data, n) << endl;
	cout << get_central_moment(test_data, n) << endl;
	cout << get_cumulant(test_data, n) << endl;
}


void analysis() {
	TFile *out_root = new TFile((config::out_path+config::out_root_name).data(), "RECREATE");

	map<int, tree_data> trees;

	int tree_num = 1;
	int num_trees = config::energy_list.size();
	for(int energy:config::energy_list) {
		string file_path = config::in_path + config::in_file_prefx + to_string(energy) + config::in_file_sufx;
		TFile *file = new TFile(file_path.data(), "READ");
		TTree *tree = (TTree *)file->Get(config::tree_name.data());
		out_root->cd();
		cout << endl << "Reading " << energy << "GeV tree" << endl;
		cout << "Tree " + to_string(tree_num) + " of " + to_string(num_trees) << endl << endl;
//		trees[energy] = read_tree(tree, energy);
		cout << energy << "GeV tree complete." << endl << endl;
		file->Close();
		delete file;
		tree_num++;
	}

	cout << endl << "Calculating Cumulants..." << endl;
	auto cumulants = calculate_cumulants(trees);

	out_root->cd();
	cout << endl << "Making ratio distribution plots..." << endl;
	make_ratio_dist_plots(out_root, trees);
	cout << endl << "Making proton distribution plots..." << endl;
	make_proton_dist_plots(out_root, trees);
	cout << endl << "Making cumulant plots..." << endl;
	make_cumulant_plots(out_root, cumulants);
	cout << endl << "Making canvases..." << endl;
	make_canvas_plots(out_root, trees);


	out_root->Close();
}



//Calculate cumulants for each cumulant_order for each centrality for each number of divisions for each energy.
map<int, map<int, map<int, map<int, double>>>> calculate_cumulants(map<int, tree_data> trees) {
	map<int, map<int, map<int, map<int, double>>>> cumulants;
	for(int energy:config::energy_list) {
		cout << "Calculating " << to_string(energy) << "GeV cumulants" << endl;
		for(int div:config::divs) {
			cout << "Calculating " << to_string(div) << " division cumulants" << endl;
			for(int cent:config::centrals) {
				for(int order:config::cumulant_orders) {
//					cumulants[energy][div][cent][order] = get_cumulant(trees[energy].ratios[div][cent], order);
//					cout << " mean: " << get_raw_moment(trees[energy].ratios[div][cent], 1) <<  " len: " << trees[energy].ratios[div][cent].size() << " divs: " << div << " cent: " << cent << " order: " << order << " cumulant: " <<  cumulants[energy][div][cent][order] << endl;
				}
			}
		}
	}

	return(cumulants);
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
