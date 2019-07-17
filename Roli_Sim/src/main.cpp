/*
 * main.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: dylan
 */


#include <iostream>
#include <thread>
#include <chrono>

#include <TROOT.h>
#include <TRandom3.h>
#include <TString.h>

#include "ThreadPool.h"

using namespace std;


void sim_efficiency(TString outputFile, double pc, double xcluster, double mean, int div);
void th_pool_test();
void th_test_func(int time = 4, string message="test function");
void trand_test();
void example();


int main() {
//	th_pool_test();
	example();

	cout << "donzo" << endl;

	return(0);
}


void run_sims() {
	vector<int> divs = {2,3,4,5,6};
	vector<double> cluster_percentages = {0.0, 0.025, 0.05, 0.075, 0.1, 0.125, 0.15};
	vector<double> mean_list = {21.33}; //{39.3, 31.2, 25.99, 24.93, 19.06, 17.62, 15.08};
	double pc;

	ROOT::EnableThreadSafety();
	vector<thread> threads;
//	for(int energy:local::energy_list) {
//		threads.push_back(thread(read_energy, energy));
//		usleep(20000000);
//	}
//	for(thread & th : threads) {
//		if(th.joinable()) {
//			th.join();
//		}
//	}
}


void trand_test() {
	TRandom3 Test1(0);
	TRandom3 Test2(0);

	for(int i = 0; i < 50; i++) {
		cout << "Test1: " << Test1.Rndm() << " | Test2: " << Test2.Rndm() << endl;
	}
}


void th_pool_test() {
	ThreadPool test_pool(2);
//	test_pool.enqueue(th_test_func); //Doesn't work
	test_pool.enqueue(th_test_func, 4, "testtet");
	test_pool.enqueue(th_test_func, 8, "8 sec");
	test_pool.enqueue(th_test_func, 2, "hardware" + to_string(thread::hardware_concurrency()));
}


void th_test_func(int time, string message) {
	chrono::seconds wait_time(time);
	this_thread::sleep_for(wait_time);
	cout << "Message: " << message << endl;
}
