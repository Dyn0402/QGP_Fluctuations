/*
 * DcaxyQABase.cpp
 *
 *  Created on: Sep 18, 2020
 *      Author: Dylan Neff
 */


#include "DcaxyQABase.h"


// Structors

DcaxyQABase::DcaxyQABase() {
	this->energy = 0;
}

DcaxyQABase::DcaxyQABase(int energy) {
	this->energy = energy;
}


DcaxyQABase::~DcaxyQABase() {
	// Nothing
}


// Getters

vector<int> DcaxyQABase::get_bad_runs() {
	if(bad_runs.size() == 0) {
		read_bad_dca_file();
	}

	return bad_runs;
}


map<int, vector<pair<int, int>>> DcaxyQABase::get_bad_ranges() {
	if(bad_ranges.size() == 0) {
		read_bad_dca_file();
	}

	return bad_ranges;
}


// Setters

void DcaxyQABase::set_energy(int energy) {
	this->energy = energy;
}

void DcaxyQABase::set_qa_path(string path) {
	qa_path = path;
}


// Doers

void DcaxyQABase::write_bad_dca_file() {
	if(!check_path(qa_path)) { cout << "Can't access path: " << qa_path << " Skipping following read/write." << endl; return; }

	string out_name = qa_path + to_string(energy) + "GeV" + qa_file_suf;
	ofstream out_txt(out_name);
	if(!out_txt.is_open()) { cout << "Could not open " << out_name << " Not writing." << endl; return; }

	out_txt << "bad_run_sigmas: " << pars.bad_run_sigmas[energy] << endl;
	out_txt << "mv_avg_pars:" << flush;
	for(auto n:pars.mv_avg_pars) {
		out_txt << "\tmoving average points = " << n.first << " bad threshold (sigma) = " << n.second.first << " relax threshold (sigma) = " << n.second.second << flush;
	}
	out_txt << "n_sigma_fit: " << pars.n_sigma_fit << endl;
	out_txt << "cent_min: " << pars.cent_min << endl;
	out_txt << "range_comb_tol: " << pars.range_comb_tol << endl;
	out_txt << "min_points_fit: " << pars.min_points_fit << endl;
	out_txt << "min_fit_entries: " << pars.min_fit_entries << endl;
	out_txt << "min_fit_count: " << pars.min_fit_count << endl;
	out_txt << endl;

	out_txt << energy << "GeV Bad DcaXY Runs (run average dca_xy greater than " << pars.bad_run_sigmas[energy] << " sigma from set average): " << endl;
	if(bad_runs.size() > 0) {
		bool first = true;
		for(int run:bad_runs) {
			if(!first) { out_txt << " " << flush; }
			out_txt << run;
			first = false;
		}
	}
	out_txt << endl << endl;

	out_txt << "Bad Ranges:";
	for(pair<int, vector<pair<int, int>>> run:bad_ranges) {
		if(run.second.size() == 0) { continue; }
		out_txt << endl << run.first << "\t" << flush;
		bool first = true;
		for(pair<int, int> range:run.second) {
			if(!first) { out_txt << "\t" << flush; }
			out_txt << "[ " << range.first << " , " << range.second << " ]" << flush;
			first = false;
		}
	}

	out_txt.close();
}


void DcaxyQABase::read_bad_dca_file() {
	if(!check_path(qa_path)) { cout << "Can't access path: " << qa_path << " Skipping following read/write." << endl; return; }

	string in_name = qa_path + to_string(energy) + "GeV" + qa_file_suf;
	ifstream in_txt(in_name);
	if(!in_txt.is_open()) { cout << "Could not open " << in_name << " Not reading." << endl; return; }

	map<int, vector<pair<int, int>>> bad_ranges_read;
	vector<int> bad_runs_read;
	string line;
	int line_num = 1;
	while(getline(in_txt, line)) {
		if(line_num == 11) {  // Bad runs line
			vector<string> bad_run_strs = split(line, ' ');
			for(string &run:bad_run_strs) {
				bad_runs_read.push_back(stoi(run));
			}
		}

		else if(line_num >= 14) {  // Bad event ranges
			vector<string> bad_run_range_str = split(line, '\t');
			if(bad_run_range_str.size() >= 2) {
				int run = stoi(bad_run_range_str[0]);
				for(int range_index = 1; range_index < (int)bad_run_range_str.size(); range_index++) {
					vector<string> bad_range_str = split(bad_run_range_str[range_index], ' ');
					if(bad_range_str.size() == 5) {
						bad_ranges_read[run].push_back(make_pair(stoi(bad_range_str[1]), stoi(bad_range_str[3])));
					} else { cout << "Bad range run read: " << bad_run_range_str[range_index] << endl; }
				}
			} else { cout << "Bad range line read" << endl; }
		}

		line_num++;
	}

	in_txt.close();

	if(bad_runs_read.size() > 0) { bad_runs = bad_runs_read; }
	if(bad_ranges_read.size() > 0) { bad_ranges = bad_ranges_read; }
}
