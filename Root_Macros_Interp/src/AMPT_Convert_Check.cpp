/*
 * AMPT_Convert_Check.cpp
 *
 *  Created on: Jul 23, 2020
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
	vector<int> *pid_vec = 0;
	vector<float> *px_vec = 0;  // Apparently need to be declared on separate lines
	vector<float> *py_vec = 0;
	vector<float> *pz_vec = 0;

	static const int track_mult = 100000;
	int pid_arr[track_mult];
	float px_arr[track_mult];
	float py_arr[track_mult];
	float pz_arr[track_mult];
};

vector<string> get_files_in_dir(string dir_path, string ext, string out="name");
vector<string> split_string_by_char(string str, char del);
bool comp_trees(TTree *in_tree, TTree *out_tree);
bool events_same(event_vars e_in, event_vars e_out);
bool tracks_same(track_vars tvars, int pmult);
event_vars set_event_branches(TTree *tree);
track_vars set_track_branches(TTree *in_tree, TTree *out_tree);




void AMPT_Convert_Check() {
	gBenchmark->Start("AMPT_Tree_Arr_Vec");

	string path = "/gpfs01/star/pwg/dneff/data/AMPT/Brian_Data/min_bias/27GeV/";
	string convert_path = "/gpfs01/star/pwg/dneff/data/AMPT/Brian_Data/min_bias/27GeV_convert/";
//	string path = "/media/ucla/Research/AMPT_To_Convert/";
//	string convert_path = "/media/ucla/Research/AMPT_Convert/";
	string tree_name = "tree";
	string tree_title = "AMPT Data";
	string new_file_ext = "_convert";

	vector<string> files = get_files_in_dir(path, "root");

	for(string file:files) {
		TFile *in_file = new TFile((path+file).data(), "READ");
		if(!in_file || in_file->IsZombie()) { cout << "Can't open " << file << endl; return; }
		TTree *in_tree = (TTree*)in_file->Get(tree_name.data());

		vector<string> name_split = split_string_by_char(file, '.');
		string new_path = convert_path + name_split[0] + new_file_ext + "." + name_split[1];
		TFile *out_file = new TFile(new_path.data(), "READ");
		if(!out_file || out_file->IsZombie()) { cout << endl << "Can't open " << new_path.data() << endl; return; }
		TTree *out_tree = (TTree*)out_file->Get(tree_name.data());

		cout << name_split[0] << ", " << flush;

		if(!comp_trees(in_tree, out_tree)) {
			cout << endl << endl << "Input file:" << endl << file << endl << "Not equivalent to  converted file:" << endl << new_path << endl << endl;
		}

		out_file->Close();
		in_file->Close();

		delete out_file; delete in_file;
	}

	cout << "donzo" << endl;

	gBenchmark->Show("AMPT_Tree_Arr_Vec");
}


bool comp_trees(TTree *in_tree, TTree *out_tree) {
	// Set branches for in_tree and out_tree
	event_vars evars_out = set_event_branches(out_tree);
	event_vars evars_in = set_event_branches(in_tree);

	track_vars tvars = set_track_branches(in_tree, out_tree);

	int pmult;
	in_tree->SetBranchAddress("pmult", &pmult);


	// Loop over events and copy to new tree
	bool same = true;
	int event_index = -1;
	if(in_tree->GetEntries() != out_tree->GetEntries()) { cout << in_tree->GetEntries() << " in | entries | out " << out_tree->GetEntries();  return false; }
	while(in_tree->GetEntry(++event_index)) {
		out_tree->GetEntry(event_index);
		if(!events_same(evars_in, evars_out)) { cout << "Event diff" << endl; same = false; break; }
		same = tracks_same(tvars, pmult);
	}

	return same;
}


bool events_same(event_vars e_in, event_vars e_out) {
	if(e_in.event != e_out.event) { cout << e_in.event << " in | event | out " << e_out.event;  return false; }
	if(e_in.refmult != e_out.refmult) { cout << e_in.refmult << " in | refmult | out " << e_out.refmult;  return false; }
	if(e_in.refmult2 != e_out.refmult2) { cout << e_in.refmult2 << " in | refmult2 | out " << e_out.refmult2;  return false; }
	if(e_in.refmult3 != e_out.refmult3) { cout << e_in.refmult3 << " in | refmult3 | out " << e_out.refmult3;  return false; }
	if(e_in.imp != e_out.imp) { cout << e_in.imp << " in | imp | out " << e_out.imp;  return false; }
	if(e_in.qx != e_out.qx) { cout << e_in.qx << " in | qx | out " << e_out.qx;  return false; }
	if(e_in.qy != e_out.qy) { cout << e_in.qy << " in | qy | out " << e_out.qy;  return false; }
	if(e_in.npp != e_out.npp) { cout << e_in.npp << " in | npp | out " << e_out.npp;  return false; }
	if(e_in.npt != e_out.npt) { cout << e_in.npt << " in | npt | out " << e_out.npt;  return false; }
	if(e_in.nesp != e_out.nesp) { cout << e_in.nesp << " in | nesp | out " << e_out.nesp;  return false; }
	if(e_in.ninesp != e_out.ninesp) { cout << e_in.ninesp << " in | ninesp | out " << e_out.ninesp;  return false; }
	if(e_in.nest != e_out.nest) { cout << e_in.nest << " in | nest | out " << e_out.nest;  return false; }
	if(e_in.ninest != e_out.ninest) { cout << e_in.ninest << " in | ninest | out " << e_out.ninest;  return false; }

	return true;
}


bool tracks_same(track_vars tvars, int pmult) {
	if(pmult != (int)tvars.pid_vec->size()) return false;
	if(pmult != (int)tvars.px_vec->size()) return false;
	if(pmult != (int)tvars.py_vec->size()) return false;
	if(pmult != (int)tvars.pz_vec->size()) return false;

	for(int track_index = 0; track_index < pmult; track_index++) {
		if(tvars.pid_arr[track_index] != tvars.pid_vec->at(track_index)) return false;
		if(tvars.px_arr[track_index] != tvars.px_vec->at(track_index)) return false;
		if(tvars.py_arr[track_index] != tvars.py_vec->at(track_index)) return false;
		if(tvars.pz_arr[track_index] != tvars.pz_vec->at(track_index)) return false;
	}


	return true;
}

event_vars set_event_branches(TTree *tree) {
	event_vars evars;

	tree->SetBranchAddress("event", &evars.event);
	tree->SetBranchAddress("refmult", &evars.refmult);
	tree->SetBranchAddress("refmult2", &evars.refmult2);
	tree->SetBranchAddress("refmult3", &evars.refmult3);
	tree->SetBranchAddress("qx", &evars.qx);
	tree->SetBranchAddress("qy", &evars.qy);
	tree->SetBranchAddress("imp", &evars.imp);

	tree->SetBranchAddress("npp", &evars.npp);
	tree->SetBranchAddress("npt", &evars.npt);
	tree->SetBranchAddress("nesp", &evars.nesp);
	tree->SetBranchAddress("ninesp", &evars.ninesp);
	tree->SetBranchAddress("nest", &evars.nest);
	tree->SetBranchAddress("ninest", &evars.ninest);


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
	TBranch *pid_branch = 0;
	out_tree->SetBranchAddress("pid", &tvars.pid_vec, &pid_branch);
	TBranch *px_branch = 0;
	out_tree->SetBranchAddress("px", &tvars.px_vec, &px_branch);
	TBranch *py_branch = 0;
	out_tree->SetBranchAddress("py", &tvars.py_vec, &py_branch);
	TBranch *pz_branch = 0;
	out_tree->SetBranchAddress("pz", &tvars.pz_vec, &pz_branch);


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

