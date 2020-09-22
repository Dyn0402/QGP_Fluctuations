/*
 * TreeCuts.cpp
 *
 *  Created on: Jul 30, 2020
 *      Author: Dylan Neff
 */


#include "TreeCuts.h"

using namespace std;


// Structors
TreeCuts::TreeCuts() {
	energy = 0;
	particle = "";
}

TreeCuts::TreeCuts(int energy) {
	this->energy = energy;
	set_values();
	particle = "";
}

TreeCuts::TreeCuts(int energy, string particle) {
	this->energy = energy;
	this->particle = particle;
	set_values();
}

TreeCuts::~TreeCuts() {
	// Nothing
}


// Doers
void TreeCuts::set_values() {
	// Dca_xy Cuts
	DcaxyQABase dca_xy_qa(energy);
	dca_xy_bad_runs = dca_xy_qa.get_bad_runs();
	dca_xy_bad_event_ranges = dca_xy_qa.get_bad_ranges();

	// Pile Up Cuts
	PileUpQAer pile_up_qa(energy);
	pile_up_low = pile_up_qa.get_low_cut();
	pile_up_high = pile_up_qa.get_high_cut();

	// Event Cuts
	bad_runs = bad_runs_by_energy[energy];

	// Track Cuts
	min_beta = min_beta_by_energy[particle][energy];
	max_beta = max_beta_by_energy[particle][energy];
	charge = charge_by_energy[particle][energy];
	max_eta = max_eta_by_energy[particle][energy];
	min_eta = min_eta_by_energy[particle][energy];
	max_nsigma = max_nsigma_by_energy[particle][energy];
	min_nsigma = min_nsigma_by_energy[particle][energy];
	max_dca = max_dca_by_energy[particle][energy];
	min_dca = min_dca_by_energy[particle][energy];
	max_m2 = max_m2_by_energy[particle][energy];
	min_m2 = min_m2_by_energy[particle][energy];
	min_pt_tof = min_pt_tof_by_energy[particle][energy];
	max_pt_tof = max_pt_tof_by_energy[particle][energy];
	min_pt_no_tof = min_pt_no_tof_by_energy[particle][energy];
	max_pt_no_tof = max_pt_no_tof_by_energy[particle][energy];
	max_p_tof = max_p_tof_by_energy[particle][energy];
	max_p_no_tof = max_p_no_tof_by_energy[particle][energy];

	// Other
	expected_events = expected_events_by_energy[energy];
}

void TreeCuts::set_values(int energy) {
	this->energy = energy;

	set_values();
}

void TreeCuts::set_values(string particle) {
	this->particle = particle;

	set_values();
}

void TreeCuts::set_values(int energy, string particle) {
	this->energy = energy;
	this->particle = particle;

	set_values();
}

