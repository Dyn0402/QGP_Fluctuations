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

#include <iostream>
#include <vector>

#include <TLeaf.h>
#include <TVector3.h>
//#include <TObject.h>

#include "Track.h"
#include "TreeBranches.h"

using namespace std;


struct event_defaults {
	float vx = 0.;
	float vy = 0.;
	float vz = 0.;
	float psi_east = 0.;
	float psi_west = 0.;
	float event_plane = 0.;
	float dca_xy_avg = 0.;
	float dca_xy_err = 0.;

	int event_id = 0;

	// Values for centrality bin 0-5% in 9 bins
	map<int, int> run {{7, 11130000}, {11, 11152039}, {19, 12114300}, {27, 12175000}, {39, 11100000}, {62, 11079000}};  // Guessed number in range and check it wasn't bad run.
	map<int, short> btof_multi {{7, 1125}, {11, 1125}, {19, 1050}, {27, 1125}, {39, 1400}, {62, 1925}};  // Fixed ref 350 value multiplied such that ratio passes cut.
	map<int, short> btof_match {{7, 117}, {11, 117}, {19, 120}, {27, 115}, {39, 115}, {62, 115}};  // Bad values, just copied multi, need to get values for match
	short ref = 200;  // Pretty arbitrary
//	short refn = 600;
	// refn[ref_num][energy][cent] for 9 bins. Took midpoint of centrality bin edges from StRefMultCorr and plugged in for refmult2 and 3.
//	map<int, map<int, map<int, short>>> test {{2, {{7, {{-1, 1}, {0, 10}}}, {11, {{-1, 2}, {0, 12}}}}}, {3, {{7, {{-1, 1}, {0, 10}}}, {11, {{-1, 2}, {0, 12}}}}}};
	map<int, map<int, map<int, short>>> refn {{3, {{7, {{-1, 1}, {0, 9}, {1, 18}, {2, 33}, {3, 55}, {4, 87}, {5, 131}, {6, 191}, {7, 248}, {8, 325}}},
		{11, {{-1, 1}, {0, 12}, {1, 23}, {2, 42}, {3, 71}, {4, 111}, {5, 167}, {6, 244}, {7, 316}, {8, 412}}},
		{19, {{-1, 1}, {0, 15}, {1, 31}, {2, 56}, {3, 94}, {4, 148}, {5, 221}, {6, 320}, {7, 413}, {8, 538}}},
		{27, {{-1, 1}, {0, 17}, {1, 34}, {2, 62}, {3, 103}, {4, 162}, {5, 243}, {6, 351}, {7, 452}, {8, 589}}},
		{39, {{-1, 1}, {0, 18}, {1, 36}, {2, 66}, {3, 110}, {4, 173}, {5, 259}, {6, 374}, {7, 481}, {8, 627}}},
		{62, {{-1, 1}, {0, 20}, {1, 39}, {2, 72}, {3, 121}, {4, 190}, {5, 285}, {6, 411}, {7, 527}, {8, 686}}}}},
	{2, {{7, {{-1, 1}, {0, 5}, {1, 10}, {2, 19}, {3, 33}, {4, 52}, {5, 79}, {6, 116}, {7, 151}, {8, 198}}},
			{11, {{-1, 1}, {0, 6}, {1, 13}, {2, 25}, {3, 42}, {4, 66}, {5, 99}, {6, 145}, {7, 189}, {8, 247}}},
			{19, {{-1, 1}, {0, 8}, {1, 17}, {2, 31}, {3, 52}, {4, 82}, {5, 124}, {6, 182}, {7, 236}, {8, 309}}},
			{27, {{-1, 1}, {0, 9}, {1, 19}, {2, 34}, {3, 57}, {4, 91}, {5, 137}, {6, 200}, {7, 260}, {8, 340}}},
			{39, {{-1, 1}, {0, 10}, {1, 20}, {2, 37}, {3, 62}, {4, 99}, {5, 150}, {6, 218}, {7, 282}, {8, 368}}},
			{62, {{-1, 1}, {0, 11}, {1, 22}, {2, 40}, {3, 67}, {4, 107}, {5, 162}, {6, 236}, {7, 306}, {8, 400}}}}}};
};

//class Event : public TObject {
class Event {
public:
	// Structors
	Event();
	Event(event_defaults& defaults, int energy, int ref_num, int cent);
	Event(tree_leaves leaves);
	Event(tree_branches branches);
	Event(Event *event);
	~Event();

	// Getters
	float get_vx() const;
	float get_vy() const;
	float get_vz() const;
	float get_psi_east() const;
	float get_psi_west() const;
	float get_event_plane() const;
	float get_dca_xy_avg() const;
	float get_dca_xy_err() const;
	short get_ref() const;
	int get_run() const;
	int get_event_id() const;
	short get_refn() const;
	short get_btof_multi() const;
	short get_btof_match() const;
	const vector<Track>& get_particles() const;
	int get_num_particles() const;

	// Setters
	void set_vx(float vx);
	void set_vy(float vy);
	void set_vz(float vz);
	void set_psi_east(float psi_east);
	void set_psi_west(float psi_west);
	void set_event_plane(float event_plane);
	void set_dca_xy_avg(float avg);
	void set_dca_xy_err(float sd);
	void set_ref(short ref);
	void set_run(int run);
	void set_event_id(int id);
	void set_refn(short refn);
	void set_btof_multi(short btof_multi);
	void set_btof_match(short btof_match);
	void set_particles(vector<Track> particles);

	// Doers
	void read_tree_event(tree_leaves leaves);
	void read_tree_event(tree_branches branches);  // Not implemented
	void read_tree_event(Event *event);
	void set_event(float vx, float vy, float vz, short ref, int run, int event_id,
			short refn, short btof_multi, short btof_match, float psi_east, float psi_west, float event_plane,
			float dca_xy_avg, float dca_xy_err);
	void clear();
	void pile_up(Event pile);
	void set_defaults();

//	ClassDef(Event, 1);

private:
	vector<Track> particles;

	int run;
	int event_id;

	short ref;
	short refn;  // Referring to refmult2, refmult3, etc
	short btof_multi;
	short btof_match;

	float vx;
	float vy;
	float vz;
	float psi_east;
	float psi_west;
	float event_plane;

	float dca_xy_avg;
	float dca_xy_err;
};



#endif /* EVENT_H_ */
