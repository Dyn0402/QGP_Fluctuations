/*
 * PileUpQAer.h
 *
 *  Created on: Aug 7, 2020
 *      Author: Dylan Neff
 */

#ifndef PILEUPQAER_H_
#define PILEUPQAER_H_


#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <mutex>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "file_io.h"
#include "TreeBranches.h"

using namespace std;


pair<double, double> rotate_xy(double x, double y, double angle);

double gaus_exp_r(double *x, double *par);  // par = {amplitude, x_bar, sigma, k}
double woods_saxon(double *x, double *par);  // par = {amplitude, x_bar, width}
double exp_woods_saxon(double *x, double *par);  // par = {amplitude, x_bar, width, decay}
double lin_woods_saxon(double *x, double *par);  // par = {amplitude, x_bar, width, slope}
double gexp_r_plus_ws(double *x, double *par);  // par = {amplitude, x_bar, sigma, k, ws_amp, ws_xbar, ws_width}
double gexp_r_plus_expws(double *x, double *par);  // par = {amplitude, x_bar, sigma, k, exp*ws_amp, ws_exp_xbar, ws_width, exp_decay}
double gexp_r_plus_linws(double *x, double *par);  // par = {amplitude, x_bar, sigma, k, exp*ws_amp, ws_exp_xbar, ws_width, slope}


struct pu_cut {
	vector<float> pol_fit_coefs;  // Coefficients of polynomial fit
	float max_fit_ref;  // Maximum refmult value of fitted points
	pair<float, float> lin_extrap;  // Coefficients of linear extrapolation past the max_fit_ref
};


class PileUpQAer {
public:
	// Structors
	PileUpQAer(int energy);
	PileUpQAer(int energy, mutex *mtx);
	~PileUpQAer();

	// Getters
	pu_cut get_low_cut();
	pu_cut get_high_cut();

	// Setters
	void set_energy(int energy);
	void set_in_path(string path);
	void set_out_path(string path);
	void set_out_txt_suf(string suf);
	void set_out_root_pre(string pre);
	void set_write_out_txt(bool write);
	void set_sigmas_left(float sigmas_left);
	void set_sigmas_right(float sigmas_right);

	// Doers
	void run_qa();

private:
	// Attributes
	mutex *mtx = NULL;  // To lock non-thread-safe graphics (TCanvas) processes

	bool write_out_txt = true;

	int energy;
	string in_path = "/media/ucla/Research/BES1_Trees/";
	string out_path = "/home/dylan/Research/Pile_Up_QA/";
	string out_txt_suf = "_Pile_Up_Cuts.txt";
	string out_root_pre = "Pile_QA_";
	TFile *out_file = NULL;
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;
	string tree_name = "tree";

	string orig_btof_ref_name = "btof_ref_original_";
	string orig_btof_ref_title = "BTof Match vs Reference Multiplicity ";

	int can_x_pix = 955;
	int can_y_pix = 900;

	float sigmas_left = 4.0;
	float sigmas_right = 4.0;
	int min_points = 350;
	int min_bins = 15;

	int min_btof = 6;

	float rot_slice_height = 5.0;
	int orig_slice_height = 3;

	int n_plot_points = 5000;

	int orig_btof_bins = 326;
	float orig_btof_low = -0.5;
	float orig_btof_high = orig_btof_low + orig_btof_bins;

	int orig_ref_bins = 601;
	float orig_ref_low = -0.5;
	float orig_ref_high = orig_ref_low + orig_ref_bins;

	// Data Containers
	struct PUEvent {
		short ref;
		short btof;
		vector<float> dca_z;
		float dca_z_avg;
		float dca_z_sd;
	};
	vector<PUEvent> pu_events;

	pu_cut low_cut;
	pu_cut high_cut;


	// Doers
	void read_trees();
	void rotate_dist();
	void get_cuts();

	void read_tree(TTree *tree);
	void set_branches(TTree *tree);

	void write_cut_file();
	void read_cut_file();
};



#endif /* PILEUPQAER_H_ */


