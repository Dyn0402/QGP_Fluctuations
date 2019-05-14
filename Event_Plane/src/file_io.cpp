/*
 * file_io.cpp
 *
 *  Created on: May 12, 2019
 *      Author: dylan
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "file_io.h"

using namespace std;

//Read coefficients from file.
vector<vector<string>> read_coef_file(string file_path) {
	vector<vector<string>> coef_entries = {};
	ifstream file(file_path, ios::in);
	if(file.is_open()) {
		coef_entries = parse_file(file);
	} else {
		cout << "No coefficient file found." << endl;
	}
	file.close();

	return(coef_entries);
}


vector<vector<string>> parse_file(ifstream& file) {
	string line;
	vector<vector<string>> coefs;
	while(getline(file,line)) {
		coefs.push_back(parse_line(line));
	}

	return(coefs);
}

vector<string> parse_line(string line) {
	stringstream line_stream(line);
	string sub_string;
	vector<string> line_split;
	while(getline(line_stream, sub_string, '\t')) {
		line_split.push_back(sub_string);
	}

	return(line_split);
}


vector<double> parse_coef(string coef) {
	vector<double> coefs = {};
	size_t start = coef.find("{");
	size_t end = coef.find("}");

	if(start!=string::npos && end!=string::npos) {
		size_t pos = start+1;
		size_t comma;
		while(pos < end) {
			comma = coef.substr(pos).find(",");
			if(comma!=string::npos) {
				comma += pos;
				coefs.push_back(stod(coef.substr(pos, comma-pos)));
				pos = comma+1;
			} else {
				coefs.push_back(stod(coef.substr(pos, end-pos)));
				pos = end;
			}
		}

	} else {
		cout << "Couldn't parse coefficient string." << endl;
	}

	return(coefs);
}


void write_coefs(vector<double> A, vector<double> B, string entry, string path) {
	//
}
