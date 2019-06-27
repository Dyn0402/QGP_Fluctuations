/*
 * config.h
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */

#ifndef CONFIG_TR_H_
#define CONFIG_TR_H_

#include <string>
#include <vector>
#include <map>

using namespace std;

namespace config {
	extern string out_path;
	extern string in_path;
	extern string out_root_name;

	extern string in_file_prefx;
	extern string in_file_sufx;

	extern string ratio_dist_dir_name;
	extern string nproton_dist_dir_name;
	extern string cumulant_dir_name;
	extern string canvas_dir_name;

	extern vector<int> energy_list;
	extern vector<int> divs;
	extern vector<int> centrals;
	extern vector<int> cumulant_orders;

	extern string tree_name;

	//File io
	extern string nproton_file_pre;
	extern vector<string> nproton_file_fields;
	extern char file_name_delimeter;
	extern vector<char> data_delimeters;
	extern string ratios_file_pre;
	extern vector<string> ratios_file_fields;
	extern string file_ext;

	extern int ratio_hist_bins;
	extern double ratio_hist_low;
	extern double ratio_hist_high;

	extern int protons_hist_bins;
	extern double protons_hist_low;
	extern double protons_hist_high;
}


namespace cuts {
	//Event cuts
	extern vector<int> bad_runs;
	extern map<int, int> ref2_min; //Maps energy (key) to ref2_min (value)
	extern int proton_min_multi; //Need this many protons or more to be a good event.

	//Proton cuts
	extern double proton_min_p;
	extern double proton_max_p;
	extern double proton_min_pt;
	extern double proton_max_pt;
	extern double proton_min_beta;
	extern double proton_max_beta;
	extern int proton_charge;
	extern double proton_min_eta;
	extern double proton_max_eta;
	extern double proton_min_nsigma;
	extern double proton_max_nsigma;
	extern double proton_min_dca;
	extern double proton_max_dca;
	extern double proton_min_m2;
	extern double proton_max_m2;
}

#endif /* CONFIG_TR_H_ */
