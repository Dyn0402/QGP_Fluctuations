/*
 * sim_v1.cpp
 *
 *  Created on: Mar 14, 2019
 *      Author: dylan
 */


#include <iostream>
#include <math.h>

#include <TFile.h>
#include <TRandom3.h>
#include <TH1D.h>

#include "sim_v1.h"
#include "ratio_methods.h"
#include "plotting.h"
#include "config.h"

using namespace std;


tree_data sim_v1() {
	tree_data data = simulate_Rs();

	return(data);
}


tree_data simulate_Rs() {
	vector<double> proton_angles;
	tree_data data;
	for(int i = 0; i < config::n_events; i++) {
		proton_angles = simulate_event();
		data.good_protons[0][(int)proton_angles.size()] ++;
		for(int r:get_Rs(proton_angles, config::divisions)) {
			data.ratios[config::divisions][0][(int)proton_angles.size()][r] ++;
		}
	}

	return(data);
}


void simulate_2pcorr() {
	vector<double> proton_angles, corr2p;
	cout << "Here" << endl;
	for(int i = 0; i < config::n_events; i++) {
		proton_angles = simulate_event();
		for(double cor:get_two_particle_correlation(proton_angles)) {
			corr2p.push_back(cor);
		}
	}

	plot_corrs(corr2p);
}


vector<double> simulate_event() {
	int n_protons, rand_protons, group_protons;
	double group_angle;
	vector<double> proton_angles;

	n_protons = get_n_protons();
	if(config::rand->Rndm() <= config::p_effect) {
		group_protons = (int) (n_protons * config::p_cluster + 0.5); //changed from 0.5 to 1.5 for testing purposes
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
		double cor_angle = config::rand->Gaus(group_angle, config::spread_sigma);
		while(cor_angle > 2*M_PI) { cor_angle -= 2*M_PI; }
		while(cor_angle < 0) { cor_angle += 2*M_PI; }
		proton_angles.push_back(cor_angle);
	}

	return(proton_angles);
}


int get_n_protons() {
//	int n = (int) (config::particle_mean * config::rand->Rndm() + 0.5) + 1;
	int n = config::rand->Poisson(config::particle_mean);
	return (n);
}
