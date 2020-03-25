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
	vector<int> centrals = {0,1,2,3,4,5,6,7,8};
	vector<int> cumulant_orders = {1,2,3,4};
	vector<string> stat_names = {"mean", "standard_deviation", "skewness", "kurtosis"};

	string in_file_prefx = "auau";
	string in_file_sufx = ".root";

//	string in_path = "/home/dylan/git/Research/QGP_Fluctuations/Tree_Reader/";
	string in_path = "/home/dylan/Research/Data/"; // "/home/dylan/local_server/dyn0402/Research/Data/"; //
	string in_mix_path = "/home/dylan/Research/Data_Mix/"; // "/home/dylan/local_server/dyn0402/Research/Data_Mix/"; //
}


namespace plot {
	string out_path = "/home/dylan/Research/Results/"; // "/home/dylan/local_server/dyn0402/Research/Results/"; //
//	string out_path = "/star/u/dneff/Documents/Roli_Thesis/";
	string out_root_name = "12-01-19_ep_rotate.root";
	string ratio_dist_dir_name = "Ratio_Dists";
	string dist_2d_dir_name = "2D_Dists";
	string nproton_dist_dir_name = "NProton_Dists";
	string cumulant_dir_name = "Cumulants";
	string stat_dir_name = "Stats";
	string canvas_dir_name = "Canvases";

	int canvas_width = 2070;
	int canvas_height = 1080;

	double can_div_x = 0.001;
	double can_div_y = 0.001;

	double legend_border_width = 0;

	int ratio_hist_bins = 23;
	double ratio_hist_low = -0.05;
	double ratio_hist_high = 1.1;

	int protons_hist_bins = 61;
	double protons_hist_low = -0.5;
	double protons_hist_high = 60.5;

//	vector<int> div_marker_styles = {8, 8, 28, 24, 25, 26, 27};
	vector<int> div_marker_styles = {8, 8, 34, 20, 21, 22, 33};
	vector<int> div_marker_width = {2, 2, 2, 2, 2, 2, 2};
	vector<int> div_marker_colors = {28, 9, 6, 1, 2, 8, 4};
	vector<double> div_marker_sizes = {1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2};

	map<int, int> energy_marker_styles = {{7,24}, {11,25}, {19,26}, {27,27}, {39,28}, {62,30}};
	map<int, int> energy_marker_width = {{7,2}, {11,2}, {19,2}, {27,2}, {39,2}, {62,2}};
	map<int, int> energy_marker_colors = {{7,1}, {11,2}, {19,8}, {27,4}, {39,6}, {62,9}};
	map<int, int> energy_marker_sizes = {{7,1}, {11,1}, {19,1}, {27,1}, {39,1}, {62,1}};;

//	map<int, int> cent_marker_styles = {{15,24}, {14,25}, {13,26}, {12,27}, {11,28}, {10,30}};
//	map<int, int> cent_marker_width = {{15,2}, {14,2}, {13,2}, {12,2}, {11,2}, {10,2}};
//	map<int, int> cent_marker_colors = {{15,1}, {14,2}, {13,8}, {12,4}, {11,6}, {10,9}};
//	map<int, int> cent_marker_sizes = {{15,1}, {14,1}, {13,1}, {12,1}, {11,1}, {10,1}};

	map<int, int> cent_marker_styles = {{9,24}, {8,25}, {7,26}, {6,27}, {5,28}, {4,30}, {3,30}, {2,30}, {1,30}, {0,30}};
	map<int, int> cent_marker_width = {{9,2}, {8,2}, {7,2}, {6,2}, {5,2}, {4,2}, {3,2}, {2,2}, {1,2}, {0,2}};
	map<int, int> cent_marker_colors = {{9,1}, {8,2}, {7,11}, {6,4}, {5,46}, {4,6}, {3,7}, {2,8}, {1,9}, {0,28}};
	map<int, int> cent_marker_sizes = {{9,1}, {8,1}, {7,1}, {6,1}, {5,1}, {4,1}, {3,1}, {2,1}, {1,1}, {0,1}};

	map<int, double> energy_match = {{7,7.7}, {11,11.5}, {19,19.6}, {27,27.0}, {39,39.0}, {62,62.4}};
	double centrality_slope = 5.0;
	double centrality_intercept = 2.5;
}
