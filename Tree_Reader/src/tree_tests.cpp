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
#include <TInterpreter.h>
#include <TSystem.h>
#include <TROOT.h>

#include "Track.h"

using namespace std;


void vec_tree_test() {
	TFile *file = new TFile("/home/dylan/Desktop/vec_tree_test.root", "RECREATE");

	if(!file) { cout << "Could not open file" << endl; return; }

	vector<double> track_vec;

	TTree *tree = new TTree("vec_test_tree", "Test Tree");
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


void obj_tree_test() {
	TFile *file = new TFile("/home/dylan/Desktop/obj_tree_test.root", "RECREATE");

//	gSystem->Load()
//	gROOT->ProcessLine(".L ./test_class.h+");
//	gInterpreter->GenerateDictionary("Track", "test_class.h");

	if(!file) { cout << "Could not open file" << endl; return; }

	Track tracks;

	TTree *tree = new TTree("obj_test_tree", "Test Tree");
	tree->Branch("test_obj", "Track", &tracks);

	TRandom3 *rand = new TRandom3(0);

	for(int i=0; i<10000; i++) {
		tracks.clear();
		for(int j=0; j<rand->Gaus(15, 0.5); j++) {
			tracks.track_size.push_back(rand->Gaus(10, 0.2));
		}
		tree->Fill();
	}
	file->Write();
	delete file;
}
