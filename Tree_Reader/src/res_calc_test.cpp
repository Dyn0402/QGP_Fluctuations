/*
 * res_calc_test.cpp
 *
 *  Created on: Mar 29, 2020
 *      Author: Dylan Neff
 */


#include <iostream>
#include <string>

#include <TH1.h>
#include <TRandom3.h>
#include <TMath.h>

using namespace std;


static Double_t EventPlane(double res, double psi);
static Double_t chi(double res);
static Double_t resEventPlane(double chi);



void res_calc_test() {
	int events = 1000;
	double res = 0.8;

	double flow_res = 0.0;
	int flow_res_n = 0;

	TH1D *ep_dist = new TH1D("ep_dist", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -M_PI, M_PI);
	for(int bin = 0; bin <= ep_dist->GetXaxis()->GetNbins(); bin++) {
		ep_dist->SetBinContent(bin, EventPlane(res, ep_dist->GetBinCenter(bin)));
	}

	TRandom3 sim_rand(0);

	for(int i=0; i<events; i++) {
		double reaction_plane = M_PI*sim_rand.Rndm();
		double deviation = ep_dist->GetRandom();
		double event_plane = reaction_plane + deviation;
		flow_res += cos(2*(event_plane - reaction_plane));
		flow_res_n++;
	}

	if(flow_res_n > 0) { cout << "Target res: " << res << " Actual res: " << flow_res / flow_res_n << endl; }
}


//-----------------------------------------------------------------------
static Double_t EventPlane(double res, double psi) {
	double chi_f = chi(res);
	double st = exp(-chi_f*chi_f/2.);
	double nd = sqrt(M_PI/2.)*chi_f*cos(psi);
	double rd = exp(-chi_f*chi_f*sin(psi)*sin(psi)/2.);
	double th = 1 + TMath::Erf(chi_f*cos(psi)/sqrt(2.));
	double eventplane = (st + nd*rd*th)/(2.*M_PI);
	return eventplane;
}
////-----------------------------------------------------------------------

static Double_t chi(double res) {
  double chi   = 2.0;
  double delta = 1.0;

  for (int i = 0; i < 15; i++) {
      while(resEventPlane(chi) < res) {chi += delta;}
      delta = delta / 2.;
      while(resEventPlane(chi) > res) {chi -= delta;}
      delta = delta / 2.;
  }

  return chi;
}

//-----------------------------------------------------------------------
static Double_t resEventPlane(double chi) {
  double con = 0.626657;
  double arg = chi * chi / 4.;

  Double_t res = con * chi * exp(-arg) * (TMath::BesselI0(arg) +
                                          TMath::BesselI1(arg));

  return res;
}
