/*
 * config.h
 *
 *  Created on: Mar 14, 2019
 *      Author: dylan
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <vector>
#include <string>

#include <TFile.h>
#include <TRandom3.h>
#include <TH1D.h>

using namespace std;


namespace config {
	struct simulation_pars {
		int n_events = 1e6;
		int divisions = 6;

		double particle_mean = 20.0;
		double p_effect = 0.2;
		double p_cluster = 0.5;

		double spread_sigma = 0.0;
		int min_protons = 2;
	};

	extern string out_path;

	extern string param;
	extern vector<double> param_list;
	extern vector<string> moments;
	extern vector<int> cumulant_orders;

	extern double can_w;
	extern double can_h;
}



#endif /* CONFIG_H_ */
