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
	two_p_corr = new TH1D();
}

Simulator2::~Simulator2() {
	delete rand;
	delete proton_dist_hist;
	delete two_p_corr;
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

int Simulator2::get_num_event_mix() {
	return(pars.num_event_mix);
}

TH1D* Simulator2::get_two_p_corr() {
	return(two_p_corr);
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

void Simulator2::set_num_event_mix(int num) {
	pars.num_event_mix = num;
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

	return(ratio_data);
}


vector<map<int, map<int, int>>> Simulator2::run_simulation_mixed() {
	vector<double> proton_angles;
	map<int, map<int, int>> ratio_data;
	map<int, vector<double>> mixed_angles;
	map<int, map<int, int>> mixed_ratio_data;
	int n_protons, n_mix_events;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event();
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		proton_angles = rotate_angles(proton_angles, rand->Rndm() * 2*M_PI);
		n_protons = (int) proton_angles.size();
		for(int r:get_Rs(proton_angles, pars.divisions)) {
			ratio_data[n_protons][r] ++;
		}
		// Mixed event logic
		mixed_angles[n_protons].insert(mixed_angles[n_protons].end(), proton_angles.begin(), proton_angles.end());
		n_mix_events = (int) mixed_angles[n_protons].size() / n_protons;
		if(n_mix_events >= pars.num_event_mix) {
			random_shuffle(mixed_angles[n_protons].begin(), mixed_angles[n_protons].end());
			for(int event_index = 0; event_index < n_mix_events; event_index++) {
				vector<double> rand_angles(mixed_angles[n_protons].begin()+event_index*n_protons, mixed_angles[n_protons].begin()+(event_index+1)*n_protons);
				rand_angles = rotate_angles(rand_angles, rand->Rndm() * 2*M_PI);
				for(int r:get_Rs(rand_angles, pars.divisions)) {
					mixed_ratio_data[(int)rand_angles.size()][r] ++;
				}
			}
			mixed_angles[n_protons] = {};
		}
	}

	// Deal with left over mixed events. Not very efficient, just groups all left over into one mix. Improve.
	vector<double> left_over;
	for(pair<int, vector<double>> num_protons:mixed_angles) {
		for(double angle:num_protons.second) {
			left_over.push_back(angle);
		}
	}
	random_shuffle(left_over.begin(), left_over.end());
	int event_protons;
	do {
		event_protons = get_protons();
	} while (event_protons < pars.min_protons);
	int proton_index = 0;
	int num_leftover = (int)left_over.size();
	while(proton_index + event_protons < num_leftover) {
		vector<double> rand_angles(left_over.begin()+proton_index, left_over.begin()+proton_index+event_protons);
		rand_angles = rotate_angles(rand_angles, rand->Rndm() * 2*M_PI);
		for(int r:get_Rs(rand_angles, pars.divisions)) {
			mixed_ratio_data[event_protons][r] ++;
		}
		proton_index += event_protons;
		do {
			event_protons = get_protons();
		} while (event_protons < pars.min_protons);
	}

	vector<map<int, map<int, int>>> both_data = {ratio_data, mixed_ratio_data};

	return(both_data);
}


vector<map<int, map<int, int>>> Simulator2::run_sim_mixed_2p(string two_p_name) {
	two_p_corr = new TH1D(two_p_name.data(), two_p_name.data(), 100, -M_PI/4, 2*M_PI - M_PI/4);
	vector<double> proton_angles;
	map<int, map<int, int>> ratio_data;
	map<int, vector<double>> mixed_angles;
	map<int, map<int, int>> mixed_ratio_data;
	int n_protons, n_mix_events;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event();
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		proton_angles = rotate_angles(proton_angles, rand->Rndm() * 2*M_PI);
		n_protons = (int) proton_angles.size();
		for(int r:get_Rs(proton_angles, pars.divisions)) {
			ratio_data[n_protons][r] ++;
		}
		// Two particle filling
		for(double cor:get_two_particle_correlation(proton_angles)) {
			if(cor > 2*M_PI - M_PI / 4) { cor -= 2*M_PI; }
			two_p_corr->Fill(cor);
		}
		// Mixed event logic
		mixed_angles[n_protons].insert(mixed_angles[n_protons].end(), proton_angles.begin(), proton_angles.end());
		n_mix_events = (int) mixed_angles[n_protons].size() / n_protons;
		if(n_mix_events >= pars.num_event_mix) {
			random_shuffle(mixed_angles[n_protons].begin(), mixed_angles[n_protons].end());
			for(int event_index = 0; event_index < n_mix_events; event_index++) {
				vector<double> rand_angles(mixed_angles[n_protons].begin()+event_index*n_protons, mixed_angles[n_protons].begin()+(event_index+1)*n_protons);
				rand_angles = rotate_angles(rand_angles, rand->Rndm() * 2*M_PI);
				for(int r:get_Rs(rand_angles, pars.divisions)) {
					mixed_ratio_data[(int)rand_angles.size()][r] ++;
				}
			}
			mixed_angles[n_protons] = {};
		}
	}

	two_p_corr->Write();

	// Deal with left over mixed events. Not very efficient, just groups all left over into one mix. Improve.
	vector<double> left_over;
	for(pair<int, vector<double>> num_protons:mixed_angles) {
		for(double angle:num_protons.second) {
			left_over.push_back(angle);
		}
	}
	random_shuffle(left_over.begin(), left_over.end());
	int event_protons;
	do {
		event_protons = get_protons();
	} while (event_protons < pars.min_protons);
	int proton_index = 0;
	int num_leftover = (int)left_over.size();
	while(proton_index + event_protons < num_leftover) {
		vector<double> rand_angles(left_over.begin()+proton_index, left_over.begin()+proton_index+event_protons);
		rand_angles = rotate_angles(rand_angles, rand->Rndm() * 2*M_PI);
		for(int r:get_Rs(rand_angles, pars.divisions)) {
			mixed_ratio_data[event_protons][r] ++;
		}
		proton_index += event_protons;
		do {
			event_protons = get_protons();
		} while (event_protons < pars.min_protons);
	}

	vector<map<int, map<int, int>>> both_data = {ratio_data, mixed_ratio_data};

	return(both_data);
}


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


void Simulator2::write_two_p_corr() {
	two_p_corr->Write();
}
