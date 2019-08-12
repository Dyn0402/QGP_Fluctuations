/*
 * Simulator2.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: dylan
 */


#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

#include <TRandom3.h>

#include "Simulator2.h"
#include "ratio_methods.h"

using namespace std;


Simulator2::Simulator2() {
	rand = new TRandom3(0);
	proton_dist_hist = new TH1D();
}

// Getters
int Simulator2::get_n_events() {
	return(pars.n_events);
}

int Simulator2::get_divisions() {
	return(pars.divisions);
}

double Simulator2::get_p_group() {
	return(pars.p_group);
}

double Simulator2::get_spread_sigma() {
	return(pars.spread_sigma);
}


// Setters
void Simulator2::set_n_events(int n) {
	pars.n_events = n;
}

void Simulator2::set_divisions(int divs) {
	pars.divisions = divs;
}

void Simulator2::set_p_group(double p_group) {
	pars.p_group = p_group;
}

void Simulator2::set_spread_sigma(double sig) {
	pars.spread_sigma = sig;
}

void Simulator2::set_min_protons(int protons) {
	pars.min_protons = protons;
}

void Simulator2::set_proton_dist(string dist) {
	pars.proton_dist = dist;
}

void Simulator2::set_particle_mean(double mean) {
	pars.particle_mean = mean;
}

void Simulator2::set_proton_dist_hist(TH1D *hist) {
	proton_dist_hist = hist;
	pars.proton_dist = "hist";
}


// Doers
map<int, map<int, int>> Simulator2::run_simulation() {
	vector<double> proton_angles;
	map<int, map<int, int>> ratio_data;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event();
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		proton_angles = rotate_angles(proton_angles, rand->Rndm() * 2*M_PI);
		for(int r:get_Rs(proton_angles, pars.divisions)) {
			ratio_data[(int)proton_angles.size()][r] ++;
		}
	}

	delete rand;

	return(ratio_data);
}


vector<map<int, map<int, int>>> Simulator2::run_simulation_mixed() {
	vector<double> proton_angles;
	map<int, map<int, int>> ratio_data;
	map<int, vector<double>> mixed_angles;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event();
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		mixed_angles[proton_angles.size()].insert(mixed_angles[proton_angles.size()].end(), proton_angles.begin(), proton_angles.end());
		proton_angles = rotate_angles(proton_angles, rand->Rndm() * 2*M_PI);
		for(int r:get_Rs(proton_angles, pars.divisions)) {
			ratio_data[(int)proton_angles.size()][r] ++;
		}
	}

	map<int, map<int, int>> mixed_ratio_data;
	for(pair<int, vector<double>> num_protons:mixed_angles) {
		random_shuffle(num_protons.second.begin(), num_protons.second.end());
		int num_events = num_protons.second.size() / num_protons.first;
		for(int event_index = 0; event_index < num_events; event_index++) {
			vector<double> rand_angles(num_protons.second.begin()+event_index*num_protons.first, num_protons.second.begin()+(event_index+1)*num_protons.first);
			proton_angles = rotate_angles(rand_angles, rand->Rndm() * 2*M_PI);
			for(int r:get_Rs(rand_angles, pars.divisions)) {
				mixed_ratio_data[(int)rand_angles.size()][r] ++;
			}
		}
	}

	delete rand;

	vector<map<int, map<int, int>>> both_data = {ratio_data, mixed_ratio_data};

	return(both_data);
}



//vector<map<int, map<int, int>>> Simulator2::run_sim_mixed_2p() {
//	vector<double> proton_angles;
//	map<int, map<int, int>> ratio_data;
//	map<int, vector<double>> mixed_angles;
//	for(int i = 0; i < pars.n_events; i++) {
//		proton_angles = simulate_event();
//		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
//		mixed_angles[proton_angles.size()].insert(mixed_angles[proton_angles.size()].end(), proton_angles.begin(), proton_angles.end());
//		proton_angles = rotate_angles(proton_angles, rand->Rndm() * 2*M_PI);
//		for(int r:get_Rs(proton_angles, pars.divisions)) {
//			ratio_data[(int)proton_angles.size()][r] ++;
//		}
//	}
//
//	for(double cor:get_two_particle_correlation(proton_angles)) {
//		corr2p.push_back(cor);
//	}
//
//	map<int, map<int, int>> mixed_ratio_data;
//	for(pair<int, vector<double>> num_protons:mixed_angles) {
//		random_shuffle(num_protons.second.begin(), num_protons.second.end());
//		int num_events = num_protons.second.size() / num_protons.first;
//		for(int event_index = 0; event_index < num_events; event_index++) {
//			vector<double> rand_angles(num_protons.second.begin()+event_index*num_protons.first, num_protons.second.begin()+(event_index+1)*num_protons.first);
//			proton_angles = rotate_angles(rand_angles, rand->Rndm() * 2*M_PI);
//			for(int r:get_Rs(rand_angles, pars.divisions)) {
//				mixed_ratio_data[(int)rand_angles.size()][r] ++;
//			}
//		}
//	}
//
//	delete rand;
//
//	vector<map<int, map<int, int>>> both_data = {ratio_data, mixed_ratio_data};
//
//	return(both_data);
//}


vector<double> Simulator2::simulate_event() {
	double group_angle;
	vector<double> proton_angles = {};

	int n_protons = get_protons();

	if(n_protons > 0) { proton_angles.push_back(rand->Rndm() * 2 * M_PI); }
	for(int j=1; j< n_protons; j++) {
		if(rand->Rndm() < pars.p_group) {
			group_angle = rand->Gaus(proton_angles.back(), pars.spread_sigma);
			while(group_angle > 2*M_PI) { group_angle -= 2*M_PI; }
			while(group_angle < 0) { group_angle += 2*M_PI; }
			proton_angles.push_back(group_angle);
		} else {
			proton_angles.push_back(rand->Rndm() * 2 * M_PI);
		}
	}

	return(proton_angles);
}


int Simulator2::get_protons() {
	int n = 0;
	if(pars.proton_dist == "poisson") {
		n = rand->Poisson(pars.particle_mean);
	} else if(pars.proton_dist == "hist") {
		n = proton_dist_hist->GetRandom();
	}

	return (n);
}
