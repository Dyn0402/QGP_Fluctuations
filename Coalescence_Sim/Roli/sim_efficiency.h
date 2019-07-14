/*
 * sim_efficiency.h
 *
 *  Created on: Jul 12, 2019
 *      Author: Dylan Neff
 */

#ifndef SIM_EFFICIENCY_H_
#define SIM_EFFICIENCY_H_

#include <TH1.h>
#include <TString.h>

int getPhiBin(double, int);
TH1D* sim_efficiency(double pc, double xcluster, double mean, int divs, int events);



#endif /* SIM_EFFICIENCY_H_ */
