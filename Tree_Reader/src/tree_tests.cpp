/*
 * tree_tests.cpp
 *
 *  Created on: Aug 21, 2019
 *      Author: Dylan Neff
 */

#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>

using namespace std;


class Track {
public:
	Track();
	vector<double> track_size;
};

Track::Track() {};


void tree_test() {
	TFile *file = new TFile("/home/dylan/Desktop/tree_test.root", "RECREATE");

	if(!file) { cout << "Could not open file" << endl; return; }

	vector<double> track_vec;

	TTree *tree = new TTree("test_tree", "Test Tree");
	tree->Branch("test_vec", &track_vec);

	TRandom3 *rand = new TRandom3(0);

	for(int i=0; i<10000; i++) {
		track_vec.clear();
		for(int j=0; j<rand->Gaus(15, 0.5); j++) {
			track_vec.push_back(rand->Gaus(10, 0.2));
		}
		tree->Fill();
	}
	file->Write();
	delete file;
}
