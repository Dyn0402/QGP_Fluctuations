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
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>

#include "ratio_methods.h"
#include "plotting.h"
#include "config_analysis.h"
#include "Stats.h"

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


void make_cumulant_plots(TFile *out_root, map<int, map<int, map<int, map<int, measure<double>>>>> cumulants) {
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



void graph_cumulant_vs_energy(map<int, map<int, map<int, map<int, measure<double>>>>> cumulants, int div, int cent, int order) {
	vector<double> cumulant, cumulant_err, energies, energies_err;
	measure<double> cum;
	for(int energy:analysis::energy_list) {
		cum = cumulants[energy][div][cent][order];
		cumulant.push_back(cum.val);
		cumulant_err.push_back(cum.err);
		energies.push_back((double)energy);
		energies_err.push_back(0.0);
	}
	string name = to_string(div) + " division " + to_string(cent) + " centrality " + to_string(order) + " order";
	TGraphErrors *graph = graph_x_vs_y_err(energies, cumulant, energies_err, cumulant_err);
	graph->Write(name.data());
}


void graph_cumulant_vs_divs(map<int, map<int, map<int, map<int, measure<double>>>>> cumulants, int energy, int cent, int order) {
	vector<double> cumulant, cumulant_err, divisions, divisions_err;
	measure<double> cum;
	for(int div:analysis::divs) {
		cum = cumulants[energy][div][cent][order];
		cumulant.push_back(cum.val);
		cumulant_err.push_back(cum.err);
		divisions.push_back((double)div);
		divisions_err.push_back(0.0);
	}
	string name = to_string(energy) + " energy " + to_string(cent) + " centrality " + to_string(order) + " order";
	TGraphErrors *graph = graph_x_vs_y_err(divisions, cumulant, divisions_err, cumulant_err);
	graph->Write(name.data());
}



void make_stat_plots(TFile *out_root, map<int, map<int, map<int, map<string, measure<double>>>>> stats) {
	TDirectory *stats_dir = out_root->mkdir(plot::stat_dir_name.data());
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



void graph_stat_vs_energy(map<int, map<int, map<int, map<string, measure<double>>>>> stats, int div, int cent, string stat) {
	vector<double> stat_val, stat_err, energies, energies_err;
	measure<double> stat_meas;
	for(int energy:analysis::energy_list) {
		stat_meas = stats[energy][div][cent][stat];
		stat_val.push_back(stat_meas.val);
		stat_err.push_back(stat_meas.err);
		energies.push_back((double)energy);
		energies_err.push_back(0.0);
	}
	string name = to_string(div) + " division " + to_string(cent) + " centrality " + stat;
	TGraphErrors *graph = graph_x_vs_y_err(energies, stat_val, energies_err, stat_err);
	graph->Write(name.data());
}


void graph_stat_vs_divs(map<int, map<int, map<int, map<string, measure<double>>>>> stats, int energy, int cent, string stat) {
	vector<double> stat_val, stat_err, divisions, divisions_err;
	measure<double> stat_meas;
	for(int div:analysis::divs) {
		stat_meas = stats[energy][div][cent][stat];
		stat_val.push_back(stat_meas.val);
		stat_err.push_back(stat_meas.err);
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



void make_canvas_plots(TFile *out_root, map<int, tree_data> data, map<int, map<int, map<int, map<int, measure<double>>>>> cumulants, map<int, map<int, map<int, map<string, measure<double>>>>> stats) {
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

	// Stat canvases
	TDirectory *stat_can_dir = can_dir->mkdir(plot::stat_dir_name.data());
	stat_can_dir->cd();
	for(string stat:analysis::stat_names) {
		TDirectory *stat_dir = stat_can_dir->mkdir(stat.data());
		stat_dir->cd();
		for(int cent:analysis::centrals) {
			string name = stat + " Centrality " + to_string(cent);
			canvas_stat_dists(stats, stat, cent, name);
		}
	}

	// Athic canvases
	can_dir->cd();
	athic_stat_vs_energy(stats, "ATHIC Stat vs Energy");
	athic_stat_vs_centrality(stats, "ATHIC Stat vs Centrality");
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


void canvas_cumulant_dists(map<int, map<int, map<int, map<int, measure<double>>>>> cumulants, int order, int cent, string name) {
	auto *can = new TCanvas();
	auto *mg = new TMultiGraph();
	mg->SetNameTitle(name.data(), name.data());
	for(int div:analysis::divs) {
		vector<double> cumulant, energy, cumulant_err, energy_err;
		measure<double> cum;
		for(int e:analysis::energy_list) {
			energy.push_back((double)e);
			energy_err.push_back(0.0);
			cum = cumulants[e][div][cent][order];
			cumulant.push_back(cum.val);
			cumulant.push_back(cum.err);
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


void canvas_stat_dists(map<int, map<int, map<int, map<string, measure<double>>>>> stats, string stat, int cent, string name) {
	auto *can = new TCanvas();
	auto *mg = new TMultiGraph();
	mg->SetNameTitle(name.data(), name.data());
	for(int div:analysis::divs) {
		vector<double> stat_vals, energy, stat_err, energy_err;
		measure<double> stat_meas;
		for(int e:analysis::energy_list) {
			energy.push_back((double)e);
			energy_err.push_back(0.0);
			stat_meas = stats[e][div][cent][stat];
			stat_vals.push_back(stat_meas.val);
			stat_vals.push_back(stat_meas.err);
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


void athic_stat_vs_centrality(map<int, map<int, map<int, map<string, measure<double>>>>> stats, string name) {
	int div = 6; //Hardcode, fix.
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	can->Divide(2,2);
	int can_index = 1;
	for(string stat:analysis::stat_names) {
		can->cd(can_index);
		auto *mg = new TMultiGraph();
		mg->SetNameTitle(stat.data(), stat.data());
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		for(int energy:analysis::energy_list) {
			vector<double> stat_vals, cent_val, stat_err, cent_err;
			measure<double> stat_meas;
			for(int cent_index=15; cent_index > 5; cent_index--) {
				cent_val.push_back((15-cent_index)*plot::centrality_slope + plot::centrality_intercept);
				cent_err.push_back(0.0);
				stat_meas = stats[energy][div][cent_index][stat];
				stat_vals.push_back(stat_meas.val);
				stat_err.push_back(stat_meas.err);
				if(stat_meas.val + stat_meas.err > y_max) { y_max = stat_meas.val + stat_meas.err; }
				if(stat_meas.val - stat_meas.err < y_min) { y_min = stat_meas.val - stat_meas.err; }
			}
			TGraphErrors *graph = graph_x_vs_y_err(cent_val, stat_vals, cent_err, stat_err);
			graph->SetNameTitle((to_string(plot::energy_match[energy]) + "GeV").data());
			graph->SetMarkerStyle(plot::energy_marker_styles[energy]);
			graph->SetMarkerColor(plot::energy_marker_colors[energy]);
			graph->SetMarkerSize(plot::energy_marker_sizes[energy]);
			mg->Add(graph, "AP");
			double x_min = *min_element(cent_val.begin(), cent_val.end());
			double x_max = *max_element(cent_val.begin(), cent_val.end());
			double x_range = x_max - x_min;
			double y_range = y_max - y_min;
			mg->GetXaxis()->SetLimits(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
			mg->GetXaxis()->SetRangeUser(x_min - 0.1 * x_range, x_max + 0.1 * x_range);
			mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetXaxis()->SetTitle("Centrality (%)");
		}
		mg->Draw("AP"); // Multigraph memory leak, fix.
		can_index++;
	}
	can->Write(name.data());
	delete can;
}


void athic_stat_vs_energy(map<int, map<int, map<int, map<string, measure<double>>>>> stats, string name) {
	int div = 6; //Hardcode, fix.
	auto *can = new TCanvas(name.data(), name.data(), plot::canvas_width, plot::canvas_height);
	can->Divide(2,2);
	int can_index = 1;
	for(string stat:analysis::stat_names) {
		can->cd(can_index);
		gPad->SetLogx();
		auto *mg = new TMultiGraph();
		mg->SetNameTitle(stat.data(), stat.data());
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		for(int cent_index=15; cent_index > 10; cent_index-=2) {
			vector<double> stat_vals, energy_val, stat_err, energy_err;
			measure<double> stat_meas;
			for(int energy:analysis::energy_list) {
				energy_val.push_back(plot::energy_match[energy]);
				energy_err.push_back(0.0);
				stat_meas = stats[energy][div][cent_index][stat];
				stat_vals.push_back(stat_meas.val);
				stat_err.push_back(stat_meas.err);
				if(stat_meas.val + stat_meas.err > y_max) { y_max = stat_meas.val + stat_meas.err; }
				if(stat_meas.val - stat_meas.err < y_min) { y_min = stat_meas.val - stat_meas.err; }
			}
			TGraphErrors *graph = graph_x_vs_y_err(energy_val, stat_vals, energy_err, stat_err);
			graph->SetNameTitle((to_string((15-cent_index)*plot::centrality_slope + plot::centrality_intercept) + " %").data());
			graph->SetMarkerStyle(plot::cent_marker_styles[cent_index]);
			graph->SetMarkerColor(plot::cent_marker_colors[cent_index]);
			graph->SetMarkerSize(plot::cent_marker_sizes[cent_index]);
			mg->Add(graph, "AP");
			double x_min = *min_element(energy_val.begin(), energy_val.end());
			double x_max = *max_element(energy_val.begin(), energy_val.end());
			double y_range = y_max - y_min;
			mg->GetXaxis()->SetLimits(pow(x_min, 1.1) / pow(x_max, 0.1), pow(x_max, 1.1) / pow(x_min, 0.1));
			mg->GetXaxis()->SetRangeUser(pow(x_min, 1.1) / pow(x_max, 0.1), pow(x_max, 1.1) / pow(x_min, 0.1));
			mg->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
			mg->GetXaxis()->SetTitle("Energy (GeV)");
		}
		mg->Draw("AP"); // Multigraph memory leak, fix.
		can_index++;
	}
	can->Write(name.data());
	delete can;
}

