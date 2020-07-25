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
	out_file = NULL;
	refmultCorrUtil = new StRefMultCorr("refmult3");
}

DcaxyQAer::~DcaxyQAer() {
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

//	cout << "here 0" << endl;

	for(string path:in_files) {

		// Display progress and time while running.
		if(!(file_index % (unsigned)(num_files/10.0+0.5))) { // Gives floating point exception for too few num_files --> % 0. Fix!!!
//			cout << "here 1" << endl;
			chrono::duration<double> elap = chrono::system_clock::now() - start_sys;
			auto datetime = chrono::system_clock::to_time_t(chrono::system_clock::now());
			vector<string> datetime_vec = split((string)ctime(&datetime), ' ');
			cout << " " << energy << "GeV " << (int)(100.0*file_index/num_files+0.5) << "% complete | time: " << (double)(clock() - start) / CLOCKS_PER_SEC << "s" << " , " << elap.count() << "s  | " << datetime_vec[0] << " " << datetime_vec[3] << endl;
		}

//		cout << "here 2" << endl;
//		cout << path << endl;
//		gErrorIgnoreLevel = 3001;
//		while(true) {  // Check if file is open before reading, if so sleep/check till closed.
//			if((string)gSystem->GetFromPipe(("lsof " + path).data()) == "") { break; };
//			cout << "Busy file, waiting:  " << path << endl;
//			this_thread::sleep_for(chrono::seconds(1));
//		}
//		gErrorIgnoreLevel = -1;
		TFile *file = new TFile(path.data(), "READ");
//		cout << "here 3" << endl;
		TTree *tree = (TTree*)file->Get(tree_name.data());
//		cout << "here 4" << endl;
		read_tree(tree);  // Read tree from file into data
//		cout << "here 5" << endl;
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
//	cout << "here 4.1" << endl;
	set_branches(tree);
//	cout << "here 4.2" << endl;

	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		event_index++;
		unsigned run = leaves.run->GetValue();

//		refmultCorrUtil->init(run);
//		refmultCorrUtil->initEvent(leaves.ref_multn->GetValue(), (double)leaves.vz->GetValue());
		int cent9_corr = 0; // refmultCorrUtil->getCentralityBin9();

//		cout << "here 4.3" << endl;
		unsigned event_id = leaves.event_id->GetValue();
		float dca_xy_avg = leaves.dca_xy_avg->GetValue();
		float dca_xy_err = leaves.dca_xy_err->GetValue();

		refmultCorrUtil->init(run);
		refmultCorrUtil->initEvent(leaves.ref_multn->GetValue(), (double)leaves.vz->GetValue());
		cent9_corr = refmultCorrUtil->getCentralityBin9();
//		cout << "Run " << run << ", event " << event_id << " | cent: " << cent9_corr << " ref3: " << leaves.ref_multn->GetValue() << " vz: " << leaves.vz->GetValue() << " dca_xy_avg: " << dca_xy_avg << endl;
//		cout << refmultCorrUtil->getRefMultCorr() << endl;

		if(cent9_corr >= cent_min) { continue; }

//		cout << event_id << " " << dca_xy_avg << "  " << dca_xy_sd << " " << leaves.ref_multn->GetValue() << endl;

		if(dca_xy_avg == -899 && dca_xy_err == -899) {
//			cout << "Zero tracks for Run " << run << " Event " << event_id << endl;
			continue;
		}

		dca_event[run][event_id] = {dca_xy_avg, dca_xy_err};
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
	tree->SetBranchStatus("run_num", 1);
	tree->SetBranchStatus("event_id", 1);
	tree->SetBranchStatus("dca_xy_avg", 1);
	tree->SetBranchStatus("dca_xy_err", 1);
	tree->SetBranchStatus("refmult3", 1);
	tree->SetBranchStatus("vz", 1);
}


void DcaxyQAer::analyze_runs() {
	cout << "Analyzing " << energy << "GeV Runs" << endl;

	// Calculate mean and sd of dca_xy for each run
	vector<float> run_num, run_indexes, dca_xy_run_avg, dca_xy_run_err;
	int set_count = 0, run_index = 1;
	double set_avg = 0., set_err = 0.;
	for(auto &run:dca_run) {
		set_count += run.second[0];
		set_avg += run.second[1]; set_err += run.second[2];
		run.second[1] /= run.second[0];
		run.second[2] = pow(run.second[2] / run.second[0] - pow(run.second[1], 2), 0.5);
		run_num.push_back(run.first);
		run_indexes.push_back(run_index++);
		dca_xy_run_avg.push_back(run.second[1]);
		dca_xy_run_err.push_back(run.second[2]);
	}
	set_avg /= set_count; set_err = pow(set_err / set_count - pow(set_avg, 2), 0.5);

	out_file = new TFile((out_path+"Dca_xy_qa_"+to_string(energy)+"_cent_6-8.root").data(), "RECREATE");
	out_file->cd();
	string energy_str = to_string(energy);
	TCanvas can_dataset((energy_str+"GeV_Dca_xy_vs_Run_Can").data(), (energy_str+"GeV Dca XY vs Run").data());
	TGraphErrors dca_xy_dataset((int)run_num.size(), run_num.data(), dca_xy_run_avg.data(), 0, dca_xy_run_err.data());
	dca_xy_dataset.SetNameTitle((energy_str+"GeV_Dca_xy_vs_Run_Graph").data(), (energy_str+"GeV Dca XY vs Run").data());
	dca_xy_dataset.SetMarkerStyle(kOpenCircle);
	dca_xy_dataset.Draw("APZ");
	can_dataset.Update();
	TF1 avg_dataset((energy_str + "GeV_avg_dataset").data(), "[0]", can_dataset.GetUxmin(), can_dataset.GetUxmax());
	avg_dataset.SetParameter(0, set_avg);
	avg_dataset.SetLineColor(kGreen+2); avg_dataset.Draw("Same");
	TF1 sig_low_3_dataset((energy_str + "GeV_sig_low_3_dataset").data(), "[0]", can_dataset.GetUxmin(), can_dataset.GetUxmax());
	sig_low_3_dataset.SetParameter(0, set_avg - 3 * set_err);
	sig_low_3_dataset.SetLineColor(kOrange+7); sig_low_3_dataset.Draw("Same");
	TF1 sig_high_3_dataset((energy_str + "GeV_sig_high_3_dataset").data(), "[0]", can_dataset.GetUxmin(), can_dataset.GetUxmax());
	sig_high_3_dataset.SetParameter(0, set_avg + 3 * set_err);
	sig_high_3_dataset.SetLineColor(kOrange+7); sig_high_3_dataset.Draw("Same");
	TF1 sig_low_7_dataset((energy_str + "GeV_sig_low_7_dataset").data(), "[0]", can_dataset.GetUxmin(), can_dataset.GetUxmax());
	sig_low_7_dataset.SetParameter(0, set_avg - 7 * set_err);
	sig_low_7_dataset.SetLineColor(kOrange+9); sig_low_7_dataset.Draw("Same");
	TF1 sig_high_7_dataset((energy_str + "GeV_sig_high_7_dataset").data(), "[0]", can_dataset.GetUxmin(), can_dataset.GetUxmax());
	sig_high_7_dataset.SetParameter(0, set_avg + 7 * set_err);
	sig_high_7_dataset.SetLineColor(kOrange+9); sig_high_7_dataset.Draw("Same");
	can_dataset.Write();
//	TCanvas *can_dataset = new TCanvas((energy_str+"GeV_Dca_xy_vs_Run_Can").data(), (energy_str+"GeV Dca XY vs Run").data());
//	TGraphErrors* dca_xy_dataset = new TGraphErrors((int)run_num.size(), run_num.data(), dca_xy_run_avg.data(), 0, dca_xy_run_err.data());
//	dca_xy_dataset->SetNameTitle((energy_str+"GeV_Dca_xy_vs_Run_Graph").data(), (energy_str+"GeV Dca XY vs Run").data());
//	dca_xy_dataset->SetMarkerStyle(kOpenCircle);
//	dca_xy_dataset->Draw("APZ");
//	can_dataset->Update();
//	TF1 *avg_dataset = new TF1((energy_str + "GeV_avg_dataset").data(), "[0]", can_dataset->GetUxmin(), can_dataset->GetUxmax());
//	avg_dataset->SetParameter(0, set_avg);
//	avg_dataset->SetLineColor(kGreen+2); avg_dataset->Draw("Same");
//	TF1 *sig_low_3_dataset = new TF1((energy_str + "GeV_sig_low_3_dataset").data(), "[0]", can_dataset->GetUxmin(), can_dataset->GetUxmax());
//	sig_low_3_dataset->SetParameter(0, set_avg - 3 * set_err);
//	sig_low_3_dataset->SetLineColor(kOrange+7); sig_low_3_dataset->Draw("Same");
//	TF1 *sig_high_3_dataset = new TF1((energy_str + "GeV_sig_high_3_dataset").data(), "[0]", can_dataset->GetUxmin(), can_dataset->GetUxmax());
//	sig_high_3_dataset->SetParameter(0, set_avg + 3 * set_err);
//	sig_high_3_dataset->SetLineColor(kOrange+7); sig_high_3_dataset->Draw("Same");
//	TF1 *sig_low_7_dataset = new TF1((energy_str + "GeV_sig_low_7_dataset").data(), "[0]", can_dataset->GetUxmin(), can_dataset->GetUxmax());
//	sig_low_7_dataset->SetParameter(0, set_avg - 7 * set_err);
//	sig_low_7_dataset->SetLineColor(kOrange+9); sig_low_7_dataset->Draw("Same");
//	TF1 *sig_high_7_dataset = new TF1((energy_str + "GeV_sig_high_7_dataset").data(), "[0]", can_dataset->GetUxmin(), can_dataset->GetUxmax());
//	sig_high_7_dataset->SetParameter(0, set_avg + 7 * set_err);
//	sig_high_7_dataset->SetLineColor(kOrange+9); sig_high_7_dataset->Draw("Same");
//	can_dataset->Write();

	TCanvas *can_index_dataset = new TCanvas((energy_str+"GeV_Dca_xy_vs_Run_Index_Can").data(), (energy_str+"GeV Dca XY vs Run Index").data());
	TGraphErrors* dca_xy_index_dataset = new TGraphErrors((int)run_indexes.size(), run_indexes.data(), dca_xy_run_avg.data(), 0, dca_xy_run_err.data());
	dca_xy_index_dataset->SetNameTitle((energy_str+"GeV_Dca_xy_vs_Run_Index_Graph").data(), (energy_str+"GeV Dca XY vs Run Index").data());
	dca_xy_index_dataset->SetMarkerStyle(kOpenCircle);
	dca_xy_index_dataset->GetXaxis()->SetTitle("Run Index");
	dca_xy_index_dataset->GetYaxis()->SetTitle("Dca_XY Run Average");
	dca_xy_index_dataset->Draw("APZ");
	can_index_dataset->Update();
	TF1 *avg_index_dataset = new TF1((energy_str + "GeV_avg_index_dataset").data(), "[0]", can_index_dataset->GetUxmin(), can_index_dataset->GetUxmax());
	avg_index_dataset->SetParameter(0, set_avg);
	avg_index_dataset->SetLineColor(kGreen+2); avg_index_dataset->Draw("Same");
	TF1 *sig_low_3_index_dataset = new TF1((energy_str + "GeV_sig_low_3_index_dataset").data(), "[0]", can_index_dataset->GetUxmin(), can_index_dataset->GetUxmax());
	sig_low_3_index_dataset->SetParameter(0, set_avg - 3 * set_err);
	sig_low_3_index_dataset->SetLineColor(kOrange+7); sig_low_3_index_dataset->Draw("Same");
	TF1 *sig_high_3_index_dataset = new TF1((energy_str + "GeV_sig_high_3_index_dataset").data(), "[0]", can_index_dataset->GetUxmin(), can_index_dataset->GetUxmax());
	sig_high_3_index_dataset->SetParameter(0, set_avg + 3 * set_err);
	sig_high_3_index_dataset->SetLineColor(kOrange+7); sig_high_3_index_dataset->Draw("Same");
	TF1 *sig_low_7_index_dataset = new TF1((energy_str + "GeV_sig_low_7_index_dataset").data(), "[0]", can_index_dataset->GetUxmin(), can_index_dataset->GetUxmax());
	sig_low_7_index_dataset->SetParameter(0, set_avg - 7 * set_err);
	sig_low_7_index_dataset->SetLineColor(kOrange+9); sig_low_7_index_dataset->Draw("Same");
	TF1 *sig_high_7_index_dataset = new TF1((energy_str + "GeV_sig_high_7_index_dataset").data(), "[0]", can_index_dataset->GetUxmin(), can_index_dataset->GetUxmax());
	sig_high_7_index_dataset->SetParameter(0, set_avg + 7 * set_err);
	sig_high_7_index_dataset->SetLineColor(kOrange+9); sig_high_7_index_dataset->Draw("Same");
	TLegend *leg_index_dataset = new TLegend();
	leg_index_dataset->SetBorderSize(0); leg_index_dataset->SetFillStyle(0);
	leg_index_dataset->AddEntry(avg_index_dataset, "Set Average", "l");
	leg_index_dataset->AddEntry(sig_low_3_index_dataset, "Set Avg +- 3 sigma", "l");
	leg_index_dataset->AddEntry(sig_low_7_index_dataset, "Set Avg +- 7 sigma", "l");
	leg_index_dataset->Draw();
	can_index_dataset->SetGrid();
	can_index_dataset->Write();


	TDirectory *runs = out_file->mkdir("Individual Runs");
	runs->cd();

	vector<TGraphErrors*> dca_xy_runs;
	vector<TGraph*> dca_xy_mv_avg;
	vector<TCanvas*> dca_xy_runs_can;

	int num_events = 0;
	int num_7sig_events = 0;
	int num_runs = 0;
	int num_7sig_runs = 0;
	int num_mv_avg_runs = 0;
	for(auto &run:dca_event) {
		num_runs++;
		float run_avg = dca_run[run.first][1];
		energy_str = to_string(energy);
		string run_str = to_string(run.first);

		vector<float> event_ids;
		vector<float> dca_xy_val;
		vector<float> dca_xy_err;
		bool flag = false;
		for(auto &event:run.second) {
			num_events++;
			float sigmas = (event.second.first - run_avg) / set_err;
			if(fabs(sigmas) > 7) {
				if(!flag) {
					flag = true;
					energy_str = "0Bad_" + energy_str;
					run_str += " Bad 7 sigma";
				}
				num_7sig_events++;
			}
			event_ids.push_back(event.first);
			dca_xy_val.push_back(event.second.first);
			dca_xy_err.push_back(event.second.second);
		}

		pair<vector<float>, vector<float>> mv_avg = moving_average(event_ids, dca_xy_val, mv_avg_num);
		bool mv_avg_flag = false;
		vector<pair<int, int>> bad_ranges;
		for(int i = 0; i < (int)mv_avg.second.size(); i++) {  // Depends on mv_avg.first being sorted ascending
			float sigmas = (mv_avg.second[i] - run_avg) / set_err;
			if(fabs(sigmas) > mv_avg_sigmas) {
				int low = i;
				float sigmas_check = sigmas;
				while(fabs(sigmas_check) >= mv_avg_sigmas_relax) {
					if(--low <= 0) { break; }
					sigmas_check = (mv_avg.second[low] - run_avg) / set_err;
				}
				if(low > 0) { low = floor(mv_avg.first[low]); }
				else { low = 0; }  // If first point then leave low = 0

				int high = i;
				sigmas_check = sigmas;
				while(fabs(sigmas_check) >= mv_avg_sigmas_relax) {
					if(!(++high < (int)mv_avg.second.size())) { break; }
					sigmas_check = (mv_avg.second[high] - run_avg) / set_err;
				}
				if(high >= (int)mv_avg.second.size()) {
					high = -1;  // If last point then set to -1 as a flag.
					i = 1 + (int)mv_avg.second.size();  // Want to break at end of loop
				} else {
					i = 1 + high;  // Skip the range already check and found to be bad
					high = ceil(mv_avg.first[high]);
				}

				bad_ranges.push_back(make_pair(low, high));

				if(!mv_avg_flag) { energy_str = "0Bad_" + energy_str; run_str += " Bad Mv Avg"; mv_avg_flag = true; }
			}
		}

		pair<float, float> y_range {-1.5, 0.3};

		dca_xy_runs.push_back(new TGraphErrors((int)event_ids.size(), event_ids.data(), dca_xy_val.data(), 0, dca_xy_err.data()));
		dca_xy_runs.back()->SetNameTitle((energy_str + "GeV_Graph_Run_"+run_str).data(), ("Run "+run_str).data());
		dca_xy_runs.back()->SetMarkerStyle(kOpenCircle);
		dca_xy_runs.back()->GetXaxis()->SetTitle("Event Number");
		dca_xy_runs.back()->GetYaxis()->SetTitle("Dca_XY Event Average");
		dca_xy_runs_can.push_back(new TCanvas((energy_str + "GeV_Can_Run_"+run_str).data(), ("Run "+run_str).data()));
		dca_xy_runs.back()->GetYaxis()->SetRangeUser(y_range.first, y_range.second);
		dca_xy_runs.back()->Draw("APZ");
		dca_xy_runs_can.back()->Update();

		for(pair<int, int> bad_pair:bad_ranges) {
			int r = bad_pair.second == -1 ? dca_xy_runs_can.back()->GetUxmax() : bad_pair.second;
			TLine *left = new TLine(bad_pair.first, y_range.first*0.9, bad_pair.first, y_range.second*0.9);
			TLine *right = new TLine(r, y_range.first*0.9, r, y_range.second*0.9);
			TLine *top = new TLine(bad_pair.first, y_range.second*0.9, r, y_range.second*0.9);
			TLine *bottom = new TLine(bad_pair.first, y_range.first*0.9, r, y_range.first*0.9);
			left->SetLineColor(kViolet); right->SetLineColor(kViolet); top->SetLineColor(kViolet); bottom->SetLineColor(kViolet);
			left->Draw("SAMEL"); right->Draw("SAMEL"); top->Draw("SAMEL"); bottom->Draw("SAMEL");
		}

		TF1 *avg_set = new TF1((energy_str + "GeV_SetAvg_Run_"+run_str).data(), "[0]", dca_xy_runs_can.back()->GetUxmin(), dca_xy_runs_can.back()->GetUxmax());
		avg_set->SetParameter(0, set_avg); avg_set->SetLineColor(kGreen+2); avg_set->Draw("Same");
		TF1 *avg_run = new TF1((energy_str + "GeV_RunAvg_Run_"+run_str).data(), "[0]", dca_xy_runs_can.back()->GetUxmin(), dca_xy_runs_can.back()->GetUxmax());
		avg_run->SetParameter(0, run_avg); avg_run->SetLineColor(kBlue); avg_run->Draw("Same");
		TF1 *sig_low_mv_avg = new TF1((energy_str + "GeV_sig_low_mv_avg_Run_"+run_str).data(), "[0]", dca_xy_runs_can.back()->GetUxmin(), dca_xy_runs_can.back()->GetUxmax());
		sig_low_mv_avg->SetParameter(0, run_avg - mv_avg_sigmas * set_err); sig_low_mv_avg->SetLineColor(kOrange+7); sig_low_mv_avg->Draw("Same");
		TF1 *sig_high_mv_avg = new TF1((energy_str + "GeV_sig_high_mv_avg_Run_"+run_str).data(), "[0]", dca_xy_runs_can.back()->GetUxmin(), dca_xy_runs_can.back()->GetUxmax());
		sig_high_mv_avg->SetParameter(0, run_avg + mv_avg_sigmas * set_err); sig_high_mv_avg->SetLineColor(kOrange+7); sig_high_mv_avg->Draw("Same");
		TF1 *sig_low_7 = new TF1((energy_str + "GeV_sig_low_7_Run_"+run_str).data(), "[0]", dca_xy_runs_can.back()->GetUxmin(), dca_xy_runs_can.back()->GetUxmax());
		sig_low_7->SetParameter(0, run_avg - 7 * set_err); sig_low_7->SetLineColor(kOrange+9); sig_low_7->Draw("Same");
		TF1 *sig_high_7 = new TF1((energy_str + "GeV_sig_high_7_Run_"+run_str).data(), "[0]", dca_xy_runs_can.back()->GetUxmin(), dca_xy_runs_can.back()->GetUxmax());
		sig_high_7->SetParameter(0, run_avg + 7 * set_err); sig_high_7->SetLineColor(kOrange+9); sig_high_7->Draw("Same");
		dca_xy_mv_avg.push_back(new TGraph((int)mv_avg.first.size(), mv_avg.first.data(), mv_avg.second.data()));
		dca_xy_mv_avg.back()->SetLineColor(kRed); dca_xy_mv_avg.back()->SetLineWidth(2);
		dca_xy_mv_avg.back()->Draw("SAMEL");
		TLegend *leg = new TLegend();
		leg->SetBorderSize(0); leg->SetFillStyle(0);
		leg->AddEntry(avg_set, "Set Average", "l");
		leg->AddEntry(avg_run, "Run Average", "l");
		leg->AddEntry(sig_low_mv_avg, ("Set Avg +- " + to_string(mv_avg_sigmas) +" sigma").data(), "l");
		leg->AddEntry(sig_low_7, "Set Avg +- 7 sigma", "l");
		leg->AddEntry(dca_xy_mv_avg.back(), (to_string(mv_avg_num) + "pt Moving Average").data(), "l");
		leg->Draw();
		dca_xy_runs_can.back()->SetGrid();
		dca_xy_runs_can.back()->Write();
		if(flag) { num_7sig_runs++; }
		if(mv_avg_flag) { num_mv_avg_runs++; }
	}
	cout << endl << energy << "GeV:" << endl;
	cout << "Runs: " << num_runs << endl;
	cout << "7sig runs: " << num_7sig_runs << endl;
	cout << "Percentage of runs with bad event: " << ((float)num_7sig_runs) / num_runs * 100 << "%" << endl;
	cout << "mv avg runs: " << num_mv_avg_runs << endl;
	cout << "Percentage of runs with bad moving average: " << ((float)num_mv_avg_runs) / num_runs * 100 << "%" << endl;
	cout << "Events: " << num_events << endl;
	cout << "7sig events: " << num_7sig_events << endl;
	cout << "Percentage of events bad: " << ((float)num_7sig_events) / num_events * 100 << "%" << endl;

	out_file->Close();

//	for(auto &graph:dca_xy_runs) { graph->Write(); }
}


pair<vector<float>, vector<float>> moving_average(vector<float> x, vector<float> y, int n_point) {
	vector<float> x_avg;
	vector<float> y_avg;
	if(x.size() != y.size()) { cout << "Moving average mismatch in vector sizes!" << endl; }
	else {
		float x_temp = 0., y_temp = 0.;
		for(int index=0; index < (int) x.size(); index++) {
			x_temp += x[index]; y_temp += y[index];
			if(index >= n_point) {
				x_temp -= x[index - n_point]; y_temp -= y[index - n_point];
				x_avg.push_back(x_temp / n_point); y_avg.push_back(y_temp / n_point);
			}
		}
	}

	return make_pair(x_avg, y_avg);
}
