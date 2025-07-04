/*
 * main.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <ctime>
#include <chrono>
//#include <unistd.h>
#include <cstdlib>
#include <mutex>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLine.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TF1.h>
#include <TH2.h>
#include <TMath.h>
#include <Math/IntegratorOptions.h>

#include "ThreadPool.h"
#include "TreeReader.h"
#include "AmptCentralityMaker.h"
#include "AmptConverter.h"
#include "DcaxyQAer.h"
#include "PileUpQAer.h"
#include "ratio_methods.h"
#include "file_io.h"

#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"
#include "TreeBranches.h"


using namespace std;


//void read_class();
void read_new();
void read_rcf_sim(string set_group, string set_name);
void read_single_job(int energy, string job_type, string set_group, string set_name, int job_num_low, int job_num_high);
//void read_comb_sys();
//void real_event_tree_test();
//void speed_test();
void res_plot();
void res_calc();
void res_calc_debug();
void speed_test_class();
void ref_mult_test();
void ampt_cent_opt();
void ampt_cent_make();
void ampt_ref_b_plot();
void dca_xy_qa(int energy, mutex *mtx, string in_path, string qa_path, string pile_up_qa_path, string variation);
void run_dca_xy_qa();
void pile_up_qa(int energy, mutex *mtx, string in_path, string qa_path, string variation);
void run_pile_up_qa();
void tchain_test();
void run_tchain();
void yunshan_example();
//void AMPT_Proton_Num_Check();
void get_flag_trail_test();
map<string, pair<int, int>> get_rand_set_pairs(int num_pairs, string rapid="rapid05", string efficiency="");
map<string, map<string, map<string, pair<int, int>>>> get_sets_from_dir(string path, int set_size=15);


void tree_read_speed();

//void run_set(int energy, int set_num, string set_name, int job_num, int jobs, mutex *mtx, vector<string> *file_list);
void run_job(int energy, map<string, map<string, pair<int, int>>> job, int job_num, string job_type, int jobs, mutex* mtx, vector<string>* file_list);
//void run_job_sim(int energy, map<string, map<string, pair<int, int>>> job, int job_num, string job_type, int jobs, mutex* mtx, vector<string>* file_list);

clock_t start = clock();
auto start_sys = chrono::system_clock::now();


int main(int argc, char** argv) {
	if (argc == 3) {  // Running on RCF
		gROOT->ProcessLine(".L Track.h");
		gROOT->ProcessLine(".L Event.h");
	}
	else if (platform == "lin") {
		gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Track.h");
		gROOT->ProcessLine(".L /home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/src/Event.h");
	} else if (platform == "win") {
		gROOT->ProcessLine(".L C:/Users/Dylan/source/repos/Dyn0402/QGP_Fluctuations/Tree_Reader/src/Track.h");
		gROOT->ProcessLine(".L C:/Users/Dylan/source/repos/Dyn0402/QGP_Fluctuations/Tree_Reader/src/Event.h");
	}

//	tree_read_speed();

	//TH2F btof_ref_hist("test", "test", 10, 0, 10, 10, 0, 10);
	//btof_ref_hist.Fill(5, 5, 10);
	//btof_ref_hist.Fill(3, 7, 3);
	//btof_ref_hist.Fill(1, 5, 6);
	//TF1 lin_fit("lin_fit_", "[0]*x", 0, 10);
	//btof_ref_hist.Fit(&lin_fit, "NQ");
	//double rot_angle = M_PI / 2 - atan(lin_fit.GetParameter(0));
	//cout << rot_angle << endl;
	//read_class();

	if (argc == 3) {
		read_rcf_sim((string)argv[1], (string)argv[2]);
		return 0;
	} else if (argc == 7) {
		read_single_job(stoi(argv[1]), (string)argv[2], (string)argv[3], (string)argv[4], stoi(argv[5]), stoi(argv[6]));
	}
	else {
		cout << "No commandline input, assume not rcf. Doing other things." << endl;
	}

//	read_new();
    yunshan_example();

//	run_dca_xy_qa();
//	run_pile_up_qa();
//	tchain_test();
	//ampt_ref_b_plot();
	//ampt_cent_opt();
//	ampt_cent_make();
//	ref_mult_test();
//	res_plot();
//	real_event_tree_test();
//	speed_test();
//	speed_test_class();
//	AMPT_Proton_Num_Check();

	cout << endl << "donzo" << endl;
	return(0);

}


void read_new() {
	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//		{"BES1_rapid05", {{"test1", {{"rapid02_n1ratios_dca05_nsprx05_m2r6_m2s0_nhfit25_", {0, 0}}}}}} };

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"BES1_def_0", {{"default", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {0, 14}}}}}},
	//	{"BES1_def_1", {{"default", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {15, 29}}}}}},
	//	{"BES1_def_2", {{"default", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {30, 44}}}}}},
	//	{"BES1_def_3", {{"default", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {45, 59}}}}}},
	//	{"BES1_def_sys_0", {{"default_sys", get_rand_set_pairs(15, "rapid05")}}},
	//	{"BES1_def_sys_1", {{"default_sys", get_rand_set_pairs(15, "rapid05")}}},
	//	{"BES1_def_sys_2", {{"default_sys", get_rand_set_pairs(15, "rapid05")}}},
	//	{"BES1_def_sys_3", {{"default_sys", get_rand_set_pairs(15, "rapid05")}}},
	//	{"BES1_def_sys_4", {{"default_sys", get_rand_set_pairs(15, "rapid05")}}},
	//	{"BES1_def_sys_5", {{"default_sys", get_rand_set_pairs(15, "rapid05")}}},
	//	{"BES1_def_sys_6", {{"default_sys", get_rand_set_pairs(15, "rapid05")}}},
	//	{"BES1_def_sys_7", {{"default_sys", get_rand_set_pairs(15, "rapid05")}}}
	//};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"BES1_rapid01_0", {{"rapid01_def", {{"rapid01_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {0, 14}}}}}},
	//	{"BES1_rapid02_0", {{"rapid02_def", {{"rapid02_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {0, 14}}}}}},
	//	{"BES1_rapid03_0", {{"rapid03_def", {{"rapid03_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {0, 14}}}}}},
	//	{"BES1_rapid01_sys_0", {{"rapid01_sys", get_rand_set_pairs(15, "rapid01")}}},
	//	{"BES1_rapid01_sys_1", {{"rapid01_sys", get_rand_set_pairs(15, "rapid01")}}},
	//	{"BES1_rapid02_sys_0", {{"rapid02_sys", get_rand_set_pairs(15, "rapid02")}}},
	//	{"BES1_rapid02_sys_1", {{"rapid02_sys", get_rand_set_pairs(15, "rapid02")}}},
	//	{"BES1_rapid03_sys_0", {{"rapid03_sys", get_rand_set_pairs(15, "rapid03")}}},
	//	{"BES1_rapid03_sys_1", {{"rapid03_sys", get_rand_set_pairs(15, "rapid03")}}}
	//};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"BES1_Eff1_0", {{"Eff1_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency1_", {0, 14}}}}}},
//		{"BES1_Eff2_0", {{"Eff2_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency2_", {0, 14}}}}}},
//		{"BES1_Eff3_0", {{"Eff3_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency3_", {0, 14}}}}}},
//		{"BES1_Eff1_sys_0", {{"Eff1_sys", get_rand_set_pairs(15, "rapid05", "Efficiency1")}}},
//		{"BES1_Eff1_sys_1", {{"Eff1_sys", get_rand_set_pairs(15, "rapid05", "Efficinecy1")}}},
//		{"BES1_Eff2_sys_0", {{"Eff2_sys", get_rand_set_pairs(15, "rapid05", "Efficinecy2")}}},
//		{"BES1_Eff2_sys_1", {{"Eff2_sys", get_rand_set_pairs(15, "rapid05", "Efficinecy2")}}},
//		{"BES1_Eff3_sys_0", {{"Eff3_sys", get_rand_set_pairs(15, "rapid05", "Efficinecy3")}}},
//		{"BES1_Eff3_sys_1", {{"Eff3_sys", get_rand_set_pairs(15, "rapid05", "Efficinecy3")}}}
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"BES1_Eff05_0", {{"Eff05_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency05_", {0, 14}}}}}},
//		{"BES1_Eff15_0", {{"Eff15_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency15_", {0, 14}}}}}},
//		{"BES1_Eff05_sys_0", {{"Eff05_sys", get_rand_set_pairs(15, "rapid05", "Efficiency05")}}},
//		{"BES1_Eff05_sys_1", {{"Eff05_sys", get_rand_set_pairs(15, "rapid05", "Efficiency05")}}},
//		{"BES1_Eff15_sys_0", {{"Eff15_sys", get_rand_set_pairs(15, "rapid05", "Efficiency15")}}},
//		{"BES1_Eff15_sys_1", {{"Eff15_sys", get_rand_set_pairs(15, "rapid05", "Efficiency15")}}}
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"BES1_Eff05_0", {{"Eff05_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency05_", {0, 4}}}}}},
//		{"BES1_Eff05_1", {{"Eff05_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency05_", {5, 9}}}}}},
//		{"BES1_Eff05_2", {{"Eff05_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency05_", {10, 14}}}}}},
//		{"BES1_Eff15_0", {{"Eff15_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency15_", {0, 4}}}}}},
//		{"BES1_Eff15_1", {{"Eff15_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency15_", {5, 9}}}}}},
//		{"BES1_Eff15_2", {{"Eff15_def", {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency15_", {10, 14}}}}}}
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"BES1_Eff05_sys_0", {{"Eff05_sys", get_rand_set_pairs(10, "rapid05", "Efficiency05")}}},
//		{"BES1_Eff05_sys_1", {{"Eff05_sys", get_rand_set_pairs(10, "rapid05", "Efficiency05")}}},
//		{"BES1_Eff05_sys_2", {{"Eff05_sys", get_rand_set_pairs(10, "rapid05", "Efficiency05")}}},
//		{"BES1_Eff15_sys_0", {{"Eff15_sys", get_rand_set_pairs(10, "rapid05", "Efficiency15")}}},
//		{"BES1_Eff15_sys_1", {{"Eff15_sys", get_rand_set_pairs(10, "rapid05", "Efficiency15")}}},
//		{"BES1_Eff15_sys_2", {{"Eff15_sys", get_rand_set_pairs(10, "rapid05", "Efficiency15")}}}
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	//{"Sim_flow_flat80_res08_v205_test", {{"flow_flat80_res08_v205_resample_test", {{"Sim_flow_flat80_res08_v205_resample_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res9_v204_test", {{"flow_flat80_flow_res9_v204_resample_test", {{"Sim_flow_flat80_res9_v204_resample_", {0, 0}}}}}},
	//};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"Sim_flow_flat80_res99_v207", {{"flow_flat80_res99_v207_resample", {{"Sim_flow_flat80_res99_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res9_v207", {{"flow_flat80_res9_v207_resample", {{"Sim_flow_flat80_res9_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res75_v207", {{"flow_flat80_res75_v207_resample", {{"Sim_flow_flat80_res75_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res6_v207", {{"flow_flat80_res6_v207_resample", {{"Sim_flow_flat80_res6_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res5_v207", {{"flow_flat80_res5_v207_resample", {{"Sim_flow_flat80_res5_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res4_v207", {{"flow_flat80_res4_v207_resample", {{"Sim_flow_flat80_res4_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res3_v207", {{"flow_flat80_res3_v207_resample", {{"Sim_flow_flat80_res3_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res2_v207", {{"flow_flat80_res2_v207_resample", {{"Sim_flow_flat80_res2_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res15_v207", {{"flow_flat80_res15_v207_resample", {{"Sim_flow_flat80_res15_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res1_v207", {{"flow_flat80_res1_v207_resample", {{"Sim_flow_flat80_res1_v207_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res15_v205", {{"flow_flat80_res15_v205_resample", {{"Sim_flow_flat80_res15_v205_resample_norotate_", {0, 0}}}}}},
//		{"Sim_flow_flat80_res15_v202", {{"flow_flat80_res15_v202_resample", {{"Sim_flow_flat80_res15_v202_resample_norotate_", {0, 0}}}}}},
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"Sim_flow_flat80_res15_v206", {{"flow_flat80_res15_v206_resample", {{"Sim_flow_flat80_res15_v206_resample_norotate_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res15_v204", {{"flow_flat80_res15_v204_resample", {{"Sim_flow_flat80_res15_v204_resample_norotate_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res15_v203", {{"flow_flat80_res15_v203_resample", {{"Sim_flow_flat80_res15_v203_resample_norotate_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res15_v201", {{"flow_flat80_res15_v201_resample", {{"Sim_flow_flat80_res15_v201_resample_norotate_", {0, 0}}}}}},

	//	{"Sim_flow_flat80_res75_v205", {{"flow_flat80_res75_v205_resample", {{"Sim_flow_flat80_res75_v205_resample_norotate_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res5_v205", {{"flow_flat80_res5_v205_resample", {{"Sim_flow_flat80_res5_v205_resample_norotate_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res4_v205", {{"flow_flat80_res4_v205_resample", {{"Sim_flow_flat80_res4_v205_resample_norotate_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res3_v205", {{"flow_flat80_res3_v205_resample", {{"Sim_flow_flat80_res3_v205_resample_norotate_", {0, 0}}}}}},

	//	{"Sim_flow_flat80_res75_v202", {{"flow_flat80_res75_v202_resample", {{"Sim_flow_flat80_res75_v202_resample_norotate_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res5_v202", {{"flow_flat80_res5_v202_resample", {{"Sim_flow_flat80_res5_v202_resample_norotate_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res4_v202", {{"flow_flat80_res4_v202_resample", {{"Sim_flow_flat80_res4_v202_resample_norotate_", {0, 0}}}}}},
	//	{"Sim_flow_flat80_res3_v202", {{"flow_flat80_res3_v202_resample", {{"Sim_flow_flat80_res3_v202_resample_norotate_", {0, 0}}}}}},
	//};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
////		{"Sim_flow_flat80_res15_v207_test", {{"flow_flat80_res15_v207_resample_test", {{"Sim_flow_flat80_res15_v207_resample_norotate_", {0, 0}}}}}},
//		//{"Sim_flat80_anticlmulti_s05a01_epbins1_test", {{"flat80_anticlmulti_spread05_amp01_resample_epbins1_test", {{"Sim_spread05_amp01_flat80_anticlmulti_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_anticlflow_s05a01_res15_v207_epbins1_test", {{"flat80_anticlflow_spread05_amp01_res15_v207_resample_epbins1_test", {{"Sim_spread05_amp01_flat80_anticlflow_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_anticlflowindep_s05a01_res15_v207_epbins1_test", {{"flat80_anticlflowindep_spread05_amp01_res15_v207_resample_epbins1_test", {{"Sim_spread05_amp01_flat80_anticlflowindep_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_Eff_anticlflow_s05a01_res15_v20_epbins1_test", {{"flat80_Eff_anticlflow_spread05_amp01_res15_v20_resample_epbins1_test", {{"Sim_Eff_spread05_amp01_flat80_anticlflow_res15_v20_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_Eff_anticlflowindep_s05a01_res15_v20_epbins1_test", {{"flat80_Eff_anticlflowindep_spread05_amp01_res15_v20_resample_epbins1_test", {{"Sim_Eff_spread05_amp01_flat80_anticlflowindep_res15_v20_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_Eff_anticlflow_s05a01_res15_v207_epbins1_test", {{"flat80_Eff_anticlflow_spread05_amp01_res15_v207_resample_epbins1_test", {{"Sim_Eff_spread05_amp01_flat80_anticlflow_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_Eff_anticlflowindep_s05a01_res15_v207_epbins1_test", {{"flat80_Eff_anticlflowindep_spread05_amp01_res15_v207_resample_epbins1_test", {{"Sim_Eff_spread05_amp01_flat80_anticlflowindep_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_anticlmulti_s1a01_epbins1_test", {{"flat80_anticlmulti_spread1_amp01_resample_epbins1_test", {{"Sim_spread1_amp01_flat80_anticlmulti_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_anticlflow_s1a01_res15_v207_epbins1_test", {{"flat80_anticlflow_spread1_amp01_res15_v207_resample_epbins1_test", {{"Sim_spread1_amp01_flat80_anticlflow_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_anticlflowindep_s1a01_res15_v207_epbins1_test", {{"flat80_anticlflowindep_spread1_amp01_res15_v207_resample_epbins1_test", {{"Sim_spread1_amp01_flat80_anticlflowindep_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_Eff_anticlflowindep_s1a01_res15_v20_epbins1_test", {{"flat80_Eff_anticlflowindep_spread1_amp01_res15_v20_resample_epbins1_test", {{"Sim_Eff_spread1_amp01_flat80_anticlflowindep_res15_v20_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_anticlmulti_s01a01_epbins1_test", {{"flat80_anticlmulti_spread01_amp01_resample_epbins1_test", {{"Sim_spread01_amp01_flat80_anticlmulti_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_anticlflow_s01a01_res15_v207_epbins1_test", {{"flat80_anticlflow_spread01_amp01_res15_v207_resample_epbins1_test", {{"Sim_spread01_amp01_flat80_anticlflow_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_anticlflowindep_s01a01_res15_v207_epbins1_test", {{"flat80_anticlflowindep_spread01_amp01_res15_v207_resample_epbins1_test", {{"Sim_spread01_amp01_flat80_anticlflowindep_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_Eff_anticlflow_s01a01_res15_v20_epbins1_test", {{"flat80_Eff_anticlflow_spread01_amp01_res15_v20_resample_epbins1_test", {{"Sim_Eff_spread01_amp01_flat80_anticlflow_res15_v20_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_Eff_anticlflowindep_s01a01_res15_v20_epbins1_test", {{"flat80_Eff_anticlflowindep_spread01_amp01_res15_v20_resample_epbins1_test", {{"Sim_Eff_spread01_amp01_flat80_anticlflowindep_res15_v20_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_anticlmulti_s08a01_epbins1_test", {{"flat80_anticlmulti_spread08_amp01_resample_epbins1_test", {{"Sim_spread08_amp01_flat80_anticlmulti_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_anticlflow_s08a01_res15_v207_epbins1_test", {{"flat80_anticlflow_spread08_amp01_res15_v207_resample_epbins1_test", {{"Sim_spread08_amp01_flat80_anticlflow_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_anticlflowindep_s08a01_res15_v207_epbins1_test", {{"flat80_anticlflowindep_spread08_amp01_res15_v207_resample_epbins1_test", {{"Sim_spread08_amp01_flat80_anticlflowindep_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_Eff_anticlflow_s08a01_res15_v20_epbins1_test", {{"flat80_Eff_anticlflow_spread08_amp01_res15_v20_resample_epbins1_test", {{"Sim_Eff_spread08_amp01_flat80_anticlflow_res15_v20_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_Eff_anticlflowindep_s08a01_res15_v20_epbins1_test", {{"flat80_Eff_anticlflowindep_spread08_amp01_res15_v20_resample_epbins1_test", {{"Sim_Eff_spread08_amp01_flat80_anticlflowindep_res15_v20_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_Eff_flow_res15_v207_epbins1_test", {{"flat80_Eff_flow_res15_v207_resample_epbins1_test", {{"Sim_flat80_Eff_flow_res15_v207_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_simpleclust_s05a2_epbins1_test", {{"flat80_simpleclust_spread05_amp2_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread05_amp2_norotate_resample_epbins1_", {0, 0}}}}}},
//		//{"Sim_flat80_Eff_simpleclust_s05a2_epbins1_test", {{"flat80_Eff_simpleclust_spread05_amp2_resample_epbins1_test", {{"Sim_flat80_Eff_simpleclust_spread05_amp2_norotate_resample_epbins1_", {0, 0}}}}}},
//	};

	map<string, map<string, map<string, pair<int, int>>>> sets = {
		{"Sim_flat80_simpleclust_s08a05_v20_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp05_v20_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp05_v20_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_simpleclust_s08a3_v20_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp3_v20_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp3_v20_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_Eff_simpleclust_s08a05_v20_res99_epbins1_test", {{"flat80_Eff_simpleclust_spread08_amp05_v20_res99_resample_epbins1_test", {{"Sim_flat80_Eff_simpleclust_spread08_amp05_v20_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_Eff_simpleclust_s08a3_v20_res99_epbins1_test", {{"flat80_Eff_simpleclust_spread08_amp3_v20_res99_resample_epbins1_test", {{"Sim_flat80_Eff_simpleclust_spread08_amp3_v20_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_simpleclust_s08a05_v21_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp05_v21_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp05_v21_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_simpleclust_s08a05_v207_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp05_v207_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp05_v207_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_simpleclust_s08a05_v205_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp05_v205_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp05_v205_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_simpleclust_s08a05_v203_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp05_v203_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp05_v203_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_simpleclust_s08a3_v21_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp3_v21_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp3_v21_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_simpleclust_s08a3_v207_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp3_v207_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp3_v207_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_simpleclust_s08a3_v205_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp3_v205_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp3_v205_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_simpleclust_s08a3_v203_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp3_v203_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp3_v203_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_Eff_simpleclust_s08a05_v207_res99_epbins1_test", {{"flat80_Eff_simpleclust_spread08_amp05_v207_res99_resample_epbins1_test", {{"Sim_flat80_Eff_simpleclust_spread08_amp05_v207_res99_norotate_resample_epbins1_", {0, 0}}}}}},
		{"Sim_flat80_Eff_simpleclust_s08a3_v207_res99_epbins1_test", {{"flat80_Eff_simpleclust_spread08_amp3_v207_res99_resample_epbins1_test", {{"Sim_flat80_Eff_simpleclust_spread08_amp3_v207_res99_norotate_resample_epbins1_", {0, 0}}}}}},
	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"Sim_flat80_simpleclust_s08a18_v21_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp18_v21_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp18_v21_res99_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_simpleclust_s08a18_v207_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp18_v207_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp18_v207_res99_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_simpleclust_s08a18_v205_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp18_v205_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp18_v205_res99_norotate_resample_epbins1_", {0, 0}}}}}},
//		{"Sim_flat80_simpleclust_s08a18_v203_res99_epbins1_test", {{"flat80_simpleclust_spread08_amp18_v203_res99_resample_epbins1_test", {{"Sim_flat80_simpleclust_spread08_amp18_v203_res99_norotate_resample_epbins1_", {0, 0}}}}}},
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
		//{"Sim_flow_flat80_res9_v207_epbins1", {{"flow_flat80_res9_v207_resample_epbins1", {{"Sim_flow_flat80_res9_v207_resample_norotate_epbins1_", {0, 0}}}}}},
		//{"Sim_flow_flat80_res9_v207_epbins1_test", {{"flow_flat80_res9_v207_resample_epbins1_test", {{"Sim_flow_flat80_res9_v207_resample_norotate_epbins1_test_", {0, 0}}}}}},
		//{"Ampt_def_resample_epbins1", {{"default_resample_epbins1", {{"Ampt_rapid05_resample_norotate_epbins1_", {0, 0}}}}}},
		//{"BES1_def", {{"default", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_", {0, 0}}}}}},
		//{"CF_def_resample_epbins1", {{"default_resample_noprerotate_epbins1", {{"CF_rapid05_resample_norotate_noprerotate_epbins1_", {0, 0}}}}}},
		//{"CF_def_resample_epbins1", {{"default_resample_epbins1", {{"CF_rapid05_resample_norotate_epbins1_", {0, 0}}}}}},
		//{"CFEV_def_resample_epbins1", {{"default_resample_epbins1", {{"CFEV_rapid05_resample_norotate_epbins1_", {0, 0}}}}}},
		//{"CFEVb342_def_resample_epbins1", {{"default_resample_epbins1", {{"CFEVb342_rapid05_resample_norotate_epbins1_", {0, 0}}}}}},
	//};

	//map<string, map<string, map<string, pair<int, int>>>> sets_init = {
	//	{"BES1_def_nofileshuffle", {{"default", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_", {0, 0}}}}}},
	//	{"BES1_sys_nofileshuffle_dca08", {{"default_sys", {{"rapid05_resample_norotate_seed_dca08_nsprx1_m2r6_m2s0_nhfit20_epbins1_", {0, 0}}}}}},
	//};

	//map<string, map<string, map<string, pair<int, int>>>> sets_init = {
	//	{"BES1_sys_nofileshuffle_dca15_test1", {{"default_sys_test", {{"rapid05_resample_norotate_seed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
	//	//{"BES1_sys_nofileshuffle_dca15_test2", {{"default_sys_test", {{"rapid05_resample_norotate_seed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {1, 1}}}}}},
	//	//{"BES1_sys_nofileshuffle_dca15_test3", {{"default_sys_test", {{"rapid05_resample_norotate_seed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {2, 2}}}}}},
	//	//{"BES1_sys_nofileshuffle_dca15_test4", {{"default_sys_test", {{"rapid05_resample_norotate_seed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {3, 3}}}}}},
	//	//{"BES1_sys_nofileshuffle_dca15_test5", {{"default_sys_test", {{"rapid05_resample_norotate_seed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {4, 4}}}}}},
	//	//{"BES1_sys_nofileshuffle_dca15_test6", {{"default_sys_test", {{"rapid05_resample_norotate_seed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {5, 5}}}}}},
	//	//{"BES1_sys_nofileshuffle_dca15_test7", {{"default_sys_test", {{"rapid05_resample_norotate_seed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {6, 6}}}}}},
	//	//{"BES1_sys_nofileshuffle_dca05_test", {{"default_sys_test", {{"rapid05_resample_norotate_seed_dca05_nsprx1_m2r6_m2s0_nhfit20_epbins1_", {0, 0}}}}}},
	//};

//	map<string, map<string, map<string, pair<int, int>>>> sets2 = {
//		{"BES1_def_nofileshuffle", {{"default", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_dca08", {{"default_sys", {{"rapid05_resample_norotate_seed_dca08_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_dca12", {{"default_sys", {{"rapid05_resample_norotate_seed_dca12_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nsprx09", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx09_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nsprx11", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx11_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nhfit15", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit15_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nhfit25", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit25_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_m2r8", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r8_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_m2r4", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r4_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_dca05", {{"default_sys", {{"rapid05_resample_norotate_seed_dca05_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_dca15", {{"default_sys", {{"rapid05_resample_norotate_seed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nsprx075", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx075_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nsprx125", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx125_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_m2r2", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r2_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_m2r10", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r10_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_Eff05", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency05_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_Eff1", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency1_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_Eff15", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency15_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_Eff2", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency2_epbins1_calcv2_", {0, 0}}}}}},
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"BES1_sys_nofileshuffle_Eff05", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency05_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_Eff1", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency1_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_Eff15", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_Efficiency15_epbins1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_rand0", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},  // Full randomization
//		{"BES1_sys_rand1", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {1, 1}}}}}},  // Full randomization
//		{"BES1_sys_rand2", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {2, 2}}}}}},  // Full randomization
//		{"BES1_sys_rand3", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {3, 3}}}}}},  // Full randomization
//		{"BES1_sys_rand4", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {4, 4}}}}}},  // Full randomization
//		{"BES1_sys_rand5", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {5, 5}}}}}},  // Full randomization
//		{"BES1_sys_rand6", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {6, 6}}}}}},  // Full randomization
//		{"BES1_sys_nofileshuffle_mixrand0", {{"default_sys", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},  // Only mixed randomization
//		{"BES1_sys_nofileshuffle_mixrand1", {{"default_sys", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {1, 1}}}}}},  // Only mixed randomization
//		{"BES1_sys_nofileshuffle_mixrand2", {{"default_sys", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {2, 2}}}}}},  // Only mixed randomization
//		{"BES1_sys_nofileshuffle_mixrand3", {{"default_sys", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {3, 3}}}}}},  // Only mixed randomization
//		{"BES1_sys_nofileshuffle_mixrand4", {{"default_sys", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {4, 4}}}}}},  // Only mixed randomization
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"BES1_def", {{"default", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},
////		{"BES1_sys_cent_+1", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_sysrefshift1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_cent_-1", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_sysrefshift-1_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_vzleft", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_vzhigh-7_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_vzright", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_vzlow7_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_vzmid", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_vzlow-5_vzhigh5_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_dcxytight", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_dcxyqatight_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_dcxy2tight", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_dcxyqa2tight_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_dcxyloose", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_dcxyqaloose_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_dcxy2loose", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_dcxyqa2loose_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_pileuptight", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_pileupqatight_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_pileup2tight", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_pileupqa2tight_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_pileuploose", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_pileupqaloose_calcv2_", {0, 0}}}}}},
//		{"BES1_sys_pileup2loose", {{"default_sys", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_pileupqa2loose_calcv2_", {0, 0}}}}}},
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"BES1_def_nofileshuffle", {{"default", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_dca08", {{"default_sys", {{"rapid05_resample_norotate_seed_dca08_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_dca12", {{"default_sys", {{"rapid05_resample_norotate_seed_dca12_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nsprx09", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx09_m2r6_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nsprx11", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx11_m2r6_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nhfit15", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit15_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nhfit25", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit25_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_m2r8", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r8_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_m2r4", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r4_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_dca05", {{"default_sys", {{"rapid05_resample_norotate_seed_dca05_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_dca15", {{"default_sys", {{"rapid05_resample_norotate_seed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nsprx075", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx075_m2r6_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_nsprx125", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx125_m2r6_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_m2r2", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r2_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//		{"BES1_sys_nofileshuffle_m2r10", {{"default_sys", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r10_m2s0_nhfit20_epbins1_calcv2_nomix_", {0, 0}}}}}},
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	//{"BES1_v2_sys_nofileshuffle_def", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 1}}}}}},
	//	{"BES1_v2_sys_nofileshuffle_dca08", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca08_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	{"BES1_v2_sys_nofileshuffle_dca12", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca12_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	{"BES1_v2_sys_nofileshuffle_dca05", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca05_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	{"BES1_v2_sys_nofileshuffle_dca15", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_nsprx09", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx09_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_nsprx11", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx11_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_nhfit15", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx1_m2r6_m2s0_nhfit15_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_nhfit25", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx1_m2r6_m2s0_nhfit25_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_m2r8", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx1_m2r8_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_m2r4", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx1_m2r4_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_nsprx075", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx075_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_nsprx125", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx125_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_m2r2", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx1_m2r2_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//	//{"BES1_v2_sys_nofileshuffle_m2r10", {{"v2_sys", {{"rapid05_resample_norotate_strefseed_dca1_nsprx1_m2r10_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
	//};

//	map<string, map<string, map<string, pair<int, int>>>> sets_39 = {
//		{"BES1_v2_sys_nsprx11", {{"v2_sys", {{"rapid05_resample_norotate_dca1_nsprx11_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
//		{"BES1_v2_sys_nsprx125", {{"v2_sys", {{"rapid05_resample_norotate_dca1_nsprx125_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"BES1_rerun_nofileshuffle_test0", {{"default_rand_sys_test", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 0}}}}}},  // No randomization
//		{"BES1_rerun_nofileshuffle_test1", {{"default_rand_sys_test", {{"rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {1, 2}}}}}},
//		{"BES1_rerun_test2", {{"default_rand_sys_test", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 5}}}}}},  // Only mixed randomization
////		{"BES1_rerun_nofileshuffle_test3", {{"default_rand_sys_test", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {1, 1}}}}}},
////		{"BES1_rerun_nofileshuffle_test4", {{"default_rand_sys_test", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {2, 2}}}}}},
////		{"BES1_rerun_nofileshuffle_test5", {{"default_rand_sys_test", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {3, 3}}}}}},
////		{"BES1_rerun_nofileshuffle_test6", {{"default_rand_sys_test", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {4, 4}}}}}},
////		{"BES1_rerun_nofileshuffle_test7", {{"default_rand_sys_test", {{"rapid05_resample_norotate_mixnoseed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {5, 5}}}}}},
//		{"BES1_rerun_test10", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefnoseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 3}}}}}},  // Only StRefMutCorr randomization
////		{"BES1_rerun_nofileshuffle_test11", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefnoseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {1, 1}}}}}},
////		{"BES1_rerun_nofileshuffle_test12", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefnoseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {2, 2}}}}}},
////		{"BES1_rerun_nofileshuffle_test13", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefnoseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {3, 3}}}}}},
//		{"BES1_rerun_test20", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {0, 5}}}}}},  // Randomization of resampling and mixing
////		{"BES1_rerun_nofileshuffle_test21", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {1, 1}}}}}},
////		{"BES1_rerun_nofileshuffle_test22", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {2, 2}}}}}},
////		{"BES1_rerun_nofileshuffle_test23", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {3, 3}}}}}},
////		{"BES1_rerun_nofileshuffle_test24", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {4, 4}}}}}},
////		{"BES1_rerun_nofileshuffle_test25", {{"default_rand_sys_test", {{"rapid05_resample_norotate_strefseed_nomix_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_", {5, 5}}}}}},
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets_11_19 = {
//		{"BES1_sys_11_19", {{"default_sys", {
//				{"rapid05_resample_norotate_dca05_nsprx1_m2r6_m2s0_nhfit20_epbins1_", {0, 0}},
//				{"rapid05_resample_norotate_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_", {0, 0}},
//				{"rapid05_resample_norotate_dca1_nsprx075_m2r6_m2s0_nhfit20_epbins1_", {0, 0}},
//				{"rapid05_resample_norotate_dca1_nsprx125_m2r6_m2s0_nhfit20_epbins1_", {0, 0}},
//				{"rapid05_resample_norotate_dca1_nsprx1_m2r2_m2s0_nhfit20_epbins1_", {0, 0}},
//				{"rapid05_resample_norotate_dca1_nsprx1_m2r10_m2s0_nhfit20_epbins1_", {0, 0}},
//		}}}},
//	};
//
//	map<string, map<string, map<string, pair<int, int>>>> sets_27 = {
//		{"BES1_sys_27_1", {{"default_sys", {
//				{"rapid05_resample_norotate_dca05_nsprx1_m2r6_m2s0_nhfit20_epbins1_", {0, 0}},
//				{"rapid05_resample_norotate_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_", {0, 0}},
//				{"rapid05_resample_norotate_dca1_nsprx075_m2r6_m2s0_nhfit20_epbins1_", {0, 0}},
//		}}}},
//		{"BES1_sys_27_2", {{"default_sys", {
//				{"rapid05_resample_norotate_dca1_nsprx125_m2r6_m2s0_nhfit20_epbins1_", {0, 0}},
//				{"rapid05_resample_norotate_dca1_nsprx1_m2r2_m2s0_nhfit20_epbins1_", {0, 0}},
//				{"rapid05_resample_norotate_dca1_nsprx1_m2r10_m2s0_nhfit20_epbins1_", {0, 0}},
//		}}}},
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"BES1_v2_sys", {{"v2_sys", {
////			{"rapid05_resample_norotate_dca08_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
////			{"rapid05_resample_norotate_dca12_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx09_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx11_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit15_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit25_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx1_m2r8_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx1_m2r4_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
////			{"rapid05_resample_norotate_dca05_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
////			{"rapid05_resample_norotate_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx075_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx125_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx1_m2r2_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca1_nsprx1_m2r10_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}
//		}}}}
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets_39 = {
	//	//{"BES1_v2_sys_39_0", {{"v2_sys", {
	//	//	{"rapid05_resample_norotate_dca08_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca12_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca1_nsprx09_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca1_nsprx11_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//}}}},
	//	//{"BES1_v2_sys_39_1", {{"v2_sys", {
	//	//	{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit15_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit25_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca1_nsprx1_m2r8_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//}}}},
	//	//{"BES1_v2_sys_39_2", {{"v2_sys", {
	//	//	{"rapid05_resample_norotate_dca1_nsprx1_m2r4_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca05_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca15_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca1_nsprx075_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//}}}},
	//	//{"BES1_v2_sys_39_3", {{"v2_sys", {
	//	//	{"rapid05_resample_norotate_dca1_nsprx125_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca1_nsprx1_m2r2_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
	//	//	{"rapid05_resample_norotate_dca1_nsprx1_m2r10_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}
	//	//}}}}
	//};

//map<string, map<string, map<string, pair<int, int>>>> sets = {
//	{"BES1_sys_0", {{"default_sys_test", get_rand_set_pairs(2, "rapid05")}}},
//};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		//{"Sim_flow_flat80_res9_v207_epbins1", {{"flow_flat80_res9_v207_resample_epbins1", {{"Sim_flow_flat80_res9_v207_resample_norotate_epbins1_", {0, 0}}}}}},
//		//{"Sim_flow_flat80_res9_v207_epbins1_test", {{"flow_flat80_res9_v207_resample_epbins1_test", {{"Sim_flow_flat80_res9_v207_resample_norotate_epbins1_test_", {0, 0}}}}}},
//		//{"Ampt_def_resample_epbins1", {{"default_resample_epbins1", {{"Ampt_rapid05_resample_norotate_epbins1_", {0, 0}}}}}},
//		//{"BES1_def_resample_test", {{"default_resample_epbins1_test", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_test_", {0, 0}}}}}},
//		{"BES1_def_resample_test", {{"default_resample_epbins1_calcv2_qaonly_test2", {
//			{"rapid05_resample_norotate_dca12_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}},
//			{"rapid05_resample_norotate_dca08_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_qaonly_", {0, 0}}}}}},
////		{"CF_def_resample_epbins1", {{"default_resample_noprerotate_epbins1", {{"CF_rapid05_resample_norotate_noprerotate_epbins1_", {0, 0}}}}}},
//		//{"CFEV_def_resample_epbins1", {{"default_resample_epbins1", {{"CFEV_rapid05_resample_norotate__epbins1_", {0, 0}}}}}},
////		{"CFEV_def_resample_epbins1", {{"default_resample_epbins1", {{"CFEV_rapid05_resample_norotate__epbins1_", {0, 0}}}}}},
////		{"CFEVb342_def_resample_epbins1", {{"default_resample_noprerotate_epbins1", {{"CFEVb342_rapid05_resample_norotate_noprerotate_epbins1_", {0, 0}}}}}},
//	};


//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"Ampt_def_resample", {{"default_resample", {{"Ampt_rapid05_resample_", {0, 0}}}}}},
//		{"Sim_single10_anticlmulti_resample_s0a0", {{"single10_anticlmulti_resample_spread0_amp0_test", {{"Sim_spread0_amp0_single40_anticlmulti_resample_", {0, 0}}}}}},
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		//{"BES1_def_resample", {{"default_resample", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_", {0, 0}}}}}},
//		{"Ampt_def_resample", {{"default_resample", {{"Ampt_rapid05_resample_norotate_noprerotate_", {0, 0}}}}}},
//		{"CF_def_resample", {{"default_resample", {{"CF_rapid05_resample_norotate_noprerotate_", {0, 0}}}}}},
//		{"CFEV_def_resample", {{"default_resample", {{"CFEV_rapid05_resample_norotate_noprerotate_", {0, 0}}}}}},
//		{"CFEVb342_def_resample", {{"default_resample", {{"CFEVb342_rapid05_resample_norotate_noprerotate_", {0, 0}}}}}},
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"BES1_def_resample", {{"default_resample", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_", {0, 0}}}}}},
	//	{"Ampt_def_resample", {{"default_resample", {{"Ampt_rapid05_resample_norotate_", {0, 0}}}}}},
	//	{"CF_def_resample", {{"default_resample", {{"CF_rapid05_resample_norotate_", {0, 0}}}}}},
	//	{"CFEV_def_resample", {{"default_resample", {{"CFEV_rapid05_resample_norotate_", {0, 0}}}}}},
	//	{"CFEVb342_def_resample", {{"default_resample", {{"CFEVb342_rapid05_resample_norotate_", {0, 0}}}}}},
	//};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	//{"BES1_def_resample", {{"default_resample_epbins1", {{"rapid05_resample_norotate_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_", {0, 0}}}}}},
	//	{"Ampt_def_resample", {{"default_resample_noprerotate_reactionplane", {{"Ampt_rapid05_resample_norotate_noprerotate_reactionplane_", {0, 0}}}}}},
	//	//{"CF_def_resample", {{"default_resample", {{"CF_rapid05_resample_norotate_noprerotate_", {0, 0}}}}}},
	//	{"CFEV_def_resample", {{"default_resample_noprerotate_reactionplane", {{"CFEV_rapid05_resample_norotate_noprerotate_reactionplane_", {0, 0}}}}}},
	//	//{"CFEVb342_def_resample", {{"default_resample", {{"CFEVb342_rapid05_resample_norotate_noprerotate_", {0, 0}}}}}},
	//};
	
//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"Ampt_def_single", {{"default_single", {{"Ampt_rapid05_single_", {0, 0}}}}}},
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"CF_def_resample", {{"default_resample", {{"CF_rapid05_resample_norotate_", {0, 0}}}}}},
	//	{"CF_Eff1_resample", {{"Eff1_resample", {{"CF_rapid05_resample_norotate_Efficiency1_", {0, 0}}}}}},
	//	{"CF_Eff2_resample", {{"Eff2_resample", {{"CF_rapid05_resample_norotate_Efficiency2_", {0, 0}}}}}},
	//};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"Ampt_def_single", {{"default_single", {{"Ampt_rapid05_", {0, 10}}}}}},
	//	{"BES1_def_single", {{"default_single", {{"rapid05_dca1_nsprx1_m2r6_m2s0_nhfit20_", {0, 10}}}}}},
	//};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"Ampt_Eff1_resample", {{"Eff1_resample", {{"Ampt_rapid05_resample_norotate_Efficiency1_", {0, 0}}}}}},
	//	{"Ampt_Eff2_resample", {{"Eff2_resample", {{"Ampt_rapid05_resample_norotate_Efficiency2_", {0, 0}}}}}},
	//	{"Ampt_Eff3_resample", {{"Eff3_resample", {{"Ampt_rapid05_resample_norotate_Efficiency3_", {0, 0}}}}}},
	//	{"Ampt_Old_Eff1_resample", {{"Eff1_resample", {{"Ampt_Old_rapid05_resample_norotate_Efficiency1_", {0, 0}}}}}},
	//	{"Ampt_Old_Eff2_resample", {{"Eff2_resample", {{"Ampt_Old_rapid05_resample_norotate_Efficiency2_", {0, 0}}}}}},
	//	{"Ampt_Old_Eff3_resample", {{"Eff3_resample", {{"Ampt_Old_rapid05_resample_norotate_Efficiency3_", {0, 0}}}}}},
	//};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"Ampt_rapid1", {{"rapid1_def", {{"Ampt_rapid1_n1ratios_", {0, 14}}}}}},
//		{"Ampt_rapid08", {{"rapid08_def", {{"Ampt_rapid08_n1ratios_", {0, 14}}}}}},
//		{"Ampt_rapid06", {{"rapid06_def", {{"Ampt_rapid06_n1ratios_", {0, 14}}}}}},
//		{"Ampt_rapid04", {{"rapid04_def", {{"Ampt_rapid04_n1ratios_", {0, 14}}}}}},
//		{"Ampt_rapid03", {{"rapid03_def", {{"Ampt_rapid03_n1ratios_", {0, 14}}}}}},
//		{"Ampt_rapid02", {{"rapid02_def", {{"Ampt_rapid02_n1ratios_", {0, 14}}}}}},
//		{"Ampt_rapid01", {{"rapid01_def", {{"Ampt_rapid01_n1ratios_", {0, 14}}}}}}
//	};
//
//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"Ampt_Eff1_0", {{"Eff1_def", {{"Ampt_rapid05_n1ratios_Efficiency1_", {0, 14}}}}}},
//		{"Ampt_Eff2_0", {{"Eff2_def", {{"Ampt_rapid05_n1ratios_Efficiency2_", {0, 14}}}}}},
//		{"Ampt_Eff3_0", {{"Eff3_def", {{"Ampt_rapid05_n1ratios_Efficiency3_", {0, 14}}}}}}
//	};
//
//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"Ampt_Eff05_0", {{"Eff05_def", {{"Ampt_rapid05_n1ratios_Efficiency05_", {0, 7}}}}}},
//		{"Ampt_Eff05_1", {{"Eff05_def", {{"Ampt_rapid05_n1ratios_Efficiency05_", {8, 14}}}}}},
//		{"Ampt_Eff15_0", {{"Eff15_def", {{"Ampt_rapid05_n1ratios_Efficiency15_", {0, 7}}}}}},
//		{"Ampt_Eff15_1", {{"Eff15_def", {{"Ampt_rapid05_n1ratios_Efficiency15_", {8, 14}}}}}}
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = { 
	//	{"BES1_rapid1_0", {{"rapid1", {{"rapid1_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {0, 19}}}}}},
	//	{"BES1_rapid08", {{"rapid08", {{"rapid08_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {20, 39}}}}}},
	//	{"BES1_rapid02", {{"rapid02", {{"rapid02_n1ratios_dca1_nsprx1_m2r6_m2s0_nhfit20_", {20, 29}}}}}},
	//	{"BES1_def_sys_0", {{"default_sys", get_rand_set_pairs(10, "rapid1")}}},
	//	{"BES1_def_sys_1", {{"default_sys", get_rand_set_pairs(10, "rapid08")}}},
	//	{"BES1_def_sys_2", {{"default_sys", get_rand_set_pairs(10, "rapid02")}}}
	//};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"CF_def_0", {{"default", {{"CF_rapid05_n1ratios_", {0, 9}}}}}},
//		{"CF_def_1", {{"default", {{"CF_rapid05_n1ratios_", {10, 19}}}}}},
//		{"CF_def_2", {{"default", {{"CF_rapid05_n1ratios_", {20, 29}}}}}},
//		{"CF_def_3", {{"default", {{"CF_rapid05_n1ratios_", {30, 39}}}}}},
//		{"CF_def_4", {{"default", {{"CF_rapid05_n1ratios_", {40, 49}}}}}},
//		{"CF_def_5", {{"default", {{"CF_rapid05_n1ratios_", {50, 59}}}}}}
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"CF_def_0", {{"default", {{"CF_rapid05_n1ratios_", {0, 0}}}}}}
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"CF_rapid1_0", {{"rapid1_def", {{"CF_rapid1_n1ratios_", {0, 7}}}}}},
//		{"CF_rapid1_1", {{"rapid1_def", {{"CF_rapid1_n1ratios_", {8, 14}}}}}},
//		{"CF_rapid08_0", {{"rapid08_def", {{"CF_rapid08_n1ratios_", {0, 7}}}}}},
//		{"CF_rapid08_1", {{"rapid08_def", {{"CF_rapid08_n1ratios_", {8, 14}}}}}},
//		{"CF_rapid06_0", {{"rapid06_def", {{"CF_rapid06_n1ratios_", {0, 7}}}}}},
//		{"CF_rapid06_1", {{"rapid06_def", {{"CF_rapid06_n1ratios_", {8, 14}}}}}},
//		{"CF_rapid04_0", {{"rapid04_def", {{"CF_rapid04_n1ratios_", {0, 7}}}}}},
//		{"CF_rapid04_1", {{"rapid04_def", {{"CF_rapid04_n1ratios_", {8, 14}}}}}},
//		{"CF_rapid03_0", {{"rapid03_def", {{"CF_rapid03_n1ratios_", {0, 7}}}}}},
//		{"CF_rapid03_1", {{"rapid03_def", {{"CF_rapid03_n1ratios_", {8, 14}}}}}},
//		{"CF_rapid02_0", {{"rapid02_def", {{"CF_rapid02_n1ratios_", {0, 7}}}}}},
//		{"CF_rapid02_1", {{"rapid02_def", {{"CF_rapid02_n1ratios_", {8, 14}}}}}},
//		{"CF_rapid01_0", {{"rapid01_def", {{"CF_rapid01_n1ratios_", {0, 7}}}}}},
//		{"CF_rapid01_1", {{"rapid01_def", {{"CF_rapid01_n1ratios_", {8, 14}}}}}},
//		{"CF_Eff1_0", {{"Eff1_def", {{"CF_rapid05_n1ratios_Efficiency1_", {0, 7}}}}}},
//		{"CF_Eff1_1", {{"Eff1_def", {{"CF_rapid05_n1ratios_Efficiency1_", {8, 14}}}}}},
//		{"CF_Eff2_0", {{"Eff2_def", {{"CF_rapid05_n1ratios_Efficiency2_", {0, 7}}}}}},
//		{"CF_Eff2_1", {{"Eff2_def", {{"CF_rapid05_n1ratios_Efficiency2_", {8, 14}}}}}},
//		{"CF_Eff3_0", {{"Eff3_def", {{"CF_rapid05_n1ratios_Efficiency3_", {0, 7}}}}}},
//		{"CF_Eff3_1", {{"Eff3_def", {{"CF_rapid05_n1ratios_Efficiency3_", {8, 14}}}}}},
//		{"CF_Eff05_0", {{"Eff05_def", {{"CF_rapid05_n1ratios_Efficiency05_", {0, 7}}}}}},
//		{"CF_Eff05_1", {{"Eff05_def", {{"CF_rapid05_n1ratios_Efficiency05_", {8, 14}}}}}},
//		{"CF_Eff15_0", {{"Eff15_def", {{"CF_rapid05_n1ratios_Efficiency15_", {0, 7}}}}}},
//		{"CF_Eff15_1", {{"Eff15_def", {{"CF_rapid05_n1ratios_Efficiency15_", {8, 14}}}}}}
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"Sim_flat80_clmulti_s04a002_test", {{"flat80_clmulti_spread04_amp002_resample_test", {{"Sim_spread04_amp002_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_clmulti_s04a004_test", {{"flat80_clmulti_spread04_amp004_resample_test", {{"Sim_spread04_amp004_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_clmulti_s04a006_test", {{"flat80_clmulti_spread04_amp006_resample_test", {{"Sim_spread04_amp006_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_clmulti_s04a008_test", {{"flat80_clmulti_spread04_amp008_resample_test", {{"Sim_spread04_amp008_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_clmulti_s04a01_test", {{"flat80_clmulti_spread04_amp01_resample_test", {{"Sim_spread04_amp01_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
	//};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {};
	//vector<string> spreads = { "03", "11", "12" };
//	vector<string> amps = { "0", "005", "01", "0125", "015", "0175", "02", "0225", "025", "03", "035", "04", "045", "05", "06", "07", "08", "09", "1", "125", "15", "175", "2", "225", "25", "3", "35", "4", "45", "5", "6", "7", "8", "9", "99" };
	//vector<string> amps = { "002", "004", "006", "008", "01"};
	//vector<string> cl_types = { "anticlmulti", "clmulti" };


	//for (string spread : spreads) {
	//	for (string amp : amps) {
	//		for (string cl_type : cl_types){
	//			string job_name = "Sim_flat80_" + cl_type + "_s" + spread + "a" + amp + "_test";
	//			string group_name = "flat80_" + cl_type + "_spread" + spread + "_amp" + amp + "_resample_test";
	//			string set_name = "Sim_spread" + spread + "_amp" + amp + "_flat80_" + cl_type + "_norotate_resample_";
	//			sets[job_name][group_name][set_name] = { 0, 0 };
	//		}
	//	}
	//}

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"Sim_flat80_clmulti_s1a002", {{"flat80_clmulti_spread1_amp002_resample_test", {{"Sim_spread1_amp002_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_clmulti_s1a004", {{"flat80_clmulti_spread1_amp004_resample_test", {{"Sim_spread1_amp004_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_clmulti_s1a006", {{"flat80_clmulti_spread1_amp006_resample_test", {{"Sim_spread1_amp006_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_clmulti_s1a008", {{"flat80_clmulti_spread1_amp008_resample_test", {{"Sim_spread1_amp008_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_clmulti_s05a002", {{"flat80_clmulti_spread05_amp002_resample_test", {{"Sim_spread05_amp002_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_clmulti_s05a004", {{"flat80_clmulti_spread05_amp004_resample_test", {{"Sim_spread05_amp004_flat80_clmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_anticlmulti_s1a002", {{"flat80_anticlmulti_spread1_amp002_resample_test", {{"Sim_spread1_amp002_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_anticlmulti_s1a004", {{"flat80_anticlmulti_spread1_amp004_resample_test", {{"Sim_spread1_amp004_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_anticlmulti_s1a006", {{"flat80_anticlmulti_spread1_amp006_resample_test", {{"Sim_spread1_amp006_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_anticlmulti_s1a008", {{"flat80_anticlmulti_spread1_amp008_resample_test", {{"Sim_spread1_amp008_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_anticlmulti_s05a002", {{"flat80_anticlmulti_spread05_amp002_resample_test", {{"Sim_spread05_amp002_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
//		{"Sim_flat80_anticlmulti_s05a004", {{"flat80_anticlmulti_spread05_amp004_resample_test", {{"Sim_spread05_amp004_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"Sim_flat80_anticlmulti_s45a0125", {{"flat80_anticlmulti_spread45_amp0125_resample", {{"Sim_spread45_amp0125_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s45a6", {{"flat80_anticlmulti_spread45_amp6_resample", {{"Sim_spread45_amp6_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s45a7", {{"flat80_anticlmulti_spread45_amp7_resample", {{"Sim_spread45_amp7_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s45a8", {{"flat80_anticlmulti_spread45_amp8_resample", {{"Sim_spread45_amp8_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s45a9", {{"flat80_anticlmulti_spread45_amp9_resample", {{"Sim_spread45_amp9_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s45a99", {{"flat80_anticlmulti_spread45_amp99_resample", {{"Sim_spread45_amp99_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},

	//	{"Sim_flat80_anticlmulti_s5a0125", {{"flat80_anticlmulti_spread5_amp0125_resample", {{"Sim_spread5_amp0125_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s5a6", {{"flat80_anticlmulti_spread5_amp6_resample", {{"Sim_spread5_amp6_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s5a7", {{"flat80_anticlmulti_spread5_amp7_resample", {{"Sim_spread5_amp7_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s5a8", {{"flat80_anticlmulti_spread5_amp8_resample", {{"Sim_spread5_amp8_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s5a9", {{"flat80_anticlmulti_spread5_amp9_resample", {{"Sim_spread5_amp9_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//	{"Sim_flat80_anticlmulti_s5a99", {{"flat80_anticlmulti_spread5_amp99_resample", {{"Sim_spread5_amp99_flat80_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//};

//	map<string, map<string, map<string, pair<int, int>>>> sets;
//	vector<string> aps = {"02", "04"};
//	vector<string> ams = {"01", "02"};
//	vector<string> sdps = {"02", "04"};
//	vector<string> sdms = {"1", "2"};
//
//	for (string ap : aps) {
//		for (string am : ams) {
//			for (string sdp: sdps) {
//				for (string sdm : sdms) {
//					string jname = "Sim_flat80_clmultiplusminus_sm" + sdm + "sp" + sdp + "am" + am + "ap" + ap + "_test";
//					string gname = "flat80_clmultiplusminus_spreadminus" + sdm + "_spreadplus" + sdp + "_ampminus" + am + "_ampplus" + ap + "_resample_test";
//					string sname = "Sim_spreadminus" + sdm + "_spreadplus" + sdp + "_ampminus" + am + "_ampplus" + ap + "_flat80_clmultiplusminus_norotate_resample_test_";
//					sets[jname][gname][sname] = {0,0};
//				}
//			}
//		}
//	}

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
//		{"Sim_flat80_clmultiplusminus_sm1sp02am01ap1_test", {{"flat80_clmultiplusminus_spreadminus1_spreadplus02_ampminus01_ampplus1_resample_test", {{"Sim_spreadminus1_spreadplus02_ampminus01_ampplus1_flat80_clmultiplusminus_norotate_resample_test_", {0, 0}}}}}},
//	};

//	map<string, map<string, map<string, pair<int, int>>>> sets = {
////			{"CFEV_def_resample", {{"default_resample", {{"CFEV_rapid05_resample_norotate_", {0, 0}}}}}},
////			{"CFEVb342_def_resample", {{"default_resample", {{"CFEVb342_rapid05_resample_norotate_", {0, 0}}}}}},
//			{"Ampt_def_resample", {{"default_resample", {{"Ampt_rapid05_resample_norotate_", {0, 0}}}}}},
//	};

	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"Ampt_qaonly", {{"qaonly", {{"Ampt_rapid05_qaonly_", {0, 0}}}}}},
	//	{"BES1_qaonly", {{"qaonly", {{"rapid05_resample_dca1_nsprx1_m2r6_m2s0_nhfit20_qaonly_", {0, 0}}}}}},
	//};

	//vector<int> energy_list{ 7, 11, 19, 27, 39, 62 };
	//vector<int> energy_list{ 39, 62, 27, 19, 11, 7 };
	//vector<int> energy_list{ 7, 11, 19, 27, 62, 39 };
	vector<int> energy_list{ 62 };

	int set_sleep = 5;
	int energy_sleep = 5;
	int free_threads = 0;

	//int jobs = sets.size() * energy_list.size() + sets_init.size() * energy_list.size();
	int jobs = sets.size() * energy_list.size();

//	// Testing
//	mutex* mtx = new mutex;
//	vector<string>* file_list = new vector<string>;
//	int job_num = 0;
//	for (pair<string, map<string, map<string, pair<int, int>>>> job : sets) {
//		for (int energy : energy_list) {
//			cout << endl << "Running " << energy << "GeV  job " << ++job_num << " of " << jobs << endl << endl;
//			run_job(energy, job.second, job_num, job.first, jobs, mtx, file_list);
//			this_thread::sleep_for(chrono::seconds(energy_sleep));
//		}
//		this_thread::sleep_for(chrono::seconds(set_sleep));
//	}

	mutex* mtx = new mutex;
	vector<string>* file_list = new vector<string>;
	ROOT::Math::IntegratorOneDimOptions::SetDefaultIntegrator("GaussLegendre");
	ROOT::EnableThreadSafety();
	{
		int job_num = 0;
		ThreadPool pool(thread::hardware_concurrency() - free_threads);

		for (int energy : energy_list) {
			for (pair<string, map<string, map<string, pair<int, int>>>> job : sets) {
				cout << endl << "Queueing " << energy << "GeV  job " << ++job_num << " of " << jobs << endl << endl;
				pool.enqueue(run_job, energy, job.second, job_num, job.first, jobs, mtx, file_list);
				this_thread::sleep_for(chrono::seconds(set_sleep));
			}
			this_thread::sleep_for(chrono::seconds(energy_sleep));
		}

//		for (int energy : energy_list) {
//			for (pair<string, map<string, map<string, pair<int, int>>>> job : sets) {
//				cout << endl << "Queueing " << energy << "GeV  job " << ++job_num << " of " << jobs << endl << endl;
//				run_job(energy, job.second, job_num, job.first, jobs, mtx, file_list);
////				this_thread::sleep_for(chrono::seconds(set_sleep));
//			}
////			this_thread::sleep_for(chrono::seconds(energy_sleep));
//		}

//		for (int energy : energy_list) {
//			for (pair<string, map<string, map<string, pair<int, int>>>> job : sets2) {
//				cout << endl << "Queueing " << energy << "GeV  job " << ++job_num << " of " << jobs << endl << endl;
//				pool.enqueue(run_job, energy, job.second, job_num, job.first, jobs, mtx, file_list);
//				this_thread::sleep_for(chrono::seconds(set_sleep));
//			}
//			this_thread::sleep_for(chrono::seconds(energy_sleep));
//		}

		//for (pair<string, map<string, map<string, pair<int, int>>>> job : sets_init) {
		//	for (int energy : {62, 27, 19}) {
		//		cout << endl << "Queueing " << energy << "GeV  job " << ++job_num << " of " << jobs << endl << endl;
		//		pool.enqueue(run_job, energy, job.second, job_num, job.first, jobs, mtx, file_list);
		//		this_thread::sleep_for(chrono::seconds(1));
		//	}
		//	this_thread::sleep_for(chrono::seconds(60));
		//}
	}
}


void read_rcf_sim(string set_group, string set_name) {
	//map<string, map<string, map<string, pair<int, int>>>> sets = {
	//	{"Sim_single8_anticlmulti_s25a05", {{"single8_anticlmulti_spread25_amp05_resample_test", {{"Sim_spread25_amp05_single8_anticlmulti_norotate_resample_", {0, 0}}}}}},
	//};

	map<string, map<string, pair<int, int>>> job {
		{set_group, {{set_name, {0, 0}}}}
	};

	mutex* mtx = new mutex;
	vector<string>* file_list = new vector<string>;
	ROOT::Math::IntegratorOneDimOptions::SetDefaultIntegrator("GaussLegendre");
	run_job(62, job, 1, "Sim_RCF", 1, mtx, file_list);
}


void read_single_job(int energy, string job_type, string set_group, string set_name, int job_num_low, int job_num_high) {
	map<string, map<string, pair<int, int>>> job {
		{set_group, {{set_name, {job_num_low, job_num_high}}}}
	};

	mutex* mtx = NULL;
	vector<string>* file_list = new vector<string>;
	ROOT::Math::IntegratorOneDimOptions::SetDefaultIntegrator("GaussLegendre");
	run_job(energy, job, 1, job_type, 1, mtx, file_list);
}

//void read_class() {
////	map<string, pair<int, int>> set_pairs = {{"Ampt_baryontotal", {0, 2}}, {"Ampt_mesontotal", {0, 2}}, {"Ampt_hadrontotal", {0, 2}}};
////	map<string, pair<int, int>> set_pairs = {{"Ampt_p+_n1ratios_Efficiency8", {0, 2}}, {"Ampt_p+_n1ratios_Efficiency5", {0, 2}}, {"Ampt_p+_n1ratios_Efficiency3", {0, 2}}, {"Ampt_p+_n1ratios_Efficiency1", {0, 2}}, {"Ampt_p+_n1ratios", {1, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"pion+_n1ratios", {0, 2}}, {"pion-_n1ratios", {0, 2}}, {"piontotal_n1ratios", {0, 2}}};
////	map<string, pair<int, int>> set_pairs = {{"eta05_n1ratios_Efficiency8", {0, 2}}, {"eta05_n1ratios_Efficiency5", {0, 2}}, {"eta05_n1ratios_Efficiency3", {0, 2}}, {"eta05_n1ratios_Efficiency1", {0, 2}}, {"eta05_n1ratios", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"eta05_n1ratios_dca1", {0, 2}}, {"eta05_n1ratios_dca3", {0, 2}}}; //, {"eta1_n1ratios", {3, 3}}};
////	map<string, pair<int, int>> set_pairs = {{"eta1_n1ratios_dca1", {0, 4}}, {"eta1_n1ratios_dca3", {0, 4}}, {"eta05_n1ratios_dca1", {0, 4}}, {"eta05_n1ratios_dca3", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"Ampt_default_eta05_n1ratios", {0, 4}}, {"Ampt_default_eta1_n1ratios", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"eta05_n1ratios_dca1", {0, 4}}, {"eta05_n1ratios_dca3", {0, 4}}, {"Ampt_eta05_n1ratios", {0, 4}}, {"Sim_n1ratios_0p0s_eta05_dca3", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"Sim_n1ratios_0p0s_eta05_dca3", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1", {2, 4}}, {"rapid1_n1ratios_dca1", {2, 4}}, {"rapid05_n1ratios_dca3", {0, 4}}, {"rapid1_n1ratios_dca3", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1", {0, 1}}, {"rapid1_n1ratios_dca1", {0, 1}}, {"Ampt_rapid05_n1ratios", {0, 4}}, {"Ampt_rapid1_n1ratios", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"Ampt_default_rapid05_n1ratios", {0, 4}}, {"Ampt_default_rapid1_n1ratios", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1_test", {0, 0}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1_Efficiency1", {0, 6}}, {"rapid05_n1ratios_dca1_Efficiency2", {0, 6}}, {"rapid05_n1ratios_dca1_Efficiency3", {0, 6}}, {"rapid05_n1ratios_dca1_Efficiency4", {0, 6}}, {"rapid05_n1ratios_dca1_Efficiency5", {0, 6}}, {"rapid05_n1ratios_dca1_Efficiency6", {0, 6}}};
////	map<string, pair<int, int>> set_pairs = {{"Ampt_rapid05_n1ratios", {0, 4}}, {"Ampt_rapid1_n1ratios", {0, 4}}, {"Ampt_rapid05_n1ratios_Efficiency1", {0, 4}}, {"Ampt_rapid05_n1ratios_Efficiency2", {0, 4}}, {"Ampt_rapid05_n1ratios_Efficiency3", {0, 4}}, {"Ampt_rapid05_n1ratios_Efficiency4", {0, 4}}, {"Ampt_rapid05_n1ratios_Efficiency5", {0, 4}}, {"Ampt_rapid05_n1ratios_Efficiency6", {0, 4}}, {"Ampt_default_rapid05_n1ratios", {0, 4}}, {"Ampt_default_rapid1_n1ratios", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid04_n1ratios_dca1", {0, 4}}, {"rapid045_n1ratios_dca1", {0, 4}}, {"rapid055_n1ratios_dca1", {0, 4}}, {"rapid06_n1ratios_dca1", {0, 4}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1_seed_", {0,0}}, {"rapid04_n1ratios_dca1_seed_", {0,0}}, {"rapid045_n1ratios_dca1_seed_", {0,0}}, {"rapid055_n1ratios_dca1_seed_", {0,0}}, {"rapid06_n1ratios_dca1_seed_", {0,0}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1_", {0,6}}, {"rapid05_n1ratios_dca1_Efficiency1_", {0,6}}, {"rapid05_n1ratios_dca1_Efficiency2_", {0,6}}, {"rapid05_n1ratios_dca09_", {0,6}}, {"rapid05_n1ratios_dca095_", {0,6}}, {"rapid05_n1ratios_dca105_", {0,6}}, {"rapid05_n1ratios_dca11_", {0,6}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {0,6}},
////			{"rapid05_n1ratios_dca08_nsprx1_m2r6_m2s0_sys_", {0,6}}, {"rapid05_n1ratios_dca12_nsprx1_m2r6_m2s0_sys_", {0,6}},
////			{"rapid05_n1ratios_dca1_nsprx09_m2r6_m2s0_sys_", {0,6}}, {"rapid05_n1ratios_dca1_nsprx11_m2r6_m2s0_sys_", {0,6}},
////			{"rapid05_n1ratios_dca1_nsprx1_m2r2_m2s0_sys_", {0,6}},
////			{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency1_sys_", {0,6}}};
//
//
////	map<string, pair<int, int>> set_pairs = get_rand_set_pairs(55);
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_def_", {17,40}}, {"Ampt_rapid05_n1ratios", {17,40}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_mixseed_", {3,20}}, {"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_mixnoseed_", {3,20}}};
////	map<string, pair<int, int>> set_pairs = {{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency1_", {0,16}},
////			{"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency2_", {0,16}}, {"rapid05_n1ratios_dca1_nsprx1_m2r6_m2s0_Efficiency3_", {0,16}}
////			};
//
//	map<string, pair<int, int>> set_pairs = {{"Ampt_rapid05_n1ratios_dca1_ReactionPlane_", {13, 16}}};
//
////	map<string, pair<int, int>> set_pairs = {};
////	float val_min = 0.5;
////	float val_max = 1.5;
////	float val_step = 0.05;
////	int mult_scale = 100;
////	unsigned chars = 3;
////	string name_pre = "rapid05_n1ratios_dca";
////	string name_post = "_seed_";
////	pair<int, int> set_nums = {0,0};
////	for(float val = val_min; val <= val_max; val+=val_step) {
////		string num = to_string(int(val * mult_scale + 0.5));
////		while(num.size() < chars) { num = "0" + num; }
////		set_pairs[name_pre + num + name_post] = set_nums;
////	}
//
//	vector<int> energy_list {39, 62, 27, 19, 11, 7};
////	vector<int> energy_list {7, 11};
//
//	int set_sleep = 1;
//	int energy_sleep = 1;
//	int free_threads = 0;
//
//	int jobs = 0;
//	for(pair<string, pair<int, int>> set_pair:set_pairs) {
//		for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
//			jobs++;
//		}
//	}
//	jobs *= energy_list.size();
//
//	mutex *mtx = new mutex;
//	vector<string> *file_list = new vector<string>;
//	ROOT::EnableThreadSafety();
//	{
//		int job_num = 0;
//		ThreadPool pool(thread::hardware_concurrency() - free_threads);
//
//		for(int energy:energy_list) {
//			for(pair<string, pair<int, int>> set_pair:set_pairs) {
//				for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
//					string set_dir = set_pair.first + to_string(set_num) + "/";
//					cout << endl << "Queueing " << set_dir << energy << "GeV  job " << ++job_num << " of " << jobs << endl << endl;
//					pool.enqueue(run_set, energy, set_num, set_pair.first, job_num, jobs, mtx, file_list);
//					this_thread::sleep_for(chrono::seconds(set_sleep));
//				}
//			}
//			this_thread::sleep_for(chrono::seconds(energy_sleep));
//		}
//	}
//}


void run_job(int energy, map<string, map<string, pair<int, int>>> job, int job_num, string job_type, int jobs, mutex* mtx, vector<string>* file_list) {
//	bool replace_existing = false;  // If folder for job exists, overwrite if true, skip job if false // Logic doesn't work. Rework a more complicated implementation if wanted
	int ref = 3;
	int sim_events_per_total_proton = 100000;
	TreeReader reader(energy, ref, mtx);
	reader.set_job_type(job_type);
//	reader.set_file_list(file_list);
	if (in_string(job_type, "_nofileshuffle")) {
		reader.set_file_shuffle_rand_seed(42);
	}

	string base_path, in_base_path, in_base_ampt_path, in_base_cf_path;
	if (in_string(job_type, "RCF")) {
		base_path = "./";  //"/star/u/dneff/Documents/";
		in_base_path = "./";
		in_base_ampt_path = "./";
		in_base_cf_path = "./";
		mkdir("Data_Sim");
		mkdir("Data_Sim_Mix");
	}
	else if (platform == "lin") {
		//base_path = "/home/dylan/Research/";
		base_path = "/media/ucla/Research/";
		in_base_path = "/media/ucla/Research/";
		in_base_ampt_path = "/media/ucla/Research/";
		in_base_cf_path = "/media/ucla/Research/";
	}
	else if (platform == "win") {
		//base_path = "C:/Users/Dylan/Desktop/Research/";
		//in_base_path = "C:/Users/Dylan/Desktop/Research/";
		//in_base_ampt_path = "E:/Research/";
		//in_base_cf_path = "E:/Research/";
		base_path = "F:/Research/";
		in_base_path = "F:/Research/";
		in_base_ampt_path = "F:/Research/";
		in_base_cf_path = "F:/Research/";
	}
	string out_base_path = base_path;
	string in_path = in_base_path;
	string out_job_dir = out_base_path;
	string mix_out_job_dir = out_base_path;

	if (in_string(job_type, "Ampt")) {
		if (in_string(job_type, "Ampt_Old")) {
			in_path = in_base_ampt_path + "AMPT_Old_Trees/";
			out_job_dir += "Data_Ampt_Old/";
			mix_out_job_dir += "Data_Ampt_Old_Mix/";
			reader.set_ampt_cent_path(base_path + "Ampt_Old_Centralities/");
		}
		else {
			in_path = in_base_ampt_path + "AMPT_Trees_New_Coalescence/";
			out_job_dir += "Data_Ampt_New_Coal/";
			mix_out_job_dir += "Data_Ampt_New_Coal_Mix/";
			reader.set_ampt_cent_path(base_path + "Ampt_Centralities/");
		}
	}
	else if (in_string(job_type, "Sim")) {
		in_path += "BES1_Trees/";
		if (in_string(job_type, "_test")) {
			//sim_events_per_total_proton = 60000;
			cout << "Running test" << endl;
			out_job_dir += "Data_Sim_tests/";
			mix_out_job_dir += "Data_Sim_tests_Mix/";
		}
		else if(in_string(job_type, "_flow")) {
			out_job_dir += "Data_Sim_Flow/";
			mix_out_job_dir += "Data_Sim_Flow_Mix/";
		}
		else {
			out_job_dir += "Data_Sim/";
			mix_out_job_dir += "Data_Sim_Mix/";
		}
	}
	else if (in_string(job_type, "CF_")) {
		in_path = in_base_cf_path + "Cooper_Frye_Trees/Protons_Only/";
		out_job_dir += "Data_CF/";
		mix_out_job_dir += "Data_CF_Mix/";
	}
	else if (in_string(job_type, "CFEV_")) {
		in_path = in_base_cf_path + "Cooper_Frye_EV_Trees/Protons_Only/";
		out_job_dir += "Data_CFEV/";
		mix_out_job_dir += "Data_CFEV_Mix/";
	}
	else if (in_string(job_type, "CFEVb342_")) {
		in_path = in_base_cf_path + "Cooper_Frye_EVb342_Trees/Protons_Only/";
		out_job_dir += "Data_CFEVb342/";
		mix_out_job_dir += "Data_CFEVb342_Mix/";
	}
	else {
		in_path += "BES1_Trees/";
		out_job_dir += "Data/";
		mix_out_job_dir += "Data_Mix/";
	}

	reader.set_in_path(in_path);
	reader.set_tree_name("tree");
//	reader.set_ampt_cent_path(base_path + "Ampt_Centralities/");

	// Pile up not implemented, need to think about it
	//if (in_string(set_name, "PileUp")) {
	//	reader.set_pile_up(true); 
	//	reader.set_pile_up_prob(str_num_dec(get_flag_trail(set_name, "PileUp", "_")[0], 1));
	//}

	vector<int> divs{ 356, 300, 288, 270, 240, 180, 120, 90, 89, 72, 60 };
//	vector<int> divs{ 60 };
	map<int, int> sim_cent_events = { {8, 80 * sim_events_per_total_proton} };
	//map<int, int> sim_cent_events = { {8, 80 * 10000} };

	// Might fail on Mixer::append_event if these are too low?
	map<string, map<int, int>> particle_bins { {"BES1", { {7, 55}, {11, 44}, {19, 38}, {27, 36}, {39, 32}, {62, 29} } },
		//{"AMPT", { {7, 75}, {11, 67}, {19, 56}, {27, 52}, {39, 46}, {62, 42} } },
//		{"AMPT", { {7, 76}, {11, 68}, {19, 57}, {27, 53}, {39, 47}, {62, 43} } },
//		{"CF", { {7, 79}, {19, 65}, {27, 58}, {39, 51}, {62, 48} } },
		{"AMPT", { {7, 100}, {11, 100}, {19, 100}, {27, 100}, {39, 90}, {62, 80} } },
		{"CF", { {7, 80}, {19, 80}, {27, 70}, {39, 70}, {62, 70} } },
	};

	if (in_string(job_type, { "BES1", "sys" }, true)) {  // If running systematics chance of getting larger max particles/event
		for (auto energy : particle_bins["BES1"]) {
			particle_bins["BES1"][energy.first] = particle_bins["BES1"][energy.first] * 1.3;
		}
	}

	for (pair<string, map<string, pair<int, int>>> set_group : job) {
		for (pair<string, pair<int, int>> set : set_group.second) {
			string out_dir = out_job_dir;
			string mix_out_dir = mix_out_job_dir;

			out_dir += set_group.first + "/";  // Add set-group directory to end of path
			mix_out_dir += set_group.first + "/";
//			if (!replace_existing) {  // Logic doesn't work. Rework a more complicated implementation if wanted
//				if (check_dir(out_dir+to_string(energy)+"GeV") && check_dir(mix_out_dir+to_string(energy)+"GeV")) { return; }  // Skip job if folders already exist.
//			}
			mkdir(out_dir);  // Create set-group directory if it does not already exist.
			mkdir(mix_out_dir);

			// Set Simulator params
			cout << set.first << endl;
			if (in_string(set.first, { "Sim", "pgroup" }, true)) {
				reader.sim.set_p_group(str_num_dec(get_flag_trail(set.first, "pgroup", "_")[0], 0));
			}
			if (in_string(set.first, { "Sim", "spread" }, true)) {
				reader.sim.set_spread_sigma(str_num_dec(get_flag_trail(set.first, "spread", "_")[0], 1));
				cout << reader.sim.get_spread_sigma() << endl;
			}
			if (in_string(set.first, { "Sim", "amp" }, true)) {
				reader.sim.set_amp_group(str_num_dec(get_flag_trail(set.first, "amp", "_")[0], 0));
				cout << reader.sim.get_amp_group() << endl;
			}
			if (in_string(set.first, { "Sim", "spreadplus" }, true)) {
				reader.sim.set_spread_sigma_plus(str_num_dec(get_flag_trail(set.first, "spreadplus", "_")[0], 1));
			}
			if (in_string(set.first, { "Sim", "spreadminus" }, true)) {
				reader.sim.set_spread_sigma_minus(str_num_dec(get_flag_trail(set.first, "spreadminus", "_")[0], 1));
			}
			if (in_string(set.first, { "Sim", "ampplus" }, true)) {
				reader.sim.set_amp_group_plus(str_num_dec(get_flag_trail(set.first, "ampplus", "_")[0], 0));
			}
			if (in_string(set.first, { "Sim", "ampminus" }, true)) {
				reader.sim.set_amp_group_minus(-str_num_dec(get_flag_trail(set.first, "ampminus", "_")[0], 0));  // Make negative
			}
			if (in_string(set.first, { "Sim", "poisson" }, true)) {
				reader.sim.set_proton_dist("poisson");
				reader.sim.set_particle_mean(stof(get_flag_trail(set.first, "poisson", "_")[0]));
			}
			if (in_string(set.first, { "Sim", "single" }, true)) {
				reader.sim.set_proton_dist("single");
				reader.sim.set_particle_mean(stof(get_flag_trail(set.first, "single", "_")[0]));
			}
			if (in_string(set.first, { "Sim", "flat" }, true)) {
				reader.sim.set_proton_dist("flat");
				reader.sim.set_particle_max(stoi(get_flag_trail(set.first, "flat", "_")[0]));
			}

			if (in_string(set.first, "Sim")) {
				reader.sim.set_hom_eff(1.0);
				reader.set_sim_proton_dist_dataset(in_base_path + "Data/default/rapid05_resample_norotate_seed_dca1_nsprx1_m2r6_m2s0_nhfit20_epbins1_calcv2_0/");
				reader.sim.set_job_energy(energy * job_num);
			}
			if (in_string(set.first, { "Sim", "Eff" }, true)) {
				reader.set_sim_eff(true);
				reader.sim.set_eff();  // Can be trumped later if flow involved.
			}
			else {
				reader.set_sim_eff(false);
			}
			if (in_string(set.first, { "Sim", "_flow" }, true) || in_string(set.first, { "Sim", "_anticlflow" }, true) || 
				in_string(set.first, { "Sim", "_anticlflowindep" }, true) || in_string(set.first, { "Sim", "_simpleclust" }, true)) {
				if (in_string(set.first, {"_v2", "_res"}, true)) {
					float res = str_num_dec(get_flag_trail(set.first, "res", "_")[0], 0);
					float v2 = str_num_dec(get_flag_trail(set.first, "v2", "_")[0], 0);
					reader.sim.set_flow(v2, res);
				}
			}
			//reader.set_sim_flow(in_string(set.first, { "Sim", "_flow" }, true));
			if (in_string(set.first, { "Sim", "_flow", "Eff" }, true)) {
				reader.sim.set_eff_flow();
			}
			if (in_string(set.first, { "Sim", "simpleclust" }, true)) {
				reader.sim.set_eff_simple_clust();
			}

			if (in_string(set.first, { "Sim", "anticlust" }, true)) { reader.sim.set_anti_clust(); }
			if (in_string(set.first, { "Sim", "clmulti" }, true)) { reader.sim.set_clust_multi(); }
			if (in_string(set.first, { "Sim", "anticlmulti" }, true)) {
				reader.sim.set_clust_multi();
				reader.sim.set_amp_group(-reader.sim.get_amp_group());  // anticl so make amp negative
			}
			if (in_string(set.first, { "Sim", "anticlflow" }, true)) {
				reader.sim.set_event_clust_multi_flow();
				reader.sim.set_amp_group(-reader.sim.get_amp_group());  // anticl so make amp negative
			}
			if (in_string(set.first, { "Sim", "anticlflowindep" }, true)) {
				reader.sim.set_event_clust_multi_flow_indep();
				reader.sim.set_amp_group(-reader.sim.get_amp_group());  // anticl so make amp negative
			}
			if (in_string(set.first, { "Sim", "clmultiplusminus" }, true)) { reader.sim.set_clust_multi(); }
			if (in_string(set.first, { "Sim", "anticlfinal" }, true)) {
				reader.sim.set_clust_final();
				reader.sim.set_amp_group(-reader.sim.get_amp_group());  // anticl so make amp negative
			}

			cout << "post_sim set" << endl;


			vector<int> set_nums;
			for (int num = set.second.first; num <= set.second.second; num++) { set_nums.push_back(num); }

			for (int set_num : set_nums) {
				// Add new AzBinner and set parameters
				AzBinner& binner = reader.add_binner();
				binner.set_divs(divs);
				binner.set_cent_binning(9);

				if (in_string(set.first, "Ampt")) { binner.set_particle_bins(particle_bins["AMPT"][energy]); }
				else if (in_string(set.first, "CF")) { binner.set_particle_bins(particle_bins["CF"][energy]); }
				else if (in_string(set.first, "Sim")) {
					if (in_string(set.first, "single")) {
						binner.set_particle_min(stoi(get_flag_trail(set.first, "single", "_")[0]));
						binner.set_particle_bins(1);
					}
					else if (in_string(set.first, "flat")) {
						binner.set_particle_bins(stoi(get_flag_trail(set.first, "flat", "_")[0]));
					} else {
						binner.set_particle_bins(100);  // Placeholder for now
					}
				}
				else { binner.set_particle_bins(particle_bins["BES1"][energy]); }

				if (in_string(set.first, "Ampt")) { reader.set_ampt(true); binner.set_ampt(true); }
				if (in_string(set.first, "CF")) { reader.set_cooper_frye(true); binner.set_cooper_frye(true); }

				if (in_string(set.first, "resample")) { binner.set_resample(true); }

				if (in_string(set.first, "cbwc")) { binner.set_cbwc(true); }
				else { binner.set_cbwc(false); }

				string set_dir = set.first + to_string(set_num) + "/";

				binner.set_out_path(out_dir + set_dir);
				binner.set_qa_path(out_dir + set_dir + to_string(energy) + "GeV/");
				binner.set_qa_name("QA_");
				binner.set_set_name(set.first + to_string(set_num));


				string dca_qa_path = base_path + "Dca_xy_QA/";
				if (in_string(set.first, "dcxyqatight")) {
					dca_qa_path += "tight/";
				} else if (in_string(set.first, "dcxyqa2tight")) {
					dca_qa_path += "2tight/";
				} else if (in_string(set.first, "dcxyqaloose")) {
					dca_qa_path += "loose/";
				} else if (in_string(set.first, "dcxyqa2loose")) {
					dca_qa_path += "2loose/";
				} else {
					dca_qa_path += "default/";
				}
				binner.cut.set_dcaqa_path(dca_qa_path);
				cout << "Dca_xy_qa_path: " << binner.cut.get_dcaqa_path() << endl;

				string pileup_qa_path = base_path + "Pile_Up_QA/";
				if (in_string(set.first, "pileupqatight")) {
					pileup_qa_path += "tight/";
				} else if (in_string(set.first, "pileupqa2tight")) {
					pileup_qa_path += "2tight/";
				} else if (in_string(set.first, "pileupqaloose")) {
					pileup_qa_path += "loose/";
				} else if (in_string(set.first, "pileupqa2loose")) {
					pileup_qa_path += "2loose/";
				} else {
					pileup_qa_path += "default/";
				}
				binner.cut.set_pileupqa_path(pileup_qa_path);
				cout << "PileUp_qa_path: " << binner.cut.get_pileupqa_path() << endl;

				if (in_string(set.first, "sysrefshift")) {
					binner.set_sys_ref_shift(stoi(get_flag_trail(set.first, "sysrefshift", "_")[0]));
				}
				cout << "Sys_ref_shift: " << binner.get_sys_ref_shift() << endl;

				binner.cut.set_values(energy, reader.get_particle());  // Setting here for now since it needs updated dca/pileup paths, think of better solution.

				if (in_string(set.first, "vzlow")) {
					float vzlow = stoi(get_flag_trail(set.first, "vzlow", "_")[0]);
					if (energy == 7) { vzlow *= 50.0 / 30.0; }
					binner.cut.vz_cut.first = vzlow;
				}
				if (in_string(set.first, "vzhigh")) {  //
					float vzhigh = stoi(get_flag_trail(set.first, "vzhigh", "_")[0]);
					if (energy == 7) { vzhigh *= 50.0 / 30.0; }
					binner.cut.vz_cut.second = vzhigh;
				}
				cout << energy << " vz_low: " << binner.cut.vz_cut.first << "  vz_high: " << binner.cut.vz_cut.second << endl;


				if (in_string(set.first, "_seed")) {
					binner.set_tree_reader_rand_seed(42);
					binner.set_mixer_rand_seed(42);
					binner.set_stref_rand_seed(42);
				}
				else if (in_string(set.first, "_mixseed")) {
					binner.set_tree_reader_rand_seed(0);
					binner.set_mixer_rand_seed(42);
					binner.set_stref_rand_seed(0);
				}
				else if (in_string(set.first, "_mixnoseed")) {
					binner.set_tree_reader_rand_seed(42);
					binner.set_mixer_rand_seed(0);
					binner.set_stref_rand_seed(42);
				}
				else if (in_string(set.first, "_strefseed")) {
					binner.set_tree_reader_rand_seed(0);
					binner.set_mixer_rand_seed(0);
					binner.set_stref_rand_seed(42);
				}
				else if (in_string(set.first, "_strefnoseed")) {
					binner.set_tree_reader_rand_seed(42);
					binner.set_mixer_rand_seed(42);
					binner.set_stref_rand_seed(0);
				}
				else {
					binner.set_tree_reader_rand_seed(0);
					binner.set_mixer_rand_seed(0);
					binner.set_stref_rand_seed(0);
					if (!in_string(job_type, "_nofileshuffle")) {
						binner.set_mix_empties(false);  // Turn off saving empty events, no randomizaion fixing so might as well.
					}
				}
				cout << "Mix Empties? " << binner.get_mix_empties() << endl;

				if (in_string(set.first, "EP_Rotate")) { binner.set_event_plane(true); }
				else { binner.set_event_plane(false); }

				if (in_string(set.first, { "_norotate", "EP_Rotate" }, false)) { binner.set_rotate_random(false); }
				else { binner.set_rotate_random(true); }

				if (in_string(set.first, { "reactionplane" }, true)) { binner.set_ampt_reaction_plane(true); }

				if (in_string(set.first, { "noprerotate" }, true)) { binner.set_prerotate(false); }

				if (in_string(set.first, "calcv2"))  { binner.set_calc_v2(true); }


				// Pile up not implemented, need to think about it
				//if (in_string(set.first, "PileUp")) {
				//	binner.set_pile_up(true); 
				//	binner.set_pile_up_prob(str_num_dec(get_flag_trail(set.first, "PileUp", "_")[0], 1));
				//}

				if (in_string(set.first, "_n1ratios")) { binner.set_n1_ratios(true); binner.set_single_ratio(false); }
				else if (in_string(set.first, "_allratios")) { binner.set_n1_ratios(false); binner.set_single_ratio(false); }
				else { binner.set_n1_ratios(false); binner.set_single_ratio(true); }

				if (in_string(set.first, "dca")) {
					binner.cut.max_dca = str_num_dec(get_flag_trail(set.first, "dca", "_")[0], 1);
				}

				if (in_string(set.first, "nsprx")) {  // Multiplier for nsigmaproton cuts. Because of 27GeV can't do values, need scale factor.
					float nsigmaprx = str_num_dec(get_flag_trail(set.first, "nsprx", "_")[0], 1);
					binner.cut.min_nsigma *= nsigmaprx;
					binner.cut.max_nsigma *= nsigmaprx;
				}

				if (in_string(set.first, { "m2r", "m2s" }, false)) {  // Set m^2 cut range
					float m2_mid = (binner.cut.max_m2 + binner.cut.min_m2) / 2;
					float m2_range = binner.cut.max_m2 - binner.cut.min_m2;

					if (in_string(set.first, "m2s")) {  // m^2 shift to the mid point
						m2_mid += str_num_dec(get_flag_trail(set.first, "m2s", "_")[0], 0);
					}
					if (in_string(set.first, "m2r")) { m2_range = str_num_dec(get_flag_trail(set.first, "m2r", "_")[0], 0); }
					binner.cut.min_m2 = m2_mid - m2_range / 2;
					binner.cut.max_m2 = m2_mid + m2_range / 2;
				}

				if (in_string(set.first, "nhfit")) {
					binner.cut.min_nhits_fit = stoi(get_flag_trail(set.first, "nhfit", "_")[0]);
				}

				if (in_string(set.first, "eta")) {
					binner.set_rapidity(false);
					float eta_cut = str_num_dec(get_flag_trail(set.first, "eta", "_")[0], 1);
					binner.cut.min_eta = -eta_cut;
					binner.cut.max_eta = eta_cut;
				} else if (in_string(set.first, "rapid")) {
					binner.set_rapidity(true);
					float rap_cut = str_num_dec(get_flag_trail(set.first, "rapid", "_")[0], 1);
					binner.cut.min_rapid = -rap_cut;
					binner.cut.max_rapid = rap_cut;
				}

				if (in_string(set.first, "Efficiency")) {
					binner.set_efficiency(true);
					binner.set_efficiency_prob(str_num_dec(get_flag_trail(set.first, "Efficiency", "_")[0], 0));
				}
				else { binner.set_efficiency(false); binner.set_efficiency_prob(0); }


//				if (in_string(set.first, { "Sim", "0p0s" }, true)) { binner.sim.set_p_group(0.0); binner.sim.set_spread_sigma(0.0); }
//				else if (in_string(set.first, { "Sim", "05p002s" }, true)) { binner.sim.set_p_group(0.05); binner.sim.set_spread_sigma(0.002); }
//				else if (in_string(set.first, { "Sim", "05p05s" }, true)) { binner.sim.set_p_group(0.05); binner.sim.set_spread_sigma(0.5); }
//				else if (in_string(set.first, { "Sim", "01p002s" }, true)) { binner.sim.set_p_group(0.01); binner.sim.set_spread_sigma(0.002); }
//				else if (in_string(set.first, { "Sim", "01p05s" }, true)) { binner.sim.set_p_group(0.01); binner.sim.set_spread_sigma(0.5); }
//				else if (in_string(set.first, { "Sim", "002p002s" }, true)) { binner.sim.set_p_group(0.002); binner.sim.set_spread_sigma(0.002); }
//				else if (in_string(set.first, { "Sim", "002p05s" }, true)) { binner.sim.set_p_group(0.002); binner.sim.set_spread_sigma(0.5); }
//				else if (in_string(set.first, { "Sim", "15p002s" }, true)) { binner.sim.set_p_group(0.15); binner.sim.set_spread_sigma(0.002); }
//				else { binner.sim.set_p_group(0.0); binner.sim.set_spread_sigma(0.0); }
//
//				if (in_string(set.first, { "Sim", "Eff" }, true)) {
//					binner.set_sim_eff(true);
//				}
//				else {
//					binner.set_sim_eff(false);
//				}
//				if (in_string(set.first, { "Sim", "Eff", "Hole3-4" }, true)) {
//					binner.set_sim_eff_dist_path(in_base_path + "Sim_Efficiency_Hists.root", "Hole_3to4");
//				}
//				if (in_string(set.first, "Sim")) {
//					binner.sim.set_hom_eff(1.0);
//					binner.set_sim_proton_dist_dataset(in_base_path + "Data/eta05_n1ratios_dca30/");
//				}
//				if (in_string(set.first, { "Sim", "flow" }, true)) {
//					binner.set_sim_flow(true);
//					if (in_string(set.first, { "08res", "05v2" }, true)) { binner.sim.set_flow(0.05, 0.8, 0.0001); }
//					else if (in_string(set.first, { "05res", "05v2" }, true)) { binner.sim.set_flow(0.05, 0.5, 0.0001); }
//					else if (in_string(set.first, { "099res", "05v2" }, true)) { binner.sim.set_flow(0.05, 0.99, 0.0001); }
//					else if (in_string(set.first, "05v2")) { binner.sim.set_flow(0.05, 0.0, 0.0001); }
//				}
//				else {
//					binner.set_sim_flow(false);
//				}
//				if (in_string(set.first, { "Sim", "Flat1000" }, true)) { binner.sim.set_flat_dist(1000); binner.set_particle_dist_hist_max(1000); }
//				else if (in_string(set.first, { "Sim", "Flat500" }, true)) { binner.sim.set_flat_dist(500); binner.set_particle_dist_hist_max(500); }
//				else if (in_string(set.first, { "Sim", "Flat100" }, true)) { binner.sim.set_flat_dist(100); binner.set_particle_dist_hist_max(100); }

				if (in_string(set.first, "Ampt")) {
					if (in_string(set.first, "_default_")) {
						binner.set_ampt_type("default");
					}
					else {
						binner.set_ampt_type("string_melting");
					}
					binner.set_ampt_cent_path(base_path + "Ampt_Centralities/");
				}

				if (in_string(set.first, "p+")) { binner.cut.charge = +1; binner.set_ampt_particle_pid({ 2212 }); }
				if (in_string(set.first, "p-")) { binner.cut.charge = -1; binner.set_ampt_particle_pid({ -2212 }); }
				if (in_string(set.first, "ptotal")) { binner.set_check_charge(false); }

				if (!in_string(set.first, "Ampt") && in_string(set.first, "pion")) {
					binner.set_particle("pion");
				}
				if (in_string(set.first, "pion+")) { binner.cut.charge = 1; binner.set_ampt_particle_pid({ 211 }); }
				if (in_string(set.first, "pion-")) { binner.cut.charge = -1; binner.set_ampt_particle_pid({ -211 }); }
				if (in_string(set.first, "pion0")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 111 }); }
				if (in_string(set.first, "piontotal")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 211, -211 }); }
				if (in_string(set.first, "pionall")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 211, 111, -211 }); }

				if (in_string(set.first, "e+")) { binner.cut.charge = +1; binner.set_ampt_particle_pid({ -11 }); }
				else if (in_string(set.first, "e-")) { binner.cut.charge = -1; binner.set_ampt_particle_pid({ 11 }); }
				else if (in_string(set.first, "etotal")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 11, -11 }); }

				if (in_string(set.first, "neutron")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 2112 }); }
				if (in_string(set.first, "neutronbar")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ -2112 }); }
				if (in_string(set.first, "neutrontotal")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 2112, -2112 }); }

				if (in_string(set.first, "gamma")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 22 }); }

				if (in_string(set.first, "baryontotal")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 2212, 2112, -2212, -2112 }); }
				if (in_string(set.first, "mesontotal")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 211, 111, -211 }); }
				if (in_string(set.first, "hadrontotal")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 2212, 2112, 211, 111, -211, -2212, -2112 }); }
				if (in_string(set.first, "hadronobs")) { binner.set_check_charge(false); binner.set_ampt_particle_pid({ 2212, 211, -211, -2212 }); }

				// Sim
				if (in_string(set.first, { "Sim", "Flat1000" }, true)) { binner.set_particle_dist_hist_max(1000); }
				else if (in_string(set.first, { "Sim", "Flat500" }, true)) { binner.set_particle_dist_hist_max(500); }
				else if (in_string(set.first, { "Sim", "Flat100" }, true)) { binner.set_particle_dist_hist_max(100); }

				binner.set_mixed(true);
				binner.mix.set_divs(divs);
				binner.mix.set_out_path(mix_out_dir + set_dir);
				binner.mix.set_max_events(250);
				binner.mix.set_min_events(150);
				binner.mix.set_vz_bins(5);
				binner.mix.set_ep_bins(20);

				if (in_string(set.first, "epbins")) {
					binner.mix.set_ep_bins(stoi(get_flag_trail(set.first, "epbins", "_")[0]));
				}

				if (in_string(set.first, "nbootstrap")) {
					binner.set_n_bootstraps(stoi(get_flag_trail(set.first, "nbootstrap", "_")[0]));
					cout << binner.get_n_bootstraps() << endl;
				}

				if (in_string(set.first, "qaonly")) {  // If qa only don't do any binning or mixing, just run though data for qa plots
					binner.set_divs({});
					binner.set_mixed(false);
				}

				if (in_string(set.first, "nomix")) {
					binner.set_mixed(false);
					cout << "Mixed? " << binner.get_mixed() << endl;
				}

				if (in_string(set.first, {"Ampt", "CF_", "CFEV_", "CFEVb342_"}, false)) { binner.mix.set_mixes_per_event(10); }
				else {
					if (energy <= 11) { binner.mix.set_mixes_per_event(50); }
					//if (energy <= 11) { binner.mix.set_mixes_per_event(2); }  // DEBUG ONLY!
					else { binner.mix.set_mixes_per_event(11); }
					//binner.mix.set_mixes_per_event(11);  // DEBUG ONLY!
				}
				if (in_string(set.first, "Sim") || in_string(set.first, "Ampt")) { binner.mix.set_mixes_per_event(10); }

				cout << "Added " << set.first << " set " << set_num << " " << energy << "GeV, job " << job_num << " of " << jobs << endl << endl;
			}

		}
	}

	if (in_string(job_type, "Sim")) {
		reader.sim_events(sim_cent_events);
	}
	else {
		reader.read_trees();
	}
}


void yunshan_example() {
    int energy = 11;  // Energy in GeV
    int ref = 3;  // Which refmult to use
    string in_path = "/star/data01/pwg/yunshancheng/ProtonPartition/trees/final/output/";  // Input path for trees
    string out_dir = "/star/data01/pwg/yunshancheng/ProtonPartition/QGP_Fluctuations/Tree_Reader/output/";  // Output directory for binned data
    TreeReader reader(energy, ref);
    reader.set_in_path(in_path);
    reader.set_tree_name("tree");

    // Might fail on Mixer::append_event if these are too low?
    map<string, map<int, int>> particle_bins { {"BES1", { {7, 55}, {11, 44}, {19, 38}, {27, 36}, {39, 32}, {62, 29} } },
                                               {"AMPT", { {7, 100}, {11, 100}, {19, 100}, {27, 100}, {39, 90}, {62, 80} } },
                                               {"CF", { {7, 80}, {19, 80}, {27, 70}, {39, 70}, {62, 70} } },
    };

    vector<int> divs{ 356, 300, 288, 270, 240, 180, 120, 90, 89, 72, 60 };

    AzBinner& binner = reader.add_binner();
    binner.set_divs(divs);
    binner.set_cent_binning(9);

    binner.set_particle_bins(particle_bins["BES1"][energy]);  // Just to preset bins, improves performance

    binner.set_out_path(out_dir);
    binner.set_qa_path(out_dir);
    binner.set_qa_name("QA_");
    binner.cut.set_dcaqa_path(out_dir);
    binner.cut.set_pileupqa_path(out_dir);

    binner.cut.set_values(energy, reader.get_particle());  // Setting here for now since it needs updated dca/pileup paths, think of better solution.
    binner.set_rotate_random(false);
    binner.set_calc_v2(true);
    binner.set_efficiency(false);
    binner.set_efficiency_prob(0);

    binner.set_divs({});
    binner.set_mixed(false);

    binner.set_mixed(true);
    binner.mix.set_divs(divs);
    binner.mix.set_out_path(out_dir);
    binner.mix.set_ep_bins(1);
    binner.mix.set_max_events(250);
    binner.mix.set_min_events(150);
    binner.mix.set_vz_bins(5);
    binner.mix.set_ep_bins(20);

    binner.set_tree_reader_rand_seed(0);
    binner.set_mixer_rand_seed(0);
    binner.set_stref_rand_seed(0);
    binner.set_event_plane(false);
    binner.set_rotate_random(false);
    binner.set_rapidity(true);
    binner.set_efficiency(false);

    binner.mix.set_mixes_per_event(11);

    reader.read_trees();
}

//void run_set(int energy, int set_num, string set_name, int job_num, int jobs, mutex *mtx, vector<string> *file_list) {
//	string base_path, in_base_path, in_base_ampt_path;
//	if (platform == "lin") {
//		base_path = "/home/dylan/Research/";
//		in_base_path = "/media/ucla/Research/";
//		in_base_ampt_path = "/media/ssd/Research/";
//	} else if (platform == "win") {
//		base_path = "C:/Users/Dylan/Desktop/Research/";
//		in_base_path = "C:/Users/Dylan/Desktop/Research/";
//		in_base_ampt_path = "E:/Research/";
//	}
//	string out_base_path = base_path;
//	int ref = 3;
//
//	string in_path = in_base_path;
//	string out_dir = out_base_path;
//	string mix_out_dir = out_base_path;
//
//	if(in_string(set_name, "Ampt")) {
//		in_path = in_base_ampt_path + "AMPT_Trees/";
//		out_dir += "Data_Ampt/";
//		mix_out_dir += "Data_Ampt_Mix/";
//	} else if(in_string(set_name, "Sim")) {
//		in_path += "BES1_Trees/";
//		out_dir += "Data_Sim/";
//		mix_out_dir += "Data_Sim_Mix/";
//	} else {
//		in_path += "BES1_Trees/";
//		out_dir += "Data/";
//		mix_out_dir += "Data_Mix/";
//	}
//
//	vector<int> divs {356, 300, 288, 270, 240, 180, 120, 90, 89, 72, 60};
////	map<int, int> sim_cent_events = {{0, 500000}, {1, 500000}, {2, 500000}, {3, 500000}, {4, 500000}, {5, 500000}, {6, 500000}, {7, 500000}, {8, 20000000}};
//	map<int, int> sim_cent_events = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 100000000}};
////	map<int, int> sim_cent_events = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 200000}};
//
//	string set_dir = set_name + to_string(set_num) + "/";
//
//	TreeReader reader(energy, ref, mtx);
//
//	reader.cut.set_dcaqa_path(base_path + "Dca_xy_QA/");
//	reader.cut.set_pileupqa_path(base_path + "Pile_Up_QA/");
//
//	reader.set_divs(divs);
//
//	if(in_string(set_name, "Ampt")) { reader.set_ampt(true); }
//
//	if(in_string(set_name, "cbwc")) { reader.set_cbwc(true); }
//	else { reader.set_cbwc(false); }
//	reader.set_cent_binning(9);
//
//	reader.set_in_path(in_path);
//	reader.set_out_path(out_dir+set_dir);
//	reader.set_qa_path(out_dir+set_dir+to_string(energy)+"GeV/");
//	reader.set_qa_name("QA_");
//	reader.set_set_name(set_name + to_string(set_num));
//	reader.set_tree_name("tree");
//
//	if(in_string(set_name, "_seed")) {
//		reader.set_tree_reader_rand_seed(42);
//		reader.set_mixer_rand_seed(42);
//		reader.set_file_shuffle_rand_seed(42);
//		reader.set_stref_rand_seed(42);
//	} else if(in_string(set_name, "_mixseed")) {
//		reader.set_tree_reader_rand_seed(0);
//		reader.set_mixer_rand_seed(42);
//		reader.set_file_shuffle_rand_seed(time(NULL));
//		reader.set_stref_rand_seed(0);
//	} else if(in_string(set_name, "_mixnoseed")) {
//		reader.set_tree_reader_rand_seed(42);
//		reader.set_mixer_rand_seed(0);
//		reader.set_file_shuffle_rand_seed(42);
//		reader.set_stref_rand_seed(42);
//	} else {
//		reader.set_tree_reader_rand_seed(0);
//		reader.set_mixer_rand_seed(0);
//		reader.set_file_shuffle_rand_seed(time(NULL));
//		reader.set_stref_rand_seed(0);
//	}
//
//	reader.set_file_list(file_list);
//
//	if(in_string(set_name, "EP_Rotate"))  { reader.set_event_plane(true); }
//	else { reader.set_event_plane(false); }
//
//	if(in_string(set_name, {"No_Rotate", "EP_Rotate"}, false)) { reader.set_rotate_random(false); }
//	else{ reader.set_rotate_random(true); }
//
//	if(in_string(set_name, {"Ampt", "ReactionPlane"}, true)) { reader.set_ampt_reaction_plane(true); }
//
//	//if(in_string(set_name, "dca")) {
//	//	reader.cut.max_dca = str_num_dec(get_flag_trail(set_name, "dca", "_")[0], 1);
//	//}
//
//	if(in_string(set_name, "Pile_Up_01_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.01); }
//	else if(in_string(set_name, "Pile_Up_008_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.008); }
//	else if(in_string(set_name, "Pile_Up_005_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.005); }
//	else if(in_string(set_name, "Pile_Up_002_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.002); }
//	else if(in_string(set_name, "Pile_Up_001_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.001); }
//	else if(in_string(set_name, "Pile_Up_0008_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.0008); }
//	else if(in_string(set_name, "Pile_Up_0005_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.0005); }
//	else if(in_string(set_name, "Pile_Up_0002_")) { reader.set_pile_up(true); reader.set_pile_up_prob(0.0002); }
//	else { reader.set_pile_up(false); reader.set_pile_up_prob(0); }
//
////	if(in_string(set_name,  "No_BTof_Rej")) {
////		reader.cut.max_slope = {{7, 999}, {11, 999}, {14, 999}, {19, 999}, {27, 999}, {39, 999}, {62, 999}, {200, 999}};
////		reader.cut.min_slope = {{7, -999}, {11, -999}, {14, -999}, {19, -999}, {27, -999}, {39, -999}, {62, -999}, {200, -999}};
////	}
//
//	if(in_string(set_name, "_n1ratios")) { reader.set_n1_ratios(true); reader.set_single_ratio(false); }
//	if(in_string(set_name, "_allratios")) { reader.set_n1_ratios(false); reader.set_single_ratio(false); }
//	else { reader.set_n1_ratios(false); reader.set_single_ratio(true); }
//
//	if(in_string(set_name, "dca")) {
//		reader.cut.max_dca = str_num_dec(get_flag_trail(set_name, "dca", "_")[0], 1);
//		cout << "max dca: " << reader.cut.max_dca << endl;
//	}
//
//	if(in_string(set_name, "nsprx")) {  // Multiplier for nsigmaproton cuts. Because of 27GeV can't do values, need scale factor.
//		float nsigmaprx = str_num_dec(get_flag_trail(set_name, "nsprx", "_")[0], 1);
//		reader.cut.min_nsigma *= nsigmaprx;
//		reader.cut.max_nsigma *= nsigmaprx;
//		cout << "nsigma_cuts: " << reader.cut.min_nsigma << " " << reader.cut.max_nsigma << endl;
//	}
//
//	if(in_string(set_name, {"m2r", "m2s"}, false)) {  // Set m^2 cut range
//		float m2_mid = (reader.cut.max_m2 + reader.cut.min_m2) / 2;
//		float m2_range = reader.cut.max_m2 - reader.cut.min_m2;
//
//		if(in_string(set_name, "m2s")) {  // m^2 shift to the mid point
//			m2_mid += str_num_dec(get_flag_trail(set_name, "m2s", "_")[0], 0);
//		}
//		if(in_string(set_name, "m2r")) { m2_range = str_num_dec(get_flag_trail(set_name, "m2r", "_")[0], 0); }
//		reader.cut.min_m2 = m2_mid - m2_range / 2;
//		reader.cut.max_m2 = m2_mid + m2_range / 2;
//		cout << "m^2_cuts: " << reader.cut.min_m2 << " " << reader.cut.max_m2 << endl;
//	}
//
//	if(in_string(set_name, "eta1")) { reader.cut.min_eta = -1.0, reader.cut.max_eta = 1.0; }
//	else if(in_string(set_name, "eta05")) { reader.cut.min_eta = -0.5, reader.cut.max_eta = 0.5; }
////	else if(in_string(set_name, "rapid1")) { reader.set_rapidity(true); reader.cut.min_rapid = -1.0; reader.cut.max_rapid = 1.0; }
////	else if(in_string(set_name, "rapid05")) { reader.set_rapidity(true); reader.cut.min_rapid = -0.5; reader.cut.max_rapid = 0.5; }
//	else if(in_string(set_name, "rapid")) {
//		float rap_cut = str_num_dec(get_flag_trail(set_name, "rapid", "_")[0], 1);
//		reader.cut.min_rapid = -rap_cut;
//		reader.cut.max_rapid = rap_cut;
//	}
//
//	if(in_string(set_name, "Efficiency")) {
//		reader.set_efficiency(true);
//		reader.set_efficiency_prob(str_num_dec(get_flag_trail(set_name, "Efficiency", "_")[0], 0));
//	} else { reader.set_efficiency(false); reader.set_efficiency_prob(0); }
//
////	if(in_string(set_name,  "Efficiency8")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.8); }
////	else if(in_string(set_name,  "Efficiency7")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.7); }
////	else if(in_string(set_name,  "Efficiency6")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.6); }
////	else if(in_string(set_name,  "Efficiency5")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.5); }
////	else if(in_string(set_name,  "Efficiency4")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.4); }
////	else if(in_string(set_name,  "Efficiency3")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.3); }
////	else if(in_string(set_name,  "Efficiency2")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.2); }
////	else if(in_string(set_name,  "Efficiency1")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.1); }
////	else if(in_string(set_name,  "Efficiency08")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.08); }
////	else if(in_string(set_name,  "Efficiency05")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.05); }
////	else if(in_string(set_name,  "Efficiency025")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.025); }
////	else if(in_string(set_name,  "Efficiency01")) { reader.set_efficiency(true); reader.set_efficiency_prob(0.01); }
////	else { reader.set_efficiency(false); reader.set_efficiency_prob(0); }
//
//	if(in_string(set_name, {"Sim", "0p0s"}, true)) { reader.sim.set_p_group(0.0); reader.sim.set_spread_sigma(0.0); }
//	else if(in_string(set_name, {"Sim", "05p002s"}, true)) { reader.sim.set_p_group(0.05); reader.sim.set_spread_sigma(0.002); }
//	else if(in_string(set_name, {"Sim", "05p05s"}, true)) { reader.sim.set_p_group(0.05); reader.sim.set_spread_sigma(0.5); }
//	else if(in_string(set_name, {"Sim", "01p002s"}, true)) { reader.sim.set_p_group(0.01); reader.sim.set_spread_sigma(0.002); }
//	else if(in_string(set_name, {"Sim", "01p05s"}, true)) { reader.sim.set_p_group(0.01); reader.sim.set_spread_sigma(0.5); }
//	else if(in_string(set_name, {"Sim", "002p002s"}, true)) { reader.sim.set_p_group(0.002); reader.sim.set_spread_sigma(0.002); }
//	else if(in_string(set_name, {"Sim", "002p05s"}, true)) { reader.sim.set_p_group(0.002); reader.sim.set_spread_sigma(0.5); }
//	else if(in_string(set_name, {"Sim", "15p002s"}, true)) { reader.sim.set_p_group(0.15); reader.sim.set_spread_sigma(0.002); }
//	else { reader.sim.set_p_group(0.0); reader.sim.set_spread_sigma(0.0); }
//
//	if(in_string(set_name, {"Sim", "Eff"}, true)) {
//		reader.set_sim_eff(true);
//	} else {
//		reader.set_sim_eff(false);
//	}
//	if(in_string(set_name, {"Sim", "Eff", "Hole3-4"}, true)) {
//		reader.set_sim_eff_dist_path(in_base_path + "Sim_Efficiency_Hists.root", "Hole_3to4");
//	}
//	if(in_string(set_name, "Sim")) {
//		reader.sim.set_hom_eff(1.0);
//		reader.set_sim_proton_dist_dataset(in_base_path + "Data/eta05_n1ratios_dca30/");
//	}
//	if(in_string(set_name, {"Sim", "flow"}, true)) {
//		reader.set_sim_flow(true);
//		if(in_string(set_name, {"08res", "05v2"}, true)) { reader.sim.set_flow(0.05, 0.8, 0.0001); }
//		else if(in_string(set_name, {"05res", "05v2"}, true)) { reader.sim.set_flow(0.05, 0.5, 0.0001); }
//		else if(in_string(set_name, {"099res", "05v2"}, true)) { reader.sim.set_flow(0.05, 0.99, 0.0001); }
//		else if(in_string(set_name, "05v2")) { reader.sim.set_flow(0.05, 0.0, 0.0001); }
//	} else {
//		reader.set_sim_flow(false);
//	}
//	if(in_string(set_name, {"Sim", "Flat1000"}, true)) { reader.sim.set_flat_dist(1000); reader.set_particle_dist_hist_max(1000); }
//	else if(in_string(set_name, {"Sim", "Flat500"}, true)) { reader.sim.set_flat_dist(500); reader.set_particle_dist_hist_max(500); }
//	else if(in_string(set_name, {"Sim", "Flat100"}, true)) { reader.sim.set_flat_dist(100); reader.set_particle_dist_hist_max(100); }
//
//	if(in_string(set_name, "Ampt")) {
//		if(in_string(set_name, "_default_")) {
//			reader.set_ampt_type("default");
//		} else {
//			reader.set_ampt_type("string_melting");
//		}
//		reader.set_ampt_cent_path(base_path + "Ampt_Centralities/");
//	}
//
//	if(in_string(set_name, "p+")) { reader.cut.charge = +1; reader.set_ampt_particle_pid({2212}); }
//	if(in_string(set_name, "p-")) { reader.cut.charge = -1; reader.set_ampt_particle_pid({-2212}); }
//	if(in_string(set_name, "ptotal")) { reader.set_check_charge(false); }
//
//	if(!in_string(set_name, "Ampt") && in_string(set_name, "pion")) {
//		reader.set_particle("pion");
//	}
//	if(in_string(set_name, "pion+")) { reader.cut.charge = 1; reader.set_ampt_particle_pid({211}); }
//	if(in_string(set_name, "pion-")) { reader.cut.charge = -1; reader.set_ampt_particle_pid({-211}); }
//	if(in_string(set_name, "pion0")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({111}); }
//	if(in_string(set_name, "piontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({211, -211}); }
//	if(in_string(set_name, "pionall")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({211, 111, -211}); }
//
//	if(in_string(set_name, "e+")) { reader.cut.charge = +1; reader.set_ampt_particle_pid({-11}); }
//	else if(in_string(set_name, "e-")) { reader.cut.charge = -1; reader.set_ampt_particle_pid({11}); }
//	else if(in_string(set_name, "etotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({11, -11}); }
//
//	if(in_string(set_name, "neutron")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2112}); }
//	if(in_string(set_name, "neutronbar")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({-2112}); }
//	if(in_string(set_name, "neutrontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2112, -2112}); }
//
//	if(in_string(set_name, "gamma")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({22}); }
//
//	if(in_string(set_name, "baryontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2212, 2112, -2212, -2112}); }
//	if(in_string(set_name, "mesontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({211, 111, -211}); }
//	if(in_string(set_name, "hadrontotal")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2212, 2112, 211, 111, -211, -2212, -2112}); }
//	if(in_string(set_name, "hadronobs")) { reader.set_check_charge(false); reader.set_ampt_particle_pid({2212, 211, -211, -2212}); }
//
//	reader.set_mixed_sets(false);
//	reader.set_rand_data(false);
//
//	reader.set_mixed(true);
//	reader.mix.set_divs(divs);
//	reader.mix.set_out_path(mix_out_dir+set_dir);
//	reader.mix.set_max_events(250);
//	reader.mix.set_min_events(150);
//	reader.mix.set_vz_bins(1);
//	reader.mix.set_ep_bins(180);
//	if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
//	else { reader.mix.set_mixes_per_event(10); }
//	if(in_string(set_name, "Sim") || in_string(set_name, "Ampt")) { reader.mix.set_mixes_per_event(10); }
//
//	if(in_string(set_name, "Sim")) {
//		reader.sim_events(sim_cent_events);
//	} else{
//		reader.read_trees();
//	}
//
//	cout << "Finished " << set_name << " set " << set_num << " " << energy <<  "GeV, job " << job_num << " of " << jobs << endl << endl;
//}


void ampt_cent_opt() {
	string min_bias_path = "/media/ucla/Research/AMPT_Trees/min_bias/string_melting/";
	string star_data_path = "/media/ucla/Research/BES1_Trees/";
	string qa_path = "/media/ucla/Research/Ampt_Centralities/string_melting/";
//	string min_bias_path = "F:/Research/AMPT_Trees/min_bias/string_melting/";
//	string star_data_path = "F:/Research/BES1_Trees/";
//	string qa_path = "F:/Research/Ampt_Centralities/string_melting/";
	string ref_quantity = "ref3";
	AmptCentralityMaker cent_maker;

	cent_maker.set_min_bias_path(min_bias_path);
	cent_maker.set_star_data_path(star_data_path);
	cent_maker.set_qa_path(qa_path);
	cent_maker.set_mult_quantity(ref_quantity);

	cent_maker.run_b_opt({7, 11, 19, 27, 39, 62});
	cent_maker.plot_ref_vs_b({7, 11, 19, 27, 39, 62});
}

void ampt_ref_b_plot() {
	string min_bias_path = "/media/ucla/Research/AMPT_Trees/min_bias/string_melting/";
	string star_data_path = "/media/ucla/Research/BES1_Trees/";
	string qa_path = "/media/ucla/Research/Ampt_Centralities/string_melting/";
//	string min_bias_path = "F:/Research/AMPT_Trees/min_bias/string_melting/";
//	string star_data_path = "F:/Research/BES1_Trees/";
//	string qa_path = "F:/Research/Ampt_Centralities/string_melting/";
	string ref_quantity = "ref3";
	AmptCentralityMaker cent_maker;

	cent_maker.set_min_bias_path(min_bias_path);
	cent_maker.set_star_data_path(star_data_path);
	cent_maker.set_qa_path(qa_path);
	cent_maker.set_mult_quantity(ref_quantity);

	cent_maker.plot_ref_vs_b({7, 11, 19, 27, 39, 62});
}

void ampt_cent_make() {
	vector<int> energy_list {7, 11, 19, 27, 39, 62};
	string min_bias_path = "/media/ucla/Research/AMPT_Trees/min_bias/string_melting/";
	string qa_path = "/media/ucla/Research/Ampt_Centralities/string_melting/";
//	string min_bias_path = "/home/dylan/Research/AMPT_Trees/min_bias/string_melting/";
//	string qa_path = "/home/dylan/Research/Ampt_Centralities/string_melting/";
//	string min_bias_path = "F:/Research/AMPT_Trees/min_bias/string_melting/";
//	string qa_path = "F:/Research/Ampt_Centralities/string_melting/";
	string ref_quantity = "ref3";
	for(int energy:energy_list) {
		AmptCentralityMaker cent_maker(energy, min_bias_path, qa_path, ref_quantity);
		cout << "Make energy " << energy << "GeV" << endl;
		cent_maker.make_centrality(true);
		cout << "Read energy " << energy << "GeV" << endl;
		vector<int> edges = cent_maker.get_ref_bin9_edges();
		for(int edge:edges) { cout << edge << " " << flush; }
		cout << endl;
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


//void read_comb_sys() {
//	string out_dir = "/home/dylan/Research/Data/";
//	string mix_sets_out_dir = "/home/dylan/Research/Data_Mix_Sets/";
//	string mix_out_dir = "/home/dylan/Research/Data_Mix/";
//	string random_out_dir = "/home/dylan/Research/Data_Random/";
//
//	map<string, pair<int, int>> set_pairs = {{"Final_Set",{0,100}}};
//	pair<float, float> pile_up_range = {0.0, 0.01};
//	pair<float, float> efficiency_range = {0.0, 0.05};
//	TRandom3 sys_rand(0);
//	int sleep = 60;
//
//	ROOT::EnableThreadSafety();
//	{
//		ThreadPool pool(thread::hardware_concurrency());
//		for(pair<string, pair<int, int>> set_pair:set_pairs) {
//			for(int set_num = set_pair.second.first; set_num <= set_pair.second.second; set_num++) {
//				string set_dir = set_pair.first + to_string(set_num) + "/";
//				cout << endl << "Queueing " + set_dir <<  "  set_num: " << set_num << endl << endl;
//				vector<int> energy_list = {39, 27, 62, 19, 11, 7};
//				vector<int> divs = {2, 3, 4, 5, 6};
//				for(int energy:energy_list) {
//					TreeReader reader(energy);
//					reader.set_divs(divs);
//					reader.set_cbwc(false);
//					reader.set_cent_binning(9);
//
//					reader.set_in_path("/home/dylan/Research/Trees/");
//					reader.set_out_path(out_dir+set_dir);
//					reader.set_qa_path(out_dir+set_dir+to_string(energy)+"GeV/");
//					reader.set_qa_name("QA_");
//					reader.set_set_name(set_pair.first + to_string(set_num));
//
//					reader.set_event_plane(false);
//					reader.set_rotate_random(true);
//
//					reader.set_pile_up(true);
//					reader.set_pile_up_prob(sys_rand.Rndm() * (pile_up_range.second - pile_up_range.first) + pile_up_range.second);
//
//					reader.set_efficiency(true);
//					reader.set_efficiency_prob(sys_rand.Rndm() * (efficiency_range.second - efficiency_range.first) + efficiency_range.second);
//
//					reader.set_mixed_sets(false);
//					reader.set_rand_data(false);
//
//					reader.set_mixed(true);
//					reader.mix.set_divs(divs);
//					reader.mix.set_out_path(mix_out_dir+set_dir);
//					reader.mix.set_max_events(250);
//					reader.mix.set_min_events(150);
//					if(energy <= 11) { reader.mix.set_mixes_per_event(50); }
//					else { reader.mix.set_mixes_per_event(10); }
//					pool.enqueue(&TreeReader::read_trees, reader);
//				}
//				this_thread::sleep_for(chrono::seconds(sleep));
//			}
//		}
//	}
//}


void ref_mult_test() {
	StRefMultCorr *refmultCorrUtil;
	refmultCorrUtil = new StRefMultCorr("refmult3");
	refmultCorrUtil->init(11079000);
	refmultCorrUtil->print();
//	refmultCorrUtil->initEvent((int)event.get_refn(), (double)event.get_vz());
//	int cent_check = refmultCorrUtil->getCentralityBin9();
}


void dca_xy_qa(int energy, mutex *mtx, string in_path, string qa_path, string pile_up_qa_path, string variation = "default") {
	{
		DcaxyQAer qa(energy, mtx);
		qa.set_in_path(in_path);
		qa_path += variation + "/";
		qa.set_qa_path(qa_path);
		qa.set_pile_up_qa_path(pile_up_qa_path);
		if (variation == "tight") {
			qa.pars.mv_avg_pars = { {1, {4.0, 0.2}}, {5, {4.5, 2.0}}, {10, {7.0, 4.0}} };
		}
		else if (variation == "2tight") {
			qa.pars.mv_avg_pars = { {1, {3.0, 0.2}}, {5, {3.5, 2.0}}, {10, {6.0, 4.0}} };
		}
		else if (variation == "loose") {
			qa.pars.mv_avg_pars = { {1, {6.0, 0.2}}, {5, {6.5, 2.0}}, {10, {9.0, 4.0}} };
		}
		else if (variation == "2loose") {
			qa.pars.mv_avg_pars = { {1, {7.0, 0.2}}, {5, {7.5, 2.0}}, {10, {10.0, 4.0}} };
		}
		qa.run_qa();
	}
//	DcaxyQAer qa(energy);
//	map<int, vector<pair<int, int>>> bad_ranges = qa.get_bad_ranges();
//	for(pair<int, vector<pair<int, int>>> run:bad_ranges) {
//		cout << "Run " << run.first << ":\t" << flush;
//		for(pair<int, int> range:run.second) {
//			cout << "[" << range.first << " , " << range.second << "]\t" << flush;
//		}
//		cout << endl;
//	}
}

void run_dca_xy_qa() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62};
//	vector<int> energies{ 62, 19, 11, 7 };
//	string in_path = "F:/Research/BES1_Trees/";
//	string qa_path = "F:/Research/Dca_xy_QA/";
//	string pile_up_qa_path = "F:/Research/Pile_Up_QA/";
	string in_path = "/media/ucla/Research/BES1_Trees/";
	string qa_path = "/media/ucla/Research/Dca_xy_QA/";
	string pile_up_qa_path = "/media/ucla/Research/Pile_Up_QA/";
	string variation = "2loose";

	mutex *mtx = new mutex;

//	for (int energy : energies) {
//		dca_xy_qa(energy, mtx, in_path, qa_path, pile_up_qa_path, variation);
//	}

	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency() - 1);
		for(int energy:energies) {
			pool.enqueue(dca_xy_qa, energy, mtx, in_path, qa_path, pile_up_qa_path, variation);
			this_thread::sleep_for(chrono::seconds(1));
		}
	}
}



void pile_up_qa(int energy, mutex *mtx, string in_path, string qa_path, string variation="default") {
	{
		PileUpQAer qa(energy, mtx);
		qa.set_in_path(in_path);

		if (variation == "tight") {
			qa.set_sigmas_left(2);
			qa.set_sigmas_right(2);
		} else if (variation == "2tight") {
			qa.set_sigmas_left(1.5);
			qa.set_sigmas_right(1.5);
		} else if (variation == "loose") {
			qa.set_sigmas_left(4);
			qa.set_sigmas_right(4);
		} else if (variation == "2loose") {
			qa.set_sigmas_left(5);
			qa.set_sigmas_right(5);
		}
		qa_path += variation + "/";
		qa.set_out_path(qa_path);

		qa.run_qa();
	}
	{
		PileUpQAer qa(energy, mtx);
		qa.set_in_path(in_path);
		qa.set_out_path(qa_path);
		auto low = qa.get_low_cut();
		auto high = qa.get_high_cut();
		cout << "low: " << endl << "poly coefs: ";
		for(unsigned coef=0; coef < low.pol_fit_coefs.size(); coef++) {
			cout << low.pol_fit_coefs[coef] << ", ";
		}
		cout << endl << "max_ref: " << low.max_fit_ref << endl;
		cout << "lin_extrap: " << low.lin_extrap.first << " + " << low.lin_extrap.second << " * x" << endl;
		cout << endl << "high: " << endl << "poly coefs: ";
		for(unsigned coef=0; coef < high.pol_fit_coefs.size(); coef++) {
			cout << high.pol_fit_coefs[coef] << ", ";
		}
		cout << endl << "max_ref: " << high.max_fit_ref << endl;
		cout << "lin_extrap: " << high.lin_extrap.first << " + " << high.lin_extrap.second << " * x" << endl;
		cout << endl << endl;
	}
//	DcaxyQAer qa(energy);
//	map<int, vector<pair<int, int>>> bad_ranges = qa.get_bad_ranges();
//	for(pair<int, vector<pair<int, int>>> run:bad_ranges) {
//		cout << "Run " << run.first << ":\t" << flush;
//		for(pair<int, int> range:run.second) {
//			cout << "[" << range.first << " , " << range.second << "]\t" << flush;
//		}
//		cout << endl;
//	}
}

void run_pile_up_qa() {
//	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	vector<int> energies{ 39, 62, 27, 19, 11, 7 };
//	string in_path = "F:/Research/BES1_Trees/";
//	string qa_path = "F:/Research/Pile_Up_QA/";
	string in_path = "/media/ucla/Research/BES1_Trees/";
	string qa_path = "/media/ucla/Research/Pile_Up_QA/";
	mutex *mtx = new mutex;

	gErrorIgnoreLevel = kError;

//	string variation = "default";
//	vector<string> variations {"default", "tight", "2tight", "loose", "2loose"};
	vector<string> variations { "2tight", "2loose"};

	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency() - 1);
		for (string variation:variations) {
			for(int energy:energies) {
				pool.enqueue(pile_up_qa, energy, mtx, in_path, qa_path, variation);
				this_thread::sleep_for(chrono::seconds(1));
			}
		}
	}
}


void tchain_test() {
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(thread::hardware_concurrency());
		pool.enqueue(run_tchain);
		pool.enqueue(run_tchain);
	}
}

void run_tchain() {
	string in_path = "/media/ucla/Research/BES1_Trees/";
	int energy = 7;

	TChain chain("tree");
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV/", "root", "path");
	for(string file:in_files) {
		chain.Add(file.data());
	}

	int event_id;
	chain.SetBranchAddress("event_id", &event_id);

	int event_index = -1;
	while(chain.GetEntry(++event_index)) {
		cout << event_index << ": " << event_id << endl;
	}
}

void get_flag_trail_test() {
	string a = "rapid05_dca13214_hero45545";
	string b = "dca";
	for(string c:get_flag_trail(a,b,"_")) { cout << str_num_dec(c, 2) << endl; }
}

map<string, pair<int, int>> get_rand_set_pairs(int num_pairs, string rapid, string efficiency) {
	TRandom3 *rand = new TRandom3(0);
	map<string, pair<int, int>> set_pairs = {};

	map<string, pair<pair<int, unsigned>, pair<float, float>>> set_ranges;  // flag: {multiplier, num_dec}: {min_val, max_val}
	set_ranges["dca"] = make_pair(make_pair(100, 3), make_pair(0.8, 1.2));
	set_ranges["m2r"] = make_pair(make_pair(100, 3), make_pair(2.0, 6.0));
	set_ranges["nsprx"] = make_pair(make_pair(100, 3), make_pair(0.9, 1.1));
	set_ranges["nhfit"] = make_pair(make_pair(1, 2), make_pair(14.5001, 25.4999));

	string name_post;
	if (efficiency == "") {
		name_post = "m2s0_sys_";
	}
	else {
		name_post = "m2s0_" + efficiency + "_sys_";
	}
	pair<int, int> set_nums = {0,0};

	//rapid05_resample_norotate_dca05_nsprx1_m2r6_m2s0_nhfit20_epbins1_
	for(int i = 0; i < num_pairs; i++) {
		string name = rapid + "_resample_norotate_epbins1_";
		for(pair<string, pair<pair<int, unsigned>, pair<float, float>>> var:set_ranges) {
			float val = rand->Rndm() * (var.second.second.second - var.second.second.first) + var.second.second.first;
			string num = to_string(int(val * var.second.first.first + 0.5));
			while(num.size() < var.second.first.second) { num = "0" + num; }
			name += var.first + num + "_";
		}
		set_pairs[name + name_post] = set_nums;
	}

	return set_pairs;
}


map<string, map<string, map<string, pair<int, int>>>> get_sets_from_dir(string path, int set_size) {
	vector<string> set_names = get_files_in_dir(path, "", "name", true);
	map<string, map<string, map<string, pair<int, int>>>> sets = {{"BES1_def_sys_0", {{"default_sys", get_rand_set_pairs(15, "rapid05")}}}};
	int i = 0;
	int j = 0;
	for (string set_name : set_names) {
		if (in_string(set_name, "_sys_")) {
//			rfind(set_name, '_')
			// Need to remove trailing number!!!
			sets["BES1_def_sys_" + to_string(i)]["default_sys"][set_name] = make_pair(0, 0);
			if (++j >= set_size) {
				i++;
				j = 0;
			}
		}
	}

	return sets;
}

