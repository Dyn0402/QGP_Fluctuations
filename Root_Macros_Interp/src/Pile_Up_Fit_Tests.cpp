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

double exp_woods_saxon(double *x, double *par) {  // par = {amplitude, x_bar, width, decay_frac}
	return woods_saxon(x, par) * exp(-par[3] / (2 * par[2]) * (x[0] - par[1]));
}

double lin_woods_saxon(double *x, double *par) {  // par = {amplitude, x_bar, width, slope}
	return (par[0] + par[3] * (x[0] - par[1])) / (1 + exp(-(x[0] - par[1]) / par[2]));
}

double gexp_r_plus_ws(double *x, double *par) {  // par = {amplitude, x_bar, sigma, k, ws_amp, ws_xbar, ws_width}
	double *par_ws = par + 4;
	return gaus_exp_r(x, par) + woods_saxon(x, par_ws);
}

double gexp_r_plus_expws(double *x, double *par) {  // par = {amplitude, x_bar, sigma, k, exp*ws_amp, ws_exp_xbar, ws_width, exp_decay_frac}
	double *par_exp_ws = par + 4;
	return gaus_exp_r(x, par) + exp_woods_saxon(x, par_exp_ws);
}

double gexp_r_plus_linws(double *x, double *par) {  // par = {amplitude, x_bar, sigma, k, exp*ws_amp, ws_exp_xbar, ws_width, slope}
	double *par_exp_ws = par + 4;
	return gaus_exp_r(x, par) + lin_woods_saxon(x, par_exp_ws);
}


void Pile_Up_Fit_Tests() {
	string file_path = "/home/dylan/Desktop/Pile_Up_QA_Tests/Pile_QA_62GeV.root";
	string hist_name = "Slice Hists/62GeV Slice 35.000000 to 40.000000";
//	string hist_name = "Slice Hists/11GeV Slice 150.000000 to 155.000000";
	TFile *file = new TFile(file_path.data(), "READ");

	TCanvas *can = new TCanvas("Fit Canvas", "Fit Canvas");

	TH1F *slice = (TH1F*)file->Get(hist_name.data())->Clone("Slice");

	int first_bin = slice->FindFirstBinAbove();
	int last_bin = slice->FindLastBinAbove();
	for(int bin=first_bin; bin < last_bin; bin++) {
		if(slice->GetBinContent(bin) == 0) { slice->SetBinError(bin, 1); }
	}
	float low_bound = slice->GetBinCenter(first_bin);
	float high_bound = slice->GetBinCenter(last_bin);

//	TF1 *fit2 = new TF1("fit2", "gaus(0)+[4]*exp(-[5]*(x-[3]))/(1+exp(-(x-[3])/[9]))+[7]/(1+exp(-(x-[6])/[8]))", -30, 150);
//	fit2->SetLineColor(kRed);
//	fit2->SetParameters(24123.7, -2.1, 5.87, 15, 1000, 0.2, 10, 5, 2, 0.1);
////	fit2->SetParameters(5964, -1, 4.7, 5, 660, 0.24, 15, 5, 0.5);
//	slice->Fit(fit2, "NR");
//
//	TF1 *gaus2 = new TF1("gaus2", "gaus", -30, 150);
//	gaus2->SetParameters(fit2->GetParameter(0), fit2->GetParameter(1), fit2->GetParameter(2));
//	gaus2->SetLineColor(kGreen); gaus2->SetNpx(1000);
//	TF1 *bkg_exp1 = new TF1("bkg_exp1", "[1]*exp(-[2]*(x-[0]))/(1+exp(-(x-[0])/[3]))", -30, 150);
//	bkg_exp1->SetParameters(fit2->GetParameter(3), fit2->GetParameter(4), fit2->GetParameter(5), fit2->GetParameter(9));
//	bkg_exp1->SetLineColor(kBlue); bkg_exp1->SetNpx(1000);
//	TF1 *bkg_exp2 = new TF1("bkg_exp2", "[1]/(1+exp(-(x-[0])/[2]))", -30, 150);
//	bkg_exp2->SetParameters(fit2->GetParameter(6), fit2->GetParameter(7), fit2->GetParameter(8));
//	bkg_exp2->SetLineColor(kViolet); bkg_exp2->SetNpx(1000);

//	TF1 *fit3 = new TF1("fit2", "gaus(0)+([3]+[4]*exp(-[5]*(x-[6])))/(1+exp(-(x-[6])/[7]))", -30, 150);
//	fit3->SetLineColor(kRed);
//	fit3->SetParameters(24123.7, -2.1, 5.87, 4, 3000, 0.2, 0, 50);
////	fit3->SetParameters(5883, -0.58, 4.9, 0.5, 100, 0.5, 20, 2);
//	slice->Fit(fit3, "NR");
//
//	TF1 *gaus3 = new TF1("gaus3", "gaus", -30, 150);
//	gaus3->SetParameters(fit3->GetParameter(0), fit3->GetParameter(1), fit3->GetParameter(2));
//	gaus3->SetLineColor(kGreen); gaus3->SetNpx(1000);
//	TF1 *bkg3 = new TF1("bkg3", "([0]+[1]*exp(-[2]*(x-[3])))/(1+exp(-(x-[3])/[4]))", -30, 150);
//	bkg3->SetParameters(fit3->GetParameter(3), fit3->GetParameter(4), fit3->GetParameter(5), fit3->GetParameter(6), fit3->GetParameter(7));
//	bkg3->SetLineColor(kBlue); bkg3->SetNpx(1000);

//	TF1 *fit = new TF1("fit", gexp_r_plus_ws, -30, 150, 7);
//	fit->SetParameters(24123.7, -2.1, 5.87, -1, 3.11, 5.66, 50);
//	fit->SetParameters(24123.7, -2.1, 5.87, -0.1, 3.11, 5.66, 50);
//	slice->Fit(fit, "NR");
//	fit->FixParameter(0, fit->GetParameter(0));
//	fit->FixParameter(1, fit->GetParameter(1));
//	fit->FixParameter(2, fit->GetParameter(2));
//	fit->FixParameter(3, fit->GetParameter(3));
//	fit->SetParameter(4, 4);
//	fit->SetParameter(5, 0);
//	fit->SetParameter(6, 40);
//	slice->Fit(fit, "NR");
//	TF1 *gaus_exp = new TF1("gaus_exp", gaus_exp_r, -30, 150, 4);
//	gaus_exp->SetParameters(fit->GetParameter(0), fit->GetParameter(1), fit->GetParameter(2), fit->GetParameter(3));
//	gaus_exp->SetLineColor(kGreen);
//	TF1 *bkg = new TF1("ws", woods_saxon, -30, 150, 3);
//	bkg->SetParameters(fit->GetParameter(4), fit->GetParameter(5), fit->GetParameter(6));
//	bkg->SetLineColor(kViolet);

	TF1 *fit4 = new TF1("fit4", gexp_r_plus_linws, low_bound, high_bound, 8);
	fit4->SetLineColor(kRed);
	fit4->SetParameters(209622, -0.57, 2.177, -1.67, 21.756, 8.982, 1.65, -0.5);
//	fit4->SetParameters(46245, -1.87, 4.5, -1.67, 10, 20, 1.65, -0.05);
//	fit4->SetParLimits(7, -100, -0.02);
//	fit2->SetParameters(5964, -1, 4.7, 5, 660, 0.24, 15, 5, 0.5);
	slice->Fit(fit4, "NR");

	TF1 *gaus_exp4 = new TF1("gaus_exp4", gaus_exp_r, -30, 150, 4);
	gaus_exp4->SetParameters(fit4->GetParameter(0), fit4->GetParameter(1), fit4->GetParameter(2), fit4->GetParameter(3));
	gaus_exp4->SetLineColor(kGreen); gaus_exp4->SetNpx(1000);
	TF1 *bkg_exp4 = new TF1("bkg_exp4", lin_woods_saxon, -30, 150, 4);
	bkg_exp4->SetParameters(fit4->GetParameter(4), fit4->GetParameter(5), fit4->GetParameter(6), fit4->GetParameter(7));
	bkg_exp4->SetLineColor(kBlue); bkg_exp4->SetNpx(1000);

	slice->Draw("HIST");
//	gaus_exp->Draw("Same");
//	bkg->Draw("Same");
//	fit->Draw("Same");
//	fit2->Draw("Same");
//	gaus2->Draw("Same");
//	bkg_exp1->Draw("Same");
//	bkg_exp2->Draw("Same");
//	fit3->Draw("Same");
//	gaus3->Draw("Same");
//	bkg3->Draw("Same");
	fit4->Draw("same");
	gaus_exp4->Draw("same");
	bkg_exp4->Draw("same");

//	TPaveText *gaus_exp_pars = new TPaveText(0.12, 0.65, 0.34, 0.9, "NDC");
//	gaus_exp_pars->AddText("Gaus_Exp Parameters");
//	gaus_exp_pars->AddLine(.0, .8, 1., .8);
//	gaus_exp_pars->AddText(("Amplitude: " + to_string(gaus_exp->GetParameter(0)) + " #pm " + to_string(fit->GetParError(0))).data());
//	gaus_exp_pars->AddText(("Center: " + to_string(gaus_exp->GetParameter(1)) + " #pm " + to_string(fit->GetParError(1))).data());
//	gaus_exp_pars->AddText(("Sigma: " + to_string(gaus_exp->GetParameter(2)) + " #pm " + to_string(fit->GetParError(2))).data());
//	gaus_exp_pars->AddText(("k: " + to_string(gaus_exp->GetParameter(3)) + " #pm " + to_string(fit->GetParError(3))).data());
//	gaus_exp_pars->Draw("Same");
//	TPaveText *bkg_pars = new TPaveText(0.12, 0.44, 0.34, 0.64, "NDC");
//	bkg_pars->AddText("Woods Saxon Parameters");
//	bkg_pars->AddLine(.0, .75, 1., .75);
//	bkg_pars->AddText(("Amplitude: " + to_string(bkg->GetParameter(0)) + " #pm " + to_string(fit->GetParError(4))).data());
//	bkg_pars->AddText(("Center: " + to_string(bkg->GetParameter(1)) + " #pm " + to_string(fit->GetParError(5))).data());
//	bkg_pars->AddText(("Width: " + to_string(bkg->GetParameter(2)) + " #pm " + to_string(fit->GetParError(6))).data());
//	bkg_pars->Draw("Same");
//	TLegend *leg = new TLegend(0.35, 0.2);
//	leg->AddEntry(slice, "Slice Distribution", "l");
//	leg->AddEntry(fit, "Full Fit", "l");
//	leg->AddEntry(gaus_exp, "Gaus_Exp Contribution", "l");
//	leg->AddEntry(bkg, "Woods Saxon Contribution", "l");
//	leg->Draw();

//	TPaveText *gaus_pars = new TPaveText(0.12, 0.65, 0.34, 0.9, "NDC");
//	gaus_pars->AddText("Gaus Parameters");
//	gaus_pars->AddLine(.0, .75, 1., .75);
//	gaus_pars->AddText(("Amplitude: " + to_string(fit2->GetParameter(0)) + " #pm " + to_string(fit2->GetParError(0))).data());
//	gaus_pars->AddText(("Center: " + to_string(fit2->GetParameter(1)) + " #pm " + to_string(fit2->GetParError(1))).data());
//	gaus_pars->AddText(("Sigma: " + to_string(fit2->GetParameter(2)) + " #pm " + to_string(fit2->GetParError(2))).data());
//	gaus_pars->Draw("Same");
//	TPaveText *exp1_pars = new TPaveText(0.12, 0.44, 0.34, 0.64, "NDC");
//	exp1_pars->AddText("Exp1 Parameters");
//	exp1_pars->AddLine(.0, .8, 1., .8);
//	exp1_pars->AddText(("Amplitude: " + to_string(fit2->GetParameter(4)) + " #pm " + to_string(fit2->GetParError(4))).data());
//	exp1_pars->AddText(("Center: " + to_string(fit2->GetParameter(3)) + " #pm " + to_string(fit2->GetParError(3))).data());
//	exp1_pars->AddText(("Decay: " + to_string(fit2->GetParameter(5)) + " #pm " + to_string(fit2->GetParError(5))).data());
//	exp1_pars->AddText(("WS Width: " + to_string(fit2->GetParameter(9)) + " #pm " + to_string(fit2->GetParError(9))).data());
//	exp1_pars->Draw("Same");
//	TPaveText *exp2_pars = new TPaveText(0.12, 0.23, 0.34, 0.43, "NDC");
//	exp2_pars->AddText("Exp2 Parameters");
//	exp2_pars->AddLine(.0, .75, 1., .75);
//	exp2_pars->AddText(("Amplitude: " + to_string(fit2->GetParameter(7)) + " #pm " + to_string(fit2->GetParError(7))).data());
//	exp2_pars->AddText(("Start: " + to_string(fit2->GetParameter(6)) + " #pm " + to_string(fit2->GetParError(6))).data());
//	exp2_pars->AddText(("WS Width: " + to_string(fit2->GetParameter(8)) + " #pm " + to_string(fit2->GetParError(8))).data());
//	exp2_pars->Draw("Same");
//	TLegend *leg = new TLegend(0.35, 0.2);
//	leg->AddEntry(slice, "Slice Distribution", "l");
//	leg->AddEntry(fit2, "Full Fit", "l");
//	leg->AddEntry(gaus2, "Gaus Contribution", "l");
//	leg->AddEntry(bkg_exp1, "Exp1 Contribution", "l");
//	leg->AddEntry(bkg_exp2, "Exp2 Contribution", "l");
//	leg->Draw();

//	TPaveText *gaus_pars = new TPaveText(0.12, 0.65, 0.34, 0.9, "NDC");
//	gaus_pars->AddText("Gaus Parameters");
//	gaus_pars->AddLine(.0, 0.75, 1., 0.75);
//	gaus_pars->AddText(("Amplitude: " + to_string(fit3->GetParameter(0)) + " #pm " + to_string(fit3->GetParError(0))).data());
//	gaus_pars->AddText(("Center: " + to_string(fit3->GetParameter(1)) + " #pm " + to_string(fit3->GetParError(1))).data());
//	gaus_pars->AddText(("Sigma: " + to_string(fit3->GetParameter(2)) + " #pm " + to_string(fit3->GetParError(2))).data());
//	gaus_pars->Draw("Same");
//	TPaveText *bkg_pars = new TPaveText(0.12, 0.3, 0.34, 0.64, "NDC");
//	bkg_pars->AddText("(Const+Exp)*Woods Saxon Parameters");
//	bkg_pars->AddLine(.0, 0.8333, 1., 0.8333);
//	bkg_pars->AddText(("Const: " + to_string(fit3->GetParameter(4)) + " #pm " + to_string(fit3->GetParError(4))).data());
//	bkg_pars->AddText(("Exp Amp: " + to_string(fit3->GetParameter(3)) + " #pm " + to_string(fit3->GetParError(3))).data());
//	bkg_pars->AddText(("Exp Decay: " + to_string(fit3->GetParameter(5)) + " #pm " + to_string(fit3->GetParError(5))).data());
//	bkg_pars->AddText(("Exp&WS Center: " + to_string(fit3->GetParameter(6)) + " #pm " + to_string(fit3->GetParError(6))).data());
//	bkg_pars->AddText(("WS Width: " + to_string(fit3->GetParameter(7)) + " #pm " + to_string(fit3->GetParError(7))).data());
//	bkg_pars->Draw("Same");
//	TLegend *leg = new TLegend(0.35, 0.2);
//	leg->AddEntry(slice, "Slice Distribution", "l");
//	leg->AddEntry(fit3, "Full Fit", "l");
//	leg->AddEntry(gaus3, "Gaus Contribution", "l");
//	leg->AddEntry(bkg3, "Bkg Contribution", "l");
//	leg->Draw();

//	TPaveText *gaus_pars = new TPaveText(0.12, 0.65, 0.34, 0.9, "NDC");
//	gaus_pars->AddText("Gaus Parameters");
//	gaus_pars->AddLine(.0, .75, 1., .75);
//	gaus_pars->AddText(("Amplitude: " + to_string(fit2->GetParameter(0)) + " #pm " + to_string(fit2->GetParError(0))).data());
//	gaus_pars->AddText(("Center: " + to_string(fit2->GetParameter(1)) + " #pm " + to_string(fit2->GetParError(1))).data());
//	gaus_pars->AddText(("Sigma: " + to_string(fit2->GetParameter(2)) + " #pm " + to_string(fit2->GetParError(2))).data());
//	gaus_pars->Draw("Same");
//	TPaveText *exp1_pars = new TPaveText(0.12, 0.44, 0.34, 0.64, "NDC");
//	exp1_pars->AddText("Exp1 Parameters");
//	exp1_pars->AddLine(.0, .8, 1., .8);
//	exp1_pars->AddText(("Amplitude: " + to_string(fit2->GetParameter(4)) + " #pm " + to_string(fit2->GetParError(4))).data());
//	exp1_pars->AddText(("Center: " + to_string(fit2->GetParameter(3)) + " #pm " + to_string(fit2->GetParError(3))).data());
//	exp1_pars->AddText(("Decay: " + to_string(fit2->GetParameter(5)) + " #pm " + to_string(fit2->GetParError(5))).data());
//	exp1_pars->AddText(("WS Width: " + to_string(fit2->GetParameter(9)) + " #pm " + to_string(fit2->GetParError(9))).data());
//	exp1_pars->Draw("Same");
//	TPaveText *exp2_pars = new TPaveText(0.12, 0.23, 0.34, 0.43, "NDC");
//	exp2_pars->AddText("Exp2 Parameters");
//	exp2_pars->AddLine(.0, .75, 1., .75);
//	exp2_pars->AddText(("Amplitude: " + to_string(fit2->GetParameter(7)) + " #pm " + to_string(fit2->GetParError(7))).data());
//	exp2_pars->AddText(("Start: " + to_string(fit2->GetParameter(6)) + " #pm " + to_string(fit2->GetParError(6))).data());
//	exp2_pars->AddText(("WS Width: " + to_string(fit2->GetParameter(8)) + " #pm " + to_string(fit2->GetParError(8))).data());
//	exp2_pars->Draw("Same");
//	TLegend *leg = new TLegend(0.35, 0.2);
//	leg->AddEntry(slice, "Slice Distribution", "l");
//	leg->AddEntry(fit2, "Full Fit", "l");
//	leg->AddEntry(gaus2, "Gaus Contribution", "l");
//	leg->AddEntry(bkg_exp1, "Exp1 Contribution", "l");
//	leg->AddEntry(bkg_exp2, "Exp2 Contribution", "l");
//	leg->Draw();

	can->SetLogy();
}
