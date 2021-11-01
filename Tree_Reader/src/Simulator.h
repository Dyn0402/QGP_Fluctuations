/*
 * Simulator.h
 *
 *  Created on: Jul 24, 2019
 *      Author: dylan
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_


#include <vector>
#include <string>
#include <functional>

#include <thread>
#include <chrono>

#include <TFile.h>
#include <TH1.h>
#include <TRandom3.h>
#include <TMath.h>

#include "Event.h"
#include "Track.h"
#include "ratio_methods.h"

using namespace std;


struct simulation_pars {
	double p_group = 0.05;
	double spread_sigma = 0.0;

	int min_protons = 2;

	string proton_dist = "poisson";  // poisson, flat, hist, single
	double particle_mean = 20.0;  // For Poisson and single
	int particle_max = 20;  // For flat distributuion

	double hom_eff = 1.0;

	double v2 = 0.0;
	double ep_res = 0.0;
};


class Simulator {
public:
	// Structors
	Simulator();
	~Simulator();

	// Doers
	function<void(Event&)> simulate_event;
	double wrap_gaus(double x, double mu, double sigma, double lower_bound, double upper_bound);

	void sim_event(Event &event);
	void sim_event_anticlust(Event& event);
	void sim_event_pairs(Event &event);
	void sim_event_eff(Event &event);
	void sim_event_eff_flow(Event &event);
	void sim_event_flow(Event &event);
	void sim_event_eff_new(Event &event);

	static double event_plane_gang(double res, double psi);
	static double event_plane(double res, double psi, double acc = 0.001);
	static double chi_gang(double res);
	static double chi(double res, double acc = 0.001);
	static double res_event_plane(double chi);

	// Getters
	double get_p_group();
	double get_spread_sigma();
	string get_proton_dist_type();
	TH1D* get_efficiency_dist();
	simulation_pars get_sim_pars();

	// Setters
	void set_p_group(double p_group);
	void set_spread_sigma(double sig);
	void set_min_protons(int protons);
	void set_proton_dist(string dist);
	void set_particle_mean(double mean);
	void set_particle_max(int max);
	void set_flat_dist(int max = 0);
	void set_proton_dist_hist(TH1D *hist);
	void set_efficiency_dist_hist(TH1D *hist);
	void set_efficiency_dist_hist(string root_path, string hist_name);
	void set_eff();
	void set_flow();
	void set_eff_flow();
	void set_anti_clust();
	void set_hom_eff(double eff);
	void set_v2(double v2);
	void set_ep_res(double res);
	void set_flow(double v2, double res, double chi_acc = 0.0001, int event_plane_n = 2);
	void set_job_energy(int num);

	// Attributes
	track_defaults track_defs;

private:
	// Attributes
	double flow_res = 0.;
	int flow_res_n = 0;
	int job_energy = 0;  // Unique number for TF1 names

	simulation_pars pars;
	TRandom3 *sim_rand;
	TH1D *proton_dist_hist;
	TH1D *efficiency_dist;
	TH1D *norm_eff_dist;
	TH1D *ep_dist;

	TF1 *gaus_wrap1;
	TF1 *prob;

	// Doers
	int get_protons();
	double get_group_angle(double center);

};


#endif /* SIMULATOR_H_ */
