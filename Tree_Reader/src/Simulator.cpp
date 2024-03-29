/*
 * Simulator.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: dylan
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
//#include <random>

#include <TRandom3.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>

#include "ratio_methods.h"
#include "Simulator.h"

using namespace std;


Simulator::Simulator() {
	sim_rand = new TRandom3(0);
	proton_dist_hist = new TH1D();
	proton_dist_hist->SetDirectory(0);
	efficiency_dist = new TH1D();
	efficiency_dist->SetDirectory(0);
	norm_eff_dist = new TH1D("Flat_Hist", "Flat Hist", 100, 0, 2 * M_PI);
	for (int bin=0; bin <= norm_eff_dist->GetXaxis()->GetNbins(); bin++) {  // Set default to flat distribution, 100% efficiency
		norm_eff_dist->SetBinContent(bin, 1.0);
	}
	norm_eff_dist->SetDirectory(0);
	ep_dist = new TH1D();
	ep_dist->SetDirectory(0);
	simulate_event = bind(&Simulator::sim_event, this, placeholders::_1);
}


Simulator::~Simulator() {
	//if(flow_res_n > 0) { cout << "Target res: " << pars.ep_res << " Actual res: " << flow_res / flow_res_n << endl; }
	//delete sim_rand;
	//delete proton_dist_hist;
	//delete efficiency_dist;
	//delete norm_eff_dist;
	//delete ep_dist;
}


// Getters

double Simulator::get_p_group() {
	return pars.p_group;
}

double Simulator::get_spread_sigma() {
	return pars.spread_sigma;
}

double Simulator::get_amp_group() {
	return pars.amp_group;
}

string Simulator::get_proton_dist_type() {
	return pars.proton_dist;
}

TH1D* Simulator::get_efficiency_dist() {
	return efficiency_dist;
}

simulation_pars Simulator::get_sim_pars() {
	return pars;
}


// Setters
void Simulator::set_p_group(double p_group) {
	pars.p_group = p_group;
}

void Simulator::set_spread_sigma(double sig) {
	pars.spread_sigma = sig;
}

void Simulator::set_spread_sigma_plus(double sig) {
	pars.spread_sigma_plus = sig;
}

void Simulator::set_spread_sigma_minus(double sig) {
	pars.spread_sigma_minus = sig;
}

void Simulator::set_amp_group(double amp) {
	pars.amp_group = amp;
}

void Simulator::set_amp_group_plus(double amp) {
	pars.amp_group_plus = amp;
}

void Simulator::set_amp_group_minus(double amp) {
	pars.amp_group_minus = amp;
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

void Simulator::set_particle_max(int max) {
	pars.particle_max = max;
}

void Simulator::set_flat_dist(int max) {
	pars.proton_dist = "flat";
	if(max > 2) {
		pars.particle_max = max;
	}
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
}

void Simulator::set_eff() {
	simulate_event = bind(&Simulator::sim_event_eff, this, placeholders::_1);
}

void Simulator::set_flow() {
	simulate_event = bind(&Simulator::sim_event_flow, this, placeholders::_1);
}

void Simulator::set_eff_flow() {
	simulate_event = bind(&Simulator::sim_event_eff_flow, this, placeholders::_1);
}

void Simulator::set_anti_clust() {
	simulate_event = bind(&Simulator::sim_event_anticlust, this, placeholders::_1);
	string gaus_wrap1_name = "gaus_wrap1_" + to_string(job_energy);
	string prob_name = "anti_clust_prob_" + to_string(job_energy);
	gaus_wrap1 = new TF1(gaus_wrap1_name.data(), "gaus(0) + gaus(3) + gaus(6)", -2 * M_PI, 4 * M_PI);
	prob = new TF1(prob_name.data(), "1 - gaus(0) - gaus(3) - gaus(6)", 0, 2 * M_PI);
	gaus_wrap1->SetParameters(1, M_PI, pars.spread_sigma, 1, M_PI - 2 * M_PI, pars.spread_sigma, 1, M_PI + 2 * M_PI, pars.spread_sigma);
	double amp = gaus_wrap1->Eval(M_PI);
	prob->SetParameters(1 / amp, M_PI, pars.spread_sigma, 1 / amp, M_PI - 2 * M_PI, pars.spread_sigma, 1 / amp, M_PI + 2 * M_PI, pars.spread_sigma);
//	UNCOMMENT BELOW
//	prob->GetRandom(sim_rand);
}

void Simulator::set_clust_multi() {
	simulate_event = bind(&Simulator::sim_event_clust_multi, this, placeholders::_1);
}

void Simulator::set_event_clust_multi_flow() {
	simulate_event = bind(&Simulator::sim_event_clust_multi_flow, this, placeholders::_1);
}

void Simulator::set_event_clust_multi_flow_indep() {
	simulate_event = bind(&Simulator::sim_event_clust_multi_flow_indep, this, placeholders::_1);
}

void Simulator::set_clust_multi_plus_minus() {
	simulate_event = bind(&Simulator::sim_event_clust_multi_plus_minus, this, placeholders::_1);
}

void Simulator::set_clust_final() {
	simulate_event = bind(&Simulator::sim_event_clust_final, this, placeholders::_1);
}

void Simulator::set_eff_simple_clust() {
	simulate_event = bind(&Simulator::sim_event_eff_simple_clust, this, placeholders::_1);
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

void Simulator::set_flow(double v2, double res, double chi_acc, int event_plane_n) {
	pars.v2 = v2;
	pars.ep_res = res;
	ep_dist = new TH1D("ep_dist", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -M_PI/event_plane_n, M_PI/event_plane_n);
	ep_dist->SetDirectory(0);
	for(int bin = 0; bin <= ep_dist->GetXaxis()->GetNbins(); bin++) {
		ep_dist->SetBinContent(bin, event_plane(res, event_plane_n*ep_dist->GetBinCenter(bin), chi_acc));
	}

}

void Simulator::set_job_energy(int num) {
	job_energy = num;
}


// Doers

// Simulate single event and return simulated proton angles.
void Simulator::sim_event(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		proton_angles.push_back(new_angle);
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			proton_angles.push_back(group_angle);
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			proton_angles.push_back(new_angle);
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event with anti-clustering and return simulated proton angles.
// DEPRICATED
void Simulator::sim_event_anticlust(Event& event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	if (n_protons > 0) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		proton_angles.push_back(new_angle);
	}

	while ((int)proton_angles.size() < n_protons) {
		if (sim_rand->Rndm() < pars.p_group) {
			// Need unique names
//			double last_proton = proton_angles.back();
//			gaus_wrap1->SetParameters(1, last_proton, pars.spread_sigma, 1, last_proton - 2 * M_PI, pars.spread_sigma, 1, last_proton + 2 * M_PI, pars.spread_sigma);
//			double amp = gaus_wrap1->Eval(last_proton);
//			prob->SetParameters(1 / amp, last_proton, pars.spread_sigma, 1 / amp, last_proton - 2 * M_PI, pars.spread_sigma, 1 / amp, last_proton + 2 * M_PI, pars.spread_sigma);
//			UNCOMMENT BELOW!!! Passing sim_rand only in new ROOT versions
//			group_angle = prob->GetRandom(0, 2 * M_PI, sim_rand) - (M_PI - last_proton);  // Generate at center then shift
			group_angle = fmod(group_angle, 2 * M_PI);  // Force to range [0, 2*pi)
			if (group_angle < 0) { group_angle += 2 * M_PI; }
			proton_angles.push_back(group_angle);
		}
		else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			proton_angles.push_back(new_angle);
		}
	}

	vector<Track> tracks;
	for (double& angle : proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event with multi-particle anti-clustering and return simulated proton angles.
void Simulator::sim_event_clust_multi(Event& event) {
	double new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	if (n_protons > 0) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		proton_angles.push_back(new_angle);
	}

	int wrap_num = ceil(pars.wrap_sigmas * pars.spread_sigma / (2 * M_PI));  // Wrap out to at least wrap_sigmas
	double x_range = pars.x_up - pars.x_low;
	vector<double> prob_vec(pars.points, 1);
	double x, x_val_up;
	while ((int)proton_angles.size() < n_protons) {
		vector<double> cdf(pars.points + 1, 0);
		int cdf_index = 0;
		for (int i=0; i<pars.points; i++) {
			x = pars.x_low + (i + 0.5) * x_range / pars.points;  // Generate prob points in middle of bins
			double prob_update = pars.base + pars.amp_group * gaus_kernel(x, new_angle, pars.spread_sigma);
			for (int wrap_i = 1; wrap_i <= wrap_num; wrap_i++) {
				prob_update += pars.amp_group * gaus_kernel(x, new_angle - 2 * M_PI * wrap_i, pars.spread_sigma) +  // wrap pdf to deal with
					pars.amp_group * gaus_kernel(x, new_angle + 2 * M_PI * wrap_i, pars.spread_sigma);  // periodic boundary
			}
			prob_vec[i] *= prob_update;
			cdf[cdf_index + 1] = cdf[cdf_index] + prob_vec[i];  // cdf[0] = 0
			cdf_index++;
		}

		double norm = cdf[pars.points];  // Last point is max, full integral
		for (double &x : cdf) {
			x /= norm;
		}

		double cdf_rand = sim_rand->Rndm();
		int i = 1;  // cdf[0] = 0 and rand > 0 so no need to check.
		while (cdf[i] < cdf_rand) { i++; }
		x_val_up = pars.x_low + i * x_range / pars.points;  // Upper bin edge
		new_angle = x_val_up - x_range / pars.points / (cdf[i] - cdf[i - 1]) * (cdf[i] - cdf_rand);  // Linear interpolation

		proton_angles.push_back(new_angle);
	}


	vector<Track> tracks;
	for (double& angle : proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event with multi-particle anti-clustering convoluted with elliptic flow and return simulated proton angles.
void Simulator::sim_event_clust_multi_flow(Event& event) {
	double new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	double reaction_plane = M_PI * sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
	if (event_plane < 0) { event_plane += M_PI; }
	event.set_event_plane(event_plane);
	//flow_res += cos(2 * (event_plane - reaction_plane));
	//flow_res_n++;

	if (n_protons > 0) {
		while ((int)proton_angles.size() == 0) {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			if (1.0 + 2 * pars.v2 * cos(2 * (new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(new_angle);
				}
			}
		}
	}

	int wrap_num = ceil(pars.wrap_sigmas * pars.spread_sigma / (2 * M_PI));  // Wrap out to at least wrap_sigmas
	double x_range = pars.x_up - pars.x_low;
	vector<double> prob_vec(pars.points, 1);
	double x, x_val_up;
	while ((int)proton_angles.size() < n_protons) {
		vector<double> cdf(pars.points + 1, 0);
		int cdf_index = 0;
		for (int i = 0; i < pars.points; i++) {
			x = pars.x_low + (i + 0.5) * x_range / pars.points;  // Generate prob points in middle of bins
			double prob_update = pars.base + pars.amp_group * gaus_kernel(x, new_angle, pars.spread_sigma);
			for (int wrap_i = 1; wrap_i <= wrap_num; wrap_i++) {
				prob_update += pars.amp_group * gaus_kernel(x, new_angle - 2 * M_PI * wrap_i, pars.spread_sigma) +  // wrap pdf to deal with
					pars.amp_group * gaus_kernel(x, new_angle + 2 * M_PI * wrap_i, pars.spread_sigma);  // periodic boundary
			}
			prob_vec[i] *= prob_update;
			cdf[cdf_index + 1] = cdf[cdf_index] + prob_vec[i];  // cdf[0] = 0
			cdf_index++;
		}

		double norm = cdf[pars.points];  // Last point is max, full integral
		for (double& x : cdf) {
			x /= norm;
		}

		bool new_angle_pass = false;
		while (!new_angle_pass) {
			double cdf_rand = sim_rand->Rndm();
			int i = 1;  // cdf[0] = 0 and rand > 0 so no need to check.
			while (cdf[i] < cdf_rand) { i++; }
			x_val_up = pars.x_low + i * x_range / pars.points;  // Upper bin edge
			new_angle = x_val_up - x_range / pars.points / (cdf[i] - cdf[i - 1]) * (cdf[i] - cdf_rand);  // Linear interpolation
			if (1.0 + 2 * pars.v2 * cos(2 * (new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
					new_angle_pass = true;
				}
			}
		}
		proton_angles.push_back(new_angle);
	}


	vector<Track> tracks;
	for (double& angle : proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event with multi-particle anti-clustering convoluted with elliptic flow and return simulated proton angles.
void Simulator::sim_event_clust_multi_flow_indep(Event& event) {
	double new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	double reaction_plane = M_PI * sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
	if (event_plane < 0) { event_plane += M_PI; }
	event.set_event_plane(event_plane);
	//flow_res += cos(2 * (event_plane - reaction_plane));
	//flow_res_n++;

	if (n_protons > 0) {
		while ((int)proton_angles.size() == 0) {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			if (1.0 + 2 * pars.v2 * cos(2 * (new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if (norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(new_angle);
				}
			}
		}
	}

	int wrap_num = ceil(pars.wrap_sigmas * pars.spread_sigma / (2 * M_PI));  // Wrap out to at least wrap_sigmas
	double x_range = pars.x_up - pars.x_low;
	vector<double> prob_vec(pars.points, 1);
	double x, x_val_up;
	while ((int)proton_angles.size() < n_protons) {
		vector<double> cdf(pars.points + 1, 0);
		int cdf_index = 0;
		for (int i = 0; i < pars.points; i++) {
			x = pars.x_low + (i + 0.5) * x_range / pars.points;  // Generate prob points in middle of bins
			double prob_update = pars.base + pars.amp_group * gaus_kernel(x, new_angle, pars.spread_sigma);
			for (int wrap_i = 1; wrap_i <= wrap_num; wrap_i++) {
				prob_update += pars.amp_group * gaus_kernel(x, new_angle - 2 * M_PI * wrap_i, pars.spread_sigma) +  // wrap pdf to deal with
					pars.amp_group * gaus_kernel(x, new_angle + 2 * M_PI * wrap_i, pars.spread_sigma);  // periodic boundary
			}
			prob_vec[i] *= prob_update;
			cdf[cdf_index + 1] = cdf[cdf_index] + prob_vec[i];  // cdf[0] = 0
			cdf_index++;
		}

		double norm = cdf[pars.points];  // Last point is max, full integral
		for (double& x : cdf) {
			x /= norm;
		}

		double cdf_rand = sim_rand->Rndm();
		int i = 1;  // cdf[0] = 0 and rand > 0 so no need to check.
		while (cdf[i] < cdf_rand) { i++; }
		x_val_up = pars.x_low + i * x_range / pars.points;  // Upper bin edge
		new_angle = x_val_up - x_range / pars.points / (cdf[i] - cdf[i - 1]) * (cdf[i] - cdf_rand);  // Linear interpolation
		if (1.0 + 2 * pars.v2 * cos(2 * (new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
			if (norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
				proton_angles.push_back(new_angle);
			}
		}
	}

	vector<Track> tracks;
	for (double& angle : proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event with multi-particle anti-clustering and return simulated proton angles.
void Simulator::sim_event_clust_multi_plus_minus(Event& event) {
	double new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	if (n_protons > 0) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		proton_angles.push_back(new_angle);
	}

	int wrap_num = ceil(pars.wrap_sigmas * pars.spread_sigma_minus / (2 * M_PI));  // Wrap out to at least wrap_sigmas, anti-correlation should be wider
	double x_range = pars.x_up - pars.x_low;
	vector<double> prob_vec(pars.points, 1);
	double x, x_val_up;
	while ((int)proton_angles.size() < n_protons) {
		vector<double> cdf(pars.points + 1, 0);
		int cdf_index = 0;
		for (int i = 0; i < pars.points; i++) {
			x = pars.x_low + (i + 0.5) * x_range / pars.points;  // Generate prob points in middle of bins
			double prob_update = pars.base + pars.amp_group_plus * gaus_kernel(x, new_angle, pars.spread_sigma_plus) + 
				pars.amp_group_minus * gaus_kernel(x, new_angle, pars.spread_sigma_minus);
			for (int wrap_i = 1; wrap_i <= wrap_num; wrap_i++) {
				prob_update += pars.amp_group_plus * gaus_kernel(x, new_angle - 2 * M_PI * wrap_i, pars.spread_sigma_plus) +  // wrap pdf to deal with
					pars.amp_group_plus * gaus_kernel(x, new_angle + 2 * M_PI * wrap_i, pars.spread_sigma_plus);  // periodic boundary for positive correlation
				prob_update += pars.amp_group_minus * gaus_kernel(x, new_angle - 2 * M_PI * wrap_i, pars.spread_sigma_minus) +  // wrap pdf to deal with
					pars.amp_group_minus * gaus_kernel(x, new_angle + 2 * M_PI * wrap_i, pars.spread_sigma_minus);  // periodic boundary for negative correlation
			}
			prob_vec[i] *= prob_update;
			cdf[cdf_index + 1] = cdf[cdf_index] + prob_vec[i];  // cdf[0] = 0
			cdf_index++;
		}

		double norm = cdf[pars.points];  // Last point is max, full integral
		for (double& x : cdf) {
			x /= norm;
		}

		double cdf_rand = sim_rand->Rndm();
		int i = 1;  // cdf[0] = 0 and rand > 0 so no need to check.
		while (cdf[i] < cdf_rand) { i++; }
		x_val_up = pars.x_low + i * x_range / pars.points;  // Upper bin edge
		new_angle = x_val_up - x_range / pars.points / (cdf[i] - cdf[i - 1]) * (cdf[i] - cdf_rand);  // Linear interpolation

		proton_angles.push_back(new_angle);
	}


	vector<Track> tracks;
	for (double& angle : proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event with multi-particle anti-clustering as sim_event_clust_multi. Use final pdf to genrate and return simulated proton angles.
// If I remember, we reasoned that using any single pdf will only generate clustering. DON'T USE FOR ANTI-CLUSTERING
void Simulator::sim_event_clust_final(Event& event) {
	double new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	if (n_protons > 0) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		proton_angles.push_back(new_angle);
	}

	int wrap_num = ceil(pars.wrap_sigmas * pars.spread_sigma / (2 * M_PI));  // Wrap out to at least wrap_sigmas
	double x_range = pars.x_up - pars.x_low;
	vector<double> prob_vec(pars.points, 1);
	double x, x_val_up;
	while ((int)proton_angles.size() < n_protons) {
		vector<double> cdf(pars.points + 1, 0);
		int cdf_index = 0;
		for (int i = 0; i < pars.points; i++) {
			x = pars.x_low + (i + 0.5) * x_range / pars.points;  // Generate prob points in middle of bins
			double prob_update = pars.base + pars.amp_group * gaus_kernel(x, new_angle, pars.spread_sigma);
			for (int wrap_i = 1; wrap_i <= wrap_num; wrap_i++) {
				prob_update += pars.amp_group * gaus_kernel(x, new_angle - 2 * M_PI * wrap_i, pars.spread_sigma) +  // wrap pdf to deal with
					pars.amp_group * gaus_kernel(x, new_angle + 2 * M_PI * wrap_i, pars.spread_sigma);  // periodic boundary
			}
			prob_vec[i] *= prob_update;
			cdf[cdf_index + 1] = cdf[cdf_index] + prob_vec[i];  // cdf[0] = 0
			cdf_index++;
		}

		double norm = cdf[pars.points];  // Last point is max, full integral
		for (double& x : cdf) {
			x /= norm;
		}

		double cdf_rand = sim_rand->Rndm();
		int i = 1;  // cdf[0] = 0 and rand > 0 so no need to check.
		while (cdf[i] < cdf_rand) { i++; }
		x_val_up = pars.x_low + i * x_range / pars.points;  // Upper bin edge
		new_angle = x_val_up - x_range / pars.points / (cdf[i] - cdf[i - 1]) * (cdf[i] - cdf_rand);  // Linear interpolation

		proton_angles.push_back(new_angle);
	}

	// Use final pdf to distribute particles
	vector<double> cdf(pars.points + 1, 0);
	int cdf_index = 0;
	for (int i = 0; i < pars.points; i++) {
		x = pars.x_low + (i + 0.5) * x_range / pars.points;  // Generate prob points in middle of bins
		cdf[cdf_index + 1] = cdf[cdf_index] + prob_vec[i];  // cdf[0] = 0
		cdf_index++;
	}

	double norm = cdf[pars.points];  // Last point is max, full integral
	for (double& x : cdf) {
		x /= norm;
	}
	
	proton_angles.clear();  // Clear protons placed in previous algorithm
	while ((int)proton_angles.size() < n_protons) {
		double cdf_rand = sim_rand->Rndm();
		int i = 1;  // cdf[0] = 0 and rand > 0 so no need to check.
		while (cdf[i] < cdf_rand) { i++; }
		x_val_up = pars.x_low + i * x_range / pars.points;  // Upper bin edge
		new_angle = x_val_up - x_range / pars.points / (cdf[i] - cdf[i - 1]) * (cdf[i] - cdf_rand);  // Linear interpolation

		proton_angles.push_back(new_angle);
	}	


	vector<Track> tracks;
	for (double& angle : proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event with particles produced in back to back pairs and return simulated proton angles.
void Simulator::sim_event_pairs(Event &event) {
	double group_angle, new_angle, pair_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		proton_angles.push_back(new_angle);
		pair_angle = new_angle + M_PI;
		pair_angle = fmod(pair_angle, 2*M_PI);  // Force to range [0, 2*pi)
		proton_angles.push_back(pair_angle);
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			proton_angles.push_back(group_angle);
			pair_angle = group_angle + M_PI;
			pair_angle = fmod(pair_angle, 2*M_PI);  // Force to range [0, 2*pi)
			proton_angles.push_back(pair_angle);
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			proton_angles.push_back(new_angle);
			pair_angle = new_angle + M_PI;
			pair_angle = fmod(pair_angle, 2*M_PI);  // Force to range [0, 2*pi)
			proton_angles.push_back(pair_angle);
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event and return simulated proton angles. Include efficiency into simulation.
void Simulator::sim_event_eff(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

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

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event and return simulated proton angles. Include elliptic flow into simulation.
void Simulator::sim_event_eff_flow(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	double reaction_plane = M_PI*sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
	if(event_plane < 0) { event_plane += M_PI; }
	event.set_event_plane(event_plane);
	//flow_res += cos(2*(event_plane - reaction_plane));
	//flow_res_n++;

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
	event.set_particles(tracks);
}



// Simulate single event and return simulated proton angles. Include elliptic flow only, no (anti)clustering effects.
void Simulator::sim_event_flow(Event& event) {
	double new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	double reaction_plane = M_PI * sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
	if (event_plane < 0) { event_plane += M_PI; }
	event.set_event_plane(event_plane);
	flow_res += cos(2 * (event_plane - reaction_plane));
	flow_res_n++;

	while ((int)proton_angles.size() < n_protons) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		if (1.0 + 2 * pars.v2 * cos(2 * (new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
			proton_angles.push_back(new_angle);
		}
	}

	vector<Track> tracks;
	for (double& angle : proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event and return simulated proton angles. Include efficiency and simple clustering.
void Simulator::sim_event_eff_simple_clust(Event& event) {
	double new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();
	double cluster_angle = 2 * M_PI * sim_rand->Rndm();

	double reaction_plane = M_PI * sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
	if (event_plane < 0) { event_plane += M_PI; }
	event.set_event_plane(event_plane);

	while ((int)proton_angles.size() < n_protons) {
		if (sim_rand->Rndm() < pars.amp_group) {  // Cluster
			new_angle = sim_rand->Gaus(cluster_angle, pars.spread_sigma);
			new_angle = fmod(new_angle, 2 * M_PI);  // Force to range [0, 2*pi)
			if (new_angle < 0) { new_angle += 2 * M_PI; }
		}
		else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
		}
		if (norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
			if (1.0 + 2 * pars.v2 * cos(2 * (new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				proton_angles.push_back(new_angle);
			}
		}
	}

	vector<Track> tracks;
	for (double& angle : proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event and return simulated proton angles. Include elliptic flow into old clustering model.
// DEPRECATED
void Simulator::sim_event_flow_old_model(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	double reaction_plane = M_PI*sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
	if(event_plane < 0) { event_plane += M_PI; }
	event.set_event_plane(event_plane);
	flow_res += cos(2*(event_plane - reaction_plane));
	flow_res_n++;

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
	event.set_particles(tracks);

}



// Simulate single event and return simulated proton angles. Include elliptic flow into simulation.
void Simulator::sim_event_eff_new(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

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
	event.set_particles(tracks);

}


int Simulator::get_protons() {
	int n = 0;
	if (pars.proton_dist == "hist") {
		n = proton_dist_hist->GetRandom();
	}else if (pars.proton_dist == "poisson") {
		n = sim_rand->Poisson(pars.particle_mean);
	} else if (pars.proton_dist == "flat") {
		n = (int)(sim_rand->Rndm() * (pars.particle_max - pars.particle_min + 1)) + pars.particle_min;
	} else if (pars.proton_dist == "single") {
		n = (int)pars.particle_mean;
	}

	return n;
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
