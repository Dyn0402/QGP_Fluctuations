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

using namespace std;

namespace config {
	extern string out_path;
	extern string in_path;
	extern string out_root_name;


	extern vector<string> tree_in_list;
	extern vector<int> divs;

	extern string tree_name;
}


#endif /* CONFIG_H_ */
