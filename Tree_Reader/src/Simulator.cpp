/*
 * Simulator.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: dylan
 */


#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

#include <TRandom3.h>
#include <TCanvas.h>
#include <TH1.h>

#include "ratio_methods.h"
#include "Simulator.h"

using namespace std;


Simulator::Simulator() {
	sim_rand = new TRandom3(0);
	proton_dist_hist = new TH1D();
	proton_dist_hist->SetDirectory(0);
	two_p_corr = new TH1D();
	two_p_corr->SetDirectory(0);
	efficiency_dist = new TH1D();
	efficiency_dist->SetDirectory(0);
	norm_eff_dist = new TH1D();
	norm_eff_dist->SetDirectory(0);
	ep_dist = new TH1D();
	ep_dist->SetDirectory(0);
//	trand = 0;
	simulate_event = bind(&Simulator::sim_event, this);
	simulate_event2 = bind(&Simulator::sim_event_flow, this, placeholders::_1);
}


Simulator::~Simulator() {
//	delete sim_rand;
//	delete proton_dist_hist;
//	delete two_p_corr; For some reason breaks, figure out later. Because it's owned by current TFile from calling class?
}

// Getters
int Simulator::get_n_events() {
	return(pars.n_events);
}

int Simulator::get_divisions() {
	return(pars.divisions);
}

double Simulator::get_p_group() {
	return(pars.p_group);
}

double Simulator::get_spread_sigma() {
	return(pars.spread_sigma);
}

int Simulator::get_num_event_mix() {
	return(pars.num_event_mix);
}

string Simulator::get_proton_dist_type() {
	return(pars.proton_dist);
}

TH1D* Simulator::get_two_p_corr() {
	return(two_p_corr);
}

TH1D* Simulator::get_efficiency_dist() {
	return(efficiency_dist);
}


// Setters
void Simulator::set_n_events(int n) {
	pars.n_events = n;
}

void Simulator::set_divisions(int divs) {
	pars.divisions = divs;
}

void Simulator::set_p_group(double p_group) {
	pars.p_group = p_group;
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
	proton_dist_hist->SetDirectory(0);
	pars.proton_dist = "hist";
}

void Simulator::set_efficiency_dist_hist(TH1D *hist) {
	efficiency_dist = hist;
	efficiency_dist->SetDirectory(0);
	norm_eff_dist = (TH1D*)efficiency_dist->Clone();
	norm_eff_dist->Scale(pars.hom_eff/norm_eff_dist->GetMaximum());
	norm_eff_dist->SetDirectory(0);
//	simulate_event = bind(&Simulator::sim_event_eff, this);
}

void Simulator::set_efficiency_dist_hist(string root_path, string hist_name) {
	TFile *file = new TFile(root_path.data(), "READ");
	TH1D *hist = (TH1D*)file->Get(hist_name.data());
	efficiency_dist = (TH1D*)hist->Clone();
	efficiency_dist->SetDirectory(0);
	norm_eff_dist = (TH1D*)efficiency_dist->Clone();
	norm_eff_dist->Scale(pars.hom_eff/norm_eff_dist->GetMaximum());
	norm_eff_dist->SetDirectory(0);

	file->Close();
	delete file;
//	simulate_event = bind(&Simulator::sim_event_eff, this);
}

void Simulator::set_eff() {
//	simulate_event = bind(&Simulator::sim_event_eff, this);
//	simulate_event2 = bind(&Simulator::sim_event_eff, this);
	cout << "Simulator set_eff not set." << endl;
}

void Simulator::set_flow() {
	simulate_event2 = bind(&Simulator::sim_event_flow, this, placeholders::_1);
}

void Simulator::set_eff_flow() {
	simulate_event2 = bind(&Simulator::sim_event_eff_flow, this, placeholders::_1);
}

void Simulator::set_num_event_mix(int num) {
	pars.num_event_mix = num;
}

void Simulator::set_hom_eff(double eff) {
	pars.hom_eff = eff;
}

void Simulator::set_v2(double v2) {
	pars.v2 = v2;
}

void Simulator::set_ep_res(double res) {
	pars.ep_res = res;
}

void Simulator::set_flow(double v2, double res, double chi_acc) {
	pars.v2 = v2;
	pars.ep_res = res;
	ep_dist = new TH1D("ep_dist", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -M_PI, M_PI);
	ep_dist->SetDirectory(0);
	for(int bin = 0; bin <= ep_dist->GetXaxis()->GetNbins(); bin++) {
		ep_dist->SetBinContent(bin, event_plane(res, ep_dist->GetBinCenter(bin), chi_acc));
	}
//	simulate_event = bind(&Simulator::sim_event_flow, this);
}


// Doers
map<int, map<int, int>> Simulator::run_simulation() {
	vector<double> proton_angles;
	map<int, map<int, int>> ratio_data;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event();
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		proton_angles = rotate_angles(proton_angles, sim_rand->Rndm() * 2*M_PI);
		for(int r:get_Rs(proton_angles, pars.divisions)) {
			ratio_data[(int)proton_angles.size()][r] ++;
		}
	}

	return(ratio_data);
}


vector<map<int, map<int, int>>> Simulator::run_simulation_mixed() {
	vector<double> proton_angles;
	map<int, map<int, int>> ratio_data;
	map<int, vector<double>> mixed_angles;
	map<int, map<int, int>> mixed_ratio_data;
	int n_protons, n_mix_events;
	for(int i = 0; i < pars.n_events; i++) {
		proton_angles = simulate_event();
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		proton_angles = rotate_angles(proton_angles, sim_rand->Rndm() * 2*M_PI);
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
				rand_angles = rotate_angles(rand_angles, sim_rand->Rndm() * 2*M_PI);
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
		rand_angles = rotate_angles(rand_angles, sim_rand->Rndm() * 2*M_PI);
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


vector<map<int, map<int, int>>> Simulator::run_sim_mixed_2p(string two_p_name) {
	two_p_corr = new TH1D(two_p_name.data(), two_p_name.data(), two_p_bins, -two_p_shift, 2*M_PI - two_p_shift);
	vector<double> proton_angles;
	map<int, map<int, int>> ratio_data;
	map<int, vector<double>> mixed_angles;
	map<int, map<int, int>> mixed_ratio_data;
	int n_protons, n_mix_events;
	for(int i = 0; i < pars.n_events; i++) {
		// Simulate Event
		proton_angles = simulate_event();
		if(proton_angles.size() < (unsigned)pars.min_protons) { continue; }
		n_protons = (int) proton_angles.size();
		// Two particle filling
		for(double cor:get_two_particle_correlation(proton_angles)) {
			while(cor > 2*M_PI - two_p_shift) { cor -= 2*M_PI; }
			while(cor < -two_p_shift) { cor += 2*M_PI; }
			two_p_corr->Fill(cor);
		}
		// Ratio data filling
		proton_angles = rotate_angles(proton_angles, sim_rand->Rndm() * 2*M_PI);
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
				rand_angles = rotate_angles(rand_angles, sim_rand->Rndm() * 2*M_PI);
				for(int r:get_Rs(rand_angles, pars.divisions)) {
					mixed_ratio_data[(int)rand_angles.size()][r] ++;
				}
			}
			mixed_angles[n_protons] = {};
		}
	}

	TCanvas *can = new TCanvas();
	two_p_corr->Draw();
	two_p_corr->GetXaxis()->SetTitle("#Delta#phi");
	can->Write(two_p_name.data());
	delete can;

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
		rand_angles = rotate_angles(rand_angles, sim_rand->Rndm() * 2*M_PI);
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


// Simulate single event and return simulated proton angles
vector<double> Simulator::sim_event() {
	double group_angle;
	vector<double> proton_angles = {};

	int n_protons = get_protons();

	if(n_protons > 0) { proton_angles.push_back(sim_rand->Rndm() * 2 * M_PI); }
	for(int j=1; j< n_protons; j++) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			proton_angles.push_back(group_angle);
		} else {
			proton_angles.push_back(sim_rand->Rndm() * 2 * M_PI);
		}
	}

	return(proton_angles);
}


// Simulate single event and return simulated proton angles. Include efficiency into simulation.
vector<double> Simulator::sim_event_eff() {
	double group_angle, new_angle;
	vector<double> proton_angles = {};

	int n_protons = get_protons();

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
			proton_angles.push_back(new_angle);
		}
	}
	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(group_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(group_angle);
			}
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
				proton_angles.push_back(new_angle);
			}
		}

	}

	return(proton_angles);
}


// Simulate single event and return simulated proton angles. Include elliptic flow into simulation.
//pair<vector<double>, double> Simulator::sim_event_flow() {
//	double group_angle, new_angle;
//	vector<double> proton_angles = {};
//
//	int n_protons = get_protons();
//
//	double reaction_plane = 2*M_PI*sim_rand->Rndm();
//	double deviation = ep_dist->GetRandom();
//	double event_plane = reaction_plane + deviation;
//	event_plane = fmod(event_plane, 2*M_PI);  // Force to range [0, 2*pi)
//	if(event_plane < 0) { event_plane += 2*M_PI; }
//
//	if(n_protons > 0) while((int)proton_angles.size() < 1) {
//		new_angle = sim_rand->Rndm() * 2 * M_PI;
//		if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
//			if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
//				proton_angles.push_back(new_angle);
//			}
//		}
//	}
//
//	while((int)proton_angles.size() < n_protons) {
//		if(sim_rand->Rndm() < pars.p_group) {
//			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
//			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
//			if(group_angle < 0) { group_angle += 2*M_PI; }
//			if(1.0 + 2 * pars.v2 * cos(2*(group_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
//				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(group_angle)) >= sim_rand->Rndm()) {
//					proton_angles.push_back(group_angle);
//				}
//			}
//		} else {
//			new_angle = sim_rand->Rndm() * 2 * M_PI;
//			if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
//				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
//					proton_angles.push_back(new_angle);
//				}
//			}
//		}
//	}
//
//	pair<vector<double>, double> result = {proton_angles, event_plane};
//
//	return(result);
//}


// Simulate single event and return simulated proton angles. Include elliptic flow into simulation.
void Simulator::sim_event_eff_flow(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles = {};

	int n_protons = get_protons();

	double reaction_plane = 2*M_PI*sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, 2*M_PI);  // Force to range [0, 2*pi)
	if(event_plane < 0) { event_plane += 2*M_PI; }
	event.set_event_plane(event_plane);

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
			if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
				proton_angles.push_back(new_angle);
			}
		}
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			if(1.0 + 2 * pars.v2 * cos(2*(group_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(group_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(group_angle);
				}
			}
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(new_angle);
				}
			}
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_protons(tracks);
}



// Simulate single event and return simulated proton angles. Include elliptic flow into simulation.
void Simulator::sim_event_flow(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles = {};

	int n_protons = get_protons();

	double reaction_plane = 2*M_PI*sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, 2*M_PI);  // Force to range [0, 2*pi)
	if(event_plane < 0) { event_plane += 2*M_PI; }
	event.set_event_plane(event_plane);

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
			proton_angles.push_back(new_angle);
		}
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			if(1.0 + 2 * pars.v2 * cos(2*(group_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				proton_angles.push_back(group_angle);
			}
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				proton_angles.push_back(new_angle);
			}
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_protons(tracks);

}



// Simulate single event and return simulated proton angles. Include elliptic flow into simulation.
void Simulator::sim_event_eff_new(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles = {};

	int n_protons = get_protons();

//	double reaction_plane = 2*M_PI*sim_rand->Rndm();
//	double deviation = ep_dist->GetRandom();
//	double event_plane = reaction_plane + deviation;
//	event_plane = fmod(event_plane, 2*M_PI);  // Force to range [0, 2*pi)
//	if(event_plane < 0) { event_plane += 2*M_PI; }

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
//		if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
			if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
				proton_angles.push_back(new_angle);
			}
//		}
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
//			if(1.0 + 2 * pars.v2 * cos(2*(group_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(group_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(group_angle);
				}
//			}
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
//			if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(new_angle);
				}
//			}
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_protons(tracks);

}



// Simulate single event and return simulated proton angles. Include efficiency into simulation.
vector<double> Simulator::sim_event_eff2() {
	double group_angle;
	vector<double> proton_angles = {};

	int n_protons = get_protons();

	if(n_protons > 0) { proton_angles.push_back(efficiency_dist->GetRandom()); }
	for(int j=1; j< n_protons; j++) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = get_group_angle(proton_angles.back());
			while(group_angle >= 2*M_PI) { group_angle -= 2*M_PI; }  // Force to range [0, 2*pi)
			while(group_angle < 0) { group_angle += 2*M_PI; }
			proton_angles.push_back(group_angle);
		} else {
			proton_angles.push_back(efficiency_dist->GetRandom());
		}

	}

	return(proton_angles);
}



int Simulator::get_protons() {
	int n = 0;
	if(pars.proton_dist == "poisson") {
		n = sim_rand->Poisson(pars.particle_mean);
	} else if(pars.proton_dist == "hist") {
		n = proton_dist_hist->GetRandom();
	}

	return (n);
}


double Simulator::get_group_angle(double center) {
	TH1D *convo = (TH1D*)efficiency_dist->Clone();

	for(int bin=1; bin<=convo->GetXaxis()->GetNbins(); bin++) {
		double new_bin_content = convo->GetBinContent(bin) * wrap_gaus(convo->GetBinCenter(bin), center, pars.spread_sigma, 0, 2*TMath::Pi());

//		cout << "bin: " << bin << " center: " << center << " old contents: " << convo->GetBinContent(bin) << " new contents: " << new_bin_content << endl;
//		double new_bin_content = convo->GetBinContent(bin) * TMath::Gaus(convo->GetBinCenter(bin), center, pars.spread_sigma);
//		if(fabs(convo->GetBinCenter(bin) - center) < 1.0) { cout << new_bin_content << endl; }
		convo->SetBinContent(bin, new_bin_content);
	}

//	this_thread::sleep_for(chrono::seconds(30));

//	cout << wrap_gaus(0, 0, pars.spread_sigma, 0, 2*TMath::Pi()) << endl;

//	if(convo->Integral() <= 0) {
//		cout << "Get random convo   bin: " << convo->FindBin(center) << "  content: " << convo->GetBinContent(convo->FindBin(center)) << "  center: " << center << endl;
//	}
	double new_angle = convo->GetRandom();
	delete convo;
	return(new_angle);

}


// Wrap gaussian with mean mu and width sigma at point x around a range from lower_bound to upper_bound.
double Simulator::wrap_gaus(double x, double mu, double sigma, double lower_bound, double upper_bound) {
	double range = upper_bound - lower_bound;
	double sum = 0;
	double gaus_val = TMath::Gaus(x, mu, sigma);
	double xi = x;
	while(gaus_val > 0) {  // Negative side loop
		sum += gaus_val;
		xi -= range;
		gaus_val = TMath::Gaus(xi, mu, sigma);
	}
	xi = x + range;
	gaus_val = TMath::Gaus(xi, mu, sigma);
	while(gaus_val > 0) {  // Positive side loop
		sum += gaus_val;
		xi += range;
		gaus_val = TMath::Gaus(xi, mu, sigma);
	}

	return(sum);
}


double Simulator::event_plane_gang(double res, double psi) {
	double chi_f = chi_gang(res);
	double st = exp(-chi_f*chi_f/2.);
	double nd = pow(M_PI/2.,0.5)*chi_f*cos(psi);
	double rd = exp(-chi_f*chi_f*sin(psi)*sin(psi)/2.);
	double th = 1 + TMath::Erf(chi_f*cos(psi)/sqrt(2.));
	double eventplane = (st + nd*rd*th)/(2.*M_PI);
	return eventplane;
}


double Simulator::event_plane(double res, double psi, double acc) {
	double chi_f = chi(res, acc);
	double st = exp(-chi_f*chi_f/2.);
	double nd = pow(M_PI/2.,0.5)*chi_f*cos(psi);
	double rd = exp(-chi_f*chi_f*sin(psi)*sin(psi)/2.);
	double th = 1 + TMath::Erf(chi_f*cos(psi)/sqrt(2.));
	double eventplane = (st + nd*rd*th)/(2.*M_PI);
	return eventplane;
}


double Simulator::chi_gang(double res) {
  double chi   = 2.0;
  double delta = 1.0;

  for (int i = 0; i < 15; i++) {
      while(res_event_plane(chi) < res) {chi += delta;}
      delta = delta / 2.;
      while(res_event_plane(chi) > res) {chi -= delta;}
      delta = delta / 2.;
  }

  return chi;
}


double Simulator::chi(double res, double acc) {
  double chi   = 2.0;
  double delta = 1.0;
  double diff = -numeric_limits<double>::max();

  while (fabs(diff) > acc) {
      while(res_event_plane(chi) < res) {chi += delta;}
      delta = delta / 2.;
      while(res_event_plane(chi) > res) {chi -= delta;}
      delta = delta / 2.;
      diff = res_event_plane(chi) - res;
  }

  return chi;
}


double Simulator::res_event_plane(double chi) {
  double con = 0.626657;
  double arg = chi * chi / 4.;

  double res = con * chi * exp(-arg) * (TMath::BesselI0(arg) +
                                          TMath::BesselI1(arg));

  return res;
}



void Simulator::write_two_p_corr() {
	two_p_corr->Write();
}
