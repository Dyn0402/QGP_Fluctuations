
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

#include <TH1.h>

using namespace std;

//File input declarations
tuple<vector<double>, vector<double>> get_coefs(string file_path, string entry_name);
vector<vector<string>> read_coef_file(string file_path);
vector<vector<string>> parse_file(ifstream& file);
vector<string> parse_line(string line);
vector<double> parse_coef(string coef);

//File ouptut declarations
void write_coefs(vector<double> A, vector<double> B, string entry, string path);
vector<vector<string>> replace_entry(vector<vector<string>> coef_entries, string entry, vector<double> A, vector<double> B);
void write_entries(vector<vector<string>> coef_entries, ofstream& file);
string get_coef_string(vector<double> coef, string coef_string);


//Flattening declarations
double getAvg(TH1D* hist, double (*f)(double), int n);
double getAvg2(TH1D *hist, TF1 *func, int n);
double getAvg2(TH1D *hist, TF1 *func);
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int nMin, int nMax);
tuple<vector<double>, vector<double>> calcCoef2(TH1D* hist, int nMin, int nMax, double x_min, double x_max);
TH1D* genFlatHist(TH1D* hist, vector<double> A, vector<double> B, int nMin, int nMax, string flatName);
TH1D* genFlatHist2(TH1D* hist, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName);
TH1D* flatten_dist(vector<double> particles, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName);

TH1D* setDGausHist(int bins, double low, double up, string name);
TH1D* genDGausHist(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms);
vector<double> genDGausHist2(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms);



//Main function (example in this case)
void flatten_roli() {
	//
}



//Get coefficients for entry in file_path if they exist.
tuple<vector<double>, vector<double>> get_coefs(string file_path, string entry_name) {
	vector<vector<string>> coef_entries = read_coef_file(file_path);

	bool found = false;
	vector<double> A, B = {};
	for(vector<string> entry:coef_entries) {
		if(entry.size() == 3) {
			if(entry[0] == entry_name) {
				found = true;
				A = parse_coef(entry[1]);
				B = parse_coef(entry[2]);
			}
		}
	}
	if(!found) { cout << "Flattening Fourier coefficient entry not found." << endl; }

	return(make_tuple(A, B));
}

//Read coefficients from file into string vector for each entry.
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


//Parse lines of file.
vector<vector<string>> parse_file(ifstream& file) {
	string line;
	vector<vector<string>> coefs;
	while(getline(file,line)) {
		coefs.push_back(parse_line(line));
	}

	return(coefs);
}


//Parse line and split by tab character.
vector<string> parse_line(string line) {
	stringstream line_stream(line);
	string sub_string;
	vector<string> line_split;
	while(getline(line_stream, sub_string, '\t')) {
		line_split.push_back(sub_string);
	}

	return(line_split);
}


//Parse string coefficient and return a vector<double> for the coefficient string.
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


//Write coefficient vectors A and B to file_path with entry as name/tag.
//If the file doesn't exist, create it and add these coefficients as the first entry/line.
//If file exists, check if entry exists. If entry exists, replace it with input A and B.
//If entry does not exist, append it to end of file.
void write_coefs(vector<double> A, vector<double> B, string entry, string file_path) {
	vector<vector<string>> coef_entries = read_coef_file(file_path);
	ofstream file(file_path, ios::out | ios::trunc);
	coef_entries = replace_entry(coef_entries, entry, A, B);
	write_entries(coef_entries, file);
	file.close();
}


//Check coef_entries for entry_name tagged entry.
//If it exists, replace coefficient strings with the input A and B.
//If it doesn't exist, append it to coef_entries.
vector<vector<string>> replace_entry(vector<vector<string>> coef_entries, string entry_name, vector<double> A, vector<double> B) {
	string A_string = get_coef_string(A, "A");
	string B_string = get_coef_string(B, "B");

	int entry_index = -1;
	for(unsigned index=0; index<coef_entries.size(); index++) {
		if(coef_entries[index].size() == 3) {
			if(coef_entries[index][0] == entry_name) {
				entry_index = index;
			}
		}
	}
	if(entry_index == -1) {
		vector<string> new_entry = {entry_name, A_string, B_string};
		coef_entries.push_back(new_entry);
	} else {
		coef_entries[entry_index][1] = A_string;
		coef_entries[entry_index][2] = B_string;
	}

	return(coef_entries);
}


//Write each entry in coef_entries as a new line in file.
//Separate entry elements (entry name, A string, B string) with a tab.
void write_entries(vector<vector<string>> coef_entries, ofstream& file) {
	for(vector<string> entry:coef_entries) {
		if(entry.size() == 3) {
			file << entry[0] << "\t" << entry[1] << "\t" << entry[2] << "\n";
		} else { cout << "Bad entry." << endl; }
	}
}


//Convert coefficient double vector to a bracketed comma separated string.
//Ex: A={0.324,324.32,-0.43}
string get_coef_string(vector<double> coef, string coef_string) {
	coef_string += "={";
	for(double val:coef) {
		coef_string += to_string(val) + ",";
	}
	coef_string[coef_string.size() - 1] = '}';

	return(coef_string);
}
