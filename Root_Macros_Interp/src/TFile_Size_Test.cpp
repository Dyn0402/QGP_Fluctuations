/*
 * TFile_Size_Test.cpp
 *
 *  Created on: Jan 22, 2021
 *      Author: dylan
 */


#define HAVE_STRUCT_TIMESPEC
#include <iostream>
//#include <sys/stat.h>
#include <vector>
#include <numeric>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TRandom3.h"

using namespace std;

void make_files(string path, int nvecs, int nvals, TRandom3 rand, bool junk=false);
void make_files4(string path, int nvecs, int nvals, TRandom3 rand, bool junk=false);


void TFile_Size_Test() {
	string test_path = "/home/dylan/Research/TFile_Size_Test4/";
	string dir_pre = "nvecs_";
	TRandom3 rand(0);
	bool junk = true;
	int nvals = 40;
	vector<int> nvecs;
	for(int i=1; i<100; i++) {
		nvecs.push_back(i * 1000);
	}

	for(int nvec:nvecs) {
		make_files(test_path, nvec, nvals, rand, junk);
	}

	cout << "donzo" << endl;
}


void make_files(string path, int nvecs, int nvals, TRandom3 rand, bool junk) {
	TFile *f_vec = new TFile((path + "/vectors_" + to_string(nvecs) + "_" + to_string(nvals) + ".root").data(), "RECREATE");
	TFile *f_arr = new TFile((path + "/arrays_" + to_string(nvecs) + "_" + to_string(nvals) + ".root").data(), "RECREATE");
	TFile *f_junk;
	if(junk) { f_junk = new TFile((path + "/junk_" + to_string(nvecs) + "_" + to_string(nvals) + ".root").data(), "RECREATE"); }

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
	f_vec->Write();


	f_arr->cd();
	float arr[nvals];
	TTree *arr_tree = new TTree("arr_tree", "arr_tree");
	string arr_type = "arr[" + to_string(nvals) + "]/F";
	arr_tree->Branch("arr", &arr, arr_type.data());
	for(int nvec = 0; nvec<nvecs; nvec++) {
		for(int i=0; i<nvals; i++) {
			arr[i] = rand.Rndm();
		}
		arr_tree->Fill();
	}
	f_arr->Write();


	f_vec->Close();
	f_arr->Close();
	if(junk) { f_junk->Close(); }
}


void make_files4(string path, int nvecs, int nvals, TRandom3 rand, bool junk) {
	TFile *f_vec = new TFile((path + "/vectors_" + to_string(nvecs) + "_" + to_string(nvals) + ".root").data(), "RECREATE");
	TFile *f_arr = new TFile((path + "/arrays_" + to_string(nvecs) + "_" + to_string(nvals) + ".root").data(), "RECREATE");
	TFile *f_junk;
	if(junk) { f_junk = new TFile((path + "/junk_" + to_string(nvecs) + "_" + to_string(nvals) + ".root").data(), "RECREATE"); }

	TTree *vec_tree = new TTree("vec_tree", "vec_tree");
	TTree *arr_tree = new TTree("arr_tree", "arr_tree");

	f_vec->cd();
	vector<float> vec;
	vec_tree->Branch("vec", &vec, 500000, 1);
	for(int nvec = 0; nvec<nvecs; nvec++) {
		vec.clear();
		for(int i=0; i<nvals; i++) {
			vec.push_back(rand.Rndm());
		}
		vec_tree->Fill();
	}
	f_vec->Write();


	f_arr->cd();
	float arr[nvals];
	string arr_type = "arr[" + to_string(nvals) + "]/F";
	arr_tree->Branch("arr", &arr, arr_type.data());
	for(int nvec = 0; nvec<nvecs; nvec++) {
		for(int i=0; i<nvals; i++) {
			arr[i] = rand.Rndm();
		}
		arr_tree->Fill();
	}
	f_arr->Write();


	f_vec->Close();
	f_arr->Close();
	if(junk) { f_junk->Close(); }
}
