/*
 * config.cpp
 *
 *  Created on: May 5, 2019
 *      Author: dylan
 */

#include <vector>
#include <TMath.h>

using namespace std;

namespace config {
	int n_flatten_min = 1; //Minimum fourier order for flattening.
	int n_flatten_max = 25; //Maximum fourier order for flattening.

	string rawHistName = "dGauss";
	string flatHistName = "Flattened";

	vector <double> gMean = {1.0, 2.0};
	vector <double> gRms = {0.1, 0.2};
	vector <int> gWeight = {2, 1};

	double lBound = TMath::Pi();
	double rBound = 2*TMath::Pi();

	int N = 1000000; //Number of weight 1 events to generate.
	int bins = 400;
}
