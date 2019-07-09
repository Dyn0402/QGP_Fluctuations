/*
 * fourier_approx.h
 *
 *  Created on: Jul 9, 2019
 *      Author: Dylan Neff
 */

#ifndef FOURIER_APPROX_H_
#define FOURIER_APPROX_H_

#include <vector>
#include <tuple>

#include <TH1.h>
#include <TF1.h>

using namespace std;


class FourierApprox
{
	public:
	FourierApprox(TH1D *hist);
	~FourierApprox();

	//Methods
	TF1* get_approx();

	//Setters
	void set_fourier_n_min(int n_min);
	void set_fourier_n_max(int n_max);

	//Getters
	int get_fourier_n_min();
	int get_fourier_n_max();


	private:
	TH1D *hist_to_approx;
	int fourier_n_min, fourier_n_max;
	double hist_x_min, hist_x_max;
};


double getAvg(TH1D *hist, TF1 *func);
tuple<vector<double>, vector<double>> calcCoef(TH1D* hist, int n_min, int n_max, double x_min, double x_max);


#endif /* FOURIER_APPROX_H_ */
