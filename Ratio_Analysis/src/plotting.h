/*
 * plotting.h
 *
 *  Created on: Jun 18, 2019
 *      Author: Dylan Neff
 */

#ifndef PLOTTING_H_
#define PLOTTING_H_

#include <AzimuthBinData.h>
#include <vector>
#include <tuple>
#include <map>
#include <string>

#include <TFile.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TLine.h>

#include "ratio_methods.h"
#include "Stats.h"

using namespace std;

void make_ratio_dist_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void make_ratio_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void create_ratio_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void hist_ratio_dist(map<int, map<int, int>> ratios, int energy, int div, int cent, string mode="write", int color=4);
void hist_diff_dist(map<double, int> diffs, int energy, int div, int cent, string mode="write", int color=4);

void make_diff_dist_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void make_diff_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void create_diff_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);

void make_2d_dist_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void make_2d_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void create_2d_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void canvas_2d_dist(map<int, map<int, int>> dist, int energy, int div, int cent);

void make_proton_dist_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void make_proton_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void create_proton_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data);
void hist_proton_dist(map<int, int> nprotons, int energy, int cent, string mode="write");

void make_cumulant_plots(TFile *out_root, map<int, map<int, map<int, map<int, Measure>>>> cumulants);
void make_cumulant_plots(TDirectory *out_root, map<int, map<int, map<int, map<int, Measure>>>> cumulants);
void create_cumulant_plots(TDirectory *out_root, map<int, map<int, map<int, map<int, Measure>>>> cumulants);
void graph_cumulant_vs_energy(map<int, map<int, map<int, map<int, Measure>>>> cumulants, int div, int cent, int order);
void graph_cumulant_vs_divs(map<int, map<int, map<int, map<int, Measure>>>> cumulants, int energy, int cent, int order);

void make_stat_plots(TFile *out_root, map<int, map<int, map<int, map<string, Measure>>>> stats);
void make_stat_plots(TDirectory *out_root, map<int, map<int, map<int, map<string, Measure>>>> stats);
void create_stat_plots(TDirectory *out_root, map<int, map<int, map<int, map<string, Measure>>>> stats);
void graph_stat_vs_energy(map<int, map<int, map<int, map<string, Measure>>>> ststs, int div, int cent, string stat);
void graph_stat_vs_divs(map<int, map<int, map<int, map<string, Measure>>>> stats, int energy, int cent, string stat);

void graph_x_vs_y(vector<int> x, vector<double> y, string name);
TGraphErrors* graph_x_vs_y_err(vector<double> x, vector<double> y, vector<double> x_err={}, vector<double> y_err={});

void make_canvas_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data, vector<int> cents, vector<int> divs);
void make_canvas_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data, vector<int> cents, vector<int> divs);
void create_canvas_plots(TDirectory *can_dir, map<int, map<int, map<int, AzimuthBinData>>> data, vector<int> cents, vector<int> divs);
void canvas_nprotons(map<int, map<int, map<int, AzimuthBinData>>> data, int cent, string name);
void canvas_ratio_dists(map<int, map<int, map<int, AzimuthBinData>>> data, int div, int cent, string name);
void canvas_ratio_dists(map<int, map<int, map<int, AzimuthBinData>>> data, vector<int> div, vector<int> cent, string name);
void canvas_cumulant_dists(map<int, map<int, map<int, map<int, Measure>>>> cumulants, int order, int cent, string name);
void canvas_stat_dists(map<int, map<int, map<int, map<string, Measure>>>> stats, string stat, int cent, string name);
void canvas_diff_dists(TDirectory *can_dir, map<string, map<int, map<int, map<int, AzimuthBinData>>>> data, vector<int> div, vector<int> cent, string name);
void canvas_diff_dists(map<int, map<int, map<int, AzimuthBinData>>> data, int div, int cent, string name);

void athic_stat_vs_centrality(map<int, map<int, map<int, map<string, Measure>>>> stats, string name);
void athic_stat_vs_energy(map<int, map<int, map<int, map<string, Measure>>>> stats, string name);

void roli_thesis_stats(map<int, map<int, map<int, map<string, Measure>>>> stats, vector<string> stat_names, vector<int> cents, vector<int> divs, string name);
void roli_thesis_stats(map<int, map<int, map<int, map<string, Measure>>>> stats, map<int, map<int, map<int, map<string, double>>>> sys, vector<string> stat_names, vector<int> cents, vector<int> divs, string name);
void roli_thesis_stats(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, vector<string> stat_names, vector<int> cents, vector<int> divs, string name);
void roli_thesis_stats(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, vector<string> stat_names, vector<int> cents, vector<int> divs, string name);

void centralities_stat(map<int, map<int, map<int, map<string, Measure>>>> stats, string stat_name, vector<int> cents, vector<int> divs, string name);
void centralities_stat(map<int, map<int, map<int, map<string, Measure>>>> stats, map<int, map<int, map<int, map<string, double>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name);
void centralities_stat(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name);
void centralities_stat(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, string stat_name, vector<int> cents, vector<int> divs, string name);

void centralities_stat2(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name);

void division_stat(map<int, map<int, map<int, map<string, Measure>>>> stats, string stat_name, vector<int> cents, vector<int> divs, vector<int> energies, string name);
void division_stat(map<int, map<int, map<int, map<string, Measure>>>> stats, map<int, map<int, map<int, map<string, double>>>> sys, string stat_name, vector<int> cents, vector<int> divs, vector<int> energies, string name);
void division_stat(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, string stat_name, vector<int> cents, vector<int> divs, vector<int> energies, string name);
void division_stat(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, string stat_name, vector<int> cents, vector<int> divs, vector<int> energies, string name);

void stat_vs_mult_mean(map<int, map<int, map<int, map<string, Measure>>>> stats, string stat_name, vector<int> cents, vector<int> divs, string name);
void stat_vs_mult_mean(map<int, map<int, map<int, map<string, Measure>>>> stats, map<int, map<int, map<int, map<string, double>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name);
void stat_vs_mult_mean(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name);
void stat_vs_mult_mean(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, string stat_name, vector<int> cents, vector<int> divs, string name);

void stat_vs_mult_mean_cor(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name);

void make_comp_stat_plot(map<int, map<string, Measure>> stats1, map<int, map<string, Measure>> stats2);
void make_comp_stat_plot_hack(map<int, map<string, Measure>> stats1, map<int, map<string, Measure>> stats2);

void type_per_row(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, vector<string> types, vector<string> stat_names, vector<int> cents, vector<int> divs, string name);

pair<int, int> get_cent9_range(int bin);
pair<int, int> get_canvas_div(int plots);

#endif /* PLOTTING_H_ */
