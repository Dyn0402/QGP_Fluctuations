/*
 * flatten_roli.cpp
 *
 *  Created on: May 15, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

#include <TH1.h>
#include <TF1.h>
#include <TMath.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TCanvas.h>

using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Parameters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int n_flatten_min = 1; //Minimum fourier order for flattening.
int n_flatten_max = 25; //Maximum fourier order for flattening.

string rawHistName = "dGauss";
string flatHistName = "Flattened";
string coef_file_path = "/home/dylan/Dropbox/Research/QGP_Fluctuations/Event_Plane_Flattening/IO_Tests/coef_IO_test1.txt";

vector <double> gMean = {1.0, 2.0};
vector <double> gRms = {0.1, 0.2};
vector <int> gWeight = {2, 1};

double lBound = TMath::Pi();
double rBound = 2*TMath::Pi();

int N = 1000000; //Number of weight 1 events to generate.
int bins = 400;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Declarations
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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


//Distribution flattening function declarations
double getAvg(TH1D *hist, TF1 *func);
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int nMin, int nMax, double x_min, double x_max);
double flatten_angle(double angle, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max);

//Test distribution generation function declarations
TH1D* set_th1d(int bins, double low, double up, string name);
vector<double> genDGausHist(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms);
TH1D* flatten_dist(vector<double> particles, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName);


//Main test functions
void flatten_test();
void read_test();
void write_test();



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Run function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Run function (runs an example function in this case)
void flatten_roli() {
	flatten_test();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Main test functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Test flattening a generated distribution of two gaussians.
void flatten_test() {
	TH1D *rawHist = set_th1d(bins, lBound, rBound, rawHistName);
	vector<double> angles = genDGausHist(rawHist, N, gWeight, gMean, gRms);
	vector<double> A, B;
	tie(A, B) = calcCoef(rawHist, n_flatten_min, n_flatten_max, lBound, rBound);

	TH1D *flatHist2 = flatten_dist(angles, A, B, n_flatten_min, n_flatten_max, lBound, rBound, "flattened_dylan");

	TCanvas* c1 = new TCanvas();
	rawHist->Draw();
	TCanvas* c2 = new TCanvas();
	flatHist2->Draw();

	cout << endl << "donzo" << endl;
}


//Test reading fourier coefficients from file.
void read_test() {
	vector<vector<string>> coef_entries = read_coef_file(coef_file_path);
	vector<double> coefs;
	for(vector<string> entry:coef_entries) {
		for(string element:entry) {
			cout << element << "\t" << flush;
		}
		if(entry.size() == 3) {
			cout << endl;
			coefs = parse_coef(entry[1]);
			for(double coef:coefs) {
				cout << coef << endl;
			}
			cout << endl;
			coefs = parse_coef(entry[2]);
			for(double coef:coefs) {
				cout << coef << endl;
			}
		}
	}

	cout << endl << "donzo" << endl;
}


//Test writing fourier coefficients to file.
void write_test() {
	string path = coef_file_path;
	string entry = "hello";

	vector<double> A_new = {103.2, -3.6323, 0.0032, 000.3243};
	vector<double> B_new = {-1234, 2132.3, 0.0012, 0.000001232};

	write_coefs(A_new, B_new, entry, path);

	vector<double> A, B;
	tie(A,B) = get_coefs(path, entry);

	for(double val:A) { cout << val << "," << flush; }
	cout << endl;
	for(double val:B) { cout << val << "," << flush; }

	cout << endl << "donzo" << endl;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//File IO
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Flatting
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Get average of TF1 func on histogram hist.
double getAvg(TH1D *hist, TF1 *func) {
	int norm = 0;
	double sum = 0.0;
	int binCont;
	double binCent;

	for(int i = hist->GetXaxis()->GetFirst(); i<=hist->GetXaxis()->GetLast(); i++) {
		binCont = hist->GetBinContent(i);
		binCent = hist->GetBinCenter(i);
		norm += binCont;
		sum += binCont * func->Eval(binCent);
	}

	return(sum / norm);
}


//Calculate coefficients of Fourier expansion.
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int n_min, int n_max, double x_min, double x_max){
	vector<double> B, A;

	double x_avg = (x_max + x_min) / 2; //Calculate average of range
	double L = (x_max - x_min) / 2; //Calculate half range (2*L is full range)

	TF1 *cos_shift = new TF1("cos_shift", "cos([3] * [2] * (x-[0]) / [1])");
	cos_shift->SetParameters(x_avg, L, TMath::Pi(), n_min);
	TF1 *sin_shift = new TF1("sin_shift", "sin([3] * [2] * (x-[0]) / [1])");
	sin_shift->SetParameters(x_avg, L, TMath::Pi(), n_min);

	for(int n = n_min; n <= n_max; n++) {
		cos_shift->SetParameter(3, n);
		sin_shift->SetParameter(3, n);
		B.push_back(getAvg(hist, cos_shift) * 2.0 / n);
		A.push_back(getAvg(hist, sin_shift) * -2.0 / n);
	}

	delete cos_shift;
	delete sin_shift;

	return(make_tuple(A, B));
}


//Apply correction to event angle to make distribution flat.
double flatten_angle(double angle, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max) {
	double new_angle, old_angle;

	double x_avg = (x_max + x_min) / 2; //Calculate average of range
	double L = (x_max - x_min) / 2; //Calculate half range (2*L is full range)

	old_angle = TMath::Pi() * (angle - x_avg) / L;
	new_angle = old_angle;
	for(int n = nMin; n <= nMax; n++) {
		new_angle += A[n-nMin] * TMath::Cos(n * old_angle) + B[n-nMin] * TMath::Sin(n * old_angle);
	}
	new_angle = L * new_angle / TMath::Pi() + x_avg;
	while(new_angle > x_max) {new_angle -= (x_max-x_min);}
	while(new_angle < x_min) {new_angle += (x_max-x_min);}


	return(new_angle);
}


//Define and set the initial th1d histogram.
TH1D* set_th1d(int bins, double low, double up, string name) {
	TH1D* hist = new TH1D(name.c_str(), name.c_str(), bins, low, up);

	return(hist);
}


//Generate a double gaussian distribution.
vector<double> genDGausHist(TH1D* hist, int N, vector<int> weight, vector<double> mean, vector<double> rms) {
	int first = hist->GetXaxis()->GetFirst();
	int last = hist->GetXaxis()->GetLast();
	int bins = last - first;
	double low = hist->GetBinLowEdge(first);
	double up = hist->GetBinLowEdge(last+1);

	TRandom3* r = new TRandom3(0);
	double rand;
	vector<double> angles;

	for(unsigned i=0; i<weight.size(); i++) {
		for(int j=0; j<N*weight[i]; j++) {
			rand = r->Gaus(mean[i], rms[i]);
			while(rand > up) {rand -= (up-low);}
			while(rand < low) {rand += (up-low);}
			hist->Fill(rand);
			angles.push_back(rand);
		}
	}

	return(angles);
}



//Given an input of event angles, fourier coefficients, minimum and maximum fourier harmonic, range (x_min, x_max) of distribution, and name of distribution
//apply correction to each angle to flatten the distribution.
//Return flattened distribution as a TH1D histogram.
TH1D* flatten_dist(vector<double> event_angles, vector<double> A, vector<double> B, int nMin, int nMax, double x_min, double x_max, string flatName) {
	TH1D *flat = new TH1D(flatName.data(), flatName.data(), 400, x_min, x_max);

	double new_angle;
	for(unsigned i = 0; i < event_angles.size(); i++) {
		new_angle = flatten_angle(event_angles[i], A, B, nMin, nMax, x_min, x_max);
		flat->Fill(new_angle);
	}

	return(flat);
}

