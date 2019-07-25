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

using namespace std;


struct simulation_pars {
		int n_events = 1e6;
		int divisions = 6;

		double particle_mean = 20.0;
		double p_effect = 0.2;
		double p_cluster = 0.5;

		double spread_sigma = 0.0;
		int min_protons = 2;
	};


class Simulator {
public:
	Simulator();

	simulation_pars pars;

private:
	string out_path = "/home/dylan/local_server/dyn0402/Research/Simulation/07-18_athic_comp.root";
	//	string out_path = "/home/dylan/Research/Roli_Sim_Data/comp_test_dylan.root";

	string param = "p_cluster";
	vector<double> param_list = {0.0, 0.05, 0.10, 0.15, 0.20};
	vector<int> cumulant_orders = {1, 2, 3, 4};
	vector<string> statistics = {"mean", "standard_deviation", "skewness", "kurtosis"};

	double can_w = 1080;
	double can_h = 1080;
};


#endif /* SIMULATOR_H_ */
