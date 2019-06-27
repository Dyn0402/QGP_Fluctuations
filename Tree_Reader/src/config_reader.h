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
	extern string in_file_prefx;
	extern string in_file_sufx;

	extern vector<int> energy_list;
	extern vector<int> divs;
	extern vector<int> centrals;
	extern vector<int> cumulant_orders;

	extern string tree_name;
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

#endif /* CONFIG_READER_H_ */
