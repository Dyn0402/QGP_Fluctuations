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
void analyze();


int main() {
	TFile *out_root = new TFile((config::out_path+config::out_root_name).data(), "UPDATE");
	vector<vector<vector<double>>> ratios = get_ratios();

	out_root->cd();
	make_dist_plots(ratios);
//	analyze(ratios);

	out_root->Close();

	cout << "donzo" << endl;
	return(0);
}


vector<vector<vector<double>>> get_ratios() {
	vector<vector<vector<double>>> ratios;
	for(string file_name:config::file_list) {
		vector<vector<double>> tree_ratio;
		TFile *file = new TFile((config::in_path+file_name+".root").data(), "READ");
		TTree *tree = (TTree *)file->Get(config::tree_name.data());
		for(int div:config::divs) {
			tree_ratio.push_back(get_tree_ratios(tree, div));
		}
		ratios.push_back(tree_ratio);
		file->Close();
		delete file;
	}

	return(ratios);
}
