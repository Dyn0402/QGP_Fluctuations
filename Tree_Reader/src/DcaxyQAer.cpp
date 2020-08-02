/*
 * Dca_xy_QAer.cpp
 *
 *  Created on: Jul 2, 2020
 *      Author: Dylan Neff
 */


#include "DcaxyQAer.h"


// Structors

DcaxyQAer::DcaxyQAer(int energy, mutex *mtx) {
	start_sys = chrono::system_clock::now();
	this->energy = energy;
	out_file = NULL;
	refmultCorrUtil = new StRefMultCorr("refmult3");
	this->mtx = mtx;
}

DcaxyQAer::DcaxyQAer(int energy) {
	start_sys = chrono::system_clock::now();
	this->energy = energy;
	out_file = NULL;
	refmultCorrUtil = new StRefMultCorr("refmult3");
}

DcaxyQAer::~DcaxyQAer() {
//	delete refmultCorrUtil;
}


// Getters

vector<int> DcaxyQAer::get_bad_runs() {
	if(bad_runs.size() == 0) {
		read_bad_dca_file();
	}

	return bad_runs;
}


map<int, vector<pair<int, int>>> DcaxyQAer::get_bad_ranges() {
	if(bad_ranges.size() == 0) {
		read_bad_dca_file();
	}

	return bad_ranges;
}


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
	write_bad_dca_file();
}


void DcaxyQAer::read_trees() {
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


void DcaxyQAer::read_tree(TTree* tree) {
	tree_leaves leaves = get_tree_leaves(tree, "Proton", 3);
	set_branches(tree);

	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		event_index++;
		unsigned run = leaves.run->GetValue();

		refmultCorrUtil->init(run);
		refmultCorrUtil->initEvent(leaves.ref_multn->GetValue(), (double)leaves.vz->GetValue());
		int cent9_corr = refmultCorrUtil->getCentralityBin9();

		if(cent9_corr < cent_min) { continue; }

		unsigned event_id = leaves.event_id->GetValue();
		float dca_xy_avg = leaves.dca_xy_avg->GetValue();
		float dca_xy_err = leaves.dca_xy_err->GetValue();

		if(dca_xy_avg == -899 && dca_xy_err == -899) {
			continue;
		}

		dca_event[run][event_id] = {dca_xy_avg, dca_xy_err};
		if(dca_run.find(run) == dca_run.end()) {
			dca_run[run] = {0, 0, 0};
		}
		dca_run[run][0]++; set_count++;
		dca_run[run][1] += dca_xy_avg; set_avg += dca_xy_avg;
		dca_run[run][2] += pow(dca_xy_avg, 2); set_err += pow(dca_xy_avg, 2);
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

	// Calculate mean and standard error of dca_xy for the entire set
	set_avg /= set_count; set_err = pow(set_err / set_count - pow(set_avg, 2), 0.5);

	// Calculate mean and stanandard error of dca_xy for each run
	vector<float> run_num, run_indexes, dca_xy_run_avg, dca_xy_run_err;
	int run_index = 1;
	for(auto &run:dca_run) {
		run.second[1] /= run.second[0];
		run.second[2] = pow(run.second[2] / run.second[0] - pow(run.second[1], 2), 0.5);
		run_num.push_back(run.first);
		run_indexes.push_back(run_index++);
		dca_xy_run_avg.push_back(run.second[1]);
		dca_xy_run_err.push_back(run.second[2]);
	}

	out_file = new TFile((out_path+"Dca_xy_qa_"+to_string(energy)+"_cent_"+to_string(cent_min)+"-8.root").data(), "RECREATE");
	out_file->cd();

	plot_run(run_num, dca_xy_run_avg, dca_xy_run_err, run_indexes);


	TDirectory *bad_runs_dir = out_file->mkdir("Bad Runs");
	TDirectory *good_runs_dir = out_file->mkdir("Good Runs");
	good_runs_dir->cd();

	int num_events = 0;
	int num_7sig_events = 0;
	int num_runs = 0;
	int num_7sig_runs = 0;
	int num_mv_avg_runs = 0;
	for(auto &run:dca_event) {
		num_runs++;
		float run_avg = dca_run[run.first][1];
		string run_str = to_string(run.first);

		if(check_bad_run(run_avg)) { bad_runs.push_back(run.first); }

		vector<float> event_ids;
		vector<float> dca_xy_val;
		vector<float> dca_xy_err;
		vector<float> event_ids_fit;
		vector<float> dca_xy_val_fit;
		vector<float> dca_xy_err_fit;
		bool flag = false;
		for(auto &event:run.second) {
			num_events++;
			float sigmas = (event.second.first - set_avg) / set_err;
			if(fabs(sigmas) > 7) {
				if(!flag) {
					flag = true;
					run_str += " Bad 7 sigma";
				}
				num_7sig_events++;
			}
			sigmas = (event.second.first - run_avg) / set_err;
			if(fabs(sigmas) < n_sigma_fit) {
				event_ids_fit.push_back(event.first);
				dca_xy_val_fit.push_back(event.second.first);
				dca_xy_err_fit.push_back(event.second.second);
			}

			event_ids.push_back(event.first);
			dca_xy_val.push_back(event.second.first);
			dca_xy_err.push_back(event.second.second);
		}

		TF1 lin_fit(("Run"+run_str+"lin_fit").data(), "pol1", event_ids.front(), event_ids.back());
		lin_fit.SetLineColor(kBlue);
		if((int)event_ids_fit.size() > min_points_fit) {
			TGraphErrors fit_graph((int)event_ids_fit.size(), event_ids_fit.data(), dca_xy_val_fit.data(), 0, dca_xy_err_fit.data());
			fit_graph.Fit(&lin_fit, "NQ");
		} else {
			lin_fit.SetParameters(set_avg, 0);
		}

		vector<vector<pair<int, int>>> mv_avg_bad_ranges;

		for(pair<int, pair<float, float>> mv_avg_num:mv_avg_pars) {
			pair<vector<float>, vector<float>> mv_avg = moving_average(event_ids, dca_xy_val, mv_avg_num.first);
			float sigmas_thresh = -convert_sigmas(-mv_avg_num.second.first, mv_avg_num.first);
			float sigmas_relax = -convert_sigmas(-mv_avg_num.second.second, mv_avg_num.first);
			vector<pair<int, int>> new_ranges = get_npt_bad_ranges(mv_avg, lin_fit, set_err, sigmas_thresh, sigmas_relax);
			if(new_ranges.size() > 0) {mv_avg_stats[mv_avg_num.first]++; run_str += " " + to_string(mv_avg_num.first) + " "; }
			mv_avg_bad_ranges.push_back(new_ranges);
		}

		vector<pair<int, int>> combined_bad_ranges = combine_ranges(mv_avg_bad_ranges);

		TDirectory *run_dir;
		if(combined_bad_ranges.size() > 0) {  // There is a bad range
			run_dir = bad_runs_dir->mkdir(("Run " + run_str).data());
			if(find(bad_runs.begin(), bad_runs.end(), run.first) == bad_runs.end()) {  // If not a bad run add ranges to bad_ranges
				bad_ranges[run.first] = combined_bad_ranges;
				num_mv_avg_runs++;
			}
		} else {
			run_dir = good_runs_dir->mkdir(("Run " + run_str).data());
		}
		run_dir->cd();

		for(pair<int, pair<float, float>> mv_avg_num:mv_avg_pars) {  // Huge waste just to get bad run status on TDirectory name but apparently faster than available options to rename TDirectory
			pair<vector<float>, vector<float>> mv_avg = moving_average(event_ids, dca_xy_val, mv_avg_num.first);
			float sigmas_thresh = -convert_sigmas(-mv_avg_num.second.first, mv_avg_num.first);
			float sigmas_relax = -convert_sigmas(-mv_avg_num.second.second, mv_avg_num.first);
			vector<pair<int, int>> new_ranges = get_npt_bad_ranges(mv_avg, lin_fit, set_err, sigmas_thresh, sigmas_relax);
			plot_mv_avg(event_ids, dca_xy_val, dca_xy_err, mv_avg, new_ranges, lin_fit, run_avg, sigmas_thresh, mv_avg_num.first, run.first);
		}

		plot_final(event_ids, dca_xy_val, dca_xy_err, combined_bad_ranges, lin_fit, run_avg, run.first);
		run_dir->Close();


		if(flag) { num_7sig_runs++; }
	}
	cout << endl << energy << "GeV:" << endl;
	good_runs_dir->Close(); bad_runs_dir->Close();
	cout << "Runs: " << num_runs << endl;
	cout << "7sig runs: " << num_7sig_runs << endl;
	cout << "Percentage of runs with bad event: " << ((float)num_7sig_runs) / num_runs * 100 << "%" << endl;
	cout << "mv avg runs: " << num_mv_avg_runs << endl;
	cout << "Percentage of runs with bad moving average: " << ((float)num_mv_avg_runs) / num_runs * 100 << "%" << endl;
	cout << "Events: " << num_events << endl;
	cout << "7sig events: " << num_7sig_events << endl;
	cout << "Percentage of events bad: " << ((float)num_7sig_events) / num_events * 100 << "%" << endl;
	cout << "Moving Average Stats: " << endl;
	for(pair<int, int> mv_avg_stat:mv_avg_stats) {
		cout << " " << mv_avg_stat.first << "pt: " << mv_avg_stat.second << " runs" << endl;
	}

	out_file->Close();
}


bool DcaxyQAer::check_bad_run(float run_avg) {
	float sigmas = (run_avg - set_avg) / set_err;
	if(fabs(sigmas) > bad_run_sigmas[energy]) {
		return true;
	} else { return false; }
}


vector<pair<int, int>> DcaxyQAer::get_npt_bad_ranges(const pair<vector<float>, vector<float>> &mv_avg, TF1 &lin_fit, float sigma, float sigma_thresh, float sigma_relax) {
	vector<pair<int, int>> bad_ranges;

	for(int i = 0; i < (int)mv_avg.second.size(); i++) {  // Depends on mv_avg.first being sorted ascending
		float sigmas = (mv_avg.second[i] - lin_fit.Eval(mv_avg.first[i])) / sigma;
		if(fabs(sigmas) > sigma_thresh) {
			int low = i;
			float sigmas_check = sigmas;
			while(sigmas_check * TMath::Sign(1, sigmas) >= sigma_relax) {
				if(--low <= 0) { break; }
				sigmas_check = (mv_avg.second[low] - lin_fit.Eval(mv_avg.first[low])) / sigma;
			}
			// Check if low is the first event or within tolerance of the first event
			if(low == 0 || mv_avg.first[low] - range_comb_tol <= 0) {
				low = 0;
			} else { low = floor(mv_avg.first[low]); }

			int high = i;
			sigmas_check = sigmas;
			while(sigmas_check * TMath::Sign(1, sigmas) >= sigma_relax) {
				if(!(++high < (int)mv_avg.second.size())) { break; }
				sigmas_check = (mv_avg.second[high] - lin_fit.Eval(mv_avg.first[high])) / sigma;
			}
			// Check if high is the last event or within tolerance of the last event
			if(high >= (int)mv_avg.second.size() || mv_avg.first[high] + range_comb_tol >= mv_avg.first.back()) {
				high = numeric_limits<int>::max();  // If last point then set to max integer.
				i = 1 + (int)mv_avg.second.size();  // Want to break at end of loop
			} else {
				i = 1 + high;  // Skip the range already checked and found to be bad
				high = ceil(mv_avg.first[high]);
			}

			bad_ranges.push_back(make_pair(low, high));
		}
	}

	return bad_ranges;
}


float convert_sigmas(float old_sigma, int new_points, int old_points) {
	double prob = pow(ROOT::Math::normal_cdf(old_sigma) * 2, (double)old_points/(double)new_points) / 2;
	return ROOT::Math::gaussian_quantile(prob, 1);
}


vector<pair<int, int>> DcaxyQAer::combine_ranges(const vector<vector<pair<int, int>>> &ranges) {
	vector<pair<int, int>> all_ranges;
	for(const vector<pair<int, int>> &range:ranges) {
		all_ranges.insert(all_ranges.end(), range.begin(), range.end());
	}
	sort(all_ranges.begin(), all_ranges.end());

	vector<pair<int, int>> combined_ranges;
	if(all_ranges.size() < 1) { return combined_ranges; }

	int start_index = all_ranges[0].first;
	int end_index = all_ranges[0].second;
	for(pair<int, int> &range:all_ranges) {
		if(end_index == numeric_limits<int>::max()) { break; }
		if(range.first <= end_index + range_comb_tol) {
			if(range.second > end_index) { end_index = range.second; }
		} else {
			combined_ranges.push_back(make_pair(start_index, end_index));
			start_index = range.first; end_index = range.second;
		}
	}
	combined_ranges.push_back(make_pair(start_index, end_index));

	return combined_ranges;
}


pair<vector<float>, vector<float>> moving_average(const vector<float> &x, const vector<float> &y, int n_point) {
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


void DcaxyQAer::plot_run(vector<float> &run_num, vector<float> &dca_xy_run_avg, vector<float> &dca_xy_run_err, vector<float> &run_indexes) {
	string energy_str = to_string(energy);

	if(mtx) mtx->lock();

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

	TCanvas can_index_dataset((energy_str+"GeV_Dca_xy_vs_Run_Index_Can").data(), (energy_str+"GeV Dca XY vs Run Index").data());
	TGraphErrors dca_xy_index_dataset((int)run_indexes.size(), run_indexes.data(), dca_xy_run_avg.data(), 0, dca_xy_run_err.data());
	dca_xy_index_dataset.SetNameTitle((energy_str+"GeV_Dca_xy_vs_Run_Index_Graph").data(), (energy_str+"GeV Dca XY vs Run Index").data());
	dca_xy_index_dataset.SetMarkerStyle(kOpenCircle);
	dca_xy_index_dataset.GetXaxis()->SetTitle("Run Index");
	dca_xy_index_dataset.GetYaxis()->SetTitle("Dca_XY Run Average");
	dca_xy_index_dataset.Draw("APZ");
	can_index_dataset.Update();
	TF1 avg_index_dataset((energy_str + "GeV_avg_index_dataset").data(), "[0]", can_index_dataset.GetUxmin(), can_index_dataset.GetUxmax());
	avg_index_dataset.SetParameter(0, set_avg);
	avg_index_dataset.SetLineColor(kGreen+2); avg_index_dataset.Draw("Same");
	TF1 sig_low_3_index_dataset((energy_str + "GeV_sig_low_3_index_dataset").data(), "[0]", can_index_dataset.GetUxmin(), can_index_dataset.GetUxmax());
	sig_low_3_index_dataset.SetParameter(0, set_avg - 3 * set_err);
	sig_low_3_index_dataset.SetLineColor(kOrange+7); sig_low_3_index_dataset.Draw("Same");
	TF1 sig_high_3_index_dataset((energy_str + "GeV_sig_high_3_index_dataset").data(), "[0]", can_index_dataset.GetUxmin(), can_index_dataset.GetUxmax());
	sig_high_3_index_dataset.SetParameter(0, set_avg + 3 * set_err);
	sig_high_3_index_dataset.SetLineColor(kOrange+7); sig_high_3_index_dataset.Draw("Same");
	TF1 sig_low_7_index_dataset((energy_str + "GeV_sig_low_7_index_dataset").data(), "[0]", can_index_dataset.GetUxmin(), can_index_dataset.GetUxmax());
	sig_low_7_index_dataset.SetParameter(0, set_avg - 7 * set_err);
	sig_low_7_index_dataset.SetLineColor(kOrange+9); sig_low_7_index_dataset.Draw("Same");
	TF1 sig_high_7_index_dataset((energy_str + "GeV_sig_high_7_index_dataset").data(), "[0]", can_index_dataset.GetUxmin(), can_index_dataset.GetUxmax());
	sig_high_7_index_dataset.SetParameter(0, set_avg + 7 * set_err);
	sig_high_7_index_dataset.SetLineColor(kOrange+9); sig_high_7_index_dataset.Draw("Same");
	TLegend leg_index_dataset;
	leg_index_dataset.SetBorderSize(0); leg_index_dataset.SetFillStyle(0);
	leg_index_dataset.AddEntry(&avg_index_dataset, "Set Average", "l");
	leg_index_dataset.AddEntry(&sig_low_3_index_dataset, "Set Avg +- 3 sigma", "l");
	leg_index_dataset.AddEntry(&sig_low_7_index_dataset, "Set Avg +- 7 sigma", "l");
	leg_index_dataset.Draw();
	can_index_dataset.SetGrid();
	can_index_dataset.Write();

	if(mtx) mtx->unlock();
}


void DcaxyQAer::plot_mv_avg(const vector<float> &event_ids, const vector<float> &dca_xy_val, const vector<float> &dca_xy_err, const pair<vector<float>, vector<float>> &mv_avg, const vector<pair<int, int>> &bad_ranges, TF1 &lin_fit, float run_avg, float sigmas_thresh, int mv_avg_num, int run_num) {
	string energy_str = to_string(energy), run_str = to_string(run_num), mv_pt_str = to_string(mv_avg_num);

	if(mtx) mtx->lock();

	TCanvas can((mv_pt_str + "pt_" + energy_str + "GeV_Can_Run_"+run_str).data(), ("Run "+run_str).data());
	TGraphErrors run_graph((int)event_ids.size(), event_ids.data(), dca_xy_val.data(), 0, dca_xy_err.data());
	run_graph.SetNameTitle((energy_str + "GeV_Graph_Run_"+run_str).data(), (mv_pt_str + " Point Moving Average " + energy_str + "GeV Run "+run_str).data());
	run_graph.SetMarkerStyle(kOpenCircle);
	run_graph.GetXaxis()->SetTitle("Event Number");
	run_graph.GetYaxis()->SetTitle("Dca_XY Event Average");
	run_graph.GetYaxis()->SetRangeUser(y_range.first, y_range.second);
	run_graph.Draw("APZ");
	can.Update();

	vector<TLine> lines;
	for(pair<int, int> bad_pair:bad_ranges) {
		int r = bad_pair.second > can.GetUxmax() ? can.GetUxmax() : bad_pair.second;
		TLine left(bad_pair.first, y_range.first*0.9, bad_pair.first, y_range.second*0.9);
		TLine right(r, y_range.first*0.9, r, y_range.second*0.9);
		TLine top(bad_pair.first, y_range.second*0.9, r, y_range.second*0.9);
		TLine bottom(bad_pair.first, y_range.first*0.9, r, y_range.first*0.9);
		left.SetLineColor(kViolet); right.SetLineColor(kViolet); top.SetLineColor(kViolet); bottom.SetLineColor(kViolet);
		lines.push_back(left); lines.push_back(right); lines.push_back(top); lines.push_back(bottom);
	} for(TLine &line:lines) { line.Draw("SAMEL"); }

	lin_fit.SetRange(can.GetUxmin(), can.GetUxmax()); lin_fit.Draw("Same");
	TF1 avg_set((energy_str + "GeV_SetAvg_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	avg_set.SetParameter(0, set_avg); avg_set.SetLineColor(kGreen+2); avg_set.SetLineStyle(2); avg_set.Draw("Same");
	TF1 avg_run((energy_str + "GeV_RunAvg_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	avg_run.SetParameter(0, run_avg); avg_run.SetLineColor(kCyan-7); avg_run.SetLineStyle(2); avg_run.Draw("Same");
	TF1 sig_low_mv_avg((energy_str + "GeV_sig_low_mv_avg_Run_"+run_str).data(), "pol1", can.GetUxmin(), can.GetUxmax());
	sig_low_mv_avg.SetParameters(lin_fit.GetParameter(0) - sigmas_thresh * set_err, lin_fit.GetParameter(1));
	sig_low_mv_avg.SetLineColor(kOrange+7); sig_low_mv_avg.Draw("Same");
	TF1 sig_high_mv_avg((energy_str + "GeV_sig_high_mv_avg_Run_"+run_str).data(), "pol1", can.GetUxmin(), can.GetUxmax());
	sig_high_mv_avg.SetParameters(lin_fit.GetParameter(0) + sigmas_thresh * set_err, lin_fit.GetParameter(1));
	sig_high_mv_avg.SetLineColor(kOrange+7); sig_high_mv_avg.Draw("Same");
	TF1 sig_low_7((energy_str + "GeV_sig_low_7_Run_"+run_str).data(), "pol1", can.GetUxmin(), can.GetUxmax());
	sig_low_7.SetParameters(lin_fit.GetParameter(0) - 7 * set_err, lin_fit.GetParameter(1));
	sig_low_7.SetLineColor(kOrange+9); sig_low_7.Draw("Same");
	TF1 sig_high_7((energy_str + "GeV_sig_high_7_Run_"+run_str).data(), "pol1", can.GetUxmin(), can.GetUxmax());
	sig_high_7.SetParameters(lin_fit.GetParameter(0) + 7 * set_err, lin_fit.GetParameter(1));
	sig_high_7.SetLineColor(kOrange+9); sig_high_7.Draw("Same");
	TGraph dca_xy_mv_avg((int)mv_avg.first.size(), mv_avg.first.data(), mv_avg.second.data());
	dca_xy_mv_avg.SetName((energy_str + "_mv_avg").data());
	dca_xy_mv_avg.SetLineColor(kRed); dca_xy_mv_avg.SetLineWidth(2);
	dca_xy_mv_avg.Draw("SAMEL");

	TLegend leg;
	leg.SetBorderSize(0); leg.SetFillStyle(0);
	leg.AddEntry(&avg_set, "Set Average", "l");
	leg.AddEntry(&avg_run, "Run Average", "l");
	leg.AddEntry(&lin_fit, "Linear Baseline", "l");
	leg.AddEntry(&sig_low_mv_avg, ("Set Avg +- " + to_string(sigmas_thresh) +" sigma").data(), "l");
	leg.AddEntry(&sig_low_7, "Set Avg +- 7 sigma", "l");
	leg.AddEntry(&dca_xy_mv_avg, (to_string(mv_avg_num) + "pt Moving Average").data(), "l");
	leg.Draw();
	can.SetGrid();
	can.Write();

	if(mtx) mtx->unlock();
}


void DcaxyQAer::plot_final(const vector<float> &event_ids, const vector<float> &dca_xy_val, const vector<float> &dca_xy_err, const vector<pair<int, int>> &bad_ranges, TF1 &lin_fit, float run_avg, int run_num) {
	string energy_str = to_string(energy), run_str = to_string(run_num);

	if(mtx) mtx->lock();

	TCanvas can(("Final_"+ energy_str + "GeV_Can_Run_"+run_str+"_final").data(), ("Run "+run_str).data());
	TGraphErrors run_graph((int)event_ids.size(), event_ids.data(), dca_xy_val.data(), 0, dca_xy_err.data());
	run_graph.SetNameTitle((energy_str + "GeV_Graph_Run_"+run_str).data(), ("Final " + energy_str + "GeV Run "+run_str).data());
	run_graph.SetMarkerStyle(kOpenCircle);
	run_graph.GetXaxis()->SetTitle("Event Number");
	run_graph.GetYaxis()->SetTitle("Dca_XY Event Average");
	run_graph.GetYaxis()->SetRangeUser(y_range.first, y_range.second);
	run_graph.Draw("APZ");
	can.Update();

	vector<TLine> lines;
	for(pair<int, int> bad_pair:bad_ranges) {
		int r = bad_pair.second > can.GetUxmax() ? can.GetUxmax() : bad_pair.second;
		TLine left(bad_pair.first, y_range.first*0.9, bad_pair.first, y_range.second*0.9);
		TLine right(r, y_range.first*0.9, r, y_range.second*0.9);
		TLine top(bad_pair.first, y_range.second*0.9, r, y_range.second*0.9);
		TLine bottom(bad_pair.first, y_range.first*0.9, r, y_range.first*0.9);
		left.SetLineColor(kViolet); right.SetLineColor(kViolet); top.SetLineColor(kViolet); bottom.SetLineColor(kViolet);
		lines.push_back(left); lines.push_back(right); lines.push_back(top); lines.push_back(bottom);
	} for(TLine &line:lines) { line.Draw("SAMEL"); }

	lin_fit.SetRange(can.GetUxmin(), can.GetUxmax()); lin_fit.Draw("Same");
	TF1 avg_set((energy_str + "GeV_SetAvg_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	avg_set.SetParameter(0, set_avg); avg_set.SetLineColor(kGreen+2); avg_set.SetLineStyle(2); avg_set.Draw("Same");
	TF1 avg_run((energy_str + "GeV_RunAvg_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	avg_run.SetParameter(0, run_avg); avg_run.SetLineColor(kCyan-7); avg_run.SetLineStyle(2); avg_run.Draw("Same");
	TF1 sig_low_7((energy_str + "GeV_sig_low_7_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	sig_low_7.SetParameter(0, run_avg - 7 * set_err); sig_low_7.SetLineColor(kOrange+9); sig_low_7.Draw("Same");
	TF1 sig_high_7((energy_str + "GeV_sig_high_7_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	sig_high_7.SetParameter(0, run_avg + 7 * set_err); sig_high_7.SetLineColor(kOrange+9); sig_high_7.Draw("Same");
	TF1 sig_low_fit((energy_str + "GeV_sig_low_fit_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	sig_low_fit.SetParameter(0, run_avg - n_sigma_fit * set_err); sig_low_fit.SetLineColor(kOrange+7); sig_low_fit.Draw("Same");
	TF1 sig_high_fit((energy_str + "GeV_sig_high_fit_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	sig_high_fit.SetParameter(0, run_avg + n_sigma_fit * set_err); sig_high_fit.SetLineColor(kOrange+7); sig_high_fit.Draw("Same");
	TF1 run_cut_low((energy_str + "GeV_run_cut_low_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	run_cut_low.SetParameter(0, run_avg - bad_run_sigmas[energy] * set_err); run_cut_low.SetLineColor(kRed); run_cut_low.Draw("Same");
	TF1 run_cut_high((energy_str + "GeV_run_cut_high_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	run_cut_high.SetParameter(0, run_avg + bad_run_sigmas[energy] * set_err); run_cut_high.SetLineColor(kRed); sig_high_fit.Draw("Same");

	TLegend leg;
	leg.SetBorderSize(0); leg.SetFillStyle(0);
	leg.AddEntry(&avg_set, "Set Average", "l");
	leg.AddEntry(&avg_run, "Run Average", "l");
	leg.AddEntry(&lin_fit, "Linear Baseline", "l");
	leg.AddEntry(&sig_low_7, "Run Avg +- 7 sigma", "l");
	leg.AddEntry(&sig_low_fit, ("Fit Range Run Avg +-" + to_string(n_sigma_fit) + " sigma").data(), "l");
	leg.AddEntry(&run_cut_low, ("Bad Run Boundary Run Avg +-" + to_string(bad_run_sigmas[energy]) + " sigma").data(), "l");
	leg.Draw();
	can.SetGrid();
	can.Write();

	if(mtx) mtx->unlock();
}


void DcaxyQAer::write_bad_dca_file() {
	if(!check_path(out_path)) { cout << "Can't access path: " << out_path << " Skipping following read/write." << endl; return; }

	string out_name = out_path + to_string(energy) + "GeV" + out_file_suf;
	ofstream out_txt(out_name);
	if(!out_txt.is_open()) { cout << "Could not open " << out_name << " Not writing." << endl; return; }
	out_txt << energy << "GeV Bad DcaXY Runs (run average dca_xy greater than " << bad_run_sigmas[energy] << " sigma from set average): " << endl;
	if(bad_runs.size() > 0) {
		bool first = true;
		for(int run:bad_runs) {
			if(!first) { out_txt << " " << flush; }
			out_txt << run;
			first = false;
		}
	}
	out_txt << endl << endl;

	out_txt << "Bad Ranges:";
	for(pair<int, vector<pair<int, int>>> run:bad_ranges) {
		out_txt << endl << run.first << "\t" << flush;
		bool first = true;
		for(pair<int, int> range:run.second) {
			if(!first) { out_txt << "\t" << flush; }
			out_txt << "[ " << range.first << " , " << range.second << " ]" << flush;
			first = false;
		}
	}

	out_txt.close();
}


void DcaxyQAer::read_bad_dca_file() {
	if(!check_path(out_path)) { cout << "Can't access path: " << out_path << " Skipping following read/write." << endl; return; }

	string in_name = out_path + to_string(energy) + "GeV" + out_file_suf;
	ifstream in_txt(in_name);
	if(!in_txt.is_open()) { cout << "Could not open " << in_name << " Not reading." << endl; return; }

	map<int, vector<pair<int, int>>> bad_ranges_read;
	vector<int> bad_runs_read;
	string line;
	int line_num = 1;
	while(getline(in_txt, line)) {
		if(line_num == 2) {  // Bad runs line
			vector<string> bad_run_strs = split(line, ' ');
			for(string &run:bad_run_strs) {
				bad_runs_read.push_back(stoi(run));
			}
		}

		else if(line_num >= 5) {  // Bad event ranges
			vector<string> bad_run_range_str = split(line, '\t');
			if(bad_run_range_str.size() >= 2) {
				int run = stoi(bad_run_range_str[0]);
				for(int range_index = 1; range_index < (int)bad_run_range_str.size(); range_index++) {
					vector<string> bad_range_str = split(bad_run_range_str[range_index], ' ');
					if(bad_range_str.size() == 5) {
						bad_ranges_read[run].push_back(make_pair(stoi(bad_range_str[1]), stoi(bad_range_str[3])));
					} else { cout << "Bad range run read" << endl; }
				}
			} else { cout << "Bad range line read" << endl; }
		}

		line_num++;
	}

	in_txt.close();

	if(bad_runs_read.size() > 0) { bad_runs = bad_runs_read; }
	if(bad_ranges_read.size() > 0) { bad_ranges = bad_ranges_read; }
}
