/*
 * Mixer_Roli.cpp
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */


#include <algorithm>
#include <iostream>
#include <math.h>

#include "ratio_methods.h"
#include "file_io.h"

#include "MixerRoli.h"


// Structors

MixerRoli::MixerRoli() {
	max_events = 100;
	mixes_per_event = 1;
}


// Getters

int MixerRoli::get_max_events() {
	return(max_events);
}

int MixerRoli::get_mixes_per_event() {
	return(mixes_per_event);
}

string MixerRoli::get_out_path() {
	return(out_path);
}

vector<int> MixerRoli::get_divs() {
	return(divs);
}


// Setters

void MixerRoli::set_max_events(int max_events) {
	this->max_events = max_events;
}

void MixerRoli::set_mixes_per_event(int mixes_per_event) {
	this->mixes_per_event = mixes_per_event;
}

void MixerRoli::set_out_path(string path) {
	this->out_path = path;
}

void MixerRoli::set_divs(vector<int> divs) {
	this->divs = divs;
}


// Doers

// Append all proton angles from an event to the specified refmult2/eventplane/vz pool of events. For CBWC enabled.
void MixerRoli::append_event_CBWC(vector<double> angles, int ref_mult2, double event_plane, double vz) {
	int ep_bin = (int)((10*event_plane)/M_PI);  // Convert event_plane to bin like Roli did.
	int vz_bin = get_vz_bin(vz);
	if((int)this->angles[ref_mult2][ep_bin][vz_bin].size() >= max_events) {  // Replace a random event if there are enough.
		int index = rand() % max_events;
		this->angles[ref_mult2][ep_bin][vz_bin][index] = angles;
		for(int i=0; i<mixes_per_event; i++) {
			get_mixed((int)angles.size(), ref_mult2, ep_bin, vz_bin);
		}
	} else {  // Append event if there are not enough.
		this->angles[ref_mult2][ep_bin][vz_bin].push_back(angles);
	}
}

// Sample angles randomly for an event. For CBWC enabled.
void MixerRoli::get_mixed_CBWC(int num_protons, int ref_mult2, int ep_bin, int vz_bin) {
	vector<double> mix_angles;
	vector<pair<int, int>> used_angles;
	pair<int, int> index;
	while((int)mix_angles.size() < num_protons) {
		index = generate_index(used_angles, ref_mult2, ep_bin, vz_bin);
		mix_angles.push_back(angles[ref_mult2][ep_bin][vz_bin][index.first][index.second]);
		used_angles.push_back(index);
	}
	for(int div:divs) {
		vector<int> event_ratios = get_Rs(mix_angles, div);  // Convert proton angles in event to ratio values.

		// Save ratio values to data
		for(int protons_in_bin:event_ratios) {
			data[div][ref_mult2][mix_angles.size()][protons_in_bin]++;
		}
	}
}


// Append all proton angles from an event to the specified cent/eventplane/vz pool of events. For CBWC disabled.
void MixerRoli::append_event(vector<double> angles, int cent, double event_plane, double vz) {
	int ep_bin = (int)((10*event_plane)/M_PI);  // Convert event_plane to bin like Roli did.
	int vz_bin = get_vz_bin(vz);
	if((int)this->angles[cent][ep_bin][vz_bin].size() >= max_events) {  // Replace a random event if there are enough.
		int index = rand() % max_events;
		this->angles[cent][ep_bin][vz_bin][index] = angles;
		for(int i=0; i<mixes_per_event; i++) {
			get_mixed(cent, (int)angles.size(), ep_bin, vz_bin);
		}
	} else {  // Append event if there are not enough.
		this->angles[cent][ep_bin][vz_bin].push_back(angles);
	}
}

// Sample angles randomly for an event. For CBWC disabled.
void MixerRoli::get_mixed(int cent, int num_protons, int ep_bin, int vz_bin) {
	vector<double> mix_angles;
	vector<pair<int, int>> used_angles;
	pair<int, int> index;
	while((int)mix_angles.size() < num_protons) {
		index = generate_index(used_angles, cent, ep_bin, vz_bin);
		mix_angles.push_back(angles[cent][ep_bin][vz_bin][index.first][index.second]);
		used_angles.push_back(index);
	}
	for(int div:divs) {
		vector<int> event_ratios = get_Rs(mix_angles, div);  // Convert proton angles in event to ratio values.

		// Save ratio values to data
		for(int protons_in_bin:event_ratios) {
			data[div][cent][mix_angles.size()][protons_in_bin]++;
		}
	}
}


// Write data to output directory
void MixerRoli::write_mixed_data() {
	reset_out_dir();
	write_tree_data("local", data, out_path);
}


// Remove out_path directory for energy if it exists and recreate it.
void MixerRoli::reset_out_dir() {
	if(!system(("test -d "+out_path).data())) { system(("rm -r " + out_path).data()); }
	if(system(("mkdir " + out_path).data())) { cout << "Could not create output directory " + out_path << endl; }
}


// Bin vz
int MixerRoli::get_vz_bin(double vz) {
	int bin = (int)(vz / 10) + 5;  // Make attribute.
	if(bin >= 10) { cout << "vz_bin bad" << endl; }
	return(bin);
}

// Test
pair<int, int> MixerRoli::generate_index(vector<pair<int, int>> used_angles, int cent_ref, int ep_bin, int vz_bin) {
	bool unique = false;
	int event_index, angle_index;
	while(!unique){
		event_index = rand() % max_events;
		angle_index = rand() % (int)angles[cent_ref][ep_bin][vz_bin][event_index].size();
		unique = true;
		for(pair<int, int> angle:used_angles) {
			if(angle.first == event_index && angle.second == angle_index) { unique = false; }
		}
	}

	pair<int, int> index;
	index.first = event_index;
	index.second = angle_index;

	return(index);
}

