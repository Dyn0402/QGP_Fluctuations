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

int get_n_protons();

vector<double> get_Rs(vector<double> angles, int divisions);

void plot(vector<double> Rs);


#endif /* SIM_V1_H_ */
