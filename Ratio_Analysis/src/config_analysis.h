/*
 * config_analysis.h
 *
 *  Created on: Jun 27, 2019
 *      Author: Dylan Neff
 */

#ifndef CONFIG_ANALYSIS_H_
#define CONFIG_ANALYSIS_H_


#include <string>
#include <vector>
#include <map>

using namespace std;


namespace analysis {
	extern vector<int> energy_list;
	extern vector<int> divs;
	extern vector<int> centrals;
	extern vector<int> cumulant_orders;

	extern string in_file_prefx;
	extern string in_file_sufx;

	extern string in_path;
}


namespace plot {
	extern string out_path;
	extern string out_root_name;

	extern string ratio_dist_dir_name;
	extern string nproton_dist_dir_name;
	extern string cumulant_dir_name;
	extern string canvas_dir_name;

	extern int ratio_hist_bins;
	extern double ratio_hist_low;
	extern double ratio_hist_high;

	extern int protons_hist_bins;
	extern double protons_hist_low;
	extern double protons_hist_high;
}



#endif /* CONFIG_ANALYSIS_H_ */
