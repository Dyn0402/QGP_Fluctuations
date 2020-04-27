/*
 * main.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */


#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <cstdlib>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLine.h>
#include <TLegend.h>

#include "ThreadPool.h"
#include "TreeReader.h"
#include "AmptCentralityMaker.h"
#include "AmptConverter.h"
#include "TreeLeaves.h"
#include "ratio_methods.h"
#include "file_io.h"

#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"

using namespace std;


void read_class();
void read_comb_sys();
//void real_event_tree_test();
//void speed_test();
void res_plot();
void res_calc();
void res_calc_debug();
void speed_test_class();
void ref_mult_test();
void ampt_cent_test();
void ampt_cent_b_corr();

void run_set(int energy, int set_num, string set_name);

clock_t start = clock();
auto start_sys = chrono::system_clock::now();


int main(int argc, char** argv) {
	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Track.h");
	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Event.h");
//	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Track.cpp");
//	gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Event.cpp");
//	AmptConverter converter39("/media/dylan/SSD_Storage/Research/ampt/AuAu_nt150_3mb_39gev/", "/media/dylan/SSD_Storage/Research/Trees_Ampt/39GeV/");
//	converter39.convert_trees();
//	AmptConverter converter11("/media/dylan/SSD_Storage/Research/ampt/AuAu_nt150_3mb_11gev/", "/media/dylan/SSD_Storage/Research/Trees_Ampt/11GeV/");
//	converter11.convert_trees();
	read_class();
//	ampt_cent_b_corr();
//	ampt_cent_test();
//	ref_mult_test();
//	res_plot();
//	real_event_tree_test();
//	speed_test();
//	speed_test_class();

	cout << endl << "donzo" << endl;
	return(0);

}




void read_class() {
//	map<string, pair<int, int>> set_pairs = {{"Sim_Flow", {0,0}}, {"Sim_Flow_No_Rotate", {0,0}}, {"Sim_Eff_Hole3-4_Flow", {0,0}}, {"Sim_Eff_Hole3-4_Flow_No_Rotate", {0,0}}};
//	map<string, pair<int, int>> set_pairs = {{"Sim_Flow_08res_05v2", {0,0}}, {"Sim_Flow_08res_05v2_No_Rotate", {0,0}}, {"Sim_Flow_05res_05v2", {0,0}}, {"Sim_Flow_05res_05v2_No_Rotate", {0,0}}};
//	map<string, pair<int, int>> set_pairs {{"Sim_Flow_05v2_08res_Test", {0,0}}, {"Sim_Flow_05v2_05res_Test", {0,0}}};
//	map<string, pair<int, int>> set_pairs = {{"Sim_0p0s", {0,0}}, {"Sim_05p002s", {0,0}}, {"Sim_0p0s_No_Rotate", {0,0}}, {"Sim_05p002s_No_Rotate", {0,0}},
//				{"Sim_0p0s_Eff_Hole3-4", {0,0}}, {"Sim_05p002s_Eff_Hole3-4", {0,0}}, {"Sim_0p0s_Eff_Hole3-4_No_Rotate", {0,0}}, {"Sim_05p002s_Eff_Hole3-4_No_Rotate", {0,0}},
//				{"Sim_0p0s_Eff", {0,0}}, {"Sim_05p002s_Eff", {0,0}}, {"Sim_0p0s_Eff_No_Rotate", {0,0}}, {"Sim_05p002s_Eff_No_Rotate", {0,0}},
//				{"Sim_0p0s_Flow_08res_05v2", {0,0}}, {"Sim_05p002s_Flow_08res_05v2", {0,0}}, {"Sim_0p0s_Flow_08res_05v2_No_Rotate", {0,0}}, {"Sim_05p002s_Flow_08res_05v2_No_Rotate", {0,0}},
//				{"Sim_0p0s_Flow_05res_05v2", {0,0}}, {"Sim_05p002s_Flow_05res_05v2", {0,0}}, {"Sim_0p0s_Flow_05res_05v2_No_Rotate", {0,0}}, {"Sim_05p002s_Flow_05res_05v2_No_Rotate", {0,0}},
//				{"Sim_0p0s_Flow_099res_05v2", {0,0}}, {"Sim_05p002s_Flow_099res_05v2", {0,0}}, {"Sim_0p0s_Flow_099res_05v2_No_Rotate", {0,0}}, {"Sim_05p002s_Flow_099res_05v2_No_Rotate", {0,0}}
//				};
//map<string, pair<int, int>> set_pairs = {{"Sim_05p05s", {0,0}}, {"Sim_05p05s_No_Rotate", {0,0}},
//				{"Sim_05p05s_Eff_Hole3-4", {0,0}}, {"Sim_05p05s_Eff_Hole3-4_No_Rotate", {0,0}},
//				{"Sim_05p05s_Eff", {0,0}}, {"Sim_05p05s_Eff_No_Rotate", {0,0}},
//				{"Sim_05p05s_Flow_08res_05v2", {0,0}}, {"Sim_05p05s_Flow_08res_05v2_No_Rotate", {0,0}},
//				{"Sim_05p05s_Flow_05res_05v2", {0,0}}, {"Sim_05p05s_Flow_05res_05v2_No_Rotate", {0,0}},
//				{"Sim_05p05s_Flow_099res_05v2", {0,0}}, {"Sim_05p05s_Flow_099res_05v2_No_Rotate", {0,0}}
//				};
//	map<string, pair<int, int>> set_pairs = {{"Sim_05p002s", {0,0}}, {"Sim_05p002s_Eff_Hole3-4", {0,0}}, {"Sim_05p002s_Eff", {0,0}}, {"Sim_05p002s_Flow_08res_05v2", {0,0}}, {"Sim_05p002s_Flow_05res_05v2", {0,0}}, {"Sim_05p002s_Flow_099res_05v2", {0,0}}};
//	map<string, pair<int, int>> set_pairs = {{"eta05", {0,0}}, {"eta1", {0,0}}, {"eta05_No_Rotate", {0,0}}, {"eta1_No_Rotate", {0,0}}};
//	map<string, pair<int, int>> set_pairs = {{"eta05_old", {5,9}}};
	map<string, pair<int, int>> set_pairs = {{"p+", {0,4}}, {"p-", {0,4}}, {"ptotal", {0,4}}};

	int set_sleep = 15;
	int energy_sleep = 1;

	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(pair<string, pair<int, int>> set_pair:set_pairs) {
			for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
				string set_dir = set_pair.first + to_string(set_num) + "/";
				cout << endl << "Queueing " + set_dir <<  "  set_num: " << set_num << endl << endl;
				vector<int> energy_list {7, 11, 39, 27, 62, 19};
				for(int energy:energy_list) {
					pool.enqueue(run_set, energy, set_num, set_pair.first);
					this_thread::sleep_for(chrono::seconds(energy_sleep));
				}
				this_thread::sleep_for(chrono::seconds(set_sleep));
			}
		}
	}
}


void run_set(int energy, int set_num, string set_name) {
	string base_path = "/home/dylan/Research/";
	string in_base_path = "/media/dylan/SSD_Storage/Research/";//base_path;
	string out_base_path = base_path;
	int ref = 3;

	string in_path = in_base_path + "Trees/";
	string out_dir = out_base_path + "Data/";
	string mix_out_dir = out_base_path + "Data_Mix/";

	vector<int> divs {2, 3, 4, 5, 6};
	map<int, int> sim_cent_events = {{0, 500000}, {1, 500000}, {2, 500000}, {3, 500000}, {4, 500000}, {5, 500000}, {6, 500000}, {7, 500000}, {8, 20000000}};
//	map<int, int> sim_cent_events = {{0, 0}, {1, 0}, {2, 0}, {3, 1000}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 100000}};

	string set_dir = set_name + to_string(set_num) + "/";

	TreeReader reader(energy, ref);
	reader.set_divs(divs);
	reader.set_cbwc(false);
	reader.set_cent_binning(9);

	reader.set_in_path(in_path);
	reader.set_out_path(out_dir+set_dir);
	reader.set_qa_path(out_dir+set_dir+to_string(energy)+"GeV/");
	reader.set_qa_name("QA_");
	reader.set_set_name(set_name + to_string(set_num));
	reader.set_tree_name("tree");
	if(in_string(set_name, "Ampt")) { reader.set_tree_name("tree"); }

	if(in_string(set_name, "EP_Rotate"))  { reader.set_event_plane(true); }
	else { reader.set_event_plane(false); }

	if(in_string(set_name, {"No_Rotate", "EP_Rotate"}, false)) { reader.set_rotate_random(false); }
	else{ reader.set_rotate_random(true); }

	if(in_string(set_name, "Pile_Up_01_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.01); }
	else if(in_string(set_name, "Pile_Up_008_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.008); }
	else if(in_string(set_name, "Pile_Up_005_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.005); }
	else if(in_string(set_name, "Pile_Up_002_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.002); }
	else if(in_string(set_name, "Pile_Up_001_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.001); }
	else if(in_string(set_name, "Pile_Up_0008_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.0008); }
	else if(in_string(set_name, "Pile_Up_0005_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.0005); }
	else if(in_string(set_name, "Pile_Up_0002_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.0002); }
	else { reader.set_pile_up(false); reader.set_pile_up_prob(0); }

	if(in_string(set_name,  "No_BTof_Rej")) {
		reader.cut.max_slope = {{7, 999}, {11, 999}, {14, 999}, {19, 999}, {27, 999}, {39, 999}, {62, 999}, {200, 999}};
		reader.cut.min_slope = {{7, -999}, {11, -999}, {14, -999}, {19, -999}, {27, -999}, {39, -999}, {62, -999}, {200, -999}};
	}

//	if(set_name == "Single_Ratio" || set_name == "No_Rotate_Single" ) { reader.set_single_ratio(true); reader.mix.set_single_ratio(true); }
	reader.set_single_ratio(true); reader.mix.set_single_ratio(true);

	if(in_string(set_name, "eta1")) { reader.cut.min_eta = -1.0, reader.cut.max_eta = 1.0; }
	else if(in_string(set_name, "eta05")) { reader.cut.min_eta = -0.5, reader.cut.max_eta = 0.5; }

	if(in_string(set_name,  "Efficiency_08_")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.08); }
	else if(in_string(set_name,  "Efficiency_05_")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.05); }
	else if(in_string(set_name,  "Efficiency_025_")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.025); }
	else if(in_string(set_name,  "Efficiency_01_")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.01); }
	else { reader.set_efficiency(false); reader.set_efficiency_prob(0); }

	if(in_string(set_name, {"Sim", "0p0s"}, true)) { reader.sim.set_p_group(0.0); reader.sim.set_spread_sigma(0.0); }
	else if(in_string(set_name, {"Sim", "05p002s"}, true)) { reader.sim.set_p_group(0.05); reader.sim.set_spread_sigma(0.002); }
	else if(in_string(set_name, {"Sim", "05p05s"}, true)) { reader.sim.set_p_group(0.05); reader.sim.set_spread_sigma(0.5); }
	else if(in_string(set_name, {"Sim", "15p002s"}, true)) { reader.sim.set_p_group(0.15); reader.sim.set_spread_sigma(0.002); }
	else { reader.sim.set_p_group(0.0); reader.sim.set_spread_sigma(0.0); }

	if(in_string(set_name, {"Sim", "Eff"}, true)) {
		reader.set_sim_eff(true);
	} else {
		reader.set_sim_eff(false);
	}
	if(in_string(set_name, {"Sim", "Eff", "Hole3-4"}, true)) {
		reader.set_sim_eff_dist_path(in_base_path + "Sim_Efficiency_Hists.root", "Hole_3to4");
	}
	if(in_string(set_name, "Sim")) {
		reader.sim.set_hom_eff(1.0);
		reader.set_sim_proton_dist_dataset(in_base_path + "Data_Old_Ref3/eta050/");
	}
	if(in_string(set_name, {"Sim", "Flow"}, true)) {
		reader.set_sim_flow(true);
		if(in_string(set_name, {"08res", "05v2"}, true)) { reader.sim.set_flow(0.05, 0.8, 0.0001); }
		else if(in_string(set_name, {"05res", "05v2"}, true)) { reader.sim.set_flow(0.05, 0.5, 0.0001); }
		else if(in_string(set_name, {"099res", "05v2"}, true)) { reader.sim.set_flow(0.05, 0.99, 0.0001); }
		else if(in_string(set_name, "05v2")) { reader.sim.set_flow(0.05, 0.0, 0.0001); }
	} else {
		reader.set_sim_flow(false);
	}

	if(in_string(set_name, "Ampt")) {
//		reader.ampt_cent = AmptCentralityMaker("/media/dylan/SSD_Storage/Research/Trees_Ampt/" + to_string(energy) + "GeV_Cent/", "ref" + to_string(ref));
//		map<int, float> opt_bmax {{7, 14.75}, {11, 14.5312}, {19, 14.3438}, {27, 14.2969}, {39, 13.5312}, {62, 13.6094}};
//		reader.ampt_cent.set_max_b(opt_bmax[energy]);
	}

	if(in_string(set_name, "p-")) { reader.cut.charge = -1; }
	if(in_string(set_name, "ptotal")) { reader.set_check_charge(false); }

	reader.set_mixed_sets(false);
	reader.set_rand_data(false);

	reader.set_mixed(true);
	reader.mix.set_divs(divs);
	reader.mix.set_out_path(mix_out_dir+set_dir);
	reader.mix.set_max_events(250);
	reader.mix.set_min_events(150);
	if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
	else { reader.mix.set_mixes_per_event(10); }
	if(in_string(set_name, "Sim") || in_string(set_name, "Ampt")) { reader.mix.set_mixes_per_event(10); }

	if(in_string(set_name, "Sim")) {
		reader.sim_events(sim_cent_events);
	} else if(in_string(set_name, "Ampt")) {
		reader.read_ampt_trees();
	} else{
		reader.read_trees();
	}
}


void ampt_cent_test() {
	int energy = 7;
	AmptCentralityMaker cent_maker("/media/dylan/SSD_Storage/Research/Trees_Ampt/" + to_string(energy) + "GeV_Cent/", "ref3");
	map<int, float> opt_bmax {{7, 14.75}, {11, 14.5312}, {19, 14.3438}, {27, 14.2969}, {39, 13.5312}, {62, 13.6094}};
	cent_maker.set_max_b(opt_bmax[energy]);
	cout << cent_maker.get_cent_bin9(200) << endl;
	vector<int> cent_ref3_9bin_edges;
	for(auto edge:cent_maker.get_ref_bin20_edges()) { cout << edge << ", "; }
	cout << endl;
	for(auto edge:cent_maker.get_ref_bin16_edges()) { cout << edge << ", "; }
}


void ampt_cent_b_corr() {
	vector<int> energies {7, 11, 19, 27, 39, 62};
	int cent = 8;
	for(int energy:energies) {
		string in_path = "/media/dylan/SSD_Storage/Research/Trees_Ampt/" + to_string(energy) + "GeV_Cent/";
		AmptCentralityMaker cent_maker(in_path, "ref3");
		map<int, float> opt_bmax {{7, 14.75}, {11, 14.5312}, {19, 14.3438}, {27, 14.2969}, {39, 13.5312}, {62, 13.6094}};
		cent_maker.set_max_b(opt_bmax[energy]);

		string ampt_path = "/media/dylan/SSD_Storage/Research/Trees_Ampt/" + to_string(energy) + ".root";

		TFile *ampt_file = new TFile(ampt_path.data(), "READ");
		TTree *ampt_tree = (TTree*)ampt_file->Get("tree");
		TH1D *ref3_dist = new TH1D(("ref3_dist_"+to_string(energy)+"_"+to_string(cent)).data(), "Ref3 Distribution", 801, -0.5, 800.5);
		ref3_dist->SetLineColor(kBlue);
		TH1D *b_dist = new TH1D(("b_dist_"+to_string(energy)+"_"+to_string(cent)).data(), "Impact Parameter Distribution", 201, -0.05, 20.05);
		b_dist->SetLineColor(kRed);
		TLeaf *ref3_leaf = ampt_tree->GetLeaf("refmult3");
		TLeaf *b_leaf = ampt_tree->GetLeaf("imp");
		int event_index = 0;
		while(ampt_tree->GetEntry(event_index)) {
			if(cent_maker.get_cent_bin9(ref3_leaf->GetValue()) == cent) {
				ref3_dist->Fill(ref3_leaf->GetValue());
				b_dist->Fill(b_leaf->GetValue());
			}
			event_index++;
		}

		TFile *out = new TFile("/home/dylan/Research/Results/ampt_cent_b_corr.root", "UPDATE");
		ref3_dist->Write();
		b_dist->Write();
		ampt_file->Close();
		out->Close();
	}
}


void res_calc() {
	int events = 1000;
	double res = 0.8;

	double flow_res = 0.0;
	int flow_res_n = 0;

	TH1D *ep_dist = new TH1D("ep_dist", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -M_PI, M_PI);
	for(int bin = 0; bin <= ep_dist->GetXaxis()->GetNbins(); bin++) {
		ep_dist->SetBinContent(bin, Simulator::event_plane_gang(res, ep_dist->GetBinCenter(bin)));
	}

	TRandom3 sim_rand(0);

	for(int i=0; i<events; i++) {
		double reaction_plane = M_PI*sim_rand.Rndm();
		double deviation = ep_dist->GetRandom();
		double event_plane = reaction_plane + deviation;
		flow_res += cos(2*(event_plane - reaction_plane));
		flow_res_n++;
	}

	if(flow_res_n > 0) { cout << "Target res: " << res << " Actual res: " << flow_res / flow_res_n << endl; }
}


void res_calc_debug() {
	int events = 1000;
	double res = 0.8;
//	double acc = 0.0001;

	double flow_res = 0.0;
	int flow_res_n = 0;

//	TH1D dist(("ep_dist_ex_res"+to_string(res)).data(), ("#splitline{Event Plane Pdf}{resolution="+to_string(res)+"}").data(), 1001, -M_PI, M_PI);
	TH1D *ep_dist = new TH1D("ep_dist", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -M_PI, M_PI);
	ep_dist->SetDirectory(0);
	for(int bin = 0; bin <= ep_dist->GetXaxis()->GetNbins(); bin++) {
		ep_dist->SetBinContent(bin, Simulator::event_plane_gang(res, ep_dist->GetBinCenter(bin)));
	}

	TRandom3 sim_rand(0);

	for(int i=0; i<events; i++) {
		double reaction_plane = 2*M_PI*sim_rand.Rndm();
		double deviation = ep_dist->GetRandom();
		double event_plane = reaction_plane + deviation;
//		event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
//		if(event_plane < 0) { event_plane += M_PI; }
//		while(event_plane >= 2*M_PI) { event_plane -= 2*M_PI; }
//		while(event_plane < 0) { event_plane += 2*M_PI; }
		flow_res += cos(2*(event_plane - reaction_plane));
		flow_res_n++;
	}

	if(flow_res_n > 0) { cout << "Target res: " << res << " Actual res: " << flow_res / flow_res_n << endl; }
}


void res_plot() {

	TFile file("/home/dylan/Research/Results/event_plane_dist_ex.root", "RECREATE");
	Simulator sim;
	vector<double> res_vec {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.95, 0.98, 0.99, 0.999};
	double acc = 0.0001;
	gStyle->SetOptStat(0);
	gStyle->SetPadTopMargin(0.13);

	for(auto res:res_vec) {

		TCanvas can(("ep_dist_ex_can_res"+to_string(res)).data(), ("Event Plane Pdf res = "+to_string(res)).data());
		TH1D dist(("ep_dist_ex_res"+to_string(res)).data(), ("#splitline{Event Plane Pdf}{resolution="+to_string(res)+"}").data(), 1001, -M_PI/2, M_PI/2);
		TH1D dist_gang(("ep_dist_ex_gang_res"+to_string(res)).data(), ("#splitline{Event Plane Pdf}{resolution="+to_string(res)+"}").data(), 1001, -M_PI/2, M_PI/2);
		dist.SetLineColor(kRed);
		dist.GetXaxis()->SetTitle("Azimuthal Angle (radians)");
		dist.GetYaxis()->SetTitle("Event Plane Probability");
//		gStyle->SetPadTopMargin(0.1);

		for(int bin = 0; bin <= dist.GetXaxis()->GetNbins(); bin++) {
			dist.SetBinContent(bin, sim.event_plane(res, 2*dist.GetBinCenter(bin), acc));
			dist_gang.SetBinContent(bin, sim.event_plane_gang(res, 2*dist.GetBinCenter(bin)));
		}

		TLine react_plane(0, 0, 0, dist_gang.GetMaximum());
		react_plane.SetLineColor(kRed);

		dist.GetYaxis()->SetRange(0, 1.05*dist_gang.GetMaximum());
		dist.GetYaxis()->SetRangeUser(0, 1.05*dist_gang.GetMaximum());

		cout << "Gang's Distribution integral: " << dist.Integral("width") << endl;
		cout << "Modified Distribution integral: " << dist_gang.Integral("width") << endl;

		dist.Draw("HIST");
		dist_gang.Draw("HISTSAMES");
		react_plane.Draw("SAME");

		TLegend leg;
		leg.AddEntry(&react_plane, "Reaction Plane", "l");
		leg.SetBorderSize(0);
		leg.Draw();

		dist.Write();
		dist_gang.Write();
		can.Write();
	}

	file.Close();

//	cout << "Gang's Function chi: " << sim.chi(res) << endl;
//	cout << "Modified Function chi: " << sim.chi2(res, acc) << endl;
//
//	cout << endl;
//
//	cout << "Gang's Function event plane: " << sim.event_plane(res, max_psi) << endl;
//	cout << "Modified Function event plane: " << sim.event_plane2(res, max_psi, acc) << endl;
}


void speed_test_class() {
	string out_dir = "/media/dylan/SSD_Storage/Research/Data_Ref2/";  // "/media/dylan/SSD_Storage/Research/Data_Ref3/"; //"/home/dylan/Research/Data/";
	int energy = 11;
	int ref = 2;

	ROOT::EnableThreadSafety();

	TreeReader reader(energy, ref);
	reader.set_in_path("/media/dylan/SSD_Storage/Research/Trees_Ref2/");
	reader.set_out_path(out_dir+"Single_Ratio0/");
	reader.read_trees();

////	map<string, pair<int, int>> set_pairs = {{"No_Rotate",{0,4}}, {"Rand_Rotate",{0,4}}, {"EP_Rotate",{0,4}}, {"Pile_Up_01_",{0,4}}, {"Pile_Up_008_",{0,4}}, {"Pile_Up_005_",{0,4}}, {"Pile_Up_002_",{0,4}}, {"Pile_Up_001_",{0,4}}, {"Efficiency_08_",{0,4}}, {"Efficiency_05_",{0,4}}, {"Efficiency_025_",{0,4}}, {"Efficiency_01_",{0,4}}};
////	map<string, pair<int, int>> set_pairs = {{"Rand_Rotate",{0,4}}, {"Pile_Up_01_",{0,4}}, {"Pile_Up_008_",{0,4}}, {"Pile_Up_005_",{0,4}}, {"Pile_Up_002_",{0,4}}, {"Pile_Up_001_",{3,4}}};
////	map<string, pair<int, int>> set_pairs = {{"No_BTof_Rej",{0,8}}, {"Pile_Up_0008_",{0,8}}, {"Pile_Up_0005_",{0,8}}, {"Pile_Up_0002_",{0,8}}};
//	map<string, pair<int, int>> set_pairs = {{"Single_Ratio",{0,0}}};
//	int sleep = 60;
//
//	ROOT::EnableThreadSafety();
//	{
//		ThreadPool pool(1); //thread::hardware_concurrency()
//		for(pair<string, pair<int, int>> set_pair:set_pairs) {
//			for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
//				string set_dir = set_pair.first + to_string(set_num) + "/";
//				cout << endl << "Queueing " + set_dir <<  "  set_num: " << set_num << endl << endl;
//				vector<int> energy_list = {11, 7}; //{7, 39, 27, 62, 19, 11};
//				vector<int> divs = {2, 3, 4, 5, 6};
//				for(int energy:energy_list) {
//					TreeReader reader(energy, 2);
//					reader.set_divs(divs);
//					reader.set_cbwc(false);
//					reader.set_cent_binning(9);
////					reader.set_ref_num(2);
//
//					reader.set_in_path("/media/dylan/SSD_Storage/Research/Trees_Ref2/");
//					reader.set_out_path(out_dir+set_dir);
//					reader.set_qa_path(out_dir+set_dir+to_string(energy)+"GeV/");
//					reader.set_qa_name("QA_");
//					reader.set_set_name(set_pair.first + to_string(set_num));
//
//					if(set_pair.first == "EP_Rotate")  { reader.set_event_plane(true); }
//					else { reader.set_event_plane(false); }
//
//					if(set_pair.first == "No_Rotate" || set_pair.first == "EP_Rotate") { reader.set_rotate_random(false); }
//					else{ reader.set_rotate_random(true); }
//
//					if(set_pair.first == "Pile_Up_01_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.01); }
//					else if(set_pair.first == "Pile_Up_008_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.008); }
//					else if(set_pair.first == "Pile_Up_005_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.005); }
//					else if(set_pair.first == "Pile_Up_002_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.002); }
//					else if(set_pair.first == "Pile_Up_001_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.001); }
//					else if(set_pair.first == "Pile_Up_0008_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.0008); }
//					else if(set_pair.first == "Pile_Up_0005_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.0005); }
//					else if(set_pair.first == "Pile_Up_0002_") { reader.set_pile_up(true); reader.set_pile_up_prob(0.0002); }
//					else { reader.set_pile_up(false); reader.set_pile_up_prob(0); }
//
//					if(set_pair.first == "No_BTof_Rej") {
//						reader.cut.max_slope = {{7, 999}, {11, 999}, {14, 999}, {19, 999}, {27, 999}, {39, 999}, {62, 999}, {200, 999}};
//						reader.cut.min_slope = {{7, -999}, {11, -999}, {14, -999}, {19, -999}, {27, -999}, {39, -999}, {62, -999}, {200, -999}};
//
//					}
//
//					if(set_pair.first == "Single_Ratio") { reader.set_single_ratio(true); reader.mix.set_single_ratio(true); }
//
//					if(set_pair.first == "Efficiency_08_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.08); }
//					else if(set_pair.first == "Efficiency_05_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.05); }
//					else if(set_pair.first == "Efficiency_025_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.025); }
//					else if(set_pair.first == "Efficiency_01_") { reader.set_efficiency(true); reader.set_efficiency_prob(0.01); }
//					else { reader.set_efficiency(false); reader.set_efficiency_prob(0); }
//
//					reader.set_mixed_sets(false);
//					reader.set_rand_data(false);
//
//					reader.set_mixed(false);
//					reader.mix.set_divs(divs);
//					reader.mix.set_out_path(mix_out_dir+set_dir);
//					reader.mix.set_max_events(250);
//					reader.mix.set_min_events(150);
//					if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
//					else { reader.mix.set_mixes_per_event(10); }
//					reader.read_trees();
////					pool.enqueue(&TreeReader::read_trees, reader);
//				}
//				this_thread::sleep_for(chrono::seconds(sleep));
//			}
//		}
//	}
}


void read_comb_sys() {
	string out_dir = "/home/dylan/Research/Data/";
	string mix_sets_out_dir = "/home/dylan/Research/Data_Mix_Sets/";
	string mix_out_dir = "/home/dylan/Research/Data_Mix/";
	string random_out_dir = "/home/dylan/Research/Data_Random/";

	map<string, pair<int, int>> set_pairs = {{"Final_Set",{0,100}}};
	pair<float, float> pile_up_range = {0.0, 0.01};
	pair<float, float> efficiency_range = {0.0, 0.05};
	TRandom3 sys_rand(0);
	int sleep = 60;

	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		for(pair<string, pair<int, int>> set_pair:set_pairs) {
			for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
				string set_dir = set_pair.first + to_string(set_num) + "/";
				cout << endl << "Queueing " + set_dir <<  "  set_num: " << set_num << endl << endl;
				vector<int> energy_list = {39, 27, 62, 19, 11, 7};
				vector<int> divs = {2, 3, 4, 5, 6};
				for(int energy:energy_list) {
					TreeReader reader(energy);
					reader.set_divs(divs);
					reader.set_cbwc(false);
					reader.set_cent_binning(9);

					reader.set_in_path("/home/dylan/Research/Trees/");
					reader.set_out_path(out_dir+set_dir);
					reader.set_qa_path(out_dir+set_dir+to_string(energy)+"GeV/");
					reader.set_qa_name("QA_");
					reader.set_set_name(set_pair.first + to_string(set_num));

					reader.set_event_plane(false);
					reader.set_rotate_random(true);

					reader.set_pile_up(true);
					reader.set_pile_up_prob(sys_rand.Rndm() * (pile_up_range.second - pile_up_range.first) + pile_up_range.second);

					reader.set_efficiency(true);
					reader.set_efficiency_prob(sys_rand.Rndm() * (efficiency_range.second - efficiency_range.first) + efficiency_range.second);

					reader.set_mixed_sets(false);
					reader.set_rand_data(false);

					reader.set_mixed(true);
					reader.mix.set_divs(divs);
					reader.mix.set_out_path(mix_out_dir+set_dir);
					reader.mix.set_max_events(250);
					reader.mix.set_min_events(150);
					if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
					else { reader.mix.set_mixes_per_event(10); }
					pool.enqueue(&TreeReader::read_trees, reader);
				}
				this_thread::sleep_for(chrono::seconds(sleep));
			}
		}
	}
}


void ref_mult_test() {
	StRefMultCorr *refmultCorrUtil;
	refmultCorrUtil = new StRefMultCorr("refmult3");
	refmultCorrUtil->init(11079000);
	refmultCorrUtil->print();
//	refmultCorrUtil->initEvent((int)event.get_refn(), (double)event.get_vz());
//	int cent_check = refmultCorrUtil->getCentralityBin9();
}
