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
	extern string out_path;

	extern int n_events;
	extern int particle_mean;
	extern int divisions;

	extern double p_effect;
	extern double p_cluster;

	extern double spread_sigma;

	extern TRandom3 *rand;

	extern vector<int> cumulant_orders;
	extern map<int, string> cumulant_name;

	extern TH1D *R_hist;
	extern TH1D *corr_hist;

//	extern TFile *out_file;
}



#endif /* CONFIG_H_ */
