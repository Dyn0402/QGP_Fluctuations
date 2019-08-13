/*
 * performance.cpp
 *
 *  Created on: Aug 12, 2019
 *      Author: dylan
 */

#include <iostream>
#include <linux/kernel.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <ios>
#include <fstream>
#include <string>

#include <TFile.h>

#include "ratio_methods.h"
#include "ThreadPool.h"
#include "Stats.h"
#include "../src/Simulator2.h"
#include "Measure.h"
#include "Plotter.h"
#include "RatioData.h"

#include "performance.h"
#include "moments_vs_sim_pars2.h"


void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}


void ram_test() {
	TFile *out_file = new TFile("/home/dylan/Desktop/perf_test.root", "RECREATE");
	const double megabyte = 1024 * 1024;
	struct sysinfo si;
	sysinfo (&si);
	double vm, rss;

	cout << "Free Ram pre: " << si.freeram / megabyte << "Mb" << endl;
	process_mem_usage(vm, rss);
	cout << "Free Ram pre:  vm=" << vm << "  rss=" << rss << endl;

	map<double, map<double, map<string, Measure>>> results;
	double mean = 30;
	double spread = 0.2;
	int n_events = 10000000;
	int div = 6;
	double p_group = 0.05;

//	run_pars_mixed2(n_events, mean, p_group, spread, div, out_file, &results);

	ostringstream os("");
	os << "mean: " << setprecision(2) << mean << "  p_group: " << setprecision(2) << p_group << "  spread: " << setprecision(2) << spread << ends;
	string name = os.str();

	cout << "Starting: \t " << name << endl;

//	vector<RatioData> data = run_sim_mixed2(p_group, mean, n_events, spread, div);
	// run_sim_mixed2 broken out
	Simulator2 sim;
	sim.set_p_group(p_group);
	sim.set_particle_mean(mean);
	sim.set_n_events(n_events);
	sim.set_spread_sigma(spread);
	sim.set_divisions(div);
	cout << "Starting simulation of mixed events in 2 seconds..." << endl;
	usleep(2000000);
	cout << "Starting simulation of mixed events." << endl;
	vector<map<int, map<int, int>>> sim_res = sim.run_simulation_mixed();
	cout << "Finished simulation of mixed events. Continuing in 10 seconds..." << endl;
	usleep(10000000);
	RatioData data1(div);
	data1.set_ratio_data(sim_res[0]);
	RatioData mix_data(div);
	mix_data.set_ratio_data(sim_res[1]);

	vector<RatioData> data = {data1, mix_data};

	cout << "Have RatioData" << endl;
	cout << "Free Ram pre: " << si.freeram / megabyte << "Mb" << endl;
	out_file->cd();
	data[0].canvas_2d_dist(name);
	data[1].canvas_2d_dist(name+"_mixed");
	cout << "Plotted" << endl;
	map<double, int> ratios = data[0].get_ratio_hist();
	map<double, int> mix_ratios = data[1].get_ratio_hist();
	cout << "Have ratios" << endl;
	cout << "Free Ram pre: " << si.freeram / megabyte << "Mb" << endl;
	vector<map<double, int>> both = {ratios, mix_ratios};

	cout << "Getting stats" << endl;

	get_moments_mixed2(both, &((results)[p_group][mean]));

	cout << "Finishing: \t " << name << endl;

//	RatioData data = run_sim2(0.05, 30, 10000000, 0.2, 6);
//	out_file->cd();
//	data.canvas_2d_dist("Perf_Test");
//	map<double, int> ratio_hist = data.get_ratio_hist();
//
//	get_moments2(ratio_hist, &((results)[0.05][30]));
//	Simulator2 sim;
//	sim.set_p_group(0.05);
//	sim.set_particle_mean(30);
//	sim.set_n_events(10000000);
//	sim.set_spread_sigma(0.2);
//	sim.set_divisions(6);
//	RatioData data(6);
//	data.set_ratio_data(sim.run_simulation());
	out_file->Close();
	cout << "Free Ram after sim run & RatioData set: " << si.freeram / megabyte << "Mb" << endl;
	process_mem_usage(vm, rss);
	cout << "Free Ram post:  vm=" << vm << "  rss=" << rss << endl;
	delete out_file;
}
