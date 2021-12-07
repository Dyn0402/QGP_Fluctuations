/*
 * Mixer.h
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */

#ifndef MIXER_H_
#define MIXER_H_

#include <map>
#include <vector>
#include <numeric>
#include <random>

#include <TRandom3.h>
#include <TH2.h>

using namespace std;


class Mixer {
public:
	// Structors
	Mixer();
	Mixer(int energy);
	Mixer(int energy, bool single_ratio, bool rand_rotate);

	// Getters
	bool get_single_ratio();
	bool get_n1_ratios();
	bool get_rand_rotate();
	bool get_event_plane_rotate();
	bool get_resample();
	int get_energy();
	int get_max_events();
	int get_min_events();
	int get_mixes_per_event();
	int get_vz_bins();
	int get_ep_bins();
	int get_n_resamples();
	int get_n_bootstraps();
	pair<double, double> get_vz_range();
	pair<double, double> get_ep_range();
	string get_out_path();
	vector<int> get_divs();

	// Setters
	void set_single_ratio(bool single_ratio);
	void set_n1_ratios(bool n1_ratios);
	void set_rand_rotate(bool rand_rotate);
	void set_event_plane_rotate(bool event_plane_rotate);
	void set_resample(bool resample);
	void set_energy(int energy);
	void set_max_events(int max_events);
	void set_min_events(int min_events);
	void set_mixes_per_event(int mixes_per_event);
	void set_vz_bins(int vz_bins);
	void set_ep_bins(int ep_bins);
	void set_vz_range(double vz_lower, double vz_upper);
	void set_ep_range(double ep_lower, double ep_upper);
	void set_out_path(string path);
	void set_divs(vector<int> divs);
	void set_rand_seed(int seed=0);
	void set_n_resamples(int n);
	void set_n_bootstraps(int n);

	// Doers
	void append_event(const vector<double>& angles, int cent, double event_plane, double vz);
	void reset_out_dir();
	void write_mixed_data();
	void write_qa();

	// Attributes

private:
	// Attributes
	bool single_ratio;
	bool n1_ratios;
	bool rand_rotate;
	bool event_plane_rotate;  // It doesn't look like this does anything
	bool resample;
	int energy;
	int min_events;
	int max_events;
	int mixes_per_event;
	int vz_bins;
	int ep_bins;
	pair<double, double> vz_range;
	pair<double, double> ep_range;
	map<int, map<int, map<int, map<int, long>>>> data; //ratios[divisions][centrality][num protons in event][num protons in bin]
	map<int, map<int, map<int, map<int, map<int, long>>>>> data_bs; //ratios[divisions][centrality][bootstrap #][num particles in event][num particles in bin]
	map<int, map<int, map<int, vector<vector<double>>>>> angles; //angles[centrality][event_plane][vz]
	vector<int> divs;
	string out_path;
	TRandom3 *trand = new TRandom3(0);
	mt19937 c_rand;

	int n_resamples = 100;
	int n_bootstraps = 100;

	// Doers
	void get_mixed(int cent, int num_protons, int ep_bin, int vz_bin);
	int get_ep_bin(double event_plane);
	int get_vz_bin(double vz);
	void define_hists(int cent);


	// QA Plots
	map<int, TH2I> vz_ep_appended_hists;  // Number of appended events, key is centrality
	map<int, TH2I> vz_ep_generated_hists;  // Number of generated events, key is centrality
};


#endif /* MIXER_H_ */
