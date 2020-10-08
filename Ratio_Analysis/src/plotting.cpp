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
#include <THStack.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TStyle.h>

#include "ratio_methods.h"
#include "plotting.h"
#include "file_io.h"

#include <AzimuthBinData.h>

#include "config_analysis.h"
#include "Stats.h"
#include "Measure.h"

using namespace std;



void make_ratio_dist_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data) {
	TDirectory *ratio_dist_dir = out_root->mkdir(plot::ratio_dist_dir_name.data());
	create_ratio_dist_plots(ratio_dist_dir, data);
}

void make_ratio_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data) {
	TDirectory *ratio_dist_dir = out_root->mkdir(plot::ratio_dist_dir_name.data());
	create_ratio_dist_plots(ratio_dist_dir, data);
}

void create_ratio_dist_plots(TDirectory *ratio_dist_dir, map<int, map<int, map<int, AzimuthBinData>>> data) {
	ratio_dist_dir->cd();
	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		TDirectory *energy_dir = ratio_dist_dir->mkdir((to_string(energy.first) + "GeV").data());
		energy_dir->cd();
		for(pair<int, map<int, AzimuthBinData>> div:energy.second) {
			TDirectory *div_dir = energy_dir->mkdir((to_string(div.first) + "_Degree_Bins").data());
			div_dir->cd();
			for(pair<int, AzimuthBinData> cent:div.second) {
				cent.second.canvas_ratio_dist(to_string(energy.first) + "GeV " + to_string(div.first) + " degree bins Centrality " + to_string(cent.first));
			}
		}
	}
}


void hist_ratio_dist(map<int, map<int, int>> ratios, int energy, int div, int cent, string mode, int color) {
	string name = to_string(energy) + "GeV " + to_string(div) + " degree bins Centrality " + to_string(cent);
	TH1D *ratio_hist = new TH1D(name.data(), name.data(), plot::ratio_hist_bins, plot::ratio_hist_low, plot::ratio_hist_high);
	ratio_hist->SetLineColor(color);
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
	} else if(mode == "draw_same") {
		ratio_hist->Draw("HISTsame");
	}
}


void hist_diff_dist(map<double, int> diffs, int energy, int div, int cent, string mode, int color) {
	string name = to_string(energy) + "GeV " + to_string(div) + " divisions Centrality " + to_string(cent);
	TH1D *diff_hist = new TH1D(name.data(), name.data(), 23, -11.5, 11.5);
	diff_hist->SetLineColor(color);
	for(pair<double, int> bin:diffs) {
		diff_hist->Fill((double)bin.first, bin.second);
	}

	if(mode == "write") {
		diff_hist->Write();
		delete diff_hist;
	} else if(mode == "draw") {
		diff_hist->Draw("HIST");
	} else if(mode == "draw_same") {
		diff_hist->Draw("HISTsame");
	}
}

void make_diff_dist_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data) {
	TDirectory *diff_dist_dir = out_root->mkdir("Difference_Plots");
	create_diff_dist_plots(diff_dist_dir, data);
}

void make_diff_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data) {
	TDirectory *diff_dist_dir = out_root->mkdir("Difference_Plots");
	create_diff_dist_plots(diff_dist_dir, data);
}

void create_diff_dist_plots(TDirectory *diff_dist_dir, map<int, map<int, map<int, AzimuthBinData>>> data) {
	diff_dist_dir->cd();
	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		TDirectory *energy_dir = diff_dist_dir->mkdir((to_string(energy.first) + "GeV").data());
		energy_dir->cd();
		for(pair<int, map<int, AzimuthBinData>> div:energy.second) {
			TDirectory *div_dir = energy_dir->mkdir((to_string(div.first) + "_Degree_Bins").data());
			div_dir->cd();
			for(pair<int, AzimuthBinData> cent:div.second) {
				cent.second.canvas_diff_dist("Diff " +to_string(energy.first) + "GeV " + to_string(div.first) + " degree bins Centrality " + to_string(cent.first), "no");
				cent.second.canvas_diff_dist("Pull " + to_string(energy.first) + "GeV " + to_string(div.first) + " degree bins Centrality " + to_string(cent.first), "yes");
			}
		}
	}
}


void make_2d_dist_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data) {
	TDirectory *dist_dir = out_root->mkdir(plot::dist_2d_dir_name.data());
	create_2d_dist_plots(dist_dir, data);
}

void make_2d_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data) {
	TDirectory *dist_dir = out_root->mkdir(plot::dist_2d_dir_name.data());
	create_2d_dist_plots(dist_dir, data);
}


void create_2d_dist_plots(TDirectory *dist_dir, map<int, map<int, map<int, AzimuthBinData>>> data) {
	dist_dir->cd();

	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		TDirectory *energy_dir = dist_dir->mkdir((to_string(energy.first) + "GeV").data());
		energy_dir->cd();
		for(pair<int, map<int, AzimuthBinData>> div:energy.second) {
			TDirectory *div_dir = energy_dir->mkdir((to_string(div.first) + "_Degree_Bins").data());
			div_dir->cd();
			for(pair<int, AzimuthBinData> cent:div.second) {
				cent.second.canvas_2d_dist(to_string(energy.first) + "GeV " + to_string(div.first) + " degree bins Centrality " + to_string(cent.first));
			}
		}
	}
}


void canvas_2d_dist(map<int, map<int, int>> dist, int energy, int div, int cent) {
	string name = to_string(energy) + "GeV " + to_string(div) + " degree bins Centrality " + to_string(cent);
	TH2I *hist = ratios_map_to_hist(dist, name);
	TCanvas *can = new TCanvas(name.data()); // Memory leak
	TF1 *avg_line = new TF1(name.data(), ("x/360*"+to_string(div)).data(), -0.5, 0.5+(--dist.end())->first);
	TF1 *max_line = new TF1((name+"max").data(), "x", -0.5, 0.5+(--dist.end())->first);
	max_line->SetLineColor(4);
	can->SetLogz();
	hist->Draw("colz");
	max_line->Draw("same");
	avg_line->Draw("same");
	hist->Write();
	can->Write();
}


void make_proton_dist_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data) {
	TDirectory *nproton_dist_dir = out_root->mkdir(plot::nproton_dist_dir_name.data());
	create_proton_dist_plots(nproton_dist_dir, data);
}

void make_proton_dist_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data) {
	TDirectory *nproton_dist_dir = out_root->mkdir(plot::nproton_dist_dir_name.data());
	create_proton_dist_plots(nproton_dist_dir, data);
}

void create_proton_dist_plots(TDirectory *nproton_dist_dir, map<int, map<int, map<int, AzimuthBinData>>> data) {
	nproton_dist_dir->cd();
	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		TDirectory *energy_dir = nproton_dist_dir->mkdir((to_string(energy.first) + "GeV").data());
		energy_dir->cd();
		for(pair<int, AzimuthBinData> cent:energy.second.begin()->second) {
			cent.second.canvas_proton_dist(to_string(energy.first) + "GeV protons Centrality " + to_string(cent.first));
		}
	}
}


void hist_proton_dist(map<int, int> nprotons, int energy, int cent, string mode) {
	string name = to_string(energy) + "GeV protons Centrality " + to_string(cent);
	int min_num = (*min_element(nprotons.begin(), nprotons.end())).first;
	int max_num = (*max_element(nprotons.begin(), nprotons.end())).first;
	TH1D *protons_hist = new TH1D(name.data(), name.data(), max_num - min_num + 5, min_num - 2.5, max_num + 2.5);
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
	string name = to_string(div) + " degree bins " + to_string(cent) + " centrality " + to_string(order) + " order";
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
	string name = to_string(div) + " degree bins " + to_string(cent) + " centrality " + stat;
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



void make_canvas_plots(TFile *out_root, map<int, map<int, map<int, AzimuthBinData>>> data, vector<int> cents, vector<int> divs) {
	TDirectory *can_dir = out_root->mkdir(plot::canvas_dir_name.data());
	create_canvas_plots(can_dir, data, cents, divs);
}

void make_canvas_plots(TDirectory *out_root, map<int, map<int, map<int, AzimuthBinData>>> data, vector<int> cents, vector<int> divs) {
	TDirectory *can_dir = out_root->mkdir(plot::canvas_dir_name.data());
	create_canvas_plots(can_dir, data, cents, divs);
	can_dir->cd();
}


void create_canvas_plots(TDirectory *can_dir, map<int, map<int, map<int, AzimuthBinData>>> data, vector<int> cents, vector<int> divs) {
	can_dir->cd();

	// Proton distribution canvases
	TDirectory *nproton_can_dir = can_dir->mkdir(plot::nproton_dist_dir_name.data());
	nproton_can_dir->cd();
	for(int cent:cents) {
		canvas_nprotons(data, cent, "Centrality " + to_string(cent));
	}

	// Ratios distribution canvases
	TDirectory *ratios_can_dir = can_dir->mkdir(plot::ratio_dist_dir_name.data());
	ratios_can_dir->cd();
	for(int div:divs) {
		TDirectory *div_dir = ratios_can_dir->mkdir((to_string(div) + "_Degree_Bins").data());
		div_dir->cd();
		for(int cent:cents) {
			canvas_ratio_dists(data, div, cent, to_string(div) + " degree bins " + to_string(cent) + " centrality");
		}
	}

	// Difference distribution canvases
	TDirectory *diff_can_dir = can_dir->mkdir("Diff_Dist_Canvases");
	diff_can_dir->cd();
	for(int div:divs) {
		TDirectory *div_dir = diff_can_dir->mkdir((to_string(div) + "_Degree_Bins").data());
		div_dir->cd();
		for(int cent:cents) {
			canvas_diff_dists(data, div, cent, to_string(div) + " degree bins " + to_string(cent) + " centrality");
		}
	}
}


void canvas_nprotons(map<int, map<int, map<int, AzimuthBinData>>> data, int cent, string name) {
	TCanvas *proton_can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	proton_can->SetTitle(name.data());
	proton_can->SetName(name.data());
	proton_can->Divide(ceil(pow(data.size(),0.5)), ceil(data.size()/ceil(pow(data.size(), 0.5))), plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(pair<int, map<int, map<int, AzimuthBinData>>>  energy:data) {
		proton_can->cd(can_index);
		hist_proton_dist((*energy.second.begin()).second[cent].get_proton_dist(), energy.first, cent, "draw"); // Memory leak
		can_index++;
	}
	proton_can->Write(name.data());
	delete proton_can;
}


void canvas_ratio_dists(map<int, map<int, map<int, AzimuthBinData>>> data, int div, int cent, string name) {
	TCanvas *ratio_can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	ratio_can->SetTitle(name.data());
	ratio_can->SetName(name.data());
	ratio_can->Divide(ceil(pow(data.size(),0.5)), ceil(data.size()/ceil(pow(data.size(), 0.5))), plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		ratio_can->cd(can_index);
		gPad->SetLogy();
		hist_ratio_dist(energy.second[div][cent].get_bin_data(), energy.first, div, cent, "draw");
		can_index++;
	}
	ratio_can->Write(name.data());
	delete ratio_can;
}


void canvas_diff_dists(map<int, map<int, map<int, AzimuthBinData>>> data, int div, int cent, string name) {
	TCanvas *diff_can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	diff_can->SetTitle(name.data());
	diff_can->SetName(name.data());
	diff_can->Divide(ceil(pow(data.size(),0.5)), ceil(data.size()/ceil(pow(data.size(), 0.5))), plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		diff_can->cd(can_index);
		gPad->SetLogy();
		hist_diff_dist(energy.second[div][cent].get_diff_hist("no"), energy.first, div, cent, "draw");
		can_index++;
	}
	diff_can->Write(name.data());
	delete diff_can;
}


void canvas_ratio_dists(map<int, map<int, map<int, AzimuthBinData>>> data, vector<int> divs, vector<int> cents, string name) {
//	double y_max = numeric_limits<double>::min(); //---
//	double y_min = numeric_limits<double>::max(); //---

	vector<TH1D*> hists;
	vector<THStack*> stacks;
	vector<TLegend*> legends;

	TCanvas *ratio_can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	ratio_can->SetTitle(name.data());
	ratio_can->SetName(name.data());
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);

	pair<int, int> can_div = {ceil(pow(data.size(),0.5)), ceil(data.size()/ceil(pow(data.size(), 0.5)))};
	ratio_can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
	int can_index = 0;

	for(pair<int, map<int, map<int, AzimuthBinData>>> energy:data) {
		ratio_can->cd(++can_index);
		THStack *stack = new THStack((to_string(energy.first) + "GeV").data(), (to_string(energy.first) + "GeV").data());
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		stacks.push_back(stack);
		legends.push_back(leg);

		for(int cent:cents) {
			for(int div:divs) {
				string name = to_string(energy.first) + "GeV " + to_string(div) + " degree bins Centrality " + to_string(cent);
				TH1D *ratio_hist = new TH1D(name.data(), name.data(), plot::ratio_hist_bins, plot::ratio_hist_low, plot::ratio_hist_high);
				ratio_hist->SetLineColor(plot::cent_marker_colors[cent]);
				for(pair<int, map<int, int>> event:energy.second[div][cent].get_bin_data()) {
					for(pair<int, int> bin:event.second) {
						ratio_hist->Fill(((double)bin.first) / event.first, bin.second);
					}
				}
				hists.push_back(ratio_hist);
				stack->Add(ratio_hist);

				if(can_index == 1) {
					pair<int, int> range = get_cent9_range(cent);
					leg->AddEntry(ratio_hist, (to_string(div) + "#circ bins \t" + to_string(range.first)+"-"+to_string(range.second)+"% central").data(), "l");
				}
			}
		}
		stack->Draw("nostackHIST");
		gStyle->SetOptStat(0);
		gPad->SetLogy();
		if(can_index > can_div.first*(can_div.second-1)) { gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.05); }
		if(can_index > can_div.first) { gPad->SetTopMargin(0.07); }
		else { gPad->SetTopMargin(0.08); }
		gPad->SetRightMargin(0.02);
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }

	}
	ratio_can->Update();
	ratio_can->Write(name.data());
	for(TH1D* hist:hists) { delete hist; }
	for(THStack* stack:stacks) { delete stack; }
	for(TLegend* legend:legends) { delete legend; }
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
		graph->SetNameTitle((to_string(div) + " degree bins").data());
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
		graph->SetNameTitle((to_string(div) + " degree bins").data());
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
	int div = 60; //Hardcode, fix.
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	can->Divide(2,2, plot::can_div_x, plot::can_div_y);
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
	int div = 60; //Hardcode, fix.
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	can->Divide(2,2, plot::can_div_x, plot::can_div_y);
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
	can->Divide((int)stat_names.size(), (int)cents.size(), plot::can_div_x, plot::can_div_y);
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
				graph->SetNameTitle((to_string(div) + " degree bins").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[div]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(graph, "AP");
				if(can_index == 1) { leg->AddEntry(graph, (to_string(div) + "#circ bins").data(), "p"); }
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
	can->Divide((int)stat_names.size(), (int)cents.size(), plot::can_div_x, plot::can_div_y);
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
				graph->SetNameTitle((to_string(div) + " degree bins").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[div]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(graph, "APZ");
				TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
				sys_graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(sys_graph, "[]");
				if(can_index == 1) { leg->AddEntry(graph, (to_string(div) + "#circ bins").data(), "p"); }
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
	can->Divide((int)stat_names.size(), (int)cents.size(), plot::can_div_x, plot::can_div_y);
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
					graph->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
					graph->SetMarkerStyle(plot::div_marker_styles[div]);
					graph->SetMarkerColor(plot::div_marker_colors[div]);
					graph->SetMarkerSize(plot::div_marker_sizes[div]);
					graph->SetLineColor(plot::div_marker_colors[div]);
					mg->Add(graph, "APZ");
					TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
					sys_graph->SetLineColor(plot::div_marker_colors[div]);
					mg->Add(sys_graph, "[]");
					if(can_index == 1) { leg->AddEntry(graph, (data_set.first + " " + to_string(div) + "#circ bins").data(), "p"); }
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
	can->Divide((int)stat_names.size(), (int)cents.size(), plot::can_div_x, plot::can_div_y);
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
					graph->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
					graph->SetMarkerStyle(plot::div_marker_styles[div]);
					graph->SetMarkerColor(plot::div_marker_colors[div]);
					graph->SetMarkerSize(plot::div_marker_sizes[div]);
					graph->SetLineColor(plot::div_marker_colors[div]);
					mg->Add(graph, "AP");
					if(can_index == 1) { leg->AddEntry(graph, (data_set.first + " " + to_string(div) + "#circ bins").data(), "p"); }
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
//	gStyle->SetTitleFontSize(0.09);
//	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"% Centrality").data());
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
					if(in_string(name, "_cor") && stat_name == "standard_deviation") {
						Measure dmean_meas = data_set.second[energy][div][cent]["particle_dist_mean"];
						stat_meas = (stat_meas - 1) / dmean_meas * 50 + 1;
					}
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					stat_sys.push_back(sys[data_set.first][energy][div][cent][stat_name]);
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
					if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
				graph->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[div]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(graph, "APLZ");
				TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
				sys_graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(sys_graph, "[]");
				set_num++;
				if(can_index == 1) {
					leg->SetBorderSize(plot::legend_border_width);
					leg->SetFillStyle(0);
					leg->AddEntry(graph, (data_set.first + " " + stat_name + " " + to_string(div) + "#circ bins").data(), "lp");
				}
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)");}// mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
//		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}
	can->Update();
	can->Write(name.data());
	delete can;
}


void centralities_stat(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, string stat_name, vector<int> cents, vector<int> divs, string name) {
	double y_max = numeric_limits<double>::min(); //---
	double y_min = numeric_limits<double>::max(); //---
	for(auto stat:stats) {
		for(int cent:cents) {
			for(int div:divs) {
				vector<double> stat_vals, stat_err;
				Measure stat_meas;
				for(int energy:analysis::energy_list) {
					stat_meas = stat.second[energy][div][cent][stat_name];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					if(stat_meas.get_val() > 0) {
						if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
						if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					}
				}
			}
		}
	}

	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
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
				graph->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[div]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(graph, "AP");
				set_num++;
				if(can_index == 1) {
					leg->SetBorderSize(plot::legend_border_width);
					leg->SetFillStyle(0);
					leg->AddEntry(graph, (data_set.first + " " + stat_name + " " + to_string(div) + "#circ bins").data(), "p");
				}
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.06);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
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
			if(divs.size() > 1 && div == 2 && stat_name == "skewness") { continue; }
			vector<double> stat_vals, stat_err, stat_sys;
			Measure stat_meas;
			for(int energy:analysis::energy_list) {
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
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
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
			if(divs.size() > 1 && div == 180 && stat_name == "skewness") { continue; }
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
			graph->SetNameTitle((stat_name + " " + to_string(div) + " degree bins").data());
			graph->SetMarkerStyle(plot::div_marker_styles[div]);
			graph->SetMarkerColor(plot::div_marker_colors[div]);
			graph->SetMarkerSize(plot::div_marker_sizes[div]);
			graph->SetLineColor(plot::div_marker_colors[div]);
			mg->Add(graph, "APLZ");
			TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
			sys_graph->SetLineColor(plot::div_marker_colors[div]);
			mg->Add(sys_graph, "[]");
			if(can_index == 1) {
				leg->SetBorderSize(plot::legend_border_width);
				leg->SetFillStyle(0);
				leg->AddEntry(graph, (stat_name + " " + to_string(div) + "#circ bins").data(), "lp");
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
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
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
			vector<double> stat_vals, stat_err;
			Measure stat_meas;
			for(int energy:analysis::energy_list) {
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
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
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
			graph->SetNameTitle((stat_name + " " + to_string(div) + " degree bins").data());
			graph->SetMarkerStyle(plot::div_marker_styles[div]);
			graph->SetMarkerColor(plot::div_marker_colors[div]);
			graph->SetMarkerSize(plot::div_marker_sizes[div]);
			graph->SetLineColor(plot::div_marker_colors[div]);
			mg->Add(graph, "AP");
			if(can_index == 1) {
				leg->SetBorderSize(plot::legend_border_width);
				leg->SetFillStyle(0);
				leg->AddEntry(graph, (stat_name + " " + to_string(div) + "#circ bins").data(), "p");
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}

	can->Update();
	can->Write(name.data());
	delete can;
}


void division_stat(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, string stat_name, vector<int> cents, vector<int> divs, vector<int> energies, string name) {
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
//	gStyle->SetTitleFontSize(0.09);
//	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"% Centrality").data());
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int energy:energies) {
			int set_num = 0;
			for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats) {
				vector<double> stat_vals, div_val, stat_err, div_err, stat_sys;
				Measure stat_meas;
				for(int div:divs) {
					div_val.push_back(div + set_num*0.4);  // Offset sets on x (Energy) axis.
					div_err.push_back(0.0);
					stat_meas = data_set.second[energy][div][cent][stat_name];
					if(in_string(name, "_cor") && stat_name == "standard_deviation") {
						Measure dmean_meas = data_set.second[energy][div][cent]["particle_dist_mean"];
						stat_meas = (stat_meas - 1) / dmean_meas * 50 + 1;
					}
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					stat_sys.push_back(sys[data_set.first][energy][div][cent][stat_name]);
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
					if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(div_val, stat_vals, div_err, stat_err);
				graph->SetNameTitle((data_set.first + " " + to_string(energy) + "GeV").data());
				graph->SetMarkerStyle(plot::energy_marker_styles[energy]);
				graph->SetMarkerColor(plot::energy_marker_colors[energy]);
				graph->SetMarkerSize(plot::energy_marker_sizes[energy]);
				graph->SetLineColor(plot::energy_marker_colors[energy]);
				mg->Add(graph, "APLZ");
				TGraphErrors *sys_graph = graph_x_vs_y_err(div_val, stat_vals, div_err, stat_sys);
				sys_graph->SetLineColor(plot::energy_marker_colors[energy]);
				mg->Add(sys_graph, "[]");
				set_num++;
				if(can_index == 1) {
					leg->SetBorderSize(plot::legend_border_width);
					leg->SetFillStyle(0);
					leg->AddEntry(graph, (data_set.first + " " + stat_name + " " + to_string(energy) + "GeV").data(), "lp");
				}
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 360);
		mg->GetXaxis()->SetRangeUser(0, 360);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Bin Width (#circ)");}// mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
//		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}
	can->Update();
	can->Write(name.data());
	delete can;
}


void division_stat(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, string stat_name, vector<int> cents, vector<int> divs, vector<int> energies, string name) {
	double y_max = numeric_limits<double>::min(); //---
	double y_min = numeric_limits<double>::max(); //---
	for(auto stat:stats) {
		for(int cent:cents) {
			for(int energy:energies) {
				vector<double> stat_vals, stat_err;
				Measure stat_meas;
				for(int div:divs) {
					stat_meas = stat.second[energy][div][cent][stat_name];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					if(stat_meas.get_val() > 0) {
						if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
						if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					}
				}
			}
		}
	}

	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"%").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int energy:energies) {
			int set_num = 0;
			for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats) {
				vector<double> stat_vals, div_val, stat_err, div_err;
				Measure stat_meas;
				for(int div:divs) {
					div_val.push_back(div + set_num*0.4);  // Offset sets on x (Energy) axis.
					div_err.push_back(0.0);
					stat_meas = data_set.second[energy][div][cent][stat_name];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(div_val, stat_vals, div_err, stat_err);
				graph->SetNameTitle((data_set.first + " " + to_string(energy) + "GeV").data());
				graph->SetMarkerStyle(plot::energy_marker_styles[energy]);
				graph->SetMarkerColor(plot::energy_marker_colors[energy]);
				graph->SetMarkerSize(plot::energy_marker_sizes[energy]);
				graph->SetLineColor(plot::energy_marker_colors[energy]);
				mg->Add(graph, "AP");
				set_num++;
				if(can_index == 1) {
					leg->SetBorderSize(plot::legend_border_width);
					leg->SetFillStyle(0);
					leg->AddEntry(graph, (data_set.first + " " + stat_name + " " + to_string(energy) + "GeV").data(), "p");
				}
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 360);
		mg->GetXaxis()->SetRangeUser(0, 360);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Bin Width (#circ)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.06);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}
	can->Update();
	can->Write(name.data());
	delete can;
}



void division_stat(map<int, map<int, map<int, map<string, Measure>>>> stats, map<int, map<int, map<int, map<string, double>>>> sys, string stat_name, vector<int> cents, vector<int> divs, vector<int> energies, string name) {
	double y_max = numeric_limits<double>::min(); //---
	double y_min = numeric_limits<double>::max(); //---
	for(int cent:cents) {
		for(int energy:energies) {
//			if(divs.size() > 1 && div == 2 && stat_name == "skewness") { continue; }
			vector<double> stat_vals, stat_err, stat_sys;
			Measure stat_meas;
			for(int div:divs) {
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
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"%").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int energy:energies) {
//			if(divs.size() > 1 && div == 180 && stat_name == "skewness") { continue; }
			vector<double> stat_vals, div_val, stat_err, div_err, stat_sys;
			Measure stat_meas;
			for(int div:divs) {
				div_val.push_back(div);
				div_err.push_back(0.0);
				stat_meas = stats[energy][div][cent][stat_name];
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				stat_sys.push_back(sys[energy][div][cent][stat_name]);
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
				if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
			}
			TGraphErrors *graph = graph_x_vs_y_err(div_val, stat_vals, div_err, stat_err);
			graph->SetNameTitle((stat_name + " " + to_string(energy) + "GeV").data());
			graph->SetMarkerStyle(plot::energy_marker_styles[energy]);
			graph->SetMarkerColor(plot::energy_marker_colors[energy]);
			graph->SetMarkerSize(plot::energy_marker_sizes[energy]);
			graph->SetLineColor(plot::energy_marker_colors[energy]);
			mg->Add(graph, "APLZ");
			TGraphErrors *sys_graph = graph_x_vs_y_err(div_val, stat_vals, div_err, stat_sys);
			sys_graph->SetLineColor(plot::energy_marker_colors[energy]);
			mg->Add(sys_graph, "[]");
			if(can_index == 1) {
				leg->SetBorderSize(plot::legend_border_width);
				leg->SetFillStyle(0);
				leg->AddEntry(graph, (stat_name + " " + to_string(energy) + "GeV").data(), "lp");
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 360);
		mg->GetXaxis()->SetRangeUser(0, 360);
		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Bin Width (#circ)"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}

	can->Update();
	can->Write(name.data());
	delete can;
}


void division_stat(map<int, map<int, map<int, map<string, Measure>>>> stats, string stat_name, vector<int> cents, vector<int> divs, vector<int> energies, string name) {
	double y_max = numeric_limits<double>::min(); //---
	double y_min = numeric_limits<double>::max(); //---
	for(int cent:cents) {
		for(int energy:energies) {
			vector<double> stat_vals, stat_err;
			Measure stat_meas;
			for(int div:divs) {
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
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"%").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int energy:energies) {
			vector<double> stat_vals, div_val, stat_err, div_err;
			Measure stat_meas;
			for(int div:divs) {
				div_val.push_back(div);
				div_err.push_back(0.0);
				stat_meas = stats[energy][div][cent][stat_name];
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			TGraphErrors *graph = graph_x_vs_y_err(div_val, stat_vals, div_err, stat_err);
			graph->SetNameTitle((stat_name + " " + to_string(energy) + "GeV").data());
			graph->SetMarkerStyle(plot::energy_marker_styles[energy]);
			graph->SetMarkerColor(plot::energy_marker_colors[energy]);
			graph->SetMarkerSize(plot::energy_marker_sizes[energy]);
			graph->SetLineColor(plot::energy_marker_colors[energy]);
			mg->Add(graph, "AP");
			if(can_index == 1) {
				leg->SetBorderSize(plot::legend_border_width);
				leg->SetFillStyle(0);
				leg->AddEntry(graph, (stat_name + " " + to_string(energy) + "GeV").data(), "p");
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 360);
		mg->GetXaxis()->SetRangeUser(0, 360);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Bin Width (#circ)"); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}

	can->Update();
	can->Write(name.data());
	delete can;
}


void stat_vs_mult_mean(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name) {
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	map<int, vector<TF1*>> lines;
//	gStyle->SetTitleFontSize(0.09);
//	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"% Centrality " + stat_name).data());
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		double x_max = numeric_limits<double>::min();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int div:divs) {
			int set_num = 0;
			for(int energy:analysis::energy_list) {
				vector<double> stat_vals, dmean_val, stat_err, dmean_err, stat_sys, total_err;
				Measure stat_meas, dmean_meas;
				for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats) {
					dmean_meas = data_set.second[energy][div][cent]["particle_dist_mean"];
					dmean_val.push_back(dmean_meas.get_val());  // Offset sets on x (Energy) axis.
					dmean_err.push_back(dmean_meas.get_err());
					stat_meas = data_set.second[energy][div][cent][stat_name];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					stat_sys.push_back(sys[data_set.first][energy][div][cent][stat_name]);
					total_err.push_back(pow(pow(sys[data_set.first][energy][div][cent][stat_name], 2) + pow(stat_meas.get_err(), 2), 0.5));
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
					if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
					if(dmean_meas.get_val() + dmean_meas.get_err() > x_max) { x_max = dmean_meas.get_val() + dmean_meas.get_err(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(dmean_val, stat_vals, dmean_err, stat_err);
				graph->SetNameTitle((to_string(energy) + "GeV " + to_string(div) + " degree bins").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[div]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[div]);
				TGraphErrors *graph_fit = graph_x_vs_y_err(dmean_val, stat_vals, dmean_err, total_err);
				lines[can_index].push_back(new TF1((to_string(energy) + "GeV_" + to_string(div) + "_degree_bins_fit").data(), "[0]+[1]*x", 0, *max_element(dmean_val.begin(), dmean_val.end()) * 1.05));
				lines[can_index].back()->SetLineColor(plot::div_marker_colors[div]);
				graph_fit->Fit(lines[can_index].back(), "Q");
				mg->Add(graph, "APZ");
				TGraphErrors *sys_graph = graph_x_vs_y_err(dmean_val, stat_vals, dmean_err, stat_sys);
				sys_graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(sys_graph, "[]");
				set_num++;
				if(can_index == 1) {
					leg->SetBorderSize(plot::legend_border_width);
					leg->SetFillStyle(0);
					string b = to_string(lines[can_index].back()->GetParameter(0));
					string m = to_string(lines[can_index].back()->GetParameter(1));
					string b_err = to_string(lines[can_index].back()->GetParError(0));
					string m_err = to_string(lines[can_index].back()->GetParError(1));
					string fit_lab = "";
					if(in_string(name, "_cor")) {
						string fit_lab = "y= " + m + "+-" + m_err + "x + " + b + "+-" + b_err; //±
					}
					leg->AddEntry(graph, (to_string(energy) + "GeV " + to_string(div) + "#circ bins " + fit_lab).data(), "p");
				}
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, x_max*1.05);
		mg->GetXaxis()->SetRangeUser(0, x_max*1.05);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Average Number of Particles per Event");}// mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
//		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		if(in_string(name, "_cor")) { for(auto line:lines[can_index]) { line->Draw("Same"); } }
		can_index++;
	}
	can->Update();
	can->Write(name.data());
	delete can;
	for(auto can_lines:lines) { for(auto line:can_lines.second) { delete line; } }
}


void stat_vs_mult_mean(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, string stat_name, vector<int> cents, vector<int> divs, string name) {
	double y_max = numeric_limits<double>::min(); //---
	double y_min = numeric_limits<double>::max(); //---
	for(auto stat:stats) {
		for(int cent:cents) {
			for(int div:divs) {
				vector<double> stat_vals, energy_val, stat_err, energy_err;
				Measure stat_meas;
				for(int energy:analysis::energy_list) {
					energy_val.push_back(plot::energy_match[energy]);
					energy_err.push_back(0.0);
					stat_meas = stat.second[energy][div][cent][stat_name];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					if(stat_meas.get_val() > 0) {
						if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
						if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					}
				}
			}
		}
	}

	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
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
			int set_num = 0;
			for(int energy:analysis::energy_list) {
				vector<double> stat_vals, dmean_val, stat_err, dmean_err;
				Measure stat_meas, dmean_meas;
				for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats) {
					dmean_meas = data_set.second[energy][div][cent]["particle_dist_mean"];
					dmean_val.push_back(dmean_meas.get_val());  // Offset sets on x (Energy) axis.
					dmean_err.push_back(dmean_meas.get_err());
					stat_meas = data_set.second[energy][div][cent][stat_name];
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(dmean_val, stat_vals, dmean_err, stat_err);
				graph->SetNameTitle((to_string(energy) + "GeV " + to_string(div) + " degree bins").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[div]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(graph, "APZ");
				set_num++;
				if(can_index == 1) {
					leg->SetBorderSize(plot::legend_border_width);
					leg->SetFillStyle(0);
					leg->AddEntry(graph, (to_string(energy) + "GeV " + stat_name + " " + to_string(div) + "#circ bins").data(), "p");
				}
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Average Number of Particles per Event"); mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.06);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}
	can->Update();
	can->Write(name.data());
	delete can;
}


// Not implemented
void stat_vs_mult_mean(map<int, map<int, map<int, map<string, Measure>>>> stats, map<int, map<int, map<int, map<string, double>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name) {
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
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
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
			graph->SetNameTitle((stat_name + " " + to_string(div) + " degree bins").data());
			graph->SetMarkerStyle(plot::div_marker_styles[div]);
			graph->SetMarkerColor(plot::div_marker_colors[div]);
			graph->SetMarkerSize(plot::div_marker_sizes[div]);
			graph->SetLineColor(plot::div_marker_colors[div]);
			mg->Add(graph, "APZ");
			TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
			sys_graph->SetLineColor(plot::div_marker_colors[div]);
			mg->Add(sys_graph, "[]");
			if(can_index == 1) {
				leg->SetBorderSize(plot::legend_border_width);
				leg->SetFillStyle(0);
				leg->AddEntry(graph, (stat_name + " " + to_string(div) + "#circ bins").data(), "p");
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
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}

	can->Update();
	can->Write(name.data());
	delete can;
}


// Not implemented
void stat_vs_mult_mean(map<int, map<int, map<int, map<string, Measure>>>> stats, string stat_name, vector<int> cents, vector<int> divs, string name) {
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
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
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
			graph->SetNameTitle((stat_name + " " + to_string(div) + " degree bins").data());
			graph->SetMarkerStyle(plot::div_marker_styles[div]);
			graph->SetMarkerColor(plot::div_marker_colors[div]);
			graph->SetMarkerSize(plot::div_marker_sizes[div]);
			graph->SetLineColor(plot::div_marker_colors[div]);
			mg->Add(graph, "AP");
			if(can_index == 1) {
				leg->SetBorderSize(plot::legend_border_width);
				leg->SetFillStyle(0);
				leg->AddEntry(graph, (stat_name + " " + to_string(div) + "#circ bins").data(), "p");
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Energy (GeV)"); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		can_index++;
	}

	can->Update();
	can->Write(name.data());
	delete can;
}


void stat_vs_mult_mean_cor(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, string stat_name, vector<int> cents, vector<int> divs, string name) {
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	map<int, vector<TF1*>> lines;
//	gStyle->SetTitleFontSize(0.09);
//	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(cents.size());
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
	int can_index = 1;
	for(int cent:cents) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		pair<int, int> range = get_cent9_range(cent);
		mg->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"%").data(), (to_string(range.first)+"-"+to_string(range.second)+"% Centrality " + stat_name).data());
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		double x_max = numeric_limits<double>::min();
		TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		for(int div:divs) {
			int set_num = 0;
			for(int energy:analysis::energy_list) {
				vector<double> stat_vals, dmean_val, stat_err, dmean_err, stat_sys, total_err;
				Measure stat_meas, dmean_meas;
				for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats) {
					dmean_meas = data_set.second[energy][div][cent]["particle_dist_mean"];
					dmean_val.push_back(dmean_meas.get_val());  // Offset sets on x (Energy) axis.
					dmean_err.push_back(dmean_meas.get_err());
					stat_meas = data_set.second[energy][div][cent][stat_name];
					stat_meas = (stat_meas - 1) / dmean_meas * 50 + 1;
					stat_vals.push_back(stat_meas.get_val());
					stat_err.push_back(stat_meas.get_err());
					stat_sys.push_back(sys[data_set.first][energy][div][cent][stat_name]);
					total_err.push_back(pow(pow(sys[data_set.first][energy][div][cent][stat_name], 2) + pow(stat_meas.get_err(), 2), 0.5));
					if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
					if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
					if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
					if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
					if(dmean_meas.get_val() + dmean_meas.get_err() > x_max) { x_max = dmean_meas.get_val() + dmean_meas.get_err(); }
				}
				TGraphErrors *graph = graph_x_vs_y_err(dmean_val, stat_vals, dmean_err, stat_err);
				graph->SetNameTitle((to_string(energy) + "GeV " + to_string(div) + " degree bins").data());
				graph->SetMarkerStyle(plot::div_marker_styles[div]);
				graph->SetMarkerColor(plot::div_marker_colors[div]);
				graph->SetMarkerSize(plot::div_marker_sizes[div]);
				graph->SetLineColor(plot::div_marker_colors[div]);
				TGraphErrors *graph_fit = graph_x_vs_y_err(dmean_val, stat_vals, dmean_err, total_err);
				lines[can_index].push_back(new TF1((to_string(energy) + "GeV_" + to_string(div) + "_degree_bins_fit").data(), "[0]+[1]*x", 0, *max_element(dmean_val.begin(), dmean_val.end()) * 1.05));
				lines[can_index].back()->SetLineColor(plot::div_marker_colors[div]);
				graph_fit->Fit(lines[can_index].back(), "Q");
				mg->Add(graph, "APZ");
				TGraphErrors *sys_graph = graph_x_vs_y_err(dmean_val, stat_vals, dmean_err, stat_sys);
				sys_graph->SetLineColor(plot::div_marker_colors[div]);
				mg->Add(sys_graph, "[]");
				set_num++;
				if(can_index == 1) {
					leg->SetBorderSize(plot::legend_border_width);
					leg->SetFillStyle(0);
					string b = to_string(lines[can_index].back()->GetParameter(0));
					string m = to_string(lines[can_index].back()->GetParameter(1));
					string b_err = to_string(lines[can_index].back()->GetParError(0));
					string m_err = to_string(lines[can_index].back()->GetParError(1));
					string fit_lab = "y= " + m + "+-" + m_err + "x + " + b + "+-" + b_err; //±
					leg->AddEntry(graph, (to_string(energy) + "GeV " + to_string(div) + "#circ bins " + fit_lab).data(), "p");
				}
			}
		}
		double y_range = y_max - y_min;
		mg->GetXaxis()->SetLimits(0, x_max*1.05);
		mg->GetXaxis()->SetRangeUser(0, x_max*1.05);
//		mg->GetXaxis()->SetLabelSize(0.06);
		mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mg->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mg->GetXaxis()->SetTitle("Average Number of Particles per Event");}// mg->GetXaxis()->SetTitleSize(0.06); mg->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
//		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.04);
		gPad->SetLeftMargin(0.1);
		mg->Draw("AP"); // Multigraph memory leak, fix.
		if(can_index == 1) { leg->SetMargin(0.1); leg->Draw(); }
		if(in_string(name, {"pull", "divide"}, false)) { for(auto line:lines[can_index]) { line->Draw("Same"); } }
		can_index++;
	}
	can->Update();
	can->Write(name.data());
	delete can;
	for(auto can_lines:lines) { for(auto line:can_lines.second) { delete line; } }
}



void type_per_row(map<string, map<int, map<int, map<int, map<string, Measure>>>>> stats, map<string, map<int, map<int, map<int, map<string, double>>>>> sys, vector<string> types, vector<string> stat_names, vector<int> cents, vector<int> divs, string name) {
//	for(int cent:cents) {
//		for(int div:divs) {
//			vector<double> stat_vals, energy_val, stat_err, energy_err, stat_sys;
//			Measure stat_meas;
//			for(int energy:analysis::energy_list) {
//				energy_val.push_back(plot::energy_match[energy]);
//				energy_err.push_back(0.0);
//				stat_meas = stats[energy][div][cent][stat_names];
//				stat_vals.push_back(stat_meas.get_val());
//				stat_err.push_back(stat_meas.get_err());
//				stat_sys.push_back(sys[energy][div][cent][stat_names]);
//				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
//				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
//				if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
//				if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
//			}
//		}
//	}


	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = {stat_names.size(), stats.size()};
	can->Divide(can_div.first, can_div.second, plot::can_div_x, plot::can_div_y);
	int can_index = 0;
	for(string type:types) {
//	for(pair<string, map<int, map<int, map<int, map<string, Measure>>>>> data_set:stats){
		for(string stat_name:stat_names) {
			can->cd(++can_index);
			double y_max = numeric_limits<double>::min();
			double y_min = numeric_limits<double>::max();
			auto *mg = new TMultiGraph();
			mg->SetNameTitle((type + " " + stat_name).data(), (type + " " + stat_name).data());
			TLegend *leg = new TLegend(0.3, 0.21, 0.3, 0.21);
			for(int cent:cents) {
				for(int div:divs) {
					vector<double> stat_vals, energy_val, stat_err, energy_err, stat_sys;
					Measure stat_meas;
					for(int energy:analysis::energy_list) {
						energy_val.push_back(plot::energy_match[energy]);
						energy_err.push_back(0.0);
						stat_meas = stats[type][energy][div][cent][stat_name];
						stat_vals.push_back(stat_meas.get_val());
						stat_err.push_back(stat_meas.get_err());
						stat_sys.push_back(sys[type][energy][div][cent][stat_name]);
						if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
						if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
						if(stat_meas.get_val() + stat_sys.back() > y_max) { y_max = stat_meas.get_val() + stat_sys.back(); }
						if(stat_meas.get_val() - stat_sys.back() < y_min) { y_min = stat_meas.get_val() - stat_sys.back(); }
					}
					TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
					pair<int, int> range = get_cent9_range(cent);
					graph->SetNameTitle((to_string(range.first)+"-"+to_string(range.second)+"% " + to_string(div) + " degree bins").data());
					graph->SetMarkerStyle(plot::div_marker_styles[div]);
					graph->SetMarkerColor(plot::cent_marker_colors[cent]);
					graph->SetMarkerSize(plot::div_marker_sizes[div]);
					graph->SetLineColor(plot::cent_marker_colors[cent]);
					mg->Add(graph, "APZ");
					TGraphErrors *sys_graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_sys);
					sys_graph->SetLineColor(plot::cent_marker_colors[cent]);
					mg->Add(sys_graph, "[]");
					if(can_index == 1) {
						leg->AddEntry(graph, (to_string(div) + "#circ bins \t" + to_string(range.first)+"-"+to_string(range.second)+"% Centrality").data(), "p");
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
		}
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
//			cout << cent_vec[i] << "\t" << stat1_val[i] << " ± " << stat1_err[i] << "\t" << stat2_val[i]  << " ± " << stat2_err[i] << endl;
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
