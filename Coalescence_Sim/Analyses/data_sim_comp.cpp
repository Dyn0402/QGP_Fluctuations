/*
 * data_sim_comp.cpp
 *
 *  Created on: Jul 31, 2019
 *      Author: dylan
 */



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

#include "data_sim_comp.h"
#include "moments_vs_sim_pars.h"
#include "ratio_methods.h"
#include "ThreadPool.h"
#include "Stats.h"
#include "../src/Simulator.h"
#include "Measure.h"
#include "Plotter.h"
#include "RatioData.h"

using namespace std;


void data_vs_sim() {
	int tree_num = 1;
	int num_trees = analysis::energy_list.size();
	for(int energy:analysis::energy_list) {
		cout << "Working on " << energy << "GeV. " << tree_num << " of " << num_trees << endl;
		string path = analysis::in_path + to_string(energy) + "GeV/";
		for(int cent:analysis::centrals) {
			map<int, int> nprotons = read_nprotons(path, cent);
			trees[energy].good_protons[cent] = nprotons;
			for(int div:analysis::divs) {
				map<int, map<int, int>> ratios = read_ratios(path, div, cent);
				trees[energy].ratios[div][cent] = ratios;
			}
		}
		tree_num++;
	}

	TFile *out_file = new TFile("/home/dylan/local_server/dyn0402/Research/Simulation/Real_Proton_Dists/07-30-19_mix.root", "RECREATE");
	TFile *file = new TFile("/home/dylan/local_server/dyn0402/Research/Results/07-23_new_data.root", "READ");
	int cent = 15;
	vector<int> energy_list = {7, 11, 19, 27, 39, 62};
	vector<double> cl_list = {0, 0.05, 0.1, 0.15, 0.2};
	double p_eff = 0.95;
	double spread = 0.2;
	int cent = 15;
	map<string, map<double, map<double, measure<double>>>> results;
	map<int, TH1D*> dist_hists;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(int energy:energy_list) {
			string data_path = "/home/dylan/local_server/dyn0402/Research/Data3/" + to_string(energy) + "GeV/";
			map<int, int> nprotons = read_nprotons(data_path, cent);
			trees[energy].good_protons[cent] = nprotons;
			for(int div:analysis::divs) {
				map<int, map<int, int>> ratios = read_ratios(path, div, cent);
				trees[energy].ratios[div][cent] = ratios;
			}
			string dist_path = "NProton_Dists/" + to_string(energy) + "GeV/" + to_string(energy) + "GeV protons Centrality " + to_string(cent);
			dist_hists[energy] = (TH1D*) file->Get(dist_path.data());
			for(double p_clust:cl_list) {
				pool.enqueue(run_mix_hist_norm_pars, energy, p_eff, p_clust, spread, dist_hists[energy], out_file, &results);
			}
		}
	}
	for(pair<int, TH1D*> hist:dist_hists) {
		delete hist.second;
	}
	file->Close();

	for(int cent:analysis::centrals) {
		map<int, int> nprotons = read_nprotons(path, cent);
		trees[energy].good_protons[cent] = nprotons;
		for(int div:analysis::divs) {
			map<int, map<int, int>> ratios = read_ratios(path, div, cent);
			trees[energy].ratios[div][cent] = ratios;
		}
	}

	vector<string> moments = {"mean", "standard_deviation", "skewness", "kurtosis"};
	out_file->cd();
	for(string mom:moments) {
		graph_2d(results[mom], mom+" with energy and cluster", "energy", "p_cluster", mom);
	}
	out_file->Close();

}


void moments_vs_sim_pars(int div) {
	TFile *out_file = new TFile("/home/dylan/local_server/dyn0402/Research/Simulation/08-01-19_mean_vs_pcl_" + to_string(div) + "div.root", "RECREATE");
	int n_events = 10000000;
	vector<double> mean_list; // = {5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0};
	for(double mean = 4.0; mean <= 30.0; mean+=0.5) { mean_list.push_back(mean); }
	vector<double> cl_list = {0.0, 0.05, 0.1, 0.15, 0.2};
	double p_eff = 0.95;
	double spread = 0.2;
	map<double, map<double, map<string, Measure>>> results;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(double mean:mean_list) {
			for(double cl:cl_list) {
				pool.enqueue(run_pars, n_events, mean, p_eff, cl, spread, div, out_file, &results);
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
	os << "mean: " << setprecision(2) << mean << "  peff: " << setprecision(2) << p_eff << "  p_clust: " << setprecision(2) << p_clust << "  spread: " << setprecision(2) << spread << ends;
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

