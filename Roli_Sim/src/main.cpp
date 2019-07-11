/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: dylan
 */


#include <iostream>
#include <thread>

#include <TROOT.h>
#include <TRandom3.h>
#include <TString.h>

using namespace std;


void sim_efficiency(TString outputFile, double pc, double xcluster, double mean, int div);


int main() {
	TRandom3 Test1(0);
	TRandom3 Test2(0);

	for(int i = 0; i < 50; i++) {
		cout << "Test1: " << Test1.Rndm() << " | Test2: " << Test2.Rndm() << endl;
	}

	return(0);
}


void run_sims() {
	vector<int> divs = {2,3,4,5,6};
	vector<double> cluster_percentages = {0.0, 0.025, 0.05, 0.075, 0.1, 0.125, 0.15};
	vector<double> mean_list = {21.33}; //{39.3, 31.2, 25.99, 24.93, 19.06, 17.62, 15.08};
	double pc;

	ROOT::EnableThreadSafety();
	vector<thread> threads;
	for(int energy:local::energy_list) {
		threads.push_back(thread(read_energy, energy));
//		usleep(20000000);
	}
	for(thread & th : threads) {
		if(th.joinable()) {
			th.join();
		}
	}
}
