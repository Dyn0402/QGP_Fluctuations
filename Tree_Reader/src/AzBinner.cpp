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

using namespace std;


// Structors

AzBinner::AzBinner(int energy, int ref_num, mutex *mtx) {
	ampt = false;
	cooper_frye = false;
	ampt_reaction_plane = false;
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;
	rapidity = false;
	resample = false;
	prerotate = true;
	calc_v2 = false;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	this->ref_num = ref_num;
	this->energy = energy;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());
	mix = Mixer(energy);

	this->mtx = mtx;

//	c_rand = mt19937_64(trand->Integer(numeric_limits<int>::max()));
//	pois_dist = poisson_distribution<int> (1);
}

AzBinner::AzBinner(int energy, int ref_num) {
	ampt = false;
	cooper_frye = false;
	ampt_reaction_plane = false;
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;
	rapidity = false;
	resample = false;
	prerotate = true;
	calc_v2 = false;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	this->ref_num = ref_num;
	this->energy = energy;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());
	mix = Mixer(energy);

//	c_rand = mt19937_64(trand->Integer(numeric_limits<int>::max()));
//	pois_dist = poisson_distribution<int> (1);
}


AzBinner::AzBinner(int energy) {
	ampt = false;
	cooper_frye = false;
	ampt_reaction_plane = false;
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;
	rapidity = false;
	resample = false;
	prerotate = true;
	calc_v2 = false;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	ref_num = 2;
	this->energy = energy;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());
	mix = Mixer(energy);

//	c_rand = mt19937_64(trand->Integer(numeric_limits<int>::max()));
//	pois_dist = poisson_distribution<int> (1);
}


AzBinner::AzBinner() {
	ampt = false;
	cooper_frye = false;
	ampt_reaction_plane = false;
	cbwc = false;
	rotate_random = false;
	event_plane = false;
	mixed = false;
	pile_up = false;
	efficiency = false;
	single_ratio = false;
	n1_ratios = false;
	check_charge = true;
	rapidity = false;
	resample = false;
	prerotate = true;
	calc_v2 = false;

	sim_eff = false;
	sim_flow = false;

	pile_up_prob = 0;
	efficiency_prob = 0;

	cent_binning = 9;
	ref_num = 2;
	energy = 7;

	refmultCorrUtil = new StRefMultCorr(("refmult" + to_string(ref_num)).data());

	mix = Mixer(energy);

//	c_rand = mt19937_64(trand->Integer(numeric_limits<int>::max()));
//	pois_dist = poisson_distribution<int> (1);
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

bool AzBinner::get_mixed() {
	return(mixed);
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

int AzBinner::get_cent_bins() {
	return cent_bins;
}

int AzBinner::get_cent_min() {
	return cent_min;
}

int AzBinner::get_particle_bins() {
	return particle_bins;
}

int AzBinner::get_particle_min() {
	return particle_min;
}


int AzBinner::get_n_bootstraps() {
	return n_bootstraps;
}

bool AzBinner::get_mix_empties() {
	return mix_empties;
}

int AzBinner::get_sys_ref_shift() {
	return sys_ref_shift;
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

//void AzBinner::set_sim_eff_dist_path(string root_path, string hist_name) {
//	if ((int)sim_eff_dist_path.size() > 0) { sim_eff_dist_path.clear(); }
//	sim_eff_dist_path.push_back(root_path);
//	sim_eff_dist_path.push_back(hist_name);
//}

void AzBinner::set_energy(int energy) {
	this->energy = energy;
}

void AzBinner::set_divs(vector<int> list) {
	divs = list;
}

void AzBinner::set_ampt(bool ampt) {
	this->ampt = ampt;
}

void AzBinner::set_cooper_frye(bool cf) {
	this->cooper_frye = cf;
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

void AzBinner::set_mixed(bool mixed) {
	this->mixed = mixed;
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

void AzBinner::set_resample(bool resample) {
	this->resample = resample;
}

void AzBinner::set_prerotate(bool prerotate) {
	this->prerotate = prerotate;
}

void AzBinner::set_calc_v2(bool calc_v2) {
	this->calc_v2 = calc_v2;
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

void AzBinner::set_resample_alg(int alg_num) {
	resample_alg = alg_num;
}

void AzBinner::set_n_resamples(int n) {
	n_resamples = n;
}

void AzBinner::set_n_bootstraps(int n) {
	n_bootstraps = n;
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
	file_shuffle_seed = seed;  // To be read in from TreeReader parent for writing in info file
}

void AzBinner::set_stref_rand_seed(int seed) {
	stref_seed = seed;
	refmultCorrUtil->set_rand_seed(seed);
}

void AzBinner::set_sim_pars(simulation_pars pars) {
	this->sim_pars = pars;
}

void AzBinner::set_cent_bins(int bins) {
	cent_bins = bins;
}

void AzBinner::set_cent_min(int min) {
	cent_min = min;
}

void AzBinner::set_particle_bins(int bins) {
	particle_bins = bins;
}

void AzBinner::set_particle_min(int min) {
	particle_min = min;
}

void AzBinner::set_mix_empties(bool mix_empties) {
	this->mix_empties = mix_empties;
}

void AzBinner::set_sys_ref_shift(int sys_ref_shift) {
	this->sys_ref_shift = sys_ref_shift;
}



// Doers

// Set everything necessary for read just in time (construction too early to set). 
void AzBinner::prep_read() {
	//cut.set_values(energy, particle);  // Resets values set in main, for now call in main, think of better way
	set_energy(energy);  // Not convinced this does anything?
	init_data();
	if (mixed) {
		mix.set_single_ratio(single_ratio);
		mix.set_n1_ratios(n1_ratios);
		mix.set_rand_rotate(rotate_random);
		mix.set_resample(resample);
		mix.set_resample_alg(resample_alg);
		mix.set_n_resamples(n_resamples);
		mix.set_n_bootstraps(n_bootstraps);
		mix.set_cent_bins(cent_bins);
		mix.set_cent_min(cent_min);
		mix.set_particle_bins(particle_bins);
		mix.set_particle_min(particle_min);
		mix.set_mutex(mtx);
		mix.init_data();
	}
}


void AzBinner::init_data() {
	if(mtx) { mtx->lock(); }
	for (unsigned div_i=0; div_i < divs.size(); div_i++) {
		data.push_back(vector<vector<vector<long>>> ());
		for (int cent_i=0; cent_i < cent_bins; cent_i++) {
			data[div_i].push_back(vector<vector<long>> ());
			for (int num_particles=0; num_particles < particle_bins; num_particles++) {
				data[div_i][cent_i].push_back(vector<long> (num_particles + particle_min + 1, 0));
			}
		}
	}

	if (resample && n_bootstraps > 0) {
		for (unsigned div_i = 0; div_i < divs.size(); div_i++) {
			data_bs.push_back(vector<vector<vector<vector<long>>>>());
			for (int cent_i = 0; cent_i < cent_bins; cent_i++) {
				data_bs[div_i].push_back(vector<vector<vector<long>>>());
				for (int bs_i = 0; bs_i < n_bootstraps; bs_i++) {
					data_bs[div_i][cent_i].push_back(vector<vector<long>>());
					for (int num_particles = 0; num_particles < particle_bins; num_particles++) {
						data_bs[div_i][cent_i][bs_i].push_back(vector<long>(num_particles + particle_min + 1, 0));
					}
				}
			}
		}
	}
	if(mtx) { mtx->unlock(); }
}


//void AzBinner::init_data() {  // Old implementation, see if it still crashes
//	for (unsigned div_i = 0; div_i < divs.size(); div_i++) {
//		data.push_back(vector<vector<vector<long>>>());
//		data_bs.push_back(vector<vector<vector<vector<long>>>>());
//		for (int cent_i = 0; cent_i < cent_bins; cent_i++) {
//			data[div_i].push_back(vector<vector<long>>());
//			data_bs[div_i].push_back(vector<vector<vector<long>>>());
//			for (int bs_i = 0; bs_i < n_bootstraps; bs_i++) {
//				data_bs[div_i][cent_i].push_back(vector<vector<long>>());
//				for (int num_particles = 0; num_particles < particle_bins; num_particles++) {
//					data_bs[div_i][cent_i][bs_i].push_back(vector<long>(num_particles + particle_min + 1, 0));
//				}
//			}
//			for (int num_particles = 0; num_particles < particle_bins; num_particles++) {
//				data[div_i][cent_i].push_back(vector<long>(num_particles + particle_min + 1, 0));
//			}
//		}
//	}
//}

// Genrate a list of random numbers in each event to use in general, reasmpling, and bootstrapping. 
void AzBinner::gen_single_randoms(vector<double>& random_list, vector<double>& random_bs_list) {
	int n_randoms = n_resamples * divs.size();
	//random_list.resize(n_randoms);
	//if (mtx) { mtx->lock(); }
	random_list.reserve(n_randoms);
	//if (mtx) { mtx->unlock(); }
	for (int i = 0; i < n_randoms; i++) {
		random_list.push_back(trand->Rndm());
		//trand->RndmArray(random_list.size(), &random_list[0]);
	}

	int n_bs_randoms = n_bootstraps * divs.size();
	//if (mtx) { mtx->lock(); }
	random_bs_list.reserve(n_bs_randoms);
	//if (mtx) { mtx->unlock(); }
	for (int i = 0; i < n_bs_randoms; i++) {
		//random_bs_list.push_back(trand->Poisson(1));
		random_bs_list.push_back(sample_poisson(trand->Rndm()));
	}
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
		vector<float> good_particle_etas = {};  // Needed for v2 calculation only

		// Iterate over particles in event and add corresponding phi to good_particle_angles if particle good.
		for (const Track& particle : event.get_particles()) {
			if (check_particle_good(particle)) {
				if (efficiency) {  // Skip good particle with chance efficiency_prob
					if (trand->Rndm() < efficiency_prob) { continue; }
				}
				//if (mtx) { mtx->lock(); }
				good_particle_angles.push_back(particle.get_phi());
				good_particle_etas.push_back(particle.get_eta());
				//if (mtx) { mtx->unlock(); }
			}
		}
		
		// Get centrality bin for event from ref_multn value for every event to keep random string the same between runs
		refmultCorrUtil->init(event.get_run());
		refmultCorrUtil->initEvent((int)event.get_refn() + sys_ref_shift, (double)event.get_vz());
		int cent16_corr = refmultCorrUtil->getCentralityBin16();
		int cent9_corr = refmultCorrUtil->getCentralityBin9();

		cent16_events.Fill(cent16_corr);
		cent9_events.Fill(cent9_corr);

		int cent;
		if (cent_binning == 16) {
			cent = cent16_corr;
		}
		else {
			cent = cent9_corr;
		}
		float ep_angle = event.get_event_plane();

		// Generate random lists for event
		vector<double> single_randoms, single_bs_randoms;
		gen_single_randoms(single_randoms, single_bs_randoms);  // Fill single randoms
		double rand_prerotate_angle = trand->Rndm() * 2 * M_PI;
		double rand_rotate_angle = trand->Rndm() * 2 * M_PI;

		if ((ampt || cooper_frye) && ampt_reaction_plane) { ep_angle = 0; }  // Ampt reaction plane is at zero.
		if ((ampt || cooper_frye) && prerotate) {  // Pre-random rotate event if ampt since all reaction planes are at zero.
			good_particle_angles = rotate_angles(good_particle_angles, rand_prerotate_angle);
			double ep_rotate = rotate_angle(ep_angle, rand_prerotate_angle);
			while (ep_rotate >= M_PI) { ep_rotate -= M_PI; }  // Force into range of [0, pi)
			while (ep_rotate < 0) { ep_rotate += M_PI; }
			ep_angle = ep_rotate;
		}

		// If there are enough good particles, calculate ratios for each division and save to data.
		int num_particles = (int)good_particle_angles.size();
		if (num_particles - particle_min >= particle_bins) { cout << "num_particles: " << num_particles << " too big for particle_bins: " << particle_bins << " !!!" << endl; }
		if (num_particles < cut.min_multi) {
			// If mixed/rand flagged append event to mix/rand object. If not enough particles just push in an empty set of angles to preserve randomization string.
			if (mixed && mix_empties) { mix.append_event({}, cent, ep_angle, event.get_vz()); }
		}
		else {
			event_cut_hist.Fill("Enough Good Particles", 1);

			post_n_particles[cent].Fill(num_particles);
			post_ref[cent].Fill(event.get_ref());
			post_refn[cent].Fill(event.get_refn());
			int cent_bin = cent - cent_min;

			pre_ep_hist.Fill(ep_angle);
			post_ep_hist.Fill(ep_angle);
			//if (!sim_flow) {
			//	TVector2 q(event.get_psi_east(), event.get_psi_west());
			//	pre_ep_hist.Fill(0.5 * q.Phi());
			//	for (double& angle : good_particle_angles) {  // Subtract contribution of particle of interest to mitigate autocorrelation.
			//		TVector2 q_new(cos(2 * angle), sin(2 * angle));
			//		q -= q_new;
			//	}
			//	ep_angle = 0.5 * q.Phi();
			//	post_ep_hist.Fill(ep_angle);
			//}
			//else {
			//	ep_angle = event.get_event_plane();
			//	pre_ep_hist.Fsill(ep_angle);
			//	post_ep_hist.Fill(ep_angle);
			//}

//			if (cbwc) { cent = event.get_refn(); }  // For centrality bin width correction use refmult n in place of centrality from here on. Not currently implemented after data structure changes

			if (calc_v2) {
				float res = cos(2 * (event.get_psi_east() - event.get_psi_west()));
				float v2_event = 0.;
				for (int i = 0; i < num_particles; i++) {
					float psi = good_particle_etas[i] >= 0 ? event.get_psi_west() : event.get_psi_east();
					v2_event += cos(2 * (good_particle_angles[i] - psi));
				}
				v2_event /= num_particles;
				v2[cent].Fill(num_particles, v2_event);
				resolution[cent].Fill(num_particles, res);
			}

			// If mixed/rand flagged append event to mix/rand object.
			if (mixed) { mix.append_event(good_particle_angles, cent, ep_angle, event.get_vz()); }

			if (event_plane) { // If event_plane flag then rotate all angles by -event_plane.
				good_particle_angles = rotate_angles(good_particle_angles, -ep_angle);
			}
			else if (rotate_random) { // If rotate_random flag then rotate all angles by random angle between 0 and 2pi
				good_particle_angles = rotate_angles(good_particle_angles, rand_rotate_angle);
			}

			int num_particles_bin = num_particles - particle_min;
			if (resample) {
				int single_random_index = 0, single_bs_random_index = 0;
				sort(good_particle_angles.begin(), good_particle_angles.end());
				for (unsigned div_bin=0; div_bin < divs.size(); div_bin++) {
					double div_rads = (double)divs[div_bin] / 180 * M_PI;
					vector<int> binned_event;
					if (resample_alg == 4) {
						vector<double> div_single_randoms(single_randoms.begin() + single_random_index, single_randoms.begin() + single_random_index + n_resamples);
						binned_event = get_resamples4(good_particle_angles, div_rads, n_resamples, div_single_randoms);
						single_random_index += n_resamples;
					} else if (resample_alg == 3) {
						binned_event = get_resamples3(good_particle_angles, div_rads, n_resamples);
					}
					else { cout << "Didn't find matching resample algorithm for #" << resample_alg << endl; }

					// Save binned values to data
					vector<long> &data_event = data[div_bin][cent_bin][num_particles_bin];  // Reduce map traversal
					for (unsigned num_in_bin=0; num_in_bin < binned_event.size(); num_in_bin++) {
						data_event[num_in_bin] += binned_event[num_in_bin];
					}

					// Save binned values to bootstraps
					for (int i = 0; i < n_bootstraps; i++) {
						vector<long> &data_event_bs = data_bs[div_bin][cent_bin][i][num_particles_bin];
						int poisson_samples = single_bs_randoms[single_bs_random_index++];
						for (int j = 0; j <= poisson_samples; j++) {  // Poisson block bootstrap
							for (unsigned num_in_bin=0; num_in_bin < binned_event.size(); num_in_bin++) {
								data_event_bs[num_in_bin] += binned_event[num_in_bin];
							}
						}
					}
				}
			}
			else {  // Not ready for systematics! Need to extend random stability.
				for (unsigned div_bin=0; div_bin < divs.size(); div_bin++) {
					int bin_num = (int)360 / divs[div_bin];
					double div_rads = (double)divs[div_bin] / 180 * M_PI;
					if (single_ratio) { bin_num = 1; }
					else if (bin_num > 1 && n1_ratios) { bin_num -= 1; }  // Ambiguous if case should change if div divides 360 or not.
					vector<int> event_ratios = get_Rs(good_particle_angles, div_rads, trand, bin_num);  // Convert particle angles in event to ratio values.

					// Save ratio values to data
					vector<long> &data_event = data[div_bin][cent_bin][num_particles_bin];  // Reduce map traversal
					for (int particles_in_bin : event_ratios) {
						data_event[particles_in_bin]++;
					}
				}
			}
		}
	}
}


//  For good events/tracks, azimuthally bin particles and save to data
void AzBinner::process_event_debug(const Event& event) {
	// Check if each event is good. Analyze if so, continue if not.
	if (check_event(event)) {
		vector<double> good_particle_angles = {};
		vector<float> good_particle_etas = {};  // Needed for v2 calculation only

		// Iterate over particles in event and add corresponding phi to good_particle_angles if particle good.
		for (const Track& particle : event.get_particles()) {
			if (check_particle_good(particle)) {
				if (efficiency) {  // Skip good particle with chance efficiency_prob
					if (trand->Rndm() < efficiency_prob) { continue; }
				}
				good_particle_angles.push_back(particle.get_phi());
				good_particle_etas.push_back(particle.get_eta());
			}
		}

		// Get centrality bin for event from ref_multn value for every event to keep random string the same between runs
		refmultCorrUtil->init(event.get_run());
		refmultCorrUtil->initEvent((int)event.get_refn(), (double)event.get_vz());
		int cent16_corr = refmultCorrUtil->getCentralityBin16();
		int cent9_corr = refmultCorrUtil->getCentralityBin9();

		cent16_events.Fill(cent16_corr);
		cent9_events.Fill(cent9_corr);

		int cent;
		if (cent_binning == 16) {
			cent = cent16_corr;
		}
		else {
			cent = cent9_corr;
		}
		float ep_angle = event.get_event_plane();

		// Generate random lists for event
		vector<double> single_randoms, single_bs_randoms;
		gen_single_randoms(single_randoms, single_bs_randoms);  // Fill single randoms
		double rand_prerotate_angle = trand->Rndm() * 2 * M_PI;
		double rand_rotate_angle = trand->Rndm() * 2 * M_PI;
		if (event.get_event_id() >= 2343 && event.get_event_id() < 2363) {
			cout << single_randoms[0] << endl;
		}

		if ((ampt || cooper_frye) && ampt_reaction_plane) { ep_angle = 0; }  // Ampt reaction plane is at zero.
		if ((ampt || cooper_frye) && prerotate) {  // Pre-random rotate event if ampt since all reaction planes are at zero.
			good_particle_angles = rotate_angles(good_particle_angles, rand_prerotate_angle);
			double ep_rotate = rotate_angle(ep_angle, rand_prerotate_angle);
			while (ep_rotate >= M_PI) { ep_rotate -= M_PI; }  // Force into range of [0, pi)
			while (ep_rotate < 0) { ep_rotate += M_PI; }
			ep_angle = ep_rotate;
		}

		// If there are enough good particles, calculate ratios for each division and save to data.
		int num_particles = (int)good_particle_angles.size();
		if (num_particles - particle_min >= particle_bins) { cout << "num_particles: " << num_particles << " too big for particle_bins: " << particle_bins << " !!!" << endl; }
		if (num_particles < cut.min_multi) {
			// If mixed/rand flagged append event to mix/rand object. If not enough particles just push in an empty set of angles to preserve randomization string.
			if (mixed && mix_empties){
				if (event.get_event_id() >= 2343 && event.get_event_id() < 2363) {
					mix.append_event_debug({}, cent, ep_angle, event.get_vz());
				}
				else {
					mix.append_event({}, cent, ep_angle, event.get_vz());
				}
			}
		}
		else {
			event_cut_hist.Fill("Enough Good Particles", 1);

			post_n_particles[cent].Fill(num_particles);
			post_ref[cent].Fill(event.get_ref());
			post_refn[cent].Fill(event.get_refn());
			int cent_bin = cent - cent_min;

			pre_ep_hist.Fill(ep_angle);
			post_ep_hist.Fill(ep_angle);
			//if (!sim_flow) {
			//	TVector2 q(event.get_psi_east(), event.get_psi_west());
			//	pre_ep_hist.Fill(0.5 * q.Phi());
			//	for (double& angle : good_particle_angles) {  // Subtract contribution of particle of interest to mitigate autocorrelation.
			//		TVector2 q_new(cos(2 * angle), sin(2 * angle));
			//		q -= q_new;
			//	}
			//	ep_angle = 0.5 * q.Phi();
			//	post_ep_hist.Fill(ep_angle);
			//}
			//else {
			//	ep_angle = event.get_event_plane();
			//	pre_ep_hist.Fsill(ep_angle);
			//	post_ep_hist.Fill(ep_angle);
			//}

//			if (cbwc) { cent = event.get_refn(); }  // For centrality bin width correction use refmult n in place of centrality from here on. Not currently implemented after data structure changes

			if (calc_v2) {
				float res = cos(2 * (event.get_psi_east() - event.get_psi_west()));
				float v2_event = 0.;
				for (int i = 0; i < num_particles; i++) {
					float psi = good_particle_etas[i] >= 0 ? event.get_psi_west() : event.get_psi_east();
					v2_event += cos(2 * (good_particle_angles[i] - psi));
				}
				v2_event /= num_particles;
				v2[cent].Fill(num_particles, v2_event);
				resolution[cent].Fill(num_particles, res);
			}

			// If mixed/rand flagged append event to mix/rand object.
			if (mixed) { 
				if (event.get_event_id() >= 2343 && event.get_event_id() < 2363) {
					mix.append_event_debug(good_particle_angles, cent, ep_angle, event.get_vz());
				}
				else {
					mix.append_event(good_particle_angles, cent, ep_angle, event.get_vz());
				}
			}

			if (event_plane) { // If event_plane flag then rotate all angles by -event_plane.
				good_particle_angles = rotate_angles(good_particle_angles, -ep_angle);
			}
			else if (rotate_random) { // If rotate_random flag then rotate all angles by random angle between 0 and 2pi
				good_particle_angles = rotate_angles(good_particle_angles, rand_rotate_angle);
			}

			int num_particles_bin = num_particles - particle_min;
			if (resample) {
				int single_random_index = 0, single_bs_random_index = 0;
				sort(good_particle_angles.begin(), good_particle_angles.end());
				for (unsigned div_bin = 0; div_bin < divs.size(); div_bin++) {
					double div_rads = (double)divs[div_bin] / 180 * M_PI;
					vector<int> binned_event;
					if (resample_alg == 4) {
						vector<double> div_single_randoms(single_randoms.begin() + single_random_index, single_randoms.begin() + single_random_index + n_resamples);
						binned_event = get_resamples4(good_particle_angles, div_rads, n_resamples, div_single_randoms);
						single_random_index += n_resamples;
					}
					else if (resample_alg == 3) {
						binned_event = get_resamples3(good_particle_angles, div_rads, n_resamples);
					}
					else { cout << "Didn't find matching resample algorithm for #" << resample_alg << endl; }

					// Save binned values to data
					vector<long>& data_event = data[div_bin][cent_bin][num_particles_bin];  // Reduce map traversal
					for (unsigned num_in_bin = 0; num_in_bin < binned_event.size(); num_in_bin++) {
						data_event[num_in_bin] += binned_event[num_in_bin];
					}

					// Save binned values to bootstraps
					for (int i = 0; i < n_bootstraps; i++) {
						vector<long>& data_event_bs = data_bs[div_bin][cent_bin][i][num_particles_bin];
						int poisson_samples = single_bs_randoms[single_bs_random_index++];
						for (int j = 0; j <= poisson_samples; j++) {  // Poisson block bootstrap
							for (unsigned num_in_bin = 0; num_in_bin < binned_event.size(); num_in_bin++) {
								data_event_bs[num_in_bin] += binned_event[num_in_bin];
							}
						}
					}
				}
			}
			else {  // Not ready for systematics! Need to extend random stability.
				for (unsigned div_bin = 0; div_bin < divs.size(); div_bin++) {
					int bin_num = (int)360 / divs[div_bin];
					double div_rads = (double)divs[div_bin] / 180 * M_PI;
					if (single_ratio) { bin_num = 1; }
					else if (bin_num > 1 && n1_ratios) { bin_num -= 1; }  // Ambiguous if case should change if div divides 360 or not.
					vector<int> event_ratios = get_Rs(good_particle_angles, div_rads, trand, bin_num);  // Convert particle angles in event to ratio values.

					// Save ratio values to data
					vector<long>& data_event = data[div_bin][cent_bin][num_particles_bin];  // Reduce map traversal
					for (int particles_in_bin : event_ratios) {
						data_event[particles_in_bin]++;
					}
				}
			}
		}
	}
}


//  For good events/tracks, azimuthally bin particles and save to data
void AzBinner::process_event_pt_n(const Event& event) {
	// Check if each event is good. Analyze if so, continue if not.
	if (check_event(event)) {
		vector<double> good_particle_angles = {};
		vector<double> good_particle_pts = {};

		// Iterate over particles in event and add corresponding phi to good_particle_angles if particle good.
		for (const Track& particle : event.get_particles()) {
			if (check_particle_good(particle)) {
				if (efficiency) {  // Skip good particle with chance efficiency_prob
					if (trand->Rndm() < efficiency_prob) { continue; }
				}
				good_particle_angles.push_back(particle.get_phi());
				good_particle_pts.push_back(particle.get_pt());
			}
		}

		// Get centrality bin for event from ref_multn value
		refmultCorrUtil->init(event.get_run());
		refmultCorrUtil->initEvent((int)event.get_refn(), (double)event.get_vz());
		int cent16_corr = refmultCorrUtil->getCentralityBin16();
		int cent9_corr = refmultCorrUtil->getCentralityBin9();

		// If there are enough good particles, calculate ratios for each division and save to data.
		int num_particles = (int)good_particle_angles.size();
		if (num_particles - particle_min >= particle_bins) { cout << "num_particles: " << num_particles << " too big for particle_bins: " << particle_bins << " !!!" << endl; }
		if (num_particles >= cut.min_multi) {
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
			post_n_particles[cent].Fill(num_particles);
			int cent_bin = cent - cent_min;

			TVector2 q(event.get_psi_east(), event.get_psi_west());
			pre_ep_hist.Fill(0.5 * q.Phi());
			for (double& angle : good_particle_angles) {  // Subtract contribution of particle of interest to mitigate autocorrelation.
				TVector2 q_new(cos(2 * angle), sin(2 * angle));
				q -= q_new;
			}
			float ep_angle = 0.5 * q.Phi();
			post_ep_hist.Fill(ep_angle);

			//			if (cbwc) { cent = event.get_refn(); }  // For centrality bin width correction use refmult n in place of centrality from here on. Not currently implemented after data structure changes

			if ((ampt || cooper_frye) && prerotate) {  // Pre-random rotate event if ampt since all reaction planes are at zero.
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


			if (event_plane) { // If event_plane flag then rotate all angles by -event_plane.
				good_particle_angles = rotate_angles(good_particle_angles, -ep_angle);
			}
			else if (rotate_random) { // If rotate_random flag then rotate all angles by random angle between 0 and 2pi
				double rand_angle = trand->Rndm() * 2 * M_PI;
				good_particle_angles = rotate_angles(good_particle_angles, rand_angle);
				//q.Rotate(rand_angle);  // Untested
				//event.set_event_plane(rotate_angle(event_plane, rand_angle));  // Need to rotate event plane if it's used after this point. Currently is not.
			}

			int num_particles_bin = num_particles - particle_min;
			if (resample) {
				sort(good_particle_angles.begin(), good_particle_angles.end());
				for (unsigned div_bin = 0; div_bin < divs.size(); div_bin++) {
					double div_rads = (double)divs[div_bin] / 180 * M_PI;
					vector<int> binned_event = get_resamples3(good_particle_angles, div_rads, n_resamples);

					// Save binned values to data
					vector<long>& data_event = data[div_bin][cent_bin][num_particles_bin];  // Reduce map traversal
					for (unsigned num_in_bin = 0; num_in_bin < binned_event.size(); num_in_bin++) {
						data_event[num_in_bin] += binned_event[num_in_bin];
					}

					// Save binned values to bootstraps
					for (int i = 0; i < n_bootstraps; i++) {
						vector<long>& data_event_bs = data_bs[div_bin][cent_bin][i][num_particles_bin];
						for (int j = 0; j < trand->Poisson(1); j++) {  // Poisson block bootstrap
							for (unsigned num_in_bin = 0; num_in_bin < binned_event.size(); num_in_bin++) {
								data_event_bs[num_in_bin] += binned_event[num_in_bin];
							}
						}
					}
				}
			}
			else {
				for (unsigned div_bin = 0; div_bin < divs.size(); div_bin++) {
					int bin_num = (int)360 / divs[div_bin];
					double div_rads = (double)divs[div_bin] / 180 * M_PI;
					if (single_ratio) { bin_num = 1; }
					else if (bin_num > 1 && n1_ratios) { bin_num -= 1; }  // Ambiguous if case should change if div divides 360 or not.
					vector<int> event_ratios = get_Rs(good_particle_angles, div_rads, trand, bin_num);  // Convert particle angles in event to ratio values.

					// Save ratio values to data
					vector<long>& data_event = data[div_bin][cent_bin][num_particles_bin];  // Reduce map traversal
					for (int particles_in_bin : event_ratios) {
						data_event[particles_in_bin]++;
					}
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

	if (!check_vz(event.get_vz())) return false;
	event_cut_hist.Fill("Good Vz", 1);

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


// Returns true if vz within vz cut, false if outside of range.
bool AzBinner::check_vz(float vz) {
	if (vz >= cut.vz_cut.first && vz <= cut.vz_cut.second) {
		return true;
	}
	return false;
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

	event_cut_hist = TH1D(("event_cut" + set_name + "_" + to_string(energy)).data(), "Event Cuts", 7, -0.5, 6.5);
	event_cut_hist.GetXaxis()->SetBinLabel(1, "Original");
	event_cut_hist.GetXaxis()->SetBinLabel(2, "Good Dca_xy");
	event_cut_hist.GetXaxis()->SetBinLabel(3, "Good Run");
	event_cut_hist.GetXaxis()->SetBinLabel(4, "Enough Particles");
	event_cut_hist.GetXaxis()->SetBinLabel(5, "Pile Up Rejected");
	event_cut_hist.GetXaxis()->SetBinLabel(6, "Good Vz");
	event_cut_hist.GetXaxis()->SetBinLabel(7, "Enough Good Particles");

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
		post_ref[i] = TH1D(("RefMult_" + set_name + "_" + to_string(energy) + "_" + to_string(i)).data(), "Reference Multiplicity", 801, -0.5, 800.5);
		post_refn[i] = TH1D(("RefMultn_" + set_name + "_" + to_string(energy) + "_" + to_string(i)).data(), "Reference Multiplicity N", 801, -0.5, 800.5);
		if (calc_v2) {
			v2[i] = TProfile(("v2_" + set_name + "_" + to_string(energy) + "_" + to_string(i)).data(), "v2", 101, -0.5, 100.5);
			resolution[i] = TProfile(("resolution_" + set_name + "_" + to_string(energy) + "_" + to_string(i)).data(), "Resolution", 101, -0.5, 100.5);
		}
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
	if (resample) {
		write_tree_data_bootstrap("local", data, data_bs, divs, cent_min, particle_min, out_path + to_string(energy) + "GeV/");
	}
	else {
		write_tree_data("local", data, divs, cent_min, particle_min, out_path + to_string(energy) + "GeV/");
	}
	if (mixed) { mix.write_mixed_data(); }
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
		out << "dca_xy qa path: " << cut.get_dcaqa_path() << endl;
		out << "pileup qa path: " << cut.get_pileupqa_path() << endl;

		out << "energy: " << to_string(energy) << endl;
		out << "divs: { ";
		for (auto& div : divs) { out << to_string(div) << " "; }
		out << "}" << endl;

		out << "particle: " << particle << endl;

		out << "ampt: " << boolalpha << ampt << endl;
		out << "cooper_frye: " << boolalpha << cooper_frye << endl;
		out << "ampt_reaction_plane: " << boolalpha << ampt_reaction_plane << endl;
		out << "cbwc: " << boolalpha << cbwc << endl;
		out << "rotate_random: " << boolalpha << rotate_random << endl;
		out << "event_plane: " << boolalpha << event_plane << endl;
		out << "mixed: " << boolalpha << mixed << endl;
		out << "pile_up: " << boolalpha << pile_up << endl;
		out << "efficiency: " << boolalpha << efficiency << endl;
		out << "single_ratio: " << boolalpha << single_ratio << endl;
		out << "n1_ratios: " << boolalpha << n1_ratios << endl;
		out << "sim_eff: " << boolalpha << sim_eff << endl;
		out << "sim_flow: " << boolalpha << sim_flow << endl;
		out << "check_charge: " << boolalpha << check_charge << endl;
		out << "rapidity: " << boolalpha << rapidity << endl;
		out << "prerotate: " << boolalpha << prerotate << endl;
		out << "calc_v2: " << boolalpha << calc_v2 << endl;

		out << "pile_up_prob: " << pile_up_prob << endl;
		out << "efficiency_prob: " << efficiency_prob << endl;

		out << "cent_binning: " << cent_binning << endl;
		out << "ref_num: " << ref_num << endl;
		out << "sys_ref_shift: " << sys_ref_shift << endl;

		out << "cent_bins: " << cent_bins << endl;
		out << "cent_min: " << cent_min << endl;
		out << "particle_bins: " << particle_bins << endl;
		out << "particle_min: " << particle_min << endl;

		out << "tree_reader_seed: " << tree_reader_seed << endl;
		out << "mixer_seed: " << mixer_seed << endl;
		out << "file_shuffle_seed: " << file_shuffle_seed << endl;
		out << "stref_seed: " << stref_seed << endl;

		out << "resample: " << boolalpha << resample << endl;
		out << "resample algorithm: " << to_string(resample_alg) << endl;
		out << "n_resamples: " << to_string(n_resamples) << endl;
		out << "n_bootstraps: " << to_string(n_bootstraps) << endl;
		out << "mix resample: " << boolalpha << mix.get_resample() << endl;
		out << "mix n_resamples: " << to_string(mix.get_n_resamples()) << endl;
		out << "mix n_bootstraps: " << to_string(mix.get_n_bootstraps()) << endl;
		out << "mix empties: " << boolalpha << mix_empties << endl;


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
		out << "min_vz: " << to_string(cut.vz_cut.first) << endl;
		out << "max_vz: " << to_string(cut.vz_cut.second) << endl;

		out << "min_multi: " << to_string(cut.min_multi) << endl;

		out << "sim p_group: " << to_string(sim_pars.p_group) << endl;
		out << "sim spread_sigma: " << to_string(sim_pars.spread_sigma) << endl;
		out << "sim amp_group: " << to_string(sim_pars.amp_group) << endl;
		out << "sim base: " << to_string(sim_pars.base) << endl;
		out << "sim points: " << to_string(sim_pars.points) << endl;
		out << "sim wrap sigmas: " << to_string(sim_pars.wrap_sigmas) << endl;
		out << "sim x_low: " << to_string(sim_pars.x_low) << endl;
		out << "sim x_up: " << to_string(sim_pars.x_up) << endl;
		out << "sim min_protons: " << to_string(sim_pars.min_protons) << endl;
		out << "sim proton_dist: " << sim_pars.proton_dist << endl;
		out << "sim particle_mean: " << to_string(sim_pars.particle_mean) << endl;
		out << "sim particle_max: " << to_string(sim_pars.particle_max) << endl;
		out << "sim hom_eff: " << to_string(sim_pars.hom_eff) << endl;
		out << "sim v2: " << to_string(sim_pars.v2) << endl;
		out << "sim ep_res: " << to_string(sim_pars.ep_res) << endl;

		out << "mix single_ratio: " << boolalpha << mix.get_single_ratio() << endl;
		out << "mix n1_ratios: " << boolalpha << mix.get_n1_ratios() << endl;
		out << "mix rand_rotate: " << boolalpha << mix.get_rand_rotate() << endl;
		out << "mix resample: " << boolalpha << mix.get_resample() << endl;
		out << "mix resample algorithm: " << to_string(mix.get_resample_alg()) << endl;
		out << "mix n_resamples: " << mix.get_n_resamples() << endl;
		out << "mix n_bootstraps: " << mix.get_n_bootstraps() << endl;
		out << "mix event_plane_rotate: " << boolalpha << mix.get_event_plane_rotate() << endl;
		out << "mix max_events: " << to_string(mix.get_max_events()) << endl;
		out << "mix min_events: " << to_string(mix.get_min_events()) << endl;
		out << "mix mixes_per_event: " << to_string(mix.get_mixes_per_event()) << endl;
		out << "mix vz_bins: " << to_string(mix.get_vz_bins()) << endl;
		out << "mix ep_bins: " << to_string(mix.get_ep_bins()) << endl;
		out << "mix vz_range: " << to_string(mix.get_vz_range().first) << ", " << to_string(mix.get_vz_range().second) << endl;
		out << "mix ep_range: " << to_string(mix.get_ep_range().first) << ", " << to_string(mix.get_ep_range().second) << endl;
		out << "mix cent_bins: " << mix.get_cent_bins() << endl;
		out << "mix cent_min: " << mix.get_cent_min() << endl;
		out << "mix particle_bins: " << mix.get_particle_bins() << endl;
		out << "mix particle_min: " << mix.get_particle_min() << endl;

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

	for (auto hist : post_n_particles) { hist.second.Write(); }
	for (auto hist : post_ref) { hist.second.Write(); }
	for (auto hist : post_refn) { hist.second.Write(); }

	if (calc_v2) {
		for (pair<int, TProfile> cent : v2) {
			vector<float> v2_cor_cent;
			vector<float> num_particles;
			vector<float> v2_cor_err;
			vector<float> npart_err;
			for (int bin = 0; bin < cent.second.GetXaxis()->GetNbins(); bin++) {
				if (cent.second.GetBinEntries(bin) >= 1) {
					v2_cor_cent.push_back(cent.second.GetBinContent(bin) / pow(resolution[cent.first].GetBinContent(bin), 0.5));
					num_particles.push_back(cent.second.GetBinCenter(bin));
					v2_cor_err.push_back(cent.second.GetBinError(bin) / pow(resolution[cent.first].GetBinContent(bin), 0.5));  // Apparently people don't propagate resolution error
					npart_err.push_back(0);
				}
			}
			cent.second.Write();
			resolution[cent.first].Write();
			TCanvas v2_cor_can(("V2_Corrected_Canvas_Cent_" + to_string(cent.first)).data());
			TGraphErrors v2_cor_errs((int)v2_cor_cent.size(), num_particles.data(), v2_cor_cent.data(), npart_err.data(), v2_cor_err.data());
			v2_cor_errs.Draw();
			v2_cor_can.Write();
		}

		vector<float> cent_bin, cent_bin_err, v2_cent, v2_cent_err, v2_cor_cent, v2_cor_cent_err, res_cent, res_cent_err;
		for (auto prof : v2) { 
			prof.second.Rebin(101); 
			float v2_int = prof.second.GetBinContent(1);
			float v2_int_err = prof.second.GetBinError(1);
			resolution[prof.first].Rebin(101);
			float res_int = pow(resolution[prof.first].GetBinContent(1), 0.5);
			float res_int_err = fabs(resolution[prof.first].GetBinError(1) / (2 * res_int));

			cent_bin.push_back(prof.first);
			cent_bin_err.push_back(0.0);
			v2_cent.push_back(v2_int);
			v2_cent_err.push_back(v2_int_err);
			v2_cor_cent.push_back(v2_int / res_int);
			v2_cor_cent_err.push_back(v2_int_err / res_int);  // Assuming negligable error on resolution, apparently what people do
			res_cent.push_back(res_int);
			res_cent_err.push_back(res_int_err);
		}

		TCanvas v2_cent_can("V2_Integrated_vs_Centrality_Bin");
		TGraphErrors v2_cent_graph((int)v2_cent.size(), cent_bin.data(), v2_cent.data(), cent_bin_err.data(), v2_cent_err.data());
		v2_cent_graph.SetTitle("v2 Uncorrected vs Centrality Bin;Centrality Bin;v2 uncorrected");
		v2_cent_graph.SetName("v2_Uncor_vs_Cent");
		v2_cent_graph.SetMarkerStyle(20);
		v2_cent_graph.SetMarkerColor(kBlue);
		v2_cent_graph.Draw();
		v2_cent_can.Write();

		TCanvas res_cent_can("Resolution_Integrated_vs_Centrality_Bin");
		TGraphErrors res_cent_graph((int)v2_cent.size(), cent_bin.data(), res_cent.data(), cent_bin_err.data(), res_cent_err.data());
		res_cent_graph.SetTitle("Resolution vs Centrality Bin;Centrality Bin;Event Plane Resolution");
		res_cent_graph.SetName("Res_vs_Cent");
		res_cent_graph.SetMarkerStyle(20);
		res_cent_graph.SetMarkerColor(kBlue);
		res_cent_graph.Draw();
		res_cent_can.Write();

		TCanvas v2_cor_cent_can("V2_Corrected_Integrated_vs_Centrality_Bin");
		TGraphErrors v2_cor_cent_graph((int)v2_cent.size(), cent_bin.data(), v2_cor_cent.data(), cent_bin_err.data(), v2_cor_cent_err.data());
		v2_cor_cent_graph.SetTitle("v2 Corrected vs Centrality Bin;Centrality Bin;v2 corrected");
		v2_cor_cent_graph.SetName("v2_Cor_vs_Cent");
		v2_cor_cent_graph.SetMarkerStyle(20);
		v2_cor_cent_graph.SetMarkerColor(kBlue);
		v2_cor_cent_graph.Draw();
		v2_cor_cent_can.Write();

		ofstream v2_out_file(qa_path + "v2.txt");
		v2_out_file << "centrality\tv2\tresolution\n";
		for (unsigned i = 0; i < cent_bin.size(); i++) {
			v2_out_file << cent_bin[i] << "\t" << v2_cor_cent[i] << " " << v2_cor_cent_err[i] << "\t" << res_cent[i] << " " << res_cent_err[i] << "\n";
		}
		v2_out_file.close();
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
