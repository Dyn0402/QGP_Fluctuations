/*
 * Mixer.cpp
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */


#include <algorithm>

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
	for(double angle:angles) {  // Append all proton angles from event to speficied ref_mult2 pool.
		this->angles[ref_mult2].push_back(angle);
	}

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
}


// Write data to output directory
void Mixer::write_mixed_data() {
	if(use_leftover) {
		for(pair<int, vector<int>> cent:num_angles) {
			mix_set(cent.first);
		}
	}
	write_tree_data("local", data, out_path);
}
