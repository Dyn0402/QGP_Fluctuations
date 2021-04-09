/*
 * AmptCentralityMaker.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */


#include "AmptCentralityMaker.h"



// Structors

AmptCentralityMaker::AmptCentralityMaker() {
	energy = 0;
	min_bias_path = "";
	qa_path = "";
	set_mult_quantity("ref3");
}

AmptCentralityMaker::AmptCentralityMaker(string min_bias_path) {
	energy = 0;
	this->min_bias_path = min_bias_path;
	qa_path = "";  // "/home/dylan/Research/Ampt_Centralities/"
	set_mult_quantity("ref3");
}

AmptCentralityMaker::AmptCentralityMaker(string min_bias_path, string mult_quantity) {
	energy = 0;
	this->min_bias_path = min_bias_path;
	set_mult_quantity(mult_quantity);
}

AmptCentralityMaker::AmptCentralityMaker(string min_bias_path, string qa_path, string mult_quantity) {
	energy = 0;
	this->min_bias_path = min_bias_path;
	this->qa_path = qa_path;
	set_mult_quantity(mult_quantity);
}

AmptCentralityMaker::AmptCentralityMaker(int energy, string min_bias_path, string qa_path, string mult_quantity) {
	this->energy = energy;
	this->min_bias_path = min_bias_path;
	this->qa_path = qa_path;
	set_mult_quantity(mult_quantity);
}

AmptCentralityMaker::~AmptCentralityMaker() {
	//Nothing
}


// Getters

string AmptCentralityMaker::get_min_bias_path() {
	return min_bias_path;
}

string AmptCentralityMaker::get_mult_quantity() {
	return mult_quantity;
}

vector<int> AmptCentralityMaker::get_ref_bin20_edges() {
	return ref_bin_edges;
}

vector<int> AmptCentralityMaker::get_ref_bin9_edges() {
	vector<int> ref_bin9_edges;
	if((int)ref_bin_edges.size() == 0) { make_centrality(); }
	for(int bin = 0; bin <= 8; bin++) {
		if(bin <= 6) { ref_bin9_edges.push_back(ref_bin_edges[3 + bin * 2]); }
		else if(bin == 7) { ref_bin9_edges.push_back(ref_bin_edges[17]); }
		else if(bin == 8) { ref_bin9_edges.push_back(ref_bin_edges[18]); }
	}
	return ref_bin9_edges;
}

vector<int> AmptCentralityMaker::get_ref_bin16_edges() {
	vector<int> ref_bin16_edges;
	if((int)ref_bin_edges.size() == 0) { make_centrality(); }
	for(int bin = 0; bin <= 15; bin++) {
		ref_bin16_edges.push_back(ref_bin_edges[3 + bin]);
	}
	return ref_bin16_edges;
}

int AmptCentralityMaker::get_cent_bin20(int mult) {
	int cent_bin = -1;
	if(in_string(mult_quantity, "ref")) {

		if ((int)ref_bin_edges.size() == 0) { make_centrality(); }

		for(int edge:ref_bin_edges) {
			cent_bin++;
			if(mult < edge) break;
		}
		if(mult > ref_bin_edges.back()) { cent_bin++; }
	} else {
		cout << "Impact parameter binning not implemented" << endl;
	}

	return cent_bin;
}

int AmptCentralityMaker::get_cent_bin9(int mult) {
	int cent20_bin = get_cent_bin20(mult);

	int cent_bin = 0;
	switch (cent20_bin) {
	case 19 : cent_bin = 8; break;
	case 18 : cent_bin = 7; break;
	case 1 : cent_bin = -1; break;
	case 0 : cent_bin = -1; break;
	case -1 : cent_bin = -1; break;
	default : cent_bin = cent20_bin / 2 - 2; break;
	}

	return cent_bin;
}


// Setters

void AmptCentralityMaker::set_qa_path(string qa_path) {
	this->qa_path = qa_path;
}

void AmptCentralityMaker::set_min_bias_path(string min_bias_path) {
	this->min_bias_path = min_bias_path;
}

void AmptCentralityMaker::set_star_data_path(string star_data_path) {
	this->star_data_path = star_data_path;
}

void AmptCentralityMaker::set_mult_quantity(string quantity) {
	if(quantity == "ref2") {
		append_dist_val = bind(&AmptCentralityMaker::get_ref2, this);
		sort_and_bin = bind(&AmptCentralityMaker::sort_bin_ref, this);
		set_ref_num(2);
	} else if(quantity == "ref3") {
		append_dist_val = bind(&AmptCentralityMaker::get_ref3, this);
		sort_and_bin = bind(&AmptCentralityMaker::sort_bin_ref, this);
		set_ref_num(3);
	} else if(quantity == "b") {
		append_dist_val = bind(&AmptCentralityMaker::get_b, this);
		sort_and_bin = bind(&AmptCentralityMaker::sort_bin_b, this);
		set_ref_num(3);
	} else {
		cout << "Invalid multiplicty quantity to define centrality: " << quantity << " \t Options are: ref2, ref3, b" << endl;
		return;
	}
	mult_quantity = quantity;
}

void AmptCentralityMaker::set_ref_num(int ref) {
	ref_num = ref;
}


// Doers

void AmptCentralityMaker::run_b_opt(vector<int> energy_vec) {
	TMultiGraph mg;
	TLegend leg;
	float chi_max = 0;

	TFile out_file((qa_path+"Ampt_Cent_B_Opt.root").data(), "RECREATE");

	for(int energy:energy_vec) {
		float b_cut_step = b_cut_step_init;
		vector<float> chi_diff;
		vector<float> b;
		map<float, float> b_chi_map;

		cout << "Reading " << energy << "GeV Ampt min bias data..." << endl;
		get_imp_ref(energy);
		cout << "Reading " << energy << "GeV Star data..." << endl;
		get_star_ref(energy);

		b.push_back(b_min[energy]);
		cout << "Calculating chi^2 for " << energy << "GeV, b = " << b.back() << endl;
		chi_diff.push_back(get_chi_dof_diff(b.back(), energy));
		b_chi_map[b.back()] = chi_diff.back();

		while(fabs(b_cut_step) >= acc) {
			b.push_back(b.back() + b_cut_step);
			cout << "Calculating chi^2 for " << energy << "GeV, b = " << b.back() << endl;
			chi_diff.push_back(get_chi_dof_diff(b.back(), energy));
			b_chi_map[b.back()] = chi_diff.back();
			while(chi_diff.back() < chi_diff[(int)chi_diff.size() - 2]) {
				b.push_back(b.back() + b_cut_step);
				cout << "Calculating chi^2 for " << energy << "GeV, b = " << b.back() << endl;
				chi_diff.push_back(get_chi_dof_diff(b.back(), energy));
				b_chi_map[b.back()] = chi_diff.back();
			}
			b_cut_step /= -2;
		}
		opt_b_max[energy] = b[(int)b.size() - 2];
		if(chi_diff[(int)chi_diff.size() - 2] > chi_max) { chi_max = chi_diff[(int)chi_diff.size() - 2]; }

		chi_diff.clear(); b.clear();
		for(pair<float, float> b_chi:b_chi_map) {
			b.push_back(b_chi.first);
			chi_diff.push_back(b_chi.second);
		}
		cout << endl;

		out_file.cd();
		plot_ref_dist_comp(opt_b_max[energy], energy);

		TGraph *graph = new TGraph((int)b.size(), b.data(), chi_diff.data());
		graph->SetMarkerColor(energy_color[energy]);
		graph->SetLineColor(energy_color[energy]);
		graph->SetLineWidth(3);
		graph->SetMarkerStyle(8);
		mg.Add(graph, "l");
		leg.AddEntry(graph, (to_string(energy) + "GeV").data(), "l");
	}

	TCanvas can("Opt_can", "B Optimization Canvas");

	mg.GetXaxis()->SetTitle("Impact Parameter b (fm)");
	mg.GetYaxis()->SetTitle("Sum( (BES1 - AMPT)^2 ) / DoF");
	mg.SetTitle("BES1 to AMPT Ref3 distribution Chi^2 vs b cutoff (max)");
	mg.Draw("a");
	leg.Draw();

	write_b_opt();

	for(pair<int, float> opt_pars:opt_b_max) {
		cout << "Energy " << opt_pars.first << "GeV optimal b = " << opt_pars.second << "fm" << endl;
		auto *line = new TLine(opt_pars.second, 0.0, opt_pars.second, chi_max);
		line->SetLineColor(energy_color[opt_pars.first]);
		can.cd();
		line->Draw();
	}
	can.Write();
	out_file.Close();
}


float AmptCentralityMaker::get_chi_dof_diff(float b_cut, int energy) {
	string name = "Ampt_" + mult_quantity + "_" + to_string(energy) + "GeV_dist";
	TH1D ampt_dist(name.data(), name.data(), ref_bins, ref_min, ref_max);
	for(event_imp_ref const &event:events) {
		if(event.b < b_cut) {
			ampt_dist.Fill(event.ref);
		}
	}

	TH1D *norm_data_dist = (TH1D*) star_ref_dist.Clone("norm_data_dist");
	TH1D *norm_ampt_dist = (TH1D*) ampt_dist.Clone("norm_ampt_dist");
	norm_data_dist->Scale(1.0/star_ref_dist.Integral());
	norm_ampt_dist->Scale(1.0/ampt_dist.Integral());

	float chi2 = 0;
	for(int i = 1; i <= norm_data_dist->GetNbinsX(); i++) {
		chi2 += pow(norm_data_dist->GetBinContent(i) - norm_ampt_dist->GetBinContent(i), 2);
	}
	chi2 /= norm_data_dist->GetNbinsX() - 1;

	delete norm_data_dist;
	delete norm_ampt_dist;

	return chi2;
}


void AmptCentralityMaker::plot_ref_dist_comp(float b_cut, int energy) {
	star_ref_dist.SetLineColor(kBlack);

	string name = "Ampt_" + mult_quantity + "_" + to_string(energy) + "GeV_dist";
	TH1D ampt_dist(name.data(), name.data(), ref_bins, ref_min, ref_max);
	ampt_dist.SetLineColor(kBlue);
	for(event_imp_ref const &event:events) {
		if(event.b < b_cut) {
			ampt_dist.Fill(event.ref);
		}
	}

	TLegend leg1;
	leg1.AddEntry(&star_ref_dist, "BES1 Distribution", "l");
	leg1.AddEntry(&ampt_dist, "Ampt Distribution", "l");
	TCanvas can1(((to_string(energy) + "GeV_Raw_Canvas")).data());
	star_ref_dist.Draw();
	ampt_dist.Draw("SAMES");
	leg1.Draw();
	can1.Write();

	TH1D *norm_data_dist = (TH1D*) star_ref_dist.Clone("norm_data_dist");
	TH1D *norm_ampt_dist = (TH1D*) ampt_dist.Clone("norm_ampt_dist");
	norm_data_dist->Scale(1.0/star_ref_dist.Integral());
	norm_ampt_dist->Scale(1.0/ampt_dist.Integral());

	TLegend leg2;
	leg2.AddEntry(norm_data_dist, "BES1 Distribution", "l");
	leg2.AddEntry(norm_ampt_dist, "Ampt Distribution", "l");
	TCanvas can2((to_string(energy) + "GeV_Norm_Canvas").data());
	norm_ampt_dist->Draw();
	norm_data_dist->Draw("SAMES");
	leg2.Draw();
	can2.Write();

	TH1D ratio_dist((to_string(energy) + "GeV_ratio_dist").data(), (to_string(energy) + "GeV Ampt / BES1 Ratio (Normed Values)").data(), star_ref_dist.GetNbinsX(), star_ref_dist.GetXaxis()->GetXmin(), star_ref_dist.GetXaxis()->GetXmax());
	for(int i = 1; i <= star_ref_dist.GetNbinsX(); i++) {
		if(norm_data_dist->GetBinContent(i) > 0) {
			ratio_dist.SetBinContent(i, norm_ampt_dist->GetBinContent(i) / norm_data_dist->GetBinContent(i));
		} else {
			ratio_dist.SetBinContent(i, 1.0);
		}
	}
	TCanvas can3((to_string(energy) + "GeV_Ratio_Canvas").data());
	ratio_dist.Draw();
	can3.Write();
}


void AmptCentralityMaker::get_imp_ref(int energy) {
	string energy_path = min_bias_path + to_string(energy) + "GeV/";
	vector<string> min_bias_files = get_files_in_dir(energy_path, "root", "path");

	events.clear();
	for(string path:min_bias_files) {
//		cout << "Reading " << path << endl;
		TFile in_file(path.data(), "READ");
		TTree *tree = (TTree*)in_file.Get(tree_name.data());
		ampt_tree_branches branches;
		set_ampt_tree_branches(tree, branches);
		set_ampt_branches(tree);

		int event_index = 0;
		while(tree->GetEntry(event_index++)) {
			event_imp_ref new_event;
			new_event.b = branches.imp;
			if(mult_quantity == "ref2") { new_event.ref = branches.refmult2; }
			else if(mult_quantity == "ref3") { new_event.ref = branches.refmult3; }
			events.push_back(new_event);
		}
		tree->ResetBranchAddresses();
		in_file.Close();
	}
}


void AmptCentralityMaker::get_star_ref(int energy) {
	string energy_path = star_data_path + to_string(energy) + "GeV/";
	vector<string> star_files = get_files_in_dir(energy_path, "root", "path");

	string name = "Star_" + mult_quantity + "_" + to_string(energy) + "GeV_dist";
	star_ref_dist = TH1D(name.data(), name.data(), ref_bins, ref_min, ref_max);

	for(string path:star_files) {
//		cout << "Reading " << path << endl;
		TFile in_file(path.data(), "READ");
		TTree *tree = (TTree*)in_file.Get(tree_name.data());
		tree_branches branches;
		set_tree_branches(tree, branches);
		set_star_branches(tree);

		int event_index = 0;
		while(tree->GetEntry(event_index++)) {
			star_ref_dist.Fill(branches.refmultn);
		}
		tree->ResetBranchAddresses();
		in_file.Close();
	}
}


void AmptCentralityMaker::set_ampt_branches(TTree* tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("imp", 1);
	if(mult_quantity == "ref2") { tree->SetBranchStatus("refmult2", 1); }
	else if(mult_quantity == "ref3") { tree->SetBranchStatus("refmult3", 1); }
}

void AmptCentralityMaker::set_star_branches(TTree *tree) {
	tree->SetBranchStatus("*", 0);
	if(mult_quantity == "ref2") { tree->SetBranchStatus("refmult2", 1); }
	else if(mult_quantity == "ref3") { tree->SetBranchStatus("refmult3", 1); }
}

void AmptCentralityMaker::make_centrality(bool make_new) {
	if(!make_new) { read_bin_edges(); }
	if((!make_new && ref_bin_edges.size() == 0) || make_new) {
		if(opt_b_max.count(energy) == 0) { read_b_opt(); }
		if(opt_b_max.count(energy) == 0) { cout << "No optimal b found for energy " << energy << endl; }
		else {
			cout << "Make " << energy << "GeV centrality" << endl;
			get_distribution();
			sort_and_bin();
			write_bin_edges();
		}
	}
}

void AmptCentralityMaker::get_distribution() {
	ref_dist.clear();
	b_dist.clear();
	vector<string> in_files = get_files_in_dir(min_bias_path + to_string(energy) + "GeV/", "root", "path");
	for(string path:in_files) {
		TFile file(path.data(), "READ");
		TTree *tree = (TTree*)file.Get(tree_name.data());
		set_ampt_tree_branches(tree, branches);
		set_ampt_branches(tree);

		int event_index = 0;
		while(tree->GetEvent(event_index++)) {
			append_dist_val();
		}
		file.Close();
	}
}

void AmptCentralityMaker::get_ref2() {
	if(branches.imp <= opt_b_max[energy]) {
		ref_dist.push_back(branches.refmult2);
	}
}

void AmptCentralityMaker::get_ref3() {
	if(branches.imp <= opt_b_max[energy]) {
		ref_dist.push_back(branches.refmult3);
	}
}

void AmptCentralityMaker::get_b() {
	b_dist.push_back(branches.imp);
}

void AmptCentralityMaker::sort_bin_ref() {
	sort(ref_dist.begin(), ref_dist.end());
	float step = (float)ref_dist.size() / bins;
	ref_bin_edges.clear();
	for(int bin_edge = 1; bin_edge < bins ; bin_edge++) {
		int index = (int)(bin_edge * step + 0.5);
		ref_bin_edges.push_back(ref_dist[index]);
	}
//	int num_last_bin = 0;
//	for(int entry:ref_dist) { if(entry > ref_bin_edges.back()) num_last_bin++; }
//	cout << "Total events: " << ref_dist.size() << "  |  Events greater than last bin edge: " << num_last_bin << endl;
	ref_dist.clear();
}

void AmptCentralityMaker::sort_bin_b() {
	sort(b_dist.begin(), b_dist.end());
	float step = (float)b_dist.size() / bins;
	b_bin_edges.clear();
	for(int bin_edge = 1; bin_edge < bins ; bin_edge++) {
		int index = (int)(bin_edge * step + 0.5);
		b_bin_edges.push_back(b_dist[index]);
	}
	b_dist.clear();
}


void AmptCentralityMaker::write_b_opt() {
	if(!check_path(qa_path)) { cout << "Can't access path: " << qa_path << " Skipping following read/write." << endl; return; }

	string out_name = qa_path + qa_file_sufx;
	ofstream out_txt(out_name);
	if(!out_txt.is_open()) { cout << "Could not open " << out_name << " Not writing." << endl; return; }

	out_txt << "ref quantity: " << mult_quantity << endl;
	out_txt << "accuracy: " << acc << endl;

	for(pair<int, float> opt_val:opt_b_max) {
		out_txt << endl << opt_val.first << " GeV\tb_opt: " << opt_val.second;
	}

	out_txt.close();
}


void AmptCentralityMaker::read_b_opt() {
	if(!check_path(qa_path)) { cout << "Can't access path: " << qa_path << " Skipping following read/write." << endl; return; }

	string in_name = qa_path + qa_file_sufx;
	ifstream in_txt(in_name);
	if(!in_txt.is_open()) { cout << "Could not open " << in_name << " Not reading." << endl; return; }

	opt_b_max.clear();
	string line;
	int line_num = 1;
	while(getline(in_txt, line)) {
		if(line_num >= 4) {
			vector<string> energy_line = split(line, '\t');
			if(energy_line.size() == 2) {
				int energy; float opt_val;

				vector<string> energy_str = split(energy_line[0], ' ');
				if(energy_str.size() == 2) {
					energy = stoi(energy_str[0]);
				} else { cout << "Bad energy string read: " << line << endl; continue; }

				vector<string> opt_str = split(energy_line[1], ' ');
				if(opt_str.size() == 2) {
					opt_val = stof(opt_str[1]);
				} else { cout << "Bad energy string read: " << line << endl; continue; }

				opt_b_max[energy] = opt_val;

			} else { cout << "Bad opt line read: " << line << endl; }
		}
		line_num++;
	}
}


void AmptCentralityMaker::write_bin_edges() {
	if(!check_path(qa_path)) { cout << "Can't access path: " << qa_path << " Skipping following read/write." << endl; return; }

	string out_name = qa_path + to_string(energy) + "GeV_" + bin_file_sufx;
	ofstream out_txt(out_name);
	if(!out_txt.is_open()) { cout << "Could not open " << out_name << " Not reading." << endl; return; }

	out_txt << energy << "GeV ref 20 bin edges" << endl;
	out_txt << endl;

	string bin_edge_str;
	for(int bin_edge:ref_bin_edges) {
		bin_edge_str += to_string(bin_edge) + " ";
	}
	bin_edge_str.pop_back();
	out_txt << bin_edge_str << flush;

	out_txt.close();
}


void AmptCentralityMaker::read_bin_edges() {
	if(!check_path(qa_path)) { cout << "Can't access path: " << qa_path << " Skipping following read/write." << endl; return; }

	string in_name = qa_path + to_string(energy) + "GeV_" + bin_file_sufx;
	ifstream in_txt(in_name);
	if(!in_txt.is_open()) { cout << "Could not open " << in_name << " Not reading." << endl; return; }

	ref_bin_edges.clear();
	string line;
	int line_num = 1;
	while(getline(in_txt, line)) {
		if(line_num == 3) {
			vector<string> bins_string = split(line, ' ');
			for(string bin_str:bins_string) {
				ref_bin_edges.push_back(stoi(bin_str));
			}
		}
		line_num++;
	}
}
