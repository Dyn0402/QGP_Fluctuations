/*
 * AmptConvert.h
 *
 *  Created on: Feb 15, 2020
 *      Author: Dylan Neff
 */

#ifndef AMPTCONVERTER_H_
#define AMPTCONVERTER_H_

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

	unsigned run = 11116006;  // Total guess
//	unsigned refn;  Could maybe calculate from other particles but for now set equal to ref.
//	unsigned btof;  Make equal to ref to pass pile-up cut.
};

struct track_params {
	float dca = 0.1;
	double nsigma = 3.0;
	float beta = -999;
	short charge = 1;
};

class AmptConverter {
public:
	// Structors
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

private:
	// Attributes
	string in_path;
	string out_path;

	string tree_name = "tr";

	int threads = 1;

	map<int, string> pid_codes = {{2212, "proton"}, {2112, "neutron"}, {22, "photon"}};

	TreeReader reader_pars;

	// Doers

};



class TreeConverter : private AmptConverter {
public:
	// Structors
	TreeConverter(string in_file_path, string out_file_path);
	~TreeConverter();

private:
	// Attributes
	string in_file_path;
	string out_file_path;

	TFile *in_file, *out_file;
	TTree *in_tree, *out_tree;
	TH1I *event_hist, *track_hist;

	track_params track_default;
	event_params event_default;

	// Doers
	void convert_tree();
	void generate_cut_hists();
	nsm_ampt_leaves get_nsm_ampt_leaves();
	void get_proton_info(nsm_ampt_leaves leaves, Event &event);
	void get_other_info(nsm_ampt_leaves leaves, Event &event);
};


#endif /* AMPTCONVERTER_H_ */
