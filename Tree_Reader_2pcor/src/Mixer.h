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

#include <TRandom3.h>
#include <TH1.h>

using namespace std;


class Mixer {
public:
	// Structors
	Mixer();
	Mixer(int energy);

	// Getters
	bool get_rand_rotate();
	bool get_event_plane_rotate();
	int get_energy();
	int get_max_events();
	int get_min_events();
	int get_mixes_per_event();
	int get_vz_bins();
	int get_ep_bins();
	pair<double, double> get_vz_range();
	pair<double, double> get_ep_range();
	string get_out_path();
	map<int, map<int, TH1D>> get_data();

	// Setters
	void set_rand_rotate(bool rand_rotate);
	void set_event_plane_rotate(bool event_plane_rotate);
	void set_energy(int energy);
	void set_max_events(int max_events);
	void set_min_events(int min_events);
	void set_mixes_per_event(int mixes_per_event);
	void set_vz_bins(int vz_bins);
	void set_ep_bins(int ep_bins);
	void set_vz_range(double vz_lower, double vz_upper);
	void set_ep_range(double ep_lower, double ep_upper);
	void set_hist_bins(int bins, double x_low, double x_high);

	// Doers
	void append_event(const vector<double>& angles, int cent, double event_plane, double vz);

	// Attributes

private:
	// Attributes
	bool rand_rotate;
	bool event_plane_rotate;
	int energy;
	int min_events;
	int max_events;
	int mixes_per_event;
	int vz_bins;
	int ep_bins;
	pair<double, double> vz_range;
	pair<double, double> ep_range;
	int hist_nbins;
	double hist_xlow;
	double hist_xhigh;
	map<int, map<int, TH1D>> data; //2pcorhist[centrality][num protons in event]
	map<int, map<int, map<int, vector<vector<double>>>>> angles; //angles[centrality][event_plane][vz]
	TRandom3 *trand = new TRandom3(0);

	// Doers
	void get_mixed(int cent, int num_protons, int ep_bin, int vz_bin);
	int get_ep_bin(double event_plane);
	int get_vz_bin(double vz);
	pair<int, int> generate_index(const vector<pair<int, int>>& used_angles, int cent, int ep_bin, int vz_bin);
};


#endif /* MIXER_H_ */
