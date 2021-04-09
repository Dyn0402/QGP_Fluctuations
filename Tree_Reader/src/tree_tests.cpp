///*
// * tree_tests.cpp
// *
// *  Re-Created on: Apr 8, 2021
// *      Author: Dylan Neff
// */
//


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

#include "Event.h"
#include "TreeBranches.h"
#include "file_io.h"

#ifdef _WIN32
static const std::string platform = "win";
#else
static const std::string platform = "lin";
#endif

using namespace std;


void read_test_tree(TTree* tree, ampt_tree_branches &branches);
void set_test_ampt_branches(TTree* tree, int ref_num);


void tree_read_speed() {
	string file_dir;
	if (platform == "lin") { file_dir = "/media/ssd/Research/ampt_tree_read_compare/test_set/"; }
	else if (platform == "win") { file_dir = "E:/Research/ampt_tree_read_compare/test_set/"; }

	clock_t start = clock();
	chrono::system_clock::time_point start_sys = chrono::system_clock::now();

	ampt_tree_branches ampt_branches;

	vector<string> in_files = get_files_in_dir(file_dir, "root", "path");

	unsigned num_files = in_files.size();
	unsigned file_index = 1;
	for (string path : in_files) {
		cout << path << endl;
		// Display progress and time while running.
		if (!(file_index % (unsigned)(num_files / 10.0 + 0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
			cout << (int)(100.0 * file_index / num_files + 0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
		}

		TFile file(path.data(), "READ");
		TTree* tree = (TTree*)file.Get("tree");
		read_test_tree(tree, ampt_branches);  // Read tree from file into data
		tree->ResetBranchAddresses();
		file.Close();
		file_index++;
	}

	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << " 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
}


void read_test_tree(TTree* tree, ampt_tree_branches& ampt_branches) {
	int ref_num = 3;
	set_ampt_tree_branches(tree, ampt_branches);
	set_test_ampt_branches(tree, ref_num);

	int* ref_n = NULL;
	switch (ref_num) {
	case 1:
		ref_n = &ampt_branches.refmult; break;
	case 2:
		ref_n = &ampt_branches.refmult2; break;
	case 3:
		ref_n = &ampt_branches.refmult3; break;
	default:
		cout << "Unknown ref_num value!" << endl;
	}

	int cent_9bin;
	int event_index = 0;
	while (tree->GetEvent(event_index)) {

		//cent_9bin = ampt_cent.get_cent_bin9(*ref_n);
		//Event event(event_defs, energy, ref_num, cent_9bin);
		//event.set_qx(ampt_branches.qx); event.set_qy(ampt_branches.qy);

		//vector<Track> particles;
		//for (int particle_index = 0; particle_index < (int)ampt_branches.pid->size(); particle_index++) {
		//	if (!count(ampt_particle_pid.begin(), ampt_particle_pid.end(), ampt_branches.pid->at(particle_index))) { continue; }
		//	TVector3 p(ampt_branches.px->at(particle_index), ampt_branches.py->at(particle_index), ampt_branches.pz->at(particle_index));
		//	Track new_particle(track_defs);
		//	new_particle.set_p(p.Mag());
		//	new_particle.set_pt(p.Perp());
		//	new_particle.set_phi(p.Phi() + M_PI);
		//	new_particle.set_eta(p.PseudoRapidity());
		//	new_particle.set_charge(ampt_branches.pid->at(particle_index) / fabs(ampt_branches.pid->at(particle_index)));
		//	new_particle.set_beta(pow((cut.max_m2 + cut.min_m2) / (2 * pow(p.Mag(), 2)) + 1, -0.5));  // Pass m^2 cut
		//	particles.push_back(new_particle);
		//}
		//event.set_particles(particles);


		event_index++;
	}
}


void set_test_ampt_branches(TTree* tree, int ref_num) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("qx", 1);
	tree->SetBranchStatus("qy", 1);
	tree->SetBranchStatus("pid", 1);
	tree->SetBranchStatus("px", 1);
	tree->SetBranchStatus("py", 1);
	tree->SetBranchStatus("pz", 1);

	switch (ref_num) {
	case 1:
		tree->SetBranchStatus("refmult", 1); break;
	case 2:
		tree->SetBranchStatus("refmult2", 1); break;
	case 3:
		tree->SetBranchStatus("refmult3", 1); break;
	default:
		cout << "Unknown ref_num value!" << endl;
	}
}
