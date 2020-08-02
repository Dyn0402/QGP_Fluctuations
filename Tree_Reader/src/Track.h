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
	float pt = 0.7;
	float p = 0.7;
	float phi = 0.0;
	float eta = 0.0;

	float dca = 0.1;
	float nsigma = 1.0;
	float beta = -999;
	short charge = 1;
};


//class Track : public TObject {
class Track {
public:
	Track();
	Track(float pt, float p, float phi, float eta, float dca, float nsigma, float beta, short charge);
	Track(const track_defaults& defaults);
	~Track();

	float get_pt();
	float get_p();
	float get_phi();
	float get_eta();
	float get_dca();
	float get_nsigma();
	float get_beta();
	short get_charge();

	void set_pt(float pt);
	void set_p(float p);
	void set_phi(float phi);
	void set_eta(float eta);
	void set_dca(float dca);
	void set_nsigma(float nsigma);
	void set_beta(float beta);
	void set_charge(short charge);

	void set_track(float pt, float p, float phi, float eta, float dca, float nsigma, float beta, short charge);
	void clear();

//	ClassDef(Track, 1);

private:
	float p;
	float pt;
	float phi;
	float eta;
	float dca;
	float nsigma;
	float beta;
	short charge;
};



#endif /* TRACK_H_ */
