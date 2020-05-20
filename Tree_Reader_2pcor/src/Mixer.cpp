/*
 * Mixer.cpp
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */


#include <algorithm>
#include <iostream>
#include <math.h>

#include "ratio_methods.h"
#include "file_io.h"

#include "Mixer.h"


// Structors

Mixer::Mixer() {
	rand_rotate = false;
	event_plane_rotate = false;
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

	this->hist_nbins = 1000;
	this->hist_xlow = -2*M_PI;
	this->hist_xhigh = 2*M_PI;
}

Mixer::Mixer(int energy) {
	rand_rotate = false;
	event_plane_rotate = false;
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

	this->hist_nbins = 1000;
	this->hist_xlow = -2*M_PI;
	this->hist_xhigh = 2*M_PI;
}

// Getters

bool Mixer::get_rand_rotate() {
	return(rand_rotate);
}

bool Mixer::get_event_plane_rotate() {
	return(event_plane_rotate);
}

int Mixer::get_energy() {
	return(energy);
}

int Mixer::get_max_events() {
	return(max_events);
}

int Mixer::get_min_events() {
	return(min_events);
}

int Mixer::get_mixes_per_event() {
	return(mixes_per_event);
}

int Mixer::get_vz_bins() {
	return(vz_bins);
}

int Mixer::get_ep_bins() {
	return(ep_bins);
}

pair<double, double> Mixer::get_vz_range() {
	return(vz_range);
}

pair<double, double> Mixer::get_ep_range() {
	return(ep_range);
}

map<int, map<int, TH1D>> Mixer::get_data() {
	return data;
}

// Setters

void Mixer::set_rand_rotate(bool rand_rotate) {
	this->rand_rotate = rand_rotate;
}

void Mixer::set_event_plane_rotate(bool event_plane_rotate) {
	this->event_plane_rotate = event_plane_rotate;
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

void Mixer::set_hist_bins(int bins, double x_low, double x_high) {
	this->hist_nbins = bins;
	this->hist_xlow = x_low;
	this->hist_xhigh = x_high;
}


// Doers


// Append all proton angles from an event to the specified cent/eventplane/vz pool of events. For CBWC pass ref_mult in place of cent (untested).
void Mixer::append_event(const vector<double>& angles, int cent, double event_plane, double vz) {
	int ep_bin = get_ep_bin(event_plane);
	int vz_bin = get_vz_bin(vz);

	if((int)this->angles[cent][ep_bin][vz_bin].size() >= max_events) {  // Replace a random event if there are enough.
		int index = trand->Rndm() * max_events;
		this->angles[cent][ep_bin][vz_bin][index] = angles;
	} else {  // Append event if there are not enough.
		this->angles[cent][ep_bin][vz_bin].push_back(angles);
	}

	if((int)this->angles[cent][ep_bin][vz_bin].size() >= min_events) {  // Generate mixes_per_event mixed events if there are enough.
		for(int i=0; i<mixes_per_event; i++) {
			get_mixed(cent, (int)angles.size(), ep_bin, vz_bin);
		}
	}
}


// Sample angles randomly for an event. For CBWC pass ref_mult in place of cent (untested).
void Mixer::get_mixed(int cent, int num_protons, int ep_bin, int vz_bin) {
	vector<double> mix_angles;
	vector<pair<int, int>> used_angles;
	pair<int, int> index;
	double rand_angle = trand->Rndm() * 2 * M_PI;
	while((int)mix_angles.size() < num_protons) {
		index = generate_index(used_angles, cent, ep_bin, vz_bin);
		double new_angle = angles[cent][ep_bin][vz_bin][index.first][index.second];
		if(rand_rotate) { new_angle = rotate_angle(new_angle, rand_angle); }
		mix_angles.push_back(new_angle);
		used_angles.push_back(index);
	}
	vector<double> event_2pcorr = get_two_particle_correlation(mix_angles);  // Convert proton angles in event to 2pcor values.

	// Save 2pcor values to data
	int nparticles = mix_angles.size();
	if((string)data[cent][nparticles].GetName() == "") {
		string name_title = "Twopcorr_Mix_Cent_" + to_string(cent) + "_NParticles_" + to_string(nparticles);
		data[cent][nparticles].SetNameTitle(name_title.data(), name_title.data());
		data[cent][nparticles].SetBins(hist_nbins, hist_xlow, hist_xhigh);
	}
	for(auto &xi:event_2pcorr) {
		data[cent][nparticles].Fill(xi);
	}
}


// Ensure same angle is not used twice for a single mixed event.
pair<int, int> Mixer::generate_index(const vector<pair<int, int>>& used_angles, int cent_ref, int ep_bin, int vz_bin) {
	bool unique = false;
	int event_index, angle_index;
	while(!unique){
		event_index = trand->Rndm() * angles[cent_ref][ep_bin][vz_bin].size();
		angle_index = trand->Rndm() * angles[cent_ref][ep_bin][vz_bin][event_index].size();
		unique = true;
		for(const pair<int, int>& angle:used_angles) {
			if(angle.first == event_index && angle.second == angle_index) { unique = false; }
		}
	}

	pair<int, int> index;
	index.first = event_index;
	index.second = angle_index;

	return(index);
}


// Bin event plane
int Mixer::get_ep_bin(double event_plane) {
	int bin = ep_bins * (event_plane  + ep_range.first) / (ep_range.second - ep_range.first);
	return(bin);
}

// Bin vz
int Mixer::get_vz_bin(double vz) {
	int bin = vz_bins * (vz + vz_range.first) / (vz_range.second - vz_range.first);
	return(bin);
}

