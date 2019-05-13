/*
 * file_io.h
 *
 *  Created on: May 12, 2019
 *      Author: dylan
 */

#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<vector<string>> read_coef_file(string file_path);
vector<vector<string>> parse_file(ifstream file);
vector<string> parse_line(string line);



#endif /* FILE_IO_H_ */
