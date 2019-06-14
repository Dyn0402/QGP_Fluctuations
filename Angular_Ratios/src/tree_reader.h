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

using namespace std;


vector<double> get_ratio(TTree *tree, int div);


#endif /* TREE_READER_H_ */
