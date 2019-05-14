/*
 * config.cpp
 *
 *  Created on: May 5, 2019
 *      Author: dylan
 */

#include <vector>
#include <TRandom3.h>
#include <TMath.h>

using namespace std;

namespace config {
	double v2 = 0.1;

	int n_events = 100000;
	int n_particles = 200;
	int n_harmonic = 2;

	int n_flatten_min = 1; //Minimum fourier order for flattening.
	int n_flatten_max = 20; //Maximum fourier order for flattening.

	TRandom3 *rand = new TRandom3(0);

	string fileName = "As4.root";
	string rawHistName = "dGauss";
	string flatHistName = "Flattened";
	string sizedHistName = "Sized";

	vector <double> gMean = {0.0, 2.0};
	vector <double> gRms = {1.0, 0.2};
	vector <int> gWeight = {2, 1};

	double lBound = -TMath::Pi();
	double rBound = +TMath::Pi();

	int N = 1000000; //Number of weight 1 events to generate.
	int bins = 400;
}
