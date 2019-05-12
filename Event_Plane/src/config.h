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

	extern int n_flatten_min;
	extern int n_flatten_max;

	extern TRandom3 *rand;

	extern string fileName;
	extern string rawHistName;
	extern string flatHistName, sizedHistName;

	extern double lBound, rBound;

	extern vector<double> gMean;
	extern vector<double> gRms;
	extern vector<int> gWeight;

	extern int nMax, nMin, N, bins;
}



#endif /* CONFIG_H_ */
