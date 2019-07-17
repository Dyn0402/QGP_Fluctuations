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

#include "sim_v1.h"
#include "ratio_methods.h"
#include "plotting.h"
#include "config.h"
#include "sim_efficiency.h"
#include "ThreadPool.h"
#include "Stats.h"

using namespace std;

void simulate();
void simulate_batch();
vector<double> bin_ratios(vector<double> ratios, double min, double max, int bins);
void bin_ratios_test();
void roli_comp();
void run_comp(config::simulation_pars pars, TFile *out_file);
void test_stats();

int main() {
//	bin_ratios_test();
//	simulate();
//	simulate_batch();
//	roli_comp();
	test_stats();

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
	config::simulation_pars pars;
	cout << "Here" << endl;
	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
	TDirectory *ratio_dir = out_file->mkdir("ratios");
	TDirectory *proton_dir = out_file->mkdir("protons");
	cout << "Here2" << endl;

	map<double, map<int, tuple<double,double>>> cumulants_with_peffect;
	for(double p_effect:{0.0, 0.05, 0.10, 0.15, 0.20, 0.25}) {
		cout << "Running p_effect: " << p_effect << endl;
		pars.p_effect = p_effect;
		tree_data data = sim_v1(pars);
		map<int, tuple<double,double>> cumulants;
		vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
//		ratios = bin_ratios(ratios, -0.5, 1.1, 23);
//		cout << "ratios: " << flush;
//		for(double r:ratios) { cout << r << " " << flush; }
		for(int order:config::cumulant_orders) {
			cumulants[order] = get_cumulant(ratios, order);
		}
		ratio_dir->cd();
		hist_ratio_dist(ratios, "write");
		proton_dir->cd();
		hist_proton_dist(nproton_map_to_vec(data.good_protons[0]),"write");
		cumulants_with_peffect[p_effect] = cumulants;
	}

	out_file->cd();
	plot_cumulants_vs_peffect(cumulants_with_peffect);

	out_file->Close();
	delete out_file;
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
		ThreadPool pool(12);
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

