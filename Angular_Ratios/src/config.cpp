/*
 * config.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */


#include <string>
#include <vector>

using namespace std;


namespace config {
	string out_path = "/star/u/dneff/Documents/Roli_Thesis/";
	string in_path = "/gpfs01/star/pwg/dneff/BES1_Trees/dylan_trees/";
	string out_root_name = "test.root";


	vector<string> file_list = {"auau7","auau11"};
	vector<int> divs = {2,3,4,5,6};

	string tree_name = "nsmTree";

}
