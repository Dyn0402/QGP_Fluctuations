/*
 * config.h
 *
 *  Created on: May 5, 2019
 *      Author: dylan
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <TRandom3.h>

namespace config {
	extern double v2;

	extern int n_events;
	extern int n_particles;
	extern int n_harmonic;

	extern TRandom3 *rand;
}



#endif /* CONFIG_H_ */
