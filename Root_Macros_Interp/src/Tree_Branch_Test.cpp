
#include <iostream>
#include <vector>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"

using namespace std;

struct event_vars {
	int run;
};

struct track_vars {
	vector<float> px;
	vector<float> py;
};

void Tree_Branch_Test() {
	TFile *f = new TFile("/home/dylan/Desktop/test.root", "RECREATE");
	TTree *t = new TTree("tree", "tree");
	TRandom3 *r = new TRandom3(0);

	event_vars event;
	t->Branch("run", &event.run, "run/I");
	track_vars p_tracks;
	t->Branch("p.", "px", &p_tracks.px, 8000, 1);
	t->Branch("p.", "py", &p_tracks.py, 8000, 1);
	track_vars r_tracks;
	t->Branch("r.px", &r_tracks.px, 8000, 1);
	t->Branch("r.py", &r_tracks.py, 8000, 1);

	for(int i=0; i<10; i++) {
		event.run = i;
		p_tracks.px.clear(); p_tracks.py.clear();
		r_tracks.px.clear(); r_tracks.py.clear();
		for(int j=0; j<5; j++) {
			p_tracks.px.push_back(r->Rndm());
			p_tracks.py.push_back(r->Rndm());
			r_tracks.px.push_back(r->Rndm());
			r_tracks.py.push_back(r->Rndm());
		}
		t->Fill();
	}
	f->Write();
	f->Close();
	cout << "donzo" << endl;
}
