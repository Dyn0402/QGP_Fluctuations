/*
 * sim_v1.h
 *
 *  Created on: Mar 14, 2019
 *      Author: dylan
 */

#ifndef SIM_V1_H_
#define SIM_V1_H_

#include <vector>

using namespace std;


void sim_v1();

vector<double> simulate();
void simulate_2pcorr();

vector<double> simulate_event();

int get_n_protons();

void plot(vector<double> Rs);
void plot_corrs(vector<double> corrs);


#endif /* SIM_V1_H_ */
