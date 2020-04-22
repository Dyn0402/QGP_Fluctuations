/*
 * speed_tests.cpp
 *
 *  Created on: Mar 2, 2020
 *      Author: Dylan Neff
 */


#include <iostream>
#include <chrono>

#include <TFile.h>
#include <TTree.h>

#include "file_io.h"
#include "Event.h"
#include "Track.h"
#include "TreeLeaves.h"

using namespace std;


void speed_test() {
	int energy = 11;
//	string old_path = "/media/dylan/SSD_Storage/Research/Trees/";
//	string new_path = "/media/dylan/SSD_Storage/Research/Trees_Ref2/";
	string old_path = "/media/dylan/SSD_Storage/Research/Trees_Ref2_Old/";
	string new_path = "/media/dylan/SSD_Storage/Research/Trees_Ref2/";

	vector<string> old_files = get_files_in_dir(old_path+to_string(energy)+"GeV/", "root", "path");
	vector<string> new_files = get_files_in_dir(new_path+to_string(energy)+"GeV/", "root", "path");

//	cout << "Reading New Tree" << endl;
//	auto timer2_start = chrono::system_clock::now();
//	int total_events2 = 0;
//	int total_protons2 = 0;
//	for(string path:new_files) {
//		TFile *file2 = new TFile(path.data(), "READ");
//		TTree *tree2 = (TTree*)file2->Get("tree");
//		Event *event_pointer = new Event;
//		TBranch *event_branch = tree2->GetBranch("Event");
//		event_branch->SetAddress(&event_pointer);
//		int event_index2 = 0;
//		while(tree2->GetEvent(event_index2)) {
//			Event event = *event_pointer;
////			event.get_vx();
////			event.get_vy();
////			event.get_vz();
////			event.get_event_plane();
////			event.get_ref();
////			event.get_run();
////			event.get_refn();
////			event.get_btof();
//			for(Track proton:event.get_protons()) {
//				total_protons2++;
//			}
//			event_index2++;
//		}
//		total_events2 += event_index2;
//		file2->Close();
//		delete file2;
//	}
//	chrono::duration<double> time2 = chrono::system_clock::now() - timer2_start;

	cout << "Reading Old Tree" << endl;
	auto timer1_start = chrono::system_clock::now();
	int total_events1 = 0;
	int total_protons1 = 0;
	for(string path:old_files) {
		TFile *file1 = new TFile(path.data(), "READ");
		TTree *tree1 = (TTree*)file1->Get("nsmTree");
		tree_leaves leaves = get_tree_leaves(tree1);
		int event_index1 = 0;
		while(tree1->GetEvent(event_index1)) {
			Event event(leaves);
//			event.get_vx();
//			event.get_vy();
//			event.get_vz();
//			event.get_event_plane();
//			event.get_ref();
//			event.get_run();
//			event.get_refn();
//			event.get_btof();
			for(Track proton:event.get_protons()) {
				total_protons1++;
			}
			event_index1++;
		}
		total_events1 += event_index1;
		file1->Close();
		delete file1;
	}
	chrono::duration<double> time1 = chrono::system_clock::now() - timer1_start;




	cout << "Old Tree:\t Time: " << time1.count() << "s  | Num events: " << total_events1 << "  |  Rate: " << (double)total_events1 / time1.count() << endl;
	cout << "Num_potons1: " << total_protons1 << endl;
//	cout << "New Tree:\t Time: " << time2.count() << "s  | Num events: " << total_events2 << "  |  Rate: " << (double)total_events2 / time2.count() << endl;
//	cout << "Num_potons2: " << total_protons2 << endl;
}
