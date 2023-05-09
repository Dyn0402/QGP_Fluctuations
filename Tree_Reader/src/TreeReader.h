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

#include "sys/types.h"
//#include "sys/sysinfo.h"

#include <TTree.h>
#include <TLeaf.h>
#include <TH2.h>
#include <TRandom3.h>
#include <TChain.h>

#include "ratio_methods.h"

#include "AzBinner.h"
#include "AmptCentralityMaker.h"
#include "TreeBranches.h"

using namespace std;

extern const std::string platform;


class TreeReader {
public:
	// Structors
	TreeReader(int energy, int ref_num);
	TreeReader(int energy, int ref_num, mutex* mtx);
	TreeReader(int energy);
	TreeReader(int energy, mutex *mtx);
	TreeReader();
	~TreeReader();

	// Getters
	string get_in_path();
	string get_out_path();
	string get_tree_name();
	string get_event_cut_hist_name();
	string get_track_cut_hist_name();
	string get_particle();

	// Setters
	void set_in_path(string path);
	void set_out_path(string path);
	void set_qa_path(string path);
	void set_ampt_cent_path(string path);
	void set_ampt_type(string type);
	void set_tree_name(string tree_name);
	void set_sim_proton_dist_dataset(string path);
	void set_sim_eff_dist_path(string root_path, string hist_name);
	void set_energy(int energy);
	void set_ampt(bool ampt);
	void set_cooper_frye(bool cf);
	void set_pile_up(bool pile_up);
	void set_sim_eff(bool sim_eff);
	void set_sim_flow(bool sim_flow);
	void set_pile_up_prob(double pile_up_prob);
	void set_particle(string particle);
//	void set_particle_dist_hist_max(int max);
	void set_ampt_particle_pid(vector<int> pid);
	void set_file_list(vector<string> *file_list);

	void set_tree_reader_rand_seed(int seed=0);
	void set_file_shuffle_rand_seed(int seed=0);

	AzBinner& add_binner();

	// Doers
	void read_trees();
	void sim_events(map<int, int> cent_num_events);

	// Attributes
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;

	vector<AzBinner> binners;

	TreeCuts cut;  // Just for AMPT default values
	event_defaults event_defs;  // Just for AMPT default values
	track_defaults track_defs;  // Just for AMPT default values
	AmptCentralityMaker ampt_cent;

	Simulator sim;

private:
	// Attributes
	mutex *mtx = NULL;
	//struct sysinfo mem_info;
	vector<string> *file_list;
	int file_wait_sleep = 60;  // Seconds to wait for path to clear up
	float percent_print = 2.0;  // Percentage of files interval at which to print update

	TRandom3* trand = new TRandom3(0);

	tree_branches branches;
	ampt_tree_branches ampt_branches;
	cooper_frye_tree_branches cooper_frye_branches;

	string tree_name = "tree";
	string event_cut_hist_name = "Event Cut Hist";
	string track_cut_hist_name = "Track Cut Hist";
	string de_dx_pq_hist_name = "dedx_pq_pid";
	string beta_pq_hist_name = "beta_pq_pid";

	string in_path = "/home/dylan/Research/Trees/";
	string out_path = "/home/dylan/local_server/dyn0402/Research/Data3/";
	string qa_path = "/home/dylan/local_server/dyn0402/Research/QA/";
	string ampt_cent_path = "/home/dylan/Research/Ampt_Centralities/";
	string ampt_type = "string_melting";

	//string sim_proton_dist_dataset = "/media/dylan/SSD_Storage/Research/Data_Old_Ref2/Single_Ratio0/";
	string sim_proton_dist_dataset = "F:/Research/Data/default_resample/rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_0/";
	string sim_dist_qa_name = "QA_";
	vector<string> sim_eff_dist_path {};

	int energy;
	int ref_num;
	string particle = "proton";
	vector<int> pdg_particle_pid{ 2212 };

	int tree_reader_seed = 0;
	int file_shuffle_seed = -1;

	bool ampt;
	bool cooper_frye;
	bool pile_up;
	bool sim_eff;
	bool sim_flow;

	double pile_up_prob;

	//int particle_dist_hist_max = 100;

	// Doers
	void read_tree(TTree* tree);
	void read_ampt_tree(TTree* tree);
	void read_cooper_frye_tree(TTree* tree);
	void read_tree_debug(TTree* tree);

	void set_branches(TTree* tree);
	void set_ampt_branches(TTree* tree);
	void set_cooper_frye_branches(TTree* tree);

	void set_binner(AzBinner& binner);

	TH1D* get_sim_proton_dist(int cent);
	TH1D* get_sim_efficiency_dist();
};


double get_available_memory_lin();


#endif /* TREEREADER_H_ */
