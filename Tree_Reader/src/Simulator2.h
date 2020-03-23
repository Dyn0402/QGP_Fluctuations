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

#include <ratio_methods.h>

using namespace std;


struct simulation_pars {
		int n_events = 1e6;
		int divisions = 6;

		double p_group = 0.05;
		double spread_sigma = 0.0;

		int min_protons = 2;

		string proton_dist = "poisson";
		double particle_mean = 20.0;

		int num_event_mix = 1000;
	};


class Simulator2 {
public:
	Simulator2();
	~Simulator2();

	// Doers
	map<int, map<int, int>> run_simulation();
	vector<map<int, map<int, int>>> run_simulation_mixed();
	vector<map<int, map<int, int>>> run_sim_mixed_2p(string two_p_name);
	void write_two_p_corr();
	vector<double> sim_event();
	vector<double> sim_event_eff();
	vector<double> sim_event_eff2();
	function<vector<double>()> simulate_event;
	double wrap_gaus(double x, double mu, double sigma, double lower_bound, double upper_bound);


	// Getters
	int get_n_events();
	int get_divisions();
	double get_p_group();
	double get_spread_sigma();
	int get_num_event_mix();
	TH1D* get_two_p_corr();
	TH1D* get_efficiency_dist();

	// Setters
	void set_n_events(int n);
	void set_divisions(int divs);
	void set_p_group(double p_group);
	void set_spread_sigma(double sig);
	void set_min_protons(int protons);
	void set_proton_dist(string dist);
	void set_particle_mean(double mean);
	void set_proton_dist_hist(TH1D *hist);
	void set_efficiency_dist_hist(TH1D *hist);
	void set_efficiency_dist_hist(string root_path, string hist_name);
	void set_num_event_mix(int num);
	void set_hom_eff(double eff);


private:
	double two_p_shift = M_PI / 4;
	int two_p_bins = 100;
	double hom_eff = 1.0;

	simulation_pars pars;
	TRandom3 *sim_rand;
	TH1D *proton_dist_hist;
	TH1D *efficiency_dist;
	TH1D *norm_eff_dist;
	TH1D *two_p_corr;

	int get_protons();
	double get_group_angle(double center);

};


#endif /* SIMULATOR_H_ */
