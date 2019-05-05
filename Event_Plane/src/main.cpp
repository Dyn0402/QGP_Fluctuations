/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: dylan
 */


#include <iostream>
#include <TCanvas.h>
#include <TH1D.h>

#include "event_plane.h"
#include "config.h"

using namespace std;


int main() {
	cout << "Hello Root!" << endl;
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

	cout << endl << "donzo" << endl;

	return(0);
}

