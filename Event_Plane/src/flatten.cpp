/*
 * flatten.cpp
 *
 *  Created on: May 5, 2019
 *      Author: Dylan Neff
 */


#include <vector>
#include <TF1.h>
#include <TH1D.h>
#include <TMath.h>
#include <TRandom.h>
#include <TRandom3.h>

#include "flatten.h"

using namespace std;


//Get average of TF1 func on histogram hist.
double get_avg(TH1D *hist, TF1 *func) {
	int norm = 0;
	double sum = 0.0;
	int binCont;
	double binCent;

	for(int i = hist->GetXaxis()->GetFirst(); i<=hist->GetXaxis()->GetLast(); i++) {
		binCont = hist->GetBinContent(i);
		binCent = hist->GetBinCenter(i);
		norm += binCont;
		sum += binCont * func->Eval(binCent);
	}

	return(sum / norm);
}


//Calculate coefficients of Fourier expansion.
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int n_min, int n_max, double x_min, double x_max){
	vector<double> B, A;

	double x_avg = (x_max + x_min) / 2; //Calculate average of range
	double L = (x_max - x_min) / 2; //Calculate half range (2*L is full range)

	TF1 *cos_shift = new TF1("cos_shift", "cos([3] * [2] * (x-[0]) / [1])");
	cos_shift->SetParameters(x_avg, L, TMath::Pi(), n_min);
	TF1 *sin_shift = new TF1("sin_shift", "sin([3] * [2] * (x-[0]) / [1])");
	sin_shift->SetParameters(x_avg, L, TMath::Pi(), n_min);

	for(int n = n_min; n <= n_max; n++) {
		cos_shift->SetParameter(3, n);
		sin_shift->SetParameter(3, n);
		B.push_back(get_avg(hist, cos_shift) * 2.0 / n);
		A.push_back(get_avg(hist, sin_shift) * -2.0 / n);
	}

	delete cos_shift;
	delete sin_shift;

	return(make_tuple(A, B));
}


//Apply correction to event angle to make distribution flat.
double flatten_angle(double angle, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max) {
	double new_angle, old_angle;

	double x_avg = (x_max + x_min) / 2; //Calculate average of range
	double L = (x_max - x_min) / 2; //Calculate half range (2*L is full range)

	old_angle = TMath::Pi() * (angle - x_avg) / L;
	new_angle = old_angle;
	for(int n = nMin; n <= nMax; n++) {
		new_angle += A[n-nMin] * TMath::Cos(n * old_angle) + B[n-nMin] * TMath::Sin(n * old_angle);
	}
	new_angle = L * new_angle / TMath::Pi() + x_avg;
	while(new_angle > x_max) {new_angle -= (x_max-x_min);}
	while(new_angle < x_min) {new_angle += (x_max-x_min);}


	return(new_angle);
}


//Define and set the initial th1d histogram.
TH1D* set_th1d(int bins, double low, double up, string name) {
	TH1D* hist = new TH1D(name.c_str(), name.c_str(), bins, low, up);

	return(hist);
}


//Generate a double gaussian distribution.
vector<double> genDGausHist(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms) {
	int first = hist->GetXaxis()->GetFirst();
	int last = hist->GetXaxis()->GetLast();
	double low = hist->GetBinLowEdge(first);
	double up = hist->GetBinLowEdge(last+1);

	TRandom3* r = new TRandom3(0);
	double rand;
	vector<double> angles;

	for(unsigned i=0; i<weight.size(); i++) {
		for(int j=0; j<N*weight[i]; j++) {
			rand = r->Gaus(mean[i], rms[i]);
			while(rand > up) {rand -= (up-low);}
			while(rand < low) {rand += (up-low);}
			hist->Fill(rand);
			angles.push_back(rand);
		}
	}

	return(angles);
}



//Given an input of event angles, fourier coefficients, minimum and maximum fourier harmonic, range (x_min, x_max) of distribution, and name of distribution
//apply correction to each angle to flatten the distribution.
//Return flattened distribution as a TH1D histogram.
TH1D* flatten_dist(vector<double> event_angles, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName) {
	TH1D *flat = new TH1D(flatName.data(), flatName.data(), 400, x_min, x_max);

	double new_angle;
	for(unsigned i = 0; i < event_angles.size(); i++) {
		new_angle = flatten_angle(event_angles[i], A, B, nMin, nMax, x_min, x_max);
		flat->Fill(new_angle);
	}

	return(flat);
}
