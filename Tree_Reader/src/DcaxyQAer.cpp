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
	cut.set_values(energy, "proton");

	// Pile Up Cuts
	PileUpQAer pile_up_qa(energy);
	cut.pile_up_low = pile_up_qa.get_low_cut();
	cut.pile_up_high = pile_up_qa.get_high_cut();
}

DcaxyQAer::DcaxyQAer(int energy) {
	start_sys = chrono::system_clock::now();
	this->energy = energy;
	out_file = NULL;
	refmultCorrUtil = new StRefMultCorr("refmult3");
	cut.set_values(energy, "proton");

	// Pile Up Cuts
	PileUpQAer pile_up_qa(energy);
	cut.pile_up_low = pile_up_qa.get_low_cut();
	cut.pile_up_high = pile_up_qa.get_high_cut();
}

DcaxyQAer::~DcaxyQAer() {
//	delete refmultCorrUtil;
}


// Getters


// Setters

void DcaxyQAer::set_in_path(string path) {
	in_path = path;
}


// Doers

void DcaxyQAer::run_qa() {
	read_trees();
	analyze_runs();
	make_proton_plots();
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
	set_tree_branches(tree, branches, particle, ref_num);
	set_branches(tree);

	int event_index = 0;
	while(tree->GetEntry(event_index)) {
		event_index++;
		unsigned run = branches.run_num;  //leaves.run->GetValue();

		refmultCorrUtil->init(run);
		refmultCorrUtil->initEvent(branches.refmultn, branches.vz);  //leaves.ref_multn->GetValue(), (double)leaves.vz->GetValue());
		int cent9_corr = refmultCorrUtil->getCentralityBin9();

		if(cent9_corr < pars.cent_min) { continue; }

		unsigned event_id = branches.event_id;  //leaves.event_id->GetValue();
		float dca_xy_avg = branches.dca_xy_avg;  //leaves.dca_xy_avg->GetValue();
		float dca_xy_err = branches.dca_xy_err;  //leaves.dca_xy_err->GetValue();

		centralities[run][event_id] = cent9_corr;

		if(dca_xy_avg == -899 && dca_xy_err == -899) { continue; }

		dca_event[run][event_id] = {dca_xy_avg, dca_xy_err};
		if(dca_run.find(run) == dca_run.end()) {
			dca_run[run] = {0, 0, 0};
		}
		dca_run[run][0]++; set_count++;
		dca_run[run][1] += dca_xy_avg; set_avg += dca_xy_avg;
		dca_run[run][2] += pow(dca_xy_avg, 2); set_err += pow(dca_xy_avg, 2);

		Event event(branches);

		if(check_event(event)) {
			vector<double> good_particle_angles = {};

			// Iterate over particles in event and add corresponding phi to good_particle_angles if particle good.
			for(Track& particle:event.get_particles()) {
				if(check_particle_good(particle)) {
					good_particle_angles.push_back(particle.get_phi());
				}
			}

			// If there are enough good particles, calculate ratios for each division and save to data.
			if(good_particle_angles.size() >= (unsigned)cut.min_multi) {
				protons[run][event_id] = (int)good_particle_angles.size();
			}
		}
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

	tree->SetBranchStatus("run_num", 1);
	tree->SetBranchStatus("event_id", 1);
	tree->SetBranchStatus("refmult", 1);
	tree->SetBranchStatus(("refmult"+to_string(ref_num)).data(), 1);
	tree->SetBranchStatus("btof_multi", 1);
	tree->SetBranchStatus("btof_match", 1);
	tree->SetBranchStatus("vz", 1);
	tree->SetBranchStatus("qx", 1);
	tree->SetBranchStatus("qy", 1);
	tree->SetBranchStatus((particle+".pt").data(), 1);
	tree->SetBranchStatus((particle+".phi").data(), 1);
	tree->SetBranchStatus((particle+".beta").data(), 1);
	tree->SetBranchStatus((particle+".charge").data(), 1);
	tree->SetBranchStatus((particle+".dca").data(), 1);
	tree->SetBranchStatus((particle+".nsigma").data(), 1);
	tree->SetBranchStatus((particle+".eta").data(), 1);
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

	out_file = new TFile((qa_path+"Dca_xy_qa_"+to_string(energy)+"_cent_"+to_string(pars.cent_min)+"-8.root").data(), "RECREATE");
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
			if(fabs(sigmas) < pars.n_sigma_fit) {
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
		if((int)event_ids_fit.size() > pars.min_points_fit) {
			TGraphErrors fit_graph((int)event_ids_fit.size(), event_ids_fit.data(), dca_xy_val_fit.data(), 0, dca_xy_err_fit.data());
			fit_graph.Fit(&lin_fit, "NQ");
		} else {
			lin_fit.SetParameters(set_avg, 0);
		}

		vector<vector<pair<int, int>>> mv_avg_bad_ranges;

		for(pair<int, pair<float, float>> mv_avg_num:pars.mv_avg_pars) {
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

		for(pair<int, pair<float, float>> mv_avg_num:pars.mv_avg_pars) {  // Huge waste just to get bad run status on TDirectory name but apparently faster than available options to rename TDirectory
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


void DcaxyQAer::make_proton_plots() {
	out_file = new TFile((qa_path+"Dca_xy_qa_"+to_string(energy)+"_cent_"+to_string(pars.cent_min)+"-8.root").data(), "UPDATE");
	out_file->cd();

	TDirectory *proton_dir = out_file->mkdir("Proton_Plots");
	proton_dir->cd();

	TH2F pre_proton_dca((to_string(energy)+"GeV_Pre_Proton_DCA").data(), (to_string(energy)+"GeV Protons vs sDCAxy").data(), 200, -1.5, 0.5, 51, -0.5, 50.5);
	pre_proton_dca.GetXaxis()->SetTitle("Dca_XY Event Average (cm)"); pre_proton_dca.GetYaxis()->SetTitle("Number of Protons in Event");
	TH2F good_proton_dca((to_string(energy)+"GeV_Good_Proton_DCA").data(), (to_string(energy)+"GeV Remaining Protons vs sDCAxy").data(), 200, -1.5, 0.5, 51, -0.5, 50.5);
	good_proton_dca.GetXaxis()->SetTitle("Dca_XY Event Average (cm)"); good_proton_dca.GetYaxis()->SetTitle("Number of Protons in Event");
	TH2F bad_proton_dca((to_string(energy)+"GeV_Bad_Proton_DCA").data(), (to_string(energy)+"GeV Removed Protons vs sDCAxy").data(), 200, -1.5, 0.5, 51, -0.5, 50.5);
	bad_proton_dca.GetXaxis()->SetTitle("Dca_XY Event Average (cm)"); bad_proton_dca.GetYaxis()->SetTitle("Number of Protons in Event");

	map<int, TH1F> pre_slices;
	for(pair<int, map<int, int>> const &run:protons) {
		for(pair<int, int> const &event_id:run.second) {
			if(centralities[run.first][event_id.first] != 8) { continue; }
			bool bad_dca_xy_event = false;
			if(find(bad_runs.begin(), bad_runs.end(), run.first) != bad_runs.end()) { bad_dca_xy_event = true; }
			else {
				for(pair<int, int> const &range:bad_ranges[run.first]) {
					if(event_id.first >= range.first && event_id.first <= range.second) { bad_dca_xy_event = true; }
				}
			}
			if(pre_slices.count(event_id.second) == 0) {
				pre_slices[event_id.second] = TH1F((to_string(energy)+"GeV_"+to_string(event_id.second)+"_Protons_sDCAxy_Slice").data(),
						(to_string(energy)+"GeV "+to_string(event_id.second)+" Proton sDCAxy Slice").data(), 200, -1.5, 0.5);
			}
			pre_slices[event_id.second].Fill(dca_event[run.first][event_id.first].first);
			pre_proton_dca.Fill(dca_event[run.first][event_id.first].first, event_id.second);
			if(bad_dca_xy_event) { bad_proton_dca.Fill(dca_event[run.first][event_id.first].first, event_id.second); }
			else { good_proton_dca.Fill(dca_event[run.first][event_id.first].first, event_id.second); }
		}
	}

	map<int, TF1> pre_slice_fits;
	for(pair<int, TH1F> slice:pre_slices) {
		if(slice.second.GetEntries() < pars.min_fit_entries) { continue; }
		float mean = slice.second.GetMean();
//		float sd = slice.second.GetStdDev();
		float max_count = slice.second.GetMaximum();
		float first_fit_bin = slice.second.GetBinCenter(slice.second.FindFirstBinAbove(pars.min_fit_count)-2);
		float last_fit_bin = slice.second.GetBinCenter(slice.second.FindLastBinAbove(pars.min_fit_count)+2);
		cout << slice.first << " protons  |  First bin: " << first_fit_bin << "  |  Last bin: " << last_fit_bin << endl;
		pre_slice_fits[slice.first] = TF1((to_string(energy)+"GeV_"+to_string(slice.first)+"_Protons_sDCAxy_Slice_Fit").data(),
				"gaus", first_fit_bin, last_fit_bin);
		pre_slice_fits[slice.first].SetParameters(max_count, mean, 0);
		slice.second.Fit(&pre_slice_fits[slice.first], "NQR");
		pre_slice_fits[slice.first].SetNpx(n_plot_points);
		pre_slice_fits[slice.first].SetRange(-1.5, 0.5);
	}
//	TF2 pre_gaus((to_string(energy)+"GeV_Pre_Gaus_Fit").data(), "xygaus", -0.2, 0.2, 0, 40);

	if(mtx) { mtx->lock(); }

	TCanvas pre_can((to_string(energy)+"GeV_Pre_Proton_DCA_Can").data(), (to_string(energy)+"GeV Protons vs DCA").data());
	pre_proton_dca.Draw("ColZ");
	pre_can.SetGrid(); pre_can.SetLogz();
	pre_can.Write();
	TCanvas good_can((to_string(energy)+"GeV_Good_Proton_DCA_Can").data(), (to_string(energy)+"GeV Remaining Protons vs DCA").data());
	good_proton_dca.Draw("ColZ");
	good_can.SetGrid(); good_can.SetLogz();
	good_can.Write();
	TCanvas bad_can((to_string(energy)+"GeV_Bad_Proton_DCA_Can").data(), (to_string(energy)+"GeV Removed Protons vs DCA").data());
	bad_proton_dca.Draw("ColZ");
	bad_can.SetGrid();
	bad_can.Write();

	TDirectory *proton_slice_dir = proton_dir->mkdir("Proton_sDCAxy_Slcies");
	proton_slice_dir->cd();
	for(pair<int, TF1> slice_fit:pre_slice_fits) {
		TCanvas slice_can((to_string(energy)+"GeV_"+to_string(slice_fit.first)+"_Protons_sDCAxy_Slice_Can").data(),
				(to_string(energy)+"GeV "+to_string(slice_fit.first)+" Proton sDCAxy Slice Can").data());
		pre_slices[slice_fit.first].Draw();
		slice_fit.second.Draw("Same");
		slice_can.Write();
	}

	if(mtx) { mtx->unlock(); }

	out_file->Close();
}


bool DcaxyQAer::check_bad_run(float run_avg) {
	float sigmas = (run_avg - set_avg) / set_err;
	if(fabs(sigmas) > pars.bad_run_sigmas[energy]) {
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
			if(low == 0 || mv_avg.first[low] - pars.range_comb_tol <= 0) {
				low = 0;
			} else { low = floor(mv_avg.first[low]); }

			int high = i;
			sigmas_check = sigmas;
			while(sigmas_check * TMath::Sign(1, sigmas) >= sigma_relax) {
				if(!(++high < (int)mv_avg.second.size())) { break; }
				sigmas_check = (mv_avg.second[high] - lin_fit.Eval(mv_avg.first[high])) / sigma;
			}
			// Check if high is the last event or within tolerance of the last event
			if(high >= (int)mv_avg.second.size() || mv_avg.first[high] + pars.range_comb_tol >= mv_avg.first.back()) {
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
		if(range.first <= end_index + pars.range_comb_tol) {
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
	TF1 sig_low_cut_index_dataset((energy_str + "GeV_sig_low_3_index_dataset").data(), "[0]", can_index_dataset.GetUxmin(), can_index_dataset.GetUxmax());
	sig_low_cut_index_dataset.SetParameter(0, set_avg - pars.bad_run_sigmas[energy] * set_err);
	sig_low_cut_index_dataset.SetLineColor(kOrange+7); sig_low_cut_index_dataset.Draw("Same");
	TF1 sig_high_cut_index_dataset((energy_str + "GeV_sig_high_3_index_dataset").data(), "[0]", can_index_dataset.GetUxmin(), can_index_dataset.GetUxmax());
	sig_high_cut_index_dataset.SetParameter(0, set_avg + pars.bad_run_sigmas[energy] * set_err);
	sig_high_cut_index_dataset.SetLineColor(kOrange+7); sig_high_cut_index_dataset.Draw("Same");
	TF1 sig_low_7_index_dataset((energy_str + "GeV_sig_low_7_index_dataset").data(), "[0]", can_index_dataset.GetUxmin(), can_index_dataset.GetUxmax());
	sig_low_7_index_dataset.SetParameter(0, set_avg - 7 * set_err);
	sig_low_7_index_dataset.SetLineColor(kOrange+9); sig_low_7_index_dataset.Draw("Same");
	TF1 sig_high_7_index_dataset((energy_str + "GeV_sig_high_7_index_dataset").data(), "[0]", can_index_dataset.GetUxmin(), can_index_dataset.GetUxmax());
	sig_high_7_index_dataset.SetParameter(0, set_avg + 7 * set_err);
	sig_high_7_index_dataset.SetLineColor(kOrange+9); sig_high_7_index_dataset.Draw("Same");
	TLegend leg_index_dataset;
	leg_index_dataset.SetBorderSize(0); leg_index_dataset.SetFillStyle(0);
	leg_index_dataset.AddEntry(&avg_index_dataset, "Set Average", "l");
	leg_index_dataset.AddEntry(&sig_low_cut_index_dataset, ("Set Avg +- " + to_string(pars.bad_run_sigmas[energy]) + " sigma").data(), "l");
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
	sig_low_fit.SetParameter(0, run_avg - pars.n_sigma_fit * set_err); sig_low_fit.SetLineColor(kOrange+7); sig_low_fit.Draw("Same");
	TF1 sig_high_fit((energy_str + "GeV_sig_high_fit_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	sig_high_fit.SetParameter(0, run_avg + pars.n_sigma_fit * set_err); sig_high_fit.SetLineColor(kOrange+7); sig_high_fit.Draw("Same");
	TF1 run_cut_low((energy_str + "GeV_run_cut_low_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	run_cut_low.SetParameter(0, run_avg - pars.bad_run_sigmas[energy] * set_err); run_cut_low.SetLineColor(kRed); run_cut_low.Draw("Same");
	TF1 run_cut_high((energy_str + "GeV_run_cut_high_Run_"+run_str).data(), "[0]", can.GetUxmin(), can.GetUxmax());
	run_cut_high.SetParameter(0, run_avg + pars.bad_run_sigmas[energy] * set_err); run_cut_high.SetLineColor(kRed); sig_high_fit.Draw("Same");

	TLegend leg;
	leg.SetBorderSize(0); leg.SetFillStyle(0);
	leg.AddEntry(&avg_set, "Set Average", "l");
	leg.AddEntry(&avg_run, "Run Average", "l");
	leg.AddEntry(&lin_fit, "Linear Baseline", "l");
	leg.AddEntry(&sig_low_7, "Run Avg +- 7 sigma", "l");
	leg.AddEntry(&sig_low_fit, ("Fit Range Run Avg +-" + to_string(pars.n_sigma_fit) + " sigma").data(), "l");
	leg.AddEntry(&run_cut_low, ("Bad Run Boundary Run Avg +-" + to_string(pars.bad_run_sigmas[energy]) + " sigma").data(), "l");
	leg.Draw();
	can.SetGrid();
	can.Write();

	if(mtx) mtx->unlock();
}


// Doers ripped from TreeReader

//Returns true if event is good, false if it is bad.
bool DcaxyQAer::check_event(Event& event) {
	if(!check_good_run(event.get_run())) return false;

	if(!check_enough_particles(event)) return false;

	if(!check_pile_up(event.get_btof_multi(), event.get_btof_match(), event.get_ref())) return false;

	return true;
}

// Check the list of bad runs for input run.
// If input run is contained in bad run list, return false (bad run) else return true (good run).
bool DcaxyQAer::check_good_run(int run) {
	if(find(cut.bad_runs.begin(), cut.bad_runs.end(), run) != cut.bad_runs.end()) { return false; }
	if(refmultCorrUtil->isBadRun(run)) { return false; }

	return true;
}


// Checks if there are enough good particles in the event.
// If more particles than minimum, return true, else false.
bool DcaxyQAer::check_enough_particles(Event& event) {
	if(event.get_num_particles() >=  cut.min_multi) { return(true);	}
	else { return false; }
}


// Check slope of event. If within cuts, return true for good event, else false.
bool DcaxyQAer::check_pile_up(int btof_multi, int btof_match, int ref_mult) {
	float max_btof = 0; float min_btof = 0;
	for(unsigned coef=0; coef < cut.pile_up_high.size(); coef++) {
		max_btof += cut.pile_up_high[coef] * pow(ref_mult, coef);
		min_btof += cut.pile_up_low[coef] * pow(ref_mult, coef);
	}

	if(btof_match > max_btof || btof_match < min_btof) {
		return false;
	}

	return true;
}


// Returns true if particle is good and false if particle is bad.
bool DcaxyQAer::check_particle_good(Track& particle) {
	bool good_particle = false;

	bool check_charge = true;  // Hack to keep same form as TreeReader
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

	if(good_particle) {  }
	else { return false; }


	return good_particle;
}
