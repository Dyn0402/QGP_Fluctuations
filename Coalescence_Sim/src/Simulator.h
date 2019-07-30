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

		double p_effect = 0.2;
		double p_cluster = 0.5;
		double spread_sigma = 0.0;

		int min_protons = 2;

		string proton_dist = "poisson";
		double particle_mean = 20.0;
	};


class Simulator {
public:
	Simulator();

	// Doers
	tree_data run_simulation();
	vector<tree_data> run_simulation_mixed();

	// Getters
	int get_n_events();
	int get_divisions();
	double get_p_effect();
	double get_p_cluster();
	double get_spread_sigma();

	// Setters
	void set_n_events(int n);
	void set_divisions(int divs);
	void set_p_effect(double p_eff);
	void set_p_cluster(double p_clust);
	void set_spread_sigma(double sig);
	void set_min_protons(int protons);
	void set_proton_dist(string dist);
	void set_particle_mean(double mean);
	void set_proton_dist_hist(TH1D *hist);

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

	vector<double> simulate_event();
	int get_protons();

};


#endif /* SIMULATOR_H_ */
