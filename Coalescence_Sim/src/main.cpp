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
#include "../Roli/sim_efficiency.h"
#include "ThreadPool.h"
#include "Stats.h"
#include "FourierApprox.h"
#include "Simulator.h"
#include "Measure.h"

#include "../Analyses/moments_vs_sim_pars.h"

using namespace std;

//void simulate();
//void simulate_batch();
//pair<map<string, measure<double>>, map<int, measure<double>>> run_param(double param_val, string param, TDirectory *ratio_dir, TDirectory *proton_dir);
//vector<double> bin_ratios(vector<double> ratios, double min, double max, int bins);
//void bin_ratios_test();
//void roli_comp();
//void run_comp(config::simulation_pars pars, TFile *out_file);
//void test_stats();
//void python_comp();
//void fourier_test();
//void sim_data_hists();
//void sim_mixed_data_hists();
//void mean_cluster_hist();
//void run_pars(int n_events, double mean, double p_eff, double p_clust, double spread, map<string, map<double, map<double, measure<double>>>> *results);
//void run_mix_norm_pars(int n_events, double particle_mean, double p_eff, double p_clust, double spread, map<string, map<double, map<double, measure<double>>>> *results);
//void run_mix_hist_norm_pars(int energy, double p_eff, double p_clust, double spread, TH1D *dist_hist, TFile *out_file, map<string, map<double, map<double, measure<double>>>> *results);
void measure_test();

int main() {
//	bin_ratios_test();
//	simulate();
//	simulate_batch();
//	roli_comp();
//	test_stats();
//	python_comp();
//	fourier_test();
//	sim_data_hists();
//	mean_cluster_hist();
//	measure_test();
//	sim_mixed_data_hists();
	moments_vs_sim_pars();

	cout << "donzo" << endl;
	return(0);
}


//void simulate() {
//	config::simulation_pars pars;
//
//	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
//	TDirectory *ratio_dir = out_file->mkdir("ratios");
//	TDirectory *proton_dir = out_file->mkdir("protons");
//
//	map<double, map<int, tuple<double,double>>> cumulants_with_peffect;
//	tree_data data = sim_v1(pars);
////	map<int, tuple<double,double>> cumulants;
//	vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
////	for(int order:config::cumulant_orders) {
////		cumulants[order] = get_cumulant(ratios, order);
////	}
//	ratio_dir->cd();
//	hist_ratio_dist(ratios, "write");
//	proton_dir->cd();
//	hist_proton_dist(nproton_map_to_vec(data.good_protons[0]),"write");
////	cumulants_with_peffect[p_effect] = cumulants;
//
////	out_file->cd();
////	plot_cumulants_vs_peffect(cumulants_with_peffect);
//
//	out_file->Close();
//	delete out_file;
//}
//
//
//void simulate_batch() {
//	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
//	TDirectory *ratio_dir = out_file->mkdir("ratios");
//	TDirectory *proton_dir = out_file->mkdir("protons");
//	map<double, future<pair<map<string, measure<double>>, map<int, measure<double>>>>> futures;
//	map<double, map<string, measure<double>>> stats;
//	map<double, map<int, measure<double>>> cumulants;
//	ROOT::EnableThreadSafety();
////	gErrorIgnoreLevel=kError;
//
//	{
//		ThreadPool pool(thread::hardware_concurrency());
//
//		for(double param_val:config::param_list) {
//			futures[param_val] = pool.enqueue(run_param, param_val, config::param, ratio_dir, proton_dir);
//		}
//	}
//
//	for(auto && run:futures) {
//		auto result = run.second.get();
//		stats[run.first] = result.first;
//		cumulants[run.first] = result.second;
//	}
//
//
//	out_file->cd();
//	plot_stats_vs_x(stats, config::param);
//	plot_cumulants_vs_x(cumulants, config::param);
//
//	out_file->Close();
//	delete out_file;
//}
//
//
//pair<map<string, measure<double>>, map<int, measure<double>>> run_param(double param_val, string param, TDirectory *ratio_dir, TDirectory *proton_dir) {
//	config::simulation_pars pars;
//	if(param == "param_val") {
//		pars.p_effect = param_val;
//	} else if(param == "p_cluster") {
//		pars.p_cluster = param_val;
//	}
//	cout << "Starting: mean=" + to_string(pars.particle_mean) + " p_effect=" + to_string(pars.p_effect) + " p_cluster=" + to_string(pars.p_cluster) + "\n";
//
//	tree_data data = sim_v1(pars);
//	map<string, measure<double>> stats;
//	map<int, measure<double>> cumulants;
//	vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
//	Stats stat(ratios);
//	stats["mean"] = stat.get_mean();
//	stats["standard_deviation"] = stat.get_standard_deviation();
//	stats["skewness"] = stat.get_skewness();
//	stats["kurtosis"] = stat.get_kurtosis();
//	for(int order:config::cumulant_orders) {
//		cumulants[order] = stat.get_cumulant(order);
//	}
//	ratio_dir->cd();
//	hist_ratio_dist(ratios, "Ratio Dist " + param + ": " + to_string(param_val), "write");
//	proton_dir->cd();
//	hist_proton_dist(nproton_map_to_vec(data.good_protons[0]), "Proton Dist "  + param + ": " + to_string(param_val), "write");
//
//	cout << "Finishing: mean=" + to_string(pars.particle_mean) + " p_effect=" + to_string(pars.p_effect) + " p_cluster=" + to_string(pars.p_cluster) + "\n";
//
//	return(make_pair(stats, cumulants));
//}
//
//
//
//vector<double> bin_ratios(vector<double> ratios, double min, double max, int bins) {
//	double bin_width = (max - min) / bins;
//	vector<double> binned_ratios;
//	int bin_index;
//	for(double r:ratios) {
//		bin_index = 0;
//		while(bin_index * bin_width < r) {
//			bin_index++;
//		}
//		binned_ratios.push_back((bin_index - 1)*bin_width + bin_width / 2.0);
//	}
//
//	return(binned_ratios);
//}
//
//
//void bin_ratios_test() {
//	vector<double> test = {0.2, 0.5, 0.6, 0.55, 0.66};
//	vector<double> test2 = bin_ratios(test, 0.0, 1.0, 3);
//
//	for(double t:test2) {
//		cout << t << endl;
//	}
//}
//
//
//void roli_comp() {
//	vector<double> mean_list = {10.0, 24.93, 39.3};
//	vector<double> peffect_list = {0.95, 0.6, 0.1};
//	vector<double> pcluster_list = {0.0, 0.1, 0.15};
//	vector<int> div_list = {2, 3, 6};
//
//	ROOT::EnableThreadSafety();
//	gErrorIgnoreLevel=kError;
//
//	TFile *out_file = new TFile(config::out_path.data(), "RECREATE");
//
//	{
//		ThreadPool pool(thread::hardware_concurrency());
//		for(double mean:mean_list) {
//			for(double p_effect:peffect_list) {
//				for(double p_cluster:pcluster_list) {
//					for(int div:div_list) {
//						config::simulation_pars pars;
//						pars.particle_mean = mean;
//						pars.p_effect = p_effect;
//						pars.p_cluster = p_cluster;
//						pars.divisions = div;
//						pool.enqueue(run_comp, pars, out_file);
////						run_comp(pars, out_file);
//	//					tree_data data = sim_v1(pars);
//	//					vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
//	//					TH1D *dylan = hist_ratios(ratios);
//	//					TH1D *roli = sim_efficiency(pars.p_effect, pars.p_cluster, pars.particle_mean, pars.divisions, pars.n_events);
//	//
//	//					comp_hists(dylan, roli, "Div_"+to_string(div)+"_Mean_"+to_string(mean).substr(0,4)+"_PEff_"+to_string(p_effect).substr(0,4)+"_PClust_"+to_string(p_cluster).substr(0,4));
//	//					delete dylan;
//	//					delete roli;
//					}
//				}
//			}
//		}
//	}
//
//	out_file->Close();
//	delete out_file;
//}
//
//
//void run_comp(config::simulation_pars pars, TFile *out_file) {
//	string name = "Div_"+to_string(pars.divisions)+"_Mean_"+to_string(pars.particle_mean).substr(0,4)+"_PEff_"+to_string(pars.p_effect).substr(0,4)+"_PClust_"+to_string(pars.p_cluster).substr(0,4);
//	cout << "Starting " + name << endl;
//	tree_data data = sim_v1(pars);
//	vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
//	TH1D *dylan = hist_ratios(ratios);
//	TH1D *roli = sim_efficiency(pars.p_effect, pars.p_cluster, pars.particle_mean, pars.divisions, pars.n_events);
//
//	out_file->cd();
//	comp_hists(dylan, roli, name);
//	delete dylan;
//	delete roli;
//	cout << "Finishing " + name << endl;
//}
//
//
//void test_stats() {
//	string dist_path = "/home/dylan/local_server/dyn0402/Research/Test_Data/test_dist.txt";
//	ifstream file(dist_path);
//
//	double point;
//	vector<double> data;
//
//	string line;
//	while (getline(file, line)) {
//		istringstream iss(line);
//		iss >> point;
//		data.push_back(point);
//	}
//
//	Stats stat(data);
//	measure<double> mean = stat.get_mean();
//	measure<double> sd = stat.get_standard_deviation();
//	measure<double> skew = stat.get_skewness();
//	measure<double> kurt = stat.get_kurtosis();
//	cout << "Mean: " << mean.val << " +- " << mean.err << endl;
//	cout << "Standard Deviation: " << sd.val << " +- " << sd.err << endl;
//	cout << "Skewness: " << skew.val << " +- " << skew.err << endl;
//	cout << "Kurtosis: " << kurt.val << " +- " << kurt.err << endl;
//}
//
//
//void python_comp() {
//	config::simulation_pars pars;
//
//	tree_data data = sim_v1(pars);
//	map<string, measure<double>> stats;
//	map<int, measure<double>> cumulants;
//	vector<double> ratios = ratios_map_to_vec(data.ratios[pars.divisions][0]);
//	Stats stat(ratios);
//	stats["mean"] = stat.get_mean();
//	stats["standard_deviation"] = stat.get_standard_deviation();
//	stats["skewness"] = stat.get_skewness();
//	stats["kurtosis"] = stat.get_kurtosis();
//	cout << "Mean: " << stat.get_mean().val << endl;
//	cout << "Standard Deviations: " << stat.get_standard_deviation().val << endl;
//	cout << "Skewness: " << stat.get_skewness().val << endl;
//	cout << "Kurtosis: " << stat.get_kurtosis().val << endl;
//	for(int order:config::cumulant_orders) {
//		cout << "Cumulant Order " << order << ": " << stat.get_cumulant(order).val << endl;
//	}
//
//	ofstream file("/home/dylan/local_server/dyn0402/Research/Test_Data/test_dist.txt");
//	for(double ratio:ratios) {
//		file << to_string(ratio) + "\n";
//	}
//	file.close();
//}
//
//
//void fourier_test() {
//	TFile *file = new TFile("/home/dylan/local_server/dyn0402/Research/Test_Data/auau11.root", "READ");
//	TTree *tree = (TTree*) file->Get("nsmTree");
//	TLeaf *phi_leaf = tree->GetLeaf("Proton.phi");
//	TH1D *hist = new TH1D("fourier_test_hist", "fourier_test_hist", 100, -2, 8);
//	int event_index = 1;
//	while(tree->GetEntry(event_index)) {
//		for(int proton_index = 0; proton_index < phi_leaf->GetLen(); proton_index++) {
//			hist->Fill(phi_leaf->GetValue(proton_index));
//		}
//		event_index++;
//	}
//	FourierApprox fapprox(hist);
//	fapprox.set_fourier_n_max(2);
//	TF1 *func = fapprox.get_approx();
//	fapprox.print_coefs();
//	cout << func->Eval(3.0) << " | " << func->Eval(3.5) << endl;
//	TFile *out_file = new TFile("/home/dylan/local_server/dyn0402/Research/Test_Data/fourier_test.root", "RECREATE");
//	out_file->cd();
//	TCanvas *can = new TCanvas();
//	hist->Draw();
//	TCanvas *can2 = new TCanvas();
//	func->Draw();
//	can->Write("Fourier_Test_Can");
//	can2->Write("Func_Test_Can");
//	out_file->Close();
//	file->Close();
//}
//
//
//void sim_data_hists() {
//	TFile *out_file = new TFile("/home/dylan/local_server/dyn0402/Research/Simulation/Real_Proton_Dists/07-26-19_eff95_clus25.root", "RECREATE");
//	TFile *file = new TFile("/home/dylan/local_server/dyn0402/Research/Results/07-23_new_data.root", "READ");
//	int cent = 15;
//	vector<int> energy_list = {7, 11, 19, 27, 39, 62};
//	map<double, map<string, measure<double>>> stats;
//	for(int energy:energy_list) {
//		cout << "Starting " << energy << "GeV" << endl;
//		string root_path = "NProton_Dists/" + to_string(energy) + "GeV/" + to_string(energy) + "GeV protons Centrality " + to_string(cent);
//		TH1D *proton_hist = (TH1D*) file->Get(root_path.data()); // Current memory leak. Fix.
//		Simulator sim;
//		sim.set_proton_dist_hist(proton_hist);
//		sim.set_p_effect(0.95);
//		sim.set_p_cluster(0.25);
//		sim.set_n_events((int)(proton_hist->GetEntries() + 0.5));
//		tree_data data = sim.run_simulation();
//		vector<double> ratios = ratios_map_to_vec(data.ratios[sim.get_divisions()][0]);
//		Stats stat(ratios);
//		stats[(double)energy]["mean"] = stat.get_mean();
//		stats[(double)energy]["standard_deviation"] = stat.get_standard_deviation();
//		stats[(double)energy]["skewness"] = stat.get_skewness();
//		stats[(double)energy]["kurtosis"] = stat.get_kurtosis();
//		TH2I *hist = ratios_map_to_hist(data.ratios[sim.get_divisions()][0], to_string(energy)+"GeV_Ratio_Hist"); // Memory leak
//		out_file->cd();
//		TCanvas *can = new TCanvas((to_string(energy)+"GeV_Ratio_Hist").data()); // Memory leak
//		TF1 *line = new TF1((to_string(energy)+"GeV_Ratio_Line").data(), ("x/"+to_string(sim.get_divisions())).data(), -0.5, 0.5+(--data.ratios[sim.get_divisions()][0].end())->first);
//		can->SetLogz();
//		hist->Draw("colz");
//		line->Draw("same");
//		hist->Write();
//		can->Write();
//	}
//	file->Close();
//
//	out_file->cd();
//	plot_stats_vs_x(stats, "Energy (GeV)");
//	out_file->Close();
//
//}
//
//
//
//void sim_mixed_data_hists() {
//	TFile *out_file = new TFile("/home/dylan/local_server/dyn0402/Research/Simulation/Real_Proton_Dists/07-30-19_mix.root", "RECREATE");
//	TFile *file = new TFile("/home/dylan/local_server/dyn0402/Research/Results/07-23_new_data.root", "READ");
//	int cent = 15;
//	vector<int> energy_list = {7, 11, 19, 27, 39, 62};
//	vector<double> cl_list = {0, 0.05, 0.1, 0.15, 0.2};
//	double p_eff = 0.95;
//	double spread = 0.2;
//	map<string, map<double, map<double, measure<double>>>> results;
//	map<int, TH1D*> dist_hists;
//	ROOT::EnableThreadSafety();
//	{
//		ThreadPool pool(thread::hardware_concurrency());
//		for(int energy:energy_list) {
//			string root_path = "NProton_Dists/" + to_string(energy) + "GeV/" + to_string(energy) + "GeV protons Centrality " + to_string(cent);
//			dist_hists[energy] = (TH1D*) file->Get(root_path.data());
//			for(double p_clust:cl_list) {
//				pool.enqueue(run_mix_hist_norm_pars, energy, p_eff, p_clust, spread, dist_hists[energy], out_file, &results);
//			}
//		}
//	}
//	for(pair<int, TH1D*> hist:dist_hists) {
//		delete hist.second;
//	}
//	file->Close();
//
//	vector<string> moments = {"mean", "standard_deviation", "skewness", "kurtosis"};
//	out_file->cd();
//	for(string mom:moments) {
//		graph_2d(results[mom], mom+" with energy and cluster", "energy", "p_cluster", mom);
//	}
//	out_file->Close();
//
//}
//
//
//void mean_cluster_hist() {
//	TFile *out_file = new TFile("/home/dylan/local_server/dyn0402/Research/Simulation/07-30-19_mean_vs_clust_spread.root", "RECREATE");
//	vector<double> mean_list = {5, 7.5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
//	vector<double> cl_list = {0, 0.025, 0.05, 0.075, 0.10, 0.125, 0.15, 0.175, 0.20};
//	int n_events = 1000000;
//	double p_eff = 0.95;
//	double spread = 0.2;
//	vector<string> moments = {"mean", "standard_deviation", "skewness", "kurtosis"};
//	map<string, map<double, map<double, measure<double>>>> results;
//	map<double, map<string, measure<double>>> stats;
//	ROOT::EnableThreadSafety();
//	{
//		ThreadPool pool(thread::hardware_concurrency());
//		for(double mean:mean_list) {
//			for(double p_clust:cl_list) {
//				pool.enqueue(run_pars, n_events, mean, p_eff, p_clust, spread, &results);
//			}
//		}
//	}
//
//	out_file->cd();
//	for(string mom:moments) {
//		graph_2d(results[mom], mom+" with mean and cluster", "mean", "p_cluster", mom);
//	}
//	out_file->Close();
//}
//
//
//void run_pars(int n_events, double mean, double p_eff, double p_clust, double spread, map<string, map<double, map<double, measure<double>>>> *results) {
//	cout << "Starting: \t events: " << n_events << "  mean: " << mean << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
//	Simulator sim;
//	sim.set_p_effect(p_eff);
//	sim.set_p_cluster(p_clust);
//	sim.set_particle_mean(mean);
//	sim.set_n_events(n_events);
//	sim.set_spread_sigma(spread);
//	tree_data data = sim.run_simulation_mixed()[0];
//	vector<double> ratios = ratios_map_to_vec(data.ratios[sim.get_divisions()][0]);
//	Stats stat(ratios);
//	(*results)["mean"][mean][p_clust] = stat.get_mean();
//	(*results)["standard_deviation"][mean][p_clust] = stat.get_standard_deviation();
//	(*results)["skewness"][mean][p_clust] = stat.get_skewness();
//	(*results)["kurtosis"][mean][p_clust] = stat.get_kurtosis();
//	cout << "Finishing: \t events: " << n_events << "  mean: " << mean << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
//}
//
//
//void run_mix_norm_pars(int n_events, double particle_mean, double p_eff, double p_clust, double spread, map<string, map<double, map<double, measure<double>>>> *results) {
//	cout << "Starting: \t events: " << n_events << "  mean: " << particle_mean << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
//	Simulator sim;
//	sim.set_p_effect(p_eff);
//	sim.set_p_cluster(p_clust);
//	sim.set_particle_mean(particle_mean);
//	sim.set_n_events(n_events);
//	sim.set_spread_sigma(spread);
//	vector<tree_data> data = sim.run_simulation_mixed();
//	vector<double> ratios = ratios_map_to_vec(data[0].ratios[sim.get_divisions()][0]);
//	vector<double> mixed_ratios = ratios_map_to_vec(data[1].ratios[sim.get_divisions()][0]);
//	Stats stat(ratios);
//	Stats mixed_stat(mixed_ratios);
//	(*results)["mean"][particle_mean][p_clust] = divide(stat.get_mean(), mixed_stat.get_mean());
//	(*results)["standard_deviation"][particle_mean][p_clust] = divide(stat.get_standard_deviation(), mixed_stat.get_standard_deviation());
//	(*results)["skewness"][particle_mean][p_clust] = divide(stat.get_skewness(), mixed_stat.get_skewness());
//	(*results)["kurtosis"][particle_mean][p_clust] = divide(stat.get_kurtosis(), mixed_stat.get_kurtosis());
//	cout << "Finishing: \t events: " << n_events << "  mean: " << particle_mean << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
//}
//
//
//
//void run_mix_hist_norm_pars(int energy, double p_eff, double p_clust, double spread, TH1D *dist_hist, TFile *out_file, map<string, map<double, map<double, measure<double>>>> *results) {
//	int n_events = (int)(dist_hist->GetEntries() + 0.5);
//	cout << "Starting: \t events: " << n_events << "  energy: " << energy << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
//	Simulator sim;
//	sim.set_p_effect(p_eff);
//	sim.set_p_cluster(p_clust);
//	sim.set_proton_dist_hist(dist_hist);
//	sim.set_n_events(n_events);
//	sim.set_spread_sigma(spread);
//
//	vector<tree_data> data = sim.run_simulation_mixed();
//	vector<double> ratios = ratios_map_to_vec(data[0].ratios[sim.get_divisions()][0]);
//	vector<double> mixed_ratios = ratios_map_to_vec(data[1].ratios[sim.get_divisions()][0]);
//	Stats stat(ratios);
//	Stats mixed_stat(mixed_ratios);
//	(*results)["mean"][energy][p_clust] = divide(stat.get_mean(), mixed_stat.get_mean());
//	(*results)["standard_deviation"][energy][p_clust] = divide(stat.get_standard_deviation(), mixed_stat.get_standard_deviation());
//	(*results)["skewness"][energy][p_clust] = divide(stat.get_skewness(), mixed_stat.get_skewness());
//	(*results)["kurtosis"][energy][p_clust] = divide(stat.get_kurtosis(), mixed_stat.get_kurtosis());
//
//
//	ostringstream os("");
//	os << energy << "GeV | peff: " << setprecision(2) << p_eff << " | p_clust: " << setprecision(2) << p_clust << " | spread: " << setprecision(2) << spread << ends;
//	string name = os.str();
//	TH2I *hist = ratios_map_to_hist(data[0].ratios[sim.get_divisions()][0], name+"GeV_Ratio_Hist");
//	out_file->cd();
//	TCanvas *can = new TCanvas((name+"GeV_Ratio_Hist").data()); // Memory leak
//	TF1 *line = new TF1((name+"GeV_Ratio_Line").data(), ("x/"+to_string(sim.get_divisions())).data(), -0.5, 0.5+(--data[0].ratios[sim.get_divisions()][0].end())->first);
//	TF1 *line2 = new TF1((name+"GeV_Ratio_Line2").data(), "x", -0.5, 0.5+(--data[0].ratios[sim.get_divisions()][0].end())->first);
//	line2->SetLineColor(kBlue);
//	can->SetLogz();
//	line2->Draw();
//	hist->Draw("colz");
//	line->Draw("same");
//	hist->Write();
//	can->Write();
//
//	ostringstream mix_os("");
//	mix_os << "Mix " << energy << "GeV | peff: " << setprecision(2) << p_eff << " | p_clust: " << setprecision(2) << p_clust << " | spread: " << setprecision(2) << spread << ends;
//	string mix_name = mix_os.str();
//	TH2I *mix_hist = ratios_map_to_hist(data[1].ratios[sim.get_divisions()][0], mix_name+"GeV_Ratio_Hist");
//	TCanvas *mix_can = new TCanvas((mix_name+"GeV_Ratio_Hist").data());
//	TF1 *mix_line = new TF1((mix_name+"GeV_Ratio_Line").data(), ("x/"+to_string(sim.get_divisions())).data(), -0.5, 0.5+(--data[1].ratios[sim.get_divisions()][0].end())->first);
//	TF1 *mix_line2 = new TF1((name+"GeV_Ratio_Line2").data(), "x", -0.5, 0.5+(--data[0].ratios[sim.get_divisions()][0].end())->first);
//	mix_line2->SetLineColor(kBlue);
//	mix_can->SetLogz();
//	mix_line2->Draw();
//	mix_hist->Draw("colz");
//	mix_line->Draw("same");
//	mix_hist->Write();
//	mix_can->Write();
//
//	delete hist;
//	delete line;
//	delete line2;
//	delete can;
//
//	delete mix_hist;
//	delete mix_line;
//	delete mix_line2;
//	delete mix_can;
//
//	cout << "Finishing: \t events: " << n_events << "  energy: " << energy << "  p_eff: " << p_eff << "  p_clust: " << p_clust << endl;
//}


void measure_test() {
	Measure a(10, 2);
	Measure b(5, 1);

	cout << "Addition: " << string(a+b) << endl;
	cout << "Subtraction: " << string(a-b) << endl;
	cout << "Multiplication: " << string(a*b) << endl;
	cout << "Division: " << string(a/b) << endl;
	cout << "Exponents: " << string(a.pow(2)) << endl;

	Measure x1(100, 1.1);
	Measure x2(50, 1.2);
	Measure y1(200, 2.2);
	Measure y2(100, 2.3);

	std::cout << std::string(((x1 - x2).pow(2.) + (y1 - y2).pow(2.)).pow(0.5)) << std::endl; // => 111.803398874989 ±2.938366893361
}

