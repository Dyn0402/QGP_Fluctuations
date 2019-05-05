/*
 * event_plane.cpp
 *
 *  Created on: May 5, 2019
 *      Author: dylan
 */


#include <iostream>
#include <TRandom3.h>
#include <TMath.h>
#include <TF1.h>

using namespace std;


double genAngle(TRandom3 *rand) {
	double angle = rand->Rndm() * 2 * TMath::Pi();

	return(angle);
}


double genv2Angle(double v2, double reactAngle, TRandom3 *rand) {
	double x = 0.0;
	double y = 2.0 * v2 + 1.0;

	while(y >= 1.0 + 2.0 * v2 * TMath::Cos(2.0 * x - 2.0 * reactAngle)) {
		x = rand->Rndm() * 2 * TMath::Pi();
		y = rand->Rndm() * (1.0 + 2.0 * v2);
	}

	return(x);
}


vector<double> genParticles(double n, double v2, double reactAngle, TRandom3 *rand) {
	vector<double> particles;
	double angle;

	for(int i=0; i<n; i++) {
		angle = genv2Angle(v2, reactAngle, rand);
		particles.push_back(angle);
	}

	return(particles);
}


vector<double> getQ(vector<double> particles, int nHarmonic) {
	vector<double> Q(2,0.0);

	for(double angle : particles) {
		Q[0] += TMath::Cos(nHarmonic*angle);
		Q[1] += TMath::Sin(nHarmonic*angle);
	}

	return(Q);
}


double getQAngle(vector<double> Q, int nHarmonic) {
	double angle = TMath::ATan2(Q[1], Q[0]);
	if(angle < 0) {
		angle += 2 * TMath::Pi();
	}
	angle /= nHarmonic;

	return(angle);
}


double getSubRes(vector<double> particles, int nHarmonic) {
	int nPerSub = particles.size() / 2;
	vector<double> subAngle;
	for(int i=0; i<2; i++) {
		vector<double> subParticles(particles.begin()+i*nPerSub, particles.begin()+(i+1)*nPerSub);
		vector<double> Q = getQ(subParticles, nHarmonic);
		double angle = getQAngle(Q, nHarmonic);
		subAngle.push_back(angle);
	}

	double subRes = TMath::Cos(nHarmonic * (subAngle[0] - subAngle[1]));

	return(subRes);
}


double getV2Obs(vector<double> particles, int nHarmonic) {
	double v2Obs = 0.0;
	vector<double> Q = getQ(particles, nHarmonic);
	double eventAngle;// = getQAngle(Q, nHarmonic);

	for(double angle : particles) {
		Q[0] -= TMath::Cos(nHarmonic*angle); // Remove auto-correlations.
		Q[1] -= TMath::Sin(nHarmonic*angle);
		eventAngle = getQAngle(Q, nHarmonic);
		v2Obs += TMath::Cos(nHarmonic * (angle - eventAngle));
		Q[0] += TMath::Cos(nHarmonic*angle);
		Q[1] += TMath::Sin(nHarmonic*angle);
	}

	return(v2Obs);
}


double resf(double *x, double *pars) {
	return(TMath::Power(TMath::Pi()/2.0, 0.5) / 2.0 * x[0] * TMath::Exp(-x[0]*x[0]/4.0) * (TMath::BesselI0(x[0]*x[0]/4.0) + TMath::BesselI1(x[0]*x[0]/4.0)));
}


double getFullRes(double subRes) {
	subRes = TMath::Power(subRes, 0.5);
	TF1 resTF = TF1("res", resf, 0, 50, 0);
	double chi = resTF.GetX(subRes) * TMath::Sqrt(2);
	double res = resTF(chi);

	return(res);
}


double getV2ObsCheat(vector<double> particles, int nHarmonic, double reactAngle) {
	double v2Obs = 0.0;

	for(double angle : particles) {
		v2Obs += TMath::Cos(nHarmonic * (angle - reactAngle));
	}

	return v2Obs;
}


