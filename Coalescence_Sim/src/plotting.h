/*
 * plotting.h
 *
 *  Created on: Jul 9, 2019
 *      Author: Dylan Neff
 */

#ifndef PLOTTING_H_
#define PLOTTING_H_

#include <vector>
#include <string>
#include <map>

#include <TFile.h>

using namespace std;

void plot_Rs(vector<double> Rs);
void plot_corrs(vector<double> corrs);
void plot_cumulants_vs_peffect(map<double, map<int, tuple<double, double>>> cumulants_with_peffect);

void hist_ratio_dist(vector<double> ratios, string mode="write");
void hist_proton_dist(vector<int> nprotons, string mode="write");


#endif /* PLOTTING_H_ */
