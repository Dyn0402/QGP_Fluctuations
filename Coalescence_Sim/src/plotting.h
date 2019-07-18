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
#include <TH1.h>

#include "Stats.h"

using namespace std;

void plot_Rs(vector<double> Rs);
void plot_corrs(vector<double> corrs);
void plot_stats_vs_x(map<double, map<string, measure<double>>> stats, string x_name = "x");
void plot_cumulants_vs_x(map<double, map<int, measure<double>>> cumulants, string x_name = "x");

void hist_ratio_dist(vector<double> ratios, string name = "ratio dist", string mode="write");
void hist_proton_dist(vector<int> nprotons, string name = "proton dist", string mode="write");

void comp_hists(TH1D *hist1, TH1D *hist2, string can_name);

TH1D* hist_ratios(vector<double> ratios);


#endif /* PLOTTING_H_ */
