/*
 * Simulator.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: dylan
 */


#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

#include <TRandom3.h>
#include <TCanvas.h>
#include <TH1.h>

#include "ratio_methods.h"
#include "Simulator.h"

using namespace std;


Simulator::Simulator() {
	sim_rand = new TRandom3(0);
	proton_dist_hist = new TH1D();
	proton_dist_hist->SetDirectory(0);
	efficiency_dist = new TH1D();
	efficiency_dist->SetDirectory(0);
	norm_eff_dist = new TH1D();
	norm_eff_dist->SetDirectory(0);
	ep_dist = new TH1D();
	ep_dist->SetDirectory(0);
	simulate_event = bind(&Simulator::sim_event, this, placeholders::_1);
}


Simulator::~Simulator() {
	if(flow_res_n > 0) { cout << "Target res: " << pars.ep_res << " Actual res: " << flow_res / flow_res_n << endl; }
	delete sim_rand;
	delete proton_dist_hist;
	delete efficiency_dist;
	delete norm_eff_dist;
	delete ep_dist;
}


// Getters

double Simulator::get_p_group() {
	return(pars.p_group);
}

double Simulator::get_spread_sigma() {
	return(pars.spread_sigma);
}

string Simulator::get_proton_dist_type() {
	return(pars.proton_dist);
}

TH1D* Simulator::get_efficiency_dist() {
	return(efficiency_dist);
}


// Setters
void Simulator::set_p_group(double p_group) {
	pars.p_group = p_group;
}

void Simulator::set_spread_sigma(double sig) {
	pars.spread_sigma = sig;
}

void Simulator::set_min_protons(int protons) {
	pars.min_protons = protons;
}

void Simulator::set_proton_dist(string dist) {
	pars.proton_dist = dist;
}

void Simulator::set_particle_mean(double mean) {
	pars.particle_mean = mean;
}

void Simulator::set_particle_max(int max) {
	pars.particle_max = max;
}

void Simulator::set_flat_dist(int max) {
	pars.proton_dist = "flat";
	if(max > 2) {
		pars.particle_max = max;
	}
}

void Simulator::set_proton_dist_hist(TH1D *hist) {
	proton_dist_hist = hist;
	proton_dist_hist->SetDirectory(0);
	pars.proton_dist = "hist";
}

void Simulator::set_efficiency_dist_hist(TH1D *hist) {
	efficiency_dist = hist;
	efficiency_dist->SetDirectory(0);
	norm_eff_dist = (TH1D*)efficiency_dist->Clone();
	norm_eff_dist->Scale(pars.hom_eff/norm_eff_dist->GetMaximum());
	norm_eff_dist->SetDirectory(0);
}

void Simulator::set_efficiency_dist_hist(string root_path, string hist_name) {
	TFile *file = new TFile(root_path.data(), "READ");
	TH1D *hist = (TH1D*)file->Get(hist_name.data());
	efficiency_dist = (TH1D*)hist->Clone();
	efficiency_dist->SetDirectory(0);
	norm_eff_dist = (TH1D*)efficiency_dist->Clone();
	norm_eff_dist->Scale(pars.hom_eff/norm_eff_dist->GetMaximum());
	norm_eff_dist->SetDirectory(0);

	file->Close();
	delete file;
}

void Simulator::set_eff() {
	simulate_event = bind(&Simulator::sim_event_eff, this, placeholders::_1);
}

void Simulator::set_flow() {
	simulate_event = bind(&Simulator::sim_event_flow, this, placeholders::_1);
}

void Simulator::set_eff_flow() {
	simulate_event = bind(&Simulator::sim_event_eff_flow, this, placeholders::_1);
}

void Simulator::set_hom_eff(double eff) {
	pars.hom_eff = eff;
}

void Simulator::set_v2(double v2) {
	pars.v2 = v2;
}

void Simulator::set_ep_res(double res) {
	pars.ep_res = res;
}

void Simulator::set_flow(double v2, double res, double chi_acc, int event_plane_n) {
	pars.v2 = v2;
	pars.ep_res = res;
	ep_dist = new TH1D("ep_dist", ("Event Plane Pdf res="+to_string(res)).data(), 1001, -M_PI/event_plane_n, M_PI/event_plane_n);
	ep_dist->SetDirectory(0);
	for(int bin = 0; bin <= ep_dist->GetXaxis()->GetNbins(); bin++) {
		ep_dist->SetBinContent(bin, event_plane(res, event_plane_n*ep_dist->GetBinCenter(bin), chi_acc));
	}

}


// Doers

// Simulate single event and return simulated proton angles.
void Simulator::sim_event(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		proton_angles.push_back(new_angle);
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			proton_angles.push_back(group_angle);
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			proton_angles.push_back(new_angle);
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event with particles produced in back to back pairs and return simulated proton angles.
void Simulator::sim_event_pairs(Event &event) {
	double group_angle, new_angle, pair_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		proton_angles.push_back(new_angle);
		pair_angle = new_angle + M_PI;
		pair_angle = fmod(pair_angle, 2*M_PI);  // Force to range [0, 2*pi)
		proton_angles.push_back(pair_angle);
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			proton_angles.push_back(group_angle);
			pair_angle = group_angle + M_PI;
			pair_angle = fmod(pair_angle, 2*M_PI);  // Force to range [0, 2*pi)
			proton_angles.push_back(pair_angle);
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			proton_angles.push_back(new_angle);
			pair_angle = new_angle + M_PI;
			pair_angle = fmod(pair_angle, 2*M_PI);  // Force to range [0, 2*pi)
			proton_angles.push_back(pair_angle);
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event and return simulated proton angles. Include efficiency into simulation.
void Simulator::sim_event_eff(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
			proton_angles.push_back(new_angle);
		}
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(group_angle)) >= sim_rand->Rndm()) {
				proton_angles.push_back(group_angle);
			}
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
				proton_angles.push_back(new_angle);
			}
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}


// Simulate single event and return simulated proton angles. Include elliptic flow into simulation.
void Simulator::sim_event_eff_flow(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	double reaction_plane = M_PI*sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
	if(event_plane < 0) { event_plane += M_PI; }
	event.set_event_plane(event_plane);
	flow_res += cos(2*(event_plane - reaction_plane));
	flow_res_n++;

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
			if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
				proton_angles.push_back(new_angle);
			}
		}
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			if(1.0 + 2 * pars.v2 * cos(2*(group_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(group_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(group_angle);
				}
			}
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(new_angle);
				}
			}
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);
}



// Simulate single event and return simulated proton angles. Include elliptic flow into simulation.
void Simulator::sim_event_flow(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

	double reaction_plane = M_PI*sim_rand->Rndm();
	double deviation = ep_dist->GetRandom();
	double event_plane = reaction_plane + deviation;
	event_plane = fmod(event_plane, M_PI);  // Force to range [0, pi)
	if(event_plane < 0) { event_plane += M_PI; }
	event.set_event_plane(event_plane);
	flow_res += cos(2*(event_plane - reaction_plane));
	flow_res_n++;

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
			proton_angles.push_back(new_angle);
		}
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
			if(1.0 + 2 * pars.v2 * cos(2*(group_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				proton_angles.push_back(group_angle);
			}
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
			if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				proton_angles.push_back(new_angle);
			}
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);

}



// Simulate single event and return simulated proton angles. Include elliptic flow into simulation.
void Simulator::sim_event_eff_new(Event &event) {
	double group_angle, new_angle;
	vector<double> proton_angles;

	int n_protons = get_protons();

//	double reaction_plane = 2*M_PI*sim_rand->Rndm();
//	double deviation = ep_dist->GetRandom();
//	double event_plane = reaction_plane + deviation;
//	event_plane = fmod(event_plane, 2*M_PI);  // Force to range [0, 2*pi)
//	if(event_plane < 0) { event_plane += 2*M_PI; }

	if(n_protons > 0) while((int)proton_angles.size() < 1) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
//		if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
			if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
				proton_angles.push_back(new_angle);
			}
//		}
	}

	while((int)proton_angles.size() < n_protons) {
		if(sim_rand->Rndm() < pars.p_group) {
			group_angle = sim_rand->Gaus(proton_angles.back(), pars.spread_sigma);
			group_angle = fmod(group_angle, 2*M_PI);  // Force to range [0, 2*pi)
			if(group_angle < 0) { group_angle += 2*M_PI; }
//			if(1.0 + 2 * pars.v2 * cos(2*(group_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(group_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(group_angle);
				}
//			}
		} else {
			new_angle = sim_rand->Rndm() * 2 * M_PI;
//			if(1.0 + 2 * pars.v2 * cos(2*(new_angle - reaction_plane)) >= sim_rand->Rndm() * (1.0 + 2 * pars.v2)) {
				if(norm_eff_dist->GetBinContent(norm_eff_dist->FindBin(new_angle)) >= sim_rand->Rndm()) {
					proton_angles.push_back(new_angle);
				}
//			}
		}
	}

	vector<Track> tracks;
	for(double& angle:proton_angles) {
		Track track(track_defs);
		track.set_phi(angle);
		tracks.push_back(track);
	}
	event.set_particles(tracks);

}


int Simulator::get_protons() {
	int n = 0;
	if(pars.proton_dist == "hist") {
		n = proton_dist_hist->GetRandom();
	}else if(pars.proton_dist == "poisson") {
		n = sim_rand->Poisson(pars.particle_mean);
	} else if(pars.proton_dist == "flat") {
		n = (int)(sim_rand->Rndm() * pars.particle_max + 0.5);
	}

	return (n);
}


double Simulator::get_group_angle(double center) {
	TH1D *convo = (TH1D*)efficiency_dist->Clone();

	for(int bin=1; bin<=convo->GetXaxis()->GetNbins(); bin++) {
		double new_bin_content = convo->GetBinContent(bin) * wrap_gaus(convo->GetBinCenter(bin), center, pars.spread_sigma, 0, 2*TMath::Pi());

//		cout << "bin: " << bin << " center: " << center << " old contents: " << convo->GetBinContent(bin) << " new contents: " << new_bin_content << endl;
//		double new_bin_content = convo->GetBinContent(bin) * TMath::Gaus(convo->GetBinCenter(bin), center, pars.spread_sigma);
//		if(fabs(convo->GetBinCenter(bin) - center) < 1.0) { cout << new_bin_content << endl; }
		convo->SetBinContent(bin, new_bin_content);
	}

//	this_thread::sleep_for(chrono::seconds(30));

//	cout << wrap_gaus(0, 0, pars.spread_sigma, 0, 2*TMath::Pi()) << endl;

//	if(convo->Integral() <= 0) {
//		cout << "Get random convo   bin: " << convo->FindBin(center) << "  content: " << convo->GetBinContent(convo->FindBin(center)) << "  center: " << center << endl;
//	}
	double new_angle = convo->GetRandom();
	delete convo;
	return(new_angle);

}


// Wrap gaussian with mean mu and width sigma at point x around a range from lower_bound to upper_bound.
double Simulator::wrap_gaus(double x, double mu, double sigma, double lower_bound, double upper_bound) {
	double range = upper_bound - lower_bound;
	double sum = 0;
	double gaus_val = TMath::Gaus(x, mu, sigma);
	double xi = x;
	while(gaus_val > 0) {  // Negative side loop
		sum += gaus_val;
		xi -= range;
		gaus_val = TMath::Gaus(xi, mu, sigma);
	}
	xi = x + range;
	gaus_val = TMath::Gaus(xi, mu, sigma);
	while(gaus_val > 0) {  // Positive side loop
		sum += gaus_val;
		xi += range;
		gaus_val = TMath::Gaus(xi, mu, sigma);
	}

	return(sum);
}


double Simulator::event_plane_gang(double res, double psi) {
	double chi_f = chi_gang(res);
	double st = exp(-chi_f*chi_f/2.);
	double nd = pow(M_PI/2.,0.5)*chi_f*cos(psi);
	double rd = exp(-chi_f*chi_f*sin(psi)*sin(psi)/2.);
	double th = 1 + TMath::Erf(chi_f*cos(psi)/sqrt(2.));
	double eventplane = (st + nd*rd*th)/(2.*M_PI);
	return eventplane;
}


double Simulator::event_plane(double res, double psi, double acc) {
	double chi_f = chi(res, acc);
	double st = exp(-chi_f*chi_f/2.);
	double nd = pow(M_PI/2.,0.5)*chi_f*cos(psi);
	double rd = exp(-chi_f*chi_f*sin(psi)*sin(psi)/2.);
	double th = 1 + TMath::Erf(chi_f*cos(psi)/sqrt(2.));
	double eventplane = (st + nd*rd*th)/(2.*M_PI);
	return eventplane;
}


double Simulator::chi_gang(double res) {
  double chi   = 2.0;
  double delta = 1.0;

  for (int i = 0; i < 15; i++) {
      while(res_event_plane(chi) < res) {chi += delta;}
      delta = delta / 2.;
      while(res_event_plane(chi) > res) {chi -= delta;}
      delta = delta / 2.;
  }

  return chi;
}


double Simulator::chi(double res, double acc) {
  double chi   = 2.0;
  double delta = 1.0;
  double diff = -numeric_limits<double>::max();

  while (fabs(diff) > acc) {
      while(res_event_plane(chi) < res) {chi += delta;}
      delta = delta / 2.;
      while(res_event_plane(chi) > res) {chi -= delta;}
      delta = delta / 2.;
      diff = res_event_plane(chi) - res;
  }

  return chi;
}


double Simulator::res_event_plane(double chi) {
  double con = 0.626657;
  double arg = chi * chi / 4.;

  double res = con * chi * exp(-arg) * (TMath::BesselI0(arg) +
                                          TMath::BesselI1(arg));

  return res;
}
