/*
 * flatten.h
 *
 *  Created on: May 6, 2019
 *      Author: dylan
 */

#ifndef FLATTEN_H_
#define FLATTEN_H_

#include <vector>
#include <TH1.h>

using namespace std;

double getAvg(TH1D* hist, double (*f)(double), int n);
double getAvg2(TH1D *hist, TF1 *func, int n);
double getAvg2(TH1D *hist, TF1 *func);
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int nMin, int nMax);
tuple<vector<double>, vector<double>> calcCoef2(TH1D* hist, int nMin, int nMax, double x_min, double x_max);
TH1D* genFlatHist(TH1D* hist, vector<double> A, vector<double> B, int nMin, int nMax, string flatName);
TH1D* genFlatHist2(TH1D* hist, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName);
TH1D* flatten_dist(vector<double> particles, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName);

TH1D* setDGausHist(int bins, double low, double up, string name);
TH1D* genDGausHist(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms);
vector<double> genDGausHist2(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms);



#endif /* FLATTEN_H_ */
