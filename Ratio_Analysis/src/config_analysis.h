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
	extern vector<string> stat_names;

	extern string in_file_prefx;
	extern string in_file_sufx;

	extern string in_path;
	extern string in_mix_path;
}


namespace plot {
	extern string out_path;
	extern string out_root_name;

	extern string ratio_dist_dir_name;
	extern string dist_2d_dir_name;
	extern string nproton_dist_dir_name;
	extern string cumulant_dir_name;
	extern string stat_dir_name;
	extern string canvas_dir_name;

	extern int canvas_width;
	extern int canvas_height;

	extern int ratio_hist_bins;
	extern double ratio_hist_low;
	extern double ratio_hist_high;

	extern int protons_hist_bins;
	extern double protons_hist_low;
	extern double protons_hist_high;

	extern vector<int> div_marker_styles;
	extern vector<int> div_marker_width;
	extern vector<int> div_marker_colors;
	extern vector<double> div_marker_sizes;

	extern map<int, int> energy_marker_styles;
	extern map<int, int> energy_marker_width;
	extern map<int, int> energy_marker_colors;
	extern map<int, int> energy_marker_sizes;

	extern map<int, int> cent_marker_styles;
	extern map<int, int> cent_marker_width;
	extern map<int, int> cent_marker_colors;
	extern map<int, int> cent_marker_sizes;

	extern map<int, double> energy_match;
	extern double centrality_slope;
	extern double centrality_intercept;
}



#endif /* CONFIG_ANALYSIS_H_ */
