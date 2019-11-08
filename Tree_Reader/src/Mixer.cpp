/*
 * Mixer.cpp
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */


#include <algorithm>
#include <iostream>
#include <cstdlib>

#include "ratio_methods.h"
#include "file_io.h"
#include "Mixer.h"


// Structors

Mixer::Mixer() {
	max_events = 100;
	use_leftover = true;
}


// Getters

int Mixer::get_max_events() {
	return(max_events);
}

bool Mixer::get_use_leftover() {
	return(use_leftover);
}

string Mixer::get_out_path() {
	return(out_path);
}

vector<int> Mixer::get_divs() {
	return(divs);
}


// Setters

void Mixer::set_max_events(int max_events) {
	this->max_events = max_events;
}

void Mixer::set_use_leftover(bool use_leftover) {
	this->use_leftover = use_leftover;
}

void Mixer::set_out_path(string path) {
	this->out_path = path;
}

void Mixer::set_divs(vector<int> divs) {
	this->divs = divs;
}


// Doers

// Append an event to the specified ref_mult2 pool of events.
void Mixer::append_event(vector<double> angles, int ref_mult2) {
	// Append all proton angles from event to specified ref_mult2 pool.
	this->angles[ref_mult2].insert(this->angles[ref_mult2].end(), angles.begin(), angles.end());

	// Append number of angles to appropriate ref_mult2 vector and mix the set if there are enough.
	num_angles[ref_mult2].push_back((int) angles.size());
	if((int)num_angles[ref_mult2].size() >= max_events) { mix_set(ref_mult2); }
}

// Mix all angles in specific ref_mult2 pool and sample randomly
void Mixer::mix_set(int ref_mult2) {
	// For given ref_mult2 value, shuffle angles randomly to mix events
	random_shuffle(angles[ref_mult2].begin(), angles[ref_mult2].end());

	int index = 0;
	for(int num:num_angles[ref_mult2]) {
		vector<double> mix_angles(angles[ref_mult2].begin() + index, angles[ref_mult2].begin() + index + num);

		for(int div:divs) {
			vector<int> event_ratios = get_Rs(mix_angles, div);  // Convert proton angles in event to ratio values.

			// Save ratio values to data
			for(int protons_in_bin:event_ratios) {
				data[div][ref_mult2][mix_angles.size()][protons_in_bin]++;
			}
		}
		index += num;
	}

	// Clear angles and num_angles vectors for mixed ref_mult2
	angles[ref_mult2].clear();
	num_angles[ref_mult2].clear();
}


// Write data to output directory
void Mixer::write_mixed_data() {
	if(use_leftover) {
		for(pair<int, vector<int>> cent:num_angles) {
			mix_set(cent.first);
		}
	}
	reset_out_dir();
	write_tree_data("local", data, out_path);
}


// Remove out_path directory for energy if it exists and recreate it.
void Mixer::reset_out_dir() {
	if(!system(("test -d "+out_path).data())) { system(("rm -r " + out_path).data()); }
	if(system(("mkdir " + out_path).data())) { cout << "Could not create output directory " + out_path << endl; }
}
