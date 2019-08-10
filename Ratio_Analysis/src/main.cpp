/*
 * main.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <thread>

#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>

#include "config_analysis.h"
#include "plotting.h"
#include "ratio_methods.h"
#include "file_io.h"
#include "Stats.h"
#include "Measure.h"
#include "ThreadPool.h"
#include "RatioData.h"

using namespace std;


void analyze();
void comp_proton_dists();
void cumulant_test();
pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> calculate_stats(map<int, map<int, map<int, RatioData>>> data); //cumulants[energy][divisions][centrality][cumulant_order]
void calc_stat(RatioData *data, int energy, int div, int cent, map<int, map<int, map<int, map<string, Measure>>>> *stats, map<int, map<int, map<int, map<int, Measure>>>> *cumulants);


int main() {
//	analyze();
	comp_proton_dists();

	cout << "donzo" << endl;
	return(0);
}


void cumulant_test() {
	int n = 4;
	vector<double> test_data = {-1,0,1,2,3,4,5,6,7,8,9,10,10};
	cout << get_raw_moment(test_data, n) << endl;
	cout << get_central_moment(test_data, n) << endl;
	double val, err;
	tie(val, err) = get_cumulant(test_data, n);
	cout << val << "+-" << err << endl;
}


void analyze() {
	TFile *out_root = new TFile((plot::out_path+plot::out_root_name).data(), "RECREATE");

	map<int, map<int, map<int, RatioData>>> data;

	int energy_num = 1;
	int num_energies = analysis::energy_list.size();
	for(int energy:analysis::energy_list) {
		cout << "Working on " << energy << "GeV. " << energy_num << " of " << num_energies << endl;
		string path = analysis::in_path + to_string(energy) + "GeV/";
		for(int div:analysis::divs) {
			for(int cent:analysis::centrals) {
				RatioData ratios(div);
				ratios.read_ratios_from_dir(path, div, cent);
				data[energy][div][cent] = ratios;
			}
		}
		energy_num++;
	}

	cout << endl << "Calculating Cumulants..." << endl;
	auto stats = calculate_stats(data);

	out_root->cd();
	cout << endl << "Making ratio distribution plots..." << endl;
	make_ratio_dist_plots(out_root, data);
	cout << endl << "Making 2d distribution plots..." << endl;
	make_2d_dist_plots(out_root, data);
	cout << endl << "Making proton distribution plots..." << endl;
	make_proton_dist_plots(out_root, data);
	cout << endl << "Making cumulant plots..." << endl;
	make_cumulant_plots(out_root, stats.second);
	cout << endl << "Making stat plots..." << endl;
	make_stat_plots(out_root, stats.first);
	cout << endl << "Making canvases..." << endl;
	make_canvas_plots(out_root, data, stats.second, stats.first);


	out_root->Close();
}


//Calculate stats for each cumulant_order for each centrality for each number of divisions for each energy.
pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> calculate_stats(map<int, map<int, map<int, RatioData>>> data) {
	map<int, map<int, map<int, map<string, Measure>>>> stats;
	map<int, map<int, map<int, map<int, Measure>>>> cumulants;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(int energy:analysis::energy_list) {
			for(int div:analysis::divs) {
				for(int cent:analysis::centrals) {
					pool.enqueue(calc_stat, &(data[energy][div][cent]), energy, div, cent, &stats, &cumulants);
				}
			}
		}
	}

	return(make_pair(stats, cumulants));
}


void calc_stat(RatioData *data, int energy, int div, int cent, map<int, map<int, map<int, map<string, Measure>>>> *stats, map<int, map<int, map<int, map<int, Measure>>>> *cumulants) {
	cout << "Starting " << energy << "GeV \t\t" << div << " divisions \t\t" << cent << " centrality" << endl;
	Stats stat(data->get_ratio_hist());
	(*stats)[energy][div][cent]["mean"] = stat.get_mean();
	(*stats)[energy][div][cent]["standard_deviation"] = stat.get_standard_deviation();
	(*stats)[energy][div][cent]["skewness"] = stat.get_skewness();
	(*stats)[energy][div][cent]["kurtosis"] = stat.get_kurtosis();
	for(int order:analysis::cumulant_orders) {
		(*cumulants)[energy][div][cent][order] = stat.get_cumulant(order);
	}
	cout << "Finishing " << energy << "GeV \t" << div << " divisions \t\t" << cent << " centrality" << endl;
}


void comp_proton_dists() {
	TFile *out_root = new TFile("/home/dylan/local_server/dyn0402/Research/Results/08-09-19_proton_dist_comp.root", "RECREATE");

	string in_path_dylan = "/home/dylan/local_server/dyn0402/Research/Data/";
	string in_path_roli = "/home/dylan/local_server/dyn0402/Research/Data_Roli/";

	vector<int> energy_list = {7, 11, 19, 27};
	vector<int> divs = {2};
	vector<int> centrals = {15};

	map<int, map<int, map<int, RatioData>>> data_dylan;
	map<int, map<int, map<int, RatioData>>> data_roli;

	int energy_num = 1;
	int num_energies = energy_list.size();
	for(int energy:energy_list) {
		cout << "Working on " << energy << "GeV. " << energy_num << " of " << num_energies << endl;
		string path_dylan = in_path_dylan + to_string(energy) + "GeV/";
		string path_roli = in_path_roli + to_string(energy) + "GeV/";
		for(int div:divs) {
			for(int cent:centrals) {
				string name = to_string(energy) + "GeV " + to_string(cent) + " Centrality";

				RatioData ratios_dylan(div);
				ratios_dylan.read_ratios_from_dir(path_dylan, div, cent);
				data_dylan[energy][div][cent] = ratios_dylan;
				map<int, int> dylan_dist = ratios_dylan.get_proton_dist();
				TH1D dylan_hist((name+"_dylan").data(), (name+"_dylan").data(), 51, -0.5, 50.5);
				TH1D dylan_hist_norm((name+"_dylan_norm").data(), (name+"_dylan_norm").data(), 51, -0.5, 50.5);
				for(pair<int, int> proton:dylan_dist) {
					for(int i=0; i<proton.second; i++) {
						dylan_hist.Fill(proton.first);
					}
				}
				for(pair<int, int> proton:dylan_dist) {
					dylan_hist_norm.SetBinContent(dylan_hist_norm.GetBin(proton.first), ((double)proton.second) / dylan_hist.GetEntries());
				}
				dylan_hist.SetLineColor(kRed);
				dylan_hist_norm.SetLineColor(kRed);

				RatioData ratios_roli(div);
				ratios_roli.read_ratios_from_dir(path_roli, div, cent);
				data_roli[energy][div][cent] = ratios_roli;
				map<int, int> roli_dist = ratios_roli.get_proton_dist();
				TH1D roli_hist((name+"_roli").data(), (name+"_roli").data(), 51, -0.5, 50.5);
				TH1D roli_hist_norm((name+"_roli_norm").data(), (name+"_roli_norm").data(), 51, -0.5, 50.5);
				for(pair<int, int> proton:roli_dist) {
					for(int i=0; i<proton.second; i++) {
						roli_hist.Fill(proton.first);
					}
				}
				for(pair<int, int> proton:roli_dist) {
					roli_hist_norm.SetBinContent(roli_hist_norm.GetBin(proton.first), ((double)proton.second) / roli_hist.GetEntries());
				}
				roli_hist.SetLineColor(kBlue);
				roli_hist_norm.SetLineColor(kBlue);

				TCanvas can(name.data());
				dylan_hist.Draw();
				roli_hist.Draw("sames");
				can.BuildLegend();


				TCanvas can_norm((name+"_norm").data());
//				dylan_hist.DrawNormalized();
//				roli_hist.DrawNormalized("sames");
				dylan_hist_norm.Draw();
				roli_hist_norm.Draw("sames");
				can_norm.BuildLegend();

				out_root->cd();
				can.Write();
				can_norm.Write();

			}
		}
		energy_num++;
	}
	out_root->Close();
}
