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
	string out_path = "/home/dylan/local_server/dyn0402/Research/Simulation/cumulants_unbinned.root";

	int n_events = 1e5;
	int particle_mean = 10;
	int divisions = 3;

	double p_effect = 0.1;
	double p_cluster = 0.1;

	double spread_sigma = 0.0;

	TRandom3 *rand = new TRandom3(0);

	vector<int> cumulant_orders = {1, 2, 3, 4};
	map<int, string> cumulant_name = {{1, "Mean"}, {2, "Sigma"}, {3, "Skewness"}, {4, "Kurtosis"}};

	TH1D *R_hist = new TH1D("Ratios", "Ratio Distribution", 20, 0.0, 1.1);
	TH1D *corr_hist = new TH1D("Two_Particle_Correlations", "Two Particle Correlations", 1000, -M_PI/2, 2*M_PI-M_PI/2);
}

