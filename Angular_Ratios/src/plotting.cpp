/*
 * plotting.cpp
 *
 *  Created on: Jun 18, 2019
 *      Author: Dylan Neff
 */


#include <vector>
#include <string>

#include <TFile.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TGraph.h>
#include <TMultiGraph.h>

#include "tree_reader.h"
#include "ratio_methods.h"
#include "plotting.h"
#include "config.h"

using namespace std;



void make_ratio_dist_plots(TFile *out_root, map<int, tree_data> data) {
	TDirectory *ratio_dist_dir = out_root->mkdir(config::ratio_dist_dir_name.data());
	ratio_dist_dir->cd();
	for(int energy:config::energy_list) {
		TDirectory *energy_dir = ratio_dist_dir->mkdir((to_string(energy) + "GeV").data());
		energy_dir->cd();
		for(int div:config::divs) {
			TDirectory *div_dir = energy_dir->mkdir((to_string(div) + "_Divs").data());
			div_dir->cd();
			for(int cent:config::centrals) {
				hist_ratio_dist(data[energy].ratios[div][cent], energy, div, cent);
			}
		}
	}
}


void hist_ratio_dist(vector<double> ratios, int energy, int div, int cent) {
	string name = to_string(energy) + "GeV " + to_string(div) + " divisions Centrality: " + to_string(cent);
	TH1D *ratio_hist = new TH1D(name.data(), name.data(), config::ratio_hist_bins, config::ratio_hist_low, config::ratio_hist_high);
	for(double ratio:ratios) {
		ratio_hist->Fill(ratio);
	}
	ratio_hist->Write();
	delete ratio_hist;
}


void make_proton_dist_plots(TFile *out_root, map<int, tree_data> data) {
	TDirectory *nproton_dist_dir = out_root->mkdir(config::nproton_dist_dir_name.data());
	nproton_dist_dir->cd();
	for(int energy:config::energy_list) {
		TDirectory *energy_dir = nproton_dist_dir->mkdir((to_string(energy) + "GeV").data());
		energy_dir->cd();
		for(int cent:config::centrals) {
			hist_proton_dist(data[energy].good_protons[cent], energy, cent);
		}
	}
}


void hist_proton_dist(vector<int> nprotons, int energy, int cent) {
	string name = to_string(energy) + "GeV protons Centrality: " + to_string(cent);
	TH1D *protons_hist = new TH1D(name.data(), name.data(), config::protons_hist_bins, config::protons_hist_low, config::protons_hist_high);
	for(double protons:nprotons) {
		protons_hist->Fill(protons);
	}
	protons_hist->Write();
	delete protons_hist;
}


void make_cumulant_plots(TFile *out_root, map<int, map<int, map<int, map<int, double>>>> cumulants) {
	TDirectory *cumulant_dir = out_root->mkdir(config::cumulant_dir_name.data());
	cumulant_dir->cd();
	for(int order:config::cumulant_orders) {
		TDirectory *order_dir = cumulant_dir->mkdir((to_string(order) + "_order").data());
		order_dir->cd();
		for(int cent:config::centrals) {
			TDirectory *cent_dir = order_dir->mkdir((to_string(cent) + "_centrality").data());
			cent_dir->cd();
			for(int div:config::divs) {
				graph_cumulant_vs_energy(cumulants, div, cent, order);
			}
			for(int energy:config::energy_list) {
				graph_cumulant_vs_divs(cumulants, energy, cent, order);
			}
		}
	}
}



void graph_cumulant_vs_energy(map<int, map<int, map<int, map<int, double>>>> cumulants, int div, int cent, int order) {
	vector<double> cumulant;
	for(int energy:config::energy_list) {
		cumulant.push_back(cumulants[energy][div][cent][order]);
	}
	graph_x_vs_y(config::energy_list, cumulant);
}


void graph_cumulant_vs_divs(map<int, map<int, map<int, map<int, double>>>> cumulants, int energy, int cent, int order) {
	vector<double> cumulant;
	for(int div:config::divs) {
		cumulant.push_back(cumulants[energy][div][cent][order]);
	}
	graph_x_vs_y(config::divs, cumulant);
}



void graph_x_vs_y(vector<int> x, vector<double> y) {
	vector<double> x_double(x.begin(), x.end());
	TGraph *graph = new TGraph((int)x_double.size(), x_double.data(), y.data());
	graph->Write();
	delete graph;
}
