
#include <iostream>
#include <vector>

#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TMath.h>
#include <TStyle.h>

using namespace std;

void Plot_BES1_Inefficiencies() {
	string root_path = "/media/dylan/SSD_Storage/Research/Data_Old_Ref2/Single_Ratio0/";
	vector<int> energies = {7, 11, 19, 27, 39, 62};
	vector<TFile*> files;
	vector<TH1D*> hists;
	vector<TLine*> lines;
	TCanvas *can = new TCanvas("Efficiencies_Canvas", "Data Efficiencies");
	gStyle->SetTitleFontSize(0.09);
	can->Divide(3, 2);
	int can_index = 1;

	for(int energy:energies) {
		can->cd(can_index);
		files.push_back(new TFile((root_path + to_string(energy) + "GeV/QA_" + to_string(energy) + "GeV.root").data(), "READ"));

		hists.push_back((TH1D*)files.back()->Get(("post_phi_Single_Ratio0_" + to_string(energy)).data()));
		hists.back()->Scale(1.0/hists.back()->GetMaximum());
//		hists.back()->SetTitleSize(0.09);
		hists.back()->SetNameTitle((to_string(energy) + "GeV Efficiency").data(), (to_string(energy) + "GeV").data());
		hists.back()->GetXaxis()->SetLabelSize(0.06);
		hists.back()->GetYaxis()->SetLabelSize(0.06);
		hists.back()->GetYaxis()->SetLabelOffset(0.015);
		hists.back()->GetYaxis()->SetRangeUser(0,1);

		if(can_index % 3 == 1) { hists.back()->GetYaxis()->SetTitle("Efficiency"); hists.back()->GetYaxis()->SetTitleSize(0.06); hists.back()->GetYaxis()->SetTitleOffset(1.25); gPad->SetLeftMargin(0.15); }
		if(can_index > 3) { hists.back()->GetXaxis()->SetTitle("Aszimuthal Angle (radians)");  hists.back()->GetXaxis()->SetTitleSize(0.06); hists.back()->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else {gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.09); gPad->SetRightMargin(0.03);
		gStyle->SetOptStat("");

		lines.push_back(new TLine(0, 1, 2*TMath::Pi(), 1));
		lines.back()->SetLineColor(kBlack);

		hists.back()->Draw("HIST");
		lines.back()->Draw("Same");

		can_index++;
	}

	cout << "donzo" << endl;
}
