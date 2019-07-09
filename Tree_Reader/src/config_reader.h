/*
 * config.h
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */

#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include <string>
#include <vector>
#include <map>

using namespace std;


namespace pars {
	extern vector<int> divs;

	extern string tree_name;
}


namespace local {
	extern string in_path;
	extern string out_path;
	extern string qa_path;

	extern vector<int> energy_list;
}


namespace cuts {
	//Event cuts
	extern vector<int> bad_runs;
	extern map<int, int> ref2_min; //Maps energy (key) to ref2_min (value)
	extern int min_multi; //Need this many protons or more to be a good event.
	extern map<int, double> max_slope;
	extern map<int, double> min_slope;

	//Proton cuts
	extern double min_p;
	extern double max_p;
	extern double min_pt;
	extern double max_pt;
	extern double min_beta;
	extern double max_beta;
	extern int charge;
	extern double min_eta;
	extern double max_eta;
	extern double min_nsigma;
	extern double max_nsigma;
	extern double min_dca;
	extern double max_dca;
	extern double min_m2;
	extern double max_m2;
	extern double min_pt_for_m;
	extern double max_pt_for_m;
}

#endif /* CONFIG_READER_H_ */
