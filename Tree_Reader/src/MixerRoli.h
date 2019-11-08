/*
 * Mixer_Roli.h
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */

#ifndef MIXERROLI_H_
#define MIXERROLI_H_

#include <map>
#include <vector>

using namespace std;


class MixerRoli {
public:
	// Structors
	MixerRoli();

	// Getters
	int get_max_events();
	string get_out_path();
	vector<int> get_divs();

	// Setters
	void set_max_events(int max_events);
	void set_out_path(string path);
	void set_divs(vector<int>);

	// Doers
	void append_event(vector<double> angles, int cent, int ref_mult2, double event_plane);
	void reset_out_dir();
	void write_mixed_data();

	// Attributes

private:
	// Attributes
	int max_events;
	map<int, map<int, map<int, map<int, int>>>> data; //ratios[divisions][centrality][num protons in event][num protons in bin]
	map<int, map<int, vector<vector<double>>>> angles;
	vector<int> divs;
	string out_path;

	// Doers
	void get_mixed(int cent, int num_protons, int ref_mult2, int ep_bin);
};


#endif /* MIXERROLI_H_ */
