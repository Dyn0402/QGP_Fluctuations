/*
 * plotting.cpp
 *
 *  Created on: Jun 18, 2019
 *      Author: Dylan Neff
 */


#include <vector>
#include <string>

#include <TH1.h>

#include "plotting.h"
#include "config.h"

using namespace std;



void make_dist_plots(vector<vector<vector<double>>> ratios) {
	for(unsigned i=0; i<config::file_list.size(); i++) {
		for(unsigned j=0; j<config::divs.size(); j++) {
			hist_ratio_dist(ratios[i][j], config::file_list[i], config::divs[j]);
		}
	}
}


void hist_ratio_dist(vector<double> ratios, string file, int div) {
	string name = file + " " + div + " divisions";
	TH1D *ratio_hist = new TH1D(name.data(), name.data(), config::ratio_hist_bins, config::ratio_hist_low, config::ratio_hist_high);
	for(double ratio:ratios) {
		ratio_hist->Fill(ratio);
	}
	ratio_hist->Write();
	delete ratio_hist;
}
