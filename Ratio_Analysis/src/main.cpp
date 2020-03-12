/*
 * main.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: Dylan Neff
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include <TROOT.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TH1.h>
#include <TCanvas.h>

#include "config_analysis.h"
#include "plotting.h"
#include "ratio_methods.h"
#include "file_io.h"
#include "Stats.h"
#include "Measure.h"
#include "ThreadPool.h"
#include "AzimuthBinData.h"
#include "AzimuthBinAnalyzer.h"
#include "BinomialAnalyzer.h"

using namespace std;

void azimuth_bin_analyze();
void binomial_analyze();


int main() {
	azimuth_bin_analyze();
//	binomial_analyze();

	cout << "donzo" << endl;
	return(0);
}


void azimuth_bin_analyze() {
	AzimuthBinAnalyzer analyzer;
	analyzer.set_in_path("/home/dylan/Desktop/Collab_Meeting/Data_Old_Ref2/");
	analyzer.set_in_mix_path("/home/dylan/Desktop/Collab_Meeting/Data_Old_Ref2_Mix/");
	analyzer.set_out_path("/home/dylan/Research/Results/");
	analyzer.set_out_root_name("3-11-20_ref2_dist_dists.root");
	analyzer.analyze();
}


void binomial_analyze() {
	BinomialAnalyzer analyzer;
	analyzer.set_in_path("/home/dylan/Desktop/Collab_Meeting/Data_Ampt/");
	analyzer.set_in_mix_path("/home/dylan/Desktop/Collab_Meeting/Data_Ampt_Mix/");
	analyzer.set_out_path("/home/dylan/Research/Results/");
	analyzer.set_out_root_name("3-10-20_ampt_binom_good_plots.root");
	analyzer.set_energies({11, 39});
	analyzer.analyze();
}

