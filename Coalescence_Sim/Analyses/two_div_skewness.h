/*
 * two_div_skewness.h
 *
 *  Created on: Aug 5, 2019
 *      Author: dylan
 */

#ifndef TWO_DIV_SKEWNESS_H_
#define TWO_DIV_SKEWNESS_H_

#include <AzimuthBinData.h>
#include <string>
#include <map>

#include <TFile.h>

#include "Measure.h"

using namespace std;

void two_div_skewness();

namespace two_div_skew {
	void run_pars(int n_events, double mean, double p_eff, double p_clust, double spread, int div, TFile *out_file, map<double, map<double, map<string, Measure>>> *results);
	RatioData run_sim(double p_eff, double p_clust, double mean, double n_events, double spread, int div);
	void get_moments(map<double, int> ratios, map<string, Measure> *results);

	void run_pars_mixed(int n_events, double mean, double p_eff, double p_clust, double spread, int div, TFile *out_file, map<double, map<double, map<string, Measure>>> *results);
	vector<RatioData> run_sim_mixed(double p_eff, double p_clust, double mean, double n_events, double spread, int div);
	void get_moments_mixed(vector<vector<double>> ratios, map<string, Measure> *results);
}




#endif /* TWO_DIV_SKEWNESS_H_ */
