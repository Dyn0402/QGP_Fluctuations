/*
 * ratio_methods.cpp
 *
 *  Created on: Jun 17, 2019
 *      Author: Dylan Neff
 */


#include <vector>
#include <math.h>

using namespace std;


vector<double> get_Rs(vector<double> angles, int divisions) {
	vector<double> Rs = {};
	double r, low_cut, high_cut;

	for(int i = 0; i < divisions; i++) {
		r = 0.0;
		low_cut = i * 2 * M_PI / divisions;
		high_cut = (i+1) * 2 * M_PI / divisions;

		for(auto angle:angles) {
			if(angle >= low_cut  && angle < high_cut) {
				r++;
			}
			if(i+1 == divisions && angle == high_cut) {
				r++;
			}
		}
		Rs.push_back(r/angles.size());
	}

	return(Rs);
}
