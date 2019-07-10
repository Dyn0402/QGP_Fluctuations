/*
 * plotting.cpp
 *
 *  Created on: Jun 18, 2019
 *      Author: Dylan Neff
 */


#include <vector>
#include <tuple>
#include <string>

#include <TFile.h>
#include <TDirectory.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>

#include "ratio_methods.h"
#include "plotting.h"
#include "config_analysis.h"

using namespace std;



void make_ratio_dist_plots(TFile *out_root, map<int, tree_data> data) {
	TDirectory *ratio_dist_dir = out_root->mkdir(plot::ratio_dist_dir_name.data());
	ratio_dist_dir->cd();
	for(int energy:analysis::energy_list) {
		TDirectory *energy_dir = ratio_dist_dir->mkdir((to_string(energy) + "GeV").data());
		energy_dir->cd();
		for(int div:analysis::divs) {
			TDirectory *div_dir = energy_dir->mkdir((to_string(div) + "_Divs").data());
			div_dir->cd();
			for(int cent:analysis::centrals) {
				vector<double> ratios = ratios_map_to_vec(data[energy].ratios[div][cent]);
				hist_ratio_dist(ratios, energy, div, cent);
			}
		}
	}
}


void hist_ratio_dist(vector<double> ratios, int energy, int div, int cent, string mode) {
	string name = to_string(energy) + "GeV " + to_string(div) + " divisions Centrality " + to_string(cent);
	TH1D *ratio_hist = new TH1D(name.data(), name.data(), plot::ratio_hist_bins, plot::ratio_hist_low, plot::ratio_hist_high);
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


void make_proton_dist_plots(TFile *out_root, map<int, tree_data> data) {
	TDirectory *nproton_dist_dir = out_root->mkdir(plot::nproton_dist_dir_name.data());
	nproton_dist_dir->cd();
	for(int energy:analysis::energy_list) {
		TDirectory *energy_dir = nproton_dist_dir->mkdir((to_string(energy) + "GeV").data());
		energy_dir->cd();
		for(int cent:analysis::centrals) {
			vector<int> nprotons = nproton_map_to_vec(data[energy].good_protons[cent]);
			hist_proton_dist(nprotons, energy, cent);
		}
	}
}


void hist_proton_dist(vector<int> nprotons, int energy, int cent, string mode) {
	string name = to_string(energy) + "GeV protons Centrality " + to_string(cent);
	TH1D *protons_hist = new TH1D(name.data(), name.data(), plot::protons_hist_bins, plot::protons_hist_low, plot::protons_hist_high);
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


void make_cumulant_plots(TFile *out_root, map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants) {
	TDirectory *cumulant_dir = out_root->mkdir(plot::cumulant_dir_name.data());
	cumulant_dir->cd();
	for(int order:analysis::cumulant_orders) {
		TDirectory *order_dir = cumulant_dir->mkdir((to_string(order) + "_order").data());
		order_dir->cd();
		for(int cent:analysis::centrals) {
			TDirectory *cent_dir = order_dir->mkdir((to_string(cent) + "_centrality").data());
			cent_dir->cd();
			for(int div:analysis::divs) {
				graph_cumulant_vs_energy(cumulants, div, cent, order);
			}
			for(int energy:analysis::energy_list) {
				graph_cumulant_vs_divs(cumulants, energy, cent, order);
			}
		}
	}
}



void graph_cumulant_vs_energy(map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants, int div, int cent, int order) {
	vector<double> cumulant, cumulant_err, energies, energies_err;
	double cum_val, cum_err;
	for(int energy:analysis::energy_list) {
		tie(cum_val, cum_err) = cumulants[energy][div][cent][order];
		cumulant.push_back(cum_val);
		cumulant_err.push_back(cum_err);
		energies.push_back((double)energy);
		energies_err.push_back(0.0);
	}
	string name = to_string(div) + " division " + to_string(cent) + " centrality " + to_string(order) + " order";
	TGraphErrors *graph = graph_x_vs_y_err(energies, cumulant, energies_err, cumulant_err);
	graph->Write(name.data());
}


void graph_cumulant_vs_divs(map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants, int energy, int cent, int order) {
	vector<double> cumulant, cumulant_err, divisions, divisions_err;
	double cum_val, cum_err;
	for(int div:analysis::divs) {
		tie(cum_val, cum_err) = cumulants[energy][div][cent][order];
		cumulant.push_back(cum_val);
		cumulant_err.push_back(cum_err);
		divisions.push_back((double)energy);
		divisions_err.push_back(0.0);
	}
	string name = to_string(energy) + " energy " + to_string(cent) + " centrality " + to_string(order) + " order";
	TGraphErrors *graph = graph_x_vs_y_err(divisions, cumulant, divisions_err, cumulant_err);
	graph->Write(name.data());
}



void graph_x_vs_y(vector<int> x, vector<double> y, string name) {
	vector<double> x_double(x.begin(), x.end());
	TGraph *graph = new TGraph((int)x_double.size(), x_double.data(), y.data());
	graph->Write(name.data());
	delete graph;
}


TGraphErrors* graph_x_vs_y_err(vector<double> x, vector<double> y, vector<double> x_err, vector<double> y_err) {
	TGraphErrors *graph = new TGraphErrors((int)x.size(), x.data(), y.data(), x_err.data(), y_err.data());
	return(graph);
}



void make_canvas_plots(TFile *out_root, map<int, tree_data> data, map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants) {
	TDirectory *can_dir = out_root->mkdir(plot::canvas_dir_name.data());
	can_dir->cd();

	// Proton distribution canvases
	TDirectory *nproton_can_dir = can_dir->mkdir(plot::nproton_dist_dir_name.data());
	nproton_can_dir->cd();
	for(int cent:analysis::centrals) {
		canvas_nprotons(data, cent, "Centrality " + to_string(cent));
	}

	// Ratios distribution canvases
	TDirectory *ratios_can_dir = can_dir->mkdir(plot::ratio_dist_dir_name.data());
	ratios_can_dir->cd();
	for(int div:analysis::divs) {
		TDirectory *div_dir = ratios_can_dir->mkdir((to_string(div) + "_Divs").data());
		div_dir->cd();
		for(int cent:analysis::centrals) {
			canvas_ratio_dists(data, div, cent, to_string(div) + " divisions " + to_string(cent) + " centrality");
		}
	}

	// Cumulant canvases
	TDirectory *cumulant_can_dir = can_dir->mkdir(plot::cumulant_dir_name.data());
	cumulant_can_dir->cd();
	for(int order:analysis::cumulant_orders) {
		TDirectory *order_dir = cumulant_can_dir->mkdir((to_string(order) + "_Order").data());
		order_dir->cd();
		for(int cent:analysis::centrals) {
			string name = "Order " + to_string(order) + " Centrality " + to_string(cent);
			canvas_cumulant_dists(cumulants, order, cent, name);
		}
	}
}


void canvas_nprotons(map<int, tree_data> data, int cent, string name) {
	TCanvas *proton_can = new TCanvas();
	proton_can->SetTitle(name.data());
	proton_can->SetName(name.data());
	proton_can->Divide(ceil(pow(data.size(),0.5)), ceil(data.size()/ceil(pow(data.size(), 0.5))));
	int can_index = 1;
	for(pair<int, tree_data>  energy:data) {
		proton_can->cd(can_index);
		vector<int> nprotons = nproton_map_to_vec(energy.second.good_protons[cent]);
		hist_proton_dist(nprotons, energy.first, cent, "draw");
		can_index++;
	}
	proton_can->Write(name.data());
	delete proton_can;
}


void canvas_ratio_dists(map<int, tree_data> data, int div, int cent, string name) {
	TCanvas *ratio_can = new TCanvas();
	ratio_can->SetTitle(name.data());
	ratio_can->SetName(name.data());
	ratio_can->Divide(ceil(pow(data.size(),0.5)), ceil(data.size()/ceil(pow(data.size(), 0.5))));
	int can_index = 1;
	for(pair<int, tree_data>  energy:data) {
		ratio_can->cd(can_index);
		gPad->SetLogy();
		vector<double> ratios = ratios_map_to_vec(energy.second.ratios[div][cent]);
		hist_ratio_dist(ratios, energy.first, div, cent, "draw");
		can_index++;
	}
	ratio_can->Write(name.data());
	delete ratio_can;
}


void canvas_cumulant_dists(map<int, map<int, map<int, map<int, tuple<double, double>>>>> cumulants, int order, int cent, string name) {
	auto *can = new TCanvas();
	auto *mg = new TMultiGraph();
	mg->SetNameTitle(name.data(), name.data());
	for(int div:analysis::divs) {
		vector<double> cumulant, energy, cumulant_err, energy_err;
		double cum_val, cum_err;
		for(int e:analysis::energy_list) {
			energy.push_back((double)e);
			energy_err.push_back(0.0);
			tie(cum_val, cum_err) = cumulants[e][div][cent][order];
			cumulant.push_back(cum_val);
			cumulant.push_back(cum_err);
		}
		TGraphErrors *graph = graph_x_vs_y_err(energy, cumulant, energy_err, cumulant_err);
		graph->SetNameTitle((to_string(div) + " divisions").data());
		graph->SetMarkerStyle(plot::div_marker_styles[div]);
		graph->SetMarkerColor(plot::div_marker_colors[div]);
		graph->SetMarkerSize(plot::div_marker_sizes[div]);
		mg->Add(graph, "AP");
	}
	mg->Draw("AP");
	can->BuildLegend();
	can->Write(name.data());
	delete mg;
	delete can;
}

