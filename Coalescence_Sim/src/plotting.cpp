/*
 * plotting.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: Dylan Neff
 */

#include <vector>
#include <map>

#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>

#include "plotting.h"
#include "ratio_methods.h"
#include "config.h"

using namespace std;


void plot_Rs(vector<double> Rs) {
	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
	for(auto r:Rs) {
//		cout << r << endl;
		config::R_hist->Fill(r);
	}
	config::R_hist->Write();
	out_file->Close();
	delete config::R_hist;
	delete out_file;
}


void plot_corrs(vector<double> corrs) {
	TFile *out_file = new TFile(config::out_path.data(), "UPDATE");
	for(double c:corrs) {
		if(c > 2*M_PI - M_PI / 2) { c-=2*M_PI; }
		config::corr_hist->Fill(c);
	}
	config::corr_hist->Write();
	out_file->Close();
	delete config::corr_hist;
	delete out_file;
}


void plot_cumulants_vs_peffect(map<double, map<int, tuple<double,double>>> cumulants_with_peffect) {
	TCanvas *can = new TCanvas();
	can->SetTitle("Cumulants vs p_effect");
	can->SetName("Cumulants vs p_effect");
	can->Divide(ceil(pow(config::cumulant_orders.size(),0.5)), ceil(config::cumulant_orders.size()/ceil(pow(config::cumulant_orders.size(), 0.5))));
	int can_index = 1;
	for(int order:config::cumulant_orders) {
		can->cd(can_index);
		vector<double> p_effect, cumulant, cumulant_err;
		double cum_val, cum_err;
		for(pair<double, map<int, tuple<double, double>>> effect:cumulants_with_peffect) {
			p_effect.push_back(effect.first);
			tie(cum_val, cum_err) = effect.second[order];
			cumulant.push_back(cum_val);
			cumulant_err.push_back(cum_err);
		}
		TGraphErrors *graph = new TGraphErrors((int)p_effect.size(), p_effect.data(), cumulant.data(), 0, cumulant_err.data());
		graph->SetNameTitle(config::cumulant_name[order].data(), config::cumulant_name[order].data());
		graph->SetMarkerStyle(8);
		graph->SetMarkerColor(kRed);
		graph->SetMarkerSize(1);
		graph->Draw("AP");
		can_index++;
	}
	can->Write("Cumulants vs p_effect");
	delete can;
}


void hist_ratio_dist(vector<double> ratios, string mode) {
	string name = "Ratio hist p_effect: " +to_string(config::p_effect);
	TH1D *ratio_hist = new TH1D(name.data(), name.data(), 23, -0.05, 1.1);
	for(double ratio:ratios) {
		ratio_hist->Fill(ratio);
	}
	if(mode == "write") {
		ratio_hist->Write();
		delete ratio_hist;
	} else if(mode == "draw") {
		ratio_hist->Draw();
	}
}



void hist_proton_dist(vector<int> nprotons, string mode) {
	string name = "Proton dist p_effect: " +to_string(config::p_effect);
	TH1D *protons_hist = new TH1D(name.data(), name.data(), 31, -0.5, 30.5);
	for(double protons:nprotons) {
		protons_hist->Fill(protons);
	}
	if(mode == "write") {
		protons_hist->Write();
		delete protons_hist;
	} else if(mode == "draw") {
		protons_hist->Draw();
	}
}
