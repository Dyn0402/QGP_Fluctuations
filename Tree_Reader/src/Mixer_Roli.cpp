/*
 * Mixer_Roli.cpp
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */


#include <algorithm>
#include <iostream>

#include "ratio_methods.h"
#include "file_io.h"
#include "Mixer_Roli.h"


// Structors

Mixer_Roli::Mixer_Roli() {
	max_events = 100;
}


// Getters

int Mixer_Roli::get_max_events() {
	return(max_events);
}

string Mixer_Roli::get_out_path() {
	return(out_path);
}

vector<int> Mixer_Roli::get_divs() {
	return(divs);
}


// Setters

void Mixer_Roli::set_max_events(int max_events) {
	this->max_events = max_events;
}

void Mixer_Roli::set_out_path(string path) {
	this->out_path = path;
}

void Mixer_Roli::set_divs(vector<int> divs) {
	this->divs = divs;
}


// Doers

// Append an event to the specified cent pool of events.
void Mixer_Roli::append_event(vector<double> angles, int cent, int ref_mult2) {
	// Append all proton angles from event to specified cent pool.
//	cout << "Cent: " << cent << " | ref_mult2: " << ref_mult2 << " | num_angles: " << angles.size() << " | num_events: " << this->angles[cent].size() << endl;
	if((int)this->angles[cent].size() >= max_events) {  // Replace a random event if there are enough.
		int index = rand() % max_events;
		this->angles[cent][index] = angles;
		get_mixed(cent, (int)angles.size(), ref_mult2);
	} else {  // Append event if there are not enough.
		this->angles[cent].push_back(angles);
	}
}

// Sample angles randomly for an event.
void Mixer_Roli::get_mixed(int cent, int num_protons, int ref_mult2) {
	vector<double> mix_angles;
	while((int)mix_angles.size() < num_protons) {
		int event_index = rand() % max_events;
		int angle_index = rand() % (int)angles[cent][event_index].size();
		mix_angles.push_back(angles[cent][event_index][angle_index]);
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
void Mixer_Roli::write_mixed_data() {
	write_tree_data("local", data, out_path);
}
