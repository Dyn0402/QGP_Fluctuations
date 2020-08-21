/*
 * PileUpQAer.cpp
 *
 *  Created on: Aug 7, 2020
 *      Author: Dylan Neff
 */



#include "PileUpQAer.h"


// Structors
PileUpQAer::PileUpQAer(int energy) {
	start_sys = chrono::system_clock::now();
	this->energy = energy;
}

PileUpQAer::PileUpQAer(int energy, mutex *mtx) {
	start_sys = chrono::system_clock::now();
	this->energy = energy;
	this->mtx = mtx;
}

PileUpQAer::~PileUpQAer() {
	// Nothing
}


// Getters


// Setters
void PileUpQAer::set_energy(int energy) {
	this->energy = energy;
}

void PileUpQAer::set_in_path(string path) {
	in_path = path;
}


// Doers
void PileUpQAer::run_qa() {
	read_trees();
	rotate_dist();
//	get_cuts();
}



void PileUpQAer::read_trees() {
	cout << "Reading " + to_string(energy) + "GeV trees." << endl << endl;
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV/", "root", "path");

	unsigned num_files = in_files.size();
	unsigned file_index = 1;

	for(string path:in_files) {

		// Display progress and time while running.
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
			cout << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (double)(clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
		}

		TFile *file = new TFile(path.data(), "READ");
		TTree *tree = (TTree*)file->Get(tree_name.data());
		read_tree(tree);  // Read tree from file into data
		file->Close();
		delete file;
		file_index++;
	}

	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << "Finished reading " + to_string(energy) + "GeV trees. 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
}



void PileUpQAer::read_tree(TTree *tree) {
	set_tree_branches(tree, branches);
	set_branches(tree);

	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		event_index++;
		ref_btof_pairs.push_back(make_pair(branches.refmult, branches.btof));
	}
}



void PileUpQAer::set_branches(TTree *tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("refmult", 1);
	tree->SetBranchStatus("btof", 1);
}



void PileUpQAer::rotate_dist() {
	out_file = new TFile((out_path+"Pile_QA_"+to_string(energy)+"GeV.root").data(), "RECREATE");
	out_file->cd();

	string name = orig_btof_ref_name+to_string(energy)+"GeV";
	string title = orig_btof_ref_title+to_string(energy)+"GeV";
	TH2F btof_ref_hist(name.data(), title.data(), orig_ref_bins, orig_ref_low, orig_ref_high, orig_btof_bins, orig_btof_low, orig_btof_high);

	map<int, int> slice_count;
	map<int, float> slice_raw_moment1;
	map<int, float> slice_raw_moment2;
	for(pair<short, short> const &event:ref_btof_pairs) {
		btof_ref_hist.Fill(event.first, event.second);
		slice_count[event.second]++;
		slice_raw_moment1[event.second]+=event.first;
		slice_raw_moment2[event.second]+=pow(event.first, 2);
	}

	TF1 lin_fit(("lin_fit_"+name).data(), "[0]*x", orig_ref_low, orig_ref_high);
	btof_ref_hist.Fit(&lin_fit, "NQ");
//	double rot_angle = atan(lin_fit.GetParameter(0));
//
//	cout << rot_angle << endl;

	vector<float> y;
	vector<float> upper_points;
	vector<float> lower_points;
	float sigma = 3.0;
	for(pair<int, int> slice:slice_count) {
		float sd = pow(slice_raw_moment2[slice.first] - pow(slice_raw_moment1[slice.first] / slice.second, 2), 0.5);
		y.push_back(slice.first);
		upper_points.push_back(lin_fit.GetParameter(0) + sigma * sd);
		lower_points.push_back(lin_fit.GetParameter(0) - sigma * sd);
	}

	TGraph upper((int)y.size(), upper_points.data(), y.data());
	TGraph lower((int)y.size(), lower_points.data(), y.data());
	TF1 upper_fit(("upper_fit_"+name).data(), "plo1", orig_ref_low, orig_ref_high);
	TF1 lower_fit(("lower_fit_"+name).data(), "pol1", orig_ref_low, orig_ref_high);
	upper.Fit(&upper_fit, "NQ");
	lower.Fit(&lower_fit, "NQ");

	cout << energy << "GeV\t Upper: " << upper_fit.GetParameter(0) << "+" << upper_fit.GetParameter(0) << "x  |  Lower: " << lower_fit.GetParameter(0) << "+" << lower_fit.GetParameter(0) << "x" << endl;

//	int x_bins = (int)((float)orig_ref_bins*sin(rot_angle) - (float));
//	float x_low = orig_ref_low * sin(rot_angle) - orig_btof_high * cos(rot_angle);
//	float x_high = orig_ref_high * sin(rot_angle) - orig_btof_low * cos(rot_angle);
//	float y_low = orig_ref_low * cos(rot_angle) + orig_btof_low * sin(rot_angle);
//	float y_high = orig_ref_high * cos(rot_angle) + orig_btof_high * sin(rot_angle);
////	TH2F rotate_btof_ref_hist(("rotate_"+name).data(), ("Rotated "+title).data(), orig_ref_bins, -0.5*orig_ref_high, +0.5*orig_ref_high, orig_btof_bins, orig_btof_low, orig_btof_high);
//	TH2F rotate_btof_ref_hist(("rotate_"+name).data(), ("Rotated "+title).data(), (int)(x_high-x_low+1), x_low, x_high, (int)(y_high-y_low+1), y_low, y_high);
//
//	for(pair<short, short> const &event:ref_btof_pairs) {
//		rotate_btof_ref_hist.Fill((float)event.first*sin(rot_angle) - (float)event.second*cos(rot_angle), (float)event.first*cos(rot_angle) + (float)event.second*sin(rot_angle));
////		rotate_btof_ref_hist.Fill((float)event.first*cos(rot_angle) - (float)event.second*sin(rot_angle), (float)event.first*sin(rot_angle) + (float)event.second*cos(rot_angle));
//	}
//
//	TF1 gaus(("gaus_fit_"+name).data(), "gaus", x_low, x_high);
//	TObjArray slices;
////	rotate_btof_ref_hist.FitSlicesX(&gaus, 0, 500, 0, "QNR", &slices);

	mtx->lock();
	{
		TCanvas orig_can(("Can_"+name).data(), title.data());
		btof_ref_hist.Draw("COLZ");
		lin_fit.Draw("same");
		upper.Draw("SameAP");
		lower.Draw("SameAP");
		upper_fit.Draw("same");
		lower_fit.Draw("same");
		orig_can.SetLogz();
		orig_can.Update();
		orig_can.Write();

//		TCanvas rot_can(("Rot_Can_"+name).data(), ("Rotated "+title).data());
//		rotate_btof_ref_hist.Draw("COLZ");
//		rot_can.SetLogz();
//		rot_can.Update();
//		rot_can.Write();
	}
	mtx->unlock();

	out_file->Close();
}


