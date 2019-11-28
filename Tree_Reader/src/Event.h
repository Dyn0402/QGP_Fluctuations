/*
 * Event.h
 *
 *  Created on: Sep 10, 2019
 *      Author: Dylan Neff
 */

#ifndef EVENT_H_
#define EVENT_H_


#include <vector>

#include <TLeaf.h>

#include "Track.h"

using namespace std;


struct tree_leaves {
	TLeaf *run;
	TLeaf *ref_mult;
	TLeaf *ref_mult2;
	TLeaf *btof;
	TLeaf *vx;
	TLeaf *vy;
	TLeaf *vz;
	TLeaf *event_plane;

	TLeaf *phi;
	TLeaf *pt;
	TLeaf *p;
	TLeaf *beta;
	TLeaf *charge;
	TLeaf *dca;
	TLeaf *nsigma;
	TLeaf *eta;
};


class Event {
public:
	// Structors
	Event();
	Event(tree_leaves leaves);
	~Event();

	// Getters
	double get_vx();
	double get_vy();
	double get_vz();
	double get_event_plane();
	unsigned get_ref();
	unsigned get_run();
	unsigned get_ref2();
	unsigned get_btof();
	vector<Track> get_protons();

	// Setters
	void set_vx(double vx);
	void set_vy(double vy);
	void set_vz(double vz);
	void set_event_plane(double event_plane);
	void set_ref(unsigned ref);
	void set_run(unsigned run);
	void set_ref2(unsigned ref2);
	void set_btof(unsigned btof);
	void set_protons(vector<Track> protons);

	// Doers
	void read_tree_event(tree_leaves leaves);
	void set_event(double vx, double vy, double vz, unsigned ref, unsigned run, unsigned ref2, unsigned btof);
	void clear();
	void pile_up(Event pile);

private:
	vector<Track> protons;

	double vx;
	double vy;
	double vz;
	double event_plane;

	unsigned ref;
	unsigned run;
	unsigned ref2;
	unsigned btof;
};



#endif /* EVENT_H_ */
