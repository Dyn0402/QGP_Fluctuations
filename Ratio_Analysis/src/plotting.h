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

#include "ratio_methods.h"

using namespace std;

void make_ratio_dist_plots(TFile *out_root, map<int, tree_data> data);
void hist_ratio_dist(vector<double> ratios, int energy, int div, int cent, string mode="write");
void make_proton_dist_plots(TFile *out_root, map<int, tree_data> data);
void hist_proton_dist(vector<int> nprotons, int energy, int cent, string mode="write");

void make_cumulant_plots(TFile *out_root, map<int, map<int, map<int, map<int, double>>>> cumulants);
void graph_cumulant_vs_energy(map<int, map<int, map<int, map<int, double>>>> cumulants, int div, int cent, int order);
void graph_cumulant_vs_divs(map<int, map<int, map<int, map<int, double>>>> cumulants, int energy, int cent, int order);
void graph_x_vs_y(vector<int> x, vector<double> y, string name);

void make_canvas_plots(TFile *out_root, map<int, tree_data> data);
void canvas_nprotons(map<int, tree_data> data, int cent, string name);
void canvas_ratio_dists(map<int, tree_data> data, int div, int cent, string name);


#endif /* PLOTTING_H_ */
