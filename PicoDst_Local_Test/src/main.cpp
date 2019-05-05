/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: dylan
 */


#include <iostream>
#include <TH1F.h>
#include <TCanvas.h>
#include <TProfile.h>

using namespace std;


int main() {
	cout << "Hello Root!" << endl;
	TCanvas *c1 = new TCanvas("c1", "Test", 600, 600);
	TProfile *p1 = new TProfile("p1","p1", 300,-1.5,1.5,0.,900.);

	TH1F *hist = new TH1F("test", "tests", 10, 0, 1);
	hist->Fill(0.1);
	hist->Fill(0.5);
	hist->Draw();

	p1->Fill(-1.1, 50.5);

	c1->SaveAs("test.png");

	delete c1;

	return(0);
}

