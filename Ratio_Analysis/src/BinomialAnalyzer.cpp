/*
 * BinomialAnalyzer.cpp
 *
 *  Created on: Jan 30, 2020
 *      Author: Dylan Neff
 */




#include <vector>

#include <TStyle.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TLine.h>

#include "BinomialAnalyzer.h"

using namespace std;


// Structors

BinomialAnalyzer::BinomialAnalyzer() {
	out_root = NULL;
}

BinomialAnalyzer::~BinomialAnalyzer() {
	out_root->Close();
	delete out_root;
}


// Getters

string BinomialAnalyzer::get_out_root_name() {
	return out_root_name;
}


// Setters

void BinomialAnalyzer::set_out_root_name(string name) {
	out_root_name = name;
}

void BinomialAnalyzer::set_in_path(string in_path) {
	this->in_path = in_path;
}

void BinomialAnalyzer::set_in_mix_path(string in_mix_path) {
	this->in_mix_path = in_mix_path;
}

void BinomialAnalyzer::set_out_path(string out_path) {
	this->out_path = out_path;
}

void BinomialAnalyzer::set_energies(vector<int> energies) {
	energy_list = energies;
}

void BinomialAnalyzer::set_sets(map<string, vector<int>> set_name) {
	this->sets = set_name;
}

// Doers

void BinomialAnalyzer::analyze() {
	out_root = new TFile((out_path+out_root_name).data(), "RECREATE");
	analyze_sets();
}


void BinomialAnalyzer::analyze_sets() {
	for(pair<string, vector<int>> set:sets) {
		analyze_set(set.first, set.second);
	}
}


void BinomialAnalyzer::analyze_set(string set_name, vector<int> set_nums) {
	TDirectory *set_dir = out_root->mkdir((set_name.data()));
	for(int set_num = set_nums[0]; set_num <= set_nums[1]; set_num++) {
		analyze_subset(set_name, set_num, set_dir);
	}
}

void BinomialAnalyzer::analyze_subset(string set_name, int set_num, TDirectory *set_dir) {
	cout << "Starting Set " + set_name + to_string(set_num) << endl << endl;

	string path = in_path + set_name + to_string(set_num) + "/";
	string path_mix = in_mix_path + set_name + to_string(set_num) + "/";

	map<int, map<int, map<int, AzimuthBinData>>> data = get_data(path);
	map<int, map<int, map<int, AzimuthBinData>>> data_mix = get_data(path_mix);

	TDirectory *set_num_dir = set_dir->mkdir((set_name + to_string(set_num)).data());

	TDirectory *slice_dir = set_num_dir->mkdir("Slice Distributions");
	plot_slices(data, slice_dir->mkdir("raw"));
	plot_slices(data_mix, slice_dir->mkdir("mix"));

	map<string, map<int, map<int, map<int,map<int, map<string, Measure>>>>>> slice_stats {{"raw", get_slice_stats(data)}, {"mix", get_slice_stats(data_mix)}};
	plot_all_stats(slice_stats, set_num_dir);

	map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> slice_divided_stats {{"raw", divide_binomial(slice_stats["raw"])}, {"mix", divide_binomial(slice_stats["mix"])}};
	plot_all_divided_stats(slice_divided_stats, set_num_dir);

}


map<int, map<int, map<int, AzimuthBinData>>> BinomialAnalyzer::get_data(string path, int min_num_events) {
	map<int, map<int, map<int, AzimuthBinData>>> data;

	for(int const &energy:energy_list) {

		string energy_path = path + to_string(energy) + "GeV/";

		for(int const &div:divs) {

			// Get centralities found in file names of given directory path.
			for(int const &cent:get_centrals(energy_path, div)) {
				AzimuthBinData az_data(div);
				az_data.read_data_from_dir(energy_path, div, cent);  // Read azimuthal bin data from file path

				if(az_data.get_num_bins() / div <= min_num_events) {
					if(div == divs[0]) {
						cout << "Centrality " << cent << " with only " << az_data.get_num_bins() / div << " events. Skipping." << endl;
					}
				} else {
					data[energy][div][cent] = az_data;  // Store azimuthal bin data in data under corresponding centrality (refmult2) value
				}
			}
		}
	}

	return data;
}


// Calculate stats for each cumulant_order for each centrality for each number of divisions for each energy.
map<int, map<int, map<int, map<int, map<string, Measure>>>>> BinomialAnalyzer::get_slice_stats(map<int, map<int, map<int, AzimuthBinData>>> &data) {
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> stats;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(stat_threads);
		for(auto &energy:data) {
			for(auto &div:energy.second) {
				for(auto &cent:div.second) {
					for(auto &total_proton:cent.second.get_bin_data()) {
						int events = 0;
						for(auto &bin_protons:total_proton.second) { events += bin_protons.second; }
						if(events > min_events) {
							pool.enqueue(&BinomialAnalyzer::calc_stat, this, total_proton.second, total_proton.first, energy.first, div.first, cent.first, &stats);
						}
					}
				}
			}
		}
	}
	return stats;
}


// Divide each statistic by the corresponding statistic for binomial distribution.
map<int, map<int, map<int, map<int, map<string, Measure>>>>> BinomialAnalyzer::divide_binomial(map<int, map<int, map<int, map<int, map<string, Measure>>>>> &slice_stats) {
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> result;
	for(auto &energy:slice_stats) {
		for(auto &div:energy.second) {
			double p = 1.0 / div.first;
			double q = 1.0 - p;
			for(auto &cent:div.second) {
				for(auto &total_proton:cent.second) {
					int n = total_proton.first;
					for(auto &stat:total_proton.second) {
						if(stat.first == "mean") {
							result[energy.first][div.first][cent.first][total_proton.first][stat.first] = stat.second / (n*p);
						} else if(stat.first == "standard_deviation") {
							result[energy.first][div.first][cent.first][total_proton.first][stat.first] = stat.second / pow(n*p*q,0.5);
						} else { // Placeholder for higher orders
							result[energy.first][div.first][cent.first][total_proton.first][stat.first] = stat.second * 0.0;
						}
					}
				}
			}
		}
	}
	return(result);
}


// Calculate statistics for total_proton slice histogram data.
void BinomialAnalyzer::calc_stat(map<int, int> &slice_data, int protons, int energy, int div, int cent, map<int, map<int, map<int, map<int, map<string, Measure>>>>> *stats) {
	Stats stat(slice_data);
//	cout << "Getting energy " << to_string(energy) << " div " << to_string(div) << " cent " << to_string(cent) << " proton " << to_string(protons) << endl;
	(*stats)[energy][div][cent][protons]["mean"] = stat.get_mean();
	(*stats)[energy][div][cent][protons]["standard_deviation"] = stat.get_standard_deviation();
	(*stats)[energy][div][cent][protons]["skewness"] = stat.get_skewness();
	(*stats)[energy][div][cent][protons]["kurtosis"] = stat.get_kurtosis();
	(*stats)[energy][div][cent][protons]["non_excess_kurtosis"] = stat.get_non_excess_kurtosis();
}



// Plotters

void BinomialAnalyzer::draw_proton_bin_plots(map<int, map<int, map<int, AzimuthBinData>>> &data, TDirectory *dir) {
	TDirectory *out_dir = dir->mkdir("Proton_Bin_Plots");

	for(int energy:energy_list) {
		TDirectory *energy_dir = out_dir->mkdir((to_string(energy)+"GeV").data());

		for(int cent:centralities) {
			TDirectory *cent_dir = energy_dir->mkdir((to_string(cent)+"_Cent").data());
			cent_dir->cd();

			for(int div:divs) {
				data[energy][div][cent].canvas_2d_dist("Proton_Bin_Dist " + to_string(energy) + "GeV, " + to_string(cent) + " cent, " + to_string(div) + " div");
			}
		}
	}
}


void BinomialAnalyzer::plot_slices(map<int, map<int, map<int, AzimuthBinData>>> &data, TDirectory *dir) {

	for(auto &div:divs) {
		TDirectory* div_dir = dir->mkdir((to_string(div)+" Divisions").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &energy:energy_list) {
				TDirectory *energy_dir = cent_dir->mkdir((to_string(energy)+"GeV").data());
				energy_dir->cd();
				for(auto total_proton:data[energy][div][cent].get_bin_data()) {
					slice_dist_plot(total_proton.second, total_proton.first, "Slice Dist " + to_string(total_proton.first) + " protons " + to_string(energy) + "GeV " + to_string(cent) + " cent " + to_string(div) + " divs");
				}
			}
		}
	}
}


void BinomialAnalyzer::plot_all_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, TDirectory *dir) {
	TDirectory* stats_dir = dir->mkdir("Stats_vs_Total_Protons");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_dir->mkdir((to_string(div)+" Divisions").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &stat:stats) {
				cout << "all " << div << " div " << cent << " cent " << stat << endl;
				slice_stats_plot(slice_stats, stat, energy_list, cent, div, stat+" "+to_string(div)+" div "+to_string(cent)+" cent");
			}
		}
	}
}


void BinomialAnalyzer::plot_all_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, TDirectory *dir) {
	TDirectory* stats_divided_dir = dir->mkdir("Stats_Divided_vs_Total_Protons");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_divided_dir->mkdir((to_string(div)+" Divisions").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &stat:stats) {
				auto fits = slice_stats_divided_plot(slice_stats, stat, energy_list, cent, div, "Divided "+stat+" "+to_string(div)+" div "+to_string(cent)+" cent");
				plot_fit(fits, cent, div, "Fits "+stat+" "+to_string(div)+" div "+to_string(cent)+" cent");
			}
		}
	}
}

void BinomialAnalyzer::slice_stats_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name) {
	TCanvas can(name.data(), name.data(), canvas_width, canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(energies.size());
	can.Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;

	vector<TMultiGraph*> mgs;
	vector<TGraphErrors*> graphs;
	vector<TLegend*> legends;
	vector<TF1*> binoms;

	for(int energy:energies) {
		can.cd(can_index);
		mgs.push_back(new TMultiGraph());
		mgs.back()->SetNameTitle((to_string(energy) + "GeV").data(), (to_string(energy) + "GeV").data());
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		legends.push_back(new TLegend(0.3, 0.21, 0.3, 0.21));
		if(stat_name == "mean") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "[0]*x", plot_x_range.first, plot_x_range.second)); binoms.back()->SetParameter(0, 1.0/div); }
		else if(stat_name == "standard_deviation") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "TMath::Sqrt([0]*(1-[0])*x)", plot_x_range.first, plot_x_range.second)); binoms.back()->SetParameter(0, 1.0/div); }
		else { binoms.push_back(new TF1()); }
		for(auto &data_set:slice_stats) {
			vector<double> stat_vals, proton_val, stat_err, proton_err;
			Measure stat_meas;

			for(auto &proton_data:data_set.second[energy][div][cent]) {
				stat_meas = proton_data.second[stat_name];
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				proton_val.push_back(proton_data.first);  // Offset sets on x (Energy) axis.
				proton_err.push_back(0.0);
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
//				cout << data_set.first << " energy: " << energy << " proton: " << proton_val.back() << " stat: " << stat_vals.back() << "±" << stat_err.back() << endl;
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			graphs.push_back(new TGraphErrors((int)proton_val.size(), proton_val.data(), stat_vals.data(), proton_err.data(), stat_err.data()));
			graphs.back()->SetNameTitle((data_set.first + " " + to_string(div) + " divisions").data());
			graphs.back()->SetMarkerStyle(raw_mix_marker_style[data_set.first]);
			graphs.back()->SetMarkerColor(raw_mix_marker_color[data_set.first]);
			graphs.back()->SetMarkerSize(raw_mix_marker_size[data_set.first]);
			graphs.back()->SetLineColor(raw_mix_marker_color[data_set.first]);
			mgs.back()->Add(graphs.back(), "APZ");
			if(can_index == 1) {
				legends.back()->SetBorderSize(legend_border_width);
				legends.back()->AddEntry(graphs.back(), (data_set.first + " " + stat_name + " " + to_string(div) + " divs").data(), "p");
			}
		}
		double y_range = y_max - y_min;
		mgs.back()->GetXaxis()->SetLimits(plot_x_range.first, plot_x_range.second);
		mgs.back()->GetXaxis()->SetRangeUser(plot_x_range.first, plot_x_range.second);
		mgs.back()->GetXaxis()->SetLabelSize(0.06);
		mgs.back()->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mgs.back()->GetXaxis()->SetTitle("Total Protons"); mgs.back()->GetXaxis()->SetTitleSize(0.06); mgs.back()->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.03);
		mgs.back()->Draw("AP");
		binoms.back()->Draw("same");
		if(can_index == 1) { legends.back()->SetMargin(0.1); legends.back()->Draw(); }
		can_index++;
	}
	can.Update();
	can.Write(name.data());

	for(auto leg:legends) { delete leg; }
	for(auto graph:graphs) { delete graph; }
	for(auto mg:mgs) { delete mg; }
}


map<string, map<int, TF1*>> BinomialAnalyzer::slice_stats_divided_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_divided_stats, string stat_name, vector<int> &energies, int cent, int div, string name) {
	TCanvas can(name.data(), name.data(), canvas_width, canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(energies.size());
	can.Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;

	vector<TMultiGraph*> mgs;
	vector<TGraphErrors*> graphs;
	vector<TLegend*> legends;
	vector<TLine*> lines;

	map<string, map<int, TF1*>> linear_fits;

	for(int energy:energies) {
		can.cd(can_index);
		mgs.push_back(new TMultiGraph());
		mgs.back()->SetNameTitle((to_string(energy) + "GeV").data(), (to_string(energy) + "GeV").data());
		double y_max = numeric_limits<double>::min();
		double y_min = numeric_limits<double>::max();
		legends.push_back(new TLegend(0.3, 0.21, 0.3, 0.21));
		lines.push_back(new TLine(plot_x_range.first, 1, plot_x_range.second, 1));
		lines.back()->SetLineColor(kBlack);
		for(auto &data_set:slice_divided_stats) {
			vector<double> stat_vals, proton_val, stat_err, proton_err;
			Measure stat_meas;

			for(auto &proton_data:data_set.second[energy][div][cent]) {
				stat_meas = proton_data.second[stat_name];
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				proton_val.push_back(proton_data.first);  // Offset sets on x (Energy) axis.
				proton_err.push_back(0.0);
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			graphs.push_back(new TGraphErrors((int)proton_val.size(), proton_val.data(), stat_vals.data(), proton_err.data(), stat_err.data()));
			graphs.back()->SetNameTitle((data_set.first + " " + to_string(div) + " divisions").data());
			graphs.back()->SetMarkerStyle(raw_mix_marker_style[data_set.first]);
			graphs.back()->SetMarkerColor(raw_mix_marker_color[data_set.first]);
			graphs.back()->SetMarkerSize(raw_mix_marker_size[data_set.first]);
			graphs.back()->SetLineColor(raw_mix_marker_color[data_set.first]);

			linear_fits[data_set.first][energy] = new TF1((data_set.first+to_string(energy)+stat_name).data(), "pol1");
			linear_fits[data_set.first][energy]->SetLineColor(raw_mix_marker_color[data_set.first]-3);
			graphs.back()->Fit(linear_fits[data_set.first][energy], "Q");

			mgs.back()->Add(graphs.back(), "APZ");
			if(can_index == 1) {
				legends.back()->SetBorderSize(legend_border_width);
				legends.back()->AddEntry(graphs.back(), (data_set.first + " " + stat_name + " " + to_string(div) + " divs").data(), "p");
			}
		}
		double y_range = y_max - y_min;
		mgs.back()->GetXaxis()->SetLimits(plot_x_range.first, plot_x_range.second);
		mgs.back()->GetXaxis()->SetRangeUser(plot_x_range.first, plot_x_range.second);
		mgs.back()->GetXaxis()->SetLabelSize(0.06);
		mgs.back()->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mgs.back()->GetYaxis()->SetLimits(0.8, 1.15);
//		mgs.back()->GetYaxis()->SetRangeUser(0.8, 1.15);
		mgs.back()->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mgs.back()->GetXaxis()->SetTitle("Total Protons"); mgs.back()->GetXaxis()->SetTitleSize(0.06); mgs.back()->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.03);
		mgs.back()->Draw("AP");
		lines.back()->Draw("same");
		if(can_index == 1) { legends.back()->SetMargin(0.1); legends.back()->Draw(); }
		can_index++;
	}
	can.Update();
	can.Write(name.data());

	for(auto line:lines) { delete line; }
	for(auto leg:legends) { delete leg; }
	for(auto graph:graphs) { delete graph; }
	for(auto mg:mgs) { delete mg; }

	return(linear_fits);
}


void BinomialAnalyzer::plot_fit(map<string, map<int, TF1*>> &fits, int cent, int div, string name) {
	TCanvas can(name.data(), name.data(), canvas_width, canvas_height);
//	gStyle->SetTitleFontSize(0.09);
//	gStyle->SetTitleOffset(1.2);
	pair<int, int> can_div = get_canvas_div(2);
	can.Divide(can_div.first, can_div.second);

	auto int_mg = new TMultiGraph();
	auto slope_mg = new TMultiGraph();
	int_mg->SetNameTitle("Intercept", "Intercept");
	slope_mg->SetNameTitle("Slope", "Slope");
	double y_max_int = numeric_limits<double>::min();
	double y_min_int = numeric_limits<double>::max();
	double y_max_slope = numeric_limits<double>::min();
	double y_min_slope = numeric_limits<double>::max();

	auto int_leg = new TLegend();
	auto zero_line = new TLine(0, 0, 80, 0);
	zero_line->SetLineColor(kBlack);
	auto one_line = new TLine(0, 1, 80, 1);
	one_line->SetLineColor(kBlack);

	vector<TGraphErrors*> graphs;

	for(auto &data_set:fits) {
		vector<double> int_vals, slope_vals, energy_val, int_err, slope_err, energy_err;

		for(auto &energy:data_set.second) {
			int_vals.push_back(energy.second->GetParameter(0));
			slope_vals.push_back(energy.second->GetParameter(1));
			energy_val.push_back(energy_match[energy.first]);
			int_err.push_back(energy.second->GetParError(0));
			slope_err.push_back(energy.second->GetParError(1));
			energy_err.push_back(0.0);
			if(int_vals.back() + int_err.back() > y_max_int) { y_max_int = int_vals.back() + int_err.back(); }
			if(int_vals.back() - int_err.back() < y_min_int) { y_min_int = int_vals.back() - int_err.back(); }
			if(slope_vals.back() + slope_err.back() > y_max_slope) { y_max_slope = slope_vals.back() + slope_err.back(); }
			if(slope_vals.back() - slope_err.back() < y_min_slope) { y_min_slope = slope_vals.back() - slope_err.back(); }
		}
		graphs.push_back(new TGraphErrors((int)energy_val.size(), energy_val.data(), int_vals.data(), energy_err.data(), int_err.data()));
		graphs.back()->SetNameTitle((data_set.first + " intercept").data());
		graphs.back()->SetMarkerStyle(raw_mix_marker_style[data_set.first]);
		graphs.back()->SetMarkerColor(raw_mix_marker_color[data_set.first]);
		graphs.back()->SetMarkerSize(1.5);
		graphs.back()->SetLineColor(raw_mix_marker_color[data_set.first]);
		int_mg->Add(graphs.back(), "APZ");
		int_leg->AddEntry(graphs.back(), data_set.first.data(), "p");

		graphs.push_back(new TGraphErrors((int)energy_val.size(), energy_val.data(), slope_vals.data(), energy_err.data(), slope_err.data()));
		graphs.back()->SetNameTitle((data_set.first + " slope").data());
		graphs.back()->SetMarkerStyle(raw_mix_marker_style[data_set.first]);
		graphs.back()->SetMarkerColor(raw_mix_marker_color[data_set.first]);
		graphs.back()->SetMarkerSize(1.5);
		graphs.back()->SetLineColor(raw_mix_marker_color[data_set.first]);
		slope_mg->Add(graphs.back(), "APZ");
	}
	double y_range_int = y_max_int - y_min_int;
	int_mg->GetXaxis()->SetLimits(0, 80);
	int_mg->GetXaxis()->SetRangeUser(0, 80);
//	int_mg->GetXaxis()->SetLabelSize(0.06);
	int_mg->GetYaxis()->SetLimits(y_min_int - 0.1 * y_range_int, y_max_int + 0.1 * y_range_int);
	int_mg->GetYaxis()->SetRangeUser(y_min_int - 0.1 * y_range_int, y_max_int + 0.1 * y_range_int);
//	int_mg->GetYaxis()->SetLabelSize(0.06);
	int_mg->GetXaxis()->SetTitle("Energy (GeV)");

	double y_range_slope = y_max_slope - y_min_slope;
	slope_mg->GetXaxis()->SetLimits(0, 80);
	slope_mg->GetXaxis()->SetRangeUser(0, 80);
//	slope_mg->GetXaxis()->SetLabelSize(0.06);
	slope_mg->GetYaxis()->SetLimits(y_min_slope - 0.1 * y_range_slope, y_max_slope + 0.1 * y_range_slope);
	slope_mg->GetYaxis()->SetRangeUser(y_min_slope - 0.1 * y_range_slope, y_max_slope + 0.1 * y_range_slope);
//	slope_mg->GetYaxis()->SetLabelSize(0.06);
	slope_mg->GetXaxis()->SetTitle("Energy (GeV)");

	can.cd(1);
	int_mg->Draw("AP");
	one_line->Draw("same");
//	int_leg->SetMargin(0.1);
	int_leg->Draw();

	can.cd(2);
	slope_mg->Draw("AP");
	zero_line->Draw("same");

	can.Update();
	can.Write(name.data());

	for(auto graph:graphs) { delete graph; }
	delete int_leg;
	delete zero_line;
	delete one_line;
	delete slope_mg;
	delete int_mg;
}


void BinomialAnalyzer::slice_dist_plot(map<int, int> &slice_data, int total_protons, string name) {
	TCanvas can((name + " canvas").data(), (name + " canvas").data(), canvas_width, canvas_height);
	TH1I dist((name + " hist").data(), (name + " canvas").data(), total_protons+1, -0.5, total_protons+0.5);
	for(auto &bin_protons:slice_data) {
		dist.Fill(bin_protons.first, bin_protons.second);
	}
	dist.Draw("HIST");
	can.Update();
	can.Write(name.data());
}

pair<int, int> BinomialAnalyzer::get_canvas_div(int plots) {
	pair<int, int> divs;
	divs.first = ceil(pow(plots, 0.5));
	divs.second = ceil((float)plots / divs.first);

	return(divs);
}
