/*
 * sim_v1.h
 *
 *  Created on: Mar 14, 2019
 *      Author: dylan
 */

#ifndef SIM_V1_H_
#define SIM_V1_H_

#include <vector>

#include "ratio_methods.h"

using namespace std;


tree_data sim_v1();

tree_data simulate_Rs();
void simulate_2pcorr();

vector<double> simulate_event();

int get_n_protons();


#endif /* SIM_V1_H_ */
