/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: Dylan Neff
 */


#include <iostream>

#include <TCanvas.h>
#include <TH1D.h>
#include <TApplication.h>
#include <TMath.h>

#include "event_plane.h"
#include "flatten.h"
#include "file_io.h"
#include "config.h"

using namespace std;

void flatten_test();
void read_test();
void write_test();


int main(int argc, char* argv[]) {
	TApplication theApp("App", &argc, argv);
	flatten_test();
	theApp.Run();

	cout << "donzo" << endl;
	return(0);
}


//Test flattening a generated distribution of two gaussians.
void flatten_test() {
	TH1D *rawHist = set_th1d(config::bins, config::lBound, config::rBound, config::rawHistName);
	vector<double> angles = genDGausHist(rawHist, config::N, config::gWeight, config::gMean, config::gRms);
	vector<double> A, B;
	tie(A, B) = calcCoef(rawHist, config::n_flatten_min, config::n_flatten_max, config::lBound, config::rBound);

	TH1D *flatHist2 = flatten_dist(angles, A, B, config::n_flatten_min, config::n_flatten_max, config::lBound, config::rBound, "flattened_dylan");

	TCanvas* c1 = new TCanvas();
	rawHist->Draw();
	TCanvas* c2 = new TCanvas();
	flatHist2->Draw();

	cout << endl << "donzo" << endl;
}


//Test reading fourier coefficients from file.
void read_test() {
	vector<vector<string>> coef_entries = read_coef_file("/home/dylan/Dropbox/Research/QGP_Fluctuations/Event_Plane_Flattening/IO_Tests/coef_IO_test1.txt");
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
	string path = "/home/dylan/Dropbox/Research/QGP_Fluctuations/Event_Plane_Flattening/IO_Tests/coef_IO_test2.txt";
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
