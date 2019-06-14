/*
 * tree_reader.cpp
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */



#include <vector>

#include <TTree.h>
#include <TLeaf.h>

using namespace std;


vector<double> get_ratio(TTree *tree, int div) {
	vector<double> ratios;
	TLeaf *psi = tree->GetLeaf("Proton.phi");



	return(ratios);
}
