/*
 * sim_v1.h
 *
 *  Created on: Mar 14, 2019
 *      Author: dylan
 */

#ifndef SIM_V1_H_
#define SIM_V1_H_

#include <vector>

#include <TRandom3.h>

#include "ratio_methods.h"
#include "config.h"

using namespace std;


tree_data sim_v1(config::simulation_pars pars);

tree_data simulate_Rs(config::simulation_pars pars);
void simulate_2pcorr(config::simulation_pars pars);

vector<double> simulate_event(TRandom3* rand, config::simulation_pars pars);

int get_n_protons(TRandom3* rand, double mean);


#endif /* SIM_V1_H_ */
