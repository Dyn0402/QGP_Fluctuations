/*
 * Mixer.h
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */

#ifndef MIXERSETS_H_
#define MIXERSETS_H_

#include <map>
#include <vector>

using namespace std;


class MixerSets {
public:
	// Structors
	MixerSets();

	// Getters
	int get_max_events();
	bool get_use_leftover();
	string get_out_path();
	vector<int> get_divs();

	// Setters
	void set_max_events(int max_events);
	void set_use_leftover(bool use_leftover);
	void set_out_path(string path);
	void set_divs(vector<int>);

	// Doers
	void append_event(vector<double> angles, int ref_mult2);
	void reset_out_dir();
	void write_mixed_data();

	// Attributes

private:
	// Attributes
	int max_events;
	bool use_leftover;
	map<int, map<int, map<int, map<int, int>>>> data; //ratios[divisions][centrality][num protons in event][num protons in bin]
	map<int, vector<double>> angles;
	map<int, vector<int>> num_angles;
	vector<int> divs;
	string out_path;

	// Doers
	void mix_set(int ref_mult2);
};


#endif /* MIXERSETS_H_ */
