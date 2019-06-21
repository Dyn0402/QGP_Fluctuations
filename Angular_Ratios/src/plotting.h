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

void make_cumulant_plots(TFile *out_root, map<int, map<int, map<int, map<int, double>>>> cumulants);
void graph_cumulant_vs_energy(map<int, map<int, map<int, map<int, double>>>> cumulants, int div, int cent, int order);
void graph_cumulant_vs_divs(map<int, map<int, map<int, map<int, double>>>> cumulants, int energy, int cent, int order);
void graph_x_vs_y(vector<int> x, vector<double> y);


#endif /* PLOTTING_H_ */
