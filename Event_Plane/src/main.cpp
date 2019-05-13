/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: dylan
 */


#include <iostream>

#include <TCanvas.h>
#include <TH1D.h>
#include <TApplication.h>
#include <TMath.h>
#include <TLine.h>

#include "event_plane.h"
#include "flatten.h"
#include "config.h"

using namespace std;

void Assignment6();
void As5();
void EventPlane();
void EventPlane2();
void flatten_test();
void read_test();


int main(int argc, char* argv[]) {
	TApplication theApp("App", &argc, argv);
	flatten_test();
	theApp.Run();

	cout << "donzo" << endl;
	return(0);
}


void EventPlane2() {
	TH1D *diff_hist = new TH1D("diff", "diff", 100, -TMath::Pi(), TMath::Pi());
	TH1D *ep_hist = new TH1D("ep_hist", "Event Plane Distribution", 100, -2*TMath::Pi(), 2*TMath::Pi());
	vector<double> particles, Q, event_angles;
	double react_angle, event_angle, diff;
	for(int i=0; i < config::n_events; i++) {
		react_angle = genAngle(config::rand);
		particles = genParticles(config::n_particles, config::v2, react_angle, config::rand);
		Q = getQ(particles, config::n_harmonic);
		event_angle = getQAngle(Q, config::n_harmonic);
		event_angles.push_back(event_angle);
		if(react_angle > TMath::Pi()) { react_angle -= TMath::Pi(); }
		diff = react_angle - event_angle;
		diff_hist->Fill(diff);
		ep_hist->Fill(event_angle);
	}

	vector<double> A, B;
	tie(A, B) = calcCoef2(ep_hist, config::n_flatten_min, config::n_flatten_max, 0, TMath::Pi());
	TH1D *flat_hist = flatten_dist(event_angles, A, B, config::n_flatten_min, config::n_flatten_max, 0, TMath::Pi(), "flattened");

	TCanvas *diff_can = new TCanvas("diff_can", "Reaction Plane Angle - Event Plane Angle");
	diff_hist->Draw();
	TCanvas *event_can = new TCanvas("event_can", "Event Plane Angle Distribution");
	ep_hist->Draw();
	TCanvas *flat_can = new TCanvas("flat_can", "Flattened Event Plane Angle Distribution");
	flat_hist->Draw();
	cout << "Drawing" << endl;
}


void EventPlane() {
	TH1D *diff_hist = new TH1D("diff", "diff", 100, -TMath::Pi(), TMath::Pi());
	TH1D *ep_hist = new TH1D("ep_hist", "Event Plane Distribution", 100, -2*TMath::Pi(), 2*TMath::Pi());
	vector<double> particles, Q;
	double react_angle, event_angle, diff;
	for(int i=0; i < config::n_events; i++) {
		react_angle = genAngle(config::rand);
		particles = genParticles(config::n_particles, config::v2, react_angle, config::rand);
		Q = getQ(particles, config::n_harmonic);
		event_angle = getQAngle(Q, config::n_harmonic);
		if(react_angle > TMath::Pi()) { react_angle -= TMath::Pi(); }
		diff = react_angle - event_angle;
		diff_hist->Fill(diff);
		ep_hist->Fill(event_angle);
	}

	vector<double> A, B;
	tie(A, B) = calcCoef(ep_hist, config::n_flatten_min, config::n_flatten_max);
	TH1D *flat_hist = genFlatHist(ep_hist, A, B, config::n_flatten_min, config::n_flatten_max, "flattened");

	TCanvas *diff_can = new TCanvas("diff_can", "Reaction Plane Angle - Event Plane Angle");
	diff_hist->Draw();
	TCanvas *event_can = new TCanvas("event_can", "Event Plane Angle Distribution");
	ep_hist->Draw();
	TCanvas *flat_can = new TCanvas("flat_can", "Flattened Event Plane Angle Distribution");
	flat_hist->Draw();
	cout << "Drawing" << endl;
}


void Assignment6() {
	double res = 0.0;
	double v2Obs = 0.0;
	double v2ObsCheat = 0.0;
	TH1D *v2ObsDist = new TH1D("v2ObsDist", "Distribution of v2 Observed", 100, -1.0, 0.5);
	TH1D *v2CheatDist = new TH1D("v2CheatDist", "Distribution of v2 from reaction angle", 100, -1, 0.5);
	for(int i=0; i < config::n_events; i++) {
		double reactAngle = genAngle(config::rand);
		vector<double> particles = genParticles(config::n_particles, config::v2, reactAngle, config::rand);
		double v2ObsCheati = getV2ObsCheat(particles, config::n_harmonic, reactAngle);
		double resi = getSubRes(particles, config::n_harmonic);
		double v2Obsi = getV2Obs(particles, config::n_harmonic);
		v2ObsCheat += v2ObsCheati;
		res += resi;
		v2Obs += v2Obsi;
		v2ObsDist->Fill(v2Obsi / config::n_particles);
		v2CheatDist->Fill(v2ObsCheati / config::n_particles);
	}

	res /= config::n_events;
	res = getFullRes(res);
	v2Obs /= config::n_events * config::n_particles;
	v2ObsCheat /= config::n_events * config::n_particles;

	double v2 = v2Obs / res;

	cout << "v2Obs: " << v2Obs << " | res: " << res << " | v2: " << v2 << endl;
	cout << "v2Cheat: " << v2ObsCheat << endl;
	TCanvas *c1 = new TCanvas("c1", "c1");
	v2ObsDist->Draw();
	TCanvas *c2 = new TCanvas("c2", "c2");
	v2CheatDist->Draw();
}


void As5() {
	TH1D *rawHist = setDGausHist(config::bins, config::lBound, config::rBound, config::rawHistName);
	rawHist = genDGausHist(rawHist, config::N, config::gWeight, config::gMean, config::gRms);
	vector<double> A, B;
	tie(A, B) = calcCoef(rawHist, config::n_flatten_min, config::n_flatten_max);

	TH1D *flatHist = genFlatHist(rawHist, A, B, config::n_flatten_min, config::n_flatten_max, config::flatHistName);
	TCanvas* c1 = new TCanvas();
	rawHist->Draw();
	TCanvas* c3 = new TCanvas();
	flatHist->Draw();

	cout << endl << "donzo" << endl;
}


void flatten_test() {
	TH1D *rawHist = setDGausHist(config::bins, config::lBound, config::rBound, config::rawHistName);
	vector<double> angles = genDGausHist2(rawHist, config::N, config::gWeight, config::gMean, config::gRms);
	vector<double> A, B;
	tie(A, B) = calcCoef2(rawHist, config::n_flatten_min, config::n_flatten_max, -TMath::Pi(), TMath::Pi());

	TH1D *flatHist = flatten_dist(angles, A, B, config::n_flatten_min, config::n_flatten_max, -TMath::Pi(), TMath::Pi(), "flattened");

//	TH1D *flatHist = genFlatHist(rawHist, A, B, config::nMin, config::nMax, config::flatHistName);
	TCanvas* c1 = new TCanvas();
	rawHist->Draw();
	TCanvas* c3 = new TCanvas();
	flatHist->Draw();

	cout << endl << "donzo" << endl;
}


void read_test() {
	//
}
