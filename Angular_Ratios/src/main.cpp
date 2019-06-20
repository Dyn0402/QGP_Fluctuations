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


int main() {
	TFile *out_root = new TFile((config::out_path+config::out_root_name).data(), "RECREATE");

	vector<tree_data> trees;

	int tree_num = 1;
	int num_trees = config::file_list.size();
	for(string file_name:config::file_list) {
		TFile *file = new TFile((config::in_path+file_name+".root").data(), "READ");
		TTree *tree = (TTree *)file->Get(config::tree_name.data());
		cout << endl << "Reading " + file_name  << endl;
		cout << "Tree " + to_string(tree_num) + " of " + to_string(num_trees) << endl << endl;
		trees.push_back(read_tree(tree));
		//calculate_cumulants
		file->Close();
		delete file;
		tree_num++;
	}

	out_root->cd();
	cout << endl << "Making ratio distributions: " << endl;
	make_ratio_dist_plots(trees);
	cout << endl << endl << "Makeing proton number distributions: " << endl;
	make_proton_dist_plots(trees);
	//plot_cumulants


	out_root->Close();

	cout << endl << "donzo" << endl;
	return(0);
}


