/*
 * TreeReader.h
 *
 *  Created on: Jul 18, 2019
 *      Author: dylan
 */

#ifndef TREEREADER_H_
#define TREEREADER_H_

#include <vector>
#include <map>
#include <ctime>
#include <chrono>
#include <mutex>

#include <TTree.h>
#include <TLeaf.h>
#include <TH2.h>
#include <TRandom3.h>
#include <TChain.h>

#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"

#include "ratio_methods.h"

#include "Event.h"
#include "Track.h"

#include "AmptCentralityMaker.h"
#include "Mixer.h"
#include "MixerSets.h"
#include "Randomizer.h"
#include "Simulator.h"
#include "TreeBranches.h"
#include "TreeCuts.h"

using namespace std;


//struct cut_values {
//	// Event cuts
//	vector<int> bad_runs = {11116006, 11116017, 11116055, 11117094, 11120084, 11122060, 11122080, 11123050, 11123056, 11125099, 11125102, 11126022, 11126068, 11127007, 11127078, 11129047, 11130043, 11130064, 11131027, 11132025, 11132058, 11132067, 11133085, 11134019, 11134053, 11134066, 11135025, 11136059, 11136077, 11137111, 11137114, 11138005, 11138047, 11138061, 11138066, 11138082, 11141019, 11142008, 11142018, 11142036, 11143027, 11143047, 11145101, 11148001, 11148006, 11148014, 11148045, 11148055, 11149001, 11149010, 11149015, 11149017, 11149047, 11150029, 11151050, 11151051, 11152036, 11153032, 11153042, 11154026, 11156003, 11156009, 11156036, 11156043, 11156044, 11156045, 12114007, 12114092, 12114103, 12115014, 12115015, 12115016, 12115018, 12115019, 12115020, 12115022, 12115023, 12115073, 12120026, 12121017, 12121022, 12121034, 12122019, 12172049, 12172050, 12172051, 12173031, 12173034, 12174077, 12174085, 12175030, 12175040, 12175062, 12176044, 12176047, 12176067, 12176069, 12176104, 12177015, 12177101, 12178051, 12178120, 12179068, 11002145, 11003001, 11003002, 11003004, 11003005, 11003006, 11003012, 11003013, 11023046, 11023048, 11025034, 11025054, 11026010, 11026114, 11028004, 11028005, 11028006, 11028018, 11028019, 11028020, 11028021, 11028022, 11030041, 11030042, 11057012, 11057035, 11059060, 11060059, 11064003, 11070051, 11070052, 11070053, 11070054, 11070055, 11070056, 11070057, 11071001, 11071002, 11071003, 11071004, 11071006, 11071007, 11071013, 11071014, 11071015, 11071016, 11071017, 11071018, 11071019, 11071020, 11072030, 11072031, 11072032, 11072044, 11072045, 11073001, 11073003, 11073030, 11073031, 11080072, 11084009, 11084010, 11087001, 11087002, 11087003, 11087004, 11087057, 11087058, 11087059, 11089026, 11089028, 11089029, 11089048, 11089055, 11092012, 11092031, 11092032, 11092033, 11092034, 11095040, 11095048, 11095049, 11095050, 11095051, 11095061, 11095062, 11095063, 11095064, 15047023, 15047065, 15047071, 15047087, 15049027, 15051131, 15053034, 15053035, 15053052, 15053053, 15053054, 15053055, 15053056, 15053057, 15061001, 15066064, 15067026, 11099124, 11100045, 11101030, 11101046, 11101104, 11102054, 11102098, 11103008, 11103009, 11103058, 11103062, 11105011, 11105018, 11105029, 11105045, 11106026, 11106027, 11106028, 11106029, 11106030, 11106039, 11106040, 11106041, 11106049, 11109013, 11109088, 11109090, 11109092, 11109097, 11109098, 11109102, 11109103, 11109104, 11110034, 11110041, 11110073, 11110076, 0, 0};
//	int min_multi = 2; //Need this many particles or more to be a good event.
//	map<int, double> max_slope = {{7, (double)(1565-187)/(310+6)}, {11, (double)(1274-172)/(250+6)}, {14, (double)(1954-240)/(350+12)}, {19, (double)(1596-195)/(364+11)}, {27, (double)(1878-207)/(400+18)}, {39, (double)(2220-128)/(415+20)}, {62, (double)(3700-116)/(544+22)}, {200, (double)(4376-142)/(543+10)}};
//	map<int, double> min_slope = {{7, (double)(924+34)/(418-7)}, {11, (double)(660+20)/(307-7)}, {14, (double)(1020+61)/(396-10)}, {19, (double)(1049+35)/(426-5)}, {27, (double)(1211+39)/(460-6)}, {39, (double)(1563+103)/(524-10)}, {62, (double)(2657+244)/(649-51)}, {200, (double)(3130+77)/(616-70)}};
//
//	// Track cuts
//	double min_p = 0.15;
//	double max_p = 0.8;
//	double min_pt = 0.4;
//	double max_pt = 2.0;
//	double min_beta = 0.0;
//	double max_beta = 1.0;
//	int charge = 1;
//	double min_eta = -0.5;
//	double max_eta = 0.5;
//	double min_nsigma = -2.0;
//	double max_nsigma = 2.0;
//	double min_dca = 0.0;
//	double max_dca = 1.0;
//	double min_m2 = 0.8;
//	double max_m2 = 1.0;
//	double min_pt_for_m = 0.8;
//	double max_pt_for_m = 2.0;
//};


class TreeReader {
public:
	// Structors
	TreeReader(int energy, int ref_num);
	TreeReader(int energy, int ref_num, mutex *mtx);
	TreeReader(int energy);
	TreeReader(int energy, mutex *mtx);
	TreeReader();
	~TreeReader();

	// Getters
	string get_in_path();
	string get_out_path();
	string get_qa_path();
	string get_set_name();
	string get_tree_name();
	string get_event_cut_hist_name();
	string get_track_cut_hist_name();
	bool get_cbwc();
	bool get_rotate_random();
	bool get_event_plane();
	bool get_mixed_sets();
	bool get_mixed();
	bool get_rand_data();
	bool get_pile_up();
	bool get_efficiency();
	bool get_single_ratio();
	double get_pile_up_prob();
	double get_efficiency_prob();
	int get_cent_binning();
	int get_ref_num();

	// Setters
	void set_in_path(string path);
	void set_out_path(string path);
	void set_qa_path(string path);
	void set_qa_name(string name);
	void set_set_name(string set_name);
	void set_tree_name(string tree_name);
	void set_sim_proton_dist_dataset(string path);
	void set_sim_eff_dist_path(string root_path, string hist_name);
	void set_energy(int energy);
	void set_divs(vector<int> list);
	void set_cbwc(bool cbwc);
	void set_rotate_random(bool rotate_random);
	void set_event_plane(bool event_plane);
	void set_mixed_sets(bool mixed);
	void set_mixed(bool mixed_roli);
	void set_rand_data(bool rand_data);
	void set_pile_up(bool pile_up);
	void set_efficiency(bool efficiency);
	void set_single_ratio(bool single_ratio);
	void set_n1_ratios(bool n1_ratios);
	void set_check_charge(bool check);
	void set_sim_eff(bool sim_eff);
	void set_sim_flow(bool sim_flow);
	void set_pile_up_prob(double pile_up_prob);
	void set_efficiency_prob(double efficiency_prob);
	void set_cent_binning(int cent_binning);
	void set_ref_num(int ref_num);
	void set_particle(string particle);
	void set_particle_dist_hist_max(int max);
	void set_ampt_particle_pid(vector<int> pid);

	// Doers
	void read_trees();
	void read_trees_chain();
	void read_ampt_trees();
	void read_nsm_ampt_trees();
	void sim_events(map<int, int> cent_num_events);
	void write_info_file();

	// Attributes
	TreeCuts cut;
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;
	MixerSets mix_sets;
	Mixer mix;
	Randomizer random;

	Simulator sim;
	AmptCentralityMaker ampt_cent;
	event_defaults event_defs;
	track_defaults track_defs;

private:
	// Attributes
	mutex *mtx = NULL;

	map<int, map<int, map<int, map<int, int>>>> data; //ratios[divisions][centrality][num particles in event][num particles in bin]
	StRefMultCorr *refmultCorrUtil;
	TRandom3 *trand = new TRandom3(0);
	tree_branches branches;

	string tree_name = "tree";
	string event_cut_hist_name = "Event Cut Hist";
	string track_cut_hist_name = "Track Cut Hist";
	string de_dx_pq_hist_name = "dedx_pq_pid";
	string beta_pq_hist_name = "beta_pq_pid";

	string qa_name = "QA_CBWC_";
	string info_file_name = "info.txt";
	string set_name = "";

	string in_path = "/home/dylan/Research/Trees/";
	string out_path = "/home/dylan/local_server/dyn0402/Research/Data3/";
	string qa_path = "/home/dylan/local_server/dyn0402/Research/QA/";

	string sim_proton_dist_dataset = "/media/dylan/SSD_Storage/Research/Data_Old_Ref2/Single_Ratio0/";
	vector<string> sim_eff_dist_path {};

	vector<int> divs = {180, 120, 90, 72, 60};
	int energy;

	string particle = "proton";
	vector<int> ampt_particle_pid {2212};

	bool cbwc; // MixerSets/randomizers aren't prepared for this to be false.
	bool rotate_random;
	bool event_plane;
	bool mixed_sets;
	bool mixed;
	bool rand_data;
	bool pile_up;
	bool efficiency;
	bool single_ratio;
	bool n1_ratios;
	bool sim_eff;
	bool sim_flow;
	bool check_charge;

	double pile_up_prob;
	double efficiency_prob;

	int cent_binning;
	int ref_num;

	int particle_dist_hist_max = 100;

	// Doers
	void read_tree(TTree* tree);
	void read_ampt_tree(TTree* tree);
	void read_nsm_ampt_tree(TTree* tree);
	void read_tree_debug(TTree* tree);
	void process_event(Event& event);
	void set_branches(TTree* tree);
	void set_branches(TChain* chain);

	TH1D* get_sim_proton_dist(int cent);
	TH1D* get_sim_efficiency_dist();

	bool check_event(Event& event);
	bool check_good_dca_xy(int run, int event_id);
	bool check_enough_particles(Event& event);
	bool check_good_run(int run);
	bool check_slope(int btof, int ref_mult);
	bool check_particle_good(Track& particle);

	void define_qa();
	void add_cut_hists(TFile* file);
	void fill_pre_track_qa(Track& particle);
	void fill_post_track_qa(Track& particle);
	void fill_pre_event_qa(Event& event);
	void fill_post_event_qa(Event& event);
	void write_qa();

	void reset_out_dir();

	// QA Plots
	TH2I cent_hist;
	TH2I btof_ref_hist;

	TH2F de_dx_pq_hist;
	TH2F beta_pq_hist;

	TH1D event_cut_tree_maker;
	TH1D track_cut_tree_maker;
	TH1D event_cut_hist;
	TH1D track_cut_hist;
	TH1I cent16_events;
	TH1I cent9_events;

	TH1I pre_run_hist;
	TH1I pre_vx_hist;
	TH1I pre_vy_hist;
	TH1I pre_vz_hist;
	TH1I pre_ref_hist;
	TH1I pre_refn_hist;
	TH1I pre_btof_hist;
//	TH1I pre_ep_hist;

	TH1I post_run_hist;
	TH1I post_vx_hist;
	TH1I post_vy_hist;
	TH1I post_vz_hist;
	TH1I post_ref_hist;
	TH1I post_refn_hist;
	TH1I post_btof_hist;
	TH1I post_ep_hist;

	TH1D pre_phi_hist;
	TH1I pre_p_hist;
	TH1I pre_pt_hist;
	TH1I pre_beta_hist;
	TH1I pre_charge_hist;
	TH1I pre_eta_hist;
	TH1I pre_nsigma_hist;
	TH1I pre_dca_hist;

	TH1D post_phi_hist;
	TH1I post_p_hist;
	TH1I post_pt_hist;
	TH1I post_beta_hist;
	TH1I post_charge_hist;
	TH1I post_eta_hist;
	TH1I post_nsigma_hist;
	TH1I post_dca_hist;

	TH1I pre_m2_hist;
	TH1I post_m2_hist;

	map<int, TH1D> post_n_particles;

};



#endif /* TREEREADER_H_ */
