/*
 * flatten.h
 *
 *  Created on: May 6, 2019
 *      Author: Dylan Neff
 */

#ifndef FLATTEN_H_
#define FLATTEN_H_

#include <vector>
#include <TH1.h>

using namespace std;

//Distribution flattening function declarations
double getAvg(TH1D *hist, TF1 *func);
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int nMin, int nMax, double x_min, double x_max);
double flatten_angle(double angle, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max);

//Test distribution generation function declarations
TH1D* set_th1d(int bins, double low, double up, string name);
vector<double> genDGausHist(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms);
TH1D* flatten_dist(vector<double> particles, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName);


#endif /* FLATTEN_H_ */
