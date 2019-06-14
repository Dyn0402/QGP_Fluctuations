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

using namespace std;


int main() {
	TFile *out_root = new TFile((config::out_path+config::out_root_name).data(), "UPDATE");
	vector<vector<vector<double>>> ratios;
	for(string tree:config::tree_in_list) {
		vector<vector<double>> tree_ratio;
		TFile *file = new TFile((config::in_path+tree+".root").data(), "READ");
		TTree *tree = (TTree *)file->Get(config::tree_name.data());
		for(int div:config::divs) {
			tree_ratio.push_back(get_ratio(tree, div));
		}
		ratios.push_back(tree_ratio);
		file->Close();
		delete file;
	}

//	analyze(ratios);

	cout << "donzo" << endl;
	return(0);
}
