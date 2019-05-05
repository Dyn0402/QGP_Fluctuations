/*
 * flatten.cpp
 *
 *  Created on: May 5, 2019
 *      Author: dylan
 */


#include <vector>
#include <TH1D.h>
#include <TMath.h>
#include <TRandom.h>

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


//Calculate coefficients of Fourier expansion.
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int nMin, int nMax){
	vector<double> B, A;

	for(int n = nMin; n <= nMax; n++) {
		B.push_back(getAvg(hist, TMath::Cos, n) * 2.0 / n);
		A.push_back(getAvg(hist, TMath::Sin, n) * -2.0 / n);
	}

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
