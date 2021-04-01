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
#include <cstdlib>

#include "ratio_methods.h"
#include "file_io.h"
#include "Randomizer.h"



// Structors

Randomizer::Randomizer() {
}



// Getters

string Randomizer::get_out_path() {
	return(out_path);
}

vector<int> Randomizer::get_divs() {
	return(divs);
}



// Setters

void Randomizer::set_out_path(string path) {
	this->out_path = path;
}

void Randomizer::set_divs(vector<int> divs) {
	this->divs = divs;
}



// Doers

// Append an event to the specified ref_mult2 pool of events.
void Randomizer::append_event(int num_protons, int cent, TRandom3 *trand) {
	// Generate num_protons random proton angles.
	vector<double> angles;
	for(int i=0; i < num_protons; i++) {
		angles.push_back(trand->Rndm() * 2.0 * M_PI);
	}

	// Get ratio values for random angles for each div and add values to data.
	for(int div:divs) {
		vector<int> event_ratios = get_Rs(angles, div);  // Convert proton angles in event to ratio values.

		// Save ratio values to data
		for(int protons_in_bin:event_ratios) {
			data[div][cent][angles.size()][protons_in_bin]++;
		}
	}
}


// Write data to output directory
void Randomizer::write_random_data() {
	reset_out_dir();
	write_tree_data("local", data, out_path);
}


// Remove out_path directory for energy if it exists and recreate it.
void Randomizer::reset_out_dir() {
	if(!system(("test -d "+out_path).data())) { system(("rm -r " + out_path).data()); }
	if(system(("mkdir " + out_path).data())) { cout << "Could not create output directory " + out_path << endl; }
}
