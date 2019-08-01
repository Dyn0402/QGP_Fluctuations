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

#include "sim_v1.h"
#include "ratio_methods.h"
#include "plotting.h"
#include "config.h"
#include "file_io.h"
#include "sim_efficiency.h"
#include "ThreadPool.h"
#include "Stats.h"
#include "FourierApprox.h"
#include "Simulator.h"
#include "Measure.h"
#include "Plotter.h"

using namespace std;


void moments_vs_sim_pars() {
	TFile *out_file = new TFile("/home/dylan/local_server/dyn0402/Research/Simulation/Real_Proton_Dists/07-30-19_mix.root", "RECREATE");
	int div = 6;
	vector<double> mean_list = {5, 10};
	vector<double> cl_list = {0, 0.05, 0.1, 0.15, 0.2};
	double p_eff = 0.95;
	double spread = 0.2;
	map<string, map<double, map<double, measure<double>>>> results;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(double mean:mean_list) {
			for(double cl:cl_list) {
				pool.enqueue(run_pars, energy, p_eff, p_clust, spread, dist_hists[energy], out_file, &results);
			}
		}
	}

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



void run_pars(int n_events, double mean, double p_eff, double p_clust, double spread, map<string, map<double, map<double, measure<double>>>> *results) {
	cout << "Starting: \t events: " << n_events << "  mean: " << mean << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
	tree_data data = run_sim(p_eff, p_clust, mean, n_events, spread);
	vector<double> ratios = ratios_map_to_vec(data.ratios[sim.get_divisions()][0]);
	Simulator sim;
	sim.set_p_effect(p_eff);
	sim.set_p_cluster(p_clust);
	sim.set_particle_mean(mean);
	sim.set_n_events(n_events);
	sim.set_spread_sigma(spread);
	tree_data data = sim.run_simulation_mixed()[0];
	vector<double> ratios = ratios_map_to_vec(data.ratios[sim.get_divisions()][0]);
	get_moments();
	Stats stat(ratios);
	(*results)["mean"][mean][p_clust] = stat.get_mean();
	(*results)["standard_deviation"][mean][p_clust] = stat.get_standard_deviation();
	(*results)["skewness"][mean][p_clust] = stat.get_skewness();
	(*results)["kurtosis"][mean][p_clust] = stat.get_kurtosis();
	cout << "Finishing: \t events: " << n_events << "  mean: " << mean << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;

	plot_dists();
	ostringstream os("");
	os << energy << "GeV | peff: " << setprecision(2) << p_eff << " | p_clust: " << setprecision(2) << p_clust << " | spread: " << setprecision(2) << spread << ends;
	string name = os.str();
	Plotter plot;
	plot.canvas_2d_dist(data[])
}


tree_data run_sim(double p_eff, double p_clust, double mean, double n_events, double spread) {
	Simulator sim;
	sim.set_p_effect(p_eff);
	sim.set_p_cluster(p_clust);
	sim.set_particle_mean(mean);
	sim.set_n_events(n_events);
	sim.set_spread_sigma(spread);
	tree_data data = sim.run_simulation();

	return data;
}


void get_moments(vector<double> ratios, int energy, TFile *out_file, map<string, measure<double>> *results) {
	Stats stat(ratios);
	(*results)["mean"] = stat.get_mean();
	(*results)["standard_deviation"] = stat.get_standard_deviation();
	(*results)["skewness"] = stat.get_skewness();
	(*results)["kurtosis"] = stat.get_kurtosis();
}


void plot_dists(map<int, map<int, int>> data, TFile *out_file, int divs, string name) {
	TH2I *hist = ratios_map_to_hist(data, name+"_Hist");
	out_file->cd();
	TCanvas *can = new TCanvas(name.data()); // Memory leak
	TF1 *line = new TF1((name+"_Line").data(), ("x/"+to_string(divs)).data(), -0.5, 0.5+(--data.end())->first);
	TF1 *line2 = new TF1((name+"_Line2").data(), "x", -0.5, 0.5+(--data.end())->first);
	line2->SetLineColor(kBlue);
	can->SetLogz();
	hist->Draw("colz");
	line->Draw("same");
	line2->Draw("same");
	can->Write();

	delete hist;
	delete line;
	delete line2;
	delete can;

	cout << "Finishing: \t events: " << n_events << "  energy: " << energy << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
}
