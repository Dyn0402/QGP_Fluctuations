/*
 * main.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <vector>
#include <map>

#include <TFile.h>
#include <TTree.h>

#include "config.h"
#include "tree_reader.h"
#include "plotting.h"

using namespace std;


int main() {
	TFile *out_root = new TFile((config::out_path+config::out_root_name).data(), "RECREATE");

	map<int, tree_data> trees;

	int tree_num = 1;
	int num_trees = config::energy_list.size();
	for(int energy:config::energy_list) {
		string file_path = config::in_path + config::in_file_prefx + to_string(energy) + config::in_file_sufx;
		TFile *file = new TFile(file_path.data(), "READ");
		TTree *tree = (TTree *)file->Get(config::tree_name.data());
		cout << endl << "Reading " << energy << "GeV tree" << endl;
		cout << "Tree " + to_string(tree_num) + " of " + to_string(num_trees) << endl << endl;
		trees[energy] = read_tree(tree, energy);
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


