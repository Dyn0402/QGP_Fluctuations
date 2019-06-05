/*
 * tree_reader.h
 *
 *  Created on: Jun 4, 2019
 *      Author: Dylan Neff
 */

#ifndef TREE_READER_H_
#define TREE_READER_H_

#include <TFile.h>
#include <TTree.h>
#include <TH1.h>


TH1D* get_dist(TTree *tree);
TH1D* get_flat_dist(TTree *tree, string coef_path, string entry);
void flatten_tree(TTree *tree, string entry, string out_path, TFile *out_file);


#endif /* TREE_READER_H_ */
