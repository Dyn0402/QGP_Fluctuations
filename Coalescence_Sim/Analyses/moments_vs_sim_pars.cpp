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

#include "moments_vs_sim_pars.h"
#include "ratio_methods.h"
#include "ThreadPool.h"
#include "Stats.h"
#include "../src/Simulator.h"
#include "Measure.h"
#include "Plotter.h"
#include "RatioData.h"

using namespace std;


void moments_vs_sim_pars(int div) {
	TFile *out_file = new TFile(("/home/dylan/Desktop/local_data/08-03-19_mean_vs_pcl_" + to_string(div) + "div_mixed.root").data(), "RECREATE");
	int n_events = 10000000;
	vector<double> mean_list; // = {5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0};
	for(double mean = 4.0; mean <= 30.0; mean+=0.5) { mean_list.push_back(mean); }
	vector<double> cl_list = {0.0, 0.05, 0.1, 0.15, 0.2};
	double p_eff = 0.95;
	double spread = 0.2;
	map<double, map<double, map<string, Measure>>> results;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(8);//thread::hardware_concurrency());
		for(double mean:mean_list) {
			for(double cl:cl_list) {
				pool.enqueue(run_pars_mixed, n_events, mean, p_eff, cl, spread, div, out_file, &results);
			}
		}
	}

	vector<string> moments = {"mean", "standard_deviation", "skewness", "kurtosis"};
	out_file->cd();
	for(string mom:moments) {
		map<double, map<double, Measure>> mom_result;
		for(pair<double, map<double, map<string, Measure>>> cl:results) {
			for(pair<double, map<string, Measure>> mean:cl.second) {
				mom_result[cl.first][mean.first] = mean.second[mom];
			}
		}
		Plotter plot;
		plot.moments_multi(mom_result, mom+" with proton mean and cluster", "p_cl", "proton mean");
	}
	out_file->Close();

}


void run_pars(int n_events, double mean, double p_eff, double p_clust, double spread, int div, TFile *out_file, map<double, map<double, map<string, Measure>>> *results) {
	ostringstream os("");
	os << "mean: " << setprecision(3) << mean << "  peff: " << setprecision(2) << p_eff << "  p_clust: " << setprecision(2) << p_clust << "  spread: " << setprecision(2) << spread << ends;
	string name = os.str();

	cout << "Starting: \t " << name << endl;

	RatioData data = run_sim(p_eff, p_clust, mean, n_events, spread, div);
	data.canvas_2d_dist(out_file, name, p_clust);
	vector<double> ratios = ratios_map_to_vec(data.get_ratio_data());

	get_moments(ratios, &((*results)[p_clust][mean]));

	cout << "Finishing: \t " << name << endl;
}


RatioData run_sim(double p_eff, double p_clust, double mean, double n_events, double spread, int div) {
	Simulator sim;
	sim.set_p_effect(p_eff);
	sim.set_p_cluster(p_clust);
	sim.set_particle_mean(mean);
	sim.set_n_events(n_events);
	sim.set_spread_sigma(spread);
	sim.set_divisions(div);
	RatioData data(div);
	data.set_ratio_data(sim.run_simulation());

	return data;
}


void get_moments(vector<double> ratios, map<string, Measure> *results) {
	Stats stat(ratios);
	(*results)["mean"] = stat.get_mean();
	(*results)["standard_deviation"] = stat.get_standard_deviation();
	(*results)["skewness"] = stat.get_skewness();
	(*results)["kurtosis"] = stat.get_kurtosis();
}



void run_pars_mixed(int n_events, double mean, double p_eff, double p_clust, double spread, int div, TFile *out_file, map<double, map<double, map<string, Measure>>> *results) {
	ostringstream os("");
	os << "mean: " << setprecision(2) << mean << "  peff: " << setprecision(2) << p_eff << "  p_clust: " << setprecision(2) << p_clust << "  spread: " << setprecision(2) << spread << ends;
	string name = os.str();

	cout << "Starting: \t " << name << endl;

	vector<RatioData> data = run_sim_mixed(p_eff, p_clust, mean, n_events, spread, div);
	data[0].canvas_2d_dist(out_file, name, p_clust);
	data[1].canvas_2d_dist(out_file, name+"_mixed", p_clust);
	vector<double> ratios = ratios_map_to_vec(data[0].get_ratio_data());
	vector<double> mix_ratios = ratios_map_to_vec(data[1].get_ratio_data());
	vector<vector<double>> both = {ratios, mix_ratios};

	get_moments_mixed(both, &((*results)[p_clust][mean]));

	cout << "Finishing: \t " << name << endl;
}


vector<RatioData> run_sim_mixed(double p_eff, double p_clust, double mean, double n_events, double spread, int div) {
	Simulator sim;
	sim.set_p_effect(p_eff);
	sim.set_p_cluster(p_clust);
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


void get_moments_mixed(vector<vector<double>> ratios, map<string, Measure> *results) {
	Stats stat(ratios[0]);
	Stats stat_mixed(ratios[1]);
	(*results)["mean"] = stat.get_mean() / stat_mixed.get_mean();
	(*results)["standard_deviation"] = stat.get_standard_deviation() / stat_mixed.get_standard_deviation();
	(*results)["skewness"] = stat.get_skewness() / stat_mixed.get_skewness();
	(*results)["kurtosis"] = stat.get_kurtosis() / stat_mixed.get_kurtosis();
}
