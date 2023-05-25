#pragma once

#include <vector>
#include <map>
#include <ctime>
#include <chrono>
#include <mutex>
#include <random>

#include <TTree.h>
#include <TLeaf.h>
#include <TH2.h>
#include <TProfile.h>
#include <TGraphErrors.h>
#include <TRandom3.h>
#include <TChain.h>

#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"

#include "ratio_methods.h"

#include "Event.h"
#include "Track.h"

#include "AmptCentralityMaker.h"
#include "Mixer.h"
#include "Simulator.h"
#include "TreeBranches.h"
#include "TreeCuts.h"


class AzBinner {
public:
	// Structors
	AzBinner(int energy, int ref_num, mutex *mtx);
	AzBinner(int energy, int ref_num);
	AzBinner(int energy);
	AzBinner();
	~AzBinner();

	// Getters
	string get_in_path();
	string get_out_path();
	string get_qa_path();
	string get_set_name();
	string get_event_cut_hist_name();
	string get_track_cut_hist_name();
	bool get_cbwc();
	bool get_rotate_random();
	bool get_event_plane();
	bool get_mixed();
	bool get_pile_up();
	bool get_efficiency();
	bool get_single_ratio();
	double get_pile_up_prob();
	double get_efficiency_prob();
	int get_cent_binning();
	int get_ref_num();
	int get_cent_bins();
	int get_cent_min();
	int get_particle_bins();
	int get_particle_min();
	int get_n_bootstraps();

	// Setters
	void set_in_path(string path);
	void set_out_path(string path);
	void set_qa_path(string path);
	void set_ampt_cent_path(string path);
	void set_ampt_type(string type);
	void set_qa_name(string name);
	void set_set_name(string set_name);
	void set_sim_proton_dist_dataset(string path);
//	void set_sim_eff_dist_path(string root_path, string hist_name);
	void set_energy(int energy);
	void set_divs(vector<int> list);
	void set_ampt(bool ampt);
	void set_cooper_frye(bool cf);
	void set_ampt_reaction_plane(bool rp);
	void set_cbwc(bool cbwc);
	void set_rotate_random(bool rotate_random);
	void set_event_plane(bool event_plane);
	void set_mixed(bool mixed);
	void set_pile_up(bool pile_up);
	void set_efficiency(bool efficiency);
	void set_single_ratio(bool single_ratio);
	void set_n1_ratios(bool n1_ratios);
	void set_check_charge(bool check);
	void set_sim_eff(bool sim_eff);
	void set_sim_flow(bool sim_flow);
	void set_rapidity(bool rapidity);
	void set_resample(bool resample);
	void set_prerotate(bool prerotate);
	void set_calc_v2(bool calc_v2);
	void set_pile_up_prob(double pile_up_prob);
	void set_efficiency_prob(double efficiency_prob);
	void set_cent_binning(int cent_binning);
	void set_ref_num(int ref_num);
	void set_resample_alg(int alg_num);
	void set_n_resamples(int n);
	void set_n_bootstraps(int n);
	void set_particle(string particle);
	void set_particle_dist_hist_max(int max);
	void set_ampt_particle_pid(vector<int> pid);
	void set_sim_pars(simulation_pars pars);

	void set_tree_reader_rand_seed(int seed = 0);
	void set_mixer_rand_seed(int seed = 0);
	void set_file_shuffle_rand_seed(int seed = 0);
	void set_stref_rand_seed(int seed = 0);

	void set_cent_bins(int bins);
	void set_cent_min(int min);
	void set_particle_bins(int bins);
	void set_particle_min(int min);

	// Doers
	void prep_read();
	void define_qa();
	void add_cut_hists(TFile* file);
	void write_info_file();
	void reset_out_dir();
	void write_qa();
	void write_binner_data();
	void process_event(const Event& event);
	void process_event_debug(const Event& event);
	void process_event_pt_n(const Event& event);

	// Attributes
	TreeCuts cut;
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;
	Mixer mix;

	AmptCentralityMaker ampt_cent;

protected:
	// Doers
	TH1D* get_sim_proton_dist(int cent);
	TH1D* get_sim_efficiency_dist();

	bool check_event(const Event& event);
	bool check_good_dca_xy(int run, int event_id);
	bool check_enough_particles(const Event& event);
	bool check_good_run(int run);
	bool check_pile_up(int btof_multi, int btof_match, int ref_mult);
	bool check_particle_good(const Track& particle);

	void init_data();

	void gen_single_randoms(vector<double>& random_list, vector<double>& random_bs_list);

	void fill_pre_track_qa(const Track& particle);
	void fill_post_track_qa(const Track& particle);
	void fill_pre_event_qa(const Event& event);
	void fill_post_event_qa(const Event& event);

private:
	// Attributes
//	map<int, map<int, map<int, vector<long>>>> data; //ratios[divisions][centrality][num particles in event][num particles in bin]
//	map<int, map<int, map<int, map<int, vector<long>>>>> data_bs; //ratios[divisions][centrality][bootstrap #][num particles in event][num particles in bin]
	vector<vector<vector<vector<long>>>> data; //ratios[divisions][centrality][num particles in event][num particles in bin]
	vector<vector<vector<vector<vector<long>>>>> data_bs; //ratios[divisions][centrality][bootstrap #][num particles in event][num particles in bin]
	StRefMultCorr* refmultCorrUtil;
	TRandom3* trand = new TRandom3(0);
	mutex *mtx = NULL;
//	mt19937_64 c_rand;
//	poisson_distribution<int> pois_dist;

	tree_branches branches;
	ampt_tree_branches ampt_branches;
	simulation_pars sim_pars;

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
	string ampt_cent_path = "/home/dylan/Research/Ampt_Centralities/";
	string ampt_type = "string_melting";

	string sim_proton_dist_dataset = "/media/dylan/SSD_Storage/Research/Data_Old_Ref2/Single_Ratio0/";
//	vector<string> sim_eff_dist_path{};

	vector<int> divs = { 180, 120, 90, 72, 60 };
	int energy;

	string particle = "proton";
	vector<int> ampt_particle_pid{ 2212 };

	int tree_reader_seed = 0;
	int mixer_seed = 0;
	int file_shuffle_seed = 0;
	int stref_seed = 0;

	int cent_bins = 10;
	int cent_min = -1;
	int particle_bins = 100;
	int particle_min = 1;

	bool ampt;
	bool cooper_frye;
	bool ampt_reaction_plane;
	bool cbwc;
	bool rotate_random;
	bool event_plane;
	bool mixed;
	bool pile_up;
	bool efficiency;
	bool single_ratio;
	bool n1_ratios;
	bool sim_eff;
	bool sim_flow;
	bool check_charge;
	bool rapidity;
	bool resample;
	bool prerotate;
	bool calc_v2;

	double pile_up_prob;
	double efficiency_prob;

	int cent_binning;
	int ref_num;

	int resample_alg = 4;  // Which resampling algorithm to use. 4 is default stochastic algorithm
	int n_resamples = 72;  // With stochastic resampling (algs 4,5) computational time increases linearly with #resamples
	int n_bootstraps = 250;

	int particle_dist_hist_max = 100;


	// QA Plots
	TH2I cent_hist;
	TH2I btof_multi_ref_hist;
	TH2I btof_match_ref_hist;

	TH2F de_dx_pq_hist;
	TH2F beta_pq_hist;

	TH2F eta_pt_hist;

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
	TH1I pre_btof_multi_hist;
	TH1I pre_btof_match_hist;
	TH1I pre_ep_hist;

	TH1I post_run_hist;
	TH1I post_vx_hist;
	TH1I post_vy_hist;
	TH1I post_vz_hist;
	TH1I post_ref_hist;
	TH1I post_refn_hist;
	TH1I post_btof_multi_hist;
	TH1I post_btof_match_hist;
	TH1I post_ep_hist;

	TH1D pre_phi_hist;
	TH1I pre_p_hist;
	TH1I pre_pt_hist;
	TH1I pre_beta_hist;
	TH1I pre_charge_hist;
	TH1I pre_eta_hist;
	TH1I pre_nsigma_hist;
	TH1I pre_dca_hist;
	TH1I pre_nhits_fit_hist;
	TH1I pre_rapid_hist;

	TH1D post_phi_hist;
	TH1I post_p_hist;
	TH1I post_pt_hist;
	TH1I post_beta_hist;
	TH1I post_charge_hist;
	TH1I post_eta_hist;
	TH1I post_nsigma_hist;
	TH1I post_dca_hist;
	TH1I post_nhits_fit_hist;
	TH1I post_rapid_hist;

	TH1I pre_m2_hist;
	TH1I post_m2_hist;

	map<int, TH1D> post_n_particles;
	map<int, TH1D> post_ref;
	map<int, TH1D> post_refn;
	map<int, TProfile> v2;
	//map<int, TGraphErrors> v2_cor;
	map<int, TProfile> resolution;
};
