/*
 * TFile_Size_Test.cpp
 *
 *  Created on: Jan 22, 2021
 *      Author: dylan
 */



#include <iostream>
//#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TRandom3.h"

using namespace std;

void make_files(string path, int nvecs, int nvals, TRandom3 rand, bool junk=false);


void TFile_Size_Test() {
	string test_path = "/home/dylan/Research/TFile_Size_Test/";
	string dir_pre = "nvecs_";
	TRandom3 rand(0);
	int nvals = 40;
	vector<int> nvecs {1, 2, 3};
	bool junk = true;

	for(int nvec:nvecs) {
		string new_dir_path = test_path + dir_pre + to_string(nvec);
		mkdir(new_dir_path.data(), 0777);
		make_files(new_dir_path, nvec, nvals, rand, junk);
	}
	mkdir((test_path+"test").data(), 0777);

	cout << "donzo" << endl;
}


void make_files(string path, int nvecs, int nvals, TRandom3 rand, bool junk) {
	TFile *f_vec = new TFile((path + "/vectors_" + to_string(nvecs) + ".root").data(), "RECREATE");
	TFile *f_arr = new TFile((path + "/arrays_" + to_string(nvecs) + ".root").data(), "RECREATE");
	TFile *f_junk;
	if(junk) { f_junk = new TFile((path + "/junk_" + to_string(nvecs) + ".root").data(), "RECREATE"); }

	f_vec->cd();
	vector<float> vec;
	TTree *vec_tree = new TTree("vec_tree", "vec_tree");
	vec_tree->Branch("vec", &vec, 500000, 1);
	for(int nvec = 0; nvec<nvecs; nvec++) {
		vec.clear();
		for(int i=0; i<nvals; i++) {
			vec.push_back(rand.Rndm());
		}
		vec_tree->Fill();
	}


	f_arr->cd();
	TTree *arr_tree = new TTree("arr_tree", "arr_tree");
	float arr[nvals];
	arr_tree->Branch("arr", &arr, "arr/F");
	for(int nvec = 0; nvec<nvecs; nvec++) {
		for(int i=0; i<nvals; i++) {
			arr[i] = rand.Rndm();
		}
		arr_tree->Fill();
	}


	f_vec->Close();
	f_arr->Close();
	if(junk) { f_junk->Close(); }
}
