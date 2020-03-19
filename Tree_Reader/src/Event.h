/*
 * Event.h
 *
 *  Created on: Sep 10, 2019
 *      Author: Dylan Neff
 */

#ifndef EVENT_H_
#define EVENT_H_

//#ifndef ROOT_Event
//#define ROOT_Event


#include <vector>

#include <TLeaf.h>
//#include <TObject.h>

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

struct event_defaults {
	double vx = 0.;
	double vy = 0.;
	double vz = 0.;
	double event_plane = 0.;

	// Values for centrality bin 0-5% in 9 bins
	map<int, unsigned> run {{7, 11130000}, {11, 11152039}, {19, 12114300}, {27, 12175000}, {39, 11100000}, {62, 11079000}};
	map<int, unsigned> btof {{7, 1125}, {11, 1125}, {19, 1050}, {27, 1125}, {39, 1400}, {62, 1925}};
	unsigned ref = 350;
	unsigned refn = 600;
};

//class Event : public TObject {
class Event {
public:
	// Structors
	Event();
	Event(event_defaults& defaults, int energy);
	Event(tree_leaves leaves);
	Event(Event *event);
	~Event();

	// Getters
	double get_vx();
	double get_vy();
	double get_vz();
	double get_event_plane();
	unsigned get_ref();
	unsigned get_run();
	unsigned get_refn();
	unsigned get_btof();
	vector<Track> get_protons();

	// Setters
	void set_vx(double vx);
	void set_vy(double vy);
	void set_vz(double vz);
	void set_event_plane(double event_plane);
	void set_ref(unsigned ref);
	void set_run(unsigned run);
	void set_refn(unsigned refn);
	void set_btof(unsigned btof);
	void set_protons(vector<Track> protons);

	// Doers
	void read_tree_event(tree_leaves leaves);
	void read_tree_event(Event *event);
	void set_event(double vx, double vy, double vz, unsigned ref, unsigned run, unsigned refn, unsigned btof, double event_plane);
	void clear();
	void pile_up(Event pile);
	void set_defaults();

//	ClassDef(Event, 1);

private:
	vector<Track> protons;

	double vx;
	double vy;
	double vz;
	double event_plane;

	unsigned ref;
	unsigned run;
	unsigned refn;  // Referring to refmult2, refmult3, etc
	unsigned btof;
};



#endif /* EVENT_H_ */
