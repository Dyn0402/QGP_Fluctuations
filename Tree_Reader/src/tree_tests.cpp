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
#include "Event.h"

using namespace std;


class Track_Test {
public:
	Track_Test();
	vector<double> track_size;
};

Track_Test::Track_Test() {};


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


//void obj_tree_test() {
//	TFile *file = new TFile("/home/dylan/Desktop/obj_tree_test.root", "RECREATE");
//
////	gSystem->Load()
////	gROOT->ProcessLine(".L ./test_class.h+");
////	gInterpreter->GenerateDictionary("Track", "test_class.h");
//
//	if(!file) { cout << "Could not open file" << endl; return; }
//
//	Track tracks;
//
//	TTree *tree = new TTree("obj_test_tree", "Test Tree");
//	tree->Branch("test_obj", "Track", &tracks);
//
//	TRandom3 *rand = new TRandom3(0);
//
//	for(int i=0; i<10000; i++) {
//		tracks.clear();
//		for(int j=0; j<rand->Gaus(15, 0.5); j++) {
//			tracks.track_size.push_back(rand->Gaus(10, 0.2));
//		}
//		tree->Fill();
//	}
//	file->Write();
//	delete file;
//}


void real_tree_test() {
	// Write
	TFile *file = new TFile("/home/dylan/Desktop/real_tree_test.root", "RECREATE");

	if(!file) { cout << "Could not open file" << endl; return; }

	Event event;
	vector<Track> tracks;

	TTree *tree = new TTree("test_tree", "Test Tree");
	tree->Branch("event", &event);
	tree->Branch("track_vec", &tracks);

	int ntracks = 0;

	TRandom3 *r = new TRandom3(0);

	for(int i=0; i<10000; i++) {
		tracks.clear();
		event.set_event(r->Gaus(0,5), r->Gaus(0,4), r->Gaus(1, 6), (unsigned) r->Gaus(20,2), (unsigned) r->Gaus(40,3), (unsigned) r->Gaus(60,5), (unsigned) r->Gaus(80,7), (unsigned) r->Gaus(0, 3));
		for(int j=0; j<r->Gaus(15, 0.5); j++) {
			Track track;
			track.set_track(r->Gaus(10, 2), r->Gaus(15, 3), r->Gaus(3, 2), r->Gaus(0, 3), r->Gaus(0, 3), r->Gaus(2, 0.5), r->Gaus(0.5, 0.1), (short)r->Gaus(0, 0.5));
			tracks.push_back(track);
			ntracks++;
		}
		tree->Fill();
	}
	file->Write();
	delete file;

	cout << ntracks << endl;


	// Read
	TFile *rfile = new TFile("/home/dylan/Desktop/real_tree_test.root", "READ");

	if(!rfile) { cout << "Could not open file" << endl; return; }

	TTree *rtree = (TTree*) rfile->Get("test_tree");

//	Event *revent;
//	vector<Track> *rtracks;

	Event *revent = new Event;
	vector<Track> *rtracks = new vector<Track>;

	auto bevent = rtree->GetBranch("event");
	auto btrack_vec = rtree->GetBranch("track_vec");

	bevent->SetAddress(&revent);
	btrack_vec->SetAddress(&rtracks);

	auto nevent = rtree->GetEntries();
	cout << nevent << endl;

//	rtree->GetEvent(100);
//	cout << revent.get_vtx_x() << endl;
//	cout << rtracks.size() << endl;

	rtree->GetEvent(100);
	cout << revent->get_vx() << endl;
	cout << rtracks->size() << endl;
	for(unsigned i=0; i<rtracks->size(); i++) {
		cout << (*rtracks)[i].get_pt() << endl;
	}

	cout << endl << endl;

	rtree->GetEvent(103);
	cout << revent->get_vx() << endl;
	cout << rtracks->size() << endl;
	for(unsigned i=0; i<rtracks->size(); i++) {
		cout << (*rtracks)[i].get_pt() << endl;
	}

	delete rfile;
}


void real_event_tree_test() {
	// Write
	TFile *file = new TFile("/home/dylan/Desktop/real_tree_test.root", "RECREATE");

	if(!file) { cout << "Could not open file" << endl; return; }

	Event event;

	TTree *tree = new TTree("test_tree", "Test Tree");
	tree->Branch("event", &event);

	int ntracks = 0;

	TRandom3 *r = new TRandom3(0);

	for(int i=0; i<10000; i++) {
		event.set_event(r->Gaus(0,5), r->Gaus(0,4), r->Gaus(1, 6), (unsigned) r->Gaus(20,2), (unsigned) r->Gaus(40,3), (unsigned) r->Gaus(60,5), (unsigned) r->Gaus(80,7), r->Gaus(0,3));
		vector<Track> tracks;
		for(int j=0; j<r->Gaus(15, 0.5); j++) {
			Track track;
			track.set_track(r->Gaus(10, 2), r->Gaus(15, 3), r->Gaus(3, 2), r->Gaus(0, 3), r->Gaus(0, 3), r->Gaus(2, 0.5), r->Gaus(0.5, 0.1), (short)r->Gaus(0, 0.5));
			tracks.push_back(track);
			ntracks++;
		}
		if(i == 100) {
			cout << endl << "Event 100: " << endl;
			for(auto t:tracks) { cout << t.get_pt() << endl; }
		}
		if(i == 101) {
			cout << endl << "Event 101: " << endl;
			for(auto t:tracks) { cout << t.get_pt() << endl; }
		}
		event.set_protons(tracks);
		tree->Fill();
	}

	file->Write();
	delete file;

	cout << ntracks << endl;


	// Read
	TFile *rfile = new TFile("/home/dylan/Desktop/real_tree_test.root", "READ");

	if(!rfile) { cout << "Could not open file" << endl; return; }

	TTree *rtree = (TTree*) rfile->Get("test_tree");

//	Event *revent;
//	vector<Track> *rtracks;

	Event *revent = new Event;

	auto bevent = rtree->GetBranch("event");

	bevent->SetAddress(&revent);

	auto nevent = rtree->GetEntries();
	cout << nevent << endl;

//	rtree->GetEvent(100);
//	cout << revent.get_vtx_x() << endl;
//	cout << rtracks.size() << endl;

	rtree->GetEvent(100);
	Event local_event = *revent;
	cout << endl << "Event 100" << endl;
	vector<Track> protons = local_event.get_protons();
	cout << "vx: " << local_event.get_vx() << endl;
	local_event.set_vx(1 + local_event.get_vx());
	cout << "Num Protons: " << protons.size() << endl;
	for(unsigned i=0; i<protons.size(); i++) {
		cout << "Proton " << i << ": " << protons[i].get_pt() << endl;
		protons[i].set_pt(protons[i].get_pt() + 1);
	}

	cout << endl << endl;

	rtree->GetEvent(100);
	cout << endl << "Event 100" << endl;
	protons = revent->get_protons();
	cout << "vx: " << revent->get_vx() << endl;
	cout << "Num Protons: " << protons.size() << endl;
	for(unsigned i=0; i<protons.size(); i++) {
		cout << "Proton " << i << ": " << protons[i].get_pt() << endl;
	}

	delete rfile;
}
