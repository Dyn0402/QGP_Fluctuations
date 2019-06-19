/*
 * main.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>

#include "config.h"
#include "tree_reader.h"
#include "plotting.h"

using namespace std;

vector<vector<vector<double>>> get_ratios();
vector<vector<int>> get_nprotons();
//void analyze();


int main() {
	TFile *out_root = new TFile((config::out_path+config::out_root_name).data(), "RECREATE");
	cout << "Getting ratios: " << endl;
	vector<vector<vector<double>>> ratios = get_ratios();
	cout << endl << "Getting number of protons: " << endl;
	vector<vector<int>> nprotons = get_nprotons();

	out_root->cd();
	cout << endl << "Making ratio distributions: " << endl;
	make_ratio_dist_plots(ratios);
	cout << endl << endl << "Makeing proton number distributions: " << endl;
	make_proton_dist_plots(nprotons);
//	analyze(ratios);

	out_root->Close();

	cout << endl << "donzo" << endl;
	return(0);
}


vector<vector<vector<double>>> get_ratios() {
	vector<vector<vector<double>>> ratios;
	int tree_num = 1;
	int num_trees = config::file_list.size() * config::divs.size();
	for(string file_name:config::file_list) {
		vector<vector<double>> tree_ratio;
		TFile *file = new TFile((config::in_path+file_name+".root").data(), "READ");
		TTree *tree = (TTree *)file->Get(config::tree_name.data());
		for(int div:config::divs) {
			cout << endl << "Reading " + file_name + " with " + to_string(div) + " divisions." << endl;
			cout << "Tree " + to_string(tree_num) + " of " + to_string(num_trees) << endl << endl;
			tree_ratio.push_back(get_tree_ratios(tree, div));
		}
		ratios.push_back(tree_ratio);
		file->Close();
		delete file;
		tree_num++;
	}

	return(ratios);
}


vector<vector<int>> get_nprotons() {
	vector<vector<int>> nprotons;
	int tree_num = 1;
	int num_trees = config::file_list.size();
	for(string file_name:config::file_list) {
		TFile *file = new TFile((config::in_path+file_name+".root").data(), "READ");
		TTree *tree = (TTree *)file->Get(config::tree_name.data());
		cout << endl << "Reading " + file_name + " proton numbers." << endl;
		cout << "Tree " + to_string(tree_num) + "/" + to_string(num_trees) << endl;
		nprotons.push_back(get_tree_nprotons(tree));
		tree_num++;
	}

	return(nprotons);

}
