/*
 * main.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <fstream>
#include <sstream>
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
void analyze_CBWC();
void comp_proton_dists();
void cumulant_test();
pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> calculate_stats(map<int, map<int, map<int, RatioData>>> data); //cumulants[energy][divisions][centrality][cumulant_order]
void calc_stat(RatioData *data, int energy, int div, int cent, map<int, map<int, map<int, map<string, Measure>>>> *stats, map<int, map<int, map<int, map<int, Measure>>>> *cumulants);
pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> calc_cbwc_stats(map<int, map<int, map<int, RatioData>>> data, pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> stats, int cent_type = 16);
void comp_moments();
map<int, map<string, Measure>> get_roli_moments(string path);
int get_centrality(double refmult2, int energy);
int get_centrality9(double refmult2, int energy);


int main() {
//	analyze();
//	comp_proton_dists();
	comp_moments();
//	analyze_CBWC();

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



void analyze_CBWC() {
	TFile *out_root = new TFile((plot::out_path+plot::out_root_name).data(), "RECREATE");

	map<int, map<int, map<int, RatioData>>> data;

	int energy_num = 1;
	int num_energies = analysis::energy_list.size();
	for(int energy:analysis::energy_list) {
		cout << "Working on " << energy << "GeV. " << energy_num << " of " << num_energies << endl;
		string path = analysis::in_path + to_string(energy) + "GeV/";
		for(int div:analysis::divs) {
			analysis::centrals = get_centrals(path, div);
			for(int cent:analysis::centrals) {
				RatioData ratios(div);
				ratios.read_ratios_from_dir(path, div, cent);
				data[energy][div][cent] = ratios;
			}
		}
		energy_num++;
	}

	cout << endl << "Calculating Cumulants..." << endl;
	auto raw_stats = calculate_stats(data);
	auto stats = calc_cbwc_stats(data, raw_stats);

	out_root->cd();
	cout << endl << "Making ratio distribution plots..." << endl;
	make_ratio_dist_plots(out_root, data);
//	cout << endl << "Making 2d distribution plots..." << endl;
//	make_2d_dist_plots(out_root, data);
//	cout << endl << "Making proton distribution plots..." << endl;
//	make_proton_dist_plots(out_root, data);
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
		ThreadPool pool(1);//thread::hardware_concurrency());
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
	TFile *out_root = new TFile("/home/dylan/local_server/dyn0402/Research/Results/09-25-19_proton_dist_comp_roli_dylan_roli.root", "RECREATE");

	string in_path_dylan = "/home/dylan/local_server/dyn0402/Research/Data/";
	string in_path_roli = "/home/dylan/local_server/dyn0402/Research/Data_Roli_Self_Gen/";

	vector<int> energy_list = {27};
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



pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> calc_cbwc_stats(map<int, map<int, map<int, RatioData>>> data, pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> stats, int cent_type) {
	pair<map<int, map<int, map<int, map<string, Measure>>>>, map<int, map<int, map<int, map<int, Measure>>>>> binned_stats;
	for(int energy:analysis::energy_list) {
		for(int div:analysis::divs) {
			map<int, int> bin_ratios;
			for(int cent:analysis::centrals) {
				int cent_ratios = data[energy][div][cent].get_num_ratios();
				int cent_bin;
				if(cent_type == 9) {
					cent_bin = get_centrality9(cent, energy);
				}
				else{
					cent_bin = get_centrality(cent, energy);
				}
				bin_ratios[cent_bin] += cent_ratios;
				for(string stat:analysis::stat_names) {
					binned_stats.first[energy][div][cent_bin][stat] = binned_stats.first[energy][div][cent_bin][stat] + stats.first[energy][div][cent][stat] * cent_ratios;
				}
				for(int order:analysis::cumulant_orders) {
					binned_stats.second[energy][div][cent_bin][order] = binned_stats.second[energy][div][cent_bin][order] + stats.second[energy][div][cent][order] * cent_ratios;
				}
			}
			for(pair<int, map<string, Measure>> cent_bin:binned_stats.first[energy][div]) {
				for(string stat:analysis::stat_names) {
					binned_stats.first[energy][div][cent_bin.first][stat] = binned_stats.first[energy][div][cent_bin.first][stat] / bin_ratios[cent_bin.first];
				}
			}
			for(pair<int, map<int, Measure>> cent_bin:binned_stats.second[energy][div]) {
				for(int order:analysis::cumulant_orders) {
					binned_stats.second[energy][div][cent_bin.first][order] = binned_stats.second[energy][div][cent_bin.first][order] / bin_ratios[cent_bin.first];
				}
			}
		}
	}

	return(binned_stats);
}


void comp_moments() {
	TFile *out_root = new TFile((plot::out_path+plot::out_root_name).data(), "RECREATE");

	map<int, map<int, map<int, RatioData>>> data;

	int energy_num = 1;
	int num_energies = analysis::energy_list.size();
	for(int energy:analysis::energy_list) {
		cout << "Working on " << energy << "GeV. " << energy_num << " of " << num_energies << endl;
		string path = analysis::in_path + to_string(energy) + "GeV/";
		for(int div:analysis::divs) {
			analysis::centrals = get_centrals(path, div);
			for(int cent:analysis::centrals) {
				RatioData ratios(div);
				ratios.read_ratios_from_dir(path, div, cent);
				data[energy][div][cent] = ratios;
			}
		}
		energy_num++;
	}

	cout << endl << "Calculating Cumulants..." << endl;
	auto raw_stats = calculate_stats(data);
	auto stats = calc_cbwc_stats(data, raw_stats, 9);


	auto roli_moments = get_roli_moments("/home/dylan/local_server/dyn0402/Research/Results/Moments_Tests/data27_fixed_CBWC.txt");

	cout << endl << "Making moment comparison..." << endl;
	out_root->cd();
	make_comp_stat_plot(stats.first[27][2], roli_moments);

	for()
	get_roli_ratio_dist();

	out_root->cd();
	cout << endl << "Making ratio distribution plots..." << endl;
	make_ratio_dist_plots(out_root, data);
//	cout << endl << "Making 2d distribution plots..." << endl;
//	make_2d_dist_plots(out_root, data);
//	cout << endl << "Making proton distribution plots..." << endl;
//	make_proton_dist_plots(out_root, data);
	cout << endl << "Making cumulant plots..." << endl;
	make_cumulant_plots(out_root, stats.second);
	cout << endl << "Making stat plots..." << endl;
	make_stat_plots(out_root, stats.first);
	cout << endl << "Making canvases..." << endl;
	make_canvas_plots(out_root, data, stats.second, stats.first);


	out_root->Close();
}



// Get Roli moments
map<int, map<string, Measure>> get_roli_moments(string path) {
	map<int, map<string, Measure>> moments;
	ifstream file(path);
	string line;
//	getline(file, line);
	while(getline(file, line)) {
		istringstream stream(line);
		int i, cent;
		map<string, vector<double>> m = {{"mean", {0,0}},  {"standard_deviation", {0,0}}, {"skewness", {0,0}}, {"kurtosis", {0,0}}};

		// Read line
		if(!(stream >> i >> cent >> m["mean"][0] >> m["mean"][1] >> m["standard_deviation"][0] >> m["standard_deviation"][1] >> m["skewness"][0] >> m["skewness"][1] >> m["kurtosis"][0] >> m["kurtosis"][1])) {
			continue;
		}

		// Fill map
		for(string stat:analysis::stat_names) {
			Measure meas = Measure(m[stat][0], m[stat][1]);
			moments[cent][stat] = meas;
		}

	}
	return(moments);
}



TH1F* get_roli_ratio_dist(string path) {
	TFile *roli_ratio_file = new TFile(path.data(), "READ");
	TH1F *roli_ratio_hist = (TH1F*) roli_ratio_file->Get("hratio_1_1");
}


//Taken directly from Roli.
//Given energy and refmult2, will return centrality of event.
int get_centrality(double refmult2, int energy){

    int cent = -1;

    if(energy == 7){

        if(refmult2 >= 165) cent = 15;
        else if(refmult2 >= 137 && refmult2 < 165) cent = 14;
        else if(refmult2 >= 114 && refmult2 < 137) cent = 13;
        else if(refmult2 >= 95 && refmult2 < 114) cent = 12;
        else if(refmult2 >= 78 && refmult2 < 95) cent = 11;
        else if(refmult2 >= 64 && refmult2 < 78) cent = 10;
        else if(refmult2 >= 51 && refmult2 < 64) cent = 9;
        else if(refmult2 >= 41 && refmult2 < 51) cent = 8;
        else if(refmult2 >= 32 && refmult2 < 41) cent = 7;
        else if(refmult2 >= 25 && refmult2 < 32) cent = 6;
        else if(refmult2 >= 19 && refmult2 < 25) cent = 5;
        else if(refmult2 >= 14 && refmult2 < 19) cent = 4;
        else if(refmult2 >= 10 && refmult2 < 14) cent = 3;
        else if(refmult2 >= 7 && refmult2 < 10) cent = 2;
        else if(refmult2 >= 5 && refmult2 < 7) cent = 1;
        else if(refmult2 >= 3 && refmult2 < 5) cent = 0;

    }

    else if(energy == 11){

        if(refmult2 >= 206) cent = 15;
        else if(refmult2 >= 172 && refmult2 < 206) cent = 14;
        else if(refmult2 >= 143 && refmult2 < 172) cent = 13;
        else if(refmult2 >= 118 && refmult2 < 143) cent = 12;
        else if(refmult2 >= 98 && refmult2 < 118) cent = 11;
        else if(refmult2 >= 80 && refmult2 < 98) cent = 10;
        else if(refmult2 >= 65 && refmult2 < 80) cent = 9;
        else if(refmult2 >= 52 && refmult2 < 65) cent = 8;
        else if(refmult2 >= 41 && refmult2 < 52) cent = 7;
        else if(refmult2 >= 32 && refmult2 < 41) cent = 6;
        else if(refmult2 >= 24 && refmult2 < 32) cent = 5;
        else if(refmult2 >= 18 && refmult2 < 24) cent = 4;
        else if(refmult2 >= 13 && refmult2 < 18) cent = 3;
        else if(refmult2 >= 9 && refmult2 < 13) cent = 2;
        else if(refmult2 >= 6 && refmult2 < 9) cent = 1;
        else if(refmult2 >= 4 && refmult2 < 6) cent = 0;

    }

    else if(energy == 14){ ///// we take refmult here

        if(refmult2 >= 239) cent = 15;
        else if(refmult2 >= 200 && refmult2 < 239) cent = 14;
        else if(refmult2 >= 167 && refmult2 < 200) cent = 13;
        else if(refmult2 >= 139 && refmult2 < 167) cent = 12;
        else if(refmult2 >= 115 && refmult2 < 139) cent = 11;
        else if(refmult2 >= 94 && refmult2 < 115) cent = 10;
        else if(refmult2 >= 76 && refmult2 < 94) cent = 9;
        else if(refmult2 >= 61 && refmult2 < 76) cent = 8;
        else if(refmult2 >= 48 && refmult2 < 61) cent = 7;
        else if(refmult2 >= 37 && refmult2 < 48) cent = 6;
        else if(refmult2 >= 28 && refmult2 < 37) cent = 5;
        else if(refmult2 >= 21 && refmult2 < 28) cent = 4;
        else if(refmult2 >= 16 && refmult2 < 21) cent = 3;
        else if(refmult2 >= 11 && refmult2 < 16) cent = 2;
        else if(refmult2 >= 8 && refmult2 < 11) cent = 1;
        else if(refmult2 >= 5 && refmult2 < 8) cent = 0;

    }

    else if(energy == 19){

        if(refmult2 >= 258) cent = 15;
        else if(refmult2 >= 215 && refmult2 < 258) cent = 14;
        else if(refmult2 >= 180 && refmult2 < 215) cent = 13;
        else if(refmult2 >= 149 && refmult2 < 180) cent = 12;
        else if(refmult2 >= 123 && refmult2 < 149) cent = 11;
        else if(refmult2 >= 100 && refmult2 < 123) cent = 10;
        else if(refmult2 >= 81 && refmult2 < 100) cent = 9;
        else if(refmult2 >= 65 && refmult2 < 81) cent = 8;
        else if(refmult2 >= 51 && refmult2 < 65) cent = 7;
        else if(refmult2 >= 40 && refmult2 < 51) cent = 6;
        else if(refmult2 >= 30 && refmult2 < 40) cent = 5;
        else if(refmult2 >= 22 && refmult2 < 30) cent = 4;
        else if(refmult2 >= 16 && refmult2 < 22) cent = 3;
        else if(refmult2 >= 12 && refmult2 < 16) cent = 2;
        else if(refmult2 >= 8 && refmult2 < 12) cent = 1;
        else if(refmult2 >= 5 && refmult2 < 8) cent = 0;

    }

    else if(energy == 27){

        if(refmult2 >= 284) cent = 15;
        else if(refmult2 >= 237 && refmult2 < 284) cent = 14;
        else if(refmult2 >= 198 && refmult2 < 237) cent = 13;
        else if(refmult2 >= 164 && refmult2 < 198) cent = 12;
        else if(refmult2 >= 135 && refmult2 < 164) cent = 11;
        else if(refmult2 >= 111 && refmult2 < 135) cent = 10;
        else if(refmult2 >= 90 && refmult2 < 111) cent = 9;
        else if(refmult2 >= 71 && refmult2 < 90) cent = 8;
        else if(refmult2 >= 56 && refmult2 < 71) cent = 7;
        else if(refmult2 >= 43 && refmult2 < 56) cent = 6;
        else if(refmult2 >= 33 && refmult2 < 43) cent = 5;
        else if(refmult2 >= 25 && refmult2 < 33) cent = 4;
        else if(refmult2 >= 18 && refmult2 < 25) cent = 3;
        else if(refmult2 >= 13 && refmult2 < 18) cent = 2;
        else if(refmult2 >= 9 && refmult2 < 13) cent = 1;
        else if(refmult2 >= 6 && refmult2 < 9) cent = 0;

    }

    else if(energy == 39){

        if(refmult2 >= 307) cent = 15;
        else if(refmult2 >= 257 && refmult2 < 307) cent = 14;
        else if(refmult2 >= 215 && refmult2 < 257) cent = 13;
        else if(refmult2 >= 179 && refmult2 < 215) cent = 12;
        else if(refmult2 >= 147 && refmult2 < 179) cent = 11;
        else if(refmult2 >= 121 && refmult2 < 147) cent = 10;
        else if(refmult2 >= 97 && refmult2 < 121) cent = 9;
        else if(refmult2 >= 78 && refmult2 < 97) cent = 8;
        else if(refmult2 >= 61 && refmult2 < 78) cent = 7;
        else if(refmult2 >= 47 && refmult2 < 61) cent = 6;
        else if(refmult2 >= 36 && refmult2 < 47) cent = 5;
        else if(refmult2 >= 27 && refmult2 < 36) cent = 4;
        else if(refmult2 >= 20 && refmult2 < 27) cent = 3;
        else if(refmult2 >= 14 && refmult2 < 20) cent = 2;
        else if(refmult2 >= 10 && refmult2 < 14) cent = 1;
        else if(refmult2 >= 7 && refmult2 < 10) cent = 0;

    }

    else if(energy == 62){

        if(refmult2 >= 334) cent = 15;
        else if(refmult2 >= 279 && refmult2 < 334) cent = 14;
        else if(refmult2 >= 233 && refmult2 < 279) cent = 13;
        else if(refmult2 >= 194 && refmult2 < 233) cent = 12;
        else if(refmult2 >= 160 && refmult2 < 194) cent = 11;
        else if(refmult2 >= 131 && refmult2 < 160) cent = 10;
        else if(refmult2 >= 106 && refmult2 < 131) cent = 9;
        else if(refmult2 >= 84 && refmult2 < 106) cent = 8;
        else if(refmult2 >= 66 && refmult2 < 84) cent = 7;
        else if(refmult2 >= 51 && refmult2 < 66) cent = 6;
        else if(refmult2 >= 39 && refmult2 < 51) cent = 5;
        else if(refmult2 >= 29 && refmult2 < 39) cent = 4;
        else if(refmult2 >= 21 && refmult2 < 29) cent = 3;
        else if(refmult2 >= 15 && refmult2 < 21) cent = 2;
        else if(refmult2 >= 10 && refmult2 < 15) cent = 1;
        else if(refmult2 >= 7 && refmult2 < 10) cent = 0;

    }

    else if(energy == 200){

        if(refmult2 >= 421) cent = 15;
        else if(refmult2 >= 355 && refmult2 < 421) cent = 14;
        else if(refmult2 >= 297 && refmult2 < 355) cent = 13;
        else if(refmult2 >= 247 && refmult2 < 297) cent = 12;
        else if(refmult2 >= 204 && refmult2 < 247) cent = 11;
        else if(refmult2 >= 167 && refmult2 < 204) cent = 10;
        else if(refmult2 >= 135 && refmult2 < 167) cent = 9;
        else if(refmult2 >= 108 && refmult2 < 135) cent = 8;
        else if(refmult2 >= 85 && refmult2 < 108) cent = 7;
        else if(refmult2 >= 65 && refmult2 < 85) cent = 6;
        else if(refmult2 >= 50 && refmult2 < 65) cent = 5;
        else if(refmult2 >= 37 && refmult2 < 50) cent = 4;
        else if(refmult2 >= 27 && refmult2 < 37) cent = 3;
        else if(refmult2 >= 19 && refmult2 < 27) cent = 2;
        else if(refmult2 >= 13 && refmult2 < 19) cent = 1;
        else if(refmult2 >= 9 && refmult2 < 13) cent = 0;

    }

    else {
    	cout << "Unimplemented energy " << energy << " returned centrality " << cent << endl;
    }


    return cent;

}


int get_centrality9(double mult, int energy) {

    int central = -1;

    if(energy == 7){

        float centFull[9] = {32,41,51,64,78,95,114,137,165};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 11){

        float centFull[9] = {41,52,65,80,98,118,143,172,206};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 19){

        float centFull[9] = {51,65,81,100,123,149,180,215,258};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 27){

        float centFull[9] = {56,71,90,111,135,164,198,237,284};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 39){

        float centFull[9] = {61,78,97,121,147,179,215,257,307};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 62){

        float centFull[9] = {66,84,106,131,160,194,233,279,334};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    else if(energy == 200){

        float centFull[9] = {85,108,135,167,204,247,297,355,421};
        if      (mult>=centFull[8]) central=9;
        else if (mult>=centFull[7]) central=8;
        else if (mult>=centFull[6]) central=7;
        else if (mult>=centFull[5]) central=6;
        else if (mult>=centFull[4]) central=5;
        else if (mult>=centFull[3]) central=4;
        else if (mult>=centFull[2]) central=3;
        else if (mult>=centFull[1]) central=2;
        else if (mult>=centFull[0]) central=1;

    }

    return central;

}
