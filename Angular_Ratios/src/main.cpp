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
#include <TDirectory.h>

#include "config.h"
#include "tree_reader.h"
#include "ratio_methods.h"
#include "plotting.h"

using namespace std;


void analysis();
void cumulant_test();
map<int, map<int, map<int, map<int, double>>>> calculate_cumulants(map<int, tree_data> trees); //cumulants[energy][divisions][centrality][cumulant_order]


int main(int argc, char** argv) {
//	cumulant_test();
//	config::centrals = {atoi(argv[1])};
//	string cent(argv[1]);
//	config::out_root_name = "cent_" + cent + ".root";
	analysis();

	cout << endl << "donzo" << endl;
	return(0);

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
		cout << endl << "Reading " << energy << "GeV tree" << endl;
		cout << "Tree " + to_string(tree_num) + " of " + to_string(num_trees) << endl << endl;
		trees[energy] = read_tree(tree, energy);
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
					cumulants[energy][div][cent][order] = get_cumulant(trees[energy].ratios[div][cent], order);
//					cout << " mean: " << get_raw_moment(trees[energy].ratios[div][cent], 1) <<  " len: " << trees[energy].ratios[div][cent].size() << " divs: " << div << " cent: " << cent << " order: " << order << " cumulant: " <<  cumulants[energy][div][cent][order] << endl;
				}
			}
		}
	}

	return(cumulants);
}
