/*
 * Track.h
 *
 *  Created on: Sep 10, 2019
 *      Author: Dylan Neff
 */

#ifndef TRACK_H_
#define TRACK_H_

//#ifndef ROOT_Track
//#define ROOT_Track

//#include <TObject.h>


struct track_defaults {
	double pt = 0.7;
	double p = 0.7;
	double phi = 0.0;
	double eta = 0.0;

	float dca = 0.1;
	double nsigma = 1.0;
	float beta = -999;
	short charge = 1;
};


//class Track : public TObject {
class Track {
public:
	Track();
	Track(double pt, double p, double phi, double eta, float dca, double nsigma, float beta, short charge);
	Track(const track_defaults& defaults);
	~Track();

	double get_pt();
	double get_p();
	double get_phi();
	double get_eta();
	float get_dca();
	double get_nsigma();
	float get_beta();
	short get_charge();

	void set_pt(double pt);
	void set_p(double p);
	void set_phi(double phi);
	void set_eta(double eta);
	void set_dca(float dca);
	void set_nsigma(double nsigma);
	void set_beta(float beta);
	void set_charge(short charge);

	void set_track(double pt, double p, double phi, double eta, float dca, double nsigma, float beta, short charge);
	void clear();

//	ClassDef(Track, 1);

private:
	double pt;
	double p;
	double phi;
	double eta;
	float dca;
	double nsigma;
	float beta;
	short charge;
};



#endif /* TRACK_H_ */
