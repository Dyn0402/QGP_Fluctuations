/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: dylan
 */

#include <iostream>
#include <tuple>
#include <vector>

#include <TFile.h>
#include <TDirectory.h>

#include "sim_v1.h"
#include "ratio_methods.h"
#include "plotting.h"
#include "config.h"

using namespace std;

void simulate();
vector<double> bin_ratios(vector<double> ratios, double min, double max, int bins);
void bin_ratios_test();

int main() {
//	bin_ratios_test();
	simulate();

	cout << "donzo" << endl;
	return(0);
}


void simulate() {
	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
	TDirectory *ratio_dir = out_file->mkdir("ratios");
	TDirectory *proton_dir = out_file->mkdir("protons");

	map<double, map<int, tuple<double,double>>> cumulants_with_peffect;
	for(double p_effect:{0.0, 0.05, 0.10, 0.15, 0.20, 0.25}) {
		cout << "Running p_effect: " << p_effect << endl;
		config::p_effect = p_effect;
		tree_data data = sim_v1();
		map<int, tuple<double,double>> cumulants;
		vector<double> ratios = ratios_map_to_vec(data.ratios[config::divisions][0]);
//		ratios = bin_ratios(ratios, -0.5, 1.1, 23);
//		cout << "ratios: " << flush;
//		for(double r:ratios) { cout << r << " " << flush; }
		for(int order:config::cumulant_orders) {
			cumulants[order] = get_cumulant(ratios, order);
		}
		ratio_dir->cd();
		hist_ratio_dist(ratios, "write");
		proton_dir->cd();
		hist_proton_dist(nproton_map_to_vec(data.good_protons[0]),"write");
		cumulants_with_peffect[p_effect] = cumulants;
	}

	out_file->cd();
	plot_cumulants_vs_peffect(cumulants_with_peffect);

	out_file->Close();
	delete out_file;
}


vector<double> bin_ratios(vector<double> ratios, double min, double max, int bins) {
	double bin_width = (max - min) / bins;
	vector<double> binned_ratios;
	int bin_index;
	for(double r:ratios) {
		bin_index = 0;
		while(bin_index * bin_width < r) {
			bin_index++;
		}
		binned_ratios.push_back((bin_index - 1)*bin_width + bin_width / 2.0);
	}

	return(binned_ratios);
}


void bin_ratios_test() {
	vector<double> test = {0.2, 0.5, 0.6, 0.55, 0.66};
	vector<double> test2 = bin_ratios(test, 0.0, 1.0, 3);

	for(double t:test2) {
		cout << t << endl;
	}
}

