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
	vector<double> y_upper;
	vector<double> y_lower;
	vector<double> upper_points;
	vector<double> lower_points;
	vector<int> erase_list;
	for(pair<int, TH1F> slice:slices) {
		if(slice.second.GetEntries() < min_points) { erase_list.push_back(slice.first); }
		else {
			string title = to_string(energy) + "GeV Slice Fit " + to_string(slice.first * rot_slice_height) + " to " + to_string((slice.first + 1) *rot_slice_height);
			double lower_bound = slice.second.GetBinCenter(slice.second.FindFirstBinAbove());
			double upper_bound = slice.second.GetBinCenter(slice.second.FindLastBinAbove());
			slice_fits[slice.first] = TF1(title.data(), "gaus(0)+[3]/(1+exp(-[4]*(x-[5])))", lower_bound, upper_bound);
//			slice_fits[slice.first] = TF1(title.data(), "gaus(0)+((x>[3])?[4]:0)", lower_bound, upper_bound);
//			slice_fits[slice.first].SetParameters(slice.second.GetMaximum() / 1000, slice.second.GetStdDev(), slice.second.GetMean()+3*slice.second.GetStdDev(), slice.second.GetMaximum(), slice.second.GetMean(), slice.second.GetStdDev());
//			slice_fits[slice.first].SetParameters(slice.second.GetMaximum(), slice.second.GetMean(), slice.second.GetStdDev(), slice.second.GetMean() + 3*slice.second.GetStdDev(), 0);
//			slice_fits[slice.first].SetParLimits(0, 0, slice.second.GetMaximum() / 100);
//			slice_fits[slice.first].SetParLimits(2, slice.second.GetMean()-1*slice.second.GetStdDev(), slice.second.GetMean()+3*slice.second.GetStdDev());
			slice_fits[slice.first].SetParameters(slice.second.GetMaximum(), slice.second.GetMean(), slice.second.GetStdDev(), 0, 1, slice.second.GetMean() + 0.2*(upper_bound - lower_bound));
//			slice_fits[slice.first].SetParLimits(0, slice.second.GetMaximum() / 3, slice.second.GetMaximum() * 3);
//			slice_fits[slice.first].SetParLimits(1, slice.second.GetMean()-1*slice.second.GetStdDev(), slice.second.GetMean()+1*slice.second.GetStdDev());
//			slice_fits[slice.first].SetParLimits(2, slice.second.GetStdDev() / 10, slice.second.GetStdDev() * 2);
//			slice_fits[slice.first].SetParLimits(3, slice.second.GetMean()-1*slice.second.GetStdDev(), slice.second.GetMean()+3*slice.second.GetStdDev());
//			slice_fits[slice.first].SetParLimits(4, 0, slice.second.GetMaximum() / 100);
			slice_fits[slice.first].SetParLimits(3, 0, slice.second.GetMaximum() / 100);
//			slice_fits[slice.first].SetParLimits(4, 0, 1000);
			slice_fits[slice.first].SetParLimits(5, lower_bound, upper_bound);
			slice.second.Fit(&slice_fits[slice.first], "NQR");
			double range = upper_bound - lower_bound;
			slice_fits[slice.first].SetRange(lower_bound - 0.2*range, upper_bound + 0.2*range);

			double y_point = slice_y_sum[slice.first] / slice.second.GetEntries();
			double up_point = slice_fits[slice.first].GetParameter(1) - sigmas * slice_fits[slice.first].GetParameter(2);
			double low_point = slice_fits[slice.first].GetParameter(1) + sigmas * slice_fits[slice.first].GetParameter(2);
			pair<double, double> up_rotate = rotate_xy(up_point, y_point, -rot_angle);
			pair<double, double> low_rotate = rotate_xy(low_point, y_point, -rot_angle);
			y_upper.push_back(up_rotate.second);
			upper_points.push_back(up_rotate.first);
			y_lower.push_back(low_rotate.second);
			lower_points.push_back(low_rotate.first);
		}
	}

	for(int slice_index:erase_list) { slices.erase(slice_index); }

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
		TCanvas orig_can(("Can_"+name).data(), title.data());
		btof_ref_hist.Draw("COLZ");
		lin_fit.Draw("same");
		orig_can.SetLogz();
		orig_can.Update();
		orig_can.Write();

		TCanvas cut_can(("Cut_Can_"+name).data(), title.data());
		btof_ref_hist.Draw("COLZ");
		lin_fit.Draw("same");
		upper.Draw("sameL");
		lower.Draw("sameL");
		upper_fit.Draw("same");
		lower_fit.Draw("same");
		cut_can.SetLogz();
		cut_can.Update();
		cut_can.Write();

		TCanvas rot_can(("Rot_Can_"+name).data(), ("Rotated "+title).data());
		rotate_btof_ref_hist.Draw("COLZ");
		rot_can.SetLogz();
		rot_can.Update();
		rot_can.Write();

		TDirectory *slice_orig_dir = out_file->mkdir("Slice Original Canvases");
		slice_orig_dir->cd();
		for(pair<int, TH1I> slice:orig_slices) {
			TCanvas slice_orig_can(("Can_"+(string)slice.second.GetName()).data(), slice.second.GetTitle());
			slice.second.SetAxisRange(0.9 * slice.second.GetBinCenter(slice.second.FindFirstBinAbove()), 1.1 * slice.second.GetBinCenter(slice.second.FindLastBinAbove()));
			slice.second.Draw();
			slice_orig_can.SetLogy();
			slice_orig_can.Write();
		}

		TDirectory *slice_canvas_dir = out_file->mkdir("Slice Canvases");
		TDirectory *slice_prob_dir = out_file->mkdir("Slice Prob Canvases");
		TDirectory *slice_dcaz_dir = out_file->mkdir("Dca_z Slice Canvases");
		TDirectory *slice_dcaz_sd_dir = out_file->mkdir("Dca_z_sd Slice Canvases");
		for(pair<int, TH1F> slice:slices) {
			slice_canvas_dir->cd();
			TCanvas slice_can(("Can_"+(string)slice.second.GetName()).data(), slice.second.GetTitle());

			slice.second.SetAxisRange(slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			slice_fits[slice.first].SetNpx(n_plot_points);
			TF1 gaus("Gaussian", "gaus", slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			gaus.SetParameters(slice_fits[slice.first].GetParameter(0), slice_fits[slice.first].GetParameter(1), slice_fits[slice.first].GetParameter(2));
			gaus.SetNpx(n_plot_points); gaus.SetLineColor(kGreen+2);
			TF1 bkg("Background", "[0]/(1+exp(-[1]*(x-[2])))", slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			bkg.SetParameters(slice_fits[slice.first].GetParameter(3), slice_fits[slice.first].GetParameter(4), slice_fits[slice.first].GetParameter(5));
//			TF1 bkg("Background", "((x>[3])?[4]:0)", slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
//			bkg.SetParameters(slice_fits[slice.first].GetParameter(3), slice_fits[slice.first].GetParameter(4));
			bkg.SetNpx(n_plot_points); bkg.SetLineColor(kViolet);

			slice.second.Draw();
			gaus.Draw("Sames");
			bkg.Draw("Sames");
			slice_fits[slice.first].Draw("Sames");

			TLegend leg(0.35, 0.2);
			leg.AddEntry(&slice.second, "Slice Distribution", "l");
			leg.AddEntry(&slice_fits[slice.first], "Full Fit", "l");
			leg.AddEntry(&gaus, "Gaussian Contribution", "l");
			leg.AddEntry(&bkg, "Step Function Contribution", "l");
//			leg.AddEntry(&bkg, "#frac{A}{1+Exp[-b*(x-x_{0})]} Contribution", "l");
			leg.Draw();
//			TPaveText gaus_pars;
//			gaus_pars.AddText("Gaussian Parameters");
//			gaus_pars.AddLine(.0, .5, 1., .5);
//			gaus_pars.AddText((to_string(gaus.GetParameter(0)) + "#{e}^{-#frac{1}{2}{(#frac{x-" + to_string(gaus.GetParameter(1)) + "}{" + to_string(gaus.GetParameter(2)) + "})}^{2}").data());
//			gaus_pars.AddText(("Amplitude: " + to_string(gaus.GetParameter(0))).data());
//			gaus_pars.AddText(("Mean: " + to_string(gaus.GetParameter(1))).data());
//			gaus_pars.AddText(("Sigma: " + to_string(gaus.GetParameter(2))).data());
//			gaus_pars.Draw("Same");
//			TPaveText erf_pars;
//			erf_pars.AddText("Error Function Parameters");
//			erf_pars.AddLine(.0, .25, 1., .25);
//			erf_pars.AddText(("Amplitude: " + to_string(erf.GetParameter(0))).data());
//			erf_pars.AddText(("Center: " + to_string(erf.GetParameter(1))).data());
//			erf_pars.AddText(("Width: " + to_string(erf.GetParameter(2))).data());
//			erf_pars.Draw();
			slice_can.SetLogy();
			slice_can.Write();

			slice_prob_dir->cd();
			TCanvas slice_prob_can(("Can_"+(string)prob_graphs[slice.first].GetName()).data(), prob_graphs[slice.first].GetTitle());
			prob_graphs[slice.first].GetXaxis()->SetRangeUser(slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			prob_graphs[slice.first].Draw("apz");
			slice_prob_can.SetLogy();
			slice_prob_can.Write();

			slice_dcaz_dir->cd();
			TCanvas slice_dcaz_can(("Can_"+(string)dca_z_slices[slice.first].GetName()).data(), dca_z_slices[slice.first].GetTitle());
			dca_z_slices[slice.first].GetXaxis()->SetRangeUser(slice_fits[slice.first].GetXmin(), slice_fits[slice.first].GetXmax());
			dca_z_slices[slice.first].Draw("Colz");
			slice_dcaz_can.SetLogz();
			slice_dcaz_can.Write();

			slice_dcaz_sd_dir->cd();
			TCanvas slice_dcaz_sd_can(("Can_"+(string)dca_z_slices_sd[slice.first].GetName()).data(), dca_z_slices_sd[slice.first].GetTitle());
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
	out_txt << energy << "GeV Pile Up cuts with curves of gaus fit +-" << sigmas << " sigmas for each rotated slice of " << rot_slice_height << " rotated units with more than " << min_points << " points: " << endl;

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
		if(line_num == 2) {
			vector<string> highs = split(split(line, '\t')[1], ',');
			for(string coef:highs) {
				high_cut.push_back(stof(coef));
			}
//			high_cut.first = stof(highs[0]);
//			high_cut.second = stof(highs[1]);
		}
		if(line_num == 3) {
			vector<string> lows = split(split(line, '\t')[1], ',');
			for(string coef:lows) {
				low_cut.push_back(stof(coef));
			}
//			low_cut.first = stof(lows[0]);
//			low_cut.second = stof(lows[1]);
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
