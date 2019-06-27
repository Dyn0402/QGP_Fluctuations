/*
 * config.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */


#include <string>
#include <vector>
#include <map>

using namespace std;


namespace config {
	string out_path = "/home/dylan/local_server/dyn0402/Research/Test_Data/";
//	string out_path = "/star/u/dneff/Documents/Roli_Thesis/";
	string in_path = "/home/dylan/local_server/dyn0402/Research/Test_Data/";
//	string in_path = "/gpfs01/star/pwg/dneff/BES1_Trees/dylan_trees/";
//	string in_path = "/home/dylan/rcf_all/direct/gpfs01/star/pwg/dneff/BES1_Trees/dylan_trees/";
	string out_root_name = "test.root";

	string in_file_prefx = "auau";
	string in_file_sufx = ".root";

	string ratio_dist_dir_name = "Ratio_Dists";
	string nproton_dist_dir_name = "NProton_Dists";
	string cumulant_dir_name = "Cumulants";
	string canvas_dir_name = "Canvases";

	vector<int> energy_list = {11};
	vector<int> divs = {3};
	vector<int> centrals = {15};//{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	vector<int> cumulant_orders = {};

	string tree_name = "nsmTree";

	//File io
	string nproton_file_pre = "nprotons";
	vector<string> nproton_file_fields = {"centrality"};
	char file_name_delimeter = '_';
	vector<char> data_delimeters = {'\t', ' ', ':'};
	string ratios_file_pre = "ratios";
	vector<string> ratios_file_fields = {"divisions", "centrality"};
	string file_ext = ".txt";


	int ratio_hist_bins = 23;
	double ratio_hist_low = -0.05;
	double ratio_hist_high = 1.1;

	int protons_hist_bins = 51;
	double protons_hist_low = -1;
	double protons_hist_high = 50;
}


namespace cuts {
	//Event cuts
	vector<int> bad_runs = {11116006, 11116017, 11116055, 11117094, 11120084, 11122060, 11122080, 11123050, 11123056, 11125099, 11125102, 11126022, 11126068, 11127007, 11127078, 11129047, 11130043, 11130064, 11131027, 11132025, 11132058, 11132067, 11133085, 11134019, 11134053, 11134066, 11135025, 11136059, 11136077, 11137111, 11137114, 11138005, 11138047, 11138061, 11138066, 11138082, 11141019, 11142008, 11142018, 11142036, 11143027, 11143047, 11145101, 11148001, 11148006, 11148014, 11148045, 11148055, 11149001, 11149010, 11149015, 11149017, 11149047, 11150029, 11151050, 11151051, 11152036, 11153032, 11153042, 11154026, 11156003, 11156009, 11156036, 11156043, 11156044, 11156045, 12114007, 12114092, 12114103, 12115014, 12115015, 12115016, 12115018, 12115019, 12115020, 12115022, 12115023, 12115073, 12120026, 12121017, 12121022, 12121034, 12122019, 12172049, 12172050, 12172051, 12173031, 12173034, 12174077, 12174085, 12175030, 12175040, 12175062, 12176044, 12176047, 12176067, 12176069, 12176104, 12177015, 12177101, 12178051, 12178120, 12179068, 11002145, 11003001, 11003002, 11003004, 11003005, 11003006, 11003012, 11003013, 11023046, 11023048, 11025034, 11025054, 11026010, 11026114, 11028004, 11028005, 11028006, 11028018, 11028019, 11028020, 11028021, 11028022, 11030041, 11030042, 11057012, 11057035, 11059060, 11060059, 11064003, 11070051, 11070052, 11070053, 11070054, 11070055, 11070056, 11070057, 11071001, 11071002, 11071003, 11071004, 11071006, 11071007, 11071013, 11071014, 11071015, 11071016, 11071017, 11071018, 11071019, 11071020, 11072030, 11072031, 11072032, 11072044, 11072045, 11073001, 11073003, 11073030, 11073031, 11080072, 11084009, 11084010, 11087001, 11087002, 11087003, 11087004, 11087057, 11087058, 11087059, 11089026, 11089028, 11089029, 11089048, 11089055, 11092012, 11092031, 11092032, 11092033, 11092034, 11095040, 11095048, 11095049, 11095050, 11095051, 11095061, 11095062, 11095063, 11095064, 15047023, 15047065, 15047071, 15047087, 15049027, 15051131, 15053034, 15053035, 15053052, 15053053, 15053054, 15053055, 15053056, 15053057, 15061001, 15066064, 15067026, 11099124, 11100045, 11101030, 11101046, 11101104, 11102054, 11102098, 11103008, 11103009, 11103058, 11103062, 11105011, 11105018, 11105029, 11105045, 11106026, 11106027, 11106028, 11106029, 11106030, 11106039, 11106040, 11106041, 11106049, 11109013, 11109088, 11109090, 11109092, 11109097, 11109098, 11109102, 11109103, 11109104, 11110034, 11110041, 11110073, 11110076, 0, 0};
	map<int, int> ref2_min = { {7, 162}, {11, 206}, {14, 225}, {19, 258}, {27, 284}, {39, 307}, {62, 334}, {200, 421} }; //Maps energy (key) to ref2_min (value)
	int proton_min_multi = 1; //Need this many protons or more to be a good event.

	//Proton cuts
	double proton_min_p = 0.4;
	double proton_max_p = 0.8;
	double proton_min_pt = 0.4;
	double proton_max_pt = 2.0;
	double proton_min_beta = 0.0;
	double proton_max_beta = 1.0;
	int proton_charge = 1;
	double proton_min_eta = -0.5;
	double proton_max_eta = 0.5;
	double proton_min_nsigma = -2.0;
	double proton_max_nsigma = 2.0;
	double proton_min_dca = 0.0;
	double proton_max_dca = 1.0;
	double proton_min_m2 = 0.8;
	double proton_max_m2 = 1.0;
}
