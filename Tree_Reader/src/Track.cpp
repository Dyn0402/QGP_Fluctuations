/*
 * Track.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: Dylan Neff
 */


#include "Track.h"

//ClassImp(Track);


// Structors

Track::Track() {
	pt = 0;
	p = 0;
	phi = 0;
	eta = 0;
	dca = 0;
	nsigma = 0;
	beta = 0;
	charge = 0;
	nhits_fit = 0;
}

Track::Track(float pt, float p, float phi, float eta, float dca, float nsigma, float beta, short charge, short nhits_fit) {
	this->pt = pt;
	this->p = p;
	this->phi = phi;
	this->eta = eta;
	this->dca = dca;
	this->nsigma = nsigma;
	this->beta = beta;
	this->charge = charge;
	this->nhits_fit = nhits_fit;
}

Track::Track(const track_defaults& defaults) {
	pt = defaults.pt;
	p = defaults.p;
	phi = defaults.phi;
	eta = defaults.eta;
	dca = defaults.dca;
	nsigma = defaults.nsigma;
	beta = defaults.beta;
	charge = defaults.charge;
	nhits_fit = defaults.nhits_fit;
}

Track::~Track() {}


// Getters

float Track::get_pt() const {
	return(pt);
}

float Track::get_p() const {
	return(p);
}

float Track::get_phi() const {
	return(phi);
}

float Track::get_eta() const {
	return(eta);
}

float Track::get_dca() const {
	return(dca);
}

float Track::get_nsigma() const {
	return(nsigma);
}

float Track::get_beta() const {
	return(beta);
}

short Track::get_charge() const {
	return(charge);
}

short Track::get_nhits_fit() const {
	return(nhits_fit);
}


// Setters

void Track::set_pt(float pt) {
	this->pt = pt;
}

void Track::set_p(float p) {
	this->p = p;
}

void Track::set_phi(float phi) {
	this->phi = phi;
}

void Track::set_eta(float eta) {
	this->eta = eta;
}

void Track::set_dca(float dca) {
	this->dca = dca;
}

void Track::set_nsigma(float nsigma) {
	this->nsigma = nsigma;
}

void Track::set_beta(float beta) {
	this->beta = beta;
}

void Track::set_charge(short charge) {
	this->charge = charge;
}

void Track::set_nhits_fit(short nhits_fit) {
	this->nhits_fit = nhits_fit;
}


// Doers

void Track::set_track(float pt, float p, float phi, float eta, float dca, float nsigma, float beta, short charge, short nhits_fit) {
	this->pt = pt;
	this->p = p;
	this->phi = phi;
	this->eta = eta;
	this->dca = dca;
	this->nsigma = nsigma;
	this->beta = beta;
	this->charge = charge;
	this->nhits_fit = nhits_fit;
}

void Track::clear() {
	pt = 0;
	p = 0;
	phi = 0;
	eta = 0;
	dca = 0;
	nsigma = 0;
	beta = 0;
	charge = 0;
	nhits_fit = 0;
}
