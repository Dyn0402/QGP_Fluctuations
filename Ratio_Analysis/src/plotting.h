/*
 * plotting.h
 *
 *  Created on: Jun 18, 2019
 *      Author: Dylan Neff
 */

#ifndef PLOTTING_H_
#define PLOTTING_H_

#include <vector>
#include <tuple>
#include <string>

#include <TFile.h>
#include <TGraphErrors.h>

#include "ratio_methods.h"

using namespace std;

void make_ratio_dist_plots(TFile *out_root, map<int, tree_data> data);
void hist_ratio_dist(vector<double> ratios, int energy, int div, int cent, string mode="write");
void make_proton_dist_plots(TFile *out_root, map<int, tree_data> data);
void hist_proton_dist(vector<int> nprotons, int energy, int cent, string mode="write");

void make_cumulant_plots(TFile *out_root, map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants);
void graph_cumulant_vs_energy(map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants, int div, int cent, int order);
void graph_cumulant_vs_divs(map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants, int energy, int cent, int order);
void graph_x_vs_y(vector<int> x, vector<double> y, string name);
TGraphErrors* graph_x_vs_y_err(vector<double> x, vector<double> y, vector<double> x_err={}, vector<double> y_err={});

void make_canvas_plots(TFile *out_root, map<int, tree_data> data, map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants);
void canvas_nprotons(map<int, tree_data> data, int cent, string name);
void canvas_ratio_dists(map<int, tree_data> data, int div, int cent, string name);
void canvas_cumulant_dists(map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants, int order, int cent, string name);


#endif /* PLOTTING_H_ */
