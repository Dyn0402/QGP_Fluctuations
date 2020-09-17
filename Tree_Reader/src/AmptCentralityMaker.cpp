/*
 * AmptCentralityMaker.cpp
 *
 *  Created on: Apr 11, 2020
 *      Author: Dylan Neff
 */


#include "AmptCentralityMaker.h"



// Structors

AmptCentralityMaker::AmptCentralityMaker() {
	in_path = "";
	out_path = "";
	set_mult_quantity("ref3");
}

AmptCentralityMaker::AmptCentralityMaker(string in_path) {
	this->in_path = in_path;
	out_path = "";  // "/home/dylan/Research/Ampt_Centralities/"
	set_mult_quantity("ref3");
}

AmptCentralityMaker::AmptCentralityMaker(string in_path, string mult_quantity) {
	this->in_path = in_path;
	set_mult_quantity(mult_quantity);
}

AmptCentralityMaker::AmptCentralityMaker(string in_path, string out_path, string mult_quantity) {
	this->in_path = in_path;
	this->out_path = out_path;
	set_mult_quantity(mult_quantity);
}

AmptCentralityMaker::~AmptCentralityMaker() {
	//Nothing
}


// Getters

string AmptCentralityMaker::get_in_path() {
	return in_path;
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

		if((int)ref_bin_edges.size() == 0) { make_centrality(); }

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

void AmptCentralityMaker::set_in_path(string in_path) {
	this->in_path = in_path;
}

void AmptCentralityMaker::set_out_path(string out_path) {
	this->out_path = out_path;
}

void AmptCentralityMaker::set_min_bias_path(string min_bias_path) {
	this->min_bias_path = min_bias_path;
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
		cout << "Invalid multiplicty quantity to define centrality. Options are: ref2, ref3, b" << endl;
		return;
	}
	mult_quantity = quantity;
}

void AmptCentralityMaker::set_max_b(float b) {
	max_b = b;
}

void AmptCentralityMaker::set_ref_num(int ref) {
	ref_num = ref;
}

// Doers

//void AmptCentralityMaker::run_b_opt(vector<int> energy_vec, string out_path) {
//	float acc = 0.01;
//	map<int, int> energy_color {{7, 1}, {11, 2}, {19, 4}, {27, 6}, {39, 8}, {62, 9}};
//	map<int, float> b_min {{7, 14}, {11, 14}, {19, 13.5}, {27, 13.5}, {39, 13}, {62, 13}};
//	map<int, float> opt;
//
//	auto *mg = new TMultiGraph();
//	auto *leg = new TLegend();
//	float chi_max = 0;
//
//	for(int energy:energy_vec) {
//		float b_cut_step = 0.5;
//		vector<float> chi_diff;
//		vector<float> b;
//		map<float, float> b_chi_map;
//
//
//
//		b.push_back(b_min[energy]);
//		cout << "Calculating chi^2 for " + to_string(energy) + "GeV, b = " << b.back() << endl;
//		chi_diff.push_back(get_chi_diff(b.back(), energy));
//		b_chi_map[b.back()] = chi_diff.back();
//
//		while(fabs(b_cut_step) >= acc) {
//			b.push_back(b.back() + b_cut_step);
//			cout << "Calculating chi^2 for " + to_string(energy) + "GeV, b = " << b.back() << endl;
//			chi_diff.push_back(get_chi_diff(b.back(), energy));
//			b_chi_map[b.back()] = chi_diff.back();
//			while(chi_diff.back() < chi_diff[(int)chi_diff.size() - 2]) {
//				b.push_back(b.back() + b_cut_step);
//				cout << "Calculating chi^2 for " + to_string(energy) + "GeV, b = " << b.back() << endl;
//				chi_diff.push_back(get_chi_diff(b.back(), energy));
//				b_chi_map[b.back()] = chi_diff.back();
//			}
//			b_cut_step /= -2;
//		}
//		opt[energy] = b[(int)b.size() - 2];
//		if(chi_diff[(int)chi_diff.size() - 2] > chi_max) { chi_max = chi_diff[(int)chi_diff.size() - 2]; }
//
//		chi_diff.clear(); b.clear();
//		for(pair<float, float> b_chi:b_chi_map) {
//			b.push_back(b_chi.first);
//			chi_diff.push_back(b_chi.second);
//		}
//
//		TGraph *graph = new TGraph((int)b.size(), b.data(), chi_diff.data());
//		graph->SetMarkerColor(energy_color[energy]);
//		graph->SetLineColor(energy_color[energy]);
//		graph->SetLineWidth(3);
//		graph->SetMarkerStyle(8);
//		mg->Add(graph, "l");
//		leg->AddEntry(graph, (to_string(energy) + "GeV").data(), "l");
//	}
//
//	mg->GetXaxis()->SetTitle("Impact Parameter b (fm)");
//	mg->GetYaxis()->SetTitle("Sum( (BES1 - AMPT)^2 )");
//	mg->SetTitle("BES1 to AMPT Ref3 distribution Chi^2 vs b cutoff (max)");
//	mg->Draw("a");
//	leg->Draw();
//
//	for(pair<int, float> opt_pars:opt) {
//		cout << "Energy " << opt_pars.first << "GeV optimal b = " << opt_pars.second << "fm" << endl;
//		auto *line = new TLine(opt_pars.second, 0.0, opt_pars.second, chi_max);
//		line->SetLineColor(energy_color[opt_pars.first]);
//		line->Draw();
//	}
//}
//
//
//float get_chi_diff(float b_cut, int energy) {
//	string data_path = "/media/dylan/SSD_Storage/Research/Data_Old_Ref3/eta050/" + to_string(energy) + "GeV/QA_" + to_string(energy) + "GeV.root";
//	string ampt_path = "/media/dylan/SSD_Storage/Research/Trees_Ampt/" + to_string(energy) + ".root";
//
//	TFile *data_file = new TFile(data_path.data(), "READ");
//	TH1I *int_data_dist = (TH1I*)data_file->Get(("pre_reftwo_eta050_" + to_string(energy)).data());
//	TH1D* data_dist = new TH1D("data_dist", "BES1 Distribution", int_data_dist->GetNbinsX(), int_data_dist->GetXaxis()->GetXmin(), int_data_dist->GetXaxis()->GetXmax());
//	for(int i =1; i<=int_data_dist->GetNbinsX(); i++) { data_dist->SetBinContent(i, int_data_dist->GetBinContent(i)); }
//
//	TFile *ampt_file = new TFile(ampt_path.data(), "READ");
//	TTree *ampt_tree = (TTree*)ampt_file->Get("tree");
//	TH1D *ampt_dist = new TH1D("ampt_dist", "Ampt Distribution", data_dist->GetNbinsX(), data_dist->GetXaxis()->GetXmin(), data_dist->GetXaxis()->GetXmax());
//	TLeaf *dist_leaf = ampt_tree->GetLeaf("refmult3");
//	TLeaf *b_leaf = ampt_tree->GetLeaf("imp");
//	int event_index = 0;
//	while(ampt_tree->GetEntry(event_index)) {
//		if(b_leaf->GetValue() < b_cut) {
//			ampt_dist->Fill(dist_leaf->GetValue());
//		}
//		event_index++;
//	}
//
//	TH1D *norm_data_dist = (TH1D*) data_dist->Clone("norm_data_dist");
//	TH1D *norm_ampt_dist = (TH1D*) ampt_dist->Clone("norm_ampt_dist");
//	norm_data_dist->Scale(1.0/data_dist->Integral());
//	norm_ampt_dist->Scale(1.0/ampt_dist->Integral());
//
//	float chi2 = 0;
//	for(int i = 1; i <= norm_data_dist->GetNbinsX(); i++) {
//		chi2 += pow(norm_data_dist->GetBinContent(i) - norm_ampt_dist->GetBinContent(i), 2);
//	}
//
//	delete int_data_dist;
//	delete data_dist;
//	delete ampt_dist;
//	delete norm_data_dist;
//	delete norm_ampt_dist;
//
//	data_file->Close();
//	ampt_file->Close();
//	delete data_file;
//	delete ampt_file;
//
//	return chi2;
//}


void AmptCentralityMaker::set_branches(TTree* tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("imp", 1);
	if(mult_quantity == "ref2") { tree->SetBranchStatus("refmult2", 1); }
	else if(mult_quantity == "ref3") { tree->SetBranchStatus("refmult3", 1); }
}

void AmptCentralityMaker::make_centrality() {
	get_distribution();
	sort_and_bin();
}

void AmptCentralityMaker::get_distribution() {
	ref_dist.clear();
	b_dist.clear();
	vector<string> in_files = get_files_in_dir(in_path, "root", "path");
	for(string path:in_files) {
		TFile *file = new TFile(path.data(), "READ");
		TTree *tree = (TTree*)file->Get(tree_name.data());
		set_ampt_tree_branches(tree, branches);
		set_branches(tree);

		int event_index = 0;
		while(tree->GetEvent(event_index)) {
			append_dist_val();
			event_index++;
		}
		file->Close();
		delete file;
	}
}

void AmptCentralityMaker::get_ref2() {
	if(branches.imp <= max_b) {
		ref_dist.push_back(branches.refmult2);
	}
}

void AmptCentralityMaker::get_ref3() {
	if(branches.imp <= max_b) {
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

