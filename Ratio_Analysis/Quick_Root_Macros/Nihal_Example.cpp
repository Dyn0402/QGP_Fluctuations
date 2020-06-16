
#include <iostream>
#include <map>

#include <TH1.h>
#include <TF1.h>
#include <TLegend.h>

using namespace std;

void Nihal_Example() {
	map<int, double> hist_vals {{0,3}, {1,4}, {2,6}, {3,11}, {4,22}, {5,41}, {6,67}, {7,97}, {8,124}, {9,138}, {10,134},
		{11,114}, {12,85}, {13,58}, {14,51}, {15,85}, {16,133}, {17,121}, {18,59}, {19,16}, {20,4}, {21,3}, {22,3}, {23,3}, {24,3}};

	TH1D *hist = new TH1D("Example", "Example Hist", 25, -0.5, 24.5);
	for(auto &val:hist_vals) {
		hist->Fill(val.first, val.second);
	}

	TF1 *fit_func = new TF1("fit_func", "[0]+gaus(1)+gaus(4)", 0, 24);
	fit_func->SetParameters(1, 100, 10, 2, 100, 10, 2);
	hist->Fit(fit_func, "N");

	TF1 *bkg = new TF1("bkg", "[0]", 0, 24);
	TF1 *gaus1 = new TF1("gaus1", "gaus", 0, 24);
	TF1 *gaus2 = new TF1("gaus2", "gaus", 0, 24);

	bkg->SetParameter(0, fit_func->GetParameter(0));
	gaus1->SetParameters(fit_func->GetParameter(1), fit_func->GetParameter(2), fit_func->GetParameter(3));
	gaus2->SetParameters(fit_func->GetParameter(4), fit_func->GetParameter(5), fit_func->GetParameter(6));

	bkg->SetLineStyle(9);
	gaus1->SetLineStyle(9);
	gaus2->SetLineStyle(9);

	bkg->SetLineColor(kBlue);
	gaus1->SetLineColor(kGreen+2);
	gaus2->SetLineColor(kViolet);

	hist->Draw("HIST");
	fit_func->Draw("SAME");
	bkg->Draw("SAME");
	gaus1->Draw("SAME");
	gaus2->Draw("SAME");

	TLegend *legend = new TLegend();
	legend->SetFillStyle(0); legend->SetBorderSize(0);
	legend->AddEntry(fit_func, "Fitting Function");
	legend->AddEntry(bkg, "Constant Background");
	legend->AddEntry(gaus1, "First Gaussian");
	legend->AddEntry(gaus2, "Second Gaussian");
	legend->Draw("SAME");

	cout << "donzo" << endl;
}
