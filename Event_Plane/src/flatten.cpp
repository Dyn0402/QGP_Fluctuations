/*
 * flatten.cpp
 *
 *  Created on: May 5, 2019
 *      Author: dylan
 */


#include <vector>
#include <TF1.h>
#include <TH1D.h>
#include <TMath.h>
#include <TRandom.h>
#include <TRandom3.h>

using namespace std;


//Get average of function f on histogram hist.
double getAvg(TH1D* hist, double (*f)(double), int n) {
	int norm = 0;
	double sum = 0.0;
	int binCont;
	double binCent;

	for(int i = hist->GetXaxis()->GetFirst(); i<=hist->GetXaxis()->GetLast(); i++) {
		binCont = hist->GetBinContent(i);
		binCent = hist->GetBinCenter(i);
		norm += binCont;
		sum += binCont * (*f)(n*binCent);
	}

	return(sum / norm);
}


//Get average of TF1 func on histogram hist.
double getAvg2(TH1D *hist, TF1 *func, int n) {
	int norm = 0;
	double sum = 0.0;
	int binCont;
	double binCent;

	for(int i = hist->GetXaxis()->GetFirst(); i<=hist->GetXaxis()->GetLast(); i++) {
		binCont = hist->GetBinContent(i);
		binCent = hist->GetBinCenter(i);
		norm += binCont;
		sum += binCont * func->Eval(n*binCent);
	}

	return(sum / norm);
}




//Calculate coefficients of Fourier expansion.
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int nMin, int nMax){
	vector<double> B, A;

	for(int n = nMin; n <= nMax; n++) {
		B.push_back(getAvg(hist, TMath::Cos, n) * 2.0 / n);
		A.push_back(getAvg(hist, TMath::Sin, n) * -2.0 / n);
	}

	return(make_tuple(A, B));
}


//Calculate coefficients of Fourier expansion.
tuple<vector<double>, vector<double>> calcCoef2(TH1D* hist, int nMin, int nMax, double x_min, double x_max){
	vector<double> B, A;

	//Get max and min of range for histogram. Need to transform to -pi to +pi range.
//	double x_max = hist->GetXaxis()->GetXmax();
//	double x_min = hist->GetXaxis()->GetXmin();

	double x_avg = (x_max + x_min) / 2; //Calculate average of range
	double L = (x_max - x_min) / 2; //Calculate half range (2*L is full range)

	TF1 *cos_shift = new TF1("cos_shift", "cos([2] * (x-[0]) / [1])");
	cos_shift->SetParameters(x_avg, L, TMath::Pi());
	TF1 *sin_shift = new TF1("sin_shift", "sin([2] * (x-[0]) / [1])");
	sin_shift->SetParameters(x_avg, L, TMath::Pi());

	for(int n = nMin; n <= nMax; n++) {
		B.push_back(getAvg2(hist, cos_shift, n) * 2.0 / n);
		A.push_back(getAvg2(hist, sin_shift, n) * -2.0 / n);
	}

	delete cos_shift;
	delete sin_shift;

	return(make_tuple(A, B));
}


TH1D* genFlatHist(TH1D* hist, vector<double> A, vector<double> B, int nMin, int nMax, string flatName) {
	int norm = hist->Integral();
	int bins = hist->GetXaxis()->GetLast() - hist->GetXaxis()->GetFirst();
	double low = hist->GetBinLowEdge(hist->GetXaxis()->GetFirst());
	double up = hist->GetBinLowEdge(hist->GetXaxis()->GetLast()+1);
	double newAngle;
	double oldAngle;

	TH1D* flatHist = new TH1D(flatName.c_str(), flatName.c_str(), bins, low, up);


	gRandom->SetSeed(0);
	for(int i=0; i < norm; i++) {
		oldAngle = hist->GetRandom();
		newAngle = oldAngle;
		for(int n = nMin; n <= nMax; n++) {
			newAngle += A[n-nMin] * TMath::Cos(n*oldAngle) + B[n-nMin] * TMath::Sin(n*oldAngle);
		}
		while(newAngle > up) {newAngle -= (up-low);}
		while(newAngle < low) {newAngle += (up-low);}

		flatHist->Fill(newAngle);
	}

	return(flatHist);
}


TH1D* genFlatHist2(TH1D* hist, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName) {
	int norm = hist->Integral();
	int bins = hist->GetXaxis()->GetLast() - hist->GetXaxis()->GetFirst();
	double low = hist->GetBinLowEdge(hist->GetXaxis()->GetFirst());
	double up = hist->GetBinLowEdge(hist->GetXaxis()->GetLast()+1);
	double newAngle;
	double oldAngle;

	TH1D* flatHist = new TH1D(flatName.c_str(), flatName.c_str(), bins, low, up);

	double x_avg = (x_max + x_min) / 2; //Calculate average of range
	double L = (x_max - x_min) / 2; //Calculate half range (2*L is full range)

	gRandom->SetSeed(0);
	for(int i=0; i < norm; i++) {
		oldAngle = hist->GetRandom();
		newAngle = oldAngle;
		for(int n = nMin; n <= nMax; n++) {
			newAngle += A[n-nMin] * TMath::Cos(n*TMath::Pi()*(oldAngle - x_avg) / L) + B[n-nMin] * TMath::Sin(n*TMath::Pi()*(oldAngle - x_avg) / L);
		}
		while(newAngle > up) {newAngle -= (up-low);}
		while(newAngle < low) {newAngle += (up-low);}

		flatHist->Fill(newAngle);
	}

	return(flatHist);
}


TH1D* flatten_dist(vector<double> event_angles, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName) {
	double new_angle, old_angle;
	TH1D *flat = new TH1D(flatName.data(), flatName.data(), 100, x_min, x_max);

	double x_avg = (x_max + x_min) / 2; //Calculate average of range
	double L = (x_max - x_min) / 2; //Calculate half range (2*L is full range)

	for(unsigned i = 0; i < event_angles.size(); i++) {
		old_angle = TMath::Pi() * (event_angles[i] - x_avg) / L;
		new_angle = old_angle;
		for(int n = nMin; n <= nMax; n++) {
			new_angle += A[n-nMin] * TMath::Cos(n * old_angle) + B[n-nMin] * TMath::Sin(n * old_angle);
		}
		new_angle = L * new_angle / TMath::Pi() + x_avg;
		flat->Fill(new_angle);
	}

	return(flat);
}


void write_coefs() {
	//
}


tuple<vector<double>, vector<double>> read_coefs() {
	//
}

//Declare and set the initial dGaus histogram.
TH1D* setDGausHist(int bins, double low, double up, string name) {
	TH1D* hist = new TH1D(name.c_str(), name.c_str(), bins, low, up);

	return(hist);
}


//Fill hist with two gaussians.
TH1D* genDGausHist(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms) {
	int first = hist->GetXaxis()->GetFirst();
	int last = hist->GetXaxis()->GetLast();
	int bins = last - first;
	double low = hist->GetBinLowEdge(first);
	double up = hist->GetBinLowEdge(last+1);

	TRandom3* r = new TRandom3(0);
	double rand;

	for(int i=0; i<weight.size(); i++) {
		for(int j=0; j<N*weight[i]; j++) {
			rand = r->Gaus(mean[i], rms[i]);
			while(rand > up) {rand -= (up-low);}
			while(rand < low) {rand += (up-low);}
			hist->Fill(rand);
		}
	}

	return(hist);
}


vector<double> genDGausHist2(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms) {
	int first = hist->GetXaxis()->GetFirst();
	int last = hist->GetXaxis()->GetLast();
	int bins = last - first;
	double low = hist->GetBinLowEdge(first);
	double up = hist->GetBinLowEdge(last+1);

	TRandom3* r = new TRandom3(0);
	double rand;
	vector<double> angles;

	for(int i=0; i<weight.size(); i++) {
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
