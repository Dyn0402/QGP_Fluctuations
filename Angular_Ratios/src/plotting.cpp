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


//void make_cumulant_plots(vector<tree_data> data) {
//	for(int cum_ord)
//}
