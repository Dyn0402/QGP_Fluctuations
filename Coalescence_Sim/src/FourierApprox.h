/*
 * fourier_approx.h
 *
 *  Created on: Jul 9, 2019
 *      Author: Dylan Neff
 */

#ifndef FOURIERAPPROX_H_
#define FOURIERAPPROX_H_

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
	pair<vector<double>, vector<double>> coefs;
	vector<TF1*> expansion_terms;
	int fourier_n_min, fourier_n_max;
	double hist_x_min, hist_x_max;
};


double get_avg(TH1D *hist, TF1 *func);
pair<vector<double>, vector<double>> calc_coef(TH1D* hist, int n_min, int n_max, double x_min, double x_max);
vector<TF1*> get_expansion(vector<double> A, vector<double> B, int n_min, int n_max, double x_min, double x_max);
TF1* get_func(vector<TF1*> expansion_terms);


#endif /* FOURIERAPPROX_H_ */
