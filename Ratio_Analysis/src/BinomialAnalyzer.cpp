/*
 * BinomialAnalyzer.cpp
 *
 *  Created on: Jan 30, 2020
 *      Author: Dylan Neff
 */




#include <vector>

#include "BinomialAnalyzer.h"

using namespace std;


// Structors

BinomialAnalyzer::BinomialAnalyzer() {
	out_root = new TFile((out_path+out_root_name).data(), "RECREATE");
}

BinomialAnalyzer::~BinomialAnalyzer() {
	out_root->Close();
	delete out_root;
}


// Getters

string BinomialAnalyzer::get_out_root_name() {
	return out_root_name;
}


// Setters

void BinomialAnalyzer::set_out_root_name(string name) {
	out_root_name = name;
}


// Doers

void BinomialAnalyzer::analyze() {
	TDirectory *set_dir = out_root->mkdir((set_name + to_string(set_num)).data());
	analyze_subset(set_name, set_num, set_dir);
}


void BinomialAnalyzer::analyze_subset(string set_name, int set_num, TDirectory *set_dir) {
	cout << "Starting Set " + set_name + to_string(set_num) << endl << endl;

	string path = in_path + set_name + to_string(set_num) + "/";
	string path_mix = in_mix_path + set_name + to_string(set_num) + "/";

	map<int, map<int, map<int, AzimuthBinData>>> data = get_data(path);
	map<int, map<int, map<int, AzimuthBinData>>> data_mix = get_data(path_mix);

	draw_proton_bin_plots(data);
}


map<int, map<int, map<int, AzimuthBinData>>> BinomialAnalyzer::get_data(string path, int min_num_events) {
	map<int, map<int, map<int, AzimuthBinData>>> data;

	for(int energy:energy_list) {

		path += to_string(energy) + "GeV/";

		for(int div:divs) {

			// Get centralities found in file names of given directory path.
			for(int cent:get_centrals(path, div)) {
				AzimuthBinData az_data(div);
				az_data.read_data_from_dir(path, div, cent);  // Read azimuthal bin data from file path

				if(az_data.get_num_bins() / div <= min_num_events) {
					if(div == divs[0]) {
						cout << "Centrality " << cent << " with only " << az_data.get_num_bins() / div << " events. Skipping." << endl;
					}
				} else {
					data[energy][div][cent] = az_data;  // Store azimuthal bin data in data under corresponding centrality (refmult2) value
				}

			}
		}
	}

	return data;
}


void BinomialAnalyzer::draw_proton_bin_plots(map<int, map<int, map<int, AzimuthBinData>>> &data) {
	TDirectory *out_dir = out_root->mkdir("Proton_Bin_Plots");

	for(int energy:energy_list) {
		TDirectory *energy_dir = out_dir->mkdir((to_string(energy)+"GeV").data());

		for(int cent:centralities) {
			TDirectory *cent_dir = energy_dir->mkdir((to_string(cent)+"_Cent").data());

			for(int div:divs) {
				data[energy][div][cent].canvas_2d_dist("Proton_Bin_Dist " + to_string(energy) + "GeV, " + to_string(cent) + " cent, " + to_string(div) + " div");
			}
		}
	}
}
