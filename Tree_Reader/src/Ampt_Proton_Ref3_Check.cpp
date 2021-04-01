/*
 * Ampt_Proton_Ref3_Check.cpp
 *
 *  Created on: Sep 24, 2020
 *      Author: Dylan Neff
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "TFile.h"
#include "TTree.h"

#include "Event.h"
#include "Track.h"
#include "AmptCentralityMaker.h"
#include "TreeCuts.h"
#include "file_io.h"

using namespace std;


bool check_particle_good2(Track &particle, TreeCuts &cut, bool check_charge) {
	bool good_particle = false;

	if(check_charge && !(particle.get_charge() == cut.charge)) { return false; }

	double eta = particle.get_eta();
	if(!(eta >= cut.min_eta && eta <= cut.max_eta)) { return false; }

	double nsigma = particle.get_nsigma();
	if(!(nsigma >= cut.min_nsigma && nsigma <= cut.max_nsigma)) { return false; }

	double dca = particle.get_dca();
	if(!(dca >= cut.min_dca && dca <= cut.max_dca)) { return false; }

	double p = particle.get_p();
	double pt = particle.get_pt();

	if(pt >= cut.min_pt_no_tof && pt <= cut.max_pt_no_tof && p <= cut.max_p_no_tof) {
		good_particle = true;
	} else if(pt >= cut.min_pt_tof && pt <= cut.max_pt_tof && p <= cut.max_p_tof) {
		double beta = particle.get_beta();
		if(beta > cut.min_beta) {
			double m2 = pow(p, 2) * (pow(beta, -2) - 1.);
			if(m2 > cut.min_m2 && m2 < cut.max_m2) {
				good_particle = true;
			}
		}  // Else leave good_particle = false
	}

	return good_particle ;
}

void set_ampt_branches2(TTree* tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("pid", 1);
	tree->SetBranchStatus("px", 1);
	tree->SetBranchStatus("py", 1);
	tree->SetBranchStatus("pz", 1);

	tree->SetBranchStatus("refmult3", 1);
}

void AMPT_Proton_Num_Check() {
	string out_path = "/home/dylan/Research/Ampt_Check/";

	string in_path = "/media/ucla/Research/AMPT_Trees/";
	int energy = 7;
	string set_name = "test_ampt";
	string tree_name = "tree";
	vector<int> ampt_particle_pid {2212};

	AmptCentralityMaker ampt_cent(energy, "/media/ucla/Research/AMPT_Trees/min_bias/", "/home/dylan/Research/Ampt_Centralities/", "ref" + to_string(3));
	ampt_cent.make_centrality(false);

	cout << "Reading " + set_name + " " + to_string(energy) + "GeV trees." << endl << endl;
	vector<string> in_files = get_files_in_dir(in_path + "most_central/" + to_string(energy)+"GeV/", "root", "path");
	vector<string> min_bias_files = get_files_in_dir(in_path + "min_bias/" + to_string(energy)+"GeV/", "root", "path");
	in_files.insert(in_files.end(), min_bias_files.begin(), min_bias_files.end());
	random_shuffle(in_files.begin(), in_files.end());

	unsigned num_files = in_files.size();
	unsigned file_index = 1;

	ampt_tree_branches ampt_branches;
	TreeCuts cuts(energy, "proton");
	event_defaults event_defs;
	track_defaults track_defs;
	TH1D protons_cent("Central_protons", "Most Central Proton Mult", 61, -0.5, 60.5);
	TH2D protons_ref3("Protons_vs_ref3", "RefMult3 vs Protons", 81, -0.5, 80.5, 601, -0.5, 600.5);

	for(string path:in_files) {
//		cout << path << endl;
		// Display progress and time while running.
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
			cout << " " << set_name << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete   | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
		}

		TFile file(path.data(), "READ");
		TTree *tree = (TTree*)file.Get(tree_name.data());

		set_ampt_tree_branches(tree, ampt_branches);
		set_ampt_branches2(tree);

		int cent_9bin;
		int event_index = 0;
		while(tree->GetEvent(event_index++)) {
			cent_9bin = ampt_cent.get_cent_bin9(ampt_branches.refmult3);
			Event event(event_defs, energy, 3, cent_9bin);

			vector<Track> particles;
			for(int particle_index = 0; particle_index < (int)ampt_branches.pid->size(); particle_index++) {
				if(!count(ampt_particle_pid.begin(), ampt_particle_pid.end(), ampt_branches.pid->at(particle_index))) { continue; }
				TVector3 p(ampt_branches.px->at(particle_index), ampt_branches.py->at(particle_index), ampt_branches.pz->at(particle_index));
				Track new_particle(track_defs);
				new_particle.set_p(p.Mag());
				new_particle.set_pt(p.Perp());
				new_particle.set_phi(p.Phi() + M_PI);
				new_particle.set_eta(p.PseudoRapidity());
				new_particle.set_charge(ampt_branches.pid->at(particle_index) / fabs(ampt_branches.pid->at(particle_index)));
				new_particle.set_beta(pow((cuts.max_m2 + cuts.min_m2) / (2 * pow(p.Mag(), 2)) + 1, -0.5));  // Pass m^2 cut
				if(check_particle_good2(new_particle, cuts, true)) {
					particles.push_back(new_particle);
				}
			}
			event.set_particles(particles);

			if(cent_9bin == 8) { protons_cent.Fill(event.get_num_particles()); }
			protons_ref3.Fill(event.get_num_particles(), ampt_branches.refmult3);
		}

		tree->ResetBranchAddresses();

		file.Close();
		file_index++;
	}

	TFile out_file((out_path+"/check.root").data(), "RECREATE");

	TCanvas can_cent("Protons_Cent_Can", "Most Central Protons");
	protons_cent.Draw();
	can_cent.Write();

	TCanvas can_ref3("Protons_Ref3_Can", "RefMult3 vs Protons");
	protons_ref3.Draw();
	can_ref3.Write();

	out_file.Close();

}


