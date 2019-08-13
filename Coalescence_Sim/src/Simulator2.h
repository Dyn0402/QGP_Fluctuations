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
	vector<map<int, map<int, int>>> run_sim_mixed_2p();
	void write_two_p_corr();

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
//	string out_path = "/home/dylan/local_server/dyn0402/Research/Simulation/07-18_athic_comp.root";
	//	string out_path = "/home/dylan/Research/Roli_Sim_Data/comp_test_dylan.root";

//	string param = "p_cluster";
//	vector<double> param_list = {0.0, 0.05, 0.10, 0.15, 0.20};
//	vector<int> cumulant_orders = {1, 2, 3, 4};
//	vector<string> statistics = {"mean", "standard_deviation", "skewness", "kurtosis"};
//
//	double can_w = 1080;
//	double can_h = 1080;

	simulation_pars pars;
	TRandom3 *rand;
	TH1D *proton_dist_hist;
	TH1D *two_p_corr;

	vector<double> simulate_event();
	int get_protons();

};


#endif /* SIMULATOR_H_ */
