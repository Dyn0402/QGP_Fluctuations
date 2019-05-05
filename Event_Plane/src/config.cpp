/*
 * config.cpp
 *
 *  Created on: May 5, 2019
 *      Author: dylan
 */

#include <TRandom3.h>

namespace config {
	double v2 = 0.025;

	int n_events = 10000;
	int n_particles = 200;
	int n_harmonic = 2;

	TRandom3 *rand = new TRandom3(0);
}
