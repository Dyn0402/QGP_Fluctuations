/*
 * config.cpp
 *
 *  Created on: Mar 14, 2019
 *      Author: dylan
 */



#include <string>

#include <TFile.h>
#include <TRandom3.h>
#include <TH1D.h>

using namespace std;


namespace config {
//	string out_path = "/home/dylan/Dropbox/Research/Coalescence/v1_sim.root";
	string out_path = "/home/dylan/local_server/dyn0402/Research/Simulation/v1_sim.root";

	int n_events = 1000000;
	int particle_mean = 25;
	int divisions = 1;

	double p_effect = 0.1;
	double p_cluster = 0.1;

	double spread_sigma = 0.2;

	TRandom3 *rand = new TRandom3(0);

	TH1D *R_hist = new TH1D("Ratios", "Ratio Distribution", 20, 0.0, 1.1);
	TH1D *corr_hist = new TH1D("Two_Particle_Correlations", "Two Particle Correlations", 1000, -M_PI/2, 2*M_PI-M_PI/2);
}

