/*
 * Event.cpp
 *
 *  Created on: Sep 10, 2019
 *      Author: Dylan Neff
 */


#include "Event.h"


// Structors

Event::Event() {
	vtx_x = 0;
	vtx_y = 0;
	vtx_z = 0;
	nprim = 0;
	run = 0;
	ref2 = 0;
	btof_multi = 0;
}

Event::Event(double vtx_x, double vtx_y, double vtx_z, unsigned nprim, unsigned run, unsigned ref2, unsigned btof_multi) {
	this->vtx_x = vtx_x;
	this->vtx_y = vtx_y;
	this->vtx_z = vtx_z;
	this->nprim = nprim;
	this->run = run;
	this->ref2 = ref2;
	this->btof_multi = btof_multi;
}

Event::~Event() {}


// Getters

double Event::get_vtx_x() {
	return(vtx_x);
}

double Event::get_vtx_y() {
	return(vtx_y);
}

double Event::get_vtx_z() {
	return(vtx_z);
}

unsigned Event::get_nprim() {
	return(nprim);
}

unsigned Event::get_run() {
	return(run);
}

unsigned Event::get_ref2() {
	return(ref2);
}

unsigned Event::get_btof_multi() {
	return(btof_multi);
}


// Setters

void Event::set_vtx_x(double vtx_x) {
	this->vtx_x = vtx_x;
}

void Event::set_vtx_y(double vtx_y) {
	this->vtx_y = vtx_y;
}

void Event::set_vtx_z(double vtx_z) {
	this->vtx_z = vtx_z;
}

void Event::set_nprim(unsigned nprim) {
	this->nprim = nprim;
}

void Event::set_run(unsigned run) {
	this->run = run;
}

void Event::set_ref2(unsigned ref2) {
	this->ref2 = ref2;
}

void Event::set_btof_multi(unsigned btof_multi) {
	this->btof_multi = btof_multi;
}


// Doers

void Event::set_event(double vtx_x, double vtx_y, double vtx_z, unsigned nprim, unsigned run, unsigned ref2, unsigned btof_multi) {
	this->vtx_x = vtx_x;
	this->vtx_y = vtx_y;
	this->vtx_z = vtx_z;
	this->nprim = nprim;
	this->run = run;
	this->ref2 = ref2;
	this->btof_multi = btof_multi;
}

void Event::clear() {
	vtx_x = 0;
	vtx_y = 0;
	vtx_z = 0;
	nprim = 0;
	run = 0;
	ref2 = 0;
	btof_multi = 0;
}
