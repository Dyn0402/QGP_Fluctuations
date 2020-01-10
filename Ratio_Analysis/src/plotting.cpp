/*
 * plotting.cpp
 *
 *  Created on: Jun 18, 2019
 *      Author: Dylan Neff
 */


#include <vector>
#include <tuple>
#include <string>
#include <limits>

#include <TFile.h>
#include <TDirectory.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TStyle.h>

#include "ratio_methods.h"
#include "plotting.h"
#include "config_analysis.h"
#include "Stats.h"
#include "Measure.h"
#include "RatioData.h"

using namespace std;



void make_ratio_dist_plots(TFile *out_root, map<int, map<int, map<int, RatioData>>> data) {
	TDirectory *ratio_dist_dir = out_root->mkdir(plot::ratio_dist_dir_name.data());
	create_ratio_dist_plots(ratio_dist_dir, data);
}

void make_ratio_dist_plots(TDirectory *out_root, map<int, map<int, map<int, RatioData>>> data) {
	TDirectory *ratio_dist_dir = out_root->mkdir(plot::ratio_dist_dir_name.data());
	create_ratio_dist_plots(ratio_dist_dir, data);
}

void create_ratio_dist_plots(TDirectory *ratio_dist_dir, map<int, map<int, map<int, RatioData>>> data) {
	ratio_dist_dir->cd();
	for(pair<int, map<int, map<int, RatioData>>> energy:data) {
		TDirectory *energy_dir = ratio_dist_dir->mkdir((to_string(energy.first) + "GeV").data());
		energy_dir->cd();
		for(pair<int, map<int, RatioData>> div:energy.second) {
			TDirectory *div_dir = energy_dir->mkdir((to_string(div.first) + "_Divs").data());
			div_dir->cd();
			for(pair<int, RatioData> cent:div.second) {
				cent.second.canvas_1d_dist(to_string(energy.first) + "GeV " + to_string(div.first) + " divisions Centrality " + to_string(cent.first));
			}
		}
	}
}


void hist_ratio_dist(map<int, map<int, int>> ratios, int energy, int div, int cent, string mode) {
	string name = to_string(energy) + "GeV " + to_string(div) + " divisions Centrality " + to_string(cent);
	TH1D *ratio_hist = new TH1D(name.data(), name.data(), plot::ratio_hist_bins, plot::ratio_hist_low, plot::ratio_hist_high);
	for(pair<int, map<int, int>> event:ratios) {
		for(pair<int, int> bin:event.second) {
			ratio_hist->Fill(((double)bin.first) / event.first, bin.second);
		}
	}

	if(mode == "write") {
		ratio_hist->Write();
		delete ratio_hist;
	} else if(mode == "draw") {
		ratio_hist->Draw("HIST");
	}
}


void make_2d_dist_plots(TFile *out_root, map<int, map<int, map<int, RatioData>>> data) {
	TDirectory *dist_dir = out_root->mkdir(plot::dist_2d_dir_name.data());
	create_2d_dist_plots(dist_dir, data);
}

void make_2d_dist_plots(TDirectory *out_root, map<int, map<int, map<int, RatioData>>> data) {
	TDirectory *dist_dir = out_root->mkdir(plot::dist_2d_dir_name.data());
	create_2d_dist_plots(dist_dir, data);
}


void create_2d_dist_plots(TDirectory *dist_dir, map<int, map<int, map<int, RatioData>>> data) {
	dist_dir->cd();

	for(pair<int, map<int, map<int, RatioData>>> energy:data) {
		TDirectory *energy_dir = dist_dir->mkdir((to_string(energy.first) + "GeV").data());
		energy_dir->cd();
		for(pair<int, map<int, RatioData>> div:energy.second) {
			TDirectory *div_dir = energy_dir->mkdir((to_string(div.first) + "_Divs").data());
			div_dir->cd();
			for(pair<int, RatioData> cent:div.second) {
				cent.second.canvas_2d_dist(to_string(energy.first) + "GeV " + to_string(div.first) + " divisions Centrality " + to_string(cent.first));
			}
		}
	}
}


void canvas_2d_dist(map<int, map<int, int>> dist, int energy, int div, int cent) {
	string name = to_string(energy) + "GeV " + to_string(div) + " divisions Centrality " + to_string(cent);
	TH2I *hist = ratios_map_to_hist(dist, name);
	TCanvas *can = new TCanvas(name.data()); // Memory leak
	TF1 *avg_line = new TF1(name.data(), ("x/"+to_string(div)).data(), -0.5, 0.5+(--dist.end())->first);
	TF1 *max_line = new TF1((name+"max").data(), "x", -0.5, 0.5+(--dist.end())->first);
	max_line->SetLineColor(4);
	can->SetLogz();
	hist->Draw("colz");
	max_line->Draw("same");
	avg_line->Draw("same");
	hist->Write();
	can->Write();
}


void make_proton_dist_plots(TFile *out_root, map<int, map<int, map<int, RatioData>>> data) {
	TDirectory *nproton_dist_dir = out_root->mkdir(plot::nproton_dist_dir_name.data());
	create_proton_dist_plots(nproton_dist_dir, data);
}

void make_proton_dist_plots(TDirectory *out_root, map<int, map<int, map<int, RatioData>>> data) {
	TDirectory *nproton_dist_dir = out_root->mkdir(plot::nproton_dist_dir_name.data());
	create_proton_dist_plots(nproton_dist_dir, data);
}

void create_proton_dist_plots(TDirectory *nproton_dist_dir, map<int, map<int, map<int, RatioData>>> data) {
	nproton_dist_dir->cd();
	for(pair<int, map<int, map<int, RatioData>>> energy:data) {
		TDirectory *energy_dir = nproton_dist_dir->mkdir((to_string(energy.first) + "GeV").data());
		energy_dir->cd();
		for(pair<int, RatioData> cent:energy.second.begin()->second) {
			cent.second.canvas_proton_dist(to_string(energy.first) + "GeV protons Centrality " + to_string(cent.first));
		}
	}
}


void hist_proton_dist(map<int, int> nprotons, int energy, int cent, string mode) {
	string name = to_string(energy) + "GeV protons Centrality " + to_string(cent);
	TH1D *protons_hist = new TH1D(name.data(), name.data(), plot::protons_hist_bins, plot::protons_hist_low, plot::protons_hist_high);
	for(pair<int, int> protons:nprotons) {
		protons_hist->Fill(protons.first, protons.second);
	}
	if(mode == "write") {
		protons_hist->Write();
		delete protons_hist;
	} else if(mode == "draw") {
		protons_hist->Draw("HIST");
	}
}


void make_cumulant_plots(TFile *out_root, map<int, map<int, map<int, map<int, Measure>>>> cumulants) {
	TDirectory *cumulant_dir = out_root->mkdir(plot::cumulant_dir_name.data());
	create_cumulant_plots(cumulant_dir, cumulants);
}

void make_cumulant_plots(TDirectory *out_root, map<int, map<int, map<int, map<int, Measure>>>> cumulants) {
	TDirectory *cumulant_dir = out_root->mkdir(plot::cumulant_dir_name.data());
	create_cumulant_plots(cumulant_dir, cumulants);
}

void create_cumulant_plots(TDirectory *cumulant_dir, map<int, map<int, map<int, map<int, Measure>>>> cumulants) {
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



void graph_cumulant_vs_energy(map<int, map<int, map<int, map<int, Measure>>>> cumulants, int div, int cent, int order) {
	vector<double> cumulant, cumulant_err, energies, energies_err;
	Measure cum;
	for(int energy:analysis::energy_list) {
		cum = cumulants[energy][div][cent][order];
		cumulant.push_back(cum.get_val());
		cumulant_err.push_back(cum.get_err());
		energies.push_back((double)energy);
		energies_err.push_back(0.0);
	}
	string name = to_string(div) + " division " + to_string(cent) + " centrality " + to_string(order) + " order";
	TGraphErrors *graph = graph_x_vs_y_err(energies, cumulant, energies_err, cumulant_err);
	graph->Write(name.data());
}


void graph_cumulant_vs_divs(map<int, map<int, map<int, map<int, Measure>>>> cumulants, int energy, int cent, int order) {
	vector<double> cumulant, cumulant_err, divisions, divisions_err;
	Measure cum;
	for(int div:analysis::divs) {
		cum = cumulants[energy][div][cent][order];
		cumulant.push_back(cum.get_val());
		cumulant_err.push_back(cum.get_err());
		divisions.push_back((double)div);
		divisions_err.push_back(0.0);
	}
	string name = to_string(energy) + " energy " + to_string(cent) + " centrality " + to_string(order) + " order";
	TGraphErrors *graph = graph_x_vs_y_err(divisions, cumulant, divisions_err, cumulant_err);
	graph->Write(name.data());
}



void make_stat_plots(TFile *out_root, map<int, map<int, map<int, map<string, Measure>>>> stats) {
	TDirectory *stats_dir = out_root->mkdir(plot::stat_dir_name.data());
	create_stat_plots(stats_dir, stats);
}

void make_stat_plots(TDirectory *out_root, map<int, map<int, map<int, map<string, Measure>>>> stats) {
	TDirectory *stats_dir = out_root->mkdir(plot::stat_dir_name.data());
	create_stat_plots(stats_dir, stats);
}

void create_stat_plots(TDirectory *stats_dir, map<int, map<int, map<int, map<string, Measure>>>> stats) {
	stats_dir->cd();
	for(string stat:analysis::stat_names) {
		TDirectory *stat_dir = stats_dir->mkdir(stat.data());
		stat_dir->cd();
		for(int cent:analysis::centrals) {
			TDirectory *cent_dir = stat_dir->mkdir((to_string(cent) + "_centrality").data());
			cent_dir->cd();
			for(int div:analysis::divs) {
				graph_stat_vs_energy(stats, div, cent, stat);
			}
			for(int energy:analysis::energy_list) {
				graph_stat_vs_divs(stats, energy, cent, stat);
			}
		}
	}
}



void graph_stat_vs_energy(map<int, map<int, map<int, map<string, Measure>>>> stats, int div, int cent, string stat) {
	vector<double> stat_val, stat_err, energies, energies_err;
	Measure stat_meas;
	for(int energy:analysis::energy_list) {
		stat_meas = stats[energy][div][cent][stat];
		stat_val.push_back(stat_meas.get_val());
		stat_err.push_back(stat_meas.get_err());
		energies.push_back((double)energy);
		energies_err.push_back(0.0);
	}
	string name = to_string(div) + " division " + to_string(cent) + " centrality " + stat;
	TGraphErrors *graph = graph_x_vs_y_err(energies, stat_val, energies_err, stat_err);
	graph->Write(name.data());
}


void graph_stat_vs_divs(map<int, map<int, map<int, map<string, Measure>>>> stats, int energy, int cent, string stat) {
	vector<double> stat_val, stat_err, divisions, divisions_err;
	Measure stat_meas;
	for(int div:analysis::divs) {
		stat_meas = stats[energy][div][cent][stat];
		stat_val.push_back(stat_meas.get_val());
		stat_err.push_back(stat_meas.get_err());
		divisions.push_back((double)div);
		divisions_err.push_back(0.0);
	}
	string name = to_string(energy) + " energy " + to_string(cent) + " centrality " + stat;
	TGraphErrors *graph = graph_x_vs_y_err(divisions, stat_val, divisions_err, stat_err);
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



void make_canvas_plots(TFile *out_root, map<int, map<int, map<int, RatioData>>> data) {
	TDirectory *can_dir = out_root->mkdir(plot::canvas_dir_name.data());
	create_canvas_plots(can_dir, data);
}

void make_canvas_plots(TDirectory *out_root, map<int, map<int, map<int, RatioData>>> data) {
	TDirectory *can_dir = out_root->mkdir(plot::canvas_dir_name.data());
	create_canvas_plots(can_dir, data);
}


void create_canvas_plots(TDirectory *can_dir, map<int, map<int, map<int, RatioData>>> data) {
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
}


void canvas_nprotons(map<int, map<int, map<int, RatioData>>> data, int cent, string name) {
	TCanvas *proton_can = new TCanvas();
	proton_can->SetTitle(name.data());
	proton_can->SetName(name.data());
	proton_can->Divide(ceil(pow(data.size(),0.5)), ceil(data.size()/ceil(pow(data.size(), 0.5))));
	int can_index = 1;
	for(pair<int, map<int, map<int, RatioData>>>  energy:data) {
		proton_can->cd(can_index);
		hist_proton_dist((*energy.second.begin()).second[cent].get_proton_dist(), energy.first, cent, "draw"); // Memory leak
		can_index++;
	}
	proton_can->Write(name.data());
	delete proton_can;
}


void canvas_ratio_dists(map<int, map<int, map<int, RatioData>>> data, int div, int cent, string name) {
	TCanvas *ratio_can = new TCanvas();
	ratio_can->SetTitle(name.data());
	ratio_can->SetName(name.data());
	ratio_can->Divide(ceil(pow(data.size(),0.5)), ceil(data.size()/ceil(pow(data.size(), 0.5))));
	int can_index = 1;
	for(pair<int, map<int, map<int, RatioData>>> energy:data) {
		ratio_can->cd(can_index);
		gPad->SetLogy();
		hist_ratio_dist(energy.second[div][cent].get_ratio_data(), energy.first, div, cent, "draw");
		can_index++;
	}
	ratio_can->Write(name.data());
	delete ratio_can;
}


void canvas_cumulant_dists(map<int, map<int, map<int, map<int, Measure>>>> cumulants, int order, int cent, string name) {
	auto *can = new TCanvas();
	auto *mg = new TMultiGraph();
	mg->SetNameTitle(name.data(), name.data());
	for(int div:analysis::divs) {
		vector<double> cumulant, energy, cumulant_err, energy_err;
		Measure cum;
		for(int e:analysis::energy_list) {
			energy.push_back((double)e);
			energy_err.push_back(0.0);
			cum = cumulants[e][div][cent][order];
			cumulant.push_back(cum.get_val());
			cumulant_err.push_back(cum.get_err());
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


void canvas_stat_dists(map<int, map<int, map<int, map<string, Measure>>>> stats, string stat, int cent, string name) {
	auto *can = new TCanvas();
	auto *mg = new TMultiGraph();
	mg->SetNameTitle(name.data(), name.data());
	for(int div:analysis::divs) {
		vector<double> stat_vals, energy, stat_err, energy_err;
		Measure stat_meas;
		for(int e:analysis::energy_list) {
			energy.push_back((double)e);
			energy_err.push_back(0.0);
			stat_meas = stats[e][div][cent][stat];
			stat_vals.push_back(stat_meas.get_val());
			stat_err.push_back(stat_meas.get_err());
		}
		TGraphErrors *graph = graph_x_vs_y_err(energy, stat_vals, energy_err, stat_err);
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


void athic_stat_vs_centrality(map<int, map<int, map<int, map<string, Measure>>>> stats, string name) {
	int div = 6; //Hardcode, fix.
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	can->Divide(2,2);
	int can_index = 1;
	for(string stat:analysis::stat_names) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		mg->SetNameTitle(stat.data(), stat.data());
		double x_min = numeric_limits<double>::max();
		double x_max = numeric_limits<double>::min();
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		for(int energy:analysis::energy_list) {
			vector<double> stat_vals, cent_val, stat_err, cent_err;
			Measure stat_meas;
			for(int cent_index=15; cent_index > 5; cent_index--) {
				cent_val.push_back((15-cent_index)*plot::centrality_slope + plot::centrality_intercept);
				cent_err.push_back(0.0);
				stat_meas = stats[energy][div][cent_index][stat];
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			TGraphErrors *graph = graph_x_vs_y_err(cent_val, stat_vals, cent_err, stat_err);
			graph->SetNameTitle((to_string(plot::energy_match[energy]).substr(0,4) + " GeV").data());
			graph->SetMarkerStyle(plot::energy_marker_styles[energy]);
			graph->SetMarkerColor(plot::energy_marker_colors[energy]);
			graph->SetMarkerSize(plot::energy_marker_sizes[energy]);
			mg->Add(graph, "AP");
			if(*min_element(cent_val.begin(), cent_val.end()) < x_min) { x_min = *min_element(cent_val.begin(), cent_val.end()); }
			if(*max_element(cent_val.begin(), cent_val.end()) > x_max) { x_max = *max_element(cent_val.begin(), cent_val.end()); }
		}
		double x_range = x_max - x_min;
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
		mg->GetXaxis()->SetRangeUser(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetXaxis()->SetTitle("Centrality (%)");
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(stat == "mean") {
//			TLegend *leg = new TLegend();
//			leg->Draw();
			gPad->BuildLegend(0.3, 0.21, 0.3, 0.21, "", "p");
		}
		can_index++;
	}
	can->Write(name.data());
	delete can;
}


void athic_stat_vs_energy(map<int, map<int, map<int, map<string, Measure>>>> stats, string name) {
	int div = 6; //Hardcode, fix.
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	can->Divide(2,2);
	int can_index = 1;
	for(string stat:analysis::stat_names) {
		can->cd(can_index);
		gPad->SetLogx();
		auto *mg = new TMultiGraph();
		mg->SetNameTitle(stat.data(), stat.data());
		double x_min = numeric_limits<double>::max();
		double x_max = numeric_limits<double>::min();
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		int cent_low, cent_high;
		for(int cent_index=15; cent_index > 10; cent_index-=2) {
			vector<double> stat_vals, energy_val, stat_err, energy_err;
			Measure stat_meas;
			for(int energy:analysis::energy_list) {
				energy_val.push_back(plot::energy_match[energy]);
				energy_err.push_back(0.0);
				stat_meas = stats[energy][div][cent_index][stat];
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
			cent_high = (int)(((15-cent_index+1)*plot::centrality_slope) + 0.5);
			cent_low = (int)(((15-cent_index)*plot::centrality_slope) + 0.5);
			graph->SetNameTitle((to_string(cent_low) + "-" + to_string(cent_high) + " %").data());
			graph->SetMarkerStyle(plot::cent_marker_styles[cent_index]);
			graph->SetMarkerColor(plot::cent_marker_colors[cent_index]);
			graph->SetMarkerSize(plot::cent_marker_sizes[cent_index]);
			mg->Add(graph, "AP");
			if(*min_element(energy_val.begin(), energy_val.end()) < x_min) { x_min = *min_element(energy_val.begin(), energy_val.end()); }
			if(*max_element(energy_val.begin(), energy_val.end()) > x_max) { x_max = *max_element(energy_val.begin(), energy_val.end()); }
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(pow(x_min, 1.1) / pow(x_max, 0.1), pow(x_max, 1.1) / pow(x_min, 0.1));
		mg->GetXaxis()->SetRangeUser(pow(x_min, 1.1) / pow(x_max, 0.1), pow(x_max, 1.1) / pow(x_min, 0.1));
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetXaxis()->SetTitle("Energy (GeV)");
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(stat == "mean") {
//			TLegend *leg = new TLegend();
//			leg->Draw();
			gPad->BuildLegend(0.3, 0.21, 0.3, 0.21, "", "p");
		}
		can_index++;
	}
	can->Write(name.data());
	delete can;
}


// Most general 11/20/19
void roli_thesis_stats(map<int, map<int, map<int, map<string, Measure>>>> stats, vector<string> stat_names, vector<int> cents, vector<int> divs, string name) {
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	can->Divide((int)stat_names.size(), (int)cents.size(), 0.001, 0.001);
	int can_index = 1;
	for(int cent:cents) {
		for(string stat:stat_names) {
			can->cd(can_index);
			auto *mg = new TMultiGraph();
			if(can_index <= (int)stat_names.size()) { mg->SetNameTitle(stat.data(), stat.data()); }
			double y_max = numeric_limits<double>::min();
			double y_min = numeric_limits<double>::max();
			TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
			for(int div:divs) {
				vector<double> stat_vals, energy_val, stat_err, energy_err;
				Measure stat_meas;
				for(int energy:analysis::energy_list) {
					energy_val.push_back(plot::energy_match[energy]);
					energy_err.push_back(0.0);
					stat_meas = stats[energy][div][cent][stat];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
				graph->SetNameTitle((to_string(div) + " divisions").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[div]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(graph, "AP");
				if(can_index == 1) { leg->AddEntry(graph, (to_string(div) + " divs").data(), "p"); }
			}
			double y_range = y_max - y_min;
			mg->GetXaxis()->SetLimits(0, 80);
			mg->GetXaxis()->SetRangeUser(0, 80);
			mg->GetXaxis()->SetLabelSize(0.06);
			mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetLabelSize(0.06);
			if(can_index % (int)stat_names.size() == 1) {
				pair<int, int> range = get_cent9_range(cent);
				mg->GetYaxis()->SetTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data());
				mg->GetYaxis()->SetTitleSize(0.1); mg->GetYaxis()->SetTitleOffset(0.95); gPad->SetLeftMargin(0.2);
			}
			else { gPad->SetLeftMargin(0.1); }
			if(can_index > (int)stat_names.size()*((int)cents.size()-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
			else { gPad->SetBottomMargin(0.05); }
			if(can_index > (int)stat_names.size()) { gPad->SetTopMargin(0.07); }
			else { gPad->SetTopMargin(0.08); }
			gPad->SetRightMargin(0.02);
			mg->Draw("AP"); // Multigraph memory leak, fix.
			if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
			can_index++;
		}
	}
	can->Update();
	can->Write(name.data());
	delete can;
}


// Most general 11/20/19 with systematics
void roli_thesis_stats(map<int, map<int, map<int, map<string, Measure>>>> stats, map<int, map<int, map<int, map<string, double>>>> sys, vector<string> stat_names, vector<int> cents, vector<int> divs, string name) {
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	can->Divide((int)stat_names.size(), (int)cents.size(), 0.001, 0.001);
	int can_index = 1;
	for(int cent:cents) {
		for(string stat:stat_names) {
			can->cd(can_index);
			auto *mg = new TMultiGraph();
			if(can_index <= (int)stat_names.size()) { mg->SetNameTitle(stat.data(), stat.data()); }
			double y_max = numeric_limits<double>::min();
			double y_min = numeric_limits<double>::max();
			TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
			for(int div:divs) {
				vector<double> stat_vals, energy_val, stat_err, energy_err, stat_sys;
				Measure stat_meas;
				for(int energy:analysis::energy_list) {
					energy_val.push_back(plot::energy_match[energy]);
					energy_err.push_back(0.0);
					stat_meas = stats[energy][div][cent][stat];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					stat_sys.push_back(sys[energy][div][cent][stat]);
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
					if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
				graph->SetNameTitle((to_string(div) + " divisions").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[div]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(graph, "APZ");
				TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
				sys_graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(sys_graph, "[]");
				if(can_index == 1) { leg->AddEntry(graph, (to_string(div) + " divs").data(), "p"); }
			}
			double y_range = y_max - y_min;
			mg->GetXaxis()->SetLimits(0, 80);
			mg->GetXaxis()->SetRangeUser(0, 80);
			mg->GetXaxis()->SetLabelSize(0.06);
			mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetLabelSize(0.06);
			if(can_index % (int)stat_names.size() == 1) {
				pair<int, int> range = get_cent9_range(cent);
				mg->GetYaxis()->SetTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data());
				mg->GetYaxis()->SetTitleSize(0.1); mg->GetYaxis()->SetTitleOffset(0.95); gPad->SetLeftMargin(0.2);
			}
			else { gPad->SetLeftMargin(0.1); }
			if(can_index > (int)stat_names.size()*((int)cents.size()-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
			else { gPad->SetBottomMargin(0.05); }
			if(can_index > (int)stat_names.size()) { gPad->SetTopMargin(0.07); }
			else { gPad->SetTopMargin(0.08); }
			gPad->SetRightMargin(0.02);
			mg->Draw("AP"); // Multigraph memory leak, fix.
			if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
			can_index++;
		}
	}
	can->Update();
	can->Write(name.data());
	delete can;
}


// Most general 11/20/19 with systematics
void roli_thesis_stats(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, vector<string> stat_names, vector<int> cents, vector<int> divs, string name) {
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	can->Divide((int)stat_names.size(), (int)cents.size(), 0.001, 0.001);
	int can_index = 1;
	for(int cent:cents) {
		for(string stat:stat_names) {
			can->cd(can_index);
			auto *mg = new TMultiGraph();
			if(can_index <= (int)stat_names.size()) { mg->SetNameTitle(stat.data(), stat.data()); }
			double y_max = numeric_limits<double>::min();
			double y_min = numeric_limits<double>::max();
			TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
			for(int div:divs) {
				int set_num = 0;
				for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats) {
					vector<double> stat_vals, energy_val, stat_err, energy_err, stat_sys;
					Measure stat_meas;
					for(int energy:analysis::energy_list) {
						energy_val.push_back(plot::energy_match[energy] + set_num*0.4);  // Offset sets on x (Energy) axis.
						energy_err.push_back(0.0);
						stat_meas = data_set.second[energy][div][cent][stat];
						stat_vals.push_back(stat_meas.get_val());
						stat_err.push_back(stat_meas.get_err());
						stat_sys.push_back(sys[data_set.first][energy][div][cent][stat]);
						if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
						if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
						if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
						if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
					}
					TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
					graph->SetNameTitle((data_set.first + " " + to_string(div) + " divisions").data());
					graph->SetMarkerStyle(plot::div_marker_styles[div]);
					graph->SetMarkerColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
					graph->SetMarkerSize(plot::div_marker_sizes[div]);
					graph->SetLineColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
					mg->Add(graph, "APZ");
					TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
					sys_graph->SetLineColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
					mg->Add(sys_graph, "[]");
					if(can_index == 1) { leg->AddEntry(graph, (data_set.first + " " + to_string(div) + " divs").data(), "p"); }
					set_num++;
				}
			}
			double y_range = y_max - y_min;
			mg->GetXaxis()->SetLimits(0, 80);
			mg->GetXaxis()->SetRangeUser(0, 80);
			mg->GetXaxis()->SetLabelSize(0.06);
			mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetLabelSize(0.06);
			if(can_index % (int)stat_names.size() == 1) {
				pair<int, int> range = get_cent9_range(cent);
				mg->GetYaxis()->SetTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data());
				mg->GetYaxis()->SetTitleSize(0.1); mg->GetYaxis()->SetTitleOffset(0.95); gPad->SetLeftMargin(0.2);
			}
			else { gPad->SetLeftMargin(0.1); }
			if(can_index > (int)stat_names.size()*((int)cents.size()-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
			else { gPad->SetBottomMargin(0.05); }
			if(can_index > (int)stat_names.size()) { gPad->SetTopMargin(0.07); }
			else { gPad->SetTopMargin(0.08); }
			gPad->SetRightMargin(0.02);
			mg->Draw("AP"); // Multigraph memory leak, fix.
			if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
			can_index++;
		}
	}
	can->Update();
	can->Write(name.data());
	delete can;
}



void roli_thesis_stats(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, vector<string> stat_names, vector<int> cents, vector<int> divs, string name) {
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	can->Divide((int)stat_names.size(), (int)cents.size(), 0.001, 0.001);
	int can_index = 1;
	for(int cent:cents) {
		for(string stat:stat_names) {
			can->cd(can_index);
			auto *mg = new TMultiGraph();
			if(can_index <= (int)stat_names.size()) { mg->SetNameTitle(stat.data(), stat.data()); }
			double y_max = numeric_limits<double>::min();
			double y_min = numeric_limits<double>::max();
			TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
			for(int div:divs) {
				int set_num = 0;
				for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats) {
					vector<double> stat_vals, energy_val, stat_err, energy_err;
					Measure stat_meas;
					for(int energy:analysis::energy_list) {
						energy_val.push_back(plot::energy_match[energy] + set_num*0.4);  // Offset sets on x (Energy) axis.
						energy_err.push_back(0.0);
						stat_meas = data_set.second[energy][div][cent][stat];
						stat_vals.push_back(stat_meas.get_val());
						stat_err.push_back(stat_meas.get_err());
						if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
						if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					}
					TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
					graph->SetNameTitle((data_set.first + " " + to_string(div) + " divisions").data());
					graph->SetMarkerStyle(plot::div_marker_styles[div]);
					graph->SetMarkerColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
					graph->SetMarkerSize(plot::div_marker_sizes[div]);
					graph->SetLineColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
					mg->Add(graph, "AP");
					if(can_index == 1) { leg->AddEntry(graph, (data_set.first + " " + to_string(div) + " divs").data(), "p"); }
					set_num++;
				}
			}
			double y_range = y_max - y_min;
			mg->GetXaxis()->SetLimits(0, 80);
			mg->GetXaxis()->SetRangeUser(0, 80);
			mg->GetXaxis()->SetLabelSize(0.06);
			mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetLabelSize(0.06);
			if(can_index % (int)stat_names.size() == 1) {
				pair<int, int> range = get_cent9_range(cent);
				mg->GetYaxis()->SetTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data());
				mg->GetYaxis()->SetTitleSize(0.1); mg->GetYaxis()->SetTitleOffset(0.95); gPad->SetLeftMargin(0.2);
			}
			else { gPad->SetLeftMargin(0.1); }
			if(can_index > (int)stat_names.size()*((int)cents.size()-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
			else { gPad->SetBottomMargin(0.05); }
			if(can_index > (int)stat_names.size()) { gPad->SetTopMargin(0.07); }
			else { gPad->SetTopMargin(0.08); }
			gPad->SetRightMargin(0.02);
			mg->Draw("AP"); // Multigraph memory leak, fix.
			if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
			can_index++;
		}
	}
	can->Update();
	can->Write(name.data());
	delete can;
}



void centralities_stat(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name) {
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"%").data());
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int div:divs) {
			int set_num = 0;
			for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats) {
				vector<double> stat_vals, energy_val, stat_err, energy_err, stat_sys;
				Measure stat_meas;
				for(int energy:analysis::energy_list) {
					energy_val.push_back(plot::energy_match[energy] + set_num*0.4);  // Offset sets on x (Energy) axis.
					energy_err.push_back(0.0);
					stat_meas = data_set.second[energy][div][cent][stat_name];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					stat_sys.push_back(sys[data_set.first][energy][div][cent][stat_name]);
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
					if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
				graph->SetNameTitle((data_set.first + " " + to_string(div) + " divisions").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
				mg->Add(graph, "APZ");
				TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
				sys_graph->SetLineColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
				mg->Add(sys_graph, "[]");
				set_num++;
				if(can_index == 1) {
					leg->AddEntry(graph, (data_set.first + " " + stat_name + " " + to_string(div) + " divs").data(), "p");
				}
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.05); }
		if(can_index > can_div.first) { gPad->SetTopMargin(0.07); }
		else { gPad->SetTopMargin(0.08); }
		gPad->SetRightMargin(0.02);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}
	can->Update();
	can->Write(name.data());
	delete can;
}



void centralities_stat(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, string stat_name, vector<int> cents, vector<int> divs, string name) {
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"%").data());
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int div:divs) {
			int set_num = 0;
			for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats) {
				vector<double> stat_vals, energy_val, stat_err, energy_err;
				Measure stat_meas;
				for(int energy:analysis::energy_list) {
					energy_val.push_back(plot::energy_match[energy] + set_num*0.4);  // Offset sets on x (Energy) axis.
					energy_err.push_back(0.0);
					stat_meas = data_set.second[energy][div][cent][stat_name];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
				graph->SetNameTitle((data_set.first + " " + to_string(div) + " divisions").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[(div+set_num)%(int)plot::div_marker_styles.size()]);
				mg->Add(graph, "AP");
				set_num++;
				if(can_index == 1) {
					leg->AddEntry(graph, (data_set.first + " " + stat_name + " " + to_string(div) + " divs").data(), "p");
				}
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.05); }
		if(can_index > can_div.first) { gPad->SetTopMargin(0.07); }
		else { gPad->SetTopMargin(0.08); }
		gPad->SetRightMargin(0.02);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}
	can->Update();
	can->Write(name.data());
	delete can;
}



void centralities_stat(map<int, map<int, map<int, map<string, Measure>>>> stats, map<int, map<int, map<int, map<string, double>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name) {
	double y_max = numeric_limits<double>::min(); //---
	double y_min = numeric_limits<double>::max(); //---
	for(int cent:cents) {
		for(int div:divs) {
			vector<double> stat_vals, energy_val, stat_err, energy_err, stat_sys;
			Measure stat_meas;
			for(int energy:analysis::energy_list) {
				energy_val.push_back(plot::energy_match[energy]);
				energy_err.push_back(0.0);
				stat_meas = stats[energy][div][cent][stat_name];
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				stat_sys.push_back(sys[energy][div][cent][stat_name]);
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
				if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
			}
		}
	}


	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"%").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int div:divs) {
			vector<double> stat_vals, energy_val, stat_err, energy_err, stat_sys;
			Measure stat_meas;
			for(int energy:analysis::energy_list) {
				energy_val.push_back(plot::energy_match[energy]);
				energy_err.push_back(0.0);
				stat_meas = stats[energy][div][cent][stat_name];
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				stat_sys.push_back(sys[energy][div][cent][stat_name]);
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
				if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
			}
			TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
			graph->SetNameTitle((stat_name + " " + to_string(div) + " divisions").data());
			graph->SetMarkerStyle(plot::div_marker_styles[div]);
			graph->SetMarkerColor(plot::div_marker_colors[div]);
			graph->SetMarkerSize(plot::div_marker_sizes[div]);
			graph->SetLineColor(plot::div_marker_colors[div]);
			mg->Add(graph, "APZ");
			TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
			sys_graph->SetLineColor(plot::div_marker_colors[div]);
			mg->Add(sys_graph, "[]");
			if(can_index == 1) {
				leg->AddEntry(graph, (stat_name + " " + to_string(div) + " divs").data(), "p");
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.05); }
		if(can_index > can_div.first) { gPad->SetTopMargin(0.07); }
		else { gPad->SetTopMargin(0.08); }
		gPad->SetRightMargin(0.02);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}

	can->Update();
	can->Write(name.data());
	delete can;
}


void centralities_stat(map<int, map<int, map<int, map<string, Measure>>>> stats, string stat_name, vector<int> cents, vector<int> divs, string name) {
	double y_max = numeric_limits<double>::min(); //---
	double y_min = numeric_limits<double>::max(); //---
	for(int cent:cents) {
		for(int div:divs) {
			vector<double> stat_vals, energy_val, stat_err, energy_err;
			Measure stat_meas;
			for(int energy:analysis::energy_list) {
				energy_val.push_back(plot::energy_match[energy]);
				energy_err.push_back(0.0);
				stat_meas = stats[energy][div][cent][stat_name];
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
		}
	}


	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"%").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int div:divs) {
			vector<double> stat_vals, energy_val, stat_err, energy_err;
			Measure stat_meas;
			for(int energy:analysis::energy_list) {
				energy_val.push_back(plot::energy_match[energy]);
				energy_err.push_back(0.0);
				stat_meas = stats[energy][div][cent][stat_name];
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
			graph->SetNameTitle((stat_name + " " + to_string(div) + " divisions").data());
			graph->SetMarkerStyle(plot::div_marker_styles[div]);
			graph->SetMarkerColor(plot::div_marker_colors[div]);
			graph->SetMarkerSize(plot::div_marker_sizes[div]);
			graph->SetLineColor(plot::div_marker_colors[div]);
			mg->Add(graph, "AP");
			if(can_index == 1) {
				leg->AddEntry(graph, (stat_name + " " + to_string(div) + " divs").data(), "p");
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.05); }
		if(can_index > can_div.first) { gPad->SetTopMargin(0.07); }
		else { gPad->SetTopMargin(0.08); }
		gPad->SetRightMargin(0.02);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}

	can->Update();
	can->Write(name.data());
	delete can;
}


void make_comp_stat_plot_hack(map<int, map<string, Measure>> dylan, map<int, map<string, Measure>> roli) {
	vector<double> roli_sys = {0.004139200212004939, 0.0032558531832097806, 0.004697130111456425, 0.0021703333166663815, 0.004128883299661329, 0.006058885056950112, 0.0051792985941065016, 0.003855254850777629, 0.010578548892656355};
	for(string stat:analysis::stat_names) {
		string name = "Moment Comparison " + stat;
		auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);

		auto *mg = new TMultiGraph();
		mg->SetNameTitle(stat.data(), stat.data());

		vector<double> stat1_val, stat1_err, stat2_val, stat2_err, cent_vec, cent_err;
		Measure stat1_meas, stat2_meas;
		for(int cent=1; cent<=9; cent++) {
			stat1_meas = dylan[cent][stat];
			stat2_meas = roli[cent][stat];
			if(!isnan(stat1_meas.get_val())) { stat1_val.push_back(stat1_meas.get_val()); }
			if(!isnan(stat1_meas.get_err())) { stat1_err.push_back(stat1_meas.get_err()); }
			if(!isnan(stat2_meas.get_val())) { stat2_val.push_back(stat2_meas.get_val()); }
			if(!isnan(stat2_meas.get_err())) { stat2_err.push_back(stat2_meas.get_err()); }
			cent_vec.push_back((double)cent);
			cent_err.push_back(0.0);
		}

		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		for(unsigned i = 0; i < cent_vec.size(); i++) {
			cout << cent_vec[i] << "\t" << stat1_val[i] << " ± " << stat1_err[i] << "\t" << stat2_val[i]  << " ± " << stat2_err[i] << endl;
			if(stat1_val[i] + stat1_err[i] > y_max) { y_max = stat1_val[i] + stat1_err[i]; }
			if(stat2_val[i] + stat2_err[i] > y_max) { y_max = stat2_val[i] + stat2_err[i]; }
			if(stat1_val[i] - stat1_err[i] < y_min) { y_min = stat1_val[i] - stat1_err[i]; }
			if(stat2_val[i] - stat2_err[i] < y_min) { y_min = stat2_val[i] - stat2_err[i]; }
		}

		TGraphErrors *graph1 = graph_x_vs_y_err(cent_vec, stat1_val, cent_err, stat1_err);
		graph1->SetNameTitle("Dylan");
		graph1->SetMarkerStyle(26);
		graph1->SetMarkerColor(kRed);
		graph1->SetMarkerSize(1.5);
		mg->Add(graph1, "AP");

		TGraphErrors *graph2 = graph_x_vs_y_err(cent_vec, stat2_val, cent_err, stat2_err);
		graph2->SetNameTitle("Roli");
		graph2->SetMarkerStyle(24);
		graph2->SetMarkerColor(kBlue);
		graph2->SetMarkerSize(1.5);
		mg->Add(graph2, "AP");

		TGraphErrors *graph3 = graph_x_vs_y_err(cent_vec, stat2_val, cent_err, roli_sys);
		mg->Add(graph3, "[]");

		double y_range = y_max - y_min;

		mg->GetXaxis()->SetLimits(0, 10);
		mg->GetXaxis()->SetRangeUser(0, 10);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetXaxis()->SetTitle("Centrality Bin");

		mg->Draw("AP");
		gPad->BuildLegend(0.1, 0.5, 0.2, 0.55, "", "p");

		can->Write(name.data());
		delete can;
	}
}


pair<int, int> get_cent9_range(int bin) {
	pair<int, int> range;
	if(bin == 8) { range.first = 0; range.second = 5; }
	else if(bin == 7) { range.first = 5; range.second = 10; }
	else { range.first = 10 + (6 - bin) * 10; range.second = 10 + (7 - bin) * 10; }

	return(range);
}


pair<int, int> get_canvas_div(int plots) {
	pair<int, int> divs;
	divs.first = ceil(pow(plots, 0.5));
	divs.second = ceil((float)plots / divs.first);

	return(divs);
}
