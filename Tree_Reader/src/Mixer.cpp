/*
 * Mixer.cpp
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */


#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include <limits>

#include <TCanvas.h>

#include "ratio_methods.h"
#include "file_io.h"

#include "Mixer.h"


// Structors

Mixer::Mixer() {
	single_ratio = false;
	n1_ratios = false;
	rand_rotate = false;
	event_plane_rotate = false;
	resample = false;
	energy = 0;
	min_events = 150;
	max_events = 250;
	mixes_per_event = 1;
	vz_range.first = -30;
	vz_range.second = 30;
	ep_range.first = 0;
	ep_range.second = M_PI;
	vz_bins = 10;
	ep_bins = 10;
	c_rand = mt19937_64(trand->Integer(numeric_limits<int>::max()));
//	pois_dist = new poisson_distribution<int> (1);
}

Mixer::Mixer(int energy) {
	single_ratio = false;
	n1_ratios = false;
	rand_rotate = false;
	event_plane_rotate = false; 
	resample = false;
	this->energy = energy;
	min_events = 150;
	max_events = 250;
	mixes_per_event = 1;
	if(energy == 7) {
		vz_range.first = -50;
		vz_range.second = 50;
	} else {
		vz_range.first = -30;
		vz_range.second = 30;
	}
	ep_range.first = 0;
	ep_range.second = M_PI;
	vz_bins = 10;
	ep_bins = 10;
	c_rand = mt19937_64(trand->Integer(numeric_limits<int>::max()));
//	pois_dist = new poisson_distribution<int> (1);
}

Mixer::Mixer(int energy, bool single_ratio, bool rand_rotate) {
	this->single_ratio = single_ratio;
	n1_ratios = false;
	this->rand_rotate = rand_rotate;
	event_plane_rotate = false;
	resample = false;
	this->energy = energy;
	min_events = 150;
	max_events = 250;
	mixes_per_event = 1;
	if(energy == 7) {
		vz_range.first = -50;
		vz_range.second = 50;
	} else {
		vz_range.first = -30;
		vz_range.second = 30;
	}
	ep_range.first = 0;
	ep_range.second = M_PI;
	vz_bins = 10;
	ep_bins = 10;
	c_rand = mt19937_64(trand->Integer(numeric_limits<int>::max()));
//	pois_dist = new poisson_distribution<int> (1);
}


// Getters

bool Mixer::get_single_ratio() {
	return single_ratio;
}

bool Mixer::get_n1_ratios() {
	return n1_ratios;
}

bool Mixer::get_rand_rotate() {
	return rand_rotate;
}

bool Mixer::get_event_plane_rotate() {
	return event_plane_rotate;
}

bool Mixer::get_resample() {
	return resample;
}

int Mixer::get_energy() {
	return energy;
}

int Mixer::get_max_events() {
	return max_events;
}

int Mixer::get_min_events() {
	return min_events;
}

int Mixer::get_mixes_per_event() {
	return mixes_per_event;
}

int Mixer::get_vz_bins() {
	return vz_bins;
}

int Mixer::get_ep_bins() {
	return ep_bins;
}

int Mixer::get_resample_alg() {
	return resample_alg;
}

int Mixer::get_n_resamples() {
	return n_resamples;
}

int Mixer::get_n_bootstraps() {
	return n_bootstraps;
}

int Mixer::get_cent_bins() {
	return cent_bins;
}

int Mixer::get_cent_min() {
	return cent_min;
}

int Mixer::get_particle_bins() {
	return particle_bins;
}

int Mixer::get_particle_min() {
	return particle_min;
}

pair<double, double> Mixer::get_vz_range() {
	return vz_range;
}

pair<double, double> Mixer::get_ep_range() {
	return ep_range;
}

string Mixer::get_out_path() {
	return out_path;
}

vector<int> Mixer::get_divs() {
	return divs;
}


// Setters

void Mixer::set_single_ratio(bool single_ratio) {
	this->single_ratio = single_ratio;
}

void Mixer::set_n1_ratios(bool n1_ratios) {
	this->n1_ratios = n1_ratios;
}

void Mixer::set_rand_rotate(bool rand_rotate) {
	this->rand_rotate = rand_rotate;
}

void Mixer::set_event_plane_rotate(bool event_plane_rotate) {
	this->event_plane_rotate = event_plane_rotate;
}

void Mixer::set_resample(bool resample) {
	this->resample = resample;
}

void Mixer::set_energy(int energy) {
	this->energy = energy;
}

void Mixer::set_max_events(int max_events) {
	this->max_events = max_events;
}

void Mixer::set_min_events(int min_events) {
	this->min_events = min_events;
}

void Mixer::set_mixes_per_event(int mixes_per_event) {
	this->mixes_per_event = mixes_per_event;
}

void Mixer::set_vz_bins(int vz_bins) {
	this->vz_bins = vz_bins;
}

void Mixer::set_ep_bins(int ep_bins) {
	this->ep_bins = ep_bins;
}

void Mixer::set_vz_range(double vz_lower, double vz_upper) {
	this->vz_range.first = vz_lower;
	this->vz_range.second = vz_upper;
}

void Mixer::set_ep_range(double ep_lower, double ep_upper) {
	this->ep_range.first = ep_lower;
	this->ep_range.second = ep_upper;
}

void Mixer::set_out_path(string path) {
	this->out_path = path;
}

void Mixer::set_divs(vector<int> divs) {
	this->divs = divs;
}

void Mixer::set_rand_seed(int seed) {
	trand = new TRandom3(seed);
}

void Mixer::set_resample_alg(int alg_num) {
	resample_alg = alg_num;
}

void Mixer::set_n_resamples(int n) {
	n_resamples = n;
}

void Mixer::set_n_bootstraps(int n) {
	n_bootstraps = n;
}

void Mixer::set_cent_bins(int bins) {
	cent_bins = bins;
}

void Mixer::set_cent_min(int min) {
	cent_min = min;
}

void Mixer::set_particle_bins(int bins) {
	particle_bins = bins;
}

void Mixer::set_particle_min(int min) {
	particle_min = min;
}


// Doers

void Mixer::init_data() {
	for (unsigned div_i = 0; div_i < divs.size(); div_i++) {
		data.push_back(vector<vector<vector<long>>>());
		for (int cent_i = 0; cent_i < cent_bins; cent_i++) {
			data[div_i].push_back(vector<vector<long>>());
			for (int num_particles = 0; num_particles < particle_bins; num_particles++) {
				data[div_i][cent_i].push_back(vector<long>(num_particles + particle_min + 1, 0));
			}
		}
	}

	if (resample && n_bootstraps > 0) {
		for (unsigned div_i = 0; div_i < divs.size(); div_i++) {
			data_bs.push_back(vector<vector<vector<vector<long>>>>());
			for (int cent_i = 0; cent_i < cent_bins; cent_i++) {
				data_bs[div_i].push_back(vector<vector<vector<long>>>());
				for (int bs_i = 0; bs_i < n_bootstraps; bs_i++) {
					data_bs[div_i][cent_i].push_back(vector<vector<long>>());
					for (int num_particles = 0; num_particles < particle_bins; num_particles++) {
						data_bs[div_i][cent_i][bs_i].push_back(vector<long>(num_particles + particle_min + 1, 0));
					}
				}
			}
		}
	}

	for (unsigned div_i = 0; div_i < divs.size(); div_i++) {
		for (int cent_i=0; cent_i < cent_bins; cent_i++) {
			angles.push_back(vector<vector<vector<vector<double>>>> ());
			for (int ep_i=0; ep_i < ep_bins; ep_i++) {
				angles[cent_i].push_back(vector<vector<vector<double>>> ());
				for (int vz_i=0; vz_i < vz_bins; vz_i++) {
					angles[cent_i][ep_i].push_back(vector<vector<double>> ());
				}
			}
		}
	}
}


//void Mixer::init_data() {  // Old implementation, see if it still crashes
//	for (unsigned div_i = 0; div_i < divs.size(); div_i++) {
//		data.push_back(vector<vector<vector<long>>>());
//		data_bs.push_back(vector<vector<vector<vector<long>>>>());
//		for (int cent_i = 0; cent_i < cent_bins; cent_i++) {
//			data[div_i].push_back(vector<vector<long>>());
//			data_bs[div_i].push_back(vector<vector<vector<long>>>());
//			for (int bs_i = 0; bs_i < n_bootstraps; bs_i++) {
//				data_bs[div_i][cent_i].push_back(vector<vector<long>>());
//				for (int num_particles = 0; num_particles < particle_bins; num_particles++) {
//					data_bs[div_i][cent_i][bs_i].push_back(vector<long>(num_particles + particle_min + 1, 0));
//				}
//			}
//			for (int num_particles = 0; num_particles < particle_bins; num_particles++) {
//				data[div_i][cent_i].push_back(vector<long>(num_particles + particle_min + 1, 0));
//			}
//		}
//
//		for (int cent_i = 0; cent_i < cent_bins; cent_i++) {
//			angles.push_back(vector<vector<vector<vector<double>>>>());
//			for (int ep_i = 0; ep_i < ep_bins; ep_i++) {
//				angles[cent_i].push_back(vector<vector<vector<double>>>());
//				for (int vz_i = 0; vz_i < vz_bins; vz_i++) {
//					angles[cent_i][ep_i].push_back(vector<vector<double>>());
//				}
//			}
//		}
//	}
//}

// Genrate a list of random numbers in each event to use in general, reasmpling, and bootstrapping for mixed events. 
void Mixer::gen_mix_randoms(vector<double>& random_list, vector<double>& random_bs_list) {
	int n_randoms = mixes_per_event * n_resamples * divs.size();
	for (int i = 0; i < n_randoms; i++) {
		random_list.push_back(trand->Rndm());
	}

	int n_bs_randoms = mixes_per_event * n_bootstraps * divs.size();
	for (int i = 0; i < n_bs_randoms; i++) {
		random_bs_list.push_back(trand->Poisson(1));
	}
}

// Append all proton angles from an event to the specified cent/eventplane/vz pool of events. For CBWC pass ref_mult in place of cent (untested).
void Mixer::append_event(const vector<double>& angles, int cent, double event_plane, double vz) {
//	cout << "cent: " << cent << " event_plane: " << event_plane << " vz: " << vz << endl;
	int ep_bin = get_ep_bin(event_plane);
	int vz_bin = get_vz_bin(vz);
	int cent_bin = cent - cent_min;

	if (vz_ep_appended_hists.count(cent) == 0) { define_hists(cent); }
	vz_ep_appended_hists[cent].Fill(event_plane, vz);

//	if (cent_bin >= (int)this->angles.size()) { cout << "cent_bin " << cent_bin << " out of range for angles size " << this->angles.size() << endl; }
//	else if (ep_bin >= (int)this->angles[cent_bin].size()) { cout << "ep_bin " << ep_bin << " out of range for angles[cent_bin] size " << this->angles[cent_bin].size() << " cent_bin: " << cent_bin << endl; }
//	else if (vz_bin >= (int)this->angles[cent_bin][ep_bin].size()) {
//		cout << "vz_bin " << vz_bin << " out of range for angles[cent_bin][ep_bin] size " << this->angles[cent_bin][ep_bin].size() << " cent_bin: " << cent_bin << " ep_bin: " << ep_bin << endl;
//		cout << "cent: " << cent << " event_plane: " << event_plane << " vz: " << vz << endl;
//		cout << "ep_bins: " << ep_bins << " vz_bins: " << vz_bins << " cent_bins: " << cent_bins << endl;
//		cout << "ep_bin: " << ep_bin << " vz_bin: " << vz_bin << " cent_bin: " << cent_bin << endl;
//	}
	
	if((int)this->angles[cent_bin][ep_bin][vz_bin].size() >= max_events) {  // Replace a random event if there are enough.
		int index = trand->Rndm() * max_events;
//		if (index >= (int)this->angles[cent_bin][ep_bin][vz_bin].size()) { cout << "index " << index << " out of range for angles[cent_bin][ep_bin][vz_bin] size " << this->angles[cent_bin][ep_bin][vz_bin].size() << " cent_bin: " << cent_bin << " ep_bin: " << ep_bin << " vz_bin: " << vz_bin << endl; }
		this->angles[cent_bin][ep_bin][vz_bin][index] = angles;
	} else {  // Append event if there are not enough.
		this->angles[cent_bin][ep_bin][vz_bin].push_back(angles);
	}

	if((int)this->angles[cent_bin][ep_bin][vz_bin].size() >= min_events) {  // Generate mixes_per_event mixed events if there are enough.
		vz_ep_generated_hists[cent].Fill(event_plane, vz, mixes_per_event);
		for(int i=0; i<mixes_per_event; i++) {
			get_mixed(cent_bin, (int)angles.size(), ep_bin, vz_bin);
		}
	}
}


// Sample angles randomly for an event. For CBWC pass ref_mult in place of cent (untested).
void Mixer::get_mixed(int cent_bin, int num_protons, int ep_bin, int vz_bin) {
	vector<double> mix_angles;
	double rand_angle = trand->Rndm() * 2 * M_PI;
	int pool_events = (int)angles[cent_bin][ep_bin][vz_bin].size();

	if (num_protons > pool_events) {
		cout << "Not enough mixed events " << pool_events << "  for num_protons " << num_protons << endl;
	}

	vector<double> mixer_randoms;  // Make sure number of random calls independent of how many tracks in each event
	int max_protons = 150;  // Big problems if this is broken!
	if (num_protons > max_protons) { cout << "Too many protons in get_mixed! " << num_protons << endl; }
	for (int i = 0; i < 2 * max_protons; i++) {
		mixer_randoms.push_back(trand->Rndm());
	}

	if (num_protons == 0) {
		int num_rands = divs.size() * (n_resamples + n_bootstraps);
		for (int i = 0; i < num_rands; i++) {
			trand->Rndm();  // Just fire these off to keep random string the same
		}
		return;
	}

	int rand_index = 0;
	vector<int> event_indices(pool_events);
	iota(begin(event_indices), end(event_indices), 0);
	int event_meta_index, event_index, num_angles, angle_index;
	double new_angle;
	for (int i = 0; i < num_protons; i++) {
		event_meta_index = mixer_randoms[rand_index++] * event_indices.size();
		event_index = event_indices[event_meta_index];
		num_angles = angles[cent_bin][ep_bin][vz_bin][event_index].size();
		while (num_angles == 0) {  // If no angles, just take first index remaining in list
			event_indices.erase(event_indices.begin() + event_meta_index);
			event_meta_index = 0;
			event_index = event_indices[event_meta_index];
			num_angles = angles[cent_bin][ep_bin][vz_bin][event_index].size();
		}
		angle_index = mixer_randoms[rand_index++] * angles[cent_bin][ep_bin][vz_bin][event_index].size();
		new_angle = angles[cent_bin][ep_bin][vz_bin][event_index][angle_index];
		if (rand_rotate) { new_angle = rotate_angle(new_angle, rand_angle); }
		mix_angles.push_back(new_angle);
		event_indices.erase(event_indices.begin() + event_meta_index);
	}

	//vector<int> event_indices(pool_events);
	//iota(begin(event_indices), end(event_indices), 0);
	//shuffle(event_indices.begin(), event_indices.end(), c_rand);
	//for (int i=0; i < num_protons; i++) {
	//	int angle_index = trand->Rndm() * angles[cent_bin][ep_bin][vz_bin][event_indices[i]].size();
	//	double new_angle = angles[cent_bin][ep_bin][vz_bin][event_indices[i]][angle_index];
	//	if(rand_rotate) { new_angle = rotate_angle(new_angle, rand_angle); }
	//	mix_angles.push_back(new_angle);
	//}

	if ((int)mix_angles.size() - particle_min >= particle_bins) { cout << "num_particles: " << mix_angles.size() << " too big for particle_bins: " << particle_bins << " !!! mix" << endl; }

	int num_particles_bin = mix_angles.size() - particle_min;
	if (resample) {
		sort(mix_angles.begin(), mix_angles.end());
		for (unsigned div_bin=0; div_bin < divs.size(); div_bin++) {
			double div_rads = (double)divs[div_bin] / 180 * M_PI;
			vector<int> binned_event;
			if (resample_alg == 4) {
				binned_event = get_resamples4(mix_angles, div_rads, n_resamples, trand);
			}
			else if (resample_alg == 3) {
				binned_event = get_resamples3(mix_angles, div_rads, n_resamples);
			}
			else { cout << "Didn't find matching resample algorithm for #" << resample_alg << endl; }

			// Save binned values to data
			vector<long> &data_event = data[div_bin][cent_bin][num_particles_bin];  // Reduce map traversal
			for (unsigned num_in_bin=0; num_in_bin < binned_event.size(); num_in_bin++) {
				data_event[num_in_bin] += binned_event[num_in_bin];
			}

			// Save binned values to bootstraps
			for (int i = 0; i < n_bootstraps; i++) {
				vector<long> &data_event_bs = data_bs[div_bin][cent_bin][i][num_particles_bin];
				int poisson_samples = trand->Poisson(1);
				for (int j = 0; j <= poisson_samples; j++) {  // Poisson block bootstrap
//				for (int j = 0; j < pois_dist(c_rand); j++) {
					for (unsigned num_in_bin=0; num_in_bin < binned_event.size(); num_in_bin++) {
						data_event_bs[num_in_bin] += binned_event[num_in_bin];
					}
				}
			}
		}
	} else {
		for (unsigned div_bin=0; div_bin < divs.size(); div_bin++) {
			int bin_num = (int) 360 / divs[div_bin];
			double div_rads = (double)divs[div_bin] / 180 * M_PI;
			if(single_ratio) { bin_num = 1; }
			else if(bin_num > 1 && n1_ratios) { bin_num -= 1; }  // Ambiguous if case should change if div divides 360 or not.
			vector<int> event_ratios = get_Rs(mix_angles, div_rads, trand, bin_num);  // Convert particle angles in event to ratio values.

			// Save ratio values to data
			vector<long> &data_event = data[div_bin][cent_bin][num_particles_bin];  // Reduce map traversal
			for (int particles_in_bin : event_ratios) {
				data_event[particles_in_bin]++;
			}
		}
	}
}


// Bin event plane
int Mixer::get_ep_bin(double event_plane) {
	int bin = ep_bins * (event_plane - ep_range.first) / (ep_range.second - ep_range.first);
	bin = (bin == ep_bins ? bin - 1 : bin);
	return bin;
}

// Bin vz
int Mixer::get_vz_bin(double vz) {
	int bin = vz_bins * (vz - vz_range.first) / (vz_range.second - vz_range.first);
	bin = (bin == vz_bins ? bin - 1 : bin);
	return bin;
}

// Initialize all QA histograms
void Mixer::define_hists(int cent) {
	string set_name = split_string_by_char(out_path, '/').back() + +"_" + to_string(energy) + "GeV_" + to_string(cent);
	vz_ep_appended_hists.emplace(cent, TH2I(("mix_vz_ep_append_hist_" + set_name).data(), "mix_vz_ep_append_hist", ep_bins, ep_range.first, ep_range.second, vz_bins, vz_range.first, vz_range.second));
	vz_ep_generated_hists.emplace(cent, TH2I(("mix_vz_ep_gen_hist_" + set_name).data(), "mix_vz_ep_gen_hist", ep_bins, ep_range.first, ep_range.second, vz_bins, vz_range.first, vz_range.second));
}

// Write data to output directory
void Mixer::write_mixed_data() {
	reset_out_dir();
	if (resample) {
		write_tree_data_bootstrap("local", data, data_bs, divs, cent_min, particle_min, out_path + to_string(energy) + "GeV/");
	}
	else {
		write_tree_data("local", data, divs, cent_min, particle_min, out_path + to_string(energy) + "GeV/");
	}
}

// Write QA plots to open TFile
void Mixer::write_qa() {
	for (pair<const int, TH2I>& hist : vz_ep_appended_hists) {
		TCanvas can(("mix_vz_ep_append_hist_" + to_string(hist.first)).data());
		hist.second.GetXaxis()->SetTitle("event_plane");
		hist.second.GetYaxis()->SetTitle("vz");
		hist.second.Draw("COLZ");
		can.SetLogz();
		can.Update();
		can.Write();
		hist.second.Write();
	}

	for (pair<const int, TH2I>& hist : vz_ep_generated_hists) {
		TCanvas can(("mix_vz_ep_gen_hist_" + to_string(hist.first)).data());
		hist.second.GetXaxis()->SetTitle("event_plane");
		hist.second.GetYaxis()->SetTitle("vz");
		hist.second.Draw("COLZ");
		can.Update();
		can.Write();
		hist.second.Write();
	}
}


// Remove out_path directory for energy if it exists and recreate it.
// Create out_path if it does not exist.
void Mixer::reset_out_dir() {
	//if(system(("test -d "+out_path).data())) {
	//	if(system(("mkdir " + out_path).data())) { cout << "Could not create output directory " + out_path << endl; }
	//}

	//string energy_path = out_path+to_string(energy)+"GeV/";
	//if(!system(("test -d "+energy_path).data())) { system(("rm -r " + energy_path).data()); }
	//if(system(("mkdir " + energy_path).data())) { cout << "Could not create output directory " + energy_path << endl; }

	// Written for Windows, hopefully will also work on Linux
	mkdir(out_path);
	mkdir(out_path + to_string(energy) + "GeV/", true);
}

