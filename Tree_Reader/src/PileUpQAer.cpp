/*
 * PileUpQAer.cpp
 *
 *  Created on: Aug 7, 2020
 *      Author: Dylan Neff
 */



#include "PileUpQAer.h"

double gaus_exp_r(double *x, double *par) {  // par = {amplitude, x_bar, sigma, k}
	double x_prime = (x[0] - par[1]) / par[2];
	if(x_prime <= -par[3]) {
		return par[0] * exp(-pow(x_prime, 2) / 2);
	} else {
		return par[0] * exp(pow(par[3], 2) / 2 + par[3] * x_prime);
	}
}

double woods_saxon(double *x, double *par) {  // par = {amplitude, width, x_bar}
	return par[0] / (1 + exp(-par[1] * (x[0] - par[2])));
}

double gexp_r_plus_ws(double *x, double *par) {  // par = {amplitude, x_bar, sigma, k, ws_amp, ws_width, ws_x}
	double *par_ws = par + 4;
	return gaus_exp_r(x, par) + woods_saxon(x, par_ws);
}


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
vector<float> PileUpQAer::get_low_cut() {
	if(low_cut.size() == 0) {
		read_cut_file();
	}

	return low_cut;
}

vector<float> PileUpQAer::get_high_cut() {
	if(high_cut.size() == 0) {
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
	tree_branches branches, pion_branches;
	set_tree_branches(tree, branches);
	set_particle_branches(tree, pion_branches, "pion");
	set_branches(tree);

	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		event_index++;
		if(branches.btof_match < min_btof) { continue; }
		PUEvent new_event;
		new_event.ref = branches.refmult;
		new_event.btof = branches.btof_match;
//		new_event.dca_z = *branches.dca_z;
//		new_event.dca_z.insert(new_event.dca_z.end(), pion_branches.dca_z->begin(), pion_branches.dca_z->end());
		new_event.dca_z_avg = 0;
		for(float dca_z:*branches.dca_z) {
			new_event.dca_z_avg += dca_z;
		}
		for(float dca_z:*pion_branches.dca_z) {
			new_event.dca_z_avg += dca_z;
			new_event.dca_z_sd += pow(dca_z, 2);
		}
		new_event.dca_z_avg /= branches.dca_z->size() + pion_branches.dca_z->size();
		new_event.dca_z_sd /= branches.dca_z->size() + pion_branches.dca_z->size();
		new_event.dca_z_sd = sqrt(new_event.dca_z_sd - pow(new_event.dca_z_avg, 2));
		pu_events.push_back(new_event);
	}
}



void PileUpQAer::set_branches(TTree *tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("refmult", 1);
	tree->SetBranchStatus("btof_match", 1);
	tree->SetBranchStatus("btof_multi", 1);
	tree->SetBranchStatus("proton.dca_z", 1);
	tree->SetBranchStatus("pion.dca_z", 1);
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
	map<int, double> slice_raw_moment1;
	map<int, double> slice_raw_moment2;
	map<int, TH1I> orig_slices;
	for(PUEvent const &event:pu_events) {
		btof_ref_hist.Fill(event.ref, event.btof);
		slice_count[event.btof]++;
		slice_raw_moment1[event.btof]+=event.ref;
		slice_raw_moment2[event.btof]+=pow(event.ref, 2);
		int slice_index = event.btof / orig_slice_height;
		if(orig_slices.count(slice_index) == 0) {
			string title = to_string(energy) + "GeV Original Slice BTOFMatch " + to_string(slice_index * orig_slice_height) + " to " + to_string((slice_index + 1) * orig_slice_height);
			orig_slices[slice_index] = TH1I(title.data(), title.data(), orig_ref_bins, orig_ref_low, orig_ref_high);
			orig_slices[slice_index].GetXaxis()->SetTitle("Refmult");
		}
		orig_slices[slice_index].Fill(event.ref);
	}

	TF1 lin_fit(("lin_fit_"+name).data(), "[0]*x", orig_ref_low, orig_ref_high);
	btof_ref_hist.Fit(&lin_fit, "NQ");
	double rot_angle = M_PI / 2 - atan(lin_fit.GetParameter(0));

	cout << rot_angle * 180 / M_PI << endl;

	double x_low = orig_ref_low * cos(rot_angle) - orig_btof_high * sin(rot_angle);
	double x_high = orig_ref_high * cos(rot_angle) - orig_btof_low * sin(rot_angle);
	double y_low = orig_ref_low * sin(rot_angle) + orig_btof_low * cos(rot_angle);
	double y_high = orig_ref_high * sin(rot_angle) + orig_btof_high * cos(rot_angle);
	TH2F rotate_btof_ref_hist(("rotate_"+name).data(), ("Rotated "+title).data(), (int)(x_high-x_low+1), x_low, x_high, (int)(y_high-y_low+1), y_low, y_high);
	rotate_btof_ref_hist.GetXaxis()->SetTitle("Rotated BTOFMatch vs Refmult X");
	rotate_btof_ref_hist.GetYaxis()->SetTitle("Rotated BTOFMatch vs Refmult Y");

	map<int, TH1F> slices;
	map<int, TH2F> dca_z_slices;
	map<int, TH2F> dca_z_slices_sd;
	map<int, double> slice_y_sum;
	map<int, map<float, int>> slice_map;
	map<int, int> slice_sum;

	for(PUEvent const &event:pu_events) {
		pair<double, double> x_y_rotate = rotate_xy((double)event.ref, (double)event.btof, rot_angle);
		rotate_btof_ref_hist.Fill(x_y_rotate.first, x_y_rotate.second);
		int slice_index = x_y_rotate.second / rot_slice_height;
		if(slices.count(slice_index) == 0) {
			string title = to_string(energy) + "GeV Slice " + to_string(slice_index * rot_slice_height) + " to " + to_string((slice_index + 1) * rot_slice_height);
			slices[slice_index] = TH1F(title.data(), title.data(), (int)(x_high-x_low+1), x_low, x_high);
			slices[slice_index].GetXaxis()->SetTitle("Rotated BTOFMatch vs Refmult X");
			dca_z_slices[slice_index] = TH2F((title+"_dca_z").data(), (title+"_dca_z").data(), (int)(x_high-x_low+1), x_low, x_high, 201, -2, 2);
			dca_z_slices[slice_index].GetXaxis()->SetTitle("Rotated BTOFMatch vs Refmult X");
			dca_z_slices[slice_index].GetYaxis()->SetTitle("Proton&Pion Event Dca_z Average");
			dca_z_slices_sd[slice_index] = TH2F((title+"_dca_z_sd").data(), (title+"_dca_z_sd").data(), (int)(x_high-x_low+1), x_low, x_high, 201, 0, 2);
			dca_z_slices_sd[slice_index].GetXaxis()->SetTitle("Rotated BTOFMatch vs Refmult X");
			dca_z_slices_sd[slice_index].GetYaxis()->SetTitle("Proton&Pion Event Dca_z Standard Deviation");
		}
		slices[slice_index].Fill(x_y_rotate.first);
		slice_y_sum[slice_index] += x_y_rotate.second;
		dca_z_slices[slice_index].Fill(x_y_rotate.first, event.dca_z_avg);
		dca_z_slices_sd[slice_index].Fill(x_y_rotate.first, event.dca_z_sd);
		slice_map[slice_index][x_y_rotate.first]++;
		slice_sum[slice_index]++;
	}

	map<int, TF1> slice_fits;
	map<int, TF1> slice_fits2;
	vector<double> y_upper;
	vector<double> y_lower;
	vector<double> upper_points;
	vector<double> lower_points;
	vector<int> erase_list;
	for(pair<int, TH1F> slice:slices) {
		int filled_bins = 0;
		for(int bin = 0; bin<slice.second.GetNbinsX(); bin++) {
			if(slice.second.GetBinContent(bin) > 0) { filled_bins++; }
		}
		if(slice.second.GetEntries() < min_points || filled_bins < min_bins) { erase_list.push_back(slice.first); }
		else {
			string title = to_string(energy) + "GeV Slice Fit " + to_string(slice.first * rot_slice_height) + " to " + to_string((slice.first + 1) *rot_slice_height);
			double lower_bound = slice.second.GetBinCenter(slice.second.FindFirstBinAbove());
			double upper_bound = slice.second.GetBinCenter(slice.second.FindLastBinAbove());
			TF1 gaus_init(("gaus_init_"+title).data(), "gaus", lower_bound, upper_bound);
			slice.second.Fit(&gaus_init, "NQR");
			gaus_init.SetRange(lower_bound, gaus_init.GetParameter(1));
			slice.second.Fit(&gaus_init, "NQR");
//			TF1 cb_init(("cb_init_"+title).data(), "crystalball", lower_bound, upper_bound);
//			cb_init.SetParameters(gaus_init.GetParameter(0), gaus_init.GetParameter(1), gaus_init.GetParameter(2), -2, 1);
//			slice_fits[slice.first] = TF1(title.data(), "crystalball(0)+[5]/(1+exp(-[6]*(x-[7])))", lower_bound, upper_bound);
//			slice_fits[slice.first].SetParameters(gaus_init.GetParameter(0), gaus_init.GetParameter(1), gaus_init.GetParameter(2), -5, 0.5, gaus_init.GetParameter(0) / 1000, 5, gaus_init.GetParameter(1) + gaus_init.GetParameter(2));
//			slice_fits[slice.first].SetParLimits(0, 0, slice.second.GetMaximum() * 2);
//			slice_fits[slice.first].SetParLimits(1, lower_bound, upper_bound);
//			slice_fits[slice.first].SetParLimits(3, -20, 0);  // Not sure
//			slice_fits[slice.first].SetParLimits(4, 0, 20);  // Not sure
//			slice_fits[slice.first].SetParLimits(5, 0.2, gaus_init.GetParameter(0) / 10);
//			slice_fits[slice.first].SetParLimits(6, 0.1, 100);
//			slice_fits[slice.first].SetParLimits(7, lower_bound, upper_bound);
//			slice.second.Fit(&slice_fits[slice.first], "NQR");

			TF1 gaus_exp(("gaus_exp_"+title).data(), gaus_exp_r, lower_bound, upper_bound, 4);
			gaus_exp.SetParameters(gaus_init.GetParameter(0), gaus_init.GetParameter(1), gaus_init.GetParameter(2), -1);
			slice.second.Fit(&gaus_exp, "NQR");
			slice_fits[slice.first] = TF1(title.data(), gexp_r_plus_ws, lower_bound, upper_bound, 7);
			slice_fits[slice.first].SetParameters(gaus_exp.GetParameter(0), gaus_exp.GetParameter(1), gaus_exp.GetParameter(2), gaus_exp.GetParameter(3), 0.3, 5, gaus_init.GetParameter(1) + gaus_init.GetParameter(2));
			slice_fits[slice.first].SetParLimits(0, 0, slice.second.GetMaximum() * 2);
			slice_fits[slice.first].SetParLimits(1, lower_bound, upper_bound);
			slice_fits[slice.first].SetParLimits(2, gaus_init.GetParameter(2) / 4, gaus_init.GetParameter(2) * 4);
//			slice_fits[slice.first].SetParLimits(3, -1000, -0.1);  // Not sure
			slice_fits[slice.first].SetParLimits(4, 0.2, gaus_init.GetParameter(0) / 10);
			slice_fits[slice.first].SetParLimits(5, 0.1, 100);
			slice_fits[slice.first].SetParLimits(6, lower_bound, upper_bound);
			slice.second.Fit(&slice_fits[slice.first], "NQR");

			slice_fits2[slice.first] = TF1(title.data(), "gaus(0)+([6]*exp(-[7]*(x-[8]))+[3])/(1+exp(-[4]*(x-[5])))", lower_bound, upper_bound);
			slice_fits2[slice.first].SetParameters(gaus_init.GetParameter(0), gaus_init.GetParameter(1), gaus_init.GetParameter(2), 0.21, 5, gaus_init.GetParameter(1) + gaus_init.GetParameter(2), 1, 5, gaus_init.GetParameter(1) + gaus_init.GetParameter(2));
			slice_fits2[slice.first].SetParLimits(0, 0, slice.second.GetMaximum() * 2);
			slice_fits2[slice.first].SetParLimits(1, lower_bound, upper_bound);
			slice_fits2[slice.first].SetParLimits(3, 0.2, gaus_init.GetParameter(0) / 10);
			slice_fits2[slice.first].SetParLimits(4, 0, 1000);
			slice_fits2[slice.first].SetParLimits(5, lower_bound, upper_bound);
			slice_fits2[slice.first].SetParLimits(6, 0, gaus_init.GetParameter(0) / 10);
			slice_fits2[slice.first].SetParLimits(7, 0.1, 100);
			slice_fits2[slice.first].SetParLimits(8, lower_bound, upper_bound);
			slice.second.Fit(&slice_fits2[slice.first], "NQR");

			double range = upper_bound - lower_bound;
			slice_fits[slice.first].SetRange(lower_bound - 0.5*range, upper_bound + 0.1*range);
			slice_fits2[slice.first].SetRange(lower_bound - 0.5*range, upper_bound + 0.1*range);

			double y_point = slice_y_sum[slice.first] / slice.second.GetEntries();
			double up_point = slice_fits[slice.first].GetParameter(1) - sigmas_left * slice_fits[slice.first].GetParameter(2);
			double low_point = slice_fits[slice.first].GetParameter(1) + sigmas_right * slice_fits[slice.first].GetParameter(2);
			pair<double, double> up_rotate = rotate_xy(up_point, y_point, -rot_angle);
			pair<double, double> low_rotate = rotate_xy(low_point, y_point, -rot_angle);
			y_upper.push_back(up_rotate.second);
			upper_points.push_back(up_rotate.first);
			y_lower.push_back(low_rotate.second);
			lower_points.push_back(low_rotate.first);
		}
	}

	for(int slice_index:erase_list) { slices.erase(slice_index); }

	map<int, TF1> slice_init_gaus;
	map<int, TH1F> slice_minus_gaus;
	for(pair<int, TH1F> slice:slices) {
		string title = to_string(energy) + "GeV Slice Left Side Gauss Fit " + to_string(slice.first * rot_slice_height) + " to " + to_string((slice.first + 1) *rot_slice_height);
		double lower_bound = slice.second.GetBinCenter(slice.second.FindFirstBinAbove());
		double upper_bound = slice.second.GetBinCenter(slice.second.FindLastBinAbove());
		slice_init_gaus[slice.first] = TF1(title.data(), "gaus", lower_bound, upper_bound);
		slice.second.Fit(&slice_init_gaus[slice.first], "NQR");
		upper_bound = slice_init_gaus[slice.first].GetParameter(1);
		slice_init_gaus[slice.first].SetRange(lower_bound, upper_bound);
		slice.second.Fit(&slice_init_gaus[slice.first], "NQR");
		slice_minus_gaus[slice.first] = slice.second;
		slice_minus_gaus[slice.first].Add(&slice_init_gaus[slice.first], -1);
	}

	TGraph upper((int)y_upper.size(), upper_points.data(), y_upper.data());
	upper.SetLineColor(kGreen); upper.SetLineWidth(2);
	TGraph lower((int)y_lower.size(), lower_points.data(), y_lower.data());
	lower.SetLineColor(kGreen); lower.SetLineWidth(2);
	TF1 upper_fit(("upper_fit_"+name).data(), "pol4", orig_ref_low, orig_ref_high);
	upper_fit.SetLineColor(kRed);
	TF1 lower_fit(("lower_fit_"+name).data(), "pol4", orig_ref_low, orig_ref_high);
	lower_fit.SetLineColor(kRed);
	upper.Fit(&upper_fit, "NQ");
	lower.Fit(&lower_fit, "NQ");

	cout << energy << "GeV: " << endl;
	low_cut.clear(); high_cut.clear();
	for(int i=0; i<=4; i++) {
		cout << "x^" << i << ":  Upper: " << upper_fit.GetParameter(i) << "  |  Lower: " << lower_fit.GetParameter(i) << endl;
		low_cut.push_back(lower_fit.GetParameter(i));
		high_cut.push_back(upper_fit.GetParameter(i));
	}
//	cout << energy << "GeV\t Upper: " << upper_fit.GetParameter(0) << "+" << upper_fit.GetParameter(1) << "x  |  Lower: " << lower_fit.GetParameter(0) << "+" << lower_fit.GetParameter(1) << "x" << endl;
//	low_cut = make_pair(lower_fit.GetParameter(0), lower_fit.GetParameter(1));
//	high_cut = make_pair(upper_fit.GetParameter(0), upper_fit.GetParameter(1));

	map<int, vector<double>> slice_prob_vec;
	map<int, vector<double>> slice_err_vec;
	map<int, vector<double>> slice_x_vec;
	for(pair<int, map<float, int>> slice:slice_map) {
		for(pair<float, int> point:slice.second) {
			slice_x_vec[slice.first].push_back(point.first);
			double prob = (double)point.second / slice_sum[slice.first];
			slice_prob_vec[slice.first].push_back(prob);
			slice_err_vec[slice.first].push_back(sqrt(prob * (1 - prob) / slice_sum[slice.first]));
		}
	}
	map<int, TGraphErrors> prob_graphs;
	for(pair<int, vector<double>> slice:slice_x_vec) {
		prob_graphs[slice.first] = TGraphErrors((int)slice.second.size(), slice.second.data(), slice_prob_vec[slice.first].data(), 0, slice_err_vec[slice.first].data());
		string title = to_string(energy) + "GeV Prob Slice " + to_string(slice.first * rot_slice_height) + " to " + to_string((slice.first + 1) * rot_slice_height);
		prob_graphs[slice.first].SetNameTitle(title.data(), title.data());
		prob_graphs[slice.first].GetYaxis()->SetRangeUser(0.9 * (double)*min_element(slice_prob_vec[slice.first].begin(), slice_prob_vec[slice.first].end()),
				1.1 * 0.9 * (double)*max_element(slice_prob_vec[slice.first].begin(), slice_prob_vec[slice.first].end()));
		prob_graphs[slice.first].SetMarkerStyle(4);
	}

	if(mtx) { mtx->lock(); }
	{
		TCanvas orig_can(("Can_"+name).data(), title.data(), can_x_pix, can_y_pix);
		btof_ref_hist.Draw("COLZ");
		lin_fit.Draw("same");
		orig_can.SetLogz();
		orig_can.Update();
		orig_can.Write();

		TCanvas cut_can(("Cut_Can_"+name).data(), title.data(), can_x_pix, can_y_pix);
		btof_ref_hist.Draw("COLZ");
		lin_fit.Draw("same");
		upper.Draw("sameL");
		lower.Draw("sameL");
		upper_fit.Draw("same");
		lower_fit.Draw("same");
		cut_can.SetLogz();
		cut_can.Update();
		cut_can.Write();

		TCanvas rot_can(("Rot_Can_"+name).data(), ("Rotated "+title).data(), can_x_pix, can_y_pix);
		rotate_btof_ref_hist.Draw("COLZ");
		rot_can.SetLogz();
		rot_can.Update();
		rot_can.Write();

		TDirectory *slice_orig_dir = out_file->mkdir("Slice Original Canvases");
		slice_orig_dir->cd();
		for(pair<int, TH1I> slice:orig_slices) {
			TCanvas slice_orig_can(("Can_"+(string)slice.second.GetName()).data(), slice.second.GetTitle(), can_x_pix, can_y_pix);
			slice.second.SetAxisRange(0.9 * slice.second.GetBinCenter(slice.second.FindFirstBinAbove()), 1.1 * slice.second.GetBinCenter(slice.second.FindLastBinAbove()));
			slice.second.Draw();
			slice_orig_can.SetLogy();
			slice_orig_can.Write();
		}

		TDirectory *slice_canvas_dir = out_file->mkdir("Slice Canvases");
		TDirectory *slice2_canvas_dir = out_file->mkdir("Slice2 Canvases");
		TDirectory *slice_gaus_dir = out_file->mkdir("Slice Guas Canvases");
		TDirectory *slice_gaus_sub_dir = out_file->mkdir("Slice Gaus Sub Canvases");
		TDirectory *slice_prob_dir = out_file->mkdir("Slice Prob Canvases");
		TDirectory *slice_dcaz_dir = out_file->mkdir("Dca_z Slice Canvases");
		TDirectory *slice_dcaz_sd_dir = out_file->mkdir("Dca_z_sd Slice Canvases");
		for(pair<int, TH1F> slice:slices) {
			slice_canvas_dir->cd();
			TCanvas slice_can(("Can_"+(string)slice.second.GetName()).data(), slice.second.GetTitle(), can_x_pix, can_y_pix);

			slice.second.SetAxisRange(slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			slice_fits[slice.first].SetNpx(n_plot_points);
//			TF1 cball("CBall", "crystalball", slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
//			cball.SetParameters(slice_fits[slice.first].GetParameter(0), slice_fits[slice.first].GetParameter(1), slice_fits[slice.first].GetParameter(2), slice_fits[slice.first].GetParameter(3), slice_fits[slice.first].GetParameter(4));
//			cball.SetNpx(n_plot_points); cball.SetLineColor(kGreen+2);
			TF1 gaus_exp("gaus_exp", gaus_exp_r, slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax(), 4);
			gaus_exp.SetParameters(slice_fits[slice.first].GetParameter(0), slice_fits[slice.first].GetParameter(1), slice_fits[slice.first].GetParameter(2), slice_fits[slice.first].GetParameter(3));
			gaus_exp.SetNpx(n_plot_points); gaus_exp.SetLineColor(kGreen+2);
			TF1 bkg("Background", "[0]/(1+exp(-[1]*(x-[2])))", slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			bkg.SetParameters(slice_fits[slice.first].GetParameter(4), slice_fits[slice.first].GetParameter(5), slice_fits[slice.first].GetParameter(6));
			bkg.SetNpx(n_plot_points); bkg.SetLineColor(kViolet);

			slice.second.Draw();
			gaus_exp.Draw("Sames");
			bkg.Draw("Sames");
			slice_fits[slice.first].Draw("Sames");

			TPaveText gaus_exp_pars(0.12, 0.65, 0.34, 0.9, "NDC");
			gaus_exp_pars.AddText("Gaus_Exp Parameters");
			gaus_exp_pars.AddLine(.0, .8, 1., .8);
			gaus_exp_pars.AddText(("Amplitude: " + to_string(gaus_exp.GetParameter(0))).data());
			gaus_exp_pars.AddText(("Mean: " + to_string(gaus_exp.GetParameter(1))).data());
			gaus_exp_pars.AddText(("Sigma: " + to_string(gaus_exp.GetParameter(2))).data());
			gaus_exp_pars.AddText(("k: " + to_string(gaus_exp.GetParameter(3))).data());
			gaus_exp_pars.Draw("Same");
			TPaveText bkg_pars(0.12, 0.44, 0.34, 0.64, "NDC");
			bkg_pars.AddText("Woods Saxon Parameters");
			bkg_pars.AddLine(.0, .75, 1., .75);
			bkg_pars.AddText(("Amplitude: " + to_string(bkg.GetParameter(0))).data());
			bkg_pars.AddText(("Center: " + to_string(bkg.GetParameter(2))).data());
			bkg_pars.AddText(("Width: " + to_string(bkg.GetParameter(1))).data());
			bkg_pars.Draw("Same");
			TLegend leg(0.35, 0.2);
			leg.AddEntry(&slice.second, "Slice Distribution", "l");
			leg.AddEntry(&slice_fits[slice.first], "Full Fit", "l");
			leg.AddEntry(&gaus_exp, "Gaus_Exp Contribution", "l");
			leg.AddEntry(&bkg, "Woods Saxon Contribution", "l");
			leg.Draw();

			slice_can.SetLogy();
			slice_can.Write();

			slice2_canvas_dir->cd();
			TCanvas slice2_can(("Can2_"+(string)slice.second.GetName()).data(), slice.second.GetTitle(), can_x_pix, can_y_pix);

			slice.second.SetAxisRange(slice_fits2[slice.first].GetXmin(), slice_fits2[slice.first].GetXmax());
			slice_fits2[slice.first].SetNpx(n_plot_points);
			TF1 gaus("Gaus", "gaus", slice_fits2[slice.first].GetXmin(), slice_fits2[slice.first].GetXmax());
			gaus.SetParameters(slice_fits2[slice.first].GetParameter(0), slice_fits2[slice.first].GetParameter(1), slice_fits2[slice.first].GetParameter(2));
			gaus.SetNpx(n_plot_points); gaus.SetLineColor(kGreen+2);
			TF1 bkg2("Background", "([3]*exp(-[4]*(x-[5]))+[0])/(1+exp(-[1]*(x-[2])))", slice_fits2[slice.first].GetXmin(), slice_fits2[slice.first].GetXmax());
			bkg2.SetParameters(slice_fits2[slice.first].GetParameter(3), slice_fits2[slice.first].GetParameter(4), slice_fits2[slice.first].GetParameter(5), slice_fits2[slice.first].GetParameter(6), slice_fits2[slice.first].GetParameter(7), slice_fits2[slice.first].GetParameter(8));
			bkg2.SetNpx(n_plot_points); bkg2.SetLineColor(kViolet);

			slice.second.Draw();
			gaus.Draw("Sames");
			bkg2.Draw("Sames");
			slice_fits2[slice.first].Draw("Sames");

			TLegend leg2(0.35, 0.2);
			leg2.AddEntry(&slice.second, "Slice Distribution", "l");
			leg2.AddEntry(&slice_fits2[slice.first], "Full Fit", "l");
			leg2.AddEntry(&gaus, "Gaussian Contribution", "l");
			leg2.AddEntry(&bkg2, "(Const+Exp)*Woods Saxon Contribution", "l");
			leg2.Draw();
			TPaveText gaus_pars(0.6, 0.35, 0.95, 0.5, "NDC");
			gaus_pars.AddText("Gaus Parameters");
			gaus_pars.AddLine(.0, .75, 1., .75);
			gaus_pars.AddText(("Amplitude: " + to_string(gaus.GetParameter(0))).data());
			gaus_pars.AddText(("Mean: " + to_string(gaus.GetParameter(1))).data());
			gaus_pars.AddText(("Sigma: " + to_string(gaus.GetParameter(2))).data());
			gaus_pars.Draw("Same");
			TPaveText bkg2_pars(0.6, 0.5, 0.95, 0.75, "NDC");
			bkg2_pars.AddText("(Const+Exp)*Woods Saxon Parameters");
			bkg2_pars.AddLine(.0, .86, 1., .86);
			bkg2_pars.AddText(("Const Amplitude: " + to_string(bkg2.GetParameter(0))).data());
			bkg2_pars.AddText(("WS Center: " + to_string(bkg2.GetParameter(2))).data());
			bkg2_pars.AddText(("WS Width: " + to_string(bkg2.GetParameter(1))).data());
			bkg2_pars.AddText(("Exp Amplitude: " + to_string(bkg2.GetParameter(3))).data());
			bkg2_pars.AddText(("Exp Width: " + to_string(bkg2.GetParameter(4))).data());
			bkg2_pars.AddText(("Exp Center: " + to_string(bkg2.GetParameter(5))).data());
			bkg2_pars.Draw("Same");
			slice2_can.SetLogy();
			slice2_can.Write();

			slice_prob_dir->cd();
			TCanvas slice_prob_can(("Can_"+(string)prob_graphs[slice.first].GetName()).data(), prob_graphs[slice.first].GetTitle(), can_x_pix, can_y_pix);
			prob_graphs[slice.first].GetXaxis()->SetRangeUser(slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			prob_graphs[slice.first].Draw("apz");
			slice_prob_can.SetLogy();
			slice_prob_can.Write();

			slice_gaus_dir->cd();
			TCanvas slice_gaus_can(("Can_Gaus_"+(string)slice.second.GetName()).data(), slice.second.GetTitle(), can_x_pix, can_y_pix);
			slice.second.Draw();
			slice_init_gaus[slice.first].SetNpx(n_plot_points);
			slice_init_gaus[slice.first].SetRange(slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			slice_init_gaus[slice.first].Draw("same");
			slice_gaus_can.SetLogy();
			slice_gaus_can.Write();

			slice_gaus_sub_dir->cd();
			TCanvas slice_gaus_sub_can(("Can_Gaus_Sub"+(string)slice.second.GetName()).data(), slice.second.GetTitle(), can_x_pix, can_y_pix);
			slice_minus_gaus[slice.first].SetAxisRange(slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			slice_minus_gaus[slice.first].Draw();
			slice_gaus_sub_can.SetLogy();
			slice_gaus_sub_can.Write();

			slice_dcaz_dir->cd();
			TCanvas slice_dcaz_can(("Can_"+(string)dca_z_slices[slice.first].GetName()).data(), dca_z_slices[slice.first].GetTitle(), can_x_pix, can_y_pix);
			dca_z_slices[slice.first].GetXaxis()->SetRangeUser(slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			dca_z_slices[slice.first].Draw("Colz");
			slice_dcaz_can.SetLogz();
			slice_dcaz_can.Write();

			slice_dcaz_sd_dir->cd();
			TCanvas slice_dcaz_sd_can(("Can_"+(string)dca_z_slices_sd[slice.first].GetName()).data(), dca_z_slices_sd[slice.first].GetTitle(), can_x_pix, can_y_pix);
			dca_z_slices_sd[slice.first].GetXaxis()->SetRangeUser(slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			dca_z_slices_sd[slice.first].Draw("Colz");
			slice_dcaz_sd_can.SetLogz();
			slice_dcaz_sd_can.Write();
		}
	}
	if(mtx) { mtx->unlock(); }

	out_file->Close();
}



void PileUpQAer::write_cut_file() {
	if(!check_path(out_path)) { cout << "Can't access path: " << out_path << " Skipping following read/write." << endl; return; }

	string out_name = out_path + to_string(energy) + "GeV" + out_file_suf;
	ofstream out_txt(out_name);
	if(!out_txt.is_open()) { cout << "Could not open " << out_name << " Not writing." << endl; return; }
	out_txt << energy << "GeV Pile Up cuts with curves of gaus fit -" << sigmas_left << " +" << sigmas_right << " sigmas for each rotated slice of " << rot_slice_height << " rotated units with more than " << min_points << " points: " << endl;

	out_txt << "sigmas_left: " << sigmas_left << endl;
	out_txt << "sigmas_right: " << sigmas_right << endl;
	out_txt << "min_points: " << min_points << endl;
	out_txt << "min_bins: " << min_bins << endl;
	out_txt << "min_btof: " << min_btof << endl;
	out_txt << "rot_slice_height: " << rot_slice_height << endl;
	out_txt << "orig_slice_height: " << orig_slice_height << endl;
	out_txt << endl;

	out_txt << scientific;
	out_txt << "High cut curve (coefficients):\t";
	for(unsigned coef=0; coef < high_cut.size(); coef++) {
		if(coef == high_cut.size() - 1) {
			out_txt << high_cut[coef] << endl;
		} else {
			out_txt << high_cut[coef] << ",";
		}
	}
	out_txt << "Low cut curve (coefficients):\t";
	for(unsigned coef=0; coef < low_cut.size(); coef++) {
		if(coef == low_cut.size() - 1) {
			out_txt << low_cut[coef] << endl;
		} else {
			out_txt << low_cut[coef] << ",";
		}
	}

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
		if(line_num == 10) {
			vector<string> highs = split(split(line, '\t')[1], ',');
			for(string coef:highs) {
				high_cut.push_back(stof(coef));
			}
		}
		if(line_num == 11) {
			vector<string> lows = split(split(line, '\t')[1], ',');
			for(string coef:lows) {
				low_cut.push_back(stof(coef));
			}
		}
		line_num++;
	}

	in_txt.close();
}


pair<double, double> rotate_xy(double x, double y, double angle) {
	double x_rot = x * cos(angle) - y * sin(angle);
	double y_rot = x * sin(angle) + y * cos(angle);

	return make_pair(x_rot, y_rot);
}
