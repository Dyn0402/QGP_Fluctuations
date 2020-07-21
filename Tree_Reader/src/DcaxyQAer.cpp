/*
 * Dca_xy_QAer.cpp
 *
 *  Created on: Jul 2, 2020
 *      Author: Dylan Neff
 */


#include "DcaxyQAer.h"


// Structors

DcaxyQAer::DcaxyQAer(int energy) {
	start_sys = chrono::system_clock::now();
	this->energy = energy;
	out_file = new TFile((out_path+"Dca_xy_qa_"+to_string(energy)+"_cent8.root").data(), "RECREATE");
	refmultCorrUtil = new StRefMultCorr("refmult3");
}

DcaxyQAer::~DcaxyQAer() {
	out_file->Close();
	delete out_file;
}


// Getters


// Setters

void DcaxyQAer::set_energy(int energy) {
	this->energy = energy;
}

void DcaxyQAer::set_in_path(string path) {
	in_path = path;
}


// Doers

void DcaxyQAer::run_qa() {
	read_trees();
	analyze_runs();
}

void DcaxyQAer::read_trees() {
	cout << "Reading " + to_string(energy) + "GeV trees." << endl << endl;
	vector<string> in_files = get_files_in_dir(in_path+to_string(energy)+"GeV/", "root", "path");

	unsigned num_files = in_files.size();
	unsigned file_index = 1;

	cout << "here 0" << endl;

	for(string path:in_files) {

		// Display progress and time while running.
//		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
//			cout << "here 1" << endl;
//			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
//			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
//			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
//			cout << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
//		}

		cout << "here 2" << endl;
		TFile *file = new TFile(path.data(), "READ");
		cout << "here 3" << endl;
		TTree *tree = (TTree*)file->Get(tree_name.data());
		cout << "here 4" << endl;
		read_tree(tree);  // Read tree from file into data
		cout << "here 5" << endl;
		file->Close();
		delete file;
		file_index++;
	}

	chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
	auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
	cout << endl << "Finished reading " + to_string(energy) + "GeV trees. 100% complete | time: " << (clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
}


void DcaxyQAer::read_tree(TTree* tree) {
	tree_leaves leaves = get_tree_leaves(tree, "Proton", 3);
	set_branches(tree);

	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		event_index++;
		unsigned run = leaves.run->GetValue();

//		refmultCorrUtil->init(run);
//		refmultCorrUtil->initEvent(leaves.ref_multn->GetValue(), (double)leaves.vz->GetValue());
		int cent9_corr = 0; // refmultCorrUtil->getCentralityBin9();

		unsigned event_id = leaves.event_id->GetValue();
		float dca_xy_avg = leaves.dca_xy_avg->GetValue();
		float dca_xy_sd = leaves.dca_xy_sd->GetValue();

		if(run == 11149014 && (event_id == 91862 || event_id == 92400 || event_id == 39892 || event_id == 149360)) {
			refmultCorrUtil->init(run);
			refmultCorrUtil->initEvent(leaves.ref_multn->GetValue(), (double)leaves.vz->GetValue());
			cent9_corr = refmultCorrUtil->getCentralityBin9();
			cout << "Run " << run << ", event " << event_id << " | cent: " << cent9_corr << " ref3: " << leaves.ref_multn->GetValue() << " vz: " << leaves.vz->GetValue() << " dca_xy_avg: " << dca_xy_avg << endl;
			cout << refmultCorrUtil->getRefMultCorr() << endl;
			//			refmultCorrUtil->print();
		}

		if(!(cent9_corr == 8)) { continue; }

//		cout << event_id << " " << dca_xy_avg << "  " << dca_xy_sd << " " << leaves.ref_multn->GetValue() << endl;

		if(dca_xy_avg == -899 && dca_xy_sd == -899) {
//			cout << "Zero tracks for Run " << run << " Event " << event_id << endl;
			continue;
		}

//		dca_event[run][event_id] = {dca_xy_avg, pow(dca_xy_sd, 0.5)};
		dca_event[run][event_id] = {dca_xy_avg, dca_xy_sd};
		if(dca_run.find(run) == dca_run.end()) {
			dca_run[run] = {0, 0, 0};
		}
		dca_run[run][0]++;
		dca_run[run][1] += dca_xy_avg;
		dca_run[run][2] += pow(dca_xy_avg, 2);
	}
}

void DcaxyQAer::set_branches(TTree* tree) {
	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus("run", 1);
	tree->SetBranchStatus("event_id", 1);
	tree->SetBranchStatus("dca_xy_avg", 1);
	tree->SetBranchStatus("dca_xy_err", 1);
	tree->SetBranchStatus("ref3", 1);
	tree->SetBranchStatus("vtx_z", 1);
}


void DcaxyQAer::analyze_runs() {
	// Calculate mean and sd of dca_xy for each run
	for(auto &run:dca_run) {
		run.second[1] /= run.second[0];
		run.second[2] = pow(run.second[2] / run.second[0] - pow(run.second[1], 2), 0.5);
	}

	out_file->cd();

//	TH2F *group = new TH2F();
	vector<TGraphErrors*> dca_xy_runs;
	vector<TCanvas*> dca_xy_runs_can;

	int num_events = 0;
	int num_7sig_events = 0;
	int num_runs = 0;
	int num_7sig_runs = 0;
	for(auto &run:dca_event) {
		num_runs++;
		vector<float> event_ids;
		vector<float> dca_xy_val;
		vector<float> dca_xy_err;
		bool flag = false;
		for(auto &event:run.second) {
			num_events++;
			float sigmas = (event.second.first - dca_run[run.first][1]) /  dca_run[run.first][2];
			if(fabs(sigmas) > 7) {
				flag = true;
				num_7sig_events++;
			}
//			if(fabs(sigmas) > 0) {
			if(run.first == 11149014) {
//				cout << event.first << " " << event.second.first << " 0 " << event.second.second << endl;
			}
			event_ids.push_back(event.first);
			dca_xy_val.push_back(event.second.first);
			dca_xy_err.push_back(event.second.second);
//			}
		}
		dca_xy_runs.push_back(new TGraphErrors((int)event_ids.size(), event_ids.data(), dca_xy_val.data(), 0, dca_xy_err.data()));
		dca_xy_runs.back()->SetNameTitle(("Graph Run "+to_string(run.first)).data(), ("Run "+to_string(run.first)).data());
		dca_xy_runs.back()->SetMarkerStyle(kOpenCircle);
		dca_xy_runs_can.push_back(new TCanvas(("Can Run "+to_string(run.first)).data(), ("Run "+to_string(run.first)).data()));
		dca_xy_runs.back()->Draw("APZ");
		dca_xy_runs_can.back()->Update();
		TLine *avg = new TLine(dca_xy_runs_can.back()->GetUxmin(), dca_run[run.first][1], dca_xy_runs_can.back()->GetUxmax(), dca_run[run.first][1]);
		avg->SetLineColor(kGreen+3);
		avg->Draw("Same");
		TLine *sig_low_3 = new TLine(dca_xy_runs_can.back()->GetUxmin(), dca_run[run.first][1] - 3*dca_run[run.first][2], dca_xy_runs_can.back()->GetUxmax(), dca_run[run.first][1] - 3*dca_run[run.first][2]);
		sig_low_3->SetLineColor(kOrange);
		sig_low_3->Draw("Same");
		TLine *sig_high_3 = new TLine(dca_xy_runs_can.back()->GetUxmin(), dca_run[run.first][1] + 3*dca_run[run.first][2], dca_xy_runs_can.back()->GetUxmax(), dca_run[run.first][1] + 3*dca_run[run.first][2]);
		sig_high_3->SetLineColor(kOrange);
		sig_high_3->Draw("Same");
		TLine *sig_low_7 = new TLine(dca_xy_runs_can.back()->GetUxmin(), dca_run[run.first][1] - 7*dca_run[run.first][2], dca_xy_runs_can.back()->GetUxmax(), dca_run[run.first][1] - 7*dca_run[run.first][2]);
		sig_low_7->SetLineColor(kRed);
		sig_low_7->Draw("Same");
		TLine *sig_high_7 = new TLine(dca_xy_runs_can.back()->GetUxmin(), dca_run[run.first][1] + 7*dca_run[run.first][2], dca_xy_runs_can.back()->GetUxmax(), dca_run[run.first][1] + 7*dca_run[run.first][2]);
		sig_high_7->SetLineColor(kRed);
		sig_high_7->Draw("Same");
		dca_xy_runs_can.back()->Write();
		delete avg;
//		delete dca_xy_runs.back();
		if(flag) { num_7sig_runs++; }
	}
	cout << "Runs: " << num_runs << endl;
	cout << "7sig runs: " << num_7sig_runs << endl;
	cout << "Percentage of runs with bad event: " << ((float)num_7sig_runs) / num_runs * 100 << "%" << endl;
	cout << "Events: " << num_events << endl;
	cout << "7sig events: " << num_7sig_events << endl;
	cout << "Percentage of events bad: " << ((float)num_7sig_events) / num_events * 100 << "%" << endl;

//	for(auto &graph:dca_xy_runs) { graph->Write(); }
}
