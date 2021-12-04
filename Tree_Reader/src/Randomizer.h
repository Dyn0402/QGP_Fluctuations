/*
 * Mixer_Roli.h
 *
 *  Created on: Oct 16, 2019
 *      Author: Dylan Neff
 */

#ifndef RANDOMIZER_H_
#define RANDOMIZER_H_

#include <map>
#include <vector>

#include <TRandom3.h>

using namespace std;


class Randomizer {
public:
	// Structors
	Randomizer();

	// Getters
	string get_out_path();
	vector<int> get_divs();

	// Setters
	void set_out_path(string path);
	void set_divs(vector<int>);

	// Doers
	void append_event(int num_protons, int cent, TRandom3 *trand);
	void reset_out_dir();
	void write_random_data();

	// Attributes

private:
	// Attributes
	map<int, map<int, map<int, map<int, long>>>> data; //ratios[divisions][centrality][num protons in event][num protons in bin]
	vector<int> divs;
	string out_path;

	// Doers
};


#endif /* RANDOMIZER_H_ */
