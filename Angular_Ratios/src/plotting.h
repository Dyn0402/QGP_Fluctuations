/*
 * plotting.h
 *
 *  Created on: Jun 18, 2019
 *      Author: Dylan Neff
 */

#ifndef PLOTTING_H_
#define PLOTTING_H_

#include <vector>
#include <string>

#include <TFile.h>

using namespace std;

void make_ratio_dist_plots(TFile *out_root, map<int, tree_data> data);
void hist_ratio_dist(vector<double> ratios, int energy, int div, int cent);
void make_proton_dist_plots(TFile *out_root, map<int, tree_data> data);
void hist_proton_dist(vector<int> nprotons, int energy, int cent);



#endif /* PLOTTING_H_ */
