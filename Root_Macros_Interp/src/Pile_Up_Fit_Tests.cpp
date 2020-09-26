/*
 * Pile_Up_Fit_Tests.cpp
 *
 *  Created on: Sep 25, 2020
 *      Author: Dylan Neff
 */

#include <string>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"

using namespace std;


double gaus_exp_r(double *x, double *par) {  // par = {amplitude, x_bar, sigma, k}
	double x_prime = (x[0] - par[1]) / par[2];
	if(x_prime <= -par[3]) {
		return par[0] * exp(-pow(x_prime, 2) / 2);
	} else {
		return par[0] * exp(pow(par[3], 2) / 2 + par[3] * x_prime);
	}
}

double woods_saxon(double *x, double *par) {  // par = {amplitude, x_bar, width}
	return par[0] / (1 + exp(-par[2] * (x[0] - par[1])));
}

double gexp_r_plus_ws(double *x, double *par) {  // par = {amplitude, x_bar, sigma, k, ws_amp, ws_width, ws_x}
	double *par_ws = par + 4;
	return gaus_exp_r(x, par) + woods_saxon(x, par_ws);
}


void Pile_Up_Fit_Tests() {
	string file_path = "/home/dylan/Research/Pile_Up_QA_Tests/62_slice_230-235.root";  // "/home/dylan/Research/Pile_Up_QA_Tests/Pile_QA_62GeV.root";
	string hist_name = "62GeV Slice 230.000000 to 235.000000";  // "Slice Hists/62GeV Slice 230.000000 to 235.000000"
	TFile *file = new TFile(file_path.data(), "READ");
	TCanvas *can = new TCanvas();
	TH1F *slice = (TH1F*)file->Get(hist_name.data())->Clone("Slice");
	TF1 *fit = new TF1("fit", gexp_r_plus_ws, -30, 150, 7);
	fit->SetParameters(24123.7, -2.1, 5.87, -1.54, 3.11, 5.66, 59.27);
	slice->Fit(fit, "R");
	TF1 *gaus_exp = new TF1();
	TF1 *bkg = new TF1();
	can->SetLogy();
//	TCanvas *can = new TCanvas;
	slice->Draw();
}
