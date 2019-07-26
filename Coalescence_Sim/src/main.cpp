/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: dylan
 */

#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>

#include <TROOT.h>
#include <TError.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>

#include "sim_v1.h"
#include "ratio_methods.h"
#include "plotting.h"
#include "config.h"
#include "sim_efficiency.h"
#include "ThreadPool.h"
#include "Stats.h"
#include "FourierApprox.h"

using namespace std;

void simulate();
void simulate_batch();
pair<map<string, measure<double>>, map<int, measure<double>>> run_param(double param_val, string param, TDirectory *ratio_dir, TDirectory *proton_dir);
vector<double> bin_ratios(vector<double> ratios, double min, double max, int bins);
void bin_ratios_test();
void roli_comp();
void run_comp(config::simulation_pars pars, TFile *out_file);
void test_stats();
void python_comp();
void fourier_test();

int main() {
//	bin_ratios_test();
//	simulate();
//	simulate_batch();
//	roli_comp();
//	test_stats();
//	python_comp();
	fourier_test();

	cout << "donzo" << endl;
	return(0);
}


void simulate() {
	config::simulation_pars pars;

	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
	TDirectory *ratio_dir = out_file->mkdir("ratios");
	TDirectory *proton_dir = out_file->mkdir("protons");

	map<double, map<int, tuple<double,double>>> cumulants_with_peffect;
	tree_data data = sim_v1(pars);
//	map<int, tuple<double,double>> cumulants;
	vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
//	for(int order:config::cumulant_orders) {
//		cumulants[order] = get_cumulant(ratios, order);
//	}
	ratio_dir->cd();
	hist_ratio_dist(ratios, "write");
	proton_dir->cd();
	hist_proton_dist(nproton_map_to_vec(data.good_protons[0]),"write");
//	cumulants_with_peffect[p_effect] = cumulants;

//	out_file->cd();
//	plot_cumulants_vs_peffect(cumulants_with_peffect);

	out_file->Close();
	delete out_file;
}


void simulate_batch() {
	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
	TDirectory *ratio_dir = out_file->mkdir("ratios");
	TDirectory *proton_dir = out_file->mkdir("protons");
	map<double, future<pair<map<string, measure<double>>, map<int, measure<double>>>>> futures;
	map<double, map<string, measure<double>>> stats;
	map<double, map<int, measure<double>>> cumulants;
	ROOT::EnableThreadSafety();
//	gErrorIgnoreLevel=kError;

	{
		ThreadPool pool(thread::hardware_concurrency());

		for(double param_val:config::param_list) {
			futures[param_val] = pool.enqueue(run_param, param_val, config::param, ratio_dir, proton_dir);
		}
	}

	for(auto && run:futures) {
		auto result = run.second.get();
		stats[run.first] = result.first;
		cumulants[run.first] = result.second;
	}


	out_file->cd();
	plot_stats_vs_x(stats, config::param);
	plot_cumulants_vs_x(cumulants, config::param);

	out_file->Close();
	delete out_file;
}


pair<map<string, measure<double>>, map<int, measure<double>>> run_param(double param_val, string param, TDirectory *ratio_dir, TDirectory *proton_dir) {
	config::simulation_pars pars;
	if(param == "param_val") {
		pars.p_effect = param_val;
	} else if(param == "p_cluster") {
		pars.p_cluster = param_val;
	}
	cout << "Starting: mean=" + to_string(pars.particle_mean) + " p_effect=" + to_string(pars.p_effect) + " p_cluster=" + to_string(pars.p_cluster) + "\n";

	tree_data data = sim_v1(pars);
	map<string, measure<double>> stats;
	map<int, measure<double>> cumulants;
	vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
	Stats stat(ratios);
	stats["mean"] = stat.get_mean();
	stats["standard_deviation"] = stat.get_standard_deviation();
	stats["skewness"] = stat.get_skewness();
	stats["kurtosis"] = stat.get_kurtosis();
	for(int order:config::cumulant_orders) {
		cumulants[order] = stat.get_cumulant(order);
	}
	ratio_dir->cd();
	hist_ratio_dist(ratios, "Ratio Dist " + param + ": " + to_string(param_val), "write");
	proton_dir->cd();
	hist_proton_dist(nproton_map_to_vec(data.good_protons[0]), "Proton Dist "  + param + ": " + to_string(param_val), "write");

	cout << "Finishing: mean=" + to_string(pars.particle_mean) + " p_effect=" + to_string(pars.p_effect) + " p_cluster=" + to_string(pars.p_cluster) + "\n";

	return(make_pair(stats, cumulants));
}



vector<double> bin_ratios(vector<double> ratios, double min, double max, int bins) {
	double bin_width = (max - min) / bins;
	vector<double> binned_ratios;
	int bin_index;
	for(double r:ratios) {
		bin_index = 0;
		while(bin_index * bin_width < r) {
			bin_index++;
		}
		binned_ratios.push_back((bin_index - 1)*bin_width + bin_width / 2.0);
	}

	return(binned_ratios);
}


void bin_ratios_test() {
	vector<double> test = {0.2, 0.5, 0.6, 0.55, 0.66};
	vector<double> test2 = bin_ratios(test, 0.0, 1.0, 3);

	for(double t:test2) {
		cout << t << endl;
	}
}


void roli_comp() {
	vector<double> mean_list = {10.0, 24.93, 39.3};
	vector<double> peffect_list = {0.95, 0.6, 0.1};
	vector<double> pcluster_list = {0.0, 0.1, 0.15};
	vector<int> div_list = {2, 3, 6};

	ROOT::EnableThreadSafety();
	gErrorIgnoreLevel=kError;

	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");

	{
		ThreadPool pool(thread::hardware_concurrency());
		for(double mean:mean_list) {
			for(double p_effect:peffect_list) {
				for(double p_cluster:pcluster_list) {
					for(int div:div_list) {
						config::simulation_pars pars;
						pars.particle_mean = mean;
						pars.p_effect = p_effect;
						pars.p_cluster = p_cluster;
						pars.divisions = div;
						pool.enqueue(run_comp, pars, out_file);
//						run_comp(pars, out_file);
	//					tree_data data = sim_v1(pars);
	//					vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
	//					TH1D *dylan = hist_ratios(ratios);
	//					TH1D *roli = sim_efficiency(pars.p_effect, pars.p_cluster, pars.particle_mean, pars.divisions, pars.n_events);
	//
	//					comp_hists(dylan, roli, "Div_"+to_string(div)+"_Mean_"+to_string(mean).substr(0,4)+"_PEff_"+to_string(p_effect).substr(0,4)+"_PClust_"+to_string(p_cluster).substr(0,4));
	//					delete dylan;
	//					delete roli;
					}
				}
			}
		}
	}

	out_file->Close();
	delete out_file;
}


void run_comp(config::simulation_pars pars, TFile *out_file) {
	string name = "Div_"+to_string(pars.divisions)+"_Mean_"+to_string(pars.particle_mean).substr(0,4)+"_PEff_"+to_string(pars.p_effect).substr(0,4)+"_PClust_"+to_string(pars.p_cluster).substr(0,4);
	cout << "Starting " + name << endl;
	tree_data data = sim_v1(pars);
	vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
	TH1D *dylan = hist_ratios(ratios);
	TH1D *roli = sim_efficiency(pars.p_effect, pars.p_cluster, pars.particle_mean, pars.divisions, pars.n_events);

	out_file->cd();
	comp_hists(dylan, roli, name);
	delete dylan;
	delete roli;
	cout << "Finishing " + name << endl;
}


void test_stats() {
	string dist_path = "/home/dylan/local_server/dyn0402/Research/Test_Data/test_dist.txt";
	ifstream file(dist_path);

	double point;
	vector<double> data;

	string line;
	while (getline(file, line)) {
		istringstream iss(line);
		iss >> point;
		data.push_back(point);
	}

	Stats stat(data);
	measure<double> mean = stat.get_mean();
	measure<double> sd = stat.get_standard_deviation();
	measure<double> skew = stat.get_skewness();
	measure<double> kurt = stat.get_kurtosis();
	cout << "Mean: " << mean.val << " +- " << mean.err << endl;
	cout << "Standard Deviation: " << sd.val << " +- " << sd.err << endl;
	cout << "Skewness: " << skew.val << " +- " << skew.err << endl;
	cout << "Kurtosis: " << kurt.val << " +- " << kurt.err << endl;
}


void python_comp() {
	config::simulation_pars pars;

	tree_data data = sim_v1(pars);
	map<string, measure<double>> stats;
	map<int, measure<double>> cumulants;
	vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
	Stats stat(ratios);
	stats["mean"] = stat.get_mean();
	stats["standard_deviation"] = stat.get_standard_deviation();
	stats["skewness"] = stat.get_skewness();
	stats["kurtosis"] = stat.get_kurtosis();
	cout << "Mean: " << stat.get_mean().val << endl;
	cout << "Standard Deviations: " << stat.get_standard_deviation().val << endl;
	cout << "Skewness: " << stat.get_skewness().val << endl;
	cout << "Kurtosis: " << stat.get_kurtosis().val << endl;
	for(int order:config::cumulant_orders) {
		cout << "Cumulant Order " << order << ": " << stat.get_cumulant(order).val << endl;
	}

	ofstream file("/home/dylan/local_server/dyn0402/Research/Test_Data/test_dist.txt");
	for(double ratio:ratios) {
		file << to_string(ratio) + "\n";
	}
	file.close();
}


void fourier_test() {
	TFile *file = new TFile("/home/dylan/local_server/dyn0402/Research/Test_Data/auau11.root", "READ");
	TTree *tree = (TTree*) file->Get("nsmTree");
	TLeaf *phi_leaf = tree->GetLeaf("Proton.phi");
	TH1D *hist = new TH1D("fourier_test_hist", "fourier_test_hist", 100, -2, 8);
	int event_index = 1;
	while(tree->GetEntry(event_index)) {
		for(int proton_index = 0; proton_index < phi_leaf->GetLen(); proton_index++) {
			hist->Fill(phi_leaf->GetValue(proton_index));
		}
		event_index++;
	}
	FourierApprox fapprox(hist);
	fapprox.set_fourier_n_max(2);
	TF1 *func = fapprox.get_approx();
	fapprox.print_coefs();
	cout << func->Eval(3.0) << " | " << func->Eval(3.5) << endl;
	TFile *out_file = new TFile("/home/dylan/local_server/dyn0402/Research/Test_Data/fourier_test.root", "RECREATE");
	out_file->cd();
	TCanvas *can = new TCanvas();
	hist->Draw();
	TCanvas *can2 = new TCanvas();
	func->Draw();
	can->Write("Fourier_Test_Can");
	can2->Write("Func_Test_Can");
	out_file->Close();
	file->Close();
}

