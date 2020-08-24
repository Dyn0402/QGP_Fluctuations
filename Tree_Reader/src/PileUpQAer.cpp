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
pair<float, float> PileUpQAer::get_low_cut() {
	if(low_cut.first == 0 && low_cut.second == 0) {
		read_cut_file();
	}

	return low_cut;
}

pair<float, float> PileUpQAer::get_high_cut() {
	if(high_cut.first == 0 && high_cut.second == 0) {
		read_cut_file();
	}

	return high_cut;
}


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
	write_cut_file();
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
		ref_btof_pairs.push_back(make_pair(branches.refmult, branches.btof_match));
	}
}



void PileUpQAer::set_branches(TTree *tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("refmult", 1);
	tree->SetBranchStatus("btof_match", 1);
	tree->SetBranchStatus("btof_multi", 1);
}



void PileUpQAer::rotate_dist() {
	out_file = new TFile((out_path+"Pile_QA_"+to_string(energy)+"GeV.root").data(), "RECREATE");
	out_file->cd();

	string name = orig_btof_ref_name+to_string(energy)+"GeV";
	string title = orig_btof_ref_title+to_string(energy)+"GeV";
	TH2F btof_ref_hist(name.data(), title.data(), orig_ref_bins, orig_ref_low, orig_ref_high, orig_btof_bins, orig_btof_low, orig_btof_high);
	btof_ref_hist.GetXaxis()->SetTitle("Refmult");
	btof_ref_hist.GetYaxis()->SetTitle("BTOFMatch");

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
	double rot_angle = M_PI / 2 - atan(lin_fit.GetParameter(0));

	cout << rot_angle << endl;

	vector<float> y;
	vector<float> upper_points;
	vector<float> lower_points;
	for(pair<int, int> slice:slice_count) {
		float mean = slice_raw_moment1[slice.first] / slice.second;
		float sd = pow(slice_raw_moment2[slice.first] / slice.second - pow(mean, 2), 0.5);
		cout << "btof " << slice.first << " mean " << mean << "  sd " << sd << endl;
		if(slice.second > min_points) {
			y.push_back(slice.first);
			lower_points.push_back(mean + sigmas * sd);
			upper_points.push_back(mean - sigmas * sd);
		}
	}

	vector<float> test_x {100, 200, 300, 400};
	vector<float> test_y {10, 20, 30, 40, 50};
	TGraph test((int)test_x.size(), test_x.data(), test_y.data());
	TGraph upper((int)y.size(), upper_points.data(), y.data());
	upper.SetLineColor(kGreen); upper.SetLineWidth(2);
	TGraph lower((int)y.size(), lower_points.data(), y.data());
	lower.SetLineColor(kGreen); lower.SetLineWidth(2);
	TF1 upper_fit(("upper_fit_"+name).data(), "pol1", orig_ref_low, orig_ref_high);
	upper_fit.SetLineColor(kRed);
	TF1 lower_fit(("lower_fit_"+name).data(), "pol1", orig_ref_low, orig_ref_high);
	lower_fit.SetLineColor(kRed);
	upper.Fit(&upper_fit, "NQ");
	lower.Fit(&lower_fit, "NQ");

//	cout << energy << "GeV\t Upper: " << upper_fit.GetParameter(0) << "+" << upper_fit.GetParameter(1) << "x  |  Lower: " << lower_fit.GetParameter(0) << "+" << lower_fit.GetParameter(1) << "x" << endl;
	low_cut = make_pair(lower_fit.GetParameter(0), lower_fit.GetParameter(1));
	high_cut = make_pair(upper_fit.GetParameter(0), upper_fit.GetParameter(1));

	float x_low = orig_ref_low * cos(rot_angle) - orig_btof_high * sin(rot_angle);
	float x_high = orig_ref_high * cos(rot_angle) - orig_btof_low * sin(rot_angle);
	float y_low = orig_ref_low * sin(rot_angle) + orig_btof_low * cos(rot_angle);
	float y_high = orig_ref_high * sin(rot_angle) + orig_btof_high * cos(rot_angle);
	TH2F rotate_btof_ref_hist(("rotate_"+name).data(), ("Rotated "+title).data(), (int)(x_high-x_low+1), x_low, x_high, (int)(y_high-y_low+1), y_low, y_high);

	map<int, TH1F> slices;

	for(pair<short, short> const &event:ref_btof_pairs) {
		pair<float, float> x_y_rotate = rotate_xy((float)event.first, (float)event.second, rot_angle);
		rotate_btof_ref_hist.Fill(x_y_rotate.first, x_y_rotate.second);
		int slice_index = x_y_rotate.second / rot_slice_height;
		if(slices.count(slice_index) == 0) {
			string title = to_string(energy) + "GeV Slice " + to_string(slice_index * rot_slice_height) + " to " +to_string((slice_index + 1) *rot_slice_height);
			slices[slice_index] = TH1F(title.data(), title.data(), (int)(x_high-x_low+1), x_low, x_high);
		}
		slices[slice_index].Fill(x_y_rotate.first);
	}

	mtx->lock();
	{
		TCanvas orig_can(("Can_"+name).data(), title.data());
		btof_ref_hist.Draw("COLZ");
//		lin_fit.Draw("same");
		upper.Draw("sameL");
		lower.Draw("sameL");
		upper_fit.Draw("same");
		lower_fit.Draw("same");
		orig_can.SetLogz();
		orig_can.Update();
		orig_can.Write();

		TCanvas rot_can(("Rot_Can_"+name).data(), ("Rotated "+title).data());
		rotate_btof_ref_hist.Draw("COLZ");
		rot_can.SetLogz();
		rot_can.Update();
		rot_can.Write();

		for(pair<int, TH1F> slice:slices) {
			slice.second.Write();
		}
	}
	mtx->unlock();

	out_file->Close();
}



void PileUpQAer::write_cut_file() {
	if(!check_path(out_path)) { cout << "Can't access path: " << out_path << " Skipping following read/write." << endl; return; }

	string out_name = out_path + to_string(energy) + "GeV" + out_file_suf;
	ofstream out_txt(out_name);
	if(!out_txt.is_open()) { cout << "Could not open " << out_name << " Not writing." << endl; return; }
	out_txt << energy << "GeV Pile Up cuts with lines of mean +-" << sigmas << " sigmas for each slice with more than " << min_points << " points: " << endl;
	out_txt << "High cut line (intercept,slope):\t" << high_cut.first << "," << high_cut.second << endl;
	out_txt << "Low cut line (intercept,slope):\t" << low_cut.first << "," << low_cut.second << endl;

	out_txt.close();
}


void PileUpQAer::read_cut_file() {
	if(!check_path(out_path)) { cout << "Can't access path: " << out_path << " Skipping following read/write." << endl; return; }

	string in_name = out_path + to_string(energy) + "GeV" + out_file_suf;
	ifstream in_txt(in_name);
	if(!in_txt.is_open()) { cout << "Could not open " << in_name << " Not reading." << endl; return; }

	pair<float, float> low_cut_read;
	pair<float, float> high_cut_read;

	string line;
	int line_num = 1;
	while(getline(in_txt, line)) {
		if(line_num == 2) {
			vector<string> highs = split(split(line, '\t')[1], ',');
			high_cut.first = stof(highs[0]);
			high_cut.second = stof(highs[1]);
		}
		if(line_num == 3) {
			vector<string> lows = split(split(line, '\t')[1], ',');
			low_cut.first = stof(lows[0]);
			low_cut.second = stof(lows[1]);
		}
		line_num++;
	}

	in_txt.close();
}


pair<float, float> rotate_xy(float x, float y, float angle) {
	float x_rot = x * cos(angle) - y * sin(angle);
	float y_rot = x * sin(angle) + y * cos(angle);

	return make_pair(x_rot, y_rot);
}
