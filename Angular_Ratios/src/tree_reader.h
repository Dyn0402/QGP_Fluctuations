/*
 * tree_reader.h
 *
 *  Created on: Jun 13, 2019
 *      Author: Dylan Neff
 */

#ifndef TREE_READER_H_
#define TREE_READER_H_

#include <vector>

#include <TTree.h>
#include <TLeaf.h>

using namespace std;


vector<double> get_tree_ratios(TTree *tree, int div);
vector<double> get_event_ratios(TLeaf *proton_psi, int div);


#endif /* TREE_READER_H_ */
