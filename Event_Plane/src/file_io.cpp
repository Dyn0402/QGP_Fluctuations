///*
// * file_io.cpp
// *
// *  Created on: May 12, 2019
// *      Author: dylan
// */
//
//
//#include <iostream>
//#include <fstream>
//#include <vector>
//
//using namespace std;
//
////Read coefficients from file.
//vector<vector<string>> read_coef_file(string file_path) {
//	string line;
//	ifstream file(file_path, ios::in);
//	if(file.is_open()) {
//		while(getline(file,line)) {
//			istringstream line_stream(line);
//			string sub_string;
//			while(getline(line_stream, sub_string, '\t')) {
//				//
//			}
//		}
//	}
//}
//
//
//vector<vector<string>> parse_file(ifstream file) {
//	string line;
//	vector<vector<string>> coefs;
//	while(getline(file,line)) {
//		coefs.push_back(parse_line(line));
//	}
//
//	return(coefs);
//}
//
//vector<string> parse_line(string line) {
//	istringstream line_stream(line);
//	string sub_string;
//	vector<string> line_split;
//	while(getline(line_stream, sub_string, '\t')) {
//		line_split.push_back(sub_string);
//	}
//
//	return();
//}
