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


void moments_vs_sim_pars2(int div);

void run_pars2(int n_events, double mean, double p_group, double spread, int div, TDirectory *out_file, map<double, map<double, map<string, Measure>>> *results);
RatioData run_sim2(double p_group, double mean, double n_events, double spread, int div);
void get_moments2(map<double, int> ratios, map<string, Measure> *results);

void run_pars_mixed2(int n_events, double mean, double p_group, double spread, int div, TDirectory *out_file, map<double, map<double, map<string, Measure>>> *results);
vector<RatioData> run_sim_mixed2(double p_group, double mean, double n_events, double spread, int div);
void get_moments_mixed2(vector<map<double, int>> ratios, map<string, Measure> *results);



#endif /* MOMENTS_VS_SIM_PARS_H_ */
