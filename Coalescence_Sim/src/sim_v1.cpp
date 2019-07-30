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


tree_data sim_v1(config::simulation_pars pars) {
	tree_data data = simulate_Rs(pars);

	return(data);
}


tree_data simulate_Rs(config::simulation_pars pars) {
	vector<double> proton_angles;
	TRandom3 *rand = new TRandom3(0);
	tree_data data;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event(rand, pars);
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		data.good_protons[0][(int)proton_angles.size()] ++;
		proton_angles = rotate_angles(proton_angles, rand->Rndm() * 2 * M_PI);
		for(int r:get_Rs(proton_angles, pars.divisions)) {
			data.ratios[pars.divisions][0][(int)proton_angles.size()][r] ++;
		}
	}

	delete rand;

	return(data);
}


vector<tree_data> simulate_Rs_mixed(config::simulation_pars pars) {
	vector<double> proton_angles;
	TRandom3 *rand = new TRandom3(0);
	tree_data data;
	map<int, vector<double>> mixed_angles;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event(rand, pars);
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		mixed_angles[proton_angles.size()].insert(mixed_angles[proton_angles.size()].end(), proton_angles.begin(), proton_angles.end());
		data.good_protons[0][(int)proton_angles.size()] ++;
		proton_angles = rotate_angles(proton_angles, rand->Rndm() * 2 * M_PI);
		for(int r:get_Rs(proton_angles, pars.divisions)) {
			data.ratios[pars.divisions][0][(int)proton_angles.size()][r] ++;
		}
	}

	tree_data mixed_data;
	for(pair<int, vector<double>> num_protons:mixed_angles) {
		int num_events = num_protons.second.size() / num_protons.first;
		for(int event_index = 0; event_index < num_events; event_index++) {
			mixed_data.good_protons[0][(int)num_protons.first] ++;

		}
	}

	delete rand;

	vector<tree_data> both_data = {data, mixed_data};

	return(both_data);
}


void simulate_2pcorr(config::simulation_pars pars) {
	vector<double> proton_angles, corr2p;
	TRandom3 *rand = new TRandom3(0);
	cout << "Here" << endl;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event(rand, pars);
		for(double cor:get_two_particle_correlation(proton_angles)) {
			corr2p.push_back(cor);
		}
	}

	delete rand;

	plot_corrs(corr2p);
}


vector<double> simulate_event(TRandom3* rand, config::simulation_pars pars) {
	int n_protons, rand_protons, group_protons;
	double group_angle;
	vector<double> proton_angles = {};

	n_protons = get_n_protons(rand, pars.particle_mean);
	if(rand->Rndm() < pars.p_effect) {
		group_protons = (int) (n_protons * pars.p_cluster + 0*0.5); //changed from 0.5 to 0,1.5 for testing purposes
		group_angle = rand->Rndm() * 2 * M_PI;
		for(int j = 0; j < group_protons; j++) {
			double cor_angle = rand->Gaus(group_angle, pars.spread_sigma);
			while(cor_angle > 2*M_PI) { cor_angle -= 2*M_PI; }
			while(cor_angle < 0) { cor_angle += 2*M_PI; }
			proton_angles.push_back(cor_angle);
		}
	} else {
		group_protons = 0;
	}

	rand_protons = n_protons - group_protons;
	for(int j = 0; j < rand_protons; j++) {
		proton_angles.push_back(rand->Rndm() * 2 * M_PI);
	}

	return(proton_angles);
}


int get_n_protons(TRandom3 *rand, double mean) {
//	int n = (int) (config::particle_mean * config::rand->Rndm() + 0.5) + 1;
	int n = rand->Poisson(mean);
	return (n);
}
