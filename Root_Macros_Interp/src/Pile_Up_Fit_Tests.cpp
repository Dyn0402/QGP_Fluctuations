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
#include "TLegend.h"
#include "TPaveText.h"

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
	return par[0] / (1 + exp(-(x[0] - par[1]) / par[2]));
}

double gexp_r_plus_ws(double *x, double *par) {  // par = {amplitude, x_bar, sigma, k, ws_amp, ws_width, ws_x}
	double *par_ws = par + 4;
	return gaus_exp_r(x, par) + woods_saxon(x, par_ws);
}


void Pile_Up_Fit_Tests() {
	string file_path = "/home/dylan/Research/Pile_Up_QA_Tests/62_slice_230-235.root";  // "/home/dylan/Research/Pile_Up_QA_Tests/Pile_QA_62GeV.root";
	string hist_name = "62GeV Slice 230.000000 to 235.000000";  // "Slice Hists/62GeV Slice 230.000000 to 235.000000"
	TFile *file = new TFile(file_path.data(), "READ");

	TCanvas *can = new TCanvas("Fit Canvas", "Fit Canvas");

	TH1F *slice = (TH1F*)file->Get(hist_name.data())->Clone("Slice");

	TF1 *fit2 = new TF1("fit2", "crystalball(0)+[5]/(1+exp(-(x-[7])/[6]))", -30, 150);
	fit2->SetLineColor(kBlue);
	fit2->SetParameters(24123.7, -2.1, 5.87, -5, 0.5, 3.11, 5.66, 50);
	slice->Fit(fit2, "NR");

	TF1 *fit = new TF1("fit", gexp_r_plus_ws, -30, 150, 7);
	fit->SetParameters(24123.7, -2.1, 5.87, -1, 3.11, 5.66, 50);
	slice->Fit(fit, "NR");
//	fit->FixParameter(0, fit->GetParameter(0));
//	fit->FixParameter(1, fit->GetParameter(1));
//	fit->FixParameter(2, fit->GetParameter(2));
//	fit->FixParameter(3, fit->GetParameter(3));
//	fit->SetParameter(4, 4);
//	fit->SetParameter(5, 0);
//	fit->SetParameter(6, 40);
//	slice->Fit(fit, "NR");
	TF1 *gaus_exp = new TF1("gaus_exp", gaus_exp_r, -30, 150, 4);
	gaus_exp->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3));
	gaus_exp->SetLineColor(kGreen);
	TF1 *bkg = new TF1("ws", woods_saxon, -30, 150, 3);
	bkg->SetParameters(fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(6));
	bkg->SetLineColor(kViolet);

	slice->Draw();
	gaus_exp->Draw("Same");
	bkg->Draw("Same");
	fit->Draw("Same");
	fit2->Draw("Same");

	TPaveText *gaus_exp_pars = new TPaveText(0.12, 0.65, 0.34, 0.9, "NDC");
	gaus_exp_pars->AddText("Gaus_Exp Parameters");
	gaus_exp_pars->AddLine(.0, .8, 1., .8);
	gaus_exp_pars->AddText(("Amplitude: " + to_string(gaus_exp->GetParameter(0)) + " #pm " + to_string(fit->GetParError(0))).data());
	gaus_exp_pars->AddText(("Center: " + to_string(gaus_exp->GetParameter(1)) + " #pm " + to_string(fit->GetParError(1))).data());
	gaus_exp_pars->AddText(("Sigma: " + to_string(gaus_exp->GetParameter(2)) + " #pm " + to_string(fit->GetParError(2))).data());
	gaus_exp_pars->AddText(("k: " + to_string(gaus_exp->GetParameter(3)) + " #pm " + to_string(fit->GetParError(3))).data());
	gaus_exp_pars->Draw("Same");
	TPaveText *bkg_pars = new TPaveText(0.12, 0.44, 0.34, 0.64, "NDC");
	bkg_pars->AddText("Woods Saxon Parameters");
	bkg_pars->AddLine(.0, .75, 1., .75);
	bkg_pars->AddText(("Amplitude: " + to_string(bkg->GetParameter(0)) + " #pm " + to_string(fit->GetParError(4))).data());
	bkg_pars->AddText(("Center: " + to_string(bkg->GetParameter(1)) + " #pm " + to_string(fit->GetParError(5))).data());
	bkg_pars->AddText(("Width: " + to_string(bkg->GetParameter(2)) + " #pm " + to_string(fit->GetParError(6))).data());
	bkg_pars->Draw("Same");
	TLegend *leg = new TLegend(0.35, 0.2);
	leg->AddEntry(slice, "Slice Distribution", "l");
	leg->AddEntry(fit, "Full Fit", "l");
	leg->AddEntry(gaus_exp, "Gaus_Exp Contribution", "l");
	leg->AddEntry(bkg, "Woods Saxon Contribution", "l");
	leg->Draw();

	can->SetLogy();
}
