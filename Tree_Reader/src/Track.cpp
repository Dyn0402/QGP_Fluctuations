/*
 * Track.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: Dylan Neff
 */


#include "Track.h"


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
}

Track::Track(double pt, double p, double phi, double eta, float dca, double nsigma, float beta, short charge) {
	this->pt = pt;
	this->p = p;
	this->phi = phi;
	this->eta = eta;
	this->dca = dca;
	this->nsigma = nsigma;
	this->beta = beta;
	this->charge = charge;
}

Track::~Track() {}


// Getters

double Track::get_pt() {
	return(pt);
}

double Track::get_p() {
	return(p);
}

double Track::get_phi() {
	return(phi);
}

double Track::get_eta() {
	return(eta);
}

float Track::get_dca() {
	return(dca);
}

double Track::get_nsigma() {
	return(nsigma);
}

float Track::get_beta() {
	return(beta);
}

short Track::get_charge() {
	return(charge);
}


// Setters

void Track::set_pt(double pt) {
	this->pt = pt;
}

void Track::set_p(double p) {
	this->p = p;
}

void Track::set_phi(double phi) {
	this->phi = phi;
}

void Track::set_eta(double eta) {
	this->eta = eta;
}

void Track::set_dca(float dca) {
	this->dca = dca;
}

void Track::set_nsigma(double nsigma) {
	this->nsigma = nsigma;
}

void Track::set_beta(float beta) {
	this->beta = beta;
}

void Track::set_charge(short charge) {
	this->charge = charge;
}


// Doers

void Track::set_track(double pt, double p, double phi, double eta, float dca, double nsigma, float beta, short charge) {
	this->pt = pt;
	this->p = p;
	this->phi = phi;
	this->eta = eta;
	this->dca = dca;
	this->nsigma = nsigma;
	this->beta = beta;
	this->charge = charge;
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
}
