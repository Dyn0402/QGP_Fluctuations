/*
 * plotting.h
 *
 *  Created on: Jun 18, 2019
 *      Author: Dylan Neff
 */

#ifndef PLOTTING_H_
#define PLOTTING_H_

#include <vector>
#include <string>

using namespace std;

void make_ratio_dist_plots(vector<tree_data> data);
void hist_ratio_dist(vector<double> ratios, string file, int div);
void make_proton_dist_plots(vector<tree_data> data);
void hist_proton_dist(vector<int> nprotons, string file);



#endif /* PLOTTING_H_ */
