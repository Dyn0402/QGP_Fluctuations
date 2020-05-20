/*
 * AmptConvert.h
 *
 *  Created on: Feb 15, 2020
 *      Author: Dylan Neff
 */

#ifndef AMPTCONVERTER_H_
#define AMPTCONVERTER_H_

#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TH1.h>
#include <TROOT.h>
#include <TMath.h>
#include <TVector3.h>

#include "TreeReader.h"
#include "file_io.h"
#include "ThreadPool.h"
#include "Event.h"
#include "Track.h"

using namespace std;


struct nsm_ampt_leaves {
	TLeaf *mult;
	TLeaf *pid;
	TLeaf *px;
	TLeaf *py;
	TLeaf *pz;
};

struct event_params {
	double vx = 0.;
	double vy = 0.;
	double vz = 0.;
	double event_plane = 0.;

	unsigned run = 11152039;  // Pulled from 11GeV, seems to work for 39GeV as well.
	unsigned btof_ref_multi = 4;  // Multiply ref by this number to get btof in order to pass pile-up cut. Fine tuned for 11GeV and 39GeV
//	unsigned refn;  Could maybe calculate from other particles but for now set equal to ref.
//	unsigned btof;  Make equal to ref to pass pile-up cut.
};

struct track_params {
	float dca = 0.1;
	double nsigma = 1.0;
	float beta = -999;
	short charge = 1;
};

struct track_cuts {
	double p_min = 0.15;
	double pt_min = 0.3;
	double pt_max = 2.5;
};

class AmptConverter {
public:
	// Structors
	AmptConverter(string in_path, string out_path);
	AmptConverter();
	~AmptConverter();

	// Getters
	string get_in_path();
	string get_out_path();

	// Setters
	void set_in_path(string path);
	void set_out_path(string path);

	// Doers
	void convert_trees();

protected:
	// Attributes
	string tree_name = "tr";
	map<int, string> pid_codes = {{2212, "proton"}, {2112, "neutron"}, {22, "photon"}};
	TreeReader reader_pars;

private:
	// Attributes
	string in_path;
	string out_path;

	int threads = 2;

	// Doers

};



class TreeConverter : public AmptConverter {
public:
	// Structors
	TreeConverter(string in_file_path, string out_file_path);
	~TreeConverter();

	// Doers
	void convert_tree();

private:
	// Attributes
	string in_file_path;
	string out_file_path;

	TFile *in_file, *out_file;
	TTree *in_tree, *out_tree;
	TH1I *event_hist, *track_hist;

	track_params track_default;
	event_params event_default;
	track_cuts cut;

	// Doers
	void generate_cut_hists();
	nsm_ampt_leaves get_nsm_ampt_leaves();
	void get_proton_info(nsm_ampt_leaves leaves, Event &event);
	void get_other_info(nsm_ampt_leaves leaves, Event &event);
};


#endif /* AMPTCONVERTER_H_ */
