/*
 * AzBinner.cpp
 *
 *  Created on: April, 21 2021
 *      Author: dylan
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <TVector3.h>

#include "AzBinner.h"
#include "file_io.h"
#include "ratio_methods.h"

#include "../StRefMultCorr/CentralityMaker.h"
#include "../StRefMultCorr/StRefMultCorr.h"
#include "Event.h"
#include "Track.h"

#include "Mixer.h"
#include "MixerSets.h"

using namespace std;


// Structors

AzBinner::AzBinner(int energy, int ref_num) {
	ampt = false;
	ampt_reaction_plane = false;
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed_sets = false;
	mixed = false;
	rand_data = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;
	rapidity = false;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	this->ref_num = ref_num;
	this->energy = energy;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());
	mix = Mixer(energy);
}


AzBinner::AzBinner(int energy) {
	ampt = false;
	ampt_reaction_plane = false;
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed_sets = false;
	mixed = false;
	rand_data = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;
	rapidity = false;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	ref_num = 2;
	this->energy = energy;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());
	mix = Mixer(energy);
}


AzBinner::AzBinner() {
	ampt = false;
	ampt_reaction_plane = false;
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed_sets = false;
	mixed = false;
	rand_data = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;
	rapidity = false;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	ref_num = 2;
	energy = 7;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());

	mix = Mixer(energy);
}


AzBinner::~AzBinner() {
	//	delete refmultCorrUtil;
}


// Getters

string AzBinner::get_in_path() {
	return(in_path);
}

string AzBinner::get_out_path() {
	return(out_path);
}

string AzBinner::get_qa_path() {
	return(qa_path);
}

string AzBinner::get_set_name() {
	return(set_name);
}

string AzBinner::get_event_cut_hist_name() {
	return(event_cut_hist_name);
}

string AzBinner::get_track_cut_hist_name() {
	return(track_cut_hist_name);
}

bool AzBinner::get_cbwc() {
	return(cbwc);
}

bool AzBinner::get_rotate_random() {
	return(rotate_random);
}

bool AzBinner::get_event_plane() {
	return(event_plane);
}

bool AzBinner::get_mixed_sets() {
	return(mixed_sets);
}

bool AzBinner::get_mixed() {
	return(mixed);
}

bool AzBinner::get_rand_data() {
	return(rand_data);
}

bool AzBinner::get_pile_up() {
	return(pile_up);
}

bool AzBinner::get_efficiency() {
	return(efficiency);
}

bool AzBinner::get_single_ratio() {
	return(single_ratio);
}

double AzBinner::get_pile_up_prob() {
	return(pile_up_prob);
}

double AzBinner::get_efficiency_prob() {
	return(efficiency_prob);
}

int AzBinner::get_cent_binning() {
	return(cent_binning);
}

int AzBinner::get_ref_num() {
	return(ref_num);
}

// Setters

void AzBinner::set_in_path(string path) {
	in_path = path;
}

void AzBinner::set_out_path(string path) {
	out_path = path;
}

void AzBinner::set_qa_path(string path) {
	qa_path = path;
}

void AzBinner::set_ampt_cent_path(string path) {
	ampt_cent_path = path;
}

void AzBinner::set_ampt_type(string type) {
	ampt_type = type;
}

void AzBinner::set_qa_name(string name) {
	qa_name = name;
}

void AzBinner::set_set_name(string set_name) {
	this->set_name = set_name;
}

void AzBinner::set_sim_proton_dist_dataset(string path) {
	sim_proton_dist_dataset = path;
}

void AzBinner::set_sim_eff_dist_path(string root_path, string hist_name) {
	if ((int)sim_eff_dist_path.size() > 0) { sim_eff_dist_path.clear(); }
	sim_eff_dist_path.push_back(root_path);
	sim_eff_dist_path.push_back(hist_name);
}

void AzBinner::set_energy(int energy) {
	this->energy = energy;
}

void AzBinner::set_divs(vector<int> list) {
	divs = list;
}

void AzBinner::set_ampt(bool ampt) {
	this->ampt = ampt;
}

void AzBinner::set_ampt_reaction_plane(bool rp) {
	this->ampt_reaction_plane = rp;
}

void AzBinner::set_cbwc(bool cbwc) {
	this->cbwc = cbwc;
}

void AzBinner::set_rotate_random(bool rotate_random) {
	this->rotate_random = rotate_random;
}

void AzBinner::set_event_plane(bool event_plane) {
	this->event_plane = event_plane;
}

void AzBinner::set_mixed_sets(bool mixed) {
	this->mixed_sets = mixed;
}

void AzBinner::set_mixed(bool mixed_roli) {
	this->mixed = mixed_roli;
}

void AzBinner::set_rand_data(bool rand_data) {
	this->rand_data = rand_data;
}

void AzBinner::set_pile_up(bool pile_up) {
	this->pile_up = pile_up;
}

void AzBinner::set_efficiency(bool efficiency) {
	this->efficiency = efficiency;
}

void AzBinner::set_single_ratio(bool single_ratio) {
	this->single_ratio = single_ratio;
}

void AzBinner::set_n1_ratios(bool n1_ratios) {
	this->n1_ratios = n1_ratios;
}

void AzBinner::set_check_charge(bool check) {
	this->check_charge = check;
}

void AzBinner::set_sim_eff(bool sim_eff) {
	this->sim_eff = sim_eff;
}

void AzBinner::set_sim_flow(bool sim_flow) {
	this->sim_flow = sim_flow;
}

void AzBinner::set_rapidity(bool rapidity) {
	this->rapidity = rapidity;
}

void AzBinner::set_pile_up_prob(double pile_up_prob) {
	this->pile_up_prob = pile_up_prob;
}

void AzBinner::set_efficiency_prob(double efficiency_prob) {
	this->efficiency_prob = efficiency_prob;
}

void AzBinner::set_cent_binning(int cent_binning) {
	this->cent_binning = cent_binning;
}

void AzBinner::set_ref_num(int ref_num) {
	this->ref_num = ref_num;
	if (refmultCorrUtil) { delete refmultCorrUtil; }
	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());
}

void AzBinner::set_particle(string particle) {
	this->particle = particle;
}

void AzBinner::set_particle_dist_hist_max(int max) {
	particle_dist_hist_max = max;
}

void AzBinner::set_ampt_particle_pid(vector<int> pid) {
	ampt_particle_pid = pid;
}

void AzBinner::set_tree_reader_rand_seed(int seed) {
	tree_reader_seed = seed;
	trand = new TRandom3(seed);
}

void AzBinner::set_mixer_rand_seed(int seed) {
	mixer_seed = seed;
	mix.set_rand_seed(seed);
}

void AzBinner::set_file_shuffle_rand_seed(int seed) {
	file_shuffle_seed = seed;
	srand(seed);
}

void AzBinner::set_stref_rand_seed(int seed) {
	stref_seed = seed;
	refmultCorrUtil->set_rand_seed(seed);
}



// Doers

// Set everything necessary for read just in time (construction too early to set). 
void AzBinner::prep_read() {
	//cut.set_values(energy, particle);  // Resets values set in main, for now call in main, think of better way
	set_energy(energy);
	mix.set_single_ratio(single_ratio);
	mix.set_rand_rotate(rotate_random);
	mix.set_n1_ratios(n1_ratios);
}


// Get proton distribution for simulation from real data QA plots.
TH1D* AzBinner::get_sim_proton_dist(int cent) {
	string file_path = sim_proton_dist_dataset + to_string(energy) + "GeV/" + qa_name + to_string(energy) + "GeV.root";
	TFile* file = new TFile(file_path.data(), "READ");
	string dataset_path = sim_proton_dist_dataset;
	dataset_path.pop_back();
	string dataset = get_name_from_path(dataset_path);
	string hist_name = ("Proton_Dist_" + dataset + "_" + to_string(energy) + "_" + to_string(cent)).data();

	TH1D* hist = (TH1D*)file->Get(hist_name.data());
	TH1D* proton_dist = (TH1D*)hist->Clone();
	proton_dist->SetDirectory(0);

	file->Close();
	delete file;

	return proton_dist;
}


// Get efficiency distribution for simulation from real data QA plots.
TH1D* AzBinner::get_sim_efficiency_dist() {
	string file_path = sim_proton_dist_dataset + to_string(energy) + "GeV/" + qa_name + to_string(energy) + "GeV.root";
	TFile* file = new TFile(file_path.data(), "READ");
	string dataset_path = sim_proton_dist_dataset;
	dataset_path.pop_back();
	string dataset = get_name_from_path(dataset_path);
	string hist_name = ("post_phi_" + dataset + "_" + to_string(energy)).data();

	TH1D* hist = (TH1D*)file->Get(hist_name.data());
	TH1D* efficiency_dist = (TH1D*)hist->Clone();
	efficiency_dist->SetDirectory(0);

	file->Close();
	delete file;

	return efficiency_dist;
}


//  For good events/tracks, azimuthally bin particles and save to data
void AzBinner::process_event(const Event& event) {

	// Check if each event is good. Analyze if so, continue if not.
	if (check_event(event)) {
		vector<double> good_particle_angles = {};

		// Iterate over particles in event and add corresponding phi to good_particle_angles if particle good.
		for (const Track& particle : event.get_particles()) {
			if (check_particle_good(particle)) {
				if (efficiency) {  // Skip good particle with chance efficiency_prob
					if (trand->Rndm() < efficiency_prob) { continue; }
				}
				good_particle_angles.push_back(particle.get_phi());
			}
		}

		// Get centrality bin for event from ref_multn value
		refmultCorrUtil->init(event.get_run());
		refmultCorrUtil->initEvent((int)event.get_refn(), (double)event.get_vz());
		int cent16_corr = refmultCorrUtil->getCentralityBin16();
		int cent9_corr = refmultCorrUtil->getCentralityBin9();

		// If there are enough good particles, calculate ratios for each division and save to data.
		if (good_particle_angles.size() >= (unsigned)cut.min_multi) {
			cent16_events.Fill(cent16_corr);
			cent9_events.Fill(cent9_corr);
			event_cut_hist.Fill("Enough Good Particles", 1);

			int cent;
			if (cent_binning == 16) {
				cent = cent16_corr;
			}
			else {
				cent = cent9_corr;
			}
			post_n_particles[cent].Fill((int)good_particle_angles.size());

			TVector2 q(event.get_qx(), event.get_qy());
			pre_ep_hist.Fill(0.5 * q.Phi());
			for (double& angle : good_particle_angles) {  // Subtract contribution of particle of interest to mitigate autocorrelation.
				TVector2 q_new(cos(2 * angle), sin(2 * angle));
				q -= q_new;
			}
			float ep_angle = 0.5 * q.Phi();
			post_ep_hist.Fill(ep_angle);

			if (cbwc) { cent = event.get_refn(); }  // For centrality bin width correction use refmunt n in place of centrality from here on.

			if (ampt) {  // Pre-random rotate event if ampt since all reaction planes are at zero.
				double rand_angle = trand->Rndm() * 2 * M_PI;
				good_particle_angles = rotate_angles(good_particle_angles, rand_angle);
				if (ampt_reaction_plane) { ep_angle = 0; }  // Ampt reaction plane is at zero.
				double ep_rotate = rotate_angle(ep_angle, rand_angle);
				while (ep_rotate >= M_PI) { ep_rotate -= M_PI; }  // Force into range of [0, pi)
				while (ep_rotate < 0) { ep_rotate += M_PI; }
				ep_angle = ep_rotate;
			}

			// If mixed/rand flagged append event to mix/rand object.
			if (mixed) { mix.append_event(good_particle_angles, cent, ep_angle, event.get_vz()); }
			if (mixed_sets) { mix_sets.append_event(good_particle_angles, event.get_refn()); }
			if (rand_data) { random.append_event((int)good_particle_angles.size(), event.get_refn(), trand); }


			if (event_plane) { // If event_plane flag then rotate all angles by -event_plane.
				good_particle_angles = rotate_angles(good_particle_angles, -ep_angle);
			}
			else if (rotate_random) { // If rotate_random flag then rotate all angles by random angle between 0 and 2pi
				double rand_angle = trand->Rndm() * 2 * M_PI;
				good_particle_angles = rotate_angles(good_particle_angles, rand_angle);
				//q.Rotate(rand_angle);  // Untested
				//event.set_event_plane(rotate_angle(event_plane, rand_angle));  // Need to rotate event plane if it's used after this point. Currently is not.
			}


			for (auto& div : divs) {
				int bin_num = (int)360 / div;
				double div_rads = (double)div / 180 * M_PI;
				if (single_ratio) { bin_num = 1; }
				else if (bin_num > 1 && n1_ratios) { bin_num -= 1; }  // Ambiguous if case should change if div divides 360 or not.
				vector<int> event_ratios = get_Rs(good_particle_angles, div_rads, trand, bin_num);  // Convert particle angles in event to ratio values.

				// Save ratio values to data
				for (int particles_in_bin : event_ratios) {
					data[div][cent][good_particle_angles.size()][particles_in_bin]++;
				}
			}
		}
	}
}


// Add file's event/track cut hists to corresponding collected histogram for energy.
void AzBinner::add_cut_hists(TFile* file) {
	TH1D* event_hist = (TH1D*)file->Get(event_cut_hist_name.data());
	TH1D* track_hist = (TH1D*)file->Get(track_cut_hist_name.data());
	TH2F* de_dx_pq = (TH2F*)file->Get(de_dx_pq_hist_name.data());
	TH2F* beta_pq = (TH2F*)file->Get(beta_pq_hist_name.data());

	event_cut_tree_maker.Add(event_hist);
	track_cut_tree_maker.Add(track_hist);
	de_dx_pq_hist.Add(de_dx_pq);
	beta_pq_hist.Add(beta_pq);
}


//Returns true if event is good, false if it is bad.
bool AzBinner::check_event(const Event& event) {
	fill_pre_event_qa(event);
	event_cut_hist.Fill("Original", 1);

	if (!check_good_dca_xy(event.get_run(), event.get_event_id())) return false;
	event_cut_hist.Fill("Good Dca_xy", 1);

	if (!check_good_run(event.get_run())) return false;
	event_cut_hist.Fill("Good Run", 1);

	if (!check_enough_particles(event)) return false;
	event_cut_hist.Fill("Enough Particles", 1);

	if (!check_pile_up(event.get_btof_multi(), event.get_btof_match(), event.get_ref())) return false;
	event_cut_hist.Fill("Pile Up Rejected", 1);

	fill_post_event_qa(event);

	return true;
}


// Check DcaxyQAer bad runs and bad events. If either bad run or bad event, return false. Otherwise true.
bool AzBinner::check_good_dca_xy(int run, int event_id) {
	if (find(cut.dca_xy_bad_runs.begin(), cut.dca_xy_bad_runs.end(), run) != cut.dca_xy_bad_runs.end()) { return false; }

	for (const pair<int, int>& range : cut.dca_xy_bad_event_ranges[run]) {
		if (event_id >= range.first && event_id <= range.second) { return false; }
	}

	return true;
}


// Check the list of bad runs for input run.
// If input run is contained in bad run list, return false (bad run) else return true (good run).
bool AzBinner::check_good_run(int run) {
	if (find(cut.bad_runs.begin(), cut.bad_runs.end(), run) != cut.bad_runs.end()) { return false; }
	if (refmultCorrUtil->isBadRun(run)) { return false; }

	return true;
}


// Checks if there are enough good particles in the event.
// If more particles than minimum, return true, else false.
bool AzBinner::check_enough_particles(const Event& event) {
	if (event.get_num_particles() >= cut.min_multi) { return(true); }
	else { return false; }
}


// Check slope of event. If within cuts, return true for good event, else false.
bool AzBinner::check_pile_up(int btof_multi, int btof_match, int ref_mult) {
	btof_multi_ref_hist.Fill(ref_mult, btof_multi);
	btof_match_ref_hist.Fill(ref_mult, btof_match);

	float min_btof = 0;
	if (ref_mult <= cut.pile_up_low.max_fit_ref) {
		for (unsigned coef = 0; coef < cut.pile_up_low.pol_fit_coefs.size(); coef++) {
			min_btof += cut.pile_up_low.pol_fit_coefs[coef] * pow(ref_mult, coef);
		}
	}
	else {
		min_btof = cut.pile_up_low.lin_extrap.first + cut.pile_up_low.lin_extrap.second * ref_mult;
	}

	float max_btof = 0;
	if (ref_mult <= cut.pile_up_high.max_fit_ref) {
		for (unsigned coef = 0; coef < cut.pile_up_high.pol_fit_coefs.size(); coef++) {
			max_btof += cut.pile_up_high.pol_fit_coefs[coef] * pow(ref_mult, coef);
		}
	}
	else {
		max_btof = cut.pile_up_high.lin_extrap.first + cut.pile_up_high.lin_extrap.second * ref_mult;
	}

	if (btof_match > max_btof || btof_match < min_btof) {
		return false;
	}

	return true;
}


// Returns true if particle is good and false if particle is bad.
bool AzBinner::check_particle_good(const Track& particle) {
	bool good_particle = false;
	track_cut_hist.Fill("Original", 1);

	fill_pre_track_qa(particle);

	if (check_charge && !(particle.get_charge() == cut.charge)) { return false; }
	track_cut_hist.Fill("Good charge", 1);

	double pt = particle.get_pt();
	double eta = particle.get_eta();
	if (rapidity) {
		float m = cut.particle_mass[this->particle];
		float rapid = log((sqrt(pow(m, 2) + pow(pt, 2) * pow(cosh(eta), 2)) + pt * sinh(eta)) / sqrt(pow(m, 2) + pow(pt, 2)));
		pre_rapid_hist.Fill(rapid);
		if (!(rapid >= cut.min_rapid && rapid <= cut.max_rapid)) { return false; }
		post_rapid_hist.Fill(rapid);
	}
	else {
		if (!(eta >= cut.min_eta && eta <= cut.max_eta)) { return false; }
	}
	track_cut_hist.Fill("Good eta", 1);

	double nsigma = particle.get_nsigma();
	if (!(nsigma >= cut.min_nsigma && nsigma <= cut.max_nsigma)) { return false; }
	track_cut_hist.Fill("Good nsigma", 1);

	double dca = particle.get_dca();
	if (!(dca >= cut.min_dca && dca <= cut.max_dca)) { return false; }
	track_cut_hist.Fill("Good dca", 1);

	if (particle.get_nhits_fit() <= cut.min_nhits_fit) { return false; }
	track_cut_hist.Fill("Good nhitsfit", 1);

	double p = particle.get_p();

	if (pt >= cut.min_pt_no_tof && pt <= cut.max_pt_no_tof && p <= cut.max_p_no_tof) {
		good_particle = true;
	}
	else if (pt >= cut.min_pt_tof && pt <= cut.max_pt_tof && p <= cut.max_p_tof) {
		double beta = particle.get_beta();
		if (beta > cut.min_beta) {
			double m2 = pow(p, 2) * (pow(beta, -2) - 1.);
			pre_m2_hist.Fill(m2);
			if (m2 > cut.min_m2 && m2 < cut.max_m2) {
				good_particle = true;
				post_m2_hist.Fill(m2);
			}
		}  // Else leave good_particle = false
	}

	if (good_particle) { track_cut_hist.Fill("Good mass*", 1); }
	else { return false; }

	fill_post_track_qa(particle);

	return good_particle;
}


// Define all histograms collected for qa.
void AzBinner::define_qa() {
	cent_hist = TH2I(("cent_comp" + set_name + "_" + to_string(energy)).data(), "Centrality Comparison", 19, -2.5, 16.5, 19, -2.5, 16.5);
	btof_multi_ref_hist = TH2I(("btof_multi_ref" + set_name + "_" + to_string(energy)).data(), "BTof_Multi vs Ref", 601, -0.5, 600.5, 2001, -0.5, 2000.5);
	btof_match_ref_hist = TH2I(("btof_match_ref" + set_name + "_" + to_string(energy)).data(), "BTof_Match vs Ref", 601, -0.5, 600.5, 501, -0.5, 500.5);

	de_dx_pq_hist = TH2F(("de_dx_pid_" + set_name + "_" + to_string(energy)).data(), "Dedx PID", 1000, -3, 3, 1000, 0, 0.5e-4);
	beta_pq_hist = TH2F(("beta_pq_pid_" + set_name + "_" + to_string(energy)).data(), "Beta PID", 1000, -3, 3, 1000, 0, 5);

	event_cut_tree_maker = TH1D(("event_cut_tree_maker" + set_name + "_" + to_string(energy)).data(), "Event Cuts", 9, -0.5, 8.5);
	event_cut_tree_maker.GetXaxis()->SetBinLabel(1, "Expected");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(2, "Events Read");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(3, "Is dstEvent");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(4, "Good Trigger");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(5, "Good Run");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(6, "Good Vz");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(7, "Good Vr");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(8, "Vertex Non-Zero");
	event_cut_tree_maker.GetXaxis()->SetBinLabel(9, "Good VPD Vz");

	event_cut_tree_maker.Fill("Expected", cut.expected_events);

	track_cut_tree_maker = TH1D(("track_cut_tree_maker" + set_name + "_" + to_string(energy)).data(), "Track Cuts", 16, -0.5, 15.5);
	track_cut_tree_maker.GetXaxis()->SetBinLabel(1, "Tracks Read");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(2, "Is Track");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(3, "Primary Track/Flags");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(4, "Charge");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(5, "nHitsRatio Min");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(6, "nHitsRatio Max");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(7, "eta");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(8, "nHitsFit");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(9, "nHitsDedx");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(10, "dca");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(11, "pt_low");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(12, "pt_high");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(13, "nsigma_proton");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(14, "m_proton");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(15, "nsigma_pion");
	track_cut_tree_maker.GetXaxis()->SetBinLabel(16, "m_pion");

	event_cut_hist = TH1D(("event_cut" + set_name + "_" + to_string(energy)).data(), "Event Cuts", 6, -0.5, 5.5);
	event_cut_hist.GetXaxis()->SetBinLabel(1, "Original");
	event_cut_hist.GetXaxis()->SetBinLabel(2, "Good Dca_xy");
	event_cut_hist.GetXaxis()->SetBinLabel(3, "Good Run");
	event_cut_hist.GetXaxis()->SetBinLabel(4, "Enough Particles");
	event_cut_hist.GetXaxis()->SetBinLabel(5, "Pile Up Rejected");
	event_cut_hist.GetXaxis()->SetBinLabel(6, "Enough Good Particles");

	track_cut_hist = TH1D(("track_cut" + set_name + "_" + to_string(energy)).data(), "Track Cuts", 7, -0.5, 6.5);
	track_cut_hist.GetXaxis()->SetBinLabel(1, "Original");
	track_cut_hist.GetXaxis()->SetBinLabel(2, "Good charge");
	track_cut_hist.GetXaxis()->SetBinLabel(3, "Good eta");
	track_cut_hist.GetXaxis()->SetBinLabel(4, "Good nsigma");
	track_cut_hist.GetXaxis()->SetBinLabel(5, "Good dca");
	track_cut_hist.GetXaxis()->SetBinLabel(6, "Good nhitsfit");
	track_cut_hist.GetXaxis()->SetBinLabel(7, "Good mass*");

	eta_pt_hist = TH2F(("eta_pt_" + set_name + "_" + to_string(energy)).data(), "Pt vs Eta", 1000, -2.2, 2.2, 1000, 0, 2.25);

	cent16_events = TH1I(("cent16_events_" + set_name + "_" + to_string(energy)).data(), "Cent16 Events", 18, -1.5, 16.5);
	cent9_events = TH1I(("cent9_events_" + set_name + "_" + to_string(energy)).data(), "Cent9 Events", 11, -1.5, 9.5);

	pre_run_hist = TH1I(("pre_run_" + set_name + "_" + to_string(energy)).data(), "pre_run", 1000, 1000000, 100000000);
	pre_vx_hist = TH1I(("pre_vx_" + set_name + "_" + to_string(energy)).data(), "pre_vx", 100, -2.5, 2.5);
	pre_vy_hist = TH1I(("pre_vy_" + set_name + "_" + to_string(energy)).data(), "pre_vy", 100, -2.5, 2.5);
	pre_vz_hist = TH1I(("pre_vz_" + set_name + "_" + to_string(energy)).data(), "pre_vz", 100, -55, 55);
	pre_ref_hist = TH1I(("pre_ref_" + set_name + "_" + to_string(energy)).data(), "pre_ref", 801, -0.5, 800.5);
	pre_refn_hist = TH1I(("pre_refn_" + set_name + "_" + to_string(energy)).data(), "pre_refn", 801, -0.5, 800.5);
	pre_btof_multi_hist = TH1I(("pre_btof_multi_" + set_name + "_" + to_string(energy)).data(), "pre_btof_multi", 2001, -0.5, 2000.5);
	pre_btof_match_hist = TH1I(("pre_btof_match_" + set_name + "_" + to_string(energy)).data(), "pre_btof_match", 501, -0.5, 500.5);
	pre_ep_hist = TH1I(("pre_ep_" + set_name + "_" + to_string(energy)).data(), "pre_ep", 100, -0.5, 3.5);

	post_run_hist = TH1I(("post_run_" + set_name + "_" + to_string(energy)).data(), "post_run", 1000, 1000000, 100000000);
	post_vx_hist = TH1I(("post_vx_" + set_name + "_" + to_string(energy)).data(), "post_vx", 100, -2.5, 2.5);
	post_vy_hist = TH1I(("post_vy_" + set_name + "_" + to_string(energy)).data(), "post_vy", 100, -2.5, 2.5);
	post_vz_hist = TH1I(("post_vz_" + set_name + "_" + to_string(energy)).data(), "post_vz", 100, -55, 55);
	post_ref_hist = TH1I(("post_ref_" + set_name + "_" + to_string(energy)).data(), "post_ref", 801, -0.5, 800.5);
	post_refn_hist = TH1I(("post_refn_" + set_name + "_" + to_string(energy)).data(), "post_refn", 801, -0.5, 800.5);
	post_btof_multi_hist = TH1I(("post_btof_multi_" + set_name + "_" + to_string(energy)).data(), "post_btof_multi", 2001, -0.5, 2000.5);
	post_btof_match_hist = TH1I(("post_btof_match_" + set_name + "_" + to_string(energy)).data(), "post_btof_match", 501, -0.5, 500.5);
	post_ep_hist = TH1I(("post_ep_" + set_name + "_" + to_string(energy)).data(), "post_ep", 100, -0.5, 3.5);

	pre_phi_hist = TH1D(("pre_phi_" + set_name + "_" + to_string(energy)).data(), "pre_phi", 1000, 0.0, 2 * TMath::Pi());
	pre_p_hist = TH1I(("pre_p_" + set_name + "_" + to_string(energy)).data(), "pre_p", 100, 0.0, 3.5);
	pre_pt_hist = TH1I(("pre_pt_" + set_name + "_" + to_string(energy)).data(), "pre_pt", 100, 0.0, 3.0);
	pre_beta_hist = TH1I(("pre_beta_" + set_name + "_" + to_string(energy)).data(), "pre_beta", 100, -0.5, 2.0);
	pre_charge_hist = TH1I(("pre_charge_" + set_name + "_" + to_string(energy)).data(), "pre_charge", 100, -2.5, 2.5);
	pre_eta_hist = TH1I(("pre_eta_" + set_name + "_" + to_string(energy)).data(), "pre_eta", 100, -2.2, 2.2);
	pre_nsigma_hist = TH1I(("pre_nsigma_" + set_name + "_" + to_string(energy)).data(), "pre_nsigma", 100, -2.5, 2.5);
	pre_dca_hist = TH1I(("pre_dca_" + set_name + "_" + to_string(energy)).data(), "pre_dca", 100, 0.0, 3.1);
	pre_nhits_fit_hist = TH1I(("pre_nhits_fit_" + set_name + "_" + to_string(energy)).data(), "pre_nhits_fit", 101, -0.5, 100.5);
	pre_rapid_hist = TH1I(("pre_rapid_" + set_name + "_" + to_string(energy)).data(), "pre_rapid", 100, -2.2, 2.2);

	post_phi_hist = TH1D(("post_phi_" + set_name + "_" + to_string(energy)).data(), "post_phi", 1000, 0.0, 2 * TMath::Pi());
	post_p_hist = TH1I(("post_p_" + set_name + "_" + to_string(energy)).data(), "post_p", 100, 0.0, 3.5);
	post_pt_hist = TH1I(("post_pt_" + set_name + "_" + to_string(energy)).data(), "post_pt", 100, 0.0, 3.0);
	post_beta_hist = TH1I(("post_beta_" + set_name + "_" + to_string(energy)).data(), "post_beta", 100, -0.5, 2.0);
	post_charge_hist = TH1I(("post_charge_" + set_name + "_" + to_string(energy)).data(), "post_charge", 100, -2.5, 2.5);
	post_eta_hist = TH1I(("post_eta_" + set_name + "_" + to_string(energy)).data(), "post_eta", 100, -2.2, 2.2);
	post_nsigma_hist = TH1I(("post_nsigma_" + set_name + "_" + to_string(energy)).data(), "post_nsigma", 100, -2.5, 2.5);
	post_dca_hist = TH1I(("post_dca_" + set_name + "_" + to_string(energy)).data(), "post_dca", 100, 0.0, 3.1);
	post_nhits_fit_hist = TH1I(("post_nhits_fit_" + set_name + "_" + to_string(energy)).data(), "post_nhits_fit", 101, -0.5, 100.5);
	post_rapid_hist = TH1I(("post_rapid_" + set_name + "_" + to_string(energy)).data(), "post_rapid", 100, -2.2, 2.2);

	pre_m2_hist = TH1I(("pre_m2_" + set_name + "_" + to_string(energy)).data(), "pre_m^2", 100, -0.5, 2.0);
	post_m2_hist = TH1I(("post_m2_" + set_name + "_" + to_string(energy)).data(), "post_m^2", 100, -0.5, 2.0);

	for (int i = -1; i < cent_binning; i++) {
		post_n_particles[i] = TH1D(("Particle_Dist_" + set_name + "_" + to_string(energy) + "_" + to_string(i)).data(), "Particle Distribution", particle_dist_hist_max + 1, -0.5, particle_dist_hist_max + 0.5);
	}

}


// Fill histograms for pre-qa for tracks
void AzBinner::fill_pre_track_qa(const Track& particle) {
	pre_phi_hist.Fill(particle.get_phi());
	pre_p_hist.Fill(particle.get_p());
	pre_pt_hist.Fill(particle.get_pt());
	pre_beta_hist.Fill(particle.get_beta());
	pre_charge_hist.Fill(particle.get_charge());
	pre_eta_hist.Fill(particle.get_eta());
	pre_nsigma_hist.Fill(particle.get_nsigma());
	pre_dca_hist.Fill(particle.get_dca());
	pre_nhits_fit_hist.Fill(particle.get_nhits_fit());
}


// Fill histograms for post-qa for tracks
void AzBinner::fill_post_track_qa(const Track& particle) {
	post_phi_hist.Fill(particle.get_phi());
	post_p_hist.Fill(particle.get_p());
	post_pt_hist.Fill(particle.get_pt());
	post_beta_hist.Fill(particle.get_beta());
	post_charge_hist.Fill(particle.get_charge());
	post_eta_hist.Fill(particle.get_eta());
	post_nsigma_hist.Fill(particle.get_nsigma());
	post_dca_hist.Fill(particle.get_dca());
	post_nhits_fit_hist.Fill(particle.get_nhits_fit());
	eta_pt_hist.Fill(particle.get_eta(), particle.get_pt());
}


// Fill histograms for pre-qa for event
void AzBinner::fill_pre_event_qa(const Event& event) {
	pre_run_hist.Fill(event.get_run());
	pre_vx_hist.Fill(event.get_vx());
	pre_vy_hist.Fill(event.get_vy());
	pre_vz_hist.Fill(event.get_vz());
	pre_ref_hist.Fill(event.get_ref());
	pre_refn_hist.Fill(event.get_refn());
	pre_btof_multi_hist.Fill(event.get_btof_multi());
	pre_btof_match_hist.Fill(event.get_btof_match());
	//	pre_ep_hist.Fill(event.get_event_plane());
}

// Fill histograms for post-qa for event
void AzBinner::fill_post_event_qa(const Event& event) {
	post_run_hist.Fill(event.get_run());
	post_vx_hist.Fill(event.get_vx());
	post_vy_hist.Fill(event.get_vy());
	post_vz_hist.Fill(event.get_vz());
	post_ref_hist.Fill(event.get_ref());
	post_refn_hist.Fill(event.get_refn());
	post_btof_multi_hist.Fill(event.get_btof_multi());
	post_btof_match_hist.Fill(event.get_btof_match());
	//	post_ep_hist.Fill(event.get_event_plane());
}


// Write binned data to histogram text files, raw and mixed
void AzBinner::write_binner_data() {
	write_tree_data("local", data, out_path + to_string(energy) + "GeV/");
	if (mixed) { mix.write_mixed_data(); }
	if (mixed_sets) { mix_sets.write_mixed_data(); }
	if (rand_data) { random.write_random_data(); }
}


// Write file to outpath containing information about AzBinner variables used.
void AzBinner::write_info_file() {
	ofstream out(out_path + to_string(energy) + "GeV/" + info_file_name);
	if (!out) { cerr << "Couldn't open info_file path" << endl; }
	else {
		out << "Information file for AzBinner run." << endl;
		out << "in_path: " << in_path << endl;
		out << "out_path: " << out_path << endl;
		out << "qa_path: " << qa_path << endl;
		out << "qa_name: " << qa_name << endl;

		out << "energy: " << to_string(energy) << endl;
		out << "divs: { ";
		for (auto& div : divs) { out << to_string(div) << " "; }
		out << "}" << endl;

		out << "particle: " << particle << endl;

		out << "ampt: " << boolalpha << ampt << endl;
		out << "ampt_reaction_plane: " << boolalpha << ampt_reaction_plane << endl;
		out << "cbwc: " << boolalpha << cbwc << endl;
		out << "rotate_random: " << boolalpha << rotate_random << endl;
		out << "event_plane: " << boolalpha << event_plane << endl;
		out << "mixed_sets: " << boolalpha << mixed_sets << endl;
		out << "mixed: " << boolalpha << mixed << endl;
		out << "rand_data: " << boolalpha << rand_data << endl;
		out << "pile_up: " << boolalpha << pile_up << endl;
		out << "efficiency: " << boolalpha << efficiency << endl;
		out << "single_ratio: " << boolalpha << single_ratio << endl;
		out << "n1_ratios: " << boolalpha << n1_ratios << endl;
		out << "sim_eff: " << boolalpha << sim_eff << endl;
		out << "sim_flow: " << boolalpha << sim_flow << endl;
		out << "check_charge: " << boolalpha << check_charge << endl;
		out << "rapidity: " << boolalpha << rapidity << endl;

		out << "pile_up_prob: " << pile_up_prob << endl;
		out << "efficiency_prob: " << efficiency_prob << endl;

		out << "cent_binning: " << cent_binning << endl;
		out << "ref_num: " << ref_num << endl;

		out << "tree_reader_seed: " << tree_reader_seed << endl;
		out << "mixer_seed: " << mixer_seed << endl;
		out << "file_shuffle_seed: " << file_shuffle_seed << endl;
		out << "stref_seed: " << stref_seed << endl;

		out << "min_beta: " << to_string(cut.min_beta) << endl;
		out << "max_beta: " << to_string(cut.max_beta) << endl;
		out << "charge: " << to_string(cut.charge) << endl;
		out << "min_eta: " << to_string(cut.min_eta) << endl;
		out << "max_eta: " << to_string(cut.max_eta) << endl;
		out << "min_rapid: " << to_string(cut.min_rapid) << endl;
		out << "max_rapid: " << to_string(cut.max_rapid) << endl;
		out << "min_nsigma: " << to_string(cut.min_nsigma) << endl;
		out << "max_nsigma: " << to_string(cut.max_nsigma) << endl;
		out << "min_dca: " << to_string(cut.min_dca) << endl;
		out << "max_dca: " << to_string(cut.max_dca) << endl;
		out << "min_m2: " << to_string(cut.min_m2) << endl;
		out << "max_m2: " << to_string(cut.max_m2) << endl;
		out << "min_pt_tof: " << to_string(cut.min_pt_tof) << endl;
		out << "max_pt_tof: " << to_string(cut.max_pt_tof) << endl;
		out << "min_pt_no_tof: " << to_string(cut.min_pt_no_tof) << endl;
		out << "max_pt_no_tof: " << to_string(cut.max_pt_no_tof) << endl;
		out << "max_p_tof: " << to_string(cut.max_p_tof) << endl;
		out << "max_p_no_tof: " << to_string(cut.max_p_no_tof) << endl;
		out << "min_nhits_fit: " << to_string(cut.min_nhits_fit) << endl;

		out << "min_multi: " << to_string(cut.min_multi) << endl;

		out << "sim p_group: " << to_string(sim.get_p_group()) << endl;
		out << "sim spread_sigma: " << to_string(sim.get_spread_sigma()) << endl;

		out << "mix single_ratio: " << boolalpha << mix.get_single_ratio() << endl;
		out << "mix n1_ratios: " << boolalpha << mix.get_n1_ratios() << endl;
		out << "mix rand_rotate: " << boolalpha << mix.get_rand_rotate() << endl;
		out << "mix event_plane_rotate: " << boolalpha << mix.get_event_plane_rotate() << endl;
		out << "mix max_events: " << to_string(mix.get_max_events()) << endl;
		out << "mix min_events: " << to_string(mix.get_min_events()) << endl;
		out << "mix mixes_per_event: " << to_string(mix.get_mixes_per_event()) << endl;
		out << "mix vz_bins: " << to_string(mix.get_vz_bins()) << endl;
		out << "mix ep_bins: " << to_string(mix.get_ep_bins()) << endl;
		out << "mix vz_range: " << to_string(mix.get_vz_range().first) << ", " << to_string(mix.get_vz_range().second) << endl;
		out << "mix ep_range: " << to_string(mix.get_ep_range().first) << ", " << to_string(mix.get_ep_range().second) << endl;

		out << "Ampt Particle PIDs: ";
		for (auto& pid : ampt_particle_pid) { out << to_string(pid) << " "; }
		out << endl;
	}
}


// Write all qa plots into TFile.
void AzBinner::write_qa() {
	TFile qa((qa_path + qa_name + to_string(energy) + "GeV.root").data(), "RECREATE");

	TCanvas cent_can("cent_can");
	cent_hist.Draw("COLZ");
	cent_can.Write();
	cent_hist.Write();

	TCanvas btof_multi_pile_can("btof_multi_pile_can");
	btof_multi_ref_hist.GetXaxis()->SetTitle("Refmult");
	btof_multi_ref_hist.GetYaxis()->SetTitle("Btof Multiplicity");
	btof_multi_ref_hist.Draw("COLZ");
	btof_multi_pile_can.Update();
	btof_multi_pile_can.Write();
	btof_multi_ref_hist.Write();

	TCanvas btof_match_pile_can("btof_match_pile_can");
	btof_match_ref_hist.GetXaxis()->SetTitle("Refmult");
	btof_match_ref_hist.GetYaxis()->SetTitle("Btof Match");
	btof_match_ref_hist.Draw("COLZ");
	btof_match_pile_can.Update();
	TF1 up_cut("btof_match_pile_up_cut", "pol4", btof_match_pile_can.GetUxmin(), cut.pile_up_high.max_fit_ref);
	vector<float> high_pars = cut.pile_up_high.pol_fit_coefs;
	up_cut.SetParameters(high_pars[0], high_pars[1], high_pars[2], high_pars[3], high_pars[4]);
	TF1 up_extrap("btof_match_pile_up_extrap", "pol1", cut.pile_up_high.max_fit_ref, btof_match_pile_can.GetUxmax());
	up_extrap.SetParameters(cut.pile_up_high.lin_extrap.first, cut.pile_up_high.lin_extrap.second);
	up_cut.SetLineColor(kRed); up_extrap.SetLineColor(kRed);
	TF1 low_cut("btof_match_pile_low_cut", "pol4", btof_match_pile_can.GetUxmin(), cut.pile_up_low.max_fit_ref);
	vector<float> low_pars = cut.pile_up_low.pol_fit_coefs;
	low_cut.SetParameters(low_pars[0], low_pars[1], low_pars[2], low_pars[3], low_pars[4]);
	TF1 low_extrap("btof_match_pile_low_extrap", "pol1", cut.pile_up_low.max_fit_ref, btof_match_pile_can.GetUxmax());
	low_extrap.SetParameters(cut.pile_up_low.lin_extrap.first, cut.pile_up_low.lin_extrap.second);
	low_cut.SetLineColor(kRed); low_extrap.SetLineColor(kRed);
	up_cut.Draw("same"); low_cut.Draw("same"); up_extrap.Draw("same"); low_extrap.Draw("same");
	btof_match_pile_can.Write();
	btof_match_ref_hist.Write();

	TCanvas de_dx_can("de_dx_pq_can");
	de_dx_pq_hist.GetXaxis()->SetTitle("p*q");
	de_dx_pq_hist.GetYaxis()->SetTitle("dEdx");
	de_dx_pq_hist.Draw("COLZ");
	de_dx_can.SetLogz();
	de_dx_can.Update();
	de_dx_can.Write();
	de_dx_pq_hist.Write();

	TCanvas beta_pq_can("beta_pq_can");
	beta_pq_hist.GetXaxis()->SetTitle("p*q");
	beta_pq_hist.GetYaxis()->SetTitle("beta");
	beta_pq_hist.Draw("COLZ");
	beta_pq_can.SetLogz();
	beta_pq_can.Update();
	beta_pq_can.Write();
	beta_pq_hist.Write();

	TCanvas eta_pt_can("eta_pt_can");
	eta_pt_hist.GetXaxis()->SetTitle("eta");
	eta_pt_hist.GetYaxis()->SetTitle("pt (GeV)");
	eta_pt_hist.Draw("COLZ");
	eta_pt_can.SetLogz();
	eta_pt_can.Update();
	eta_pt_can.Write();
	eta_pt_hist.Write();

	event_cut_tree_maker.Write();
	track_cut_tree_maker.Write();
	event_cut_hist.Write();
	track_cut_hist.Write();
	cent16_events.Write();
	cent9_events.Write();

	pre_run_hist.Write();
	pre_vx_hist.Write();
	pre_vy_hist.Write();
	pre_vz_hist.Write();
	pre_ref_hist.Write();
	pre_refn_hist.Write();
	pre_btof_multi_hist.Write();
	pre_btof_match_hist.Write();
	pre_ep_hist.Write();

	post_run_hist.Write();
	post_vx_hist.Write();
	post_vy_hist.Write();
	post_vz_hist.Write();
	post_ref_hist.Write();
	post_refn_hist.Write();
	post_btof_multi_hist.Write();
	post_btof_match_hist.Write();
	post_ep_hist.Write();

	pre_phi_hist.Write();
	pre_p_hist.Write();
	pre_pt_hist.Write();
	pre_beta_hist.Write();
	pre_charge_hist.Write();
	pre_eta_hist.Write();
	pre_nsigma_hist.Write();
	pre_dca_hist.Write();
	pre_nhits_fit_hist.Write();
	pre_rapid_hist.Write();
	post_phi_hist.Write();
	post_p_hist.Write();
	post_pt_hist.Write();
	post_beta_hist.Write();
	post_charge_hist.Write();
	post_eta_hist.Write();
	post_nsigma_hist.Write();
	post_dca_hist.Write();
	post_nhits_fit_hist.Write();
	post_rapid_hist.Write();

	pre_m2_hist.Write();
	post_m2_hist.Write();

	for (auto hist : post_n_particles) {
		hist.second.Write();
	}


	// Make bar plots for event/track cut histograms
	TCanvas event_cut_maker_can("event_cut_maker_can");
	gPad->SetLeftMargin(0.15);
	event_cut_tree_maker.SetFillColor(45);
	event_cut_maker_can.SetFillColor(42);
	gPad->SetFrameFillColor(33);
	event_cut_tree_maker.Draw("hbar2");
	event_cut_maker_can.Write();
	TCanvas track_cut_maker_can("track_cut_maker_can");
	gPad->SetLeftMargin(0.15);
	track_cut_tree_maker.SetFillColor(45);
	track_cut_maker_can.SetFillColor(42);
	gPad->SetFrameFillColor(33);
	track_cut_tree_maker.Draw("hbar2");
	track_cut_maker_can.Write();
	TCanvas event_cut_hist_can("event_cut_hist_can");
	gPad->SetLeftMargin(0.15);
	event_cut_hist.SetFillColor(45);
	event_cut_hist_can.SetFillColor(42);
	gPad->SetFrameFillColor(33);
	event_cut_hist.Draw("hbar2");
	event_cut_hist_can.Write();
	TCanvas track_cut_hist_can("track_cut_hist_can");
	gPad->SetLeftMargin(0.15);
	track_cut_hist.SetFillColor(45);
	track_cut_hist_can.SetFillColor(42);
	gPad->SetFrameFillColor(33);
	track_cut_hist.Draw("hbar2");
	track_cut_hist_can.Write();

	// Make before/after canvases for each variable
	TCanvas run_can("run_can");
	pre_run_hist.GetYaxis()->SetRangeUser(0, pre_run_hist.GetMaximum() * 1.05);
	pre_run_hist.Draw();
	post_run_hist.SetLineColor(kRed);
	post_run_hist.Draw("sames");
	run_can.Write();
	TCanvas vx_can("vx_can");
	pre_vx_hist.GetYaxis()->SetRangeUser(0, pre_vx_hist.GetMaximum() * 1.05);
	pre_vx_hist.Draw();
	post_vx_hist.SetLineColor(kRed);
	post_vx_hist.Draw("sames");
	vx_can.Write();
	TCanvas vy_can("vy_can");
	pre_vy_hist.GetYaxis()->SetRangeUser(0, pre_vy_hist.GetMaximum() * 1.05);
	pre_vy_hist.Draw();
	post_vy_hist.SetLineColor(kRed);
	post_vy_hist.Draw("sames");
	vy_can.Write();
	TCanvas vz_can("vz_can");
	pre_vz_hist.GetYaxis()->SetRangeUser(0, pre_vz_hist.GetMaximum() * 1.05);
	pre_vz_hist.Draw();
	post_vz_hist.SetLineColor(kRed);
	post_vz_hist.Draw("sames");
	vz_can.Write();
	TCanvas ref_can("ref_can");
	pre_ref_hist.GetYaxis()->SetRangeUser(0, pre_ref_hist.GetMaximum() * 1.05);
	pre_ref_hist.Draw();
	post_ref_hist.SetLineColor(kRed);
	post_ref_hist.Draw("sames");
	ref_can.Write();
	TCanvas refn_can("refn_can");
	pre_run_hist.GetYaxis()->SetRangeUser(0, pre_run_hist.GetMaximum() * 1.05);
	pre_refn_hist.Draw();
	post_refn_hist.SetLineColor(kRed);
	post_refn_hist.Draw("sames");
	refn_can.Write();
	TCanvas btof_multi_can("btof_multi_can");
	pre_run_hist.GetYaxis()->SetRangeUser(0, pre_run_hist.GetMaximum() * 1.05);
	pre_btof_multi_hist.Draw();
	post_btof_multi_hist.SetLineColor(kRed);
	post_btof_multi_hist.Draw("sames");
	btof_multi_can.Write();
	TCanvas btof_match_can("btof_match_can");
	pre_run_hist.GetYaxis()->SetRangeUser(0, pre_run_hist.GetMaximum() * 1.05);
	pre_btof_match_hist.Draw();
	post_btof_match_hist.SetLineColor(kRed);
	post_btof_match_hist.Draw("sames");
	btof_match_can.Write();
	TCanvas ep_can("ep_can");
	post_ep_hist.SetLineColor(kRed);
	post_ep_hist.Draw();
	//	pre_run_hist.GetYaxis()->SetRangeUser(0, pre_run_hist.GetMaximum()*1.05);
	pre_ep_hist.Draw("sames");
	ep_can.Write();
	TCanvas phi_can("phi_can");
	pre_phi_hist.GetYaxis()->SetRangeUser(0, pre_phi_hist.GetMaximum() * 1.05);
	pre_phi_hist.Draw();
	post_phi_hist.SetLineColor(kRed);
	post_phi_hist.Draw("sames");
	phi_can.Write();
	TCanvas p_can("p_can");
	pre_p_hist.GetYaxis()->SetRangeUser(0, pre_p_hist.GetMaximum() * 1.05);
	pre_p_hist.Draw();
	post_p_hist.SetLineColor(kRed);
	post_p_hist.Draw("sames");
	p_can.Write();
	TCanvas pt_can("pt_can");
	pre_pt_hist.GetYaxis()->SetRangeUser(0, pre_pt_hist.GetMaximum() * 1.05);
	pre_pt_hist.Draw();
	post_pt_hist.SetLineColor(kRed);
	post_pt_hist.Draw("sames");
	pt_can.Write();
	TCanvas beta_can("beta_can");
	pre_beta_hist.GetYaxis()->SetRangeUser(0, pre_beta_hist.GetMaximum() * 1.05);
	pre_beta_hist.Draw();
	post_beta_hist.SetLineColor(kRed);
	post_beta_hist.Draw("sames");
	beta_can.Write();
	TCanvas charge_can("charge_can");
	pre_charge_hist.GetYaxis()->SetRangeUser(0, pre_charge_hist.GetMaximum() * 1.05);
	pre_charge_hist.Draw();
	post_charge_hist.SetLineColor(kRed);
	post_charge_hist.Draw("sames");
	charge_can.Write();
	TCanvas eta_can("eta_can");
	pre_eta_hist.GetYaxis()->SetRangeUser(0, pre_eta_hist.GetMaximum() * 1.05);
	pre_eta_hist.Draw();
	post_eta_hist.SetLineColor(kRed);
	post_eta_hist.Draw("sames");
	eta_can.Write();
	TCanvas nsigma_can("nsigma_can");
	pre_nsigma_hist.GetYaxis()->SetRangeUser(0, pre_nsigma_hist.GetMaximum() * 1.05);
	pre_nsigma_hist.Draw();
	post_nsigma_hist.SetLineColor(kRed);
	post_nsigma_hist.Draw("sames");
	nsigma_can.Write();
	TCanvas dca_can("dca_can");
	pre_dca_hist.GetYaxis()->SetRangeUser(0, pre_dca_hist.GetMaximum() * 1.05);
	pre_dca_hist.Draw();
	post_dca_hist.SetLineColor(kRed);
	post_dca_hist.Draw("sames");
	dca_can.Write();
	TCanvas nhits_fit_can("nhits_fit_can");
	pre_nhits_fit_hist.GetYaxis()->SetRangeUser(0, pre_nhits_fit_hist.GetMaximum() * 1.05);
	pre_nhits_fit_hist.Draw();
	post_nhits_fit_hist.SetLineColor(kRed);
	post_nhits_fit_hist.Draw("sames");
	nhits_fit_can.Write();
	TCanvas rapid_can("rapid_can");
	pre_rapid_hist.GetYaxis()->SetRangeUser(0, pre_rapid_hist.GetMaximum() * 1.05);
	pre_rapid_hist.Draw();
	post_rapid_hist.SetLineColor(kRed);
	post_rapid_hist.Draw("sames");
	rapid_can.Write();
	TCanvas m2_can("m2_can");
	pre_m2_hist.GetYaxis()->SetRangeUser(0, pre_m2_hist.GetMaximum() * 1.05);
	pre_m2_hist.Draw();
	post_m2_hist.SetLineColor(kRed);
	post_m2_hist.Draw("sames");
	m2_can.Write();

	mix.write_qa();

	qa.Close();
}


// Remove out_path directory for energy if it exists and recreate it.
// Create out_path directory if it does not exist.
void AzBinner::reset_out_dir() {
	// Seems to work on both Linux and Windows, need to double check Windows after changes
	mkdir(out_path);
	mkdir(out_path + to_string(energy) + "GeV/", true);
}
