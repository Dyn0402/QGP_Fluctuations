/*
 * fourier_approx.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: Dylan Neff
 */


#include <vector>
#include <tuple>

#include <TH1.h>
#include <TF1.h>

#include "fourier_approx.h"

using namespace std;


FourierApprox::FourierApprox(TH1D *hist) {
	hist_to_approx = hist;
	fourier_n_min = 1;
	fourier_n_max = 10;
}


FourierApprox::~FourierApprox() {
	//Nothing
}


TF1* get_approx() {
	//calcCoef(hist_to_approx, fourier_n_min, fourier_n_max, )
}

void FourierApprox::set_fourier_n_min(int n_min) {
	fourier_n_min = n_min;
}

void FourierApprox::set_fourier_n_max(int n_max) {
	fourier_n_max = n_max;
}

int FourierApprox::get_fourier_n_min() {
	return(fourier_n_min);
}

int FourierApprox::get_fourier_n_max() {
	return(fourier_n_max);
}


//Get average of TF1 func on histogram hist.
double getAvg(TH1D *hist, TF1 *func) {
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
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int n_min, int n_max, double x_min, double x_max) {
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
		B.push_back(getAvg(hist, cos_shift) * 2.0 / n);
		A.push_back(getAvg(hist, sin_shift) * -2.0 / n);
	}

	delete cos_shift;
	delete sin_shift;

	return(make_tuple(A, B));
}
