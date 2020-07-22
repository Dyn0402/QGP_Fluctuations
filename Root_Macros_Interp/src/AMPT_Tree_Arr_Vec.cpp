/*
 * AMPT_Tree_Arr_Vec.cpp
 *
 *  Created on: Jul 22, 2020
 *      Author: Dylan Neff
 */


#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/stat.h>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TBenchmark.h"

using namespace std;


struct event_vars {
	int npp, npt, nesp, ninesp, nest, ninest, pmult;
	int event, refmult, refmult2, refmult3;
	float imp, qx, qy;
};

struct track_vars {
	int buffer_size = 5000000;
	int split_level = 1;

	vector<int> pid_vec;
	vector<float> px_vec;  // Apparently need to be declared on separate lines
	vector<float> py_vec;
	vector<float> pz_vec;

	static const int track_mult = 100000;
	int pid_arr[track_mult];
	float px_arr[track_mult];
	float py_arr[track_mult];
	float pz_arr[track_mult];
};

vector<string> get_files_in_dir(string dir_path, string ext, string out);
vector<string> split_string_by_char(string str, char del);
void copy_tree(TTree *in_tree, TTree *out_tree);
event_vars set_event_branches(TTree *in_tree, TTree *out_tree);
track_vars set_track_branches(TTree *in_tree, TTree *out_tree);




void AMPT_Tree_Arr_Vec() {
	gBenchmark->Start("AMPT_Tree_Arr_Vec");

	string path = "/gpfs01/star/pwg/dneff/data/AMPT/ampt_brian_min_bias/output/27GeV/";
	string tree_name = "tree";
	string tree_title = "AMPT Data";
	string new_file_ext = "_convert";

	vector<string> files = get_files_in_dir(path, "root", "path");

	for(string file:files) {
		TFile *in_file = new TFile(file.data(), "READ");
		if(!in_file || in_file->IsZombie()) { cout << "Can't open " << file << endl; return; }
		TTree *in_tree = (TTree*)in_file->Get(tree_name.data());

		vector<string> name_split = split_string_by_char(file, '.');
		string new_name = name_split[0] + new_file_ext + "." + name_split[1];
		TFile *out_file = new TFile(new_name.data(), "RECREATE");
		if(!out_file || out_file->IsZombie()) { cout << "Can't open " << new_name.data() << endl; return; }
		TTree *out_tree = new TTree(tree_name.data(), tree_title.data());

		cout << "Copying input file:" << endl << file << endl << "To destination file:" << endl << new_name << endl << endl;

		copy_tree(in_tree, out_tree);

		out_file->Write();

		out_file->Close();
		in_file->Close();

		delete out_file; delete in_file;
	}

	cout << "donzo" << endl;

	gBenchmark->Show("AMPT_Tree_Arr_Vec");
}


void copy_tree(TTree *in_tree, TTree *out_tree) {
	// Set branches for in_tree and out_tree
	event_vars evars = set_event_branches(in_tree, out_tree);

	track_vars tvars = set_track_branches(in_tree, out_tree);


	// Loop over events and copy to new tree
	int event_index = 0;
	string input;
	while(in_tree->GetEntry(event_index++)) {
		tvars.pid_vec.clear(); tvars.px_vec.clear(); tvars.py_vec.clear(); tvars.pz_vec.clear();
		for(int track_index = 0; track_index < evars.pmult; track_index++) {
			tvars.pid_vec.push_back(tvars.pid_arr[track_index]);
			tvars.px_vec.push_back(tvars.px_arr[track_index]);
			tvars.py_vec.push_back(tvars.py_arr[track_index]);
			tvars.pz_vec.push_back(tvars.pz_arr[track_index]);
		}
		out_tree->Fill();
	}
}


event_vars set_event_branches(TTree *in_tree, TTree *out_tree) {
	event_vars evars;

	// In Tree Event Branches
	in_tree->SetBranchAddress("event", &evars.event);
	in_tree->SetBranchAddress("refmult", &evars.refmult);
	in_tree->SetBranchAddress("refmult2", &evars.refmult2);
	in_tree->SetBranchAddress("refmult3", &evars.refmult3);
	in_tree->SetBranchAddress("qx", &evars.qx);
	in_tree->SetBranchAddress("qy", &evars.qy);
	in_tree->SetBranchAddress("imp", &evars.imp);
	in_tree->SetBranchAddress("pmult", &evars.pmult);

	in_tree->SetBranchAddress("npp", &evars.npp);
	in_tree->SetBranchAddress("npt", &evars.npt);
	in_tree->SetBranchAddress("nesp", &evars.nesp);
	in_tree->SetBranchAddress("ninesp", &evars.ninesp);
	in_tree->SetBranchAddress("nest", &evars.nest);
	in_tree->SetBranchAddress("ninest", &evars.ninest);


	// Out Tree Event Branches
	out_tree->Branch("event",    &evars.event,     "event/I");
	out_tree->Branch("refmult",  &evars.refmult,   "refmult/I");
	out_tree->Branch("refmult2", &evars.refmult2,  "refmult2/I");
	out_tree->Branch("refmult3", &evars.refmult3,  "refmult3/I");
	out_tree->Branch("qx",       &evars.qx,        "qx/F");
	out_tree->Branch("qy",       &evars.qy,        "qy/F");
	out_tree->Branch("imp",      &evars.imp,       "imp/F");

	out_tree->Branch("npp",      &evars.npp,       "npp/I");
	out_tree->Branch("npt",      &evars.npt,       "npt/I");
	out_tree->Branch("nesp",     &evars.nesp,      "nesp/I");
	out_tree->Branch("ninesp",   &evars.ninesp,    "ninesp/I");
	out_tree->Branch("nest",     &evars.nest,      "nest/I");
	out_tree->Branch("ninest",   &evars.ninest,    "ninest/I");


	return evars;
}


track_vars set_track_branches(TTree *in_tree, TTree *out_tree) {
	track_vars tvars;

	// In Tree Track Branches
	in_tree->SetBranchAddress("pid", tvars.pid_arr);
	in_tree->SetBranchAddress("px", tvars.px_arr);
	in_tree->SetBranchAddress("py", tvars.py_arr);
	in_tree->SetBranchAddress("pz", tvars.pz_arr);

	// Out Tree Track Branches
	out_tree->Branch("pid",      &tvars.pid_vec, tvars.buffer_size, tvars.split_level);
	out_tree->Branch("px",       &tvars.px_vec,  tvars.buffer_size, tvars.split_level);
	out_tree->Branch("py",       &tvars.py_vec,  tvars.buffer_size, tvars.split_level);
	out_tree->Branch("pz",       &tvars.pz_vec,  tvars.buffer_size, tvars.split_level);


	return tvars;
}


//Return name (not path) of all files in dir_path with extension ext.
vector<string> get_files_in_dir(string dir_path, string ext, string out) {
	vector<string> files;
	struct stat info;

	if( stat(dir_path.data(), &info) !=0 ) {
		cout << "Can't access path: " << dir_path << " Skipping following read/write." << endl;
	} else {
		DIR* files_dir = opendir(dir_path.data());
		struct dirent* dp;
		while((dp=readdir(files_dir)) != NULL) {
			string file = dp->d_name;
			vector<string> fields = split_string_by_char(file, '.');
			if(fields.back() == ext) {
				files.push_back(file);
			}
		}
		if(out == "path") {
			for(unsigned i=0; i<files.size(); i++) {
				files[i] = dir_path + files[i];
			}
		}
	}

	return(files);
}


vector<string> split_string_by_char(string str, char del) {
	vector<string> split;
	stringstream stream(str);
	string hold;
	while(getline(stream, hold, del)) {
		split.push_back(hold);
	}

	return(split);
}

