/*
 * DcaxyQABase.h
 *
 *  Created on: Sep 18, 2020
 *      Author: Dylan Neff
 */

#ifndef DCAXYQABASE_H_
#define DCAXYQABASE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "file_io.h"

using namespace std;

struct DcaxyQAPars {
	map<int, float> bad_run_sigmas = {{7, 3.0}, {11, 3.0}, {14, 3.0}, {19, 4.5}, {27, 3.0}, {39, 3.0}, {54, 3.0}, {62, 4.0}, {200, 3.0}};

	map<int, pair<float, float>> mv_avg_pars {{1, {5.0, 0.2}}, {5, {5.5, 2.0}}, {10, {8.0, 4.0}}};//, {20, {9.5, 1.0}}, {30, {12.0, 1.5}}};
	float n_sigma_fit = 2.0;
	int cent_min = 6;
	int range_comb_tol = 1000;
	int min_points_fit = 4;

	int min_fit_entries = 100;
	int min_fit_count = 3;
};

class DcaxyQABase {
public:
	// Structors
	DcaxyQABase();
	DcaxyQABase(int energy);
	~DcaxyQABase();

	// Getters
	vector<int> get_bad_runs();
	map<int, vector<pair<int, int>>> get_bad_ranges();

	// Setters
	void set_energy(int energy);
	void set_qa_path(string path);

	// Attributes
	DcaxyQAPars pars;

protected:
	// Attributes
	int energy;

	string qa_path = "/home/dylan/Research/Dca_xy_QA/";
	string qa_file_suf = "_Bad_DCA_Runs_Ranges.txt";

	// Data Containers
	vector<int> bad_runs;
	map<int, vector<pair<int, int>>> bad_ranges;

	// Doers
	void write_bad_dca_file();
	void read_bad_dca_file();
};


#endif /* DCAXYQABASE_H_ */
