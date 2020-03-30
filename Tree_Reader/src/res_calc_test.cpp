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
#include <TCanvas.h>

using namespace std;


static Double_t EventPlane(double res, double psi);
static Double_t chi(double res);
static Double_t resEventPlane(double chi);



void res_calc_test() {
	int events = 1000;
	double res = 0.8;

	double flow_res = 0.0;
	int flow_res_n = 0;

	TH1D *ep_dist = new TH1D("ep_dist", "Event Plane Pdf", 1001, -TMath::Pi(), TMath::Pi());
	for(int bin = 0; bin <= ep_dist->GetXaxis()->GetNbins(); bin++) {
		ep_dist->SetBinContent(bin, EventPlane(res, ep_dist->GetBinCenter(bin)));
	}

	TRandom3 sim_rand(0);

	for(int i=0; i<events; i++) {
		double reaction_plane = TMath::Pi()*sim_rand.Rndm();
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
	double nd = sqrt(TMath::Pi()/2.)*chi_f*cos(psi);
	double rd = exp(-chi_f*chi_f*sin(psi)*sin(psi)/2.);
	double th = 1 + TMath::Erf(chi_f*cos(psi)/sqrt(2.));
	double eventplane = (st + nd*rd*th)/(2.*TMath::Pi());
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



//void res_calc_test() {
//	int events = 1000000;
//	double res = 0.8;
//
//	double flow_res = 0.0;
//	int flow_res_n = 0;
//
//	TH1D *ep_dist = new TH1D("ep_dist", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -M_PI, M_PI);
//	for(int bin = 0; bin <= ep_dist->GetXaxis()->GetNbins(); bin++) {
//		ep_dist->SetBinContent(bin, EventPlane(res, ep_dist->GetBinCenter(bin)));
//	}
//	TH1D *result_dist = new TH1D("result_dist", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -M_PI, M_PI);
//	result_dist->SetLineColor(kRed);
//
//	TH1D *ep = new TH1D("ep", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -2*M_PI, 2*M_PI);
//	TH1D *rp = new TH1D("rp", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -2*M_PI, 2*M_PI);
//	ep->SetLineColor(kRed);
//
//	TRandom3 sim_rand(0);
//
//	for(int i=0; i<events; i++) {
//		double reaction_plane = 2*M_PI*sim_rand.Rndm() - M_PI;
//		double deviation = ep_dist->GetRandom();
//		double event_plane = reaction_plane + deviation;
////		event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
////		if(event_plane < 0) { event_plane += M_PI; }
//		while(event_plane >= M_PI) { event_plane -= 2*M_PI; }
//		while(event_plane < -M_PI) { event_plane += 2*M_PI; }
//		ep->Fill(event_plane);
//		rp->Fill(reaction_plane);
//		result_dist->Fill(event_plane - reaction_plane);
//		flow_res += cos(2*(event_plane - reaction_plane));
//		flow_res_n++;
//	}
//
//	if(flow_res_n > 0) { cout << "Target res: " << res << " Actual res: " << flow_res / flow_res_n << endl; }
//
//	result_dist->Scale(1.0/result_dist->Integral("width"));
//
//	TCanvas *can1 = new TCanvas();
//	rp->Draw();
//	ep->Draw("SAME");
//	TCanvas *can2 = new TCanvas();
//	ep_dist->Draw();
//	result_dist->Draw("SAMES");
//
//}
