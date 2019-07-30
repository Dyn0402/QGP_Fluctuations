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
	string out_path = "/home/dylan/local_server/dyn0402/Research/Simulation/07-18_athic_comp.root";
//	string out_path = "/home/dylan/Research/Roli_Sim_Data/comp_test_dylan.root";

	string param = "p_cluster";
	vector<double> param_list = {0.0, 0.05, 0.10, 0.15, 0.20};
	vector<int> cumulant_orders = {1, 2, 3, 4};
	vector<string> moments = {"mean", "standard_deviation", "skewness", "kurtosis"};

	double can_w = 1080;
	double can_h = 1080;
}

