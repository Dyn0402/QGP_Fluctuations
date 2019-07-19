/*
 * plotting.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: Dylan Neff
 */

#include <vector>
#include <string>
#include <map>

#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>

#include "plotting.h"
#include "ratio_methods.h"
#include "Stats.h"
#include "config.h"

using namespace std;


void plot_Rs(vector<double> Rs) {
	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
	TH1D *R_hist = new TH1D("Ratios", "Ratio Distribution", 23, -0.05, 1.10);
	for(auto r:Rs) {
//		cout << r << endl;
		R_hist->Fill(r);
	}
	R_hist->Write();
	out_file->Close();
	delete R_hist;
	delete out_file;
}


void plot_corrs(vector<double> corrs) {
	TFile *out_file = new TFile(config::out_path.data(), "UPDATE");
	TH1D *corr_hist = new TH1D("Two_Particle_Correlations", "Two Particle Correlations", 1000, -M_PI/2, 2*M_PI-M_PI/2);
	for(double c:corrs) {
		if(c > 2*M_PI - M_PI / 2) { c-=2*M_PI; }
		corr_hist->Fill(c);
	}
	corr_hist->Write();
	out_file->Close();
	delete corr_hist;
	delete out_file;
}


void plot_stats_vs_x(map<double, map<string, measure<double>>> stats, string x_name) {
	TCanvas *can = new TCanvas(("Stats vs " + x_name).data(), ("Stats vs " + x_name).data(), config::can_w, config::can_h);
//	can->Divide(ceil(pow(config::statistics.size(),0.5)), ceil(config::statistics.size()/ceil(pow(config::statistics.size(), 0.5))));
	can->Divide(4,1);
	int can_index = 1;
	for(string stat:config::statistics) {
		can->cd(can_index);
		vector<double> x, statistic, statistic_err;
		for(pair<double, map<string, measure<double>>> effect:stats) {
			x.push_back(effect.first);
			measure<double> stat_measure = effect.second[stat];
			statistic.push_back(stat_measure.val);
			statistic_err.push_back(stat_measure.err);
		}
		TGraphErrors *graph = new TGraphErrors((int)x.size(), x.data(), statistic.data(), 0, statistic_err.data());
		graph->SetNameTitle(stat.data(), stat.data());
		graph->SetMarkerStyle(8);
		graph->SetMarkerColor(kRed);
		graph->SetMarkerSize(1);
		double x_min = *min_element(x.begin(), x.end());
		double x_max = *max_element(x.begin(), x.end());
		double x_range = x_max - x_min;
		graph->GetXaxis()->SetLimits(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
		graph->GetXaxis()->SetTitle(x_name.data());
		graph->Draw("AP");
		can_index++;
	}
	can->Write(("Stats vs " + x_name).data());
	delete can;
}


void plot_cumulants_vs_x(map<double, map<int, measure<double>>> cumulants, string x_name) {
	TCanvas *can = new TCanvas(("Cumulants vs " + x_name).data(), ("Cumulants vs " + x_name).data(), config::can_w, config::can_h);
	can->Divide(ceil(pow(config::cumulant_orders.size(),0.5)), ceil(config::cumulant_orders.size()/ceil(pow(config::cumulant_orders.size(), 0.5))));
	int can_index = 1;
	for(int order:config::cumulant_orders) {
		can->cd(can_index);
		vector<double> x, cumulant, cumulant_err;
		for(pair<double, map<int, measure<double>>> effect:cumulants) {
			x.push_back(effect.first);
			measure<double> cumulant_measure = effect.second[order];
			cumulant.push_back(cumulant_measure.val);
			cumulant_err.push_back(cumulant_measure.err);
		}
		TGraphErrors *graph = new TGraphErrors((int)x.size(), x.data(), cumulant.data(), 0, cumulant_err.data());
		graph->SetNameTitle(("Cumulant Order " + to_string(order)).data(), ("Cumulant Order " + to_string(order)).data());
		graph->SetMarkerStyle(8);
		graph->SetMarkerColor(kRed);
		graph->SetMarkerSize(1);
		double x_min = *min_element(x.begin(), x.end());
		double x_max = *max_element(x.begin(), x.end());
		double x_range = x_max - x_min;
		graph->GetXaxis()->SetLimits(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
		graph->GetXaxis()->SetTitle(x_name.data());
		graph->Draw("AP");
		can_index++;
	}
	can->Write(("Cumulants vs " + x_name).data());
	delete can;
}


void hist_ratio_dist(vector<double> ratios, string name, string mode) {
	TH1D *ratio_hist = new TH1D(name.data(), name.data(), 23, -0.05, 1.10);
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



void hist_proton_dist(vector<int> nprotons, string name, string mode) {
	TH1D *protons_hist = new TH1D(name.data(), name.data(), 101, -0.5, 100.5);
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


void comp_hists(TH1D *hist1, TH1D *hist2, string can_name) {
	hist2->SetLineColor(kRed);
	TCanvas can;
	can.SetName(can_name.data());
	hist1->Draw();
	hist2->Draw("sames");
	can.SetLogy();
	can.BuildLegend();
	can.Write();
}


TH1D* hist_ratios(vector<double> ratios) {
	string name = "Dylan";
	TH1D *ratio_hist = new TH1D(name.data(), name.data(), 23, -0.05, 1.10);
	for(double ratio:ratios) {
		ratio_hist->Fill(ratio);
	}
	return(ratio_hist);
}
