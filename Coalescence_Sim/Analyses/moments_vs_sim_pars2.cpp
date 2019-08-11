/*
 * moments_vs_sim_pars.cpp
 *
 *  Created on: Jul 31, 2019
 *      Author: Dylan Neff
 */

#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>

#include <TROOT.h>
#include <TError.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>

#include "moments_vs_sim_pars2.h"
#include "ratio_methods.h"
#include "ThreadPool.h"
#include "Stats.h"
#include "../src/Simulator2.h"
#include "Measure.h"
#include "Plotter.h"
#include "RatioData.h"

using namespace std;


void moments_vs_sim_pars2(int div) {
	TFile *out_file = new TFile(("/home/dylan/local_server/dyn0402/Research/Simulation/08-09-19/08-11-19_mean_vs_pgroup_" + to_string(div) + "div_mixed.root").data(), "RECREATE");
	TDirectory *indiv_dir = out_file->mkdir("Individual 2Ds");
	indiv_dir->cd();
	int n_events = 1000000;
	vector<double> mean_list = {5.0, 10.0, 15.0}; //{5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0};
//	for(double mean = 4.0; mean <= 30.0; mean+=0.5) { mean_list.push_back(mean); }
	vector<double> group_list = {0.0, 0.05, 0.1};// 0.15, 0.2};
	double spread = 0.2;
	map<double, map<double, map<string, Measure>>> results;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(double mean:mean_list) {
			for(double group:group_list) {
				pool.enqueue(run_pars_mixed2, n_events, mean, group, spread, div, indiv_dir, &results);
			}
		}
	}

	out_file->cd();
	vector<string> moments = {"mean", "standard_deviation", "skewness", "kurtosis"};
	out_file->cd();
	for(string mom:moments) {
		map<double, map<double, Measure>> mom_result;
		for(pair<double, map<double, map<string, Measure>>> group:results) {
			for(pair<double, map<string, Measure>> mean:group.second) {
				mom_result[group.first][mean.first] = mean.second[mom];
			}
		}
		Plotter plot;
		plot.set_line_width(2);
		plot.moments_multi(mom_result, mom+" with proton mean and group", "p_group", "proton mean");
	}
	for(string mom:moments) {
		map<double, map<double, Measure>> mom_result;
		for(pair<double, map<double, map<string, Measure>>> group:results) {
			for(pair<double, map<string, Measure>> mean:group.second) {
				mom_result[group.first][mean.first] = mean.second[mom+"_r"];
			}
		}
		Plotter plot;
		plot.moments_multi(mom_result, mom+" divided by mixed with proton mean and group", "p_group", "proton mean");
	}
	out_file->Close();

}


void run_pars2(int n_events, double mean, double p_group, double spread, int div, TDirectory *out_file, map<double, map<double, map<string, Measure>>> *results) {
	ostringstream os("");
	os << "mean: " << setprecision(3) << mean << "  p_group: " << setprecision(2) << p_group << "  spread: " << setprecision(2) << spread << ends;
	string name = os.str();

	cout << "Starting: \t " << name << endl;

	RatioData data = run_sim2(p_group, mean, n_events, spread, div);
	out_file->cd();
	data.canvas_2d_dist(name);
	map<double, int> ratio_hist = data.get_ratio_hist();

	get_moments2(ratio_hist, &((*results)[p_group][mean]));

	cout << "Finishing: \t " << name << endl;
}


RatioData run_sim2(double p_group, double mean, double n_events, double spread, int div) {
	Simulator2 sim;
	sim.set_p_group(p_group);
	sim.set_particle_mean(mean);
	sim.set_n_events(n_events);
	sim.set_spread_sigma(spread);
	sim.set_divisions(div);
	RatioData data(div);
	data.set_ratio_data(sim.run_simulation());

	return data;
}


void get_moments2(map<double, int> ratios, map<string, Measure> *results) {
	Stats stat(ratios);
	(*results)["mean"] = stat.get_mean();
	(*results)["standard_deviation"] = stat.get_standard_deviation();
	(*results)["skewness"] = stat.get_skewness();
	(*results)["kurtosis"] = stat.get_kurtosis();
}



void run_pars_mixed2(int n_events, double mean, double p_group, double spread, int div, TDirectory *out_file, map<double, map<double, map<string, Measure>>> *results) {
	ostringstream os("");
	os << "mean: " << setprecision(2) << mean << "  p_group: " << setprecision(2) << p_group << "  spread: " << setprecision(2) << spread << ends;
	string name = os.str();

	cout << "Starting: \t " << name << endl;

	vector<RatioData> data = run_sim_mixed2(p_group, mean, n_events, spread, div);
	out_file->cd();
	data[0].canvas_2d_dist(name);
	data[1].canvas_2d_dist(name+"_mixed");
	map<double, int> ratios = data[0].get_ratio_hist();
	map<double, int> mix_ratios = data[1].get_ratio_hist();
	vector<map<double, int>> both = {ratios, mix_ratios};

	get_moments_mixed2(both, &((*results)[p_group][mean]));

	cout << "Finishing: \t " << name << endl;
}


vector<RatioData> run_sim_mixed2(double p_group, double mean, double n_events, double spread, int div) {
	Simulator2 sim;
	sim.set_p_group(p_group);
	sim.set_particle_mean(mean);
	sim.set_n_events(n_events);
	sim.set_spread_sigma(spread);
	sim.set_divisions(div);
	vector<map<int, map<int, int>>> sim_res = sim.run_simulation_mixed();
	RatioData data(div);
	data.set_ratio_data(sim_res[0]);
	RatioData mix_data(div);
	mix_data.set_ratio_data(sim_res[1]);

	vector<RatioData> both = {data, mix_data};

	return both;
}


void get_moments_mixed2(vector<map<double, int>> ratios, map<string, Measure> *results) {
	Stats stat(ratios[0]);
	Stats stat_mixed(ratios[1]);
	(*results)["mean"] = stat.get_mean();
	(*results)["standard_deviation"] = stat.get_standard_deviation();
	(*results)["skewness"] = stat.get_skewness();
	(*results)["kurtosis"] = stat.get_kurtosis();
	(*results)["mean_r"] = stat.get_mean() / stat_mixed.get_mean();
	(*results)["standard_deviation_r"] = stat.get_standard_deviation() / stat_mixed.get_standard_deviation();
	(*results)["skewness_r"] = stat.get_skewness() / stat_mixed.get_skewness();
	(*results)["kurtosis_r"] = stat.get_kurtosis() / stat_mixed.get_kurtosis();
}



void stat_test() {
	Simulator2 sim;
	sim.set_divisions(3);
	sim.set_spread_sigma(0.2);
	vector<map<int, map<int, int>>> sim_res = sim.run_simulation_mixed();
	RatioData data;
	data.set_ratio_data(sim_res[0]);
	Stats stat(data.get_ratio_hist());
	Stats stat_vec(data.get_ratio_vec());
	cout << "Hist mean: " << string(stat.get_mean()) << endl;
	cout << "Vec mean: " << string(stat_vec.get_mean()) << endl;
	cout << "Hist sd: " << string(stat.get_standard_deviation()) << endl;
	cout << "Vec sd: " << string(stat_vec.get_standard_deviation()) << endl;
	cout << "Hist skew: " << string(stat.get_skewness()) << endl;
	cout << "Vec skew: " << string(stat_vec.get_skewness()) << endl;
	cout << "Hist kurt: " << string(stat.get_kurtosis()) << endl;
	cout << "Vec kurt: " << string(stat_vec.get_kurtosis()) << endl;
}
