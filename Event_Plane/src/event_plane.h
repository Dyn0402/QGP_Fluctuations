/*
 * event_plane.h
 *
 *  Created on: May 5, 2019
 *      Author: dylan
 */

#ifndef EVENT_PLANE_H_
#define EVENT_PLANE_H_

#include <vector>
#include <TRandom3.h>

using namespace std;

double genAngle(TRandom3 *rand);
double genv2Angle(double v2, double reactAngle, TRandom3 *rand);
vector<double> genParticles(double n, double v2, double reactAngle, TRandom3 *rand);
vector<double> getQ(vector<double> particles, int nHarmonic);
double getQAngle(vector<double> Q, int nHarmonic);
double getSubRes(vector<double> particles, int nHarmonic);
double getV2Obs(vector<double> particles, int nHarmonic);
double resf(double *x, double *pars);
double getFullRes(double subRes);
double getV2ObsCheat(vector<double> particles, int nHarmonic, double reactAngle);



#endif /* EVENT_PLANE_H_ */
