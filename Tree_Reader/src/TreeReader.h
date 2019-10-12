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

#include <TTree.h>
#include <TLeaf.h>
#include <TH2.h>

#include "../StRoot/StRefMultCorr/CentralityMaker.h"
#include "../StRoot/StRefMultCorr/StRefMultCorr.h"

#include "ratio_methods.h"

using namespace std;

struct event_leaves {
	TLeaf *run;
	TLeaf *ref_mult;
	TLeaf *ref_mult2;
	TLeaf *btof_mult;
	TLeaf *vz;
	TLeaf *event_plane;
};

struct proton_leaves {
	TLeaf *phi;
	TLeaf *pt;
	TLeaf *p;
	TLeaf *beta;
	TLeaf *dedx;
	TLeaf *charge;
	TLeaf *dca;
	TLeaf *nsigma;
	TLeaf *eta;
};

struct cut_values {
	//Event cuts
	vector<int> bad_runs = {11116006, 11116017, 11116055, 11117094, 11120084, 11122060, 11122080, 11123050, 11123056, 11125099, 11125102, 11126022, 11126068, 11127007, 11127078, 11129047, 11130043, 11130064, 11131027, 11132025, 11132058, 11132067, 11133085, 11134019, 11134053, 11134066, 11135025, 11136059, 11136077, 11137111, 11137114, 11138005, 11138047, 11138061, 11138066, 11138082, 11141019, 11142008, 11142018, 11142036, 11143027, 11143047, 11145101, 11148001, 11148006, 11148014, 11148045, 11148055, 11149001, 11149010, 11149015, 11149017, 11149047, 11150029, 11151050, 11151051, 11152036, 11153032, 11153042, 11154026, 11156003, 11156009, 11156036, 11156043, 11156044, 11156045, 12114007, 12114092, 12114103, 12115014, 12115015, 12115016, 12115018, 12115019, 12115020, 12115022, 12115023, 12115073, 12120026, 12121017, 12121022, 12121034, 12122019, 12172049, 12172050, 12172051, 12173031, 12173034, 12174077, 12174085, 12175030, 12175040, 12175062, 12176044, 12176047, 12176067, 12176069, 12176104, 12177015, 12177101, 12178051, 12178120, 12179068, 11002145, 11003001, 11003002, 11003004, 11003005, 11003006, 11003012, 11003013, 11023046, 11023048, 11025034, 11025054, 11026010, 11026114, 11028004, 11028005, 11028006, 11028018, 11028019, 11028020, 11028021, 11028022, 11030041, 11030042, 11057012, 11057035, 11059060, 11060059, 11064003, 11070051, 11070052, 11070053, 11070054, 11070055, 11070056, 11070057, 11071001, 11071002, 11071003, 11071004, 11071006, 11071007, 11071013, 11071014, 11071015, 11071016, 11071017, 11071018, 11071019, 11071020, 11072030, 11072031, 11072032, 11072044, 11072045, 11073001, 11073003, 11073030, 11073031, 11080072, 11084009, 11084010, 11087001, 11087002, 11087003, 11087004, 11087057, 11087058, 11087059, 11089026, 11089028, 11089029, 11089048, 11089055, 11092012, 11092031, 11092032, 11092033, 11092034, 11095040, 11095048, 11095049, 11095050, 11095051, 11095061, 11095062, 11095063, 11095064, 15047023, 15047065, 15047071, 15047087, 15049027, 15051131, 15053034, 15053035, 15053052, 15053053, 15053054, 15053055, 15053056, 15053057, 15061001, 15066064, 15067026, 11099124, 11100045, 11101030, 11101046, 11101104, 11102054, 11102098, 11103008, 11103009, 11103058, 11103062, 11105011, 11105018, 11105029, 11105045, 11106026, 11106027, 11106028, 11106029, 11106030, 11106039, 11106040, 11106041, 11106049, 11109013, 11109088, 11109090, 11109092, 11109097, 11109098, 11109102, 11109103, 11109104, 11110034, 11110041, 11110073, 11110076, 0, 0};
	int min_multi = 2; //Need this many protons or more to be a good event.
	map<int, double> max_slope = {{7, (double)(1565-187)/(310+6)}, {11, (double)(1274-172)/(250+6)}, {14, (double)(1954-240)/(350+12)}, {19, (double)(1596-195)/(364+11)}, {27, (double)(1878-207)/(400+18)}, {39, (double)(2220-128)/(415+20)}, {62, (double)(3700-116)/(544+22)}, {200, (double)(4376-142)/(543+10)}};
	map<int, double> min_slope = {{7, (double)(924+34)/(418-7)}, {11, (double)(660+20)/(307-7)}, {14, (double)(1020+61)/(396-10)}, {19, (double)(1049+35)/(426-5)}, {27, (double)(1211+39)/(460-6)}, {39, (double)(1563+103)/(524-10)}, {62, (double)(2657+244)/(649-51)}, {200, (double)(3130+77)/(616-70)}};

	//Proton cuts
	double min_p = 0.15;
	double max_p = 0.8;
	double min_pt = 0.4;
	double max_pt = 2.0;
	double min_beta = 0.0;
	double max_beta = 1.0;
	int charge = 1;
	double min_eta = -0.5;
	double max_eta = 0.5;
	double min_nsigma = -2.0;
	double max_nsigma = 2.0;
	double min_dca = 0.0;
	double max_dca = 2.0;
	double min_m2 = 0.8;
	double max_m2 = 1.0;
	double min_pt_for_m = 0.8;
	double max_pt_for_m = 2.0;
};


class TreeReader {
public:
	// Structors
	TreeReader(int energy);

	// Getters
	string get_in_path();
	string get_out_path();
	string get_qa_path();
	bool get_cbwc();
	bool get_rotate_random();

	// Setters
	void set_in_path(string path);
	void set_out_path(string path);
	void set_qa_path(string path);
	void set_qa_name(string name);
	void set_energy(int energy);
	void set_divs(vector<int> list);
	void set_cbwc(bool cbwc);
	void set_rotate_random(bool rotate_random);

	// Doers
	void read_trees();

	// Attributes
	cut_values cut;
	clock_t start = clock();
	chrono::system_clock::time_point start_sys;

private:
	// Attributes
	string tree_name = "nsmTree";
	string qa_name = "QA_CBWC_";

	string in_path = "/home/dylan/Research/Trees/";
	string out_path = "/home/dylan/local_server/dyn0402/Research/Data3/";
	string qa_path = "/home/dylan/local_server/dyn0402/Research/QA/";

	vector<int> divs = {2,3,4,5,6};
	int energy;

	bool cbwc;
	bool rotate_random;

	// Doers
	void read_tree(TTree* tree, tree_data *data, StRefMultCorr *refmult2CorrUtil);
	void read_tree_cbwc(TTree* tree, tree_data *data, StRefMultCorr *refmult2CorrUtil);

	event_leaves get_event_leaves(TTree* tree);
	proton_leaves get_proton_leaves(TTree* tree);

	bool check_event_good(event_leaves event, proton_leaves, int energy);
	bool check_enough_protons(proton_leaves protons);
	bool check_good_run(int run);

	bool check_proton_good(proton_leaves protons, int proton_index);

	int get_centrality(int refmult2, int energy);
	int get_centrality9(int refmult2, int energy);
	bool check_slope(double btof_mult, double ref_mult, int energy);

	void write_qa();

	// QA Plots
//	TH1I *event_cut_hist = new TH1I(("event_cut"+to_string(energy)).data(), "Event Cuts", 10, -0.5, 9.5);
//	TH1I *track_cut_hist = new TH1I(("track_cut"+to_string(energy)).data(), "Track Cuts", 10, -0.5, 9.5);
//	TH2I *cent_hist = new TH2I(("cent_comp"+to_string(energy)).data(), "Centrality Comparison", 19, -2.5, 16.5, 19, -2.5, 16.5);
	TH1I event_cut_hist;
	TH1I track_cut_hist;
	TH2I cent_hist;
};



#endif /* TREEREADER_H_ */
