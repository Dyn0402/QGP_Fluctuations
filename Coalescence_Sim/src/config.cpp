/*
 * config.cpp
 *
 *  Created on: Mar 14, 2019
 *      Author: dylan
 */


#include <vector>
#include <string>
#include <map>

#include <TFile.h>
#include <TRandom3.h>
#include <TH1D.h>

using namespace std;


namespace config {
//	string out_path = "/home/dylan/Dropbox/Research/Coalescence/v1_sim.root";
	string out_path = "/home/dylan/local_server/dyn0402/Research/Simulation/07-12_compare_12thread.root";
//	string out_path = "/home/dylan/Research/Roli_Sim_Data/comp_test_dylan.root";

	vector<int> cumulant_orders = {1, 2, 3, 4};
	map<int, string> cumulant_name = {{1, "Mean"}, {2, "Sigma"}, {3, "Skewness"}, {4, "Kurtosis"}};
}

