/*
 * config.cpp
 *
 *  Created on: Mar 14, 2019
 *      Author: dylan
 */



#include <string>
#include "TFile.h"
#include "TRandom3.h"
#include "TH1D.h"

using namespace std;


namespace config {
//	string out_path = "/home/dylan/Dropbox/Research/Coalescence/v1_sim.root";
	string out_path = "/home/dylan/Dropbox/Research/Coalescence/v1_sim.root";

	int n_events = 1000;
	int divisions = 4;

	double p_effect = 1.0;
	double p_cluster = 0.2;

//	TFile *out_file = new TFile("", "RECREATE");

	TRandom3 *rand = new TRandom3(0);

	TH1D *R_hist = new TH1D("Ratios", "Ratio Distribution", 20, 0.0, 1.1);
}

