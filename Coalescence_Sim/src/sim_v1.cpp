/*
 * sim_v1.cpp
 *
 *  Created on: Mar 14, 2019
 *      Author: dylan
 */


#include <iostream>
#include <math.h>

#include "TFile.h"
#include "TRandom3.h"
#include "TH1D.h"

#include "sim_v1.h"
#include "ratio_methods.h"
#include "config.h"

using namespace std;


void sim_v1() {
	cout << "Starting Simulation v1." << endl;

	vector<double>Rs = simulate();
//	analyze(Rs);
	plot(Rs);


	cout << "donzo" << endl;
}


vector<double> simulate() {
	int n_protons, rand_protons, group_protons;
	double group_angle;
	vector<double> proton_angles, Rs;
	for(int i = 0; i < config::n_events; i++) {
		n_protons = get_n_protons();
		if(config::rand->Rndm() <= config::p_effect) {
			group_protons = (int) (n_protons * config::p_cluster + 0.5);
		} else {
			group_protons = 0;
		}
		rand_protons = n_protons - group_protons;

		proton_angles = {};
		for(int j = 0; j < rand_protons; j++) {
			proton_angles.push_back(config::rand->Rndm() * 2 * M_PI);
		}
		group_angle = config::rand->Rndm() * 2 * M_PI;
		for(int j = 0; j < group_protons; j++) {
			proton_angles.push_back(group_angle);
		}
		for(auto r:get_Rs(proton_angles, config::divisions)) {
			Rs.push_back(r);
		}
	}


	return(Rs);
}


void plot(vector<double> Rs) {
	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
	for(auto r:Rs) {
//		cout << r << endl;
		config::R_hist->Fill(r);
	}
	config::R_hist->Write();
	delete config::R_hist;
	delete config::rand;
	delete out_file;
}


int get_n_protons() {
	int n = (int) (25 * config::rand->Rndm() + 0.5) + 1;
	return (n);
}
