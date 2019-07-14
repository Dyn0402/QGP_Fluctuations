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
		int n_events = 1e7;
		int divisions = 3;

		double particle_mean = 10.0;
		double p_effect = 0.1;
		double p_cluster = 0.1;

		double spread_sigma = 0.0;
		int min_protons = 2;
	};

	extern string out_path;

	extern vector<int> cumulant_orders;
	extern map<int, string> cumulant_name;
}



#endif /* CONFIG_H_ */
