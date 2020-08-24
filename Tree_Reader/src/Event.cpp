/*
 * Event.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: Dylan Neff
 */


#include "Event.h"

//ClassImp(Event);


// Structors

Event::Event() {
	vx = 0;
	vy = 0;
	vz = 0;
	qx = 0;
	qy = 0;
	event_plane = 0.;
	dca_xy_avg = 0;
	dca_xy_err = 0;
	ref = 0;
	run = 0;
	event_id = 0;
	refn = 0;
	btof_multi = 0;
	btof_match = 0;
}

Event::Event(event_defaults& defaults, int energy, int ref_num, int cent) {
	vx = defaults.vx;
	vy = defaults.vy;
	vz = defaults.vz;
	qx = defaults.qx;
	qy = defaults.qy;
	event_plane = defaults.event_plane;
	dca_xy_avg = defaults.dca_xy_avg;
	dca_xy_err = defaults.dca_xy_err;
	ref = defaults.ref;
	run = defaults.run[energy];
	event_id = defaults.event_id;
	refn = defaults.refn[ref_num][energy][cent];
	btof_multi = defaults.btof_multi[energy];
	btof_match = defaults.btof_match[energy];
}

Event::Event(tree_leaves leaves) {
	read_tree_event(leaves);
}

Event::Event(tree_branches branches) {
	read_tree_event(branches);
}

Event::Event(Event *event) {
	read_tree_event(event);
}

Event::~Event() {}


// Getters

float Event::get_vx() {
	return vx;
}

float Event::get_vy() {
	return vy;
}

float Event::get_vz() {
	return vz;
}

float Event::get_qx() {
	return qx;
}

float Event::get_qy() {
	return qy;
}

float Event::get_event_plane() {
	return event_plane;
}

float Event::get_dca_xy_avg() {
	return dca_xy_avg;
}

float Event::get_dca_xy_err() {
	return dca_xy_err;
}

short Event::get_ref() {
	return ref;
}

int Event::get_run() {
	return run;
}

int Event::get_event_id() {
	return event_id;
}

short Event::get_refn() {
	return refn;
}

short Event::get_btof_multi() {
	return btof_multi;
}

short Event::get_btof_match() {
	return btof_match;
}

vector<Track> Event::get_particles() {
	return particles;  // Can I return reference to speed up?
}

int Event::get_num_particles() {
	return (int)particles.size();
}


// Setters

void Event::set_vx(float vx) {
	this->vx = vx;
}

void Event::set_vy(float vy) {
	this->vy = vy;
}

void Event::set_vz(float vz) {
	this->vz = vz;
}

void Event::set_qx(float qx) {
	this->qx = qx;
}

void Event::set_qy(float qy) {
	this->qy = qy;
}

void Event::set_event_plane(float event_plane) {
	this->event_plane = event_plane;
}

void Event::set_dca_xy_avg(float avg) {
	dca_xy_avg = avg;
}

void Event::set_dca_xy_err(float sd) {
	dca_xy_err = sd;
}

void Event::set_ref(short ref) {
	this->ref = ref;
}

void Event::set_run(int run) {
	this->run = run;
}

void Event::set_event_id(int id) {
	event_id = id;
}

void Event::set_refn(short refn) {
	this->refn = refn;
}

void Event::set_btof_multi(short btof_multi) {
	this->btof_multi = btof_multi;
}

void Event::set_btof_match(short btof_match) {
	this->btof_match = btof_match;
}

void Event::set_particles(vector<Track> particles) {
	this->particles = particles;  // Currently copying vector, can I take reference/move to speed up?
}


// Doers

void Event::read_tree_event(tree_leaves leaves) {
	run = leaves.run->GetValue();
	event_id = leaves.event_id->GetValue();
	ref = leaves.ref_mult->GetValue();
	refn = leaves.ref_multn->GetValue();
	btof_multi = leaves.btof->GetValue();
	vx = leaves.vx->GetValue();
	vy = leaves.vy->GetValue();
	vz = leaves.vz->GetValue();
	qx = leaves.qx->GetValue();
	qy = leaves.qy->GetValue();
	// Need to calculate event plane from qx, qy
	dca_xy_avg = leaves.dca_xy_avg->GetValue();
	dca_xy_err = leaves.dca_xy_err->GetValue();

	for(int particle_index = 0; particle_index<leaves.phi->GetLen(); particle_index++) {
		Track particle;

		particle.set_pt(leaves.pt->GetValue(particle_index));
		particle.set_phi(leaves.phi->GetValue(particle_index));
		particle.set_eta(leaves.eta->GetValue(particle_index));

		TVector3 vec(1,1,1);
		vec.SetPtEtaPhi(particle.get_pt(), particle.get_eta(), particle.get_phi());
		particle.set_p(vec.Mag());

		particle.set_dca(leaves.dca->GetValue(particle_index));
		particle.set_nsigma(leaves.nsigma->GetValue(particle_index));
		particle.set_beta(leaves.beta->GetValue(particle_index));
		particle.set_charge(leaves.charge->GetValue(particle_index));

		particles.push_back(particle);
	}

}

void Event::read_tree_event(tree_branches branches) {
	run = branches.run_num;
	event_id = branches.event_id;

	ref = branches.refmult;
	refn = branches.refmultn;
	btof_multi = branches.btof_multi;
	btof_match = branches.btof_match;

	vx = branches.vx;
	vy = branches.vy;
	vz = branches.vz;

	qx = branches.qx;
	qy = branches.qy;
	// Need to calculate event plane from qx, qy

	dca_xy_avg = branches.dca_xy_avg;
	dca_xy_err = branches.dca_xy_err;

	for(int particle_index = 0; particle_index < (int)branches.phi->size(); particle_index++) {
		Track particle;

		particle.set_pt(branches.pt->at(particle_index));
		particle.set_phi(branches.phi->at(particle_index));
		particle.set_eta(branches.eta->at(particle_index));

		TVector3 vec(1,1,1);
		vec.SetPtEtaPhi(particle.get_pt(), particle.get_eta(), particle.get_phi());
		particle.set_p(vec.Mag());

		particle.set_dca(branches.dca->at(particle_index));
		particle.set_nsigma(branches.nsigma->at(particle_index));
		particle.set_beta(branches.beta->at(particle_index));
		particle.set_charge(branches.charge->at(particle_index));

		particles.push_back(particle);
	}

}


void Event::read_tree_event(Event *event) {
	run = event->get_run();
	event_id = event->get_event_id();
	ref = event->get_ref();
	refn = event->get_refn();
	btof_multi = event->get_btof_multi();
	btof_match = event->get_btof_match();
	vx = event->get_vx();
	vy = event->get_vy();
	vz = event->get_vz();
	qx = event->get_qx();
	qy = event->get_qy();
	event_plane = event->get_event_plane();
	dca_xy_avg = event->get_dca_xy_avg();
	dca_xy_err = event->get_dca_xy_err();

	vector<Track> old_particles = event->get_particles();
	short num_particles = old_particles.size();
	for(short i = 0; i < num_particles; i++) {
		Track particle;
		particle.set_pt(old_particles[i].get_pt());
		particle.set_p(old_particles[i].get_p());
		particle.set_phi(old_particles[i].get_phi());
		particle.set_eta(old_particles[i].get_eta());
		particle.set_dca(old_particles[i].get_dca());
		particle.set_nsigma(old_particles[i].get_nsigma());
		particle.set_beta(old_particles[i].get_beta());
		particle.set_charge(old_particles[i].get_charge());
		particles.push_back(particle);
	}
}

void Event::set_event(float vx, float vy, float vz, short ref, int run, int event_id,
		short refn, short btof_multi, short btof_match, float qx, float qy, float event_plane,
		float dca_xy_avg, float dca_xy_err) {
	this->vx = vx;
	this->vy = vy;
	this->vz = vz;
	this->ref = ref;
	this->run = run;
	this->event_id = event_id;
	this->refn = refn;
	this->btof_multi = btof_multi;
	this->btof_match = btof_match;
	this->qx = qx;
	this->qy = qy;
	this->event_plane = event_plane;
	this->dca_xy_avg = dca_xy_avg;
	this->dca_xy_err = dca_xy_err;
}

void Event::clear() {
	vx = 0;
	vy = 0;
	vz = 0;
	ref = 0;
	run = 0;
	event_id = 0;
	refn = 0;
	btof_multi = 0;
	btof_match = 0;
	qx = 0;
	qy = 0;
	event_plane = 0;
	dca_xy_avg = 0;
	dca_xy_err = 0;

	particles.clear();
}

// Pile up input pile event onto this event.
void Event::pile_up(Event pile) {
	ref += pile.get_ref();
	refn += pile.get_refn();
	btof_multi += pile.get_btof_multi();
	btof_match += pile.get_btof_match();
	qx += pile.get_qx();
	qy += pile.get_qy();
	vector<Track> pile_particless = pile.get_particles();
	particles.insert(particles.end(), pile_particless.begin(), pile_particless.end());
}
