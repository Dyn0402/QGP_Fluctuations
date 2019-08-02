/*
 * moments_vs_sim_pars.h
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */

#ifndef MOMENTS_VS_SIM_PARS_H_
#define MOMENTS_VS_SIM_PARS_H_

#include <string>
#include <map>

#include <TFile.h>

#include "RatioData.h"
#include "Measure.h"

using namespace std;


void moments_vs_sim_pars(int div);
void run_pars(int n_events, double mean, double p_eff, double p_clust, double spread, int div, TFile *out_file, map<double, map<double, map<string, Measure>>> *results);
RatioData run_sim(double p_eff, double p_clust, double mean, double n_events, double spread, int div);
void get_moments(vector<double> ratios, map<string, Measure> *results);



#endif /* MOMENTS_VS_SIM_PARS_H_ */
