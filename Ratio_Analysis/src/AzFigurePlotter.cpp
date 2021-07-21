
#include <vector>

#include "AzFigurePlotter.h"
#include "plotting.h"

#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TStyle.h>

using namespace std;


// Structors

AzFigurePlotter::AzFigurePlotter() {
	// Nothing
}

AzFigurePlotter::~AzFigurePlotter() {
	// Nothing
}


// Getters


// Setters

void AzFigurePlotter::set_zoom(bool zoom) {
	this->zoom = zoom;
}


// Doers

void AzFigurePlotter::plot_paper_figs() {
	// [setgroup_name] [data_type] [energy] [div] [cent] [stat_name]
	cout << "Making paper figures" << endl;
	TFile fig_file((out_path+"figures_"+out_root_name).data(), "RECREATE");
	fig_file.cd();
	kurt_vs_energy();
	kurt_vs_energy_divs();
	kurt_vs_energy_cents();
	moments_vs_energy();
	kurt_vs_rapidity();
	kurt_vs_efficiency();
	fig_file.Close();
}

void AzFigurePlotter::kurt_vs_energy() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	int div = 120;
	int cent = 8;
	string stat = "non_excess_kurtosis";

	plot_data bes1_ratio;
	plot_data ampt_ratio;
	plot_data bes1_pull;
	plot_data ampt_pull;

	for (int energy : energies) {
		bes1_ratio.val.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_val());
		bes1_ratio.stat.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_err());
		bes1_ratio.sys.push_back(def_systematics["BES1"]["divide"][energy][div][cent][stat]);
		bes1_pull.val.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_val());
		bes1_pull.stat.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_err());
		bes1_pull.sys.push_back(def_systematics["BES1"]["pull_divide"][energy][div][cent][stat]);
		ampt_ratio.val.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_val());
		ampt_ratio.stat.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_err());
		ampt_ratio.sys.push_back(def_systematics["AMPT"]["divide"][energy][div][cent][stat]);
		ampt_pull.val.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_val());
		ampt_pull.stat.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_err());
		ampt_pull.sys.push_back(def_systematics["AMPT"]["pull_divide"][energy][div][cent][stat]);
	}

	kurt_vs_energy_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	kurt_vs_energy_plot(energies, bes1_pull, ampt_pull, "Pull");
}


void AzFigurePlotter::kurt_vs_energy_divs() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	vector<int> divs{ 60, 72, 90, 120, 180, 240, 270, 300 };
	int cent = 8;
	string stat = "non_excess_kurtosis";

	map<int, plot_data> bes1_ratio;
	map<int, plot_data> ampt_ratio;
	map<int, plot_data> bes1_pull;
	map<int, plot_data> ampt_pull;

	for (int div : divs) {
		for (int energy : energies) {
			bes1_ratio[div].val.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_val());
			bes1_ratio[div].stat.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_err());
			bes1_ratio[div].sys.push_back(def_systematics["BES1"]["divide"][energy][div][cent][stat]);
			bes1_pull[div].val.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_val());
			bes1_pull[div].stat.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_err());
			bes1_pull[div].sys.push_back(def_systematics["BES1"]["pull_divide"][energy][div][cent][stat]);
			ampt_ratio[div].val.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_val());
			ampt_ratio[div].stat.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_err());
			ampt_ratio[div].sys.push_back(def_systematics["AMPT"]["divide"][energy][div][cent][stat]);
			ampt_pull[div].val.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_val());
			ampt_pull[div].stat.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_err());
			ampt_pull[div].sys.push_back(def_systematics["AMPT"]["pull_divide"][energy][div][cent][stat]);
		}
	}

	kurt_vs_energy_divs_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	kurt_vs_energy_divs_plot(energies, bes1_pull, ampt_pull, "Pull");
}


void AzFigurePlotter::kurt_vs_energy_cents() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	int div = 120;
	vector<int> cents{ 8, 7, 6, 5, 4, 3, 2, 1 };
	string stat = "non_excess_kurtosis";

	map<int, plot_data> bes1_ratio;
	map<int, plot_data> ampt_ratio;
	map<int, plot_data> bes1_pull;
	map<int, plot_data> ampt_pull;

	for (int cent : cents) {
		for (int energy : energies) {
			bes1_ratio[cent].val.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_val());
			bes1_ratio[cent].stat.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_err());
			bes1_ratio[cent].sys.push_back(def_systematics["BES1"]["divide"][energy][div][cent][stat]);
			bes1_pull[cent].val.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_val());
			bes1_pull[cent].stat.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_err());
			bes1_pull[cent].sys.push_back(def_systematics["BES1"]["pull_divide"][energy][div][cent][stat]);
			ampt_ratio[cent].val.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_val());
			ampt_ratio[cent].stat.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_err());
			ampt_ratio[cent].sys.push_back(def_systematics["AMPT"]["divide"][energy][div][cent][stat]);
			ampt_pull[cent].val.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_val());
			ampt_pull[cent].stat.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_err());
			ampt_pull[cent].sys.push_back(def_systematics["AMPT"]["pull_divide"][energy][div][cent][stat]);
		}
	}

	kurt_vs_energy_cents_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	kurt_vs_energy_cents_plot(energies, bes1_pull, ampt_pull, "Pull");
}


void AzFigurePlotter::moments_vs_energy() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	int div = 120;
	int cent = 8;
	vector<string> stats { "mean", "standard_deviation", "skewness", "non_excess_kurtosis" };

	map<string, map<string, plot_data>> bes1_ratio;
	map<string, map<string, plot_data>> ampt_ratio;
	map<string, map<string, plot_data>> bes1_pull;
	map<string, map<string, plot_data>> ampt_pull;

	for (string stat: stats) {
		for (int energy : energies) {
			bes1_ratio["divide"][stat].val.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_val());
			bes1_ratio["divide"][stat].stat.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_err());
			bes1_ratio["divide"][stat].sys.push_back(def_systematics["BES1"]["divide"][energy][div][cent][stat]);
			bes1_pull["divide"][stat].val.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_val());
			bes1_pull["divide"][stat].stat.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_err());
			bes1_pull["divide"][stat].sys.push_back(def_systematics["BES1"]["pull_divide"][energy][div][cent][stat]);
			ampt_ratio["divide"][stat].val.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_val());
			ampt_ratio["divide"][stat].stat.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_err());
			ampt_ratio["divide"][stat].sys.push_back(def_systematics["AMPT"]["divide"][energy][div][cent][stat]);
			ampt_pull["divide"][stat].val.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_val());
			ampt_pull["divide"][stat].stat.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_err());
			ampt_pull["divide"][stat].sys.push_back(def_systematics["AMPT"]["pull_divide"][energy][div][cent][stat]);

			bes1_ratio["raw"][stat].val.push_back(def_medians["BES1"]["raw"][energy][div][cent][stat].get_val());
			bes1_ratio["raw"][stat].stat.push_back(def_medians["BES1"]["raw"][energy][div][cent][stat].get_err());
			bes1_ratio["raw"][stat].sys.push_back(def_systematics["BES1"]["raw"][energy][div][cent][stat]);
			bes1_pull["raw"][stat].val.push_back(def_medians["BES1"]["pull_raw"][energy][div][cent][stat].get_val());
			bes1_pull["raw"][stat].stat.push_back(def_medians["BES1"]["pull_raw"][energy][div][cent][stat].get_err());
			bes1_pull["raw"][stat].sys.push_back(def_systematics["BES1"]["pull_raw"][energy][div][cent][stat]);
			ampt_ratio["raw"][stat].val.push_back(def_medians["AMPT"]["raw"][energy][div][cent][stat].get_val());
			ampt_ratio["raw"][stat].stat.push_back(def_medians["AMPT"]["raw"][energy][div][cent][stat].get_err());
			ampt_ratio["raw"][stat].sys.push_back(def_systematics["AMPT"]["raw"][energy][div][cent][stat]);
			ampt_pull["raw"][stat].val.push_back(def_medians["AMPT"]["pull_raw"][energy][div][cent][stat].get_val());
			ampt_pull["raw"][stat].stat.push_back(def_medians["AMPT"]["pull_raw"][energy][div][cent][stat].get_err());
			ampt_pull["raw"][stat].sys.push_back(def_systematics["AMPT"]["pull_raw"][energy][div][cent][stat]);

			bes1_ratio["mix"][stat].val.push_back(def_medians["BES1"]["mix"][energy][div][cent][stat].get_val());
			bes1_ratio["mix"][stat].stat.push_back(def_medians["BES1"]["mix"][energy][div][cent][stat].get_err());
			bes1_ratio["mix"][stat].sys.push_back(def_systematics["BES1"]["mix"][energy][div][cent][stat]);
			bes1_pull["mix"][stat].val.push_back(def_medians["BES1"]["pull_mix"][energy][div][cent][stat].get_val());
			bes1_pull["mix"][stat].stat.push_back(def_medians["BES1"]["pull_mix"][energy][div][cent][stat].get_err());
			bes1_pull["mix"][stat].sys.push_back(def_systematics["BES1"]["pull_mix"][energy][div][cent][stat]);
			ampt_ratio["mix"][stat].val.push_back(def_medians["AMPT"]["mix"][energy][div][cent][stat].get_val());
			ampt_ratio["mix"][stat].stat.push_back(def_medians["AMPT"]["mix"][energy][div][cent][stat].get_err());
			ampt_ratio["mix"][stat].sys.push_back(def_systematics["AMPT"]["mix"][energy][div][cent][stat]);
			ampt_pull["mix"][stat].val.push_back(def_medians["AMPT"]["pull_mix"][energy][div][cent][stat].get_val());
			ampt_pull["mix"][stat].stat.push_back(def_medians["AMPT"]["pull_mix"][energy][div][cent][stat].get_err());
			ampt_pull["mix"][stat].sys.push_back(def_systematics["AMPT"]["pull_mix"][energy][div][cent][stat]);
		}
	}

	moments_vs_energy_plot(energies, bes1_ratio, ampt_ratio, stats, "Ratio");
	moments_vs_energy_plot(energies, bes1_pull, ampt_pull, stats, "Pull");
}


void AzFigurePlotter::kurt_vs_rapidity() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	int div = 120;
	int cent = 8;
//	vector<float> rapidities{ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0 };
	map<float, string> rapidities{ {0.1, "01"}, {0.2, "02"}, {0.3, "03"}, {0.4, "04"}, {0.5, "default"}, {0.6, "06"}, {0.8, "08"}, {1.0, "1"} };
	string stat = "non_excess_kurtosis";

	map<string, map<float, plot_data>> bes1_ratio;
	map<string, map<float, plot_data>> ampt_ratio;
	map<string, map<float, plot_data>> bes1_pull;
	map<string, map<float, plot_data>> ampt_pull;

	for (pair<float, string> rapid : rapidities) {
		string bes = "BES1_rapid" + rapid.second;
		string ampt = "AMPT_rapid" + rapid.second;
		if (rapid.second == "default") {
			bes = "BES1";
			ampt = "AMPT";
		}
		for (int energy : energies) {
			bes1_ratio["divide"][rapid.first].val.push_back(def_medians[bes]["divide"][energy][div][cent][stat].get_val());
			bes1_ratio["divide"][rapid.first].stat.push_back(def_medians[bes]["divide"][energy][div][cent][stat].get_err());
			bes1_ratio["divide"][rapid.first].sys.push_back(def_systematics[bes]["divide"][energy][div][cent][stat]);
			bes1_pull["divide"][rapid.first].val.push_back(def_medians[bes]["pull_divide"][energy][div][cent][stat].get_val());
			bes1_pull["divide"][rapid.first].stat.push_back(def_medians[bes]["pull_divide"][energy][div][cent][stat].get_err());
			bes1_pull["divide"][rapid.first].sys.push_back(def_systematics[bes]["pull_divide"][energy][div][cent][stat]);
			ampt_ratio["divide"][rapid.first].val.push_back(def_medians[ampt]["divide"][energy][div][cent][stat].get_val());
			ampt_ratio["divide"][rapid.first].stat.push_back(def_medians[ampt]["divide"][energy][div][cent][stat].get_err());
			ampt_ratio["divide"][rapid.first].sys.push_back(def_systematics[ampt]["divide"][energy][div][cent][stat]);
			ampt_pull["divide"][rapid.first].val.push_back(def_medians[ampt]["pull_divide"][energy][div][cent][stat].get_val());
			ampt_pull["divide"][rapid.first].stat.push_back(def_medians[ampt]["pull_divide"][energy][div][cent][stat].get_err());
			ampt_pull["divide"][rapid.first].sys.push_back(def_systematics[ampt]["pull_divide"][energy][div][cent][stat]);

			bes1_ratio["raw"][rapid.first].val.push_back(def_medians[bes]["raw"][energy][div][cent][stat].get_val());
			bes1_ratio["raw"][rapid.first].stat.push_back(def_medians[bes]["raw"][energy][div][cent][stat].get_err());
			bes1_ratio["raw"][rapid.first].sys.push_back(def_systematics[bes]["raw"][energy][div][cent][stat]);
			bes1_pull["raw"][rapid.first].val.push_back(def_medians[bes]["pull_raw"][energy][div][cent][stat].get_val());
			bes1_pull["raw"][rapid.first].stat.push_back(def_medians[bes]["pull_raw"][energy][div][cent][stat].get_err());
			bes1_pull["raw"][rapid.first].sys.push_back(def_systematics[bes]["pull_raw"][energy][div][cent][stat]);
			ampt_ratio["raw"][rapid.first].val.push_back(def_medians[ampt]["raw"][energy][div][cent][stat].get_val());
			ampt_ratio["raw"][rapid.first].stat.push_back(def_medians[ampt]["raw"][energy][div][cent][stat].get_err());
			ampt_ratio["raw"][rapid.first].sys.push_back(def_systematics[ampt]["raw"][energy][div][cent][stat]);
			ampt_pull["raw"][rapid.first].val.push_back(def_medians[ampt]["pull_raw"][energy][div][cent][stat].get_val());
			ampt_pull["raw"][rapid.first].stat.push_back(def_medians[ampt]["pull_raw"][energy][div][cent][stat].get_err());
			ampt_pull["raw"][rapid.first].sys.push_back(def_systematics[ampt]["pull_raw"][energy][div][cent][stat]);

			bes1_ratio["mix"][rapid.first].val.push_back(def_medians[bes]["mix"][energy][div][cent][stat].get_val());
			bes1_ratio["mix"][rapid.first].stat.push_back(def_medians[bes]["mix"][energy][div][cent][stat].get_err());
			bes1_ratio["mix"][rapid.first].sys.push_back(def_systematics[bes]["mix"][energy][div][cent][stat]);
			bes1_pull["mix"][rapid.first].val.push_back(def_medians[bes]["pull_mix"][energy][div][cent][stat].get_val());
			bes1_pull["mix"][rapid.first].stat.push_back(def_medians[bes]["pull_mix"][energy][div][cent][stat].get_err());
			bes1_pull["mix"][rapid.first].sys.push_back(def_systematics[bes]["pull_mix"][energy][div][cent][stat]);
			ampt_ratio["mix"][rapid.first].val.push_back(def_medians[ampt]["mix"][energy][div][cent][stat].get_val());
			ampt_ratio["mix"][rapid.first].stat.push_back(def_medians[ampt]["mix"][energy][div][cent][stat].get_err());
			ampt_ratio["mix"][rapid.first].sys.push_back(def_systematics[ampt]["mix"][energy][div][cent][stat]);
			ampt_pull["mix"][rapid.first].val.push_back(def_medians[ampt]["pull_mix"][energy][div][cent][stat].get_val());
			ampt_pull["mix"][rapid.first].stat.push_back(def_medians[ampt]["pull_mix"][energy][div][cent][stat].get_err());
			ampt_pull["mix"][rapid.first].sys.push_back(def_systematics[ampt]["pull_mix"][energy][div][cent][stat]);
		}
	}

	kurt_vs_rapid_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	kurt_vs_rapid_plot(energies, bes1_pull, ampt_pull, "Pull");
}


void AzFigurePlotter::kurt_vs_efficiency() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	int div = 120;
	int cent = 8;
	map<float, string> efficiencies{ {0.0, "default"}, {0.05, "05"}, {0.1, "1"}, {0.15, "15"}, {0.2, "2"}, {0.3, "3"} };
	string stat = "non_excess_kurtosis";

	map<string, map<float, plot_data>> bes1_ratio;
	map<string, map<float, plot_data>> ampt_ratio;
	map<string, map<float, plot_data>> bes1_pull;
	map<string, map<float, plot_data>> ampt_pull;

	for (pair<float, string> eff : efficiencies) {
		string bes = "BES1_Eff" + eff.second;
		string ampt = "AMPT_Eff" + eff.second;
		if (eff.second == "default") {
			bes = "BES1";
			ampt = "AMPT";
		}
		for (int energy : energies) {
			bes1_ratio["divide"][eff.first].val.push_back(def_medians[bes]["divide"][energy][div][cent][stat].get_val());
			bes1_ratio["divide"][eff.first].stat.push_back(def_medians[bes]["divide"][energy][div][cent][stat].get_err());
			bes1_ratio["divide"][eff.first].sys.push_back(def_systematics[bes]["divide"][energy][div][cent][stat]);
			bes1_pull["divide"][eff.first].val.push_back(def_medians[bes]["pull_divide"][energy][div][cent][stat].get_val());
			bes1_pull["divide"][eff.first].stat.push_back(def_medians[bes]["pull_divide"][energy][div][cent][stat].get_err());
			bes1_pull["divide"][eff.first].sys.push_back(def_systematics[bes]["pull_divide"][energy][div][cent][stat]);
			ampt_ratio["divide"][eff.first].val.push_back(def_medians[ampt]["divide"][energy][div][cent][stat].get_val());
			ampt_ratio["divide"][eff.first].stat.push_back(def_medians[ampt]["divide"][energy][div][cent][stat].get_err());
			ampt_ratio["divide"][eff.first].sys.push_back(def_systematics[ampt]["divide"][energy][div][cent][stat]);
			ampt_pull["divide"][eff.first].val.push_back(def_medians[ampt]["pull_divide"][energy][div][cent][stat].get_val());
			ampt_pull["divide"][eff.first].stat.push_back(def_medians[ampt]["pull_divide"][energy][div][cent][stat].get_err());
			ampt_pull["divide"][eff.first].sys.push_back(def_systematics[ampt]["pull_divide"][energy][div][cent][stat]);

			bes1_ratio["raw"][eff.first].val.push_back(def_medians[bes]["raw"][energy][div][cent][stat].get_val());
			bes1_ratio["raw"][eff.first].stat.push_back(def_medians[bes]["raw"][energy][div][cent][stat].get_err());
			bes1_ratio["raw"][eff.first].sys.push_back(def_systematics[bes]["raw"][energy][div][cent][stat]);
			bes1_pull["raw"][eff.first].val.push_back(def_medians[bes]["pull_raw"][energy][div][cent][stat].get_val());
			bes1_pull["raw"][eff.first].stat.push_back(def_medians[bes]["pull_raw"][energy][div][cent][stat].get_err());
			bes1_pull["raw"][eff.first].sys.push_back(def_systematics[bes]["pull_raw"][energy][div][cent][stat]);
			ampt_ratio["raw"][eff.first].val.push_back(def_medians[ampt]["raw"][energy][div][cent][stat].get_val());
			ampt_ratio["raw"][eff.first].stat.push_back(def_medians[ampt]["raw"][energy][div][cent][stat].get_err());
			ampt_ratio["raw"][eff.first].sys.push_back(def_systematics[ampt]["raw"][energy][div][cent][stat]);
			ampt_pull["raw"][eff.first].val.push_back(def_medians[ampt]["pull_raw"][energy][div][cent][stat].get_val());
			ampt_pull["raw"][eff.first].stat.push_back(def_medians[ampt]["pull_raw"][energy][div][cent][stat].get_err());
			ampt_pull["raw"][eff.first].sys.push_back(def_systematics[ampt]["pull_raw"][energy][div][cent][stat]);

			bes1_ratio["mix"][eff.first].val.push_back(def_medians[bes]["mix"][energy][div][cent][stat].get_val());
			bes1_ratio["mix"][eff.first].stat.push_back(def_medians[bes]["mix"][energy][div][cent][stat].get_err());
			bes1_ratio["mix"][eff.first].sys.push_back(def_systematics[bes]["mix"][energy][div][cent][stat]);
			bes1_pull["mix"][eff.first].val.push_back(def_medians[bes]["pull_mix"][energy][div][cent][stat].get_val());
			bes1_pull["mix"][eff.first].stat.push_back(def_medians[bes]["pull_mix"][energy][div][cent][stat].get_err());
			bes1_pull["mix"][eff.first].sys.push_back(def_systematics[bes]["pull_mix"][energy][div][cent][stat]);
			ampt_ratio["mix"][eff.first].val.push_back(def_medians[ampt]["mix"][energy][div][cent][stat].get_val());
			ampt_ratio["mix"][eff.first].stat.push_back(def_medians[ampt]["mix"][energy][div][cent][stat].get_err());
			ampt_ratio["mix"][eff.first].sys.push_back(def_systematics[ampt]["mix"][energy][div][cent][stat]);
			ampt_pull["mix"][eff.first].val.push_back(def_medians[ampt]["pull_mix"][energy][div][cent][stat].get_val());
			ampt_pull["mix"][eff.first].stat.push_back(def_medians[ampt]["pull_mix"][energy][div][cent][stat].get_err());
			ampt_pull["mix"][eff.first].sys.push_back(def_systematics[ampt]["pull_mix"][energy][div][cent][stat]);
		}
	}

	kurt_vs_efficiency_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	kurt_vs_efficiency_plot(energies, bes1_pull, ampt_pull, "Pull");
}
 

void AzFigurePlotter::kurt_vs_energy_plot(vector<int> energies, plot_data bes1, plot_data ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	for (double energy : energies) {
		energy_bes1.push_back(energy - 0.5);
		energy_ampt.push_back(energy + 0.5);
	}

	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_can_"+type_name).data(), ("Kurtosis vs Energy " + type_name).data(), 955, 805);
	TMultiGraph* mg = new TMultiGraph();
	mg->SetName("Kurtosis_vs_energy_mg");
	TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
	TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1.val, energy_err, bes1.stat);
	TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt.val, energy_err, ampt.stat);
	bes1_ratio_def_g->SetNameTitle("bes1");
	ampt_ratio_def_g->SetNameTitle("ampt");
	TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1.val, energy_err, bes1.sys);
	TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt.val, energy_err, ampt.sys);

	bes1_ratio_def_g->SetMarkerStyle(29);
	bes1_ratio_def_g->SetMarkerColor(kBlack);
	bes1_ratio_def_g->SetLineColor(kBlack);
	bes1_ratio_def_g->SetMarkerSize(2);
	bes1_ratio_sys_g->SetLineColor(kBlack);

	ampt_ratio_def_g->SetMarkerStyle(22);
	ampt_ratio_def_g->SetMarkerColor(kRed);
	ampt_ratio_def_g->SetLineColor(kRed);
	ampt_ratio_def_g->SetMarkerSize(1.8);
	ampt_ratio_sys_g->SetLineColor(kRed);

	mg->Add(bes1_ratio_def_g, "APLZ");
	mg->Add(ampt_ratio_def_g, "PL");
	mg->Add(bes1_ratio_sys_g, "[]");
	mg->Add(ampt_ratio_sys_g, "[]");

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->AddEntry(bes1_ratio_def_g, "BES1", "lp");
	leg->AddEntry(ampt_ratio_def_g, "AMPT", "lp");

	mg->GetXaxis()->SetLimits(0, 80);
	mg->GetXaxis()->SetRangeUser(0, 80);
	mg->GetYaxis()->SetLimits(0.9955, 1.0145);
	mg->GetYaxis()->SetRangeUser(0.9955, 1.0145);

	mg->GetXaxis()->SetTitle("Energy (GeV)");
	mg->GetYaxis()->SetTitle("Kurtosis");

	mg->Draw("AP");

	TLine* one_line = new TLine(0, 1, 80, 1);
	one_line->SetLineStyle(2);
	one_line->Draw();

	leg->SetMargin(0.1); leg->Draw();

	gPad->SetTopMargin(0.04);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.04);

	can->Update();
	can->Write();

	delete can;
}


void AzFigurePlotter::kurt_vs_energy_divs_plot(vector<int> energies, map<int, plot_data> bes1, map<int, plot_data> ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	for (double energy : energies) {
		energy_bes1.push_back(energy - 0.6);
		energy_ampt.push_back(energy + 0.6);
	}

	map<int, int> div_color{ {60, kBlack}, {72, kRed}, {90, kGreen+3}, {120, kBlue}, {180, kViolet}, {240, kCyan + 2}, {270, kGreen + 1}, {300, kGray + 2} };
	int bes_marker_style = 29;
	int ampt_marker_style = 22;
	map<int, int> div_marker_size{ {60, 2}, {72, 2}, {90, 2}, {120, 2}, {180, 2}, {240, 2}, {270, 2}, {300, 2} };


	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_divs_can_" + type_name).data(), ("Kurtosis vs Energy with Bin Width " + type_name).data(), 955, 805);
	TMultiGraph* mg = new TMultiGraph();
	mg->SetName("Kurtosis_vs_energy_mg");
	TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
	for ( pair<int, plot_data> div : bes1) {
		TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1[div.first].val, energy_err, bes1[div.first].stat);
		TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt[div.first].val, energy_err, ampt[div.first].stat);
		bes1_ratio_def_g->SetNameTitle(("bes1_"+to_string(div.first)).data());
		ampt_ratio_def_g->SetNameTitle(("ampt_" + to_string(div.first)).data());
		TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1[div.first].val, energy_err, bes1[div.first].sys);
		TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt[div.first].val, energy_err, ampt[div.first].sys);

		bes1_ratio_def_g->SetMarkerStyle(bes_marker_style);
		bes1_ratio_def_g->SetMarkerColor(div_color[div.first]);
		bes1_ratio_def_g->SetLineColor(div_color[div.first]);
		bes1_ratio_def_g->SetMarkerSize(div_marker_size[div.first]);
		bes1_ratio_sys_g->SetLineColor(div_color[div.first]);

		ampt_ratio_def_g->SetMarkerStyle(ampt_marker_style);
		ampt_ratio_def_g->SetMarkerColor(div_color[div.first]);
		ampt_ratio_def_g->SetLineColor(div_color[div.first]);
		ampt_ratio_def_g->SetMarkerSize(div_marker_size[div.first]);
		ampt_ratio_sys_g->SetLineColor(div_color[div.first]);

		mg->Add(bes1_ratio_def_g, "APLZ");
		mg->Add(ampt_ratio_def_g, "PL");
		mg->Add(bes1_ratio_sys_g, "[]");
		mg->Add(ampt_ratio_sys_g, "[]");

		leg->AddEntry(bes1_ratio_def_g, ("BES1 " + to_string(div.first) + "#circ").data(), "lp");
		leg->AddEntry(ampt_ratio_def_g, ("AMPT " +  to_string(div.first) + "#circ").data(), "lp");
	}

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);

	mg->GetXaxis()->SetLimits(0, 80);
	mg->GetXaxis()->SetRangeUser(0, 80);
	if (!zoom) {
		mg->GetYaxis()->SetLimits(0.9935, 1.0185);
		mg->GetYaxis()->SetRangeUser(0.9935, 1.0185);
	}

	mg->GetXaxis()->SetTitle("Energy (GeV)");
	mg->GetYaxis()->SetTitle("Kurtosis");

	mg->Draw("AP");

	TLine* one_line = new TLine(0, 1, 80, 1);
	one_line->SetLineStyle(2);
	one_line->Draw();

	leg->SetMargin(0.1); leg->Draw();

	gPad->SetTopMargin(0.04);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.04);

	can->Update();
	can->Write();

	delete can;
}


void AzFigurePlotter::kurt_vs_energy_cents_plot(vector<int> energies, map<int, plot_data> bes1, map<int, plot_data> ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	for (double energy : energies) {
		energy_bes1.push_back(energy - 0.6);
		energy_ampt.push_back(energy + 0.6);
	}

	map<int, int> cent_color{ {8, kBlack}, {7, kBlue}, {6, kRed}, {5, kGreen + 3}, {4, kViolet}, {3, kOrange + 2}, {2, kCyan + 2}, {1, kYellow + 3} };
	int bes_marker_style = 29;
	int ampt_marker_style = 22;
	map<int, int> cent_marker_size{ {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2} };
	map<int, string> cent_string{ {8, "0-5%"}, {7, "5-10%"}, {6, "10-20%"}, {5, "20-30%"}, {4, "30-40%"}, {3, "40-50%"}, {2, "50-60%"}, {1, "60-70%"} };


	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_cents_can_" + type_name).data(), ("Kurtosis vs Energy with Centrality " + type_name).data(), 955, 805);
	TMultiGraph* mg = new TMultiGraph();
	mg->SetName("Kurtosis_vs_energy_mg");
	TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
	for (pair<int, plot_data> cent : bes1) {
		TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1[cent.first].val, energy_err, bes1[cent.first].stat);
		TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt[cent.first].val, energy_err, ampt[cent.first].stat);
		bes1_ratio_def_g->SetNameTitle(("bes1_" + to_string(cent.first)).data());
		ampt_ratio_def_g->SetNameTitle(("ampt_" + to_string(cent.first)).data());
		TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1[cent.first].val, energy_err, bes1[cent.first].sys);
		TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt[cent.first].val, energy_err, ampt[cent.first].sys);

		bes1_ratio_def_g->SetMarkerStyle(bes_marker_style);
		bes1_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetLineColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		bes1_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		ampt_ratio_def_g->SetMarkerStyle(ampt_marker_style);
		ampt_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetLineColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		ampt_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		mg->Add(bes1_ratio_def_g, "APLZ");
		mg->Add(ampt_ratio_def_g, "PL");
		mg->Add(bes1_ratio_sys_g, "[]");
		mg->Add(ampt_ratio_sys_g, "[]");

		leg->AddEntry(bes1_ratio_def_g, ("BES1 " + cent_string[cent.first]).data(), "lp");
		leg->AddEntry(ampt_ratio_def_g, ("AMPT " + cent_string[cent.first]).data(), "lp");
	}

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);

	mg->GetXaxis()->SetLimits(0, 80);
	mg->GetXaxis()->SetRangeUser(0, 80);
	if (!zoom) {
		mg->GetYaxis()->SetLimits(0.9935, 1.0185);
		mg->GetYaxis()->SetRangeUser(0.9935, 1.0185);
	}

	mg->GetXaxis()->SetTitle("Energy (GeV)");
	mg->GetYaxis()->SetTitle("Kurtosis");

	mg->Draw("AP");

	TLine* one_line = new TLine(0, 1, 80, 1);
	one_line->SetLineStyle(2);
	one_line->Draw();

	leg->SetMargin(0.1); leg->Draw();

	gPad->SetTopMargin(0.04);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.04);

	can->Update();
	can->Write();

	delete can;


	// Just BES1
	TCanvas* can_bes = new TCanvas(("BES1_Kurtosis_vs_energy_cents_can_" + type_name).data(), ("Kurtosis vs Energy with Centrality " + type_name).data(), 955, 805);
	TMultiGraph* mg_bes = new TMultiGraph();
	mg_bes->SetName("Kurtosis_vs_energy_mg");
	TLegend* leg_bes = new TLegend(0.3, 0.21, 0.3, 0.21);
	for (pair<int, plot_data> cent : bes1) {
		TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1[cent.first].val, energy_err, bes1[cent.first].stat);
		bes1_ratio_def_g->SetNameTitle(("bes1_" + to_string(cent.first)).data());
		TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1[cent.first].val, energy_err, bes1[cent.first].sys);

		bes1_ratio_def_g->SetMarkerStyle(bes_marker_style);
		bes1_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetLineColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		bes1_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		mg_bes->Add(bes1_ratio_def_g, "APLZ");
		mg_bes->Add(bes1_ratio_sys_g, "[]");

		leg_bes->AddEntry(bes1_ratio_def_g, ("BES1 " + cent_string[cent.first]).data(), "lp");
	}

	leg_bes->SetBorderSize(0);
	leg_bes->SetFillStyle(0);

	mg_bes->GetXaxis()->SetLimits(0, 80);
	mg_bes->GetXaxis()->SetRangeUser(0, 80);
	if (!zoom) {
		mg_bes->GetYaxis()->SetLimits(0.9935, 1.0185);
		mg_bes->GetYaxis()->SetRangeUser(0.9935, 1.0185);
	}

	mg_bes->GetXaxis()->SetTitle("Energy (GeV)");
	mg_bes->GetYaxis()->SetTitle("Kurtosis");

	mg_bes->Draw("AP");

	TLine* one_line_bes = new TLine(0, 1, 80, 1);
	one_line_bes->SetLineStyle(2);
	one_line_bes->Draw();

	leg_bes->SetMargin(0.1); leg_bes->Draw();

	gPad->SetTopMargin(0.04);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.04);

	can_bes->Update();
	can_bes->Write();

	delete can_bes;


	// Just AMPT
	TCanvas* can_ampt = new TCanvas(("AMPT_Kurtosis_vs_energy_cents_can_" + type_name).data(), ("Kurtosis vs Energy with Centrality " + type_name).data(), 955, 805);
	TMultiGraph* mg_ampt = new TMultiGraph();
	mg_ampt->SetName("Kurtosis_vs_energy_mg");
	TLegend* leg_ampt = new TLegend(0.3, 0.21, 0.3, 0.21);
	for (pair<int, plot_data> cent : bes1) {
		TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt[cent.first].val, energy_err, ampt[cent.first].stat);
		ampt_ratio_def_g->SetNameTitle(("ampt_" + to_string(cent.first)).data());
		TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt[cent.first].val, energy_err, ampt[cent.first].sys);

		ampt_ratio_def_g->SetMarkerStyle(ampt_marker_style);
		ampt_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetLineColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		ampt_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		mg_ampt->Add(ampt_ratio_def_g, "PL");
		mg_ampt->Add(ampt_ratio_sys_g, "[]");

		leg_ampt->AddEntry(ampt_ratio_def_g, ("AMPT " + cent_string[cent.first]).data(), "lp");
	}

	leg_ampt->SetBorderSize(0);
	leg_ampt->SetFillStyle(0);

	mg_ampt->GetXaxis()->SetLimits(0, 80);
	mg_ampt->GetXaxis()->SetRangeUser(0, 80);
	if (!zoom) {
		mg_ampt->GetYaxis()->SetLimits(0.9935, 1.0185);
		mg_ampt->GetYaxis()->SetRangeUser(0.9935, 1.0185);
	}

	mg_ampt->GetXaxis()->SetTitle("Energy (GeV)");
	mg_ampt->GetYaxis()->SetTitle("Kurtosis");

	mg_ampt->Draw("AP");

	TLine* one_line_ampt = new TLine(0, 1, 80, 1);
	one_line_ampt->SetLineStyle(2);
	one_line_ampt->Draw();

	leg_ampt->SetMargin(0.1); leg_ampt->Draw();

	gPad->SetTopMargin(0.04);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.04);

	can_ampt->Update();
	can_ampt->Write();

	delete can_ampt;
}


void AzFigurePlotter::moments_vs_energy_plot(vector<int> energies, map<string, map<string, plot_data>> bes1, map<string, map<string, plot_data>> ampt, vector<string> stat_order, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	for (double energy : energies) {
		energy_bes1.push_back(energy - 0.6);
		energy_ampt.push_back(energy + 0.6);
	}

	map<string, int> color{ {"raw", kBlue}, {"mix", kGreen+2}, {"divide", kRed} };
	map<string, int> marker_size{ {"raw", 2}, {"mix", 2}, {"divide", 2} };
	int bes_marker_style = 29;
	int ampt_marker_style = 22;

	TCanvas* can = new TCanvas(("Moments_vs_energy_can_" + type_name).data(), ("Moments vs Energy " + type_name).data(), 955, 805);
//	can->SetPad(0, 0.01, 0.99, 1);
	can->Divide((int)bes1["raw"].size(), 2);
	int can_index = 1;
	for ( string stat : stat_order) { //pair<string, plot_data> stat : bes1["raw"]) {  // Iterate over stats, left to right on plot

		// Raw vs mixed on top plot
		can->cd(can_index);
		gPad->SetTopMargin(0.001);
		gPad->SetBottomMargin(0.001);
//		gPad->SetRightMargin(0.01);
//		gPad->SetLeftMargin(0.01);

		TMultiGraph* mg = new TMultiGraph();
		mg->SetName((stat + "_raw_mixed").data());
		mg->SetTitle((stat).data());

		TGraphErrors* bes1_ratio_raw_def_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][stat].val, energy_err, bes1["raw"][stat].stat);
		TGraphErrors* ampt_ratio_raw_def_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][stat].val, energy_err, ampt["raw"][stat].stat);
		bes1_ratio_raw_def_g->SetNameTitle(("bes1_raw_" + stat).data());
		ampt_ratio_raw_def_g->SetNameTitle(("ampt_raw_" + stat).data());
		TGraphErrors* bes1_ratio_raw_sys_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][stat].val, energy_err, bes1["raw"][stat].sys);
		TGraphErrors* ampt_ratio_raw_sys_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][stat].val, energy_err, ampt["raw"][stat].sys);

		bes1_ratio_raw_def_g->SetMarkerStyle(bes_marker_style);
		bes1_ratio_raw_def_g->SetMarkerColor(color["raw"]);
		bes1_ratio_raw_def_g->SetLineColor(color["raw"]);
		bes1_ratio_raw_def_g->SetMarkerSize(marker_size["raw"]);
		bes1_ratio_raw_sys_g->SetLineColor(color["raw"]);

		ampt_ratio_raw_def_g->SetMarkerStyle(ampt_marker_style);
		ampt_ratio_raw_def_g->SetMarkerColor(color["raw"]);
		ampt_ratio_raw_def_g->SetLineColor(color["raw"]);
		ampt_ratio_raw_def_g->SetMarkerSize(marker_size["raw"]);
		ampt_ratio_raw_sys_g->SetLineColor(color["raw"]);

		TGraphErrors* bes1_ratio_mix_def_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][stat].val, energy_err, bes1["mix"][stat].stat);
		TGraphErrors* ampt_ratio_mix_def_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][stat].val, energy_err, ampt["mix"][stat].stat);
		bes1_ratio_mix_def_g->SetNameTitle(("bes1_mix_" + stat).data());
		ampt_ratio_mix_def_g->SetNameTitle(("ampt_mix_" + stat).data());
		TGraphErrors* bes1_ratio_mix_sys_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][stat].val, energy_err, bes1["mix"][stat].sys);
		TGraphErrors* ampt_ratio_mix_sys_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][stat].val, energy_err, ampt["mix"][stat].sys);

		bes1_ratio_mix_def_g->SetMarkerStyle(bes_marker_style);
		bes1_ratio_mix_def_g->SetMarkerColor(color["mix"]);
		bes1_ratio_mix_def_g->SetLineColor(color["mix"]);
		bes1_ratio_mix_def_g->SetMarkerSize(marker_size["mix"]);
		bes1_ratio_mix_sys_g->SetLineColor(color["mix"]);

		ampt_ratio_mix_def_g->SetMarkerStyle(ampt_marker_style);
		ampt_ratio_mix_def_g->SetMarkerColor(color["mix"]);
		ampt_ratio_mix_def_g->SetLineColor(color["mix"]);
		ampt_ratio_mix_def_g->SetMarkerSize(marker_size["mix"]);
		ampt_ratio_mix_sys_g->SetLineColor(color["mix"]);

		mg->Add(bes1_ratio_raw_def_g, "APLZ");
		mg->Add(ampt_ratio_raw_def_g, "PL");
		mg->Add(bes1_ratio_raw_sys_g, "[]");
		mg->Add(ampt_ratio_raw_sys_g, "[]");
		mg->Add(bes1_ratio_mix_def_g, "PL");
		mg->Add(ampt_ratio_mix_def_g, "PL");
		mg->Add(bes1_ratio_mix_sys_g, "[]");
		mg->Add(ampt_ratio_mix_sys_g, "[]");

		if (stat == "mean") {
			TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
			leg->AddEntry(bes1_ratio_raw_def_g, "BES1 Raw", "lp");
			leg->AddEntry(ampt_ratio_raw_def_g, "AMPT Raw", "lp");
			leg->AddEntry(bes1_ratio_mix_def_g, "BES1 Mixed", "lp");
			leg->AddEntry(ampt_ratio_mix_def_g, "AMPT Mixed", "lp");
			leg->SetBorderSize(0);
			leg->SetFillStyle(0);
			leg->Draw();
		}

		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
		//mg->GetYaxis()->SetLimits(0.9955, 1.0145);
		//mg->GetYaxis()->SetRangeUser(0.9955, 1.0145);

		mg->GetXaxis()->SetTitle("Energy (GeV)");
		//mg->GetYaxis()->SetTitle("Kurtosis");

		mg->Draw("AP");


		// Divided plot on bottom
		can->cd(can_index + (int)bes1["raw"].size());
		gPad->SetTopMargin(0.001);
//		gPad->SetBottomMargin(0.001);
//		gPad->SetRightMargin(0.01);
//		gPad->SetLeftMargin(0.01);
//		gStyle->SetPadTickY(1);

		TMultiGraph* mg_div = new TMultiGraph();
		mg_div->SetName((stat + "_divided").data());

		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][stat].val, energy_err, bes1["divide"][stat].stat);
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][stat].val, energy_err, ampt["divide"][stat].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + stat).data());
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + stat).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][stat].val, energy_err, bes1["divide"][stat].sys);
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][stat].val, energy_err, ampt["divide"][stat].sys);

		bes1_ratio_div_def_g->SetMarkerStyle(bes_marker_style);
		bes1_ratio_div_def_g->SetMarkerColor(color["divide"]);
		bes1_ratio_div_def_g->SetLineColor(color["divide"]);
		bes1_ratio_div_def_g->SetMarkerSize(marker_size["divide"]);
		bes1_ratio_div_sys_g->SetLineColor(color["divide"]);

		ampt_ratio_div_def_g->SetMarkerStyle(ampt_marker_style);
		ampt_ratio_div_def_g->SetMarkerColor(color["divide"]);
		ampt_ratio_div_def_g->SetLineColor(color["divide"]);
		ampt_ratio_div_def_g->SetMarkerSize(marker_size["divide"]);
		ampt_ratio_div_sys_g->SetLineColor(color["divide"]);

		mg_div->Add(bes1_ratio_div_def_g, "APLZ");
		mg_div->Add(ampt_ratio_div_def_g, "PL");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");

		if (stat == "mean") {
			TLegend* leg_div = new TLegend(0.3, 0.21, 0.3, 0.21);
			leg_div->AddEntry(bes1_ratio_div_def_g, "BES1 Raw/Mixed", "lp");
			leg_div->AddEntry(ampt_ratio_div_def_g, "AMPT Raw/Mixed", "lp");
			leg_div->SetBorderSize(0);
			leg_div->SetFillStyle(0);
		}

		mg_div->GetXaxis()->SetLimits(0, 80);
		mg_div->GetXaxis()->SetRangeUser(0, 80);
		//mg_div->GetYaxis()->SetLimits(0.9955, 1.0145);
		//mg_div->GetYaxis()->SetRangeUser(0.9955, 1.0145);

		mg_div->GetXaxis()->SetTitle("Energy (GeV)");
		//mg_div->GetYaxis()->SetTitle("Kurtosis");

		mg_div->Draw("AP");

		if (1 < mg_div->GetYaxis()->GetXmax() && 1 > mg_div->GetYaxis()->GetXmin()) {
			TLine* one_line = new TLine(0, 1, 80, 1);
			one_line->SetLineStyle(2);
			one_line->Draw();
		}

		can_index++;
	}

	//leg->SetMargin(0.1); leg->Draw();

	//gPad->SetTopMargin(0.04);
	//gPad->SetLeftMargin(0.12);
	//gPad->SetRightMargin(0.04);

	can->Update();
	can->Write();

	delete can;
}


void AzFigurePlotter::kurt_vs_rapid_plot(vector<int> energies, map<string, map<float, plot_data>> bes1, map<string, map<float, plot_data>> ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	for (double energy : energies) {
		energy_bes1.push_back(energy - 0.6);
		energy_ampt.push_back(energy + 0.6);
	}

	map<float, int> color{ {1.0, kBlue}, {0.8, kGreen + 3}, {0.6, kRed}, {0.5, kBlack}, {0.4, kGray + 2}, {0.3, kViolet}, {0.2, kCyan + 2}, {0.1, kYellow + 3} };
	map<string, int> marker_style{ {"bes", 29}, {"ampt", 22} };
	map<string, int> marker_size{ {"bes", 2}, {"ampt", 2} };
	map<string, int> marker_color{ {"bes", kBlack}, {"ampt", kRed} };

	TCanvas* can = new TCanvas(("Rapidity_vs_energy_can_" + type_name).data(), ("Rapidity vs Energy " + type_name).data(), 955, 805);
	can->SetPad(0.05, 0.01, 0.99, 1);
	can->Divide(3, 3, 0, 0);
	int can_index = 0;

	for (pair<float, plot_data> rapid : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
		can->cd(++can_index);

		TMultiGraph* mg_div = new TMultiGraph();
		mg_div->SetName(("Rapidity_" + to_string(rapid.first)).data());

		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][rapid.first].val, energy_err, bes1["divide"][rapid.first].stat);
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][rapid.first].val, energy_err, ampt["divide"][rapid.first].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + to_string(rapid.first)).data());
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + to_string(rapid.first)).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][rapid.first].val, energy_err, bes1["divide"][rapid.first].sys);
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][rapid.first].val, energy_err, ampt["divide"][rapid.first].sys);

		bes1_ratio_div_def_g->SetMarkerStyle(marker_style["bes"]);
		bes1_ratio_div_def_g->SetMarkerColor(marker_color["bes"]);
		bes1_ratio_div_def_g->SetLineColor(marker_color["bes"]);
		bes1_ratio_div_def_g->SetMarkerSize(marker_size["bes"]);
		bes1_ratio_div_sys_g->SetLineColor(marker_color["bes"]);

		ampt_ratio_div_def_g->SetMarkerStyle(marker_style["ampt"]);
		ampt_ratio_div_def_g->SetMarkerColor(marker_color["ampt"]);
		ampt_ratio_div_def_g->SetLineColor(marker_color["ampt"]);
		ampt_ratio_div_def_g->SetMarkerSize(marker_size["ampt"]);
		ampt_ratio_div_sys_g->SetLineColor(marker_color["ampt"]);

		mg_div->Add(bes1_ratio_div_def_g, "APLZ");
		mg_div->Add(ampt_ratio_div_def_g, "PL");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");

		TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		leg->SetBorderSize(0);
		leg->SetFillStyle(0);

		leg->AddEntry(("|#eta|<" + to_string(rapid.first).substr(0, 3)).data());

//		leg->AddEntry(bes1_ratio_div_def_g, ("BES1 |#eta|<" + to_string(rapid.first).substr(0, 3)).data(), "lp");
//		leg->AddEntry(ampt_ratio_div_def_g, ("AMPT |#eta|<" + to_string(rapid.first).substr(0, 3)).data(), "lp");

		mg_div->GetXaxis()->SetLimits(0.1, 79.9);
		mg_div->GetXaxis()->SetRangeUser(0.1, 79.9);
		if (!zoom) {
			mg_div->GetYaxis()->SetLimits(0.9935, 1.0185);
			mg_div->GetYaxis()->SetRangeUser(0.9935, 1.0185);
		} else {
			mg_div->GetYaxis()->SetLimits(0.9975, 1.0035);
			mg_div->GetYaxis()->SetRangeUser(0.9975, 1.0035);
		}

		mg_div->GetXaxis()->SetTitle("Energy (GeV)");
		mg_div->GetYaxis()->SetTitle("Kurtosis");

		mg_div->Draw("AP");

		TLine* one_line = new TLine(0, 1, 80, 1);
		one_line->SetLineStyle(2);
		one_line->Draw();

		leg->SetMargin(0.1); leg->Draw();

		if (can_index == 4) {
			can->cd(++can_index);
			TLegend *leg2 = new TLegend();
			leg2->AddEntry(bes1_ratio_div_def_g, "BES1", "lp");
			leg2->AddEntry(ampt_ratio_div_def_g, "AMPT", "lp");
			leg2->Draw();
		}
	}

	can->Update();
	can->Write();

	delete can;


	// Just BES1
	TCanvas* can_bes = new TCanvas(("BES_Rapidity_vs_energy_can_" + type_name).data(), ("Rapidity vs Energy " + type_name).data(), 955, 805);
//	can_bes->Divide(3, 3, 0.001, 0.001);

	TMultiGraph* mg_div_bes = new TMultiGraph();
	mg_div_bes->SetName("Rapidities_divided");

	TLegend* leg_bes = new TLegend(0.3, 0.21, 0.3, 0.21);
	leg_bes->SetBorderSize(0);
	leg_bes->SetFillStyle(0);

	for (pair<float, plot_data> rapid : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][rapid.first].val, energy_err, bes1["divide"][rapid.first].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + to_string(rapid.first)).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][rapid.first].val, energy_err, bes1["divide"][rapid.first].sys);

		bes1_ratio_div_def_g->SetMarkerStyle(marker_style["bes"]);
		bes1_ratio_div_def_g->SetMarkerColor(color[rapid.first]);
		bes1_ratio_div_def_g->SetLineColor(color[rapid.first]);
		bes1_ratio_div_def_g->SetMarkerSize(marker_size["bes"]);
		bes1_ratio_div_sys_g->SetLineColor(color[rapid.first]);

		mg_div_bes->Add(bes1_ratio_div_def_g, "APLZ");
		mg_div_bes->Add(bes1_ratio_div_sys_g, "[]");

		leg_bes->AddEntry(bes1_ratio_div_def_g, ("BES1 |#eta|<" + to_string(rapid.first).substr(0, 3)).data(), "lp");
	}

	mg_div_bes->GetXaxis()->SetLimits(0, 80);
	mg_div_bes->GetXaxis()->SetRangeUser(0, 80);
	if (!zoom) {
		mg_div_bes->GetYaxis()->SetLimits(0.9935, 1.0185);
		mg_div_bes->GetYaxis()->SetRangeUser(0.9935, 1.0185);
	}

	mg_div_bes->GetXaxis()->SetTitle("Energy (GeV)");
	mg_div_bes->GetYaxis()->SetTitle("Kurtosis");

	mg_div_bes->Draw("AP");

	TLine* one_line_bes = new TLine(0, 1, 80, 1);
	one_line_bes->SetLineStyle(2);
	one_line_bes->Draw();

	leg_bes->SetMargin(0.1); leg_bes->Draw();

	//gPad->SetTopMargin(0.04);
	//gPad->SetLeftMargin(0.12);
	//gPad->SetRightMargin(0.04);

	can_bes->Update();
	can_bes->Write();

	delete can_bes;


	// Just AMPT
	TCanvas* can_ampt = new TCanvas(("AMPT_Rapidity_vs_energy_can_" + type_name).data(), ("Rapidity vs Energy " + type_name).data(), 955, 805);
//	can_ampt->Divide(3, 3, 0.001, 0.001);

	TMultiGraph* mg_div_ampt = new TMultiGraph();
	mg_div_ampt->SetName("Rapidities_divided");

	TLegend* leg_ampt = new TLegend(0.3, 0.21, 0.3, 0.21);
	leg_ampt->SetBorderSize(0);
	leg_ampt->SetFillStyle(0);

	for (pair<float, plot_data> rapid : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][rapid.first].val, energy_err, ampt["divide"][rapid.first].stat);
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + to_string(rapid.first)).data());
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][rapid.first].val, energy_err, ampt["divide"][rapid.first].sys);

		ampt_ratio_div_def_g->SetMarkerStyle(marker_style["ampt"]);
		ampt_ratio_div_def_g->SetMarkerColor(color[rapid.first]);
		ampt_ratio_div_def_g->SetLineColor(color[rapid.first]);
		ampt_ratio_div_def_g->SetMarkerSize(marker_size["ampt"]);
		ampt_ratio_div_sys_g->SetLineColor(color[rapid.first]);

		mg_div_ampt->Add(ampt_ratio_div_def_g, "PL");
		mg_div_ampt->Add(ampt_ratio_div_sys_g, "[]");

		leg_ampt->AddEntry(ampt_ratio_div_def_g, ("AMPT |#eta|<" + to_string(rapid.first).substr(0, 3)).data(), "lp");

//		can_index++;
	}

	mg_div_ampt->GetXaxis()->SetLimits(0, 80);
	mg_div_ampt->GetXaxis()->SetRangeUser(0, 80);
	if (!zoom) {
		mg_div_ampt->GetYaxis()->SetLimits(0.9935, 1.0185);
		mg_div_ampt->GetYaxis()->SetRangeUser(0.9935, 1.0185);
	}

	mg_div_ampt->GetXaxis()->SetTitle("Energy (GeV)");
	mg_div_ampt->GetYaxis()->SetTitle("Kurtosis");

	mg_div_ampt->Draw("AP");

	TLine* one_line_ampt = new TLine(0, 1, 80, 1);
	one_line_ampt->SetLineStyle(2);
	one_line_ampt->Draw();

	leg_ampt->SetMargin(0.1); leg_ampt->Draw();

	//gPad->SetTopMargin(0.04);
	//gPad->SetLeftMargin(0.12);
	//gPad->SetRightMargin(0.04);

	can_ampt->Update();
	can_ampt->Write();

	delete can_ampt;
}


void AzFigurePlotter::kurt_vs_efficiency_plot(vector<int> energies, map<string, map<float, plot_data>> bes1, map<string, map<float, plot_data>> ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	for (double energy : energies) {
		energy_bes1.push_back(energy - 0.6);
		energy_ampt.push_back(energy + 0.6);
	}

	map<float, int> color_raw{ {0.0, kBlue + 4}, {0.05, kBlue + 3}, {0.1, kBlue + 2}, {0.15, kBlue + 1}, {0.2, kBlue}, {0.3, kAzure + 10} };
	map<float, int> color_mix{ {0.0, kGreen + 4}, {0.05, kGreen + 3}, {0.1, kGreen + 2}, {0.15, kGreen + 1}, {0.2, kGreen}, {0.3, kSpring + 10} };
	map<float, int> color_div{ {0.0, kRed + 4}, {0.05, kRed + 3}, {0.1, kRed + 2}, {0.15, kRed + 1}, {0.2, kRed}, {0.3, kPink + 10} };
	map<float, int> marker_style{ {0.0, 20}, {0.05, 20}, {0.1, 20}, {0.15, 20}, {0.2, 20}, {0.3, 20} };
	map<float, int> marker_size{ {0.0, 2}, {0.05, 2}, {0.1, 2}, {0.15, 2}, {0.2, 2}, {0.3, 2} };


	// BES1
	TCanvas* can_bes = new TCanvas(("BES1_Efficiency_vs_energy_can_" + type_name).data(), ("Efficiency vs Energy " + type_name).data(), 955, 805);
	can_bes->SetPad(0.01, 0.01, 0.99, 0.99);
	can_bes->Divide(1, 2, 0, 0);

	can_bes->cd(1);
	TMultiGraph* mg_bes = new TMultiGraph();
	mg_bes->SetName("Efficiencies_Raw_Mixed_BES1");

	TLegend* leg_bes = new TLegend(0.3, 0.21, 0.3, 0.21);
	leg_bes->SetBorderSize(0);
	leg_bes->SetFillStyle(0);

	for (pair<float, plot_data> eff : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
		TGraphErrors* bes1_ratio_raw_def_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][eff.first].val, energy_err, bes1["raw"][eff.first].stat);
		bes1_ratio_raw_def_g->SetNameTitle(("bes1_raw_" + to_string(eff.first).substr(0, 3)).data());
		TGraphErrors* bes1_ratio_raw_sys_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][eff.first].val, energy_err, bes1["raw"][eff.first].sys);

		bes1_ratio_raw_def_g->SetMarkerStyle(marker_style[eff.first]);
		bes1_ratio_raw_def_g->SetMarkerColor(color_raw[eff.first]);
		bes1_ratio_raw_def_g->SetLineColor(color_raw[eff.first]);
		bes1_ratio_raw_def_g->SetMarkerSize(marker_size[eff.first]);
		bes1_ratio_raw_sys_g->SetLineColor(color_raw[eff.first]);

		TGraphErrors* bes1_ratio_mix_def_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][eff.first].val, energy_err, bes1["mix"][eff.first].stat);
		bes1_ratio_mix_def_g->SetNameTitle(("bes1_mix_" + to_string(eff.first).substr(0, 3)).data());
		TGraphErrors* bes1_ratio_mix_sys_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][eff.first].val, energy_err, bes1["mix"][eff.first].sys);

		bes1_ratio_mix_def_g->SetMarkerStyle(marker_style[eff.first]);
		bes1_ratio_mix_def_g->SetMarkerColor(color_mix[eff.first]);
		bes1_ratio_mix_def_g->SetLineColor(color_mix[eff.first]);
		bes1_ratio_mix_def_g->SetMarkerSize(marker_size[eff.first]);
		bes1_ratio_mix_sys_g->SetLineColor(color_mix[eff.first]);

		mg_bes->Add(bes1_ratio_raw_def_g, "APLZ");
		mg_bes->Add(bes1_ratio_raw_sys_g, "[]");
		mg_bes->Add(bes1_ratio_mix_def_g, "PL");
		mg_bes->Add(bes1_ratio_mix_sys_g, "[]");

		leg_bes->AddEntry(bes1_ratio_raw_def_g, ("Raw " + to_string(eff.first*100).substr(0, 3) + "%").data(), "lp");
		leg_bes->AddEntry(bes1_ratio_mix_def_g, ("Mixed " + to_string(eff.first*100).substr(0, 3) + "%").data(), "lp");
	}

	mg_bes->GetXaxis()->SetLimits(0, 80);
	mg_bes->GetXaxis()->SetRangeUser(0, 80);
//	if (!zoom) {
//		mg_bes->GetYaxis()->SetLimits(0.9935, 1.0185);
//		mg_bes->GetYaxis()->SetRangeUser(0.9935, 1.0185);
//	}

//	mg->GetXaxis()->SetTitle("Energy (GeV)");
	mg_bes->GetYaxis()->SetTitle("Kurtosis");

	mg_bes->Draw("AP");

	leg_bes->SetMargin(0.1); leg_bes->Draw();

	//gPad->SetTopMargin(0.04);
	//gPad->SetLeftMargin(0.12);
	//gPad->SetRightMargin(0.04);

	can_bes->cd(2);
	TMultiGraph* mg_div_bes = new TMultiGraph();
	mg_div_bes->SetName("Efficiencies_Divided_BES1");

	TLegend* leg_div_bes = new TLegend(0.3, 0.21, 0.3, 0.21);
	leg_div_bes->SetBorderSize(0);
	leg_div_bes->SetFillStyle(0);

	for (pair<float, plot_data> eff : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][eff.first].val, energy_err, bes1["divide"][eff.first].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + to_string(eff.first)).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][eff.first].val, energy_err, bes1["divide"][eff.first].sys);

		bes1_ratio_div_def_g->SetMarkerStyle(marker_style[eff.first]);
		bes1_ratio_div_def_g->SetMarkerColor(color_div[eff.first]);
		bes1_ratio_div_def_g->SetLineColor(color_div[eff.first]);
		bes1_ratio_div_def_g->SetMarkerSize(marker_size[eff.first]);
		bes1_ratio_div_sys_g->SetLineColor(color_div[eff.first]);

		mg_div_bes->Add(bes1_ratio_div_def_g, "APLZ");
		mg_div_bes->Add(bes1_ratio_div_sys_g, "[]");

		leg_div_bes->AddEntry(bes1_ratio_div_def_g, (to_string(eff.first*100).substr(0, 3) + "%").data(), "lp");
	}

	mg_div_bes->GetXaxis()->SetLimits(0, 80);
	mg_div_bes->GetXaxis()->SetRangeUser(0, 80);
	if (!zoom) {
		mg_div_bes->GetYaxis()->SetLimits(0.9935, 1.0185);
		mg_div_bes->GetYaxis()->SetRangeUser(0.9935, 1.0185);
	}

	mg_div_bes->GetXaxis()->SetTitle("Energy (GeV)");
	mg_div_bes->GetYaxis()->SetTitle("Kurtosis");

	mg_div_bes->Draw("AP");

	TLine* one_line = new TLine(0, 1, 80, 1);
	one_line->SetLineStyle(2);
	one_line->Draw();

	leg_div_bes->SetMargin(0.1); leg_div_bes->Draw();

	//gPad->SetTopMargin(0.04);
	//gPad->SetLeftMargin(0.12);
	//gPad->SetRightMargin(0.04);

	can_bes->Update();
	can_bes->Write();

	delete can_bes;


	// AMPT
	TCanvas* can_ampt = new TCanvas(("AMPT_Efficiency_vs_energy_can_" + type_name).data(), ("Rapidity vs Energy " + type_name).data(), 955, 805);
	can_ampt->SetPad(0.01, 0.01, 0.99, 0.99);
	can_ampt->Divide(1, 2, 0, 0);

	can_ampt->cd(1);
	TMultiGraph* mg_ampt = new TMultiGraph();
	mg_ampt->SetName("Efficiencies_Raw_Mixed_AMPT");

	TLegend* leg_ampt = new TLegend(0.3, 0.21, 0.3, 0.21);
	leg_ampt->SetBorderSize(0);
	leg_ampt->SetFillStyle(0);

	for (pair<float, plot_data> eff : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
		TGraphErrors* ampt_ratio_raw_def_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][eff.first].val, energy_err, ampt["raw"][eff.first].stat);
		ampt_ratio_raw_def_g->SetNameTitle(("ampt_raw_" + to_string(eff.first).substr(0, 3)).data());
		TGraphErrors* ampt_ratio_raw_sys_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][eff.first].val, energy_err, ampt["raw"][eff.first].sys);

		ampt_ratio_raw_def_g->SetMarkerStyle(marker_style[eff.first]);
		ampt_ratio_raw_def_g->SetMarkerColor(color_raw[eff.first]);
		ampt_ratio_raw_def_g->SetLineColor(color_raw[eff.first]);
		ampt_ratio_raw_def_g->SetMarkerSize(marker_size[eff.first]);
		ampt_ratio_raw_sys_g->SetLineColor(color_raw[eff.first]);

		TGraphErrors* ampt_ratio_mix_def_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][eff.first].val, energy_err, ampt["mix"][eff.first].stat);
		ampt_ratio_mix_def_g->SetNameTitle(("ampt_mix_" + to_string(eff.first).substr(0, 3)).data());
		TGraphErrors* ampt_ratio_mix_sys_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][eff.first].val, energy_err, ampt["mix"][eff.first].sys);

		ampt_ratio_mix_def_g->SetMarkerStyle(marker_style[eff.first]);
		ampt_ratio_mix_def_g->SetMarkerColor(color_mix[eff.first]);
		ampt_ratio_mix_def_g->SetLineColor(color_mix[eff.first]);
		ampt_ratio_mix_def_g->SetMarkerSize(marker_size[eff.first]);
		ampt_ratio_mix_sys_g->SetLineColor(color_mix[eff.first]);

		mg_ampt->Add(ampt_ratio_raw_def_g, "APLZ");
		mg_ampt->Add(ampt_ratio_raw_sys_g, "[]");
		mg_ampt->Add(ampt_ratio_mix_def_g, "PL");
		mg_ampt->Add(ampt_ratio_mix_sys_g, "[]");

		leg_ampt->AddEntry(ampt_ratio_raw_def_g, ("Raw " + to_string(eff.first*100).substr(0, 3) + "%").data(), "lp");
		leg_ampt->AddEntry(ampt_ratio_mix_def_g, ("Mixed " + to_string(eff.first*100).substr(0, 3) + "%").data(), "lp");
	}

	mg_ampt->GetXaxis()->SetLimits(0, 80);
	mg_ampt->GetXaxis()->SetRangeUser(0, 80);
//	if (!zoom) {
//		mg_ampt->GetYaxis()->SetLimits(0.9935, 1.0185);
//		mg_ampt->GetYaxis()->SetRangeUser(0.9935, 1.0185);
//	}

//	mg->GetXaxis()->SetTitle("Energy (GeV)");
	mg_ampt->GetYaxis()->SetTitle("Kurtosis");

	mg_ampt->Draw("AP");

	leg_ampt->SetMargin(0.1); leg_ampt->Draw();

	//gPad->SetTopMargin(0.04);
	//gPad->SetLeftMargin(0.12);
	//gPad->SetRightMargin(0.04);

	can_ampt->cd(2);
	TMultiGraph* mg_div_ampt = new TMultiGraph();
	mg_div_ampt->SetName("Efficiencies_Raw_Mixed");

	TLegend* leg_div_ampt = new TLegend(0.3, 0.21, 0.3, 0.21);
	leg_div_ampt->SetBorderSize(0);
	leg_div_ampt->SetFillStyle(0);

	for (pair<float, plot_data> eff : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][eff.first].val, energy_err, ampt["divide"][eff.first].stat);
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + to_string(eff.first)).data());
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][eff.first].val, energy_err, ampt["divide"][eff.first].sys);

		ampt_ratio_div_def_g->SetMarkerStyle(marker_style[eff.first]);
		ampt_ratio_div_def_g->SetMarkerColor(color_div[eff.first]);
		ampt_ratio_div_def_g->SetLineColor(color_div[eff.first]);
		ampt_ratio_div_def_g->SetMarkerSize(marker_size[eff.first]);
		ampt_ratio_div_sys_g->SetLineColor(color_div[eff.first]);

		mg_div_ampt->Add(ampt_ratio_div_def_g, "APLZ");
		mg_div_ampt->Add(ampt_ratio_div_sys_g, "[]");

		leg_div_ampt->AddEntry(ampt_ratio_div_def_g, (to_string(eff.first*100).substr(0, 3) + "%").data(), "lp");
	}

	mg_div_ampt->GetXaxis()->SetLimits(0, 80);
	mg_div_ampt->GetXaxis()->SetRangeUser(0, 80);
//	if (!zoom) {
//		mg_div_ampt->GetYaxis()->SetLimits(0.9935, 1.0185);
//		mg_div_ampt->GetYaxis()->SetRangeUser(0.9935, 1.0185);
//	}

	mg_div_ampt->GetXaxis()->SetTitle("Energy (GeV)");
	mg_div_ampt->GetYaxis()->SetTitle("Kurtosis");

	mg_div_ampt->Draw("AP");

	TLine* one_line_ampt = new TLine(0, 1, 80, 1);
	one_line_ampt->SetLineStyle(2);
	one_line_ampt->Draw();

	leg_div_ampt->SetMargin(0.1); leg_div_ampt->Draw();

	//gPad->SetTopMargin(0.04);
	//gPad->SetLeftMargin(0.12);
	//gPad->SetRightMargin(0.04);

	can_ampt->Update();
	can_ampt->Write();

	delete can_ampt;
}
