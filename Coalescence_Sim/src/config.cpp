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
	string out_path = "/home/dylan/local_server/dyn0402/Research/Simulation/07-17_0.5pclust.root";
//	string out_path = "/home/dylan/Research/Roli_Sim_Data/comp_test_dylan.root";

	vector<double> p_effect_list = {0.0, 0.05, 0.10, 0.15, 0.20, 0.25};
	vector<int> cumulant_orders = {1, 2, 3, 4};
	vector<string> statistics = {"mean", "standard_deviation", "skewness", "kurtosis"};
}

