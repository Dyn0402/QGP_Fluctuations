/*
 * Event.h
 *
 *  Created on: Sep 10, 2019
 *      Author: Dylan Neff
 */

#ifndef EVENT_H_
#define EVENT_H_


#include <vector>

using namespace std;


class Event {
public:
	Event();
	Event(double vtx_x, double vtx_y, double vtx_z, unsigned nprim, unsigned run, unsigned ref2, unsigned btof_multi);
	~Event();

	double get_vtx_x();
	double get_vtx_y();
	double get_vtx_z();
	unsigned get_nprim();
	unsigned get_run();
	unsigned get_ref2();
	unsigned get_btof_multi();

	void set_vtx_x(double vtx_x);
	void set_vtx_y(double vtx_y);
	void set_vtx_z(double vtx_z);
	void set_nprim(unsigned nprim);
	void set_run(unsigned run);
	void set_ref2(unsigned ref2);
	void set_btof_multi(unsigned btof_multi);

	void set_event(double vtx_x, double vtx_y, double vtx_z, unsigned nprim, unsigned run, unsigned ref2, unsigned btof_multi);
	void clear();

private:
	double vtx_x;
	double vtx_y;
	double vtx_z;

	unsigned nprim;
	unsigned run;
	unsigned ref2;
	unsigned btof_multi;
};



#endif /* EVENT_H_ */
