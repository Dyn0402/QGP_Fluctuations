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

#include <TH1.h>
#include <TRandom3.h>

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
	vector<double> simulate_event();

	// Getters
	int get_n_events();
	int get_divisions();
	double get_p_group();
	double get_spread_sigma();
	int get_num_event_mix();
	TH1D* get_two_p_corr();

	// Setters
	void set_n_events(int n);
	void set_divisions(int divs);
	void set_p_group(double p_group);
	void set_spread_sigma(double sig);
	void set_min_protons(int protons);
	void set_proton_dist(string dist);
	void set_particle_mean(double mean);
	void set_proton_dist_hist(TH1D *hist);
	void set_num_event_mix(int num);


private:
	double two_p_shift = M_PI / 4;
	int two_p_bins = 100;

	simulation_pars pars;
	TRandom3 *sim_rand;
	TH1D *proton_dist_hist;
	TH1D *two_p_corr;

	int get_protons();

};


#endif /* SIMULATOR_H_ */
