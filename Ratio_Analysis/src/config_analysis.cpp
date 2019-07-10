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
	vector<int> energy_list = {7,11,19,27,39,62};
	vector<int> divs = {2,3,4,5,6};
	vector<int> centrals = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	vector<int> cumulant_orders = {1,2,3,4};

	string in_file_prefx = "auau";
	string in_file_sufx = ".root";

//	string in_path = "/home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/";
	string in_path = "/home/dylan/local_server/dyn0402/Research/Test_Data2/";
}


namespace plot {
	string out_path = "/home/dylan/local_server/dyn0402/Research/Results/";
//	string out_path = "/star/u/dneff/Documents/Roli_Thesis/";
	string out_root_name = "07-10_test.root";

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

//	vector<int> div_marker_styles = {8, 8, 28, 24, 25, 26, 27};
	vector<int> div_marker_styles = {8, 8, 34, 20, 21, 22, 33};
	vector<int> div_marker_width = {2, 2, 2, 2, 2, 2, 2};
	vector<int> div_marker_colors = {5, 5, 6, 1, 2, 8, 4};
	vector<int> div_marker_sizes = {1, 1, 1, 1, 1, 1, 1};
}
