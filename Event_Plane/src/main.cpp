/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: dylan
 */


#include <iostream>
#include "TH1F.h"
#include "TCanvas.h"

using namespace std;


int main() {
	cout << "Hello Root!" << endl;
	TCanvas *c1 = new TCanvas("c1", "Test", 600, 600);

	TH1F *hist = new TH1F("test", "tests", 10, 0, 1);
	hist->Fill(0.1);
	hist->Fill(0.5);
	hist->Draw();

	c1->SaveAs("test.png");

	delete c1;

	return(0);
}

