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
#include <TH1.h>
#include <TROOT.h>

#include "TreeReader.h"
#include "file_io.h"
#include "ThreadPool.h"
#include "Event.h"

using namespace std;


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

	TreeReader reader_pars;

	// Doers

};



class TreeConverter : public AmptConverter {
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

	// Doers
	void convert_tree();
	void generate_cut_hists();
};


#endif /* AMPTCONVERTER_H_ */
