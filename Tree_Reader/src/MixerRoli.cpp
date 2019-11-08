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
}


// Getters

int MixerRoli::get_max_events() {
	return(max_events);
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

void MixerRoli::set_out_path(string path) {
	this->out_path = path;
}

void MixerRoli::set_divs(vector<int> divs) {
	this->divs = divs;
}


// Doers

// Append an event to the specified cent pool of events.
void MixerRoli::append_event(vector<double> angles, int cent, int ref_mult2, double event_plane) {
	// Append all proton angles from event to specified cent pool.
//	cout << "Cent: " << cent << " | ref_mult2: " << ref_mult2 << " | num_angles: " << angles.size() << " | num_events: " << this->angles[cent].size() << endl;
	int ep_bin = (int)((10*event_plane)/M_PI);  // Convert event_plane to bin like Roli did.
	if((int)this->angles[cent][ep_bin].size() >= max_events) {  // Replace a random event if there are enough.
		int index = rand() % max_events;
		this->angles[cent][ep_bin][index] = angles;
		get_mixed(cent, (int)angles.size(), ref_mult2, ep_bin);
	} else {  // Append event if there are not enough.
		this->angles[cent][ep_bin].push_back(angles);
	}
}

// Sample angles randomly for an event.
void MixerRoli::get_mixed(int cent, int num_protons, int ref_mult2, int ep_bin) {
	vector<double> mix_angles;
	while((int)mix_angles.size() < num_protons) {
		int event_index = rand() % max_events;
		int angle_index = rand() % (int)angles[cent][ep_bin][event_index].size();
		mix_angles.push_back(angles[cent][ep_bin][event_index][angle_index]);
	}
	for(int div:divs) {
		vector<int> event_ratios = get_Rs(mix_angles, div);  // Convert proton angles in event to ratio values.

		// Save ratio values to data
		for(int protons_in_bin:event_ratios) {
			data[div][ref_mult2][mix_angles.size()][protons_in_bin]++;
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
