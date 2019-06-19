/*
 * config.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */


#include <string>
#include <vector>

using namespace std;


namespace config {
	string out_path = "/home/dylan/local_server/home/dyn0402/Research/Test_Data/";//"/star/u/dneff/Documents/Roli_Thesis/";
	string in_path = "/home/dylan/local_server/home/dyn0402/Research/Test_Data/";//"/gpfs01/star/pwg/dneff/BES1_Trees/dylan_trees/";
	string out_root_name = "test.root";


	vector<string> file_list = {"tree_auau11"};
	vector<int> divs = {3,4};

	string tree_name = "nsmTree";

	int ratio_hist_bins = 23;
	double ratio_hist_low = -0.05;
	double ratio_hist_high = 1.1;

	int protons_hist_bins = 51;
	double protons_hist_low = -1;
	double protons_hist_high = 50;
}
