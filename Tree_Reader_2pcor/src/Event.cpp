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
	event_plane = 0;
	ref = 0;
	run = 0;
	refn = 0;
	btof = 0;
}

Event::Event(event_defaults& defaults, int energy, int ref_num, int cent) {
	vx = defaults.vx;
	vy = defaults.vy;
	vz = defaults.vz;
	event_plane = defaults.event_plane;
	ref = defaults.ref;
	run = defaults.run[energy];
	refn = defaults.refn[ref_num][energy][cent];
	btof = defaults.btof[energy];
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

vector<Track> Event::get_particles() {
	return(particles);  // Can I return reference to speed up?
}

int Event::get_num_particles() {
	return( (int)particles.size() );
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

void Event::set_particles(vector<Track> particles) {
	this->particles = particles;  // Currently copying vector, can I take reference/move to speed up?
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
	cout << "Read tree_event" << endl;
	run = branches.run;
	cout << "Read run for event" << endl;
	ref = branches.ref_mult;
	refn = branches.ref_multn;
	btof = branches.btof;
	vx = branches.vx;
	vy = branches.vy;
	vz = branches.vz;
	event_plane = branches.event_plane;
	cout << "read event values" << endl;

	// Don't know how to read in tracks from branches

}


void Event::read_tree_event(Event *event) {
	run = event->get_run();
	ref = event->get_ref();
	refn = event->get_refn();
	btof = event->get_btof();
	vx = event->get_vx();
	vy = event->get_vy();
	vz = event->get_vz();
	event_plane = event->get_event_plane();

	vector<Track> old_particles = event->get_particles();
	unsigned num_particles = old_particles.size();
	for(unsigned i = 0; i < num_particles; i++) {
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

	particles.clear();
}

// Pile up input pile event onto this event.
void Event::pile_up(Event pile) {
	ref += pile.get_ref();
	refn += pile.get_refn();
	btof += pile.get_btof();
	vector<Track> pile_particless = pile.get_particles();
	particles.insert(particles.end(), pile_particless.begin(), pile_particless.end());
}
