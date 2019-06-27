/*
 * config_analysis.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: Dylan Neff
 */



#include <string>
#include <vector>
#include <map>

using namespace std;


namespace analysis {
}


namespace plot {
	string out_path = "/home/dylan/local_server/dyn0402/Research/Test_Data/";
//	string out_path = "/star/u/dneff/Documents/Roli_Thesis/";
	string in_path = "/home/dylan/local_server/dyn0402/Research/Test_Data/";
//	string in_path = "/gpfs01/star/pwg/dneff/BES1_Trees/dylan_trees/";
//	string in_path = "/home/dylan/rcf_all/direct/gpfs01/star/pwg/dneff/BES1_Trees/dylan_trees/";
	string out_root_name = "test.root";

	string ratio_dist_dir_name = "Ratio_Dists";
	string nproton_dist_dir_name = "NProton_Dists";
	string cumulant_dir_name = "Cumulants";
	string canvas_dir_name = "Canvases";

	int ratio_hist_bins = 23;
	double ratio_hist_low = -0.05;
	double ratio_hist_high = 1.1;

	int protons_hist_bins = 51;
	double protons_hist_low = -1;
	double protons_hist_high = 50;
}
