/*
 * Event.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: Dylan Neff
 */


#include "Event.h"


// Structors

Event::Event() {
	vx = 0;
	vy = 0;
	vz = 0;
	event_plane = 0;
	ref = 0;
	run = 0;
	refn = 0;
	btof = 0;
}

Event::Event(tree_leaves leaves) {
	read_tree_event(leaves);
}

Event::~Event() {}


// Getters

double Event::get_vx() {
	return(vx);
}

double Event::get_vy() {
	return(vy);
}

double Event::get_vz() {
	return(vz);
}

double Event::get_event_plane() {
	return(event_plane);
}

unsigned Event::get_ref() {
	return(ref);
}

unsigned Event::get_run() {
	return(run);
}

unsigned Event::get_refn() {
	return(refn);
}

unsigned Event::get_btof() {
	return(btof);
}

vector<Track> Event::get_protons() {
	return(protons);
}


// Setters

void Event::set_vx(double vx) {
	this->vx = vx;
}

void Event::set_vy(double vy) {
	this->vy = vy;
}

void Event::set_vz(double vz) {
	this->vz = vz;
}

void Event::set_event_plane(double event_plane) {
	while(event_plane >= M_PI) { event_plane -= M_PI; }
	while(event_plane < 0) { event_plane += M_PI; }
	this->event_plane = event_plane;
}

void Event::set_ref(unsigned ref) {
	this->ref = ref;
}

void Event::set_run(unsigned run) {
	this->run = run;
}

void Event::set_refn(unsigned refn) {
	this->refn = refn;
}

void Event::set_btof(unsigned btof) {
	this->btof = btof;
}

void Event::set_protons(vector<Track> protons) {
	this->protons = protons;
}


// Doers

void Event::read_tree_event(tree_leaves leaves) {
	run = leaves.run->GetValue();
	ref = leaves.ref_mult->GetValue();
	refn = leaves.ref_multn->GetValue();
	btof = leaves.btof->GetValue();
	vx = leaves.vx->GetValue();
	vy = leaves.vy->GetValue();
	vz = leaves.vz->GetValue();
	event_plane = leaves.event_plane->GetValue();

	for(int proton_index = 0; proton_index<leaves.phi->GetLen(); proton_index++) {
		Track proton;
		proton.set_pt(leaves.pt->GetValue(proton_index));
		proton.set_p(leaves.p->GetValue(proton_index));
		proton.set_phi(leaves.phi->GetValue(proton_index));
		proton.set_eta(leaves.eta->GetValue(proton_index));
		proton.set_dca(leaves.dca->GetValue(proton_index));
		proton.set_nsigma(leaves.nsigma->GetValue(proton_index));
		proton.set_beta(leaves.beta->GetValue(proton_index));
		proton.set_charge(leaves.charge->GetValue(proton_index));
		protons.push_back(proton);
	}

}

void Event::set_event(double vx, double vy, double vz, unsigned ref, unsigned run, unsigned refn, unsigned btof, double event_plane) {
	this->vx = vx;
	this->vy = vy;
	this->vz = vz;
	this->ref = ref;
	this->run = run;
	this->refn = refn;
	this->btof = btof;
	this->event_plane = event_plane;
}

void Event::clear() {
	vx = 0;
	vy = 0;
	vz = 0;
	ref = 0;
	run = 0;
	refn = 0;
	btof = 0;
	event_plane = 0;

	protons.clear();
}

// Pile up input pile event onto this event.
void Event::pile_up(Event pile) {
	ref += pile.get_ref();
	refn += pile.get_refn();
	btof += pile.get_btof();
	vector<Track> pile_protons = pile.get_protons();
	protons.insert(protons.end(), pile_protons.begin(), pile_protons.end());
}
