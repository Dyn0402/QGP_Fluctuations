/*
 * data_sim_comp.cpp
 *
 *  Created on: Jul 31, 2019
 *      Author: dylan
 */



#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>

#include <TROOT.h>
#include <TError.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>

#include "sim_v1.h"
#include "ratio_methods.h"
#include "plotting.h"
#include "config.h"
#include "file_io.h"
#include "sim_efficiency.h"
#include "ThreadPool.h"
#include "Stats.h"
#include "FourierApprox.h"
#include "Simulator.h"
#include "Measure.h"

using namespace std;


void data_vs_sim() {
	TFile *out_file = new TFile("/home/dylan/local_server/dyn0402/Research/Simulation/Real_Proton_Dists/07-30-19_mix.root", "RECREATE");
	TFile *file = new TFile("/home/dylan/local_server/dyn0402/Research/Results/07-23_new_data.root", "READ");
	int cent = 15;
	vector<int> energy_list = {7, 11, 19, 27, 39, 62};
	vector<double> cl_list = {0, 0.05, 0.1, 0.15, 0.2};
	double p_eff = 0.95;
	double spread = 0.2;
	int cent = 15;
	map<string, map<double, map<double, measure<double>>>> results;
	map<int, TH1D*> dist_hists;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(int energy:energy_list) {
			string data_path = "/home/dylan/local_server/dyn0402/Research/Data3/" + to_string(energy) + "GeV/";
			map<int, int> nprotons = read_nprotons(data_path, cent);
			trees[energy].good_protons[cent] = nprotons;
			for(int div:analysis::divs) {
				map<int, map<int, int>> ratios = read_ratios(path, div, cent);
				trees[energy].ratios[div][cent] = ratios;
			}
			string dist_path = "NProton_Dists/" + to_string(energy) + "GeV/" + to_string(energy) + "GeV protons Centrality " + to_string(cent);
			dist_hists[energy] = (TH1D*) file->Get(dist_path.data());
			for(double p_clust:cl_list) {
				pool.enqueue(run_mix_hist_norm_pars, energy, p_eff, p_clust, spread, dist_hists[energy], out_file, &results);
			}
		}
	}
	for(pair<int, TH1D*> hist:dist_hists) {
		delete hist.second;
	}
	file->Close();

	for(int cent:analysis::centrals) {
		map<int, int> nprotons = read_nprotons(path, cent);
		trees[energy].good_protons[cent] = nprotons;
		for(int div:analysis::divs) {
			map<int, map<int, int>> ratios = read_ratios(path, div, cent);
			trees[energy].ratios[div][cent] = ratios;
		}
	}

	vector<string> moments = {"mean", "standard_deviation", "skewness", "kurtosis"};
	out_file->cd();
	for(string mom:moments) {
		graph_2d(results[mom], mom+" with energy and cluster", "energy", "p_cluster", mom);
	}
	out_file->Close();

}



void run_sim(int energy, double p_eff, double p_clust, double spread, TH1D *dist_hist, TFile *out_file, map<string, map<double, map<double, measure<double>>>> *results) {
	int n_events = (int)(dist_hist->GetEntries() + 0.5);
	cout << "Starting: \t events: " << n_events << "  energy: " << energy << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
	Simulator sim;
	sim.set_p_effect(p_eff);
	sim.set_p_cluster(p_clust);
	sim.set_proton_dist_hist(dist_hist);
	sim.set_n_events(n_events);
	sim.set_spread_sigma(spread);

	vector<tree_data> data = sim.run_simulation_mixed();
	vector<double> ratios = ratios_map_to_vec(data[0].ratios[sim.get_divisions()][0]);
	vector<double> mixed_ratios = ratios_map_to_vec(data[1].ratios[sim.get_divisions()][0]);
	Stats stat(ratios);
	Stats mixed_stat(mixed_ratios);
	(*results)["mean"][energy][p_clust] = divide(stat.get_mean(), mixed_stat.get_mean());
	(*results)["standard_deviation"][energy][p_clust] = divide(stat.get_standard_deviation(), mixed_stat.get_standard_deviation());
	(*results)["skewness"][energy][p_clust] = divide(stat.get_skewness(), mixed_stat.get_skewness());
	(*results)["kurtosis"][energy][p_clust] = divide(stat.get_kurtosis(), mixed_stat.get_kurtosis());


	ostringstream os("");
	os << energy << "GeV | peff: " << setprecision(2) << p_eff << " | p_clust: " << setprecision(2) << p_clust << " | spread: " << setprecision(2) << spread << ends;
	string name = os.str();
	TH2I *hist = ratios_map_to_hist(data[0].ratios[sim.get_divisions()][0], name+"GeV_Ratio_Hist");
	out_file->cd();
	TCanvas *can = new TCanvas((name+"GeV_Ratio_Hist").data()); // Memory leak
	TF1 *line = new TF1((name+"GeV_Ratio_Line").data(), ("x/"+to_string(sim.get_divisions())).data(), -0.5, 0.5+(--data[0].ratios[sim.get_divisions()][0].end())->first);
	TF1 *line2 = new TF1((name+"GeV_Ratio_Line2").data(), "x", -0.5, 0.5+(--data[0].ratios[sim.get_divisions()][0].end())->first);
	line2->SetLineColor(kBlue);
	can->SetLogz();
	line2->Draw();
	hist->Draw("colz");
	line->Draw("same");
	hist->Write();
	can->Write();

	ostringstream mix_os("");
	mix_os << "Mix " << energy << "GeV | peff: " << setprecision(2) << p_eff << " | p_clust: " << setprecision(2) << p_clust << " | spread: " << setprecision(2) << spread << ends;
	string mix_name = mix_os.str();
	TH2I *mix_hist = ratios_map_to_hist(data[1].ratios[sim.get_divisions()][0], mix_name+"GeV_Ratio_Hist");
	TCanvas *mix_can = new TCanvas((mix_name+"GeV_Ratio_Hist").data());
	TF1 *mix_line = new TF1((mix_name+"GeV_Ratio_Line").data(), ("x/"+to_string(sim.get_divisions())).data(), -0.5, 0.5+(--data[1].ratios[sim.get_divisions()][0].end())->first);
	TF1 *mix_line2 = new TF1((name+"GeV_Ratio_Line2").data(), "x", -0.5, 0.5+(--data[0].ratios[sim.get_divisions()][0].end())->first);
	mix_line2->SetLineColor(kBlue);
	mix_can->SetLogz();
	mix_line2->Draw();
	mix_hist->Draw("colz");
	mix_line->Draw("same");
	mix_hist->Write();
	mix_can->Write();

	delete hist;
	delete line;
	delete line2;
	delete can;

	delete mix_hist;
	delete mix_line;
	delete mix_line2;
	delete mix_can;

	cout << "Finishing: \t events: " << n_events << "  energy: " << energy << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
}


void get_moments(vector<double> ratios, int energy, TFile *out_file, map<string, measure<double>> *results) {
	Stats stat(ratios);
	(*results)["mean"] = stat.get_mean();
	(*results)["standard_deviation"] = stat.get_standard_deviation();
	(*results)["skewness"] = stat.get_skewness();
	(*results)["kurtosis"] = stat.get_kurtosis();


	ostringstream os("");
	os << energy << "GeV | peff: " << setprecision(2) << p_eff << " | p_clust: " << setprecision(2) << p_clust << " | spread: " << setprecision(2) << spread << ends;
	string name = os.str();
	TH2I *hist = ratios_map_to_hist(data[0].ratios[sim.get_divisions()][0], name+"GeV_Ratio_Hist");
	out_file->cd();
	TCanvas *can = new TCanvas((name+"GeV_Ratio_Hist").data()); // Memory leak
	TF1 *line = new TF1((name+"GeV_Ratio_Line").data(), ("x/"+to_string(sim.get_divisions())).data(), -0.5, 0.5+(--data[0].ratios[sim.get_divisions()][0].end())->first);
	TF1 *line2 = new TF1((name+"GeV_Ratio_Line2").data(), "x", -0.5, 0.5+(--data[0].ratios[sim.get_divisions()][0].end())->first);
	line2->SetLineColor(kBlue);
	can->SetLogz();
	line2->Draw();
	hist->Draw("colz");
	line->Draw("same");
	hist->Write();
	can->Write();

	delete hist;
	delete line;
	delete line2;
	delete can;

	cout << "Finishing: \t events: " << n_events << "  energy: " << energy << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
}
