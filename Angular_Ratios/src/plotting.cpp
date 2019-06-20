/*
 * plotting.cpp
 *
 *  Created on: Jun 18, 2019
 *      Author: Dylan Neff
 */


#include <vector>
#include <string>

#include <TH1.h>

#include "tree_reader.h"
#include "plotting.h"
#include "config.h"

using namespace std;



void make_ratio_dist_plots(vector<tree_data> data) {
	for(unsigned i=0; i<config::file_list.size(); i++) {
		for(unsigned j=0; j<config::divs.size(); j++) {
			hist_ratio_dist(data[i].ratios[j], config::file_list[i], config::divs[j]);
		}
	}
}


void hist_ratio_dist(vector<double> ratios, string file, int div) {
	string name = file + " " + to_string(div) + " divisions";
	TH1D *ratio_hist = new TH1D(name.data(), name.data(), config::ratio_hist_bins, config::ratio_hist_low, config::ratio_hist_high);
	for(double ratio:ratios) {
		ratio_hist->Fill(ratio);
	}
	ratio_hist->Write();
	delete ratio_hist;
}


void make_proton_dist_plots(vector<tree_data> data) {
	for(unsigned i=0; i<config::file_list.size(); i++) {
		hist_proton_dist(data[i].good_protons, config::file_list[i]);
	}
}


void hist_proton_dist(vector<int> nprotons, string file) {
	string name = file + " protons";
	TH1D *protons_hist = new TH1D(name.data(), name.data(), config::protons_hist_bins, config::protons_hist_low, config::protons_hist_high);
	for(double protons:nprotons) {
		protons_hist->Fill(protons);
	}
	protons_hist->Write();
	delete protons_hist;
}
