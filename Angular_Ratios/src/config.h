/*
 * config.h
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

namespace config {
	extern string out_path;
	extern string in_path;
	extern string out_root_name;

	extern string in_file_prefx;
	extern string in_file_sufx;

	extern vector<int> energy_list;
	extern vector<int> divs;

	extern string tree_name;

	//Event cuts
	extern vector<int> bad_runs;
	extern unordered_map<int, int> ref2_min;
	extern int proton_min_multi;

	//Proton cuts
	extern double proton_max_pt;
	extern double proton_min_beta;
	extern double proton_min_m;
	extern double proton_max_m;

	extern int ratio_hist_bins;
	extern double ratio_hist_low;
	extern double ratio_hist_high;

	extern int protons_hist_bins;
	extern double protons_hist_low;
	extern double protons_hist_high;
}


#endif /* CONFIG_H_ */
