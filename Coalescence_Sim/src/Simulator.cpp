/*
 * Simulator.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: dylan
 */


#include <vector>
#include <cmath>

#include <TRandom3.h>

#include "Simulator.h"
#include "ratio_methods.h"

using namespace std;


Simulator::Simulator() {
	rand = new TRandom3(0);
	proton_dist_hist = new TH1D();
}

// Getters
int Simulator::get_n_events() {
	return(pars.n_events);
}

int Simulator::get_divisions() {
	return(pars.divisions);
}

double Simulator::get_p_effect() {
	return(pars.p_effect);
}

double Simulator::get_p_cluster() {
	return(pars.p_cluster);
}

double Simulator::get_spread_sigma() {
	return(pars.spread_sigma);
}


// Setters
void Simulator::set_n_events(int n) {
	pars.n_events = n;
}

void Simulator::set_divisions(int divs) {
	pars.divisions = divs;
}

void Simulator::set_p_effect(double p_eff) {
	pars.p_effect = p_eff;
}

void Simulator::set_p_cluster(double p_clust) {
	pars.p_cluster = p_clust;
}

void Simulator::set_spread_sigma(double sig) {
	pars.spread_sigma = sig;
}

void Simulator::set_min_protons(int protons) {
	pars.min_protons = protons;
}

void Simulator::set_proton_dist(string dist) {
	pars.proton_dist = dist;
}

void Simulator::set_particle_mean(double mean) {
	pars.particle_mean = mean;
}

void Simulator::set_proton_dist_hist(TH1D *hist) {
	proton_dist_hist = hist;
	pars.proton_dist = "hist";
}


// Doers
tree_data Simulator::run_simulation() {
	vector<double> proton_angles;
	tree_data data;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event();
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		data.good_protons[0][(int)proton_angles.size()] ++;
		proton_angles = rotate_angles(proton_angles, rand->Rndm() * 2*M_PI);
		for(int r:get_Rs(proton_angles, pars.divisions)) {
			data.ratios[pars.divisions][0][(int)proton_angles.size()][r] ++;
		}
	}

	delete rand;

	return(data);
}


vector<double> Simulator::simulate_event() {
	int n_protons, rand_protons, group_protons;
	double group_angle;
	vector<double> proton_angles = {};

	n_protons = get_protons();
	if(rand->Rndm() < pars.p_effect) {
		group_protons = (int) (n_protons * pars.p_cluster + 0.5);
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


int Simulator::get_protons() {
	int n = 0;
	if(pars.proton_dist == "poisson") {
		n = rand->Poisson(pars.particle_mean);
	} else if(pars.proton_dist == "hist") {
		n = proton_dist_hist->GetRandom();
	}

	return (n);
}
