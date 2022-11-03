
#include <vector>

#include "AzFigurePlotter.h"
#include "plotting.h"

#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TText.h>
#include <TLatex.h>

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
	cout << "Making paper figures" << endl;
	TFile fig_file((out_path+"figures_"+out_root_name).data(), "RECREATE");
	fig_file.cd();
//	kurt_vs_energy();
	kurt_vs_divs_w_energy();
	kurt_vs_energy_cents();
	moments_vs_energy();
	moments_vs_energy_cf();
	kurt_vs_energy_w_rapidity();
	kurt_vs_rapidity_w_energy();
	kurt_with_efficiency();
	kurt_vs_efficiency();
	sd_vs_energy_cents();
	fig_file.Close();
	cout << "Wrote " << out_path+"figures_"+out_root_name << endl;
}


void AzFigurePlotter::plot_pres_figs() {
	cout << "Making presentation figure" << endl;
	TFile fig_file((out_path+"figures_"+out_root_name).data(), "RECREATE");
	fig_file.cd();
	kurt_vs_energy_pres();
	kurt_vs_divs_w_energy_pres();
	fig_file.Close();
	cout << "Wrote " << out_path+"figures_"+out_root_name << endl;
}


void AzFigurePlotter::kurt_vs_energy_pres() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	int div = 120;
	int cent = 8;
	string stat = "non_excess_kurtosis";

	map<string, plot_data> bes1_ratio;
	map<string, plot_data> bes1_pull;
	map<string, plot_data> ampt_ratio;
	map<string, plot_data> ampt_pull;
	map<string, plot_data> cf_ratio;
	map<string, plot_data> cf_pull;

	for (int energy : energies) {
		bes1_ratio["raw"].val.push_back(def_medians["BES1"]["raw"][energy][div][cent][stat].get_val());
		bes1_ratio["raw"].stat.push_back(def_medians["BES1"]["raw"][energy][div][cent][stat].get_err());
		bes1_ratio["raw"].sys.push_back(def_systematics["BES1"]["raw"][energy][div][cent][stat]);
		bes1_pull["raw"].val.push_back(def_medians["BES1"]["pull_raw"][energy][div][cent][stat].get_val());
		bes1_pull["raw"].stat.push_back(def_medians["BES1"]["pull_raw"][energy][div][cent][stat].get_err());
		bes1_pull["raw"].sys.push_back(def_systematics["BES1"]["pull_raw"][energy][div][cent][stat]);
		ampt_ratio["raw"].val.push_back(def_medians["AMPT"]["raw"][energy][div][cent][stat].get_val());
		ampt_ratio["raw"].stat.push_back(def_medians["AMPT"]["raw"][energy][div][cent][stat].get_err());
		ampt_ratio["raw"].sys.push_back(def_systematics["AMPT"]["raw"][energy][div][cent][stat]);
		ampt_pull["raw"].val.push_back(def_medians["AMPT"]["pull_raw"][energy][div][cent][stat].get_val());
		ampt_pull["raw"].stat.push_back(def_medians["AMPT"]["pull_raw"][energy][div][cent][stat].get_err());
		ampt_pull["raw"].sys.push_back(def_systematics["AMPT"]["pull_raw"][energy][div][cent][stat]);
		cf_ratio["raw"].val.push_back(def_medians["CF"]["raw"][energy][div][cent][stat].get_val());
		cf_ratio["raw"].stat.push_back(def_medians["CF"]["raw"][energy][div][cent][stat].get_err());
		cf_ratio["raw"].sys.push_back(def_systematics["CF"]["raw"][energy][div][cent][stat]);
		cf_pull["raw"].val.push_back(def_medians["CF"]["pull_raw"][energy][div][cent][stat].get_val());
		cf_pull["raw"].stat.push_back(def_medians["CF"]["pull_raw"][energy][div][cent][stat].get_err());
		cf_pull["raw"].sys.push_back(def_systematics["CF"]["pull_raw"][energy][div][cent][stat]);

		bes1_ratio["mix"].val.push_back(def_medians["BES1"]["mix"][energy][div][cent][stat].get_val());
		bes1_ratio["mix"].stat.push_back(def_medians["BES1"]["mix"][energy][div][cent][stat].get_err());
		bes1_ratio["mix"].sys.push_back(def_systematics["BES1"]["mix"][energy][div][cent][stat]);
		bes1_pull["mix"].val.push_back(def_medians["BES1"]["pull_mix"][energy][div][cent][stat].get_val());
		bes1_pull["mix"].stat.push_back(def_medians["BES1"]["pull_mix"][energy][div][cent][stat].get_err());
		bes1_pull["mix"].sys.push_back(def_systematics["BES1"]["pull_mix"][energy][div][cent][stat]);
		ampt_ratio["mix"].val.push_back(def_medians["AMPT"]["mix"][energy][div][cent][stat].get_val());
		ampt_ratio["mix"].stat.push_back(def_medians["AMPT"]["mix"][energy][div][cent][stat].get_err());
		ampt_ratio["mix"].sys.push_back(def_systematics["AMPT"]["mix"][energy][div][cent][stat]);
		ampt_pull["mix"].val.push_back(def_medians["AMPT"]["pull_mix"][energy][div][cent][stat].get_val());
		ampt_pull["mix"].stat.push_back(def_medians["AMPT"]["pull_mix"][energy][div][cent][stat].get_err());
		ampt_pull["mix"].sys.push_back(def_systematics["AMPT"]["pull_mix"][energy][div][cent][stat]);
		cf_ratio["mix"].val.push_back(def_medians["CF"]["mix"][energy][div][cent][stat].get_val());
		cf_ratio["mix"].stat.push_back(def_medians["CF"]["mix"][energy][div][cent][stat].get_err());
		cf_ratio["mix"].sys.push_back(def_systematics["CF"]["mix"][energy][div][cent][stat]);
		cf_pull["mix"].val.push_back(def_medians["CF"]["pull_mix"][energy][div][cent][stat].get_val());
		cf_pull["mix"].stat.push_back(def_medians["CF"]["pull_mix"][energy][div][cent][stat].get_err());
		cf_pull["mix"].sys.push_back(def_systematics["CF"]["pull_mix"][energy][div][cent][stat]);

		bes1_ratio["divide"].val.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_val());
		bes1_ratio["divide"].stat.push_back(def_medians["BES1"]["divide"][energy][div][cent][stat].get_err());
		bes1_ratio["divide"].sys.push_back(def_systematics["BES1"]["divide"][energy][div][cent][stat]);
		bes1_pull["divide"].val.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_val());
		bes1_pull["divide"].stat.push_back(def_medians["BES1"]["pull_divide"][energy][div][cent][stat].get_err());
		bes1_pull["divide"].sys.push_back(def_systematics["BES1"]["pull_divide"][energy][div][cent][stat]);
		ampt_ratio["divide"].val.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_val());
		ampt_ratio["divide"].stat.push_back(def_medians["AMPT"]["divide"][energy][div][cent][stat].get_err());
		ampt_ratio["divide"].sys.push_back(def_systematics["AMPT"]["divide"][energy][div][cent][stat]);
		ampt_pull["divide"].val.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_val());
		ampt_pull["divide"].stat.push_back(def_medians["AMPT"]["pull_divide"][energy][div][cent][stat].get_err());
		ampt_pull["divide"].sys.push_back(def_systematics["AMPT"]["pull_divide"][energy][div][cent][stat]);
		cf_ratio["divide"].val.push_back(def_medians["CF"]["divide"][energy][div][cent][stat].get_val());
		cf_ratio["divide"].stat.push_back(def_medians["CF"]["divide"][energy][div][cent][stat].get_err());
		cf_ratio["divide"].sys.push_back(def_systematics["CF"]["divide"][energy][div][cent][stat]);
		cf_pull["divide"].val.push_back(def_medians["CF"]["pull_divide"][energy][div][cent][stat].get_val());
		cf_pull["divide"].stat.push_back(def_medians["CF"]["pull_divide"][energy][div][cent][stat].get_err());
		cf_pull["divide"].sys.push_back(def_systematics["CF"]["pull_divide"][energy][div][cent][stat]);
	}

	kurt_vs_energy_pres_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	kurt_vs_energy_pres_plot(energies, bes1_pull, ampt_pull, "Pull");

	kurt_vs_energy_cf_pres_plot(energies, bes1_ratio, ampt_ratio, cf_ratio, "Ratio");
	kurt_vs_energy_cf_pres_plot(energies, bes1_pull, ampt_pull, cf_pull, "Pull");
}


void AzFigurePlotter::kurt_vs_divs_w_energy_pres() {
	map<int, int> energies{ {7, 7.7}, {11, 11.5}, {19, 19.6}, {27, 27}, {39, 39}, {62, 62.4} };
//	map<int, string> energies{ {7, "7.7"}, {11, "11.5"}, {19, "19.6"}, {27, "27"}, {39, "39"}, {62, "62.4"} };
	vector<int> divs{ 180, 120, 240, 90, 270, 60, 300 };
	int cent = 8;
	string stat = "non_excess_kurtosis";

	map<int, plot_data> bes1_ratio;
	map<int, plot_data> bes1_pull;
	map<int, plot_data> ampt_ratio;
	map<int, plot_data> ampt_pull;
	map<int, plot_data> cf_ratio;
	map<int, plot_data> cf_pull;

	for (pair<int, int> energy : energies) {
		for (int div : divs) {
			bes1_ratio[div].val.push_back(def_medians["BES1"]["divide"][energy.first][div][cent][stat].get_val());
			bes1_ratio[div].stat.push_back(def_medians["BES1"]["divide"][energy.first][div][cent][stat].get_err());
			bes1_ratio[div].sys.push_back(def_systematics["BES1"]["divide"][energy.first][div][cent][stat]);
			bes1_pull[div].val.push_back(def_medians["BES1"]["pull_divide"][energy.first][div][cent][stat].get_val());
			bes1_pull[div].stat.push_back(def_medians["BES1"]["pull_divide"][energy.first][div][cent][stat].get_err());
			bes1_pull[div].sys.push_back(def_systematics["BES1"]["pull_divide"][energy.first][div][cent][stat]);

			ampt_ratio[div].val.push_back(def_medians["AMPT"]["divide"][energy.first][div][cent][stat].get_val());
			ampt_ratio[div].stat.push_back(def_medians["AMPT"]["divide"][energy.first][div][cent][stat].get_err());
			ampt_ratio[div].sys.push_back(def_systematics["AMPT"]["divide"][energy.first][div][cent][stat]);
			ampt_pull[div].val.push_back(def_medians["AMPT"]["pull_divide"][energy.first][div][cent][stat].get_val());
			ampt_pull[div].stat.push_back(def_medians["AMPT"]["pull_divide"][energy.first][div][cent][stat].get_err());
			ampt_pull[div].sys.push_back(def_systematics["AMPT"]["pull_divide"][energy.first][div][cent][stat]);

			cf_ratio[div].val.push_back(def_medians["CF"]["divide"][energy.first][div][cent][stat].get_val());
			cf_ratio[div].stat.push_back(def_medians["CF"]["divide"][energy.first][div][cent][stat].get_err());
			cf_ratio[div].sys.push_back(def_systematics["CF"]["divide"][energy.first][div][cent][stat]);
			cf_pull[div].val.push_back(def_medians["CF"]["pull_divide"][energy.first][div][cent][stat].get_val());
			cf_pull[div].stat.push_back(def_medians["CF"]["pull_divide"][energy.first][div][cent][stat].get_err());
			cf_pull[div].sys.push_back(def_systematics["CF"]["pull_divide"][energy.first][div][cent][stat]);
		}
	}

	kurt_vs_divs_w_energy_pres_plot(energies, divs, bes1_ratio, ampt_ratio, "Ratio");
	kurt_vs_divs_w_energy_pres_plot(energies, divs, bes1_pull, ampt_pull, "Pull");
}


void AzFigurePlotter::kurt_vs_divs_w_energy() {
	map<int, string> energies{ {7, "7.7"}, {11, "11.5"}, {19, "19.6"}, {27, "27"}, {39, "39"}, {62, "62.4"} };
	vector<int> divs{ 60, 90, 120, 180, 240, 270, 300 };
	int cent = 8;
	string stat = "non_excess_kurtosis";

	map<string, map<int, plot_data>> bes1_ratio;
	map<string, map<int, plot_data>> bes1_pull;
	map<string, map<int, plot_data>> ampt_ratio;
	map<string, map<int, plot_data>> ampt_pull;
	map<string, map<int, plot_data>> cf_ratio;
	map<string, map<int, plot_data>> cf_pull;

	for (int div : divs) {
		for (pair<int, string> energy : energies) {
			bes1_ratio["divide"][energy.first].val.push_back(def_medians["BES1"]["divide"][energy.first][div][cent][stat].get_val());
			bes1_ratio["divide"][energy.first].stat.push_back(def_medians["BES1"]["divide"][energy.first][div][cent][stat].get_err());
			bes1_ratio["divide"][energy.first].sys.push_back(def_systematics["BES1"]["divide"][energy.first][div][cent][stat]);
			bes1_pull["divide"][energy.first].val.push_back(def_medians["BES1"]["pull_divide"][energy.first][div][cent][stat].get_val());
			bes1_pull["divide"][energy.first].stat.push_back(def_medians["BES1"]["pull_divide"][energy.first][div][cent][stat].get_err());
			bes1_pull["divide"][energy.first].sys.push_back(def_systematics["BES1"]["pull_divide"][energy.first][div][cent][stat]);

			ampt_ratio["divide"][energy.first].val.push_back(def_medians["AMPT"]["divide"][energy.first][div][cent][stat].get_val());
			ampt_ratio["divide"][energy.first].stat.push_back(def_medians["AMPT"]["divide"][energy.first][div][cent][stat].get_err());
			ampt_ratio["divide"][energy.first].sys.push_back(def_systematics["AMPT"]["divide"][energy.first][div][cent][stat]);
			ampt_pull["divide"][energy.first].val.push_back(def_medians["AMPT"]["pull_divide"][energy.first][div][cent][stat].get_val());
			ampt_pull["divide"][energy.first].stat.push_back(def_medians["AMPT"]["pull_divide"][energy.first][div][cent][stat].get_err());
			ampt_pull["divide"][energy.first].sys.push_back(def_systematics["AMPT"]["pull_divide"][energy.first][div][cent][stat]);

			cf_ratio["divide"][energy.first].val.push_back(def_medians["CF"]["divide"][energy.first][div][cent][stat].get_val());
			cf_ratio["divide"][energy.first].stat.push_back(def_medians["CF"]["divide"][energy.first][div][cent][stat].get_err());
			cf_ratio["divide"][energy.first].sys.push_back(def_systematics["CF"]["divide"][energy.first][div][cent][stat]);
			cf_pull["divide"][energy.first].val.push_back(def_medians["CF"]["pull_divide"][energy.first][div][cent][stat].get_val());
			cf_pull["divide"][energy.first].stat.push_back(def_medians["CF"]["pull_divide"][energy.first][div][cent][stat].get_err());
			cf_pull["divide"][energy.first].sys.push_back(def_systematics["CF"]["pull_divide"][energy.first][div][cent][stat]);
		}
	}

	kurt_vs_divs_w_energy_plot(energies, divs, bes1_ratio, ampt_ratio, cf_ratio, "Ratio");
	kurt_vs_divs_w_energy_plot(energies, divs, bes1_pull, ampt_pull, cf_pull, "Pull");
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


void AzFigurePlotter::sd_vs_energy_cents() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	int div = 120;
	vector<int> cents{ 8, 7, 6, 5, 4, 3, 2, 1 };
	string stat = "standard_deviation";

	map<int, plot_data> bes1_ratio;
	map<int, plot_data> bes1_pull;
	map<int, plot_data> ampt_ratio;
	map<int, plot_data> ampt_pull;
	map<int, plot_data> cf_ratio;
	map<int, plot_data> cf_pull;

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
			// Only centrality 8 for cf_ratio, remove from cent loop if there's a problem with grabbing non-existing centralities
			cf_ratio[cent].val.push_back(def_medians["CF"]["divide"][energy][div][cent][stat].get_val());
			cf_ratio[cent].stat.push_back(def_medians["CF"]["divide"][energy][div][cent][stat].get_err());
			cf_ratio[cent].sys.push_back(def_systematics["CF"]["divide"][energy][div][cent][stat]);
			cf_pull[cent].val.push_back(def_medians["CF"]["pull_divide"][energy][div][cent][stat].get_val());
			cf_pull[cent].stat.push_back(def_medians["CF"]["pull_divide"][energy][div][cent][stat].get_err());
			cf_pull[cent].sys.push_back(def_systematics["CF"]["pull_divide"][energy][div][cent][stat]);
		}
	}

	sd_vs_energy_cents_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	sd_vs_energy_cents_plot(energies, bes1_pull, ampt_pull, "Pull");

	sd_vs_energy_cf_plot(energies, bes1_ratio[8], ampt_ratio[8], cf_ratio[8], "Ratio");
	sd_vs_energy_cf_plot(energies, bes1_pull[8], ampt_pull[8], cf_pull[8], "Pull");
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


void AzFigurePlotter::moments_vs_energy_cf() {
	vector<int> energies{ 7, 11, 19, 27, 39, 62 };
	int div = 120;
	int cent = 8;
	vector<string> stats { "mean", "standard_deviation", "skewness", "non_excess_kurtosis" };

	map<string, map<string, plot_data>> bes1_ratio;
	map<string, map<string, plot_data>> bes1_pull;
	map<string, map<string, plot_data>> ampt_ratio;
	map<string, map<string, plot_data>> ampt_pull;
	map<string, map<string, plot_data>> cf_ratio;
	map<string, map<string, plot_data>> cf_pull;

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
			cf_ratio["divide"][stat].val.push_back(def_medians["CF"]["divide"][energy][div][cent][stat].get_val());
			cf_ratio["divide"][stat].stat.push_back(def_medians["CF"]["divide"][energy][div][cent][stat].get_err());
			cf_ratio["divide"][stat].sys.push_back(def_systematics["CF"]["divide"][energy][div][cent][stat]);
			cf_pull["divide"][stat].val.push_back(def_medians["CF"]["pull_divide"][energy][div][cent][stat].get_val());
			cf_pull["divide"][stat].stat.push_back(def_medians["CF"]["pull_divide"][energy][div][cent][stat].get_err());
			cf_pull["divide"][stat].sys.push_back(def_systematics["CF"]["pull_divide"][energy][div][cent][stat]);

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
			cf_ratio["raw"][stat].val.push_back(def_medians["CF"]["raw"][energy][div][cent][stat].get_val());
			cf_ratio["raw"][stat].stat.push_back(def_medians["CF"]["raw"][energy][div][cent][stat].get_err());
			cf_ratio["raw"][stat].sys.push_back(def_systematics["CF"]["raw"][energy][div][cent][stat]);
			cf_pull["raw"][stat].val.push_back(def_medians["CF"]["pull_raw"][energy][div][cent][stat].get_val());
			cf_pull["raw"][stat].stat.push_back(def_medians["CF"]["pull_raw"][energy][div][cent][stat].get_err());
			cf_pull["raw"][stat].sys.push_back(def_systematics["CF"]["pull_raw"][energy][div][cent][stat]);

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
			cf_ratio["mix"][stat].val.push_back(def_medians["CF"]["mix"][energy][div][cent][stat].get_val());
			cf_ratio["mix"][stat].stat.push_back(def_medians["CF"]["mix"][energy][div][cent][stat].get_err());
			cf_ratio["mix"][stat].sys.push_back(def_systematics["CF"]["mix"][energy][div][cent][stat]);
			cf_pull["mix"][stat].val.push_back(def_medians["CF"]["pull_mix"][energy][div][cent][stat].get_val());
			cf_pull["mix"][stat].stat.push_back(def_medians["CF"]["pull_mix"][energy][div][cent][stat].get_err());
			cf_pull["mix"][stat].sys.push_back(def_systematics["CF"]["pull_mix"][energy][div][cent][stat]);
		}
	}

	moments_vs_energy_cf_plot(energies, bes1_ratio, ampt_ratio, cf_ratio, stats, "Ratio");
	moments_vs_energy_cf_plot(energies, bes1_pull, ampt_pull, cf_pull, stats, "Pull");
}


void AzFigurePlotter::kurt_vs_energy_w_rapidity() {
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

	kurt_vs_energy_w_rapid_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	kurt_vs_energy_w_rapid_plot(energies, bes1_pull, ampt_pull, "Pull");
}


void AzFigurePlotter::kurt_vs_rapidity_w_energy() {
	map<int, string> energies{ {7, "7.7"}, {11, "11.5"}, {19, "19.6"}, {27, "27"}, {39, "39"}, {62, "62.4"} };
	int div = 120;
	int cent = 8;
	//	vector<float> rapidities{ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 1.0 };
	map<float, string> rapidities{ {0.1, "01"}, {0.2, "02"}, {0.3, "03"}, {0.4, "04"}, {0.5, "default"}, {0.6, "06"}, {0.8, "08"}, {1.0, "1"} };
	string stat = "non_excess_kurtosis";

	map<string, map<int, plot_data>> bes1_ratio;
	map<string, map<int, plot_data>> bes1_pull;
	map<string, map<int, plot_data>> ampt_ratio;
	map<string, map<int, plot_data>> ampt_pull;
	map<string, map<int, plot_data>> cf_ratio;
	map<string, map<int, plot_data>> cf_pull;

	vector<float> rapid_vals;

	for (pair<float, string> rapid : rapidities) {
		rapid_vals.push_back(rapid.first);
		string bes = "BES1_rapid" + rapid.second;
		string ampt = "AMPT_rapid" + rapid.second;
		string cf = "CF_rapid" + rapid.second;
		if (rapid.second == "default") {
			bes = "BES1";
			ampt = "AMPT";
			cf = "CF";
		}
		for (pair<int, string> energy : energies) {
			bes1_ratio["divide"][energy.first].val.push_back(def_medians[bes]["divide"][energy.first][div][cent][stat].get_val());
			bes1_ratio["divide"][energy.first].stat.push_back(def_medians[bes]["divide"][energy.first][div][cent][stat].get_err());
			bes1_ratio["divide"][energy.first].sys.push_back(def_systematics[bes]["divide"][energy.first][div][cent][stat]);
			bes1_pull["divide"][energy.first].val.push_back(def_medians[bes]["pull_divide"][energy.first][div][cent][stat].get_val());
			bes1_pull["divide"][energy.first].stat.push_back(def_medians[bes]["pull_divide"][energy.first][div][cent][stat].get_err());
			bes1_pull["divide"][energy.first].sys.push_back(def_systematics[bes]["pull_divide"][energy.first][div][cent][stat]);
			ampt_ratio["divide"][energy.first].val.push_back(def_medians[ampt]["divide"][energy.first][div][cent][stat].get_val());
			ampt_ratio["divide"][energy.first].stat.push_back(def_medians[ampt]["divide"][energy.first][div][cent][stat].get_err());
			ampt_ratio["divide"][energy.first].sys.push_back(def_systematics[ampt]["divide"][energy.first][div][cent][stat]);
			ampt_pull["divide"][energy.first].val.push_back(def_medians[ampt]["pull_divide"][energy.first][div][cent][stat].get_val());
			ampt_pull["divide"][energy.first].stat.push_back(def_medians[ampt]["pull_divide"][energy.first][div][cent][stat].get_err());
			ampt_pull["divide"][energy.first].sys.push_back(def_systematics[ampt]["pull_divide"][energy.first][div][cent][stat]);
			cf_ratio["divide"][energy.first].val.push_back(def_medians[cf]["divide"][energy.first][div][cent][stat].get_val());
			cf_ratio["divide"][energy.first].stat.push_back(def_medians[cf]["divide"][energy.first][div][cent][stat].get_err());
			cf_ratio["divide"][energy.first].sys.push_back(def_systematics[cf]["divide"][energy.first][div][cent][stat]);
			cf_pull["divide"][energy.first].val.push_back(def_medians[cf]["pull_divide"][energy.first][div][cent][stat].get_val());
			cf_pull["divide"][energy.first].stat.push_back(def_medians[cf]["pull_divide"][energy.first][div][cent][stat].get_err());
			cf_pull["divide"][energy.first].sys.push_back(def_systematics[cf]["pull_divide"][energy.first][div][cent][stat]);

			bes1_ratio["raw"][energy.first].val.push_back(def_medians[bes]["raw"][energy.first][div][cent][stat].get_val());
			bes1_ratio["raw"][energy.first].stat.push_back(def_medians[bes]["raw"][energy.first][div][cent][stat].get_err());
			bes1_ratio["raw"][energy.first].sys.push_back(def_systematics[bes]["raw"][energy.first][div][cent][stat]);
			bes1_pull["raw"][energy.first].val.push_back(def_medians[bes]["pull_raw"][energy.first][div][cent][stat].get_val());
			bes1_pull["raw"][energy.first].stat.push_back(def_medians[bes]["pull_raw"][energy.first][div][cent][stat].get_err());
			bes1_pull["raw"][energy.first].sys.push_back(def_systematics[bes]["pull_raw"][energy.first][div][cent][stat]);
			ampt_ratio["raw"][energy.first].val.push_back(def_medians[ampt]["raw"][energy.first][div][cent][stat].get_val());
			ampt_ratio["raw"][energy.first].stat.push_back(def_medians[ampt]["raw"][energy.first][div][cent][stat].get_err());
			ampt_ratio["raw"][energy.first].sys.push_back(def_systematics[ampt]["raw"][energy.first][div][cent][stat]);
			ampt_pull["raw"][energy.first].val.push_back(def_medians[ampt]["pull_raw"][energy.first][div][cent][stat].get_val());
			ampt_pull["raw"][energy.first].stat.push_back(def_medians[ampt]["pull_raw"][energy.first][div][cent][stat].get_err());
			ampt_pull["raw"][energy.first].sys.push_back(def_systematics[ampt]["pull_raw"][energy.first][div][cent][stat]);
			cf_ratio["raw"][energy.first].val.push_back(def_medians[cf]["raw"][energy.first][div][cent][stat].get_val());
			cf_ratio["raw"][energy.first].stat.push_back(def_medians[cf]["raw"][energy.first][div][cent][stat].get_err());
			cf_ratio["raw"][energy.first].sys.push_back(def_systematics[cf]["raw"][energy.first][div][cent][stat]);
			cf_pull["raw"][energy.first].val.push_back(def_medians[cf]["pull_raw"][energy.first][div][cent][stat].get_val());
			cf_pull["raw"][energy.first].stat.push_back(def_medians[cf]["pull_raw"][energy.first][div][cent][stat].get_err());
			cf_pull["raw"][energy.first].sys.push_back(def_systematics[cf]["pull_raw"][energy.first][div][cent][stat]);

			bes1_ratio["mix"][energy.first].val.push_back(def_medians[bes]["mix"][energy.first][div][cent][stat].get_val());
			bes1_ratio["mix"][energy.first].stat.push_back(def_medians[bes]["mix"][energy.first][div][cent][stat].get_err());
			bes1_ratio["mix"][energy.first].sys.push_back(def_systematics[bes]["mix"][energy.first][div][cent][stat]);
			bes1_pull["mix"][energy.first].val.push_back(def_medians[bes]["pull_mix"][energy.first][div][cent][stat].get_val());
			bes1_pull["mix"][energy.first].stat.push_back(def_medians[bes]["pull_mix"][energy.first][div][cent][stat].get_err());
			bes1_pull["mix"][energy.first].sys.push_back(def_systematics[bes]["pull_mix"][energy.first][div][cent][stat]);
			ampt_ratio["mix"][energy.first].val.push_back(def_medians[ampt]["mix"][energy.first][div][cent][stat].get_val());
			ampt_ratio["mix"][energy.first].stat.push_back(def_medians[ampt]["mix"][energy.first][div][cent][stat].get_err());
			ampt_ratio["mix"][energy.first].sys.push_back(def_systematics[ampt]["mix"][energy.first][div][cent][stat]);
			ampt_pull["mix"][energy.first].val.push_back(def_medians[ampt]["pull_mix"][energy.first][div][cent][stat].get_val());
			ampt_pull["mix"][energy.first].stat.push_back(def_medians[ampt]["pull_mix"][energy.first][div][cent][stat].get_err());
			ampt_pull["mix"][energy.first].sys.push_back(def_systematics[ampt]["pull_mix"][energy.first][div][cent][stat]);
			cf_ratio["mix"][energy.first].val.push_back(def_medians[cf]["mix"][energy.first][div][cent][stat].get_val());
			cf_ratio["mix"][energy.first].stat.push_back(def_medians[cf]["mix"][energy.first][div][cent][stat].get_err());
			cf_ratio["mix"][energy.first].sys.push_back(def_systematics[cf]["mix"][energy.first][div][cent][stat]);
			cf_pull["mix"][energy.first].val.push_back(def_medians[cf]["pull_mix"][energy.first][div][cent][stat].get_val());
			cf_pull["mix"][energy.first].stat.push_back(def_medians[cf]["pull_mix"][energy.first][div][cent][stat].get_err());
			cf_pull["mix"][energy.first].sys.push_back(def_systematics[cf]["pull_mix"][energy.first][div][cent][stat]);
		}
	}

	kurt_vs_rapid_w_energy_plot(energies, rapid_vals, bes1_ratio, ampt_ratio, "Ratio");
	kurt_vs_rapid_w_energy_plot(energies, rapid_vals, bes1_pull, ampt_pull, "Pull");

	kurt_vs_rapid_w_energy_cf_plot(energies, rapid_vals, bes1_ratio, ampt_ratio, cf_ratio, "Ratio");
	kurt_vs_rapid_w_energy_cf_plot(energies, rapid_vals, bes1_pull, ampt_pull, cf_pull, "Pull");
}


void AzFigurePlotter::kurt_with_efficiency() {
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

	kurt_with_efficiency_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	kurt_with_efficiency_plot(energies, bes1_pull, ampt_pull, "Pull");
}



void AzFigurePlotter::kurt_vs_efficiency() {
	map<int, string> energies{ {7, "7.7"}, {11, "11.5"}, {19, "19.6"}, {27, "27"}, {39, "39"}, {62, "62.4"} };
	int div = 120;
	int cent = 8;
	map<float, string> efficiencies{ {0.0, "default"}, {0.05, "05"}, {0.1, "1"}, {0.15, "15"}, {0.2, "2"}, {0.3, "3"} };
	vector<float> effs;
	string stat = "non_excess_kurtosis";

	map<string, map<int, plot_data>> bes1_ratio;
	map<string, map<int, plot_data>> ampt_ratio;
	map<string, map<int, plot_data>> cf_ratio;
	map<string, map<int, plot_data>> bes1_pull;
	map<string, map<int, plot_data>> ampt_pull;
	map<string, map<int, plot_data>> cf_pull;

	for (pair<float, string> eff : efficiencies) {
		effs.push_back(eff.first);
		string bes = "BES1_Eff" + eff.second;
		string ampt = "AMPT_Eff" + eff.second;
		string cf = "CF_Eff" + eff.second;
		if (eff.second == "default") {
			bes = "BES1";
			ampt = "AMPT";
			cf = "CF";
		}
		for (pair<int, string> energy : energies) {
			bes1_ratio["divide"][energy.first].val.push_back(def_medians[bes]["divide"][energy.first][div][cent][stat].get_val());
			bes1_ratio["divide"][energy.first].stat.push_back(def_medians[bes]["divide"][energy.first][div][cent][stat].get_err());
			bes1_ratio["divide"][energy.first].sys.push_back(def_systematics[bes]["divide"][energy.first][div][cent][stat]);
			bes1_pull["divide"][energy.first].val.push_back(def_medians[bes]["pull_divide"][energy.first][div][cent][stat].get_val());
			bes1_pull["divide"][energy.first].stat.push_back(def_medians[bes]["pull_divide"][energy.first][div][cent][stat].get_err());
			bes1_pull["divide"][energy.first].sys.push_back(def_systematics[bes]["pull_divide"][energy.first][div][cent][stat]);
			ampt_ratio["divide"][energy.first].val.push_back(def_medians[ampt]["divide"][energy.first][div][cent][stat].get_val());
			ampt_ratio["divide"][energy.first].stat.push_back(def_medians[ampt]["divide"][energy.first][div][cent][stat].get_err());
			ampt_ratio["divide"][energy.first].sys.push_back(def_systematics[ampt]["divide"][energy.first][div][cent][stat]);
			ampt_pull["divide"][energy.first].val.push_back(def_medians[ampt]["pull_divide"][energy.first][div][cent][stat].get_val());
			ampt_pull["divide"][energy.first].stat.push_back(def_medians[ampt]["pull_divide"][energy.first][div][cent][stat].get_err());
			ampt_pull["divide"][energy.first].sys.push_back(def_systematics[ampt]["pull_divide"][energy.first][div][cent][stat]);
			cf_ratio["divide"][energy.first].val.push_back(def_medians[cf]["divide"][energy.first][div][cent][stat].get_val());
			cf_ratio["divide"][energy.first].stat.push_back(def_medians[cf]["divide"][energy.first][div][cent][stat].get_err());
			cf_ratio["divide"][energy.first].sys.push_back(def_systematics[cf]["divide"][energy.first][div][cent][stat]);
			cf_pull["divide"][energy.first].val.push_back(def_medians[cf]["pull_divide"][energy.first][div][cent][stat].get_val());
			cf_pull["divide"][energy.first].stat.push_back(def_medians[cf]["pull_divide"][energy.first][div][cent][stat].get_err());
			cf_pull["divide"][energy.first].sys.push_back(def_systematics[cf]["pull_divide"][energy.first][div][cent][stat]);

			bes1_ratio["raw"][energy.first].val.push_back(def_medians[bes]["raw"][energy.first][div][cent][stat].get_val());
			bes1_ratio["raw"][energy.first].stat.push_back(def_medians[bes]["raw"][energy.first][div][cent][stat].get_err());
			bes1_ratio["raw"][energy.first].sys.push_back(def_systematics[bes]["raw"][energy.first][div][cent][stat]);
			bes1_pull["raw"][energy.first].val.push_back(def_medians[bes]["pull_raw"][energy.first][div][cent][stat].get_val());
			bes1_pull["raw"][energy.first].stat.push_back(def_medians[bes]["pull_raw"][energy.first][div][cent][stat].get_err());
			bes1_pull["raw"][energy.first].sys.push_back(def_systematics[bes]["pull_raw"][energy.first][div][cent][stat]);
			ampt_ratio["raw"][energy.first].val.push_back(def_medians[ampt]["raw"][energy.first][div][cent][stat].get_val());
			ampt_ratio["raw"][energy.first].stat.push_back(def_medians[ampt]["raw"][energy.first][div][cent][stat].get_err());
			ampt_ratio["raw"][energy.first].sys.push_back(def_systematics[ampt]["raw"][energy.first][div][cent][stat]);
			ampt_pull["raw"][energy.first].val.push_back(def_medians[ampt]["pull_raw"][energy.first][div][cent][stat].get_val());
			ampt_pull["raw"][energy.first].stat.push_back(def_medians[ampt]["pull_raw"][energy.first][div][cent][stat].get_err());
			ampt_pull["raw"][energy.first].sys.push_back(def_systematics[ampt]["pull_raw"][energy.first][div][cent][stat]);
			cf_ratio["raw"][energy.first].val.push_back(def_medians[cf]["raw"][energy.first][div][cent][stat].get_val());
			cf_ratio["raw"][energy.first].stat.push_back(def_medians[cf]["raw"][energy.first][div][cent][stat].get_err());
			cf_ratio["raw"][energy.first].sys.push_back(def_systematics[cf]["raw"][energy.first][div][cent][stat]);
			cf_pull["raw"][energy.first].val.push_back(def_medians[cf]["pull_raw"][energy.first][div][cent][stat].get_val());
			cf_pull["raw"][energy.first].stat.push_back(def_medians[cf]["pull_raw"][energy.first][div][cent][stat].get_err());
			cf_pull["raw"][energy.first].sys.push_back(def_systematics[cf]["pull_raw"][energy.first][div][cent][stat]);

			bes1_ratio["mix"][energy.first].val.push_back(def_medians[bes]["mix"][energy.first][div][cent][stat].get_val());
			bes1_ratio["mix"][energy.first].stat.push_back(def_medians[bes]["mix"][energy.first][div][cent][stat].get_err());
			bes1_ratio["mix"][energy.first].sys.push_back(def_systematics[bes]["mix"][energy.first][div][cent][stat]);
			bes1_pull["mix"][energy.first].val.push_back(def_medians[bes]["pull_mix"][energy.first][div][cent][stat].get_val());
			bes1_pull["mix"][energy.first].stat.push_back(def_medians[bes]["pull_mix"][energy.first][div][cent][stat].get_err());
			bes1_pull["mix"][energy.first].sys.push_back(def_systematics[bes]["pull_mix"][energy.first][div][cent][stat]);
			ampt_ratio["mix"][energy.first].val.push_back(def_medians[ampt]["mix"][energy.first][div][cent][stat].get_val());
			ampt_ratio["mix"][energy.first].stat.push_back(def_medians[ampt]["mix"][energy.first][div][cent][stat].get_err());
			ampt_ratio["mix"][energy.first].sys.push_back(def_systematics[ampt]["mix"][energy.first][div][cent][stat]);
			ampt_pull["mix"][energy.first].val.push_back(def_medians[ampt]["pull_mix"][energy.first][div][cent][stat].get_val());
			ampt_pull["mix"][energy.first].stat.push_back(def_medians[ampt]["pull_mix"][energy.first][div][cent][stat].get_err());
			ampt_pull["mix"][energy.first].sys.push_back(def_systematics[ampt]["pull_mix"][energy.first][div][cent][stat]);
			cf_ratio["mix"][energy.first].val.push_back(def_medians[cf]["mix"][energy.first][div][cent][stat].get_val());
			cf_ratio["mix"][energy.first].stat.push_back(def_medians[cf]["mix"][energy.first][div][cent][stat].get_err());
			cf_ratio["mix"][energy.first].sys.push_back(def_systematics[cf]["mix"][energy.first][div][cent][stat]);
			cf_pull["mix"][energy.first].val.push_back(def_medians[cf]["pull_mix"][energy.first][div][cent][stat].get_val());
			cf_pull["mix"][energy.first].stat.push_back(def_medians[cf]["pull_mix"][energy.first][div][cent][stat].get_err());
			cf_pull["mix"][energy.first].sys.push_back(def_systematics[cf]["pull_mix"][energy.first][div][cent][stat]);
		}
	}

	kurt_vs_efficiency_plot(energies, effs, bes1_ratio, ampt_ratio, cf_ratio, "Ratio");
	kurt_vs_efficiency_plot(energies, effs, bes1_pull, ampt_pull, cf_pull, "Pull");
}
 

void AzFigurePlotter::kurt_vs_energy_pres_plot(vector<int> energies, map<string, plot_data> bes1, map<string, plot_data> ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_bes1_raw;
	vector<double> energy_bes1_mix;
	vector<double> energy_ampt;
	vector<double> energy_ampt_raw;
	vector<double> energy_ampt_mix;
	for (double energy : energies) {
		energy_ampt.push_back(energy - 0.7);
		energy_bes1.push_back(energy + 0.7);

		energy_ampt_raw.push_back(energy - 0.3);
		energy_ampt_mix.push_back(energy + 0.4);
		energy_bes1_raw.push_back(energy - 0.4);
		energy_bes1_mix.push_back(energy + 0.3);
	}

	// Divide plot
	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_can_"+type_name).data(), ("Kurtosis vs Energy " + type_name).data(), 955, 805);
	TMultiGraph* mg = new TMultiGraph();
	mg->SetName("Kurtosis_vs_energy_mg");
	float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	if (type_name == "Ratio") { x1 = 0.7743966; y1 = 0.1286812; x2 = 0.9737671; y2 = 0.2989757; }
	else if (type_name == "Pull") { x1 = 0.6222455; y1 = 0.5428937; x2 = 0.8205666; y2 = 0.7131882; }
	TLegend* leg = new TLegend(x1, y1, x2, y2);
	TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"].val, energy_err, bes1["divide"].stat);
	TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"].val, energy_err, ampt["divide"].stat);
	bes1_ratio_def_g->SetNameTitle("bes1");
	ampt_ratio_def_g->SetNameTitle("ampt");
	TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"].val, energy_err, bes1["divide"].sys);
	TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"].val, energy_err, ampt["divide"].sys);

	bes1_ratio_def_g->SetMarkerStyle(29);
	bes1_ratio_def_g->SetMarkerColor(kRed);
	bes1_ratio_def_g->SetLineColor(kRed);
	bes1_ratio_def_g->SetMarkerSize(3);
	bes1_ratio_sys_g->SetLineColor(kRed);

	ampt_ratio_def_g->SetMarkerStyle(20);
	ampt_ratio_def_g->SetMarkerColor(kBlue);
	ampt_ratio_def_g->SetLineColor(kBlue);
	ampt_ratio_def_g->SetMarkerSize(2);
	ampt_ratio_sys_g->SetLineColor(kBlue);

	mg->Add(ampt_ratio_def_g, "APZ");
	mg->Add(ampt_ratio_sys_g, "[]");
	mg->Add(bes1_ratio_def_g, "P");
	mg->Add(bes1_ratio_sys_g, "[]");

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->SetTextFont(43);
	leg->SetTextSize(26);
	leg->AddEntry(bes1_ratio_def_g, "STAR", "p");
	leg->AddEntry(ampt_ratio_def_g, "AMPT", "p");

	mg->GetXaxis()->SetLimits(-2, 78);
	mg->GetXaxis()->SetRangeUser(-2, 78);
	mg->GetYaxis()->SetLimits(0.9961, 1.0109);
	mg->GetYaxis()->SetRangeUser(0.9961, 1.0109);
	mg->GetXaxis()->SetLabelFont(43);
	mg->GetXaxis()->SetTitleFont(43);
	mg->GetXaxis()->SetLabelSize(28);
	mg->GetXaxis()->SetTitleSize(28);
	mg->GetYaxis()->SetLabelFont(43);
	mg->GetYaxis()->SetTitleFont(43);
	mg->GetYaxis()->SetLabelSize(28);
	mg->GetYaxis()->SetTitleSize(28);
	mg->GetYaxis()->SetDecimals(3);
	mg->GetXaxis()->SetTitleOffset(1.2);
	mg->GetYaxis()->SetTitleOffset(1.7);

	mg->GetXaxis()->SetTitle("Energy (GeV)");
	mg->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mixed}");

	mg->Draw("AP");

	TLine* one_line = new TLine(-2, 1, 78, 1);
	one_line->SetLineStyle(2);
	one_line->Draw();

	leg->Draw();

	gPad->SetTopMargin(0.04);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.04);

	TLatex label;
	label.SetTextFont(43);
	label.SetTextAlign(12);
	label.SetTextSize(26);
	string label_text = "#splitline{#splitline{#splitline{#splitline{#splitline{#bf{" + type_name + " Transformation}}{Protons}}{0-5% Centrality}}{120#circ Divisions}}{|y| < 0.5}}{0.4 < p_{T}(GeV/c) < 2.0}";
	if (type_name == "Pull") { label.DrawLatex(6.858242, 1.008371, label_text.data()); }
	else if (type_name == "Ratio") { label.DrawLatex(34.14051, 1.002157, label_text.data()); }

//	TLatex y_val_label;
//	y_val_label.SetTextFont(43);
//	y_val_label.SetTextAlign(12);
//	y_val_label.SetTextSize(30);
//	string y_val_text = "#frac{Raw " + type_name + " Distribution Kurtosis}{Mixed " + type_name + " Distribution Kurtosis}";
//	if (type_name == "Pull") { y_val_label.DrawLatex(3.360516, 1.009804, y_val_text.data()); }
//	else if (type_name == "Ratio") { y_val_label.DrawLatex(3.360516, 1.009804, y_val_text.data()); }

	TLatex prelim;
	prelim.SetTextFont(43);
	prelim.SetTextAlign(12);
	prelim.SetTextSize(31);
	prelim.SetNDC();
	if (type_name == "Pull") { prelim.DrawLatex(0.71, 0.93, "STAR Preliminary"); }
	else if (type_name == "Ratio") { prelim.DrawLatex(0.71, 0.93, "STAR Preliminary"); }

	TLatex x_shift;
	x_shift.SetTextFont(43);
	x_shift.SetTextAlign(12);
	x_shift.SetTextSize(18);
	x_shift.SetNDC();
	if (type_name == "Pull") { x_shift.DrawLatex(0.1479538, 0.03457106, "Energy values slightly offset for clarity"); }
	if (type_name == "Ratio") { x_shift.DrawLatex(0.1479538, 0.03457106, "Energy values slightly offset for clarity"); }

	can->Update();
	can->Write();

	delete can;


	// Raw vs mixed plot
	TCanvas* can_rm = new TCanvas(("Kurtosis_vs_energy_raw_mix_can_"+type_name).data(), ("Kurtosis vs Energy " + type_name).data(), 955, 805);
	TMultiGraph* mg_rm = new TMultiGraph();
	mg_rm->SetName("Kurtosis_vs_energy_raw_mix_mg");
	if (type_name == "Ratio") { x1 = 0.7240294; y1 = 0.1792574; x2 = 0.980063; y2 = 0.3290653; }
	else if (type_name == "Pull") { x1 = 0.7040923; y1 = 0.7016645; x2 = 0.9601259; y2 = 0.8514725; }
	TLegend* leg_rm = new TLegend(x1, y1, x2, y2);
	TGraphErrors* bes1_ratio_def_raw_g = graph_x_vs_y_err(energy_bes1_raw, bes1["raw"].val, energy_err, bes1["raw"].stat);
	TGraphErrors* ampt_ratio_def_raw_g = graph_x_vs_y_err(energy_ampt_raw, ampt["raw"].val, energy_err, ampt["raw"].stat);
	bes1_ratio_def_raw_g->SetNameTitle("bes1");
	ampt_ratio_def_raw_g->SetNameTitle("ampt");
	TGraphErrors* bes1_ratio_sys_raw_g = graph_x_vs_y_err(energy_bes1_raw, bes1["raw"].val, energy_err, bes1["raw"].sys);
	TGraphErrors* ampt_ratio_sys_raw_g = graph_x_vs_y_err(energy_ampt_raw, ampt["raw"].val, energy_err, ampt["raw"].sys);

	TGraphErrors* bes1_ratio_def_mix_g = graph_x_vs_y_err(energy_bes1_mix, bes1["mix"].val, energy_err, bes1["mix"].stat);
	TGraphErrors* ampt_ratio_def_mix_g = graph_x_vs_y_err(energy_ampt_mix, ampt["mix"].val, energy_err, ampt["mix"].stat);
	bes1_ratio_def_mix_g->SetNameTitle("bes1");
	ampt_ratio_def_mix_g->SetNameTitle("ampt");
	TGraphErrors* bes1_ratio_sys_mix_g = graph_x_vs_y_err(energy_bes1_mix, bes1["mix"].val, energy_err, bes1["mix"].sys);
	TGraphErrors* ampt_ratio_sys_mix_g = graph_x_vs_y_err(energy_ampt_mix, ampt["mix"].val, energy_err, ampt["mix"].sys);

	bes1_ratio_def_raw_g->SetMarkerStyle(29);
	bes1_ratio_def_raw_g->SetMarkerColor(kRed);
	bes1_ratio_def_raw_g->SetLineColor(kRed);
	bes1_ratio_def_raw_g->SetMarkerSize(3);
	bes1_ratio_sys_raw_g->SetLineColor(kRed);

	bes1_ratio_def_mix_g->SetMarkerStyle(30);
	bes1_ratio_def_mix_g->SetMarkerColor(kRed);
	bes1_ratio_def_mix_g->SetLineColor(kRed);
	bes1_ratio_def_mix_g->SetMarkerSize(3);
	bes1_ratio_sys_mix_g->SetLineColor(kRed);

	ampt_ratio_def_raw_g->SetMarkerStyle(20);
	ampt_ratio_def_raw_g->SetMarkerColor(kBlue);
	ampt_ratio_def_raw_g->SetLineColor(kBlue);
	ampt_ratio_def_raw_g->SetMarkerSize(2);
	ampt_ratio_sys_raw_g->SetLineColor(kBlue);

	ampt_ratio_def_mix_g->SetMarkerStyle(24);
	ampt_ratio_def_mix_g->SetMarkerColor(kBlue);
	ampt_ratio_def_mix_g->SetLineColor(kBlue);
	ampt_ratio_def_mix_g->SetMarkerSize(2);
	ampt_ratio_sys_mix_g->SetLineColor(kBlue);

	mg_rm->Add(ampt_ratio_def_mix_g, "APZ");
	mg_rm->Add(ampt_ratio_sys_mix_g, "[]");
	mg_rm->Add(ampt_ratio_def_raw_g, "P");
	mg_rm->Add(ampt_ratio_sys_raw_g, "[]");
	mg_rm->Add(bes1_ratio_def_mix_g, "P");
	mg_rm->Add(bes1_ratio_sys_mix_g, "[]");
	mg_rm->Add(bes1_ratio_def_raw_g, "P");
	mg_rm->Add(bes1_ratio_sys_raw_g, "[]");

	leg_rm->SetNColumns(3);
	leg_rm->SetBorderSize(0);
	leg_rm->SetFillStyle(0);
	leg_rm->SetTextFont(43);
	leg_rm->SetTextSize(26);
	leg_rm->SetMargin(-0.7);
	leg_rm->AddEntry("", "", "");
	leg_rm->AddEntry("", "STAR", "");
	leg_rm->AddEntry("", "AMPT", "");
	leg_rm->AddEntry("", "Raw", "");
	leg_rm->AddEntry(bes1_ratio_def_raw_g, "", "p");
	leg_rm->AddEntry(ampt_ratio_def_raw_g, "", "p");
	leg_rm->AddEntry("", "Mixed", "");
	leg_rm->AddEntry(bes1_ratio_def_mix_g, "", "p");
	leg_rm->AddEntry(ampt_ratio_def_mix_g, "", "p");
//	leg_rm->AddEntry(bes1_ratio_def_raw_g, "STAR Raw", "p");
//	leg_rm->AddEntry(ampt_ratio_def_raw_g, "AMPT Raw", "p");
//	leg_rm->AddEntry(bes1_ratio_def_mix_g, "STAR Mixed", "p");
//	leg_rm->AddEntry(ampt_ratio_def_mix_g, "AMPT Mixed", "p");

	mg_rm->GetXaxis()->SetLimits(-2, 78);
	mg_rm->GetXaxis()->SetRangeUser(-2, 78);
	if (type_name == "Pull") {
		mg_rm->GetYaxis()->SetLimits(2.79, 3.01);
		mg_rm->GetYaxis()->SetRangeUser(2.79, 3.01);
	}
	else if (type_name == "Ratio") {
		mg_rm->GetYaxis()->SetLimits(2.99, 3.26);
		mg_rm->GetYaxis()->SetRangeUser(2.99, 3.26);
	}
//	mg_rm->GetYaxis()->SetLimits(0.9955, 1.0145);
//	mg_rm->GetYaxis()->SetRangeUser(0.9955, 1.0145);
	mg_rm->GetXaxis()->SetLabelFont(43);
	mg_rm->GetXaxis()->SetTitleFont(43);
	mg_rm->GetXaxis()->SetLabelSize(28);
	mg_rm->GetXaxis()->SetTitleSize(28);
	mg_rm->GetYaxis()->SetLabelFont(43);
	mg_rm->GetYaxis()->SetTitleFont(43);
	mg_rm->GetYaxis()->SetLabelSize(28);
	mg_rm->GetYaxis()->SetTitleSize(28);
	mg_rm->GetYaxis()->SetDecimals(3);
	mg_rm->GetXaxis()->SetTitleOffset(1.2);
	mg_rm->GetYaxis()->SetTitleOffset(1.7);

	mg_rm->GetXaxis()->SetTitle("Energy (GeV)");
	mg_rm->GetYaxis()->SetTitle("Kurtosis");

	mg_rm->Draw("AP");

	TLine* one_line_rm = new TLine(-2, 1, 78, 1);
	one_line_rm->SetLineStyle(2);
	one_line_rm->Draw();

	leg_rm->Draw();

	gPad->SetTopMargin(0.04);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.04);

	TLatex label_rm;
	label_rm.SetTextFont(43);
	label_rm.SetTextAlign(12);
	label_rm.SetTextSize(26);
	label_rm.SetNDC();
	string label_text_rm = "#splitline{#splitline{#splitline{#splitline{#splitline{#bf{" + type_name + " Transformation}}{Protons}}{0-5% Centrality}}{120#circ Divisions}}{|y| < 0.5}}{0.4 < p_{T}(GeV/c) < 2.0}";
	if (type_name == "Pull") { label_rm.DrawLatex(0.1710388, 0.2432778, label_text_rm.data()); }
	else if (type_name == "Ratio") { label_rm.DrawLatex(0.1710388, 0.7861716, label_text_rm.data()); }

//	TLatex y_val_label_rm;
//	y_val_label_rm.SetTextFont(43);
//	y_val_label_rm.SetTextAlign(12);
//	y_val_label_rm.SetTextSize(30);
//	y_val_label_rm.SetNDC();
//	string y_val_text_rm = type_name + " Distribution Kurtosis";
//	if (type_name == "Pull") { y_val_label_rm.DrawLatex(0.1741868, 0.8924456, y_val_text_rm.data()); }
//	else if (type_name == "Ratio") { y_val_label_rm.DrawLatex(0.1741868, 0.8924456, y_val_text_rm.data()); }

	TLatex prelim_rm;
	prelim_rm.SetTextFont(43);
	prelim_rm.SetTextAlign(12);
	prelim_rm.SetTextSize(31);
	prelim_rm.SetNDC();
	if (type_name == "Pull") { prelim_rm.DrawLatex(0.71, 0.93, "STAR Preliminary"); }
	else if (type_name == "Ratio") { prelim_rm.DrawLatex(0.71, 0.93, "STAR Preliminary"); }

	TLatex x_shift_rm;
	x_shift_rm.SetTextFont(43);
	x_shift_rm.SetTextAlign(12);
	x_shift_rm.SetTextSize(18);
	x_shift_rm.SetNDC();
	if (type_name == "Pull") { x_shift_rm.DrawLatex(0.1479538, 0.03457106, "Energy values slightly offset for clarity"); }
	if (type_name == "Ratio") { x_shift_rm.DrawLatex(0.1479538, 0.03457106, "Energy values slightly offset for clarity"); }

	can_rm->Update();
	can_rm->Write();

	delete can_rm;
}


void AzFigurePlotter::kurt_vs_energy_cf_pres_plot(vector<int> energies, map<string, plot_data> bes1, map<string, plot_data> ampt, map<string, plot_data> cf, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	vector<double> energy_cf;
	for (double energy : energies) {
		energy_ampt.push_back(energy - 0.5);
		energy_bes1.push_back(energy);
		energy_cf.push_back(energy + 0.5);
	}

	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_cf_can_"+type_name).data(), ("Kurtosis vs Energy CF " + type_name).data(), 955, 805);
	TMultiGraph* mg = new TMultiGraph();
	mg->SetName("Kurtosis_vs_energy_cf_mg");
	TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
	TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"].val, energy_err, bes1["divide"].stat);
	TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"].val, energy_err, ampt["divide"].stat);
	TGraphErrors* cf_ratio_def_g = graph_x_vs_y_err(energy_cf, cf["divide"].val, energy_err, cf["divide"].stat);
	bes1_ratio_def_g->SetNameTitle("bes1");
	ampt_ratio_def_g->SetNameTitle("ampt");
	cf_ratio_def_g->SetNameTitle("cf");
	TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"].val, energy_err, bes1["divide"].sys);
	TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"].val, energy_err, ampt["divide"].sys);
	TGraphErrors* cf_ratio_sys_g = graph_x_vs_y_err(energy_cf, cf["divide"].val, energy_err, cf["divide"].sys);

	bes1_ratio_def_g->SetMarkerStyle(20);
	bes1_ratio_def_g->SetMarkerColor(kBlack);
	bes1_ratio_def_g->SetLineColor(kBlack);
	bes1_ratio_def_g->SetMarkerSize(2);
	bes1_ratio_sys_g->SetLineColor(kBlack);

	ampt_ratio_def_g->SetMarkerStyle(24);
	ampt_ratio_def_g->SetMarkerColor(kRed);
	ampt_ratio_def_g->SetLineColor(kRed);
	ampt_ratio_def_g->SetMarkerSize(2);
	ampt_ratio_sys_g->SetLineColor(kRed);

	cf_ratio_def_g->SetMarkerStyle(24);
	cf_ratio_def_g->SetMarkerColor(kBlue);
	cf_ratio_def_g->SetLineColor(kBlue);
	cf_ratio_def_g->SetMarkerSize(2);
	cf_ratio_sys_g->SetLineColor(kBlue);

	mg->Add(cf_ratio_def_g, "APZ");
	mg->Add(cf_ratio_sys_g, "[]");
	mg->Add(ampt_ratio_def_g, "P");
	mg->Add(ampt_ratio_sys_g, "[]");
	mg->Add(bes1_ratio_def_g, "P");
	mg->Add(bes1_ratio_sys_g, "[]");

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->AddEntry(bes1_ratio_def_g, "BES1", "p");
	leg->AddEntry(ampt_ratio_def_g, "AMPT", "p");
	leg->AddEntry(cf_ratio_def_g, "Cooper-Frye", "p");

	mg->GetXaxis()->SetLimits(-2, 78);
	mg->GetXaxis()->SetRangeUser(-2, 78);
	mg->GetYaxis()->SetLimits(0.9955, 1.0145);
	mg->GetYaxis()->SetRangeUser(0.9955, 1.0145);
	mg->GetXaxis()->SetLabelFont(43);
	mg->GetXaxis()->SetTitleFont(43);
	mg->GetXaxis()->SetLabelSize(def_text_size);
	mg->GetXaxis()->SetTitleSize(def_text_size);
	mg->GetYaxis()->SetLabelFont(43);
	mg->GetYaxis()->SetTitleFont(43);
	mg->GetYaxis()->SetLabelSize(def_text_size);
	mg->GetYaxis()->SetTitleSize(def_text_size);
	mg->GetYaxis()->SetDecimals(3);
	mg->GetXaxis()->SetTitleOffset(2.9);
	mg->GetYaxis()->SetTitleOffset(4.4);

	mg->GetXaxis()->SetTitle("Energy (GeV)");
	mg->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mixed}");

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


void AzFigurePlotter::kurt_vs_divs_w_energy_pres_plot(map<int, int> energies, vector<int> div_order, map<int, plot_data> bes1, map<int, plot_data> ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	map<int, vector<double>> energy_vals;
	double offset = 0;
	for ( int div : div_order) {
		for (pair<int, int> energy : energies) {
			if (div > 180) { offset = 0.7; }
			else if (div < 180) { offset = -0.7; }
			else { offset = 0; }

			energy_vals[div].push_back(energy.second + offset);
		}
	}

	map<int, int> div_color{ {60, kBlack}, {72, kRed}, {90, kGreen+3}, {120, kBlue}, {180, kViolet}, {240, kCyan + 2}, {270, kGreen + 1}, {300, kGray + 2} };
	map<int, int> marker_style{ {60, 22}, {72, 2}, {90, 21}, {120, 20}, {180, 34}, {240, 24}, {270, 25}, {300, 26} };
	map<int, int> div_marker_size{ {60, 2}, {72, 2}, {90, 2}, {120, 2}, {180, 2}, {240, 2}, {270, 2}, {300, 2} };
	int text_size = 28;

	// BES1
	TCanvas* can_bes = new TCanvas(("Kurtosis_vs_energy_divs_can_bes_" + type_name).data(), ("BES1 Kurtosis vs Energy with Bin Width " + type_name).data(), 955, 805);
	TMultiGraph* mg_bes = new TMultiGraph();
	mg_bes->SetName("Kurtosis_vs_energy_mg_bes");

	double y1 = 0.1, y2 = 0.2, x1 = 0.7, x2 = 0.9;
	if (type_name == "Ratio") { x1 = 0.7177335; y1 = 0.1395647; x2 = 0.9171039; y2 = 0.3393086; }
	else if (type_name == "Pull") { x1 = 0.5687303; y1 = 0.6696543; x2 = 0.7691501; y2 = 0.8693982; }
	TLegend* leg_bes = new TLegend(x1, y1, x2, y2);
	leg_bes->SetHeader("Angular Width", "C");
	for ( int div : div_order) {
		TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_vals[div], bes1[div].val, energy_err, bes1[div].stat);
		bes1_ratio_def_g->SetNameTitle(("bes1_"+to_string(div)).data());
		TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_vals[div], bes1[div].val, energy_err, bes1[div].sys);

		bes1_ratio_def_g->SetMarkerStyle(marker_style[div]);
		bes1_ratio_def_g->SetMarkerColor(div_color[div]);
		bes1_ratio_def_g->SetLineColor(div_color[div]);
		bes1_ratio_def_g->SetMarkerSize(div_marker_size[div]);
		bes1_ratio_sys_g->SetLineColor(div_color[div]);

		mg_bes->Add(bes1_ratio_def_g, "APZ");
		mg_bes->Add(bes1_ratio_sys_g, "[]");

		leg_bes->AddEntry(bes1_ratio_def_g, (to_string(div) + "#circ").data(), "p");
		if (div == 180) { leg_bes->AddEntry("", "", ""); }
	}

	leg_bes->SetBorderSize(0);
	leg_bes->SetFillStyle(0);

	mg_bes->GetXaxis()->SetLimits(-2, 78);
	mg_bes->GetXaxis()->SetRangeUser(-2, 78);
	if (!zoom) {
		mg_bes->GetYaxis()->SetLimits(0.988, 1.0129);
		mg_bes->GetYaxis()->SetRangeUser(0.988, 1.0129);
	}

	mg_bes->GetXaxis()->SetTitle("Energy (GeV)");
	mg_bes->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mix}");
	mg_bes->GetXaxis()->SetLabelFont(43);
	mg_bes->GetXaxis()->SetTitleFont(43);
	mg_bes->GetXaxis()->SetLabelSize(text_size);
	mg_bes->GetXaxis()->SetTitleSize(text_size);
	mg_bes->GetYaxis()->SetLabelFont(43);
	mg_bes->GetYaxis()->SetTitleFont(43);
	mg_bes->GetYaxis()->SetLabelSize(text_size);
	mg_bes->GetYaxis()->SetTitleSize(text_size);
	mg_bes->GetYaxis()->SetDecimals(3);
	mg_bes->GetXaxis()->SetTitleOffset(1.4);
	mg_bes->GetYaxis()->SetTitleOffset(1.8);

	mg_bes->Draw("AP");

	TLine* one_line_bes = new TLine(-2, 1, 78, 1);
	one_line_bes->SetLineStyle(2);
	one_line_bes->Draw();

	leg_bes->SetNColumns(2);
//	leg_bes->SetTextFont(43);
//	leg_bes->SetTextSize(text_size);
	leg_bes->SetMargin(0.3);
	leg_bes->Draw();

	TLatex label_bes;
	label_bes.SetTextFont(43);
	label_bes.SetTextAlign(12);
	label_bes.SetTextSize(text_size - 2);
	label_bes.SetNDC();
	string label_text_bes = "#splitline{#splitline{#splitline{#splitline{#splitline{#bf{" + type_name + " Transformation}}{STAR}}{Protons}}{0-5% Centrality}}{|y| < 0.5}}{0.4 < p_{T}(GeV/c) < 2.0}";
	if (type_name == "Pull") { label_bes.DrawLatex(0.18468, 0.8245839, label_text_bes.data()); }
	else if (type_name == "Ratio") { label_bes.DrawLatex(0.3913956, 0.2317542, label_text_bes.data()); }

//	TLatex y_val_label_bes;
//	y_val_label_bes.SetTextFont(43);
//	y_val_label_bes.SetTextAlign(12);
//	y_val_label_bes.SetTextSize(text_size + 2);
//	y_val_label_bes.SetNDC();
//	string y_val_text_bes = "#frac{Raw " + type_name + " Distribution Kurtosis}{Mixed " + type_name + " Distribution Kurtosis}";
//	if (type_name == "Pull") { y_val_label_bes.DrawLatex(0.1668416, 0.8975672, y_val_text_bes.data()); }
//	else if (type_name == "Ratio") { y_val_label_bes.DrawLatex(0.1490031, 0.1702945, y_val_text_bes.data()); }

	TLatex prelim_bes;
	prelim_bes.SetTextFont(43);
	prelim_bes.SetTextAlign(12);
	prelim_bes.SetTextSize(text_size + 3);
	prelim_bes.SetNDC();
	if (type_name == "Pull") { prelim_bes.DrawLatex(0.71, 0.93, "STAR Preliminary"); }
	else if (type_name == "Ratio") { prelim_bes.DrawLatex(0.71, 0.93, "STAR Preliminary"); }

	TLatex x_shift;
	x_shift.SetTextFont(43);
	x_shift.SetTextAlign(12);
	x_shift.SetTextSize(18);
	x_shift.SetNDC();
	if (type_name == "Pull") { x_shift.DrawLatex(0.1479538, 0.03457106, "Energy values slightly offset for clarity"); }
	if (type_name == "Ratio") { x_shift.DrawLatex(0.1479538, 0.03457106, "Energy values slightly offset for clarity"); }

	gPad->SetTopMargin(0.04);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.04);

	can_bes->Update();
	can_bes->Write();

	delete can_bes;


	TCanvas* can_ampt = new TCanvas(("Kurtosis_vs_energy_divs_can_ampt_" + type_name).data(), ("AMPT Kurtosis vs Energy with Bin Width " + type_name).data(), 955, 805);
	TMultiGraph* mg_ampt = new TMultiGraph();
	mg_ampt->SetName("Kurtosis_vs_energy_mg_ampt");
	if (type_name == "Ratio") { x1 = 0.1500525; y1 = 0.1293214; x2 = 0.3494229; y2 = 0.3290653; }
	else if (type_name == "Pull") { x1 = 0.5897167; y1 = 0.7323944; x2 = 0.7901364; y2 = 0.9321383; }
	TLegend* leg_ampt = new TLegend(x1, y1, x2, y2);
	leg_ampt->SetHeader("Angular Width", "C");
	for ( int div : div_order) {
		TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_vals[div], ampt[div].val, energy_err, ampt[div].stat);
		ampt_ratio_def_g->SetNameTitle(("ampt_" + to_string(div)).data());
		TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_vals[div], ampt[div].val, energy_err, ampt[div].sys);

		ampt_ratio_def_g->SetMarkerStyle(marker_style[div]);
		ampt_ratio_def_g->SetMarkerColor(div_color[div]);
		ampt_ratio_def_g->SetLineColor(div_color[div]);
		ampt_ratio_def_g->SetMarkerSize(div_marker_size[div]);
		ampt_ratio_sys_g->SetLineColor(div_color[div]);

		mg_ampt->Add(ampt_ratio_def_g, "APZ");
		mg_ampt->Add(ampt_ratio_sys_g, "[]");

		leg_ampt->AddEntry(ampt_ratio_def_g, (to_string(div) + "#circ").data(), "p");
		if (div == 180) { leg_ampt->AddEntry("", "", ""); }
	}

	leg_ampt->SetBorderSize(0);
	leg_ampt->SetFillStyle(0);

	mg_ampt->GetXaxis()->SetLimits(-2, 78);
	mg_ampt->GetXaxis()->SetRangeUser(-2, 78);
	if (!zoom) {
		mg_ampt->GetYaxis()->SetLimits(0.988, 1.0129);
		mg_ampt->GetYaxis()->SetRangeUser(0.988, 1.0129);
	}

	mg_ampt->GetXaxis()->SetTitle("Energy (GeV)");
	mg_ampt->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mixed}");
	mg_ampt->GetXaxis()->SetLabelFont(43);
	mg_ampt->GetXaxis()->SetTitleFont(43);
	mg_ampt->GetXaxis()->SetLabelSize(text_size);
	mg_ampt->GetXaxis()->SetTitleSize(text_size);
	mg_ampt->GetYaxis()->SetLabelFont(43);
	mg_ampt->GetYaxis()->SetTitleFont(43);
	mg_ampt->GetYaxis()->SetLabelSize(text_size);
	mg_ampt->GetYaxis()->SetTitleSize(text_size);
	mg_ampt->GetYaxis()->SetDecimals(3);
	mg_ampt->GetXaxis()->SetTitleOffset(1.4);
	mg_ampt->GetYaxis()->SetTitleOffset(1.8);

	mg_ampt->Draw("AP");

	TLine* one_line = new TLine(-2, 1, 78, 1);
	one_line->SetLineStyle(2);
	one_line->Draw();

	leg_ampt->SetNColumns(2);
//	leg_ampt->SetTextFont(43);  // Somehow lose header with this
//	leg_ampt->SetTextSize(text_size);
	leg_ampt->SetMargin(0.3);
	leg_ampt->Draw();

	TLatex label_ampt;
	label_ampt.SetTextFont(43);
	label_ampt.SetTextAlign(12);
	label_ampt.SetTextSize(text_size - 2);
	label_ampt.SetNDC();
	string label_text_ampt = "#splitline{#splitline{#splitline{#splitline{#splitline{#bf{" + type_name + " Transformation}}{AMPT}}{Protons}}{0-5% Centrality}}{|y| < 0.5}}{0.4 < p_{T}(GeV/c) < 2.0}";
	if (type_name == "Pull") { label_ampt.DrawLatex(0.1741868, 0.8322663, label_text_ampt.data()); }
	else if (type_name == "Ratio") { label_ampt.DrawLatex(0.570829, 0.2291933, label_text_ampt.data()); }

//	TLatex y_val_label_ampt;
//	y_val_label_ampt.SetTextFont(43);
//	y_val_label_ampt.SetTextAlign(12);
//	y_val_label_ampt.SetTextSize(text_size + 2);
//	y_val_label_ampt.SetNDC();
//	string y_val_text_ampt = "#frac{Raw " + type_name + " Distribution Kurtosis}{Mixed " + type_name + " Distribution Kurtosis}";
//	if (type_name == "Pull") { y_val_label_ampt.DrawLatex(0.1668416, 0.8975672, y_val_text_ampt.data()); }
//	else if (type_name == "Ratio") { y_val_label_ampt.DrawLatex(0.1490031, 0.1702945, y_val_text_ampt.data()); }

//	TLatex prelim_ampt;
//	prelim_ampt.SetTextFont(43);
//	prelim_ampt.SetTextAlign(12);
//	prelim_ampt.SetTextSize(text_size);
//	if (type_name == "Pull") { prelim_ampt.DrawLatex(5, 0.991, "STAR Preliminary"); }
//	else if (type_name == "Ratio") { prelim_ampt.DrawLatex(3, 1.012, "STAR Preliminary"); }

	TLatex x_shift_ampt;
	x_shift_ampt.SetTextFont(43);
	x_shift_ampt.SetTextAlign(12);
	x_shift_ampt.SetTextSize(18);
	x_shift_ampt.SetNDC();
	if (type_name == "Pull") { x_shift_ampt.DrawLatex(0.1479538, 0.03457106, "Energy values slightly offset for clarity"); }
	if (type_name == "Ratio") { x_shift_ampt.DrawLatex(0.1479538, 0.03457106, "Energy values slightly offset for clarity"); }

	gPad->SetTopMargin(0.04);
	gPad->SetLeftMargin(0.12);
	gPad->SetRightMargin(0.04);

	can_ampt->Update();
	can_ampt->Write();

	delete can_ampt;
}


void AzFigurePlotter::sd_vs_energy_cf_plot(vector<int> energies, plot_data bes1, plot_data ampt, plot_data cf, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	vector<double> energy_cf;
	for (double energy : energies) {
		energy_ampt.push_back(energy - 0.5);
		energy_bes1.push_back(energy);
		energy_cf.push_back(energy + 0.5);
	}

	TCanvas* can = new TCanvas(("SD_vs_energy_cf_can_"+type_name).data(), ("Standard Deviation vs Energy CF " + type_name).data(), 955, 805);
	TMultiGraph* mg = new TMultiGraph();
	mg->SetName("SD_vs_energy_cf_mg");
	TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
	TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1.val, energy_err, bes1.stat);
	TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt.val, energy_err, ampt.stat);
	TGraphErrors* cf_ratio_def_g = graph_x_vs_y_err(energy_cf, cf.val, energy_err, cf.stat);
	bes1_ratio_def_g->SetNameTitle("bes1");
	ampt_ratio_def_g->SetNameTitle("ampt");
	cf_ratio_def_g->SetNameTitle("cf");
	TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1.val, energy_err, bes1.sys);
	TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt.val, energy_err, ampt.sys);
	TGraphErrors* cf_ratio_sys_g = graph_x_vs_y_err(energy_cf, cf.val, energy_err, cf.sys);

	bes1_ratio_def_g->SetMarkerStyle(20);
	bes1_ratio_def_g->SetMarkerColor(kBlack);
	bes1_ratio_def_g->SetLineColor(kBlack);
	bes1_ratio_def_g->SetMarkerSize(2);
	bes1_ratio_sys_g->SetLineColor(kBlack);

	ampt_ratio_def_g->SetMarkerStyle(24);
	ampt_ratio_def_g->SetMarkerColor(kRed);
	ampt_ratio_def_g->SetLineColor(kRed);
	ampt_ratio_def_g->SetMarkerSize(2);
	ampt_ratio_sys_g->SetLineColor(kRed);

	cf_ratio_def_g->SetMarkerStyle(24);
	cf_ratio_def_g->SetMarkerColor(kBlue);
	cf_ratio_def_g->SetLineColor(kBlue);
	cf_ratio_def_g->SetMarkerSize(2);
	cf_ratio_sys_g->SetLineColor(kBlue);

	mg->Add(cf_ratio_def_g, "APZ");
	mg->Add(cf_ratio_sys_g, "[]");
	mg->Add(ampt_ratio_def_g, "P");
	mg->Add(ampt_ratio_sys_g, "[]");
	mg->Add(bes1_ratio_def_g, "P");
	mg->Add(bes1_ratio_sys_g, "[]");

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->AddEntry(bes1_ratio_def_g, "BES1", "p");
	leg->AddEntry(ampt_ratio_def_g, "AMPT", "p");
	leg->AddEntry(cf_ratio_def_g, "Cooper-Frye", "p");

	mg->GetXaxis()->SetLimits(-2, 78);
	mg->GetXaxis()->SetRangeUser(-2, 78);
	if (!zoom) {
		mg->GetYaxis()->SetLimits(0.948, 1.012);
		mg->GetYaxis()->SetRangeUser(0.948, 1.012);
	}
	mg->GetXaxis()->SetLabelFont(43);
	mg->GetXaxis()->SetTitleFont(43);
	mg->GetXaxis()->SetLabelSize(def_text_size);
	mg->GetXaxis()->SetTitleSize(def_text_size);
	mg->GetYaxis()->SetLabelFont(43);
	mg->GetYaxis()->SetTitleFont(43);
	mg->GetYaxis()->SetLabelSize(def_text_size);
	mg->GetYaxis()->SetTitleSize(def_text_size);
	mg->GetYaxis()->SetDecimals(3);
	mg->GetXaxis()->SetTitleOffset(2.9);
	mg->GetYaxis()->SetTitleOffset(4.4);

	mg->GetXaxis()->SetTitle("Energy (GeV)");
	mg->GetYaxis()->SetTitle("Standard Deviation_{Raw}  /  Standard Deviation_{Mix}");

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


void AzFigurePlotter::kurt_vs_divs_w_energy_plot(map<int, string> energies, vector<int> divs, map<string, map<int, plot_data>> bes1, map<string, map<int, plot_data>> ampt, map<string, map<int, plot_data>> cf, string type_name) {
	vector<double> divs_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> divs_val(divs.begin(), divs.end());
	vector<double> divs_bes1;
	vector<double> divs_ampt;
	vector<double> divs_cf;
	for (int div : divs) {
		divs_ampt.push_back(div - 7);
		divs_bes1.push_back(div);
		divs_cf.push_back(div + 7);
	}

	map<string, int> color{ {"7.7", kBlue}, {"11.5", kGreen + 3}, {"19.6", kRed}, {"27", kBlack}, {"39", kGray + 2}, {"62.4", kViolet} };
	map<string, int> marker_style{ {"bes", 20}, {"ampt", 24}, {"cf", 24} };
	map<string, int> marker_size{ {"bes", 2}, {"ampt", 2}, {"cf", 2} };
	map<string, int> marker_color{ {"bes", kBlack}, {"ampt", kRed}, {"cf", kBlue} };


	TCanvas* can_multi = new TCanvas(("Kurtosis_vs_Bin_Width_with_energy_can_" + type_name).data(), ("Kurtosis vs Bin Width with Energy " + type_name).data(), 955, 805);

	int x_num = 3;
	int y_num = 2;

	double y_top_pad = 0.01;
	double y_bot_pad = 0.05;
	double x_left_pad = 0.076;
	double x_right_pad = 0.01;
	double x_low = 0;
	double x_up = 0;
	double y_low = 0;
	double y_up = 1.0;
	double def_left_margin = 0.0;
	double def_right_margin = 0.0;
	double def_top_margin = 0.0;
	double def_bot_margin = 0.0;

	double x_step = (1.0 - x_left_pad - x_right_pad) / x_num;
	double y_step = (1.0 - y_top_pad - y_bot_pad) / y_num;
	//	cout << "x_step: " << x_step << "  |  y_step: " << y_step << endl;
	for (int y_index = 0; y_index < y_num; y_index++) {
		x_low = 0;
		if (y_index == 0) { y_low = y_up - y_step - y_top_pad; }
		else if (y_index == y_num - 1) { y_low = y_up - y_step - y_bot_pad; }
		else { y_low = y_up - y_step; }

		for (int x_index = 0; x_index < x_num; x_index++) {
			int pad_index = x_index + x_num * y_index + 1;

			if (x_index == 0) { x_up = x_low + x_step + x_left_pad; }
			else if (x_index == x_num - 1) { x_up = x_low + x_step + x_right_pad; }
			else { x_up = x_low + x_step; }

			if (y_up > 1.0) { y_up = 1.0; }
			if (y_low < 0.0) { y_low = 0.0; }
			if (x_up > 1.0) { x_up = 1.0; }
			if (x_low < 0.0) { x_low = 0.0; }

			//			cout << "x_index: " << x_index << " x_low: " << x_low << " x_up: " << x_up << endl;
			//			cout << "y_index: " << y_index << " y_low: " << y_low << " y_up: " << y_up << endl;

			string pad_name = gPad->GetName() + to_string(pad_index);
			TPad* pad = new TPad(pad_name.data(), pad_name.data(), x_low, y_low, x_up, y_up);
			pad->SetTickx();
			pad->SetTicky();

			if (x_index == 0) { pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin); }
			else { pad->SetLeftMargin(def_left_margin); }
			if (x_index == x_num - 1) { pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin); }
			else { pad->SetRightMargin(def_right_margin); }
			if (y_index == 0) { pad->SetTopMargin(y_top_pad / (y_up - y_low) + def_top_margin); }
			else { pad->SetTopMargin(def_top_margin); }
			if (y_index == y_num - 1) { pad->SetBottomMargin(y_bot_pad / (y_up - y_low) + def_bot_margin); }
			else { pad->SetBottomMargin(def_bot_margin); }

			pad->SetNumber(pad_index);

			pad->Draw();
			x_low = x_up;
		}
		y_up = y_low;
	}

	int can_index = 0;

	for (pair<int, string> energy : energies) { // Iterate over energies, left to right on plot
		can_multi->cd(++can_index);
//		cout << energy.second << " GeV can_index: " << can_index << endl;

		TMultiGraph* mg_div = new TMultiGraph();
		mg_div->SetName(("Energy_" + energy.second).data());

		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(divs_bes1, bes1["divide"][energy.first].val, divs_err, bes1["divide"][energy.first].stat);
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(divs_ampt, ampt["divide"][energy.first].val, divs_err, ampt["divide"][energy.first].stat);
		TGraphErrors* cf_ratio_div_def_g = graph_x_vs_y_err(divs_cf, cf["divide"][energy.first].val, divs_err, cf["divide"][energy.first].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + energy.second).data());
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + energy.second).data());
		cf_ratio_div_def_g->SetNameTitle(("cf_div_" + energy.second).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(divs_bes1, bes1["divide"][energy.first].val, divs_err, bes1["divide"][energy.first].sys);
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(divs_ampt, ampt["divide"][energy.first].val, divs_err, ampt["divide"][energy.first].sys);
		TGraphErrors* cf_ratio_div_sys_g = graph_x_vs_y_err(divs_cf, cf["divide"][energy.first].val, divs_err, cf["divide"][energy.first].sys);

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

		cf_ratio_div_def_g->SetMarkerStyle(marker_style["cf"]);
		cf_ratio_div_def_g->SetMarkerColor(marker_color["cf"]);
		cf_ratio_div_def_g->SetLineColor(marker_color["cf"]);
		cf_ratio_div_def_g->SetMarkerSize(marker_size["cf"]);
		cf_ratio_div_sys_g->SetLineColor(marker_color["cf"]);

		mg_div->Add(cf_ratio_div_def_g, "APZ");
		mg_div->Add(cf_ratio_div_sys_g, "[]");
		mg_div->Add(ampt_ratio_div_def_g, "P");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");
		mg_div->Add(bes1_ratio_div_def_g, "P");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");

		mg_div->GetXaxis()->SetLimits(15, 345);
		mg_div->GetXaxis()->SetRangeUser(15, 345);
		mg_div->GetXaxis()->SetLabelFont(43);
		mg_div->GetXaxis()->SetTitleFont(43);
		mg_div->GetXaxis()->SetLabelSize(def_text_size);
		mg_div->GetXaxis()->SetTitleSize(def_text_size);
		mg_div->GetYaxis()->SetLabelFont(43);
		mg_div->GetYaxis()->SetTitleFont(43);
		mg_div->GetYaxis()->SetLabelSize(def_text_size);
		mg_div->GetYaxis()->SetTitleSize(def_text_size);
		mg_div->GetYaxis()->SetDecimals(3);
		mg_div->GetXaxis()->SetTitleOffset(1.9);
		mg_div->GetYaxis()->SetTitleOffset(4.4);
		if (!zoom) {
			mg_div->GetYaxis()->SetLimits(0.9935, 1.0185);
			mg_div->GetYaxis()->SetRangeUser(0.9935, 1.0185);
		}
		else {
			mg_div->GetYaxis()->SetLimits(0.9975, 1.0035);
			mg_div->GetYaxis()->SetRangeUser(0.9975, 1.0035);
		}

		mg_div->GetXaxis()->SetTitle("Bin Width");
		mg_div->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mix}");

		mg_div->Draw("AP");


		TLatex energy_label;
		energy_label.SetTextFont(43);
		energy_label.SetTextAlign(22);
		energy_label.SetTextSize(25);
		energy_label.DrawLatex(260, 1.017, (energy.second + " GeV").data());


		TLine* one_line = new TLine(15, 1, 345, 1);
		one_line->SetLineStyle(2);
		one_line->Draw();

		if (energy.first == 11) {
			float y1 = 0.5, y2 = 0.7;
			if (type_name == "Ratio") { y1 = 0.7; y2 = 0.95; }
			else if (type_name == "Pull") { y1 = 0.7; y2 = 0.95; }
			TLegend *leg2 = new TLegend(0.0, y1, 1.0, y2);
			leg2->AddEntry(bes1_ratio_div_def_g, "BES1", "p");
			leg2->AddEntry(ampt_ratio_div_def_g, "AMPT", "p");
			leg2->AddEntry(cf_ratio_div_def_g, "MUSIC + FIST", "p");
			leg2->SetBorderSize(0);
			leg2->SetFillStyle(0);
			leg2->Draw();
		}
	}

	can_multi->Update();
	can_multi->Write();

	delete can_multi;


//	map<int, int> div_color{ {60, kBlack}, {72, kRed}, {90, kGreen+3}, {120, kBlue}, {180, kViolet}, {240, kCyan + 2}, {270, kGreen + 1}, {300, kGray + 2} };
//	map<string, int> marker_style{ {"bes", 20}, {"ampt", 24}, {"cf", 24} };
//	map<int, int> div_marker_size{ {60, 2}, {72, 2}, {90, 2}, {120, 2}, {180, 2}, {240, 2}, {270, 2}, {300, 2} };

//	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_divs_can_" + type_name).data(), ("Kurtosis vs Energy with Bin Width " + type_name).data(), 955, 805);
//	TMultiGraph* mg = new TMultiGraph();
//	mg->SetName("Kurtosis_vs_energy_mg");
//	TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
//	for ( pair<int, plot_data> div : bes1) {
//		TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1[div.first].val, energy_err, bes1[div.first].stat);
//		TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt[div.first].val, energy_err, ampt[div.first].stat);
//		bes1_ratio_def_g->SetNameTitle(("bes1_"+to_string(div.first)).data());
//		ampt_ratio_def_g->SetNameTitle(("ampt_" + to_string(div.first)).data());
//		TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1[div.first].val, energy_err, bes1[div.first].sys);
//		TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt[div.first].val, energy_err, ampt[div.first].sys);
//
//		bes1_ratio_def_g->SetMarkerStyle(marker_style["bes"]);
//		bes1_ratio_def_g->SetMarkerColor(div_color[div.first]);
//		bes1_ratio_def_g->SetLineColor(div_color[div.first]);
//		bes1_ratio_def_g->SetMarkerSize(div_marker_size[div.first]);
//		bes1_ratio_sys_g->SetLineColor(div_color[div.first]);
//
//		ampt_ratio_def_g->SetMarkerStyle(marker_style["ampt"]);
//		ampt_ratio_def_g->SetMarkerColor(div_color[div.first]);
//		ampt_ratio_def_g->SetLineColor(div_color[div.first]);
//		ampt_ratio_def_g->SetMarkerSize(div_marker_size[div.first]);
//		ampt_ratio_sys_g->SetLineColor(div_color[div.first]);
//
//		mg->Add(ampt_ratio_def_g, "APZ");
//		mg->Add(ampt_ratio_sys_g, "[]");
//		mg->Add(bes1_ratio_def_g, "P");
//		mg->Add(bes1_ratio_sys_g, "[]");
//
//		leg->AddEntry(bes1_ratio_def_g, ("BES1 " + to_string(div.first) + "#circ").data(), "p");
//		leg->AddEntry(ampt_ratio_def_g, ("AMPT " +  to_string(div.first) + "#circ").data(), "p");
//	}
//
//	leg->SetBorderSize(0);
//	leg->SetFillStyle(0);
//
//	mg->GetXaxis()->SetLimits(-2, 78);
//	mg->GetXaxis()->SetRangeUser(-2, 78);
//	if (!zoom) {
//		mg->GetYaxis()->SetLimits(0.9935, 1.0185);
//		mg->GetYaxis()->SetRangeUser(0.9935, 1.0185);
//	}
//
//	mg->GetXaxis()->SetTitle("Energy (GeV)");
//	mg->GetYaxis()->SetTitle("Kurtosis");
//
//	mg->Draw("AP");
//
//	TLine* one_line = new TLine(0, 1, 80, 1);
//	one_line->SetLineStyle(2);
//	one_line->Draw();
//
//	leg->SetMargin(0.1); leg->Draw();
//
//	gPad->SetTopMargin(0.04);
//	gPad->SetLeftMargin(0.12);
//	gPad->SetRightMargin(0.04);
//
//	can->Update();
//	can->Write();
//
//	delete can;

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

	vector<int> cent_order { 8, 7, 6, 5, 4, 3, 2, 1 };

	map<int, int> cent_color{ {8, kBlack}, {7, kBlue}, {6, kRed}, {5, kGreen + 3}, {4, kViolet}, {3, kOrange + 2}, {2, kCyan + 2}, {1, kYellow + 3} };
//	map<string, int> marker_style{ {"bes", 29}, {"ampt", 22} };
	map<string, int> marker_style{ {"bes", 20}, {"ampt", 24} };
	map<string, int> marker_size{ {"bes", 2}, {"ampt", 2} };
	map<string, int> marker_color{ {"bes", kBlack}, {"ampt", kRed} };
	map<int, int> cent_marker_size{ {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2} };
	map<int, string> cent_string{ {8, "0-5%"}, {7, "5-10%"}, {6, "10-20%"}, {5, "20-30%"}, {4, "30-40%"}, {3, "40-50%"}, {2, "50-60%"}, {1, "60-70%"} };


	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_cents_can_" + type_name).data(), ("Kurtosis vs Energy with Centrality " + type_name).data(), 955, 805);

	int x_num = 3;
	int y_num = 3;

	double y_top_pad = 0.01;
	double y_bot_pad = 0.06;
	double x_left_pad = 0.075;
	double x_right_pad = 0.01;
	double x_low = 0;
	double x_up = 0;
	double y_low = 0;
	double y_up = 1.0;
	double def_left_margin = 0.0;
	double def_right_margin = 0.0;
	double def_top_margin = 0.0;
	double def_bot_margin = 0.0;

	double x_step = (1.0 - x_left_pad - x_right_pad) / x_num;
	double y_step = (1.0 - y_top_pad - y_bot_pad) / y_num;
//	cout << "x_step: " << x_step << "  |  y_step: " << y_step << endl;
	for (int y_index=0; y_index<y_num; y_index++) {
		x_low = 0;
		if (y_index == 0) { y_low = y_up - y_step - y_top_pad; }
		else if (y_index == y_num - 1) { y_low = y_up - y_step - y_bot_pad; }
		else { y_low = y_up - y_step; }

		for (int x_index=0; x_index<x_num; x_index++) {
			int pad_index = x_index + x_num * y_index + 1;

			if (x_index == 0) { x_up = x_low + x_step + x_left_pad; }
			else if (x_index == x_num - 1) { x_up = x_low + x_step + x_right_pad; }
			else { x_up = x_low + x_step; }

			if (y_up > 1.0) { y_up = 1.0;}
			if (y_low < 0.0) { y_low = 0.0; }
			if (x_up > 1.0) { x_up = 1.0;}
			if (x_low < 0.0) { x_low = 0.0; }

//			cout << "x_index: " << x_index << " x_low: " << x_low << " x_up: " << x_up << endl;
//			cout << "y_index: " << y_index << " y_low: " << y_low << " y_up: " << y_up << endl;

			string pad_name = gPad->GetName() + to_string(pad_index);
			TPad *pad = new TPad(pad_name.data(), pad_name.data(), x_low, y_low, x_up, y_up);
			pad->SetTickx();
			pad->SetTicky();

			if (x_index == 0) {	pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin); }
			else { pad->SetLeftMargin(def_left_margin); }
			if (x_index == x_num - 1) {	pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin); }
			else { pad->SetRightMargin(def_right_margin); }
			if (y_index == 0) { pad->SetTopMargin(y_top_pad / (y_up - y_low) + def_top_margin); }
			else { pad->SetTopMargin(def_top_margin); }
			if (y_index == y_num -1) { pad->SetBottomMargin(y_bot_pad / (y_up - y_low) + def_bot_margin); }
			else { pad->SetBottomMargin(def_bot_margin); }

			pad->SetNumber(pad_index);

			pad->Draw();
			x_low = x_up;
		}
		y_up = y_low;
	}
	int can_index = 0;

	for (int cent : cent_order) {
//	for (pair<int, plot_data> cent : bes1) {
		can->cd(++can_index);
		TMultiGraph* mg = new TMultiGraph();
		mg->SetName(("Kurtosis_vs_energy_mg_" + to_string(cent)).data());
//		mg->SetTitle((cent_string[cent]).data());

		TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1[cent].val, energy_err, bes1[cent].stat);
		TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt[cent].val, energy_err, ampt[cent].stat);
		bes1_ratio_def_g->SetNameTitle(("bes1_" + to_string(cent)).data());
		ampt_ratio_def_g->SetNameTitle(("ampt_" + to_string(cent)).data());
		TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1[cent].val, energy_err, bes1[cent].sys);
		TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt[cent].val, energy_err, ampt[cent].sys);

		bes1_ratio_def_g->SetMarkerStyle(marker_style["bes"]);
		bes1_ratio_def_g->SetMarkerColor(marker_color["bes"]);
		bes1_ratio_def_g->SetLineColor(marker_color["bes"]);
		bes1_ratio_def_g->SetMarkerSize(marker_size["bes"]);
		bes1_ratio_sys_g->SetLineColor(marker_color["bes"]);

		ampt_ratio_def_g->SetMarkerStyle(marker_style["ampt"]);
		ampt_ratio_def_g->SetMarkerColor(marker_color["ampt"]);
		ampt_ratio_def_g->SetLineColor(marker_color["ampt"]);
		ampt_ratio_def_g->SetMarkerSize(marker_size["ampt"]);
		ampt_ratio_sys_g->SetLineColor(marker_color["ampt"]);

		mg->Add(ampt_ratio_def_g, "APZ");
		mg->Add(ampt_ratio_sys_g, "[]");
		mg->Add(bes1_ratio_def_g, "P");
		mg->Add(bes1_ratio_sys_g, "[]");

		mg->GetXaxis()->SetLimits(-2, 78);
		mg->GetXaxis()->SetRangeUser(-2, 78);
		mg->GetXaxis()->SetLabelFont(43);
		mg->GetXaxis()->SetTitleFont(43);
		mg->GetXaxis()->SetLabelSize(def_text_size);
		mg->GetXaxis()->SetTitleSize(def_text_size);
		mg->GetYaxis()->SetLabelFont(43);
		mg->GetYaxis()->SetTitleFont(43);
		mg->GetYaxis()->SetLabelSize(def_text_size);
		mg->GetYaxis()->SetTitleSize(def_text_size);
		mg->GetYaxis()->SetDecimals(3);
		mg->GetXaxis()->SetTitleOffset(2.9);
		mg->GetYaxis()->SetTitleOffset(4.4);
		if (!zoom) {
			mg->GetYaxis()->SetLimits(0.9935, 1.0185);
			mg->GetYaxis()->SetRangeUser(0.9935, 1.0185);
		}

		mg->GetXaxis()->SetTitle("Energy (GeV)");
		mg->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mix}");

		mg->Draw("AP");

		TLine* one_line = new TLine(0, 1, 80, 1);
		one_line->SetLineStyle(2);
		one_line->Draw();

		TLatex cent_label;
		cent_label.SetTextFont(43);
		cent_label.SetTextAlign(22);
		cent_label.SetTextSize(25);
		cent_label.DrawLatex(65, 0.9965, (cent_string[cent]).data());


		if (can_index == 8) {
			can->cd(++can_index);
			vector<double> x_hold {-1};
			vector<double> y_hold {-1};
			TGraph *g_hold = new TGraph(1, x_hold.data(), y_hold.data());
			g_hold->SetTitle("");
			g_hold->GetXaxis()->SetLimits(-2, 78);
			g_hold->GetXaxis()->SetRangeUser(-2, 78);
			g_hold->GetXaxis()->SetTitle("Energy (GeV)");
			g_hold->GetXaxis()->SetTitleFont(43);
			g_hold->GetXaxis()->SetLabelFont(43);
			g_hold->GetXaxis()->SetTitleSize(def_text_size);
			g_hold->GetXaxis()->SetLabelSize(def_text_size);
			g_hold->GetXaxis()->SetTitleOffset(2.9);
			if (!zoom) {
				g_hold->GetYaxis()->SetLimits(0.9935, 1.0185);
				g_hold->GetYaxis()->SetRangeUser(0.9935, 1.0185);
			} else {
				g_hold->GetYaxis()->SetLimits(0.9975, 1.0035);
				g_hold->GetYaxis()->SetRangeUser(0.9975, 1.0035);
			}
			g_hold->Draw("AP");
			TLegend *leg2 = new TLegend(0.3, 0.4, 0.7, 0.6);
			leg2->AddEntry(bes1_ratio_def_g, "BES1", "p");
			leg2->AddEntry(ampt_ratio_def_g, "AMPT", "p");
			leg2->SetBorderSize(0);
			leg2->SetFillStyle(0);
			leg2->Draw();
		}

	}

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

		bes1_ratio_def_g->SetMarkerStyle(marker_style["bes"]);
		bes1_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetLineColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		bes1_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		mg_bes->Add(bes1_ratio_def_g, "APZ");
		mg_bes->Add(bes1_ratio_sys_g, "[]");

		leg_bes->AddEntry(bes1_ratio_def_g, ("BES1 " + cent_string[cent.first]).data(), "p");
	}

	leg_bes->SetBorderSize(0);
	leg_bes->SetFillStyle(0);

	mg_bes->GetXaxis()->SetLimits(-2, 78);
	mg_bes->GetXaxis()->SetRangeUser(-2, 78);
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

		ampt_ratio_def_g->SetMarkerStyle(marker_style["ampt"]);
		ampt_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetLineColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		ampt_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		mg_ampt->Add(ampt_ratio_def_g, "P");
		mg_ampt->Add(ampt_ratio_sys_g, "[]");

		leg_ampt->AddEntry(ampt_ratio_def_g, ("AMPT " + cent_string[cent.first]).data(), "p");
	}

	leg_ampt->SetBorderSize(0);
	leg_ampt->SetFillStyle(0);

	mg_ampt->GetXaxis()->SetLimits(-2, 78);
	mg_ampt->GetXaxis()->SetRangeUser(-2, 78);
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


void AzFigurePlotter::sd_vs_energy_cents_plot(vector<int> energies, map<int, plot_data> bes1, map<int, plot_data> ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	for (double energy : energies) {
		energy_bes1.push_back(energy - 0.6);
		energy_ampt.push_back(energy + 0.6);
	}

	vector<int> cent_order { 8, 7, 6, 5, 4, 3, 2, 1 };

	map<int, int> cent_color{ {8, kBlack}, {7, kBlue}, {6, kRed}, {5, kGreen + 3}, {4, kViolet}, {3, kOrange + 2}, {2, kCyan + 2}, {1, kYellow + 3} };
	map<string, int> marker_style{ {"bes", 29}, {"ampt", 22} };
	map<string, int> marker_size{ {"bes", 2}, {"ampt", 2} };
	map<string, int> marker_color{ {"bes", kBlack}, {"ampt", kRed} };
	map<int, int> cent_marker_size{ {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2} };
	map<int, string> cent_string{ {8, "0-5%"}, {7, "5-10%"}, {6, "10-20%"}, {5, "20-30%"}, {4, "30-40%"}, {3, "40-50%"}, {2, "50-60%"}, {1, "60-70%"} };


	TCanvas* can = new TCanvas(("SD_vs_energy_cents_can_" + type_name).data(), ("SD vs Energy with Centrality " + type_name).data(), 955, 805);

	int x_num = 3;
	int y_num = 3;

	double y_top_pad = 0.01;
	double y_bot_pad = 0.04;
	double x_left_pad = 0.045;
	double x_right_pad = 0.01;
	double x_low = 0;
	double x_up = 0;
	double y_low = 0;
	double y_up = 1.0;
	double def_left_margin = 0.0;
	double def_right_margin = 0.0;
	double def_top_margin = 0.0;
	double def_bot_margin = 0.0;

	double x_step = (1.0 - x_left_pad - x_right_pad) / x_num;
	double y_step = (1.0 - y_top_pad - y_bot_pad) / y_num;
//	cout << "x_step: " << x_step << "  |  y_step: " << y_step << endl;
	for (int y_index=0; y_index<y_num; y_index++) {
		x_low = 0;
		if (y_index == 0) { y_low = y_up - y_step - y_top_pad; }
		else if (y_index == y_num - 1) { y_low = y_up - y_step - y_bot_pad; }
		else { y_low = y_up - y_step; }

		for (int x_index=0; x_index<x_num; x_index++) {
			int pad_index = x_index + x_num * y_index + 1;

			if (x_index == 0) { x_up = x_low + x_step + x_left_pad; }
			else if (x_index == x_num - 1) { x_up = x_low + x_step + x_right_pad; }
			else { x_up = x_low + x_step; }

			if (y_up > 1.0) { y_up = 1.0;}
			if (y_low < 0.0) { y_low = 0.0; }
			if (x_up > 1.0) { x_up = 1.0;}
			if (x_low < 0.0) { x_low = 0.0; }

//			cout << "x_index: " << x_index << " x_low: " << x_low << " x_up: " << x_up << endl;
//			cout << "y_index: " << y_index << " y_low: " << y_low << " y_up: " << y_up << endl;

			string pad_name = gPad->GetName() + to_string(pad_index);
			TPad *pad = new TPad(pad_name.data(), pad_name.data(), x_low, y_low, x_up, y_up);
			pad->SetTickx();
			pad->SetTicky();

			if (x_index == 0) {	pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin); }
			else { pad->SetLeftMargin(def_left_margin); }
			if (x_index == x_num - 1) {	pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin); }
			else { pad->SetRightMargin(def_right_margin); }
			if (y_index == 0) { pad->SetTopMargin(y_top_pad / (y_up - y_low) + def_top_margin); }
			else { pad->SetTopMargin(def_top_margin); }
			if (y_index == y_num -1) { pad->SetBottomMargin(y_bot_pad / (y_up - y_low) + def_bot_margin); }
			else { pad->SetBottomMargin(def_bot_margin); }

			pad->SetNumber(pad_index);

			pad->Draw();
			x_low = x_up;
		}
		y_up = y_low;
	}
	int can_index = 0;

	for (int cent : cent_order) {
//	for (pair<int, plot_data> cent : bes1) {
		can->cd(++can_index);
		TMultiGraph* mg = new TMultiGraph();
		mg->SetName(("SD_vs_energy_mg_" + to_string(cent)).data());
//		mg->SetTitle((cent_string[cent]).data());

		TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1[cent].val, energy_err, bes1[cent].stat);
		TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt[cent].val, energy_err, ampt[cent].stat);
		bes1_ratio_def_g->SetNameTitle(("bes1_" + to_string(cent)).data());
		ampt_ratio_def_g->SetNameTitle(("ampt_" + to_string(cent)).data());
		TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1[cent].val, energy_err, bes1[cent].sys);
		TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt[cent].val, energy_err, ampt[cent].sys);

		bes1_ratio_def_g->SetMarkerStyle(marker_style["bes"]);
		bes1_ratio_def_g->SetMarkerColor(marker_color["bes"]);
		bes1_ratio_def_g->SetLineColor(marker_color["bes"]);
		bes1_ratio_def_g->SetMarkerSize(marker_size["bes"]);
		bes1_ratio_sys_g->SetLineColor(marker_color["bes"]);

		ampt_ratio_def_g->SetMarkerStyle(marker_style["ampt"]);
		ampt_ratio_def_g->SetMarkerColor(marker_color["ampt"]);
		ampt_ratio_def_g->SetLineColor(marker_color["ampt"]);
		ampt_ratio_def_g->SetMarkerSize(marker_size["ampt"]);
		ampt_ratio_sys_g->SetLineColor(marker_color["ampt"]);

		mg->Add(ampt_ratio_def_g, "APZ");
		mg->Add(ampt_ratio_sys_g, "[]");
		mg->Add(bes1_ratio_def_g, "P");
		mg->Add(bes1_ratio_sys_g, "[]");

		mg->GetXaxis()->SetLimits(-2, 78);
		mg->GetXaxis()->SetRangeUser(-2, 78);
		if (!zoom) {
			mg->GetYaxis()->SetLimits(0.948, 1.002);
			mg->GetYaxis()->SetRangeUser(0.948, 1.002);
		}
		mg->GetYaxis()->SetDecimals(3);

		mg->GetXaxis()->SetTitle("Energy (GeV)");
		mg->GetYaxis()->SetTitle("Standard Deviation_{Raw}  /  Standard Deviation_{<ix}");

		mg->Draw("AP");

		TLine* one_line = new TLine(0, 1, 80, 1);
		one_line->SetLineStyle(2);
		one_line->Draw();

		TLatex cent_label;
		cent_label.SetTextFont(43);
		cent_label.SetTextAlign(22);
		cent_label.SetTextSize(25);
		cent_label.DrawLatex(65, 0.955, (cent_string[cent]).data());


		if (can_index == 8) {
			can->cd(++can_index);
			vector<double> x_hold {-1};
			vector<double> y_hold {-1};
			TGraph *g_hold = new TGraph(1, x_hold.data(), y_hold.data());
			g_hold->SetTitle("");
			g_hold->GetXaxis()->SetLimits(-2, 78);
			g_hold->GetXaxis()->SetRangeUser(-2, 78);
			if (!zoom) {
				g_hold->GetYaxis()->SetLimits(0.948, 1.0);
				g_hold->GetYaxis()->SetRangeUser(0.948, 1.0);
			} else {
				g_hold->GetYaxis()->SetLimits(0.948, 1.0);
				g_hold->GetYaxis()->SetRangeUser(0.948, 1.0);
			}
			g_hold->Draw("AP");
			TLegend *leg2 = new TLegend(0.3, 0.4, 0.7, 0.6);
			leg2->AddEntry(bes1_ratio_def_g, "BES1", "p");
			leg2->AddEntry(ampt_ratio_def_g, "AMPT", "p");
			leg2->SetBorderSize(0);
			leg2->SetFillStyle(0);
			leg2->Draw();
		}

	}

	can->Update();
	can->Write();

	delete can;


	// Just BES1
	TCanvas* can_bes = new TCanvas(("BES1_SD_vs_energy_cents_can_" + type_name).data(), ("SD vs Energy with Centrality " + type_name).data(), 955, 805);
	TMultiGraph* mg_bes = new TMultiGraph();
	mg_bes->SetName("SD_vs_energy_mg");
	TLegend* leg_bes = new TLegend(0.3, 0.21, 0.3, 0.21);
	for (pair<int, plot_data> cent : bes1) {
		TGraphErrors* bes1_ratio_def_g = graph_x_vs_y_err(energy_bes1, bes1[cent.first].val, energy_err, bes1[cent.first].stat);
		bes1_ratio_def_g->SetNameTitle(("bes1_" + to_string(cent.first)).data());
		TGraphErrors* bes1_ratio_sys_g = graph_x_vs_y_err(energy_bes1, bes1[cent.first].val, energy_err, bes1[cent.first].sys);

		bes1_ratio_def_g->SetMarkerStyle(marker_style["bes"]);
		bes1_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetLineColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		bes1_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		mg_bes->Add(bes1_ratio_def_g, "APZ");
		mg_bes->Add(bes1_ratio_sys_g, "[]");

		leg_bes->AddEntry(bes1_ratio_def_g, ("BES1 " + cent_string[cent.first]).data(), "p");
	}

	leg_bes->SetBorderSize(0);
	leg_bes->SetFillStyle(0);

	mg_bes->GetXaxis()->SetLimits(-2, 78);
	mg_bes->GetXaxis()->SetRangeUser(-2, 78);
	if (!zoom) {
		mg_bes->GetYaxis()->SetLimits(0.948, 1.0);
		mg_bes->GetYaxis()->SetRangeUser(0.948, 1.0);
	}
	mg_bes->GetYaxis()->SetDecimals(3);

	mg_bes->GetXaxis()->SetTitle("Energy (GeV)");
	mg_bes->GetYaxis()->SetTitle("Standard Deviation");

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
	TCanvas* can_ampt = new TCanvas(("AMPT_SD_vs_energy_cents_can_" + type_name).data(), ("SD vs Energy with Centrality " + type_name).data(), 955, 805);
	TMultiGraph* mg_ampt = new TMultiGraph();
	mg_ampt->SetName("SD_vs_energy_mg");
	TLegend* leg_ampt = new TLegend(0.3, 0.21, 0.3, 0.21);
	for (pair<int, plot_data> cent : bes1) {
		TGraphErrors* ampt_ratio_def_g = graph_x_vs_y_err(energy_ampt, ampt[cent.first].val, energy_err, ampt[cent.first].stat);
		ampt_ratio_def_g->SetNameTitle(("ampt_" + to_string(cent.first)).data());
		TGraphErrors* ampt_ratio_sys_g = graph_x_vs_y_err(energy_ampt, ampt[cent.first].val, energy_err, ampt[cent.first].sys);

		ampt_ratio_def_g->SetMarkerStyle(marker_style["ampt"]);
		ampt_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetLineColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		ampt_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		mg_ampt->Add(ampt_ratio_def_g, "APZ");
		mg_ampt->Add(ampt_ratio_sys_g, "[]");

		leg_ampt->AddEntry(ampt_ratio_def_g, ("AMPT " + cent_string[cent.first]).data(), "p");
	}

	leg_ampt->SetBorderSize(0);
	leg_ampt->SetFillStyle(0);

	mg_ampt->GetXaxis()->SetLimits(-2, 78);
	mg_ampt->GetXaxis()->SetRangeUser(-2, 78);
	if (!zoom) {
		mg_ampt->GetYaxis()->SetLimits(0.948, 1.0);
		mg_ampt->GetYaxis()->SetRangeUser(0.948, 1.0);
	}
	mg_ampt->GetYaxis()->SetDecimals(3);

	mg_ampt->GetXaxis()->SetTitle("Energy (GeV)");
	mg_ampt->GetYaxis()->SetTitle("Standard Deviation");

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

	map<string, int> color{ {"raw_bes", kBlue}, {"raw_ampt", kBlue}, {"mix_bes", kGreen+2}, {"mix_ampt", kGreen+2}, {"div_bes", kBlack}, {"div_ampt", kRed} };
	map<string, int> marker_size{ {"raw", 2}, {"mix", 2}, {"divide", 2} };
//	map<string, int> marker_style{ {"raw_bes", 29}, {"raw_ampt", 22}, {"mix_bes", 30}, {"mix_ampt", 26}, {"div_bes", 29}, {"div_ampt", 22} };
	map<string, int> marker_style{ {"raw_bes", 20}, {"raw_ampt", 24}, {"mix_bes", 20}, {"mix_ampt", 24}, {"div_bes", 20}, {"div_ampt", 24} };
	map<string, string> stat_title{ {"mean", "Mean"}, {"standard_deviation", "Standard Deviation"}, {"skewness", "Skewness"}, {"non_excess_kurtosis", "Kurtosis"} };
//	int bes_marker_style = 29;
//	int ampt_marker_style = 22;

	TCanvas* can = new TCanvas(("Moments_vs_energy_can_" + type_name).data(), ("Moments vs Energy " + type_name).data(), 1250, 800);
	int stat_num = (int)stat_order.size();

	double y_top_pad = 0.01;
	double y_bot_pad = 0.1;
	double x_left_pad = 0.012;
	double x_right_pad = 0.01;
	double y_top_length = 0.6;
	double x_low = 0;
	double x_up = 0;
	double def_left_margin = 0.12;
	double def_right_margin = 0.01;

	double x_step = (1.0 - x_left_pad - x_right_pad) / stat_num;
//	cout << "x_step: " << x_step << endl;
	for (int x_index=0; x_index<stat_num; x_index++) {
		if (x_index == 0) { x_up = x_low + x_step + x_left_pad; }
		else if (x_index == stat_num - 1) { x_up = x_low + x_step + x_right_pad; }
		else { x_up = x_low + x_step; }
//		cout << "x_index: " << x_index << " x_low: " << x_low << " x_up: " << x_up << endl;

		int top_index = x_index * 2 + 1;
		string name_top = gPad->GetName() + to_string(top_index);
		TPad *top_pad = new TPad(name_top.data(), name_top.data(), x_low, 1 - y_top_length, x_up, 1.0);
		top_pad->SetTopMargin(y_top_pad);
		top_pad->SetBottomMargin(0);

		int bottom_index = x_index * 2 + 2;
		string name_bottom = gPad->GetName() + to_string(bottom_index);
		TPad *bottom_pad = new TPad(name_top.data(), name_top.data(), x_low, 0, x_up, 1 - y_top_length);
		bottom_pad->SetTickx();
		bottom_pad->SetBottomMargin(y_bot_pad);
		bottom_pad->SetTopMargin(0);

		if (x_index == 0) {
			top_pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin);
			bottom_pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin);
		} else { top_pad->SetLeftMargin(def_left_margin); bottom_pad->SetLeftMargin(def_left_margin); }
		if (x_index == stat_num - 1) {
			top_pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin);
			bottom_pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin);
		} else { top_pad->SetRightMargin(def_right_margin); bottom_pad->SetRightMargin(def_right_margin); }

		top_pad->SetNumber(top_index);
		bottom_pad->SetNumber(bottom_index);

		top_pad->Draw();
		bottom_pad->Draw();
		x_low = x_up;
	}

	int can_index = 1;
	for ( string stat : stat_order) { //pair<string, plot_data> stat : bes1["raw"]) {  // Iterate over stats, left to right on plot

		// Raw vs mixed on top plot
		can->cd(can_index * 2 - 1);

		TMultiGraph* mg = new TMultiGraph();
		mg->SetName((stat + "_raw_mixed").data());
//		mg->SetTitle((stat_title[stat]).data());

		TGraphErrors* bes1_ratio_raw_def_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][stat].val, energy_err, bes1["raw"][stat].stat);
		TGraphErrors* ampt_ratio_raw_def_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][stat].val, energy_err, ampt["raw"][stat].stat);
		bes1_ratio_raw_def_g->SetNameTitle(("bes1_raw_" + stat).data());
		ampt_ratio_raw_def_g->SetNameTitle(("ampt_raw_" + stat).data());
		TGraphErrors* bes1_ratio_raw_sys_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][stat].val, energy_err, bes1["raw"][stat].sys);
		TGraphErrors* ampt_ratio_raw_sys_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][stat].val, energy_err, ampt["raw"][stat].sys);

		bes1_ratio_raw_def_g->SetMarkerStyle(marker_style["raw_bes"]);
		bes1_ratio_raw_def_g->SetMarkerColor(color["raw_bes"]);
		bes1_ratio_raw_def_g->SetLineColor(color["raw_bes"]);
		bes1_ratio_raw_def_g->SetMarkerSize(marker_size["raw"]);
		bes1_ratio_raw_sys_g->SetLineColor(color["raw_bes"]);

		ampt_ratio_raw_def_g->SetMarkerStyle(marker_style["raw_ampt"]);
		ampt_ratio_raw_def_g->SetMarkerColor(color["raw_ampt"]);
		ampt_ratio_raw_def_g->SetLineColor(color["raw_ampt"]);
		ampt_ratio_raw_def_g->SetMarkerSize(marker_size["raw"]);
		ampt_ratio_raw_sys_g->SetLineColor(color["raw_ampt"]);

		TGraphErrors* bes1_ratio_mix_def_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][stat].val, energy_err, bes1["mix"][stat].stat);
		TGraphErrors* ampt_ratio_mix_def_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][stat].val, energy_err, ampt["mix"][stat].stat);
		bes1_ratio_mix_def_g->SetNameTitle(("bes1_mix_" + stat).data());
		ampt_ratio_mix_def_g->SetNameTitle(("ampt_mix_" + stat).data());
		TGraphErrors* bes1_ratio_mix_sys_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][stat].val, energy_err, bes1["mix"][stat].sys);
		TGraphErrors* ampt_ratio_mix_sys_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][stat].val, energy_err, ampt["mix"][stat].sys);

		bes1_ratio_mix_def_g->SetMarkerStyle(marker_style["mix_bes"]);
		bes1_ratio_mix_def_g->SetMarkerColor(color["mix_bes"]);
		bes1_ratio_mix_def_g->SetLineColor(color["mix_bes"]);
		bes1_ratio_mix_def_g->SetMarkerSize(marker_size["mix"]);
		bes1_ratio_mix_sys_g->SetLineColor(color["mix_bes"]);

		ampt_ratio_mix_def_g->SetMarkerStyle(marker_style["mix_ampt"]);
		ampt_ratio_mix_def_g->SetMarkerColor(color["mix_ampt"]);
		ampt_ratio_mix_def_g->SetLineColor(color["mix_ampt"]);
		ampt_ratio_mix_def_g->SetMarkerSize(marker_size["mix"]);
		ampt_ratio_mix_sys_g->SetLineColor(color["mix_ampt"]);

		mg->Add(ampt_ratio_mix_def_g, "APZ");
		mg->Add(ampt_ratio_mix_sys_g, "[]");
		mg->Add(ampt_ratio_raw_def_g, "P");
		mg->Add(ampt_ratio_raw_sys_g, "[]");
		mg->Add(bes1_ratio_mix_def_g, "P");
		mg->Add(bes1_ratio_mix_sys_g, "[]");
		mg->Add(bes1_ratio_raw_def_g, "P");
		mg->Add(bes1_ratio_raw_sys_g, "[]");

//		gStyle->SetTitleFont(3, "t");
//		gStyle->SetTitleSize(20, "t");
		mg->GetXaxis()->SetLimits(-2, 78);
		mg->GetXaxis()->SetRangeUser(-2, 78);
		mg->GetXaxis()->SetLabelFont(43);
		mg->GetXaxis()->SetLabelSize(def_text_size);
		mg->GetYaxis()->SetLabelFont(43);
		mg->GetYaxis()->SetLabelSize(def_text_size);
		if (stat == "mean") { mg->GetYaxis()->SetDecimals(5); }
		else { mg->GetYaxis()->SetDecimals(2); }
		//mg->GetYaxis()->SetLimits(0.9955, 1.0145);
		//mg->GetYaxis()->SetRangeUser(0.9955, 1.0145);

		mg->GetXaxis()->SetTitle("Energy (GeV)");
		mg->GetXaxis()->SetTitleFont(43);
		mg->GetXaxis()->SetTitleSize(def_text_size);

		mg->Draw("AP");

		TLegend* leg = new TLegend(0.6, 0.7, 0.9, 0.9);
		leg->SetTextFont(43);
		leg->SetTextSize(def_text_size);
		leg->AddEntry(bes1_ratio_raw_def_g, "STAR Raw", "p");
		leg->AddEntry(bes1_ratio_mix_def_g, "STAR Mixed", "p");
		leg->AddEntry(ampt_ratio_raw_def_g, "AMPT Raw", "p");
		leg->AddEntry(ampt_ratio_mix_def_g, "AMPT Mixed", "p");
		leg->SetBorderSize(0);
		leg->SetFillStyle(0);
		if (stat == "mean") { leg->Draw(); }

		TText *title = new TText(0.55, 0.965, (stat_title[stat]).data());
		title->SetNDC();
		title->SetTextFont(43);
		title->SetTextSize(20);
		title->SetTextAlign(22);
		title->Draw();

		// Divided plot on bottom
		can->cd(can_index * 2);

		TMultiGraph* mg_div = new TMultiGraph();
		mg_div->SetName((stat + "_divided").data());

		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][stat].val, energy_err, bes1["divide"][stat].stat);
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][stat].val, energy_err, ampt["divide"][stat].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + stat).data());
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + stat).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][stat].val, energy_err, bes1["divide"][stat].sys);
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][stat].val, energy_err, ampt["divide"][stat].sys);

		bes1_ratio_div_def_g->SetMarkerStyle(marker_style["div_bes"]);
		bes1_ratio_div_def_g->SetMarkerColor(color["div_bes"]);
		bes1_ratio_div_def_g->SetLineColor(color["div_bes"]);
		bes1_ratio_div_def_g->SetMarkerSize(marker_size["divide"]);
		bes1_ratio_div_sys_g->SetLineColor(color["div_bes"]);

		ampt_ratio_div_def_g->SetMarkerStyle(marker_style["div_ampt"]);
		ampt_ratio_div_def_g->SetMarkerColor(color["div_ampt"]);
		ampt_ratio_div_def_g->SetLineColor(color["div_ampt"]);
		ampt_ratio_div_def_g->SetMarkerSize(marker_size["divide"]);
		ampt_ratio_div_sys_g->SetLineColor(color["div_ampt"]);

		mg_div->Add(ampt_ratio_div_def_g, "APZ");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");
		mg_div->Add(bes1_ratio_div_def_g, "P");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");

		mg_div->GetXaxis()->SetLimits(-2, 78);
		mg_div->GetXaxis()->SetRangeUser(-2, 78);
		mg_div->GetXaxis()->SetLabelFont(43);
		mg_div->GetXaxis()->SetLabelSize(13);
		mg_div->GetYaxis()->SetLabelFont(43);
		mg_div->GetYaxis()->SetLabelSize(13);
		if (stat == "mean") { mg_div->GetYaxis()->SetDecimals(5); }
		else if (stat == "skewness") { mg_div->GetYaxis()->SetDecimals(2); }
		else { mg_div->GetYaxis()->SetDecimals(3); }
		//mg_div->GetYaxis()->SetLimits(0.9955, 1.0145);
		//mg_div->GetYaxis()->SetRangeUser(0.9955, 1.0145);

		mg_div->GetXaxis()->SetTitle("Energy (GeV)");
		mg_div->GetXaxis()->SetTitleFont(43);
		mg_div->GetXaxis()->SetTitleSize(13);
		mg_div->GetXaxis()->SetTitleOffset(3);

		mg_div->Draw("AP");


		TLegend* leg_div = new TLegend(0.5, 0.75, 0.9, 0.9);
		leg_div->SetTextFont(43);
		leg_div->SetTextSize(def_text_size);
		leg_div->AddEntry(bes1_ratio_div_def_g, "STAR Raw/Mixed", "p");
		leg_div->AddEntry(ampt_ratio_div_def_g, "AMPT Raw/Mixed", "p");
		leg_div->SetBorderSize(0);
		leg_div->SetFillStyle(0);
		if (stat == "mean") { leg_div->Draw(); }

		if (1 < mg_div->GetYaxis()->GetXmax() && 1 > mg_div->GetYaxis()->GetXmin()) {
			TLine* one_line = new TLine(0, 1, 80, 1);
			one_line->SetLineStyle(2);
			one_line->Draw();
		}

		can_index++;


		// Individual Canvases
		TCanvas *can_stat = new TCanvas((stat + "_vs_energy_can_" + type_name).data(), (stat + " vs Energy " + type_name).data(), 1000, 1000);
		string name_top = gPad->GetName() + stat + "_top";
		TPad *top_pad = new TPad(name_top.data(), name_top.data(), 0, 1 - y_top_length, 1, 1);
		top_pad->SetTopMargin(y_top_pad);
		top_pad->SetBottomMargin(0);

		string name_bottom = gPad->GetName() + stat + "_bottom";
		TPad *bottom_pad = new TPad(name_top.data(), name_top.data(), 0, 0, 1, 1 - y_top_length);
		bottom_pad->SetTickx();
		bottom_pad->SetBottomMargin(y_bot_pad);
		bottom_pad->SetTopMargin(0);

		if (stat == "mean") {
			top_pad->SetLeftMargin(0.12);
			bottom_pad->SetLeftMargin(0.12);
		} else {
			top_pad->SetLeftMargin(0.08);
			bottom_pad->SetLeftMargin(0.08);
		}
		top_pad->SetRightMargin(0.01);
		bottom_pad->SetRightMargin(0.01);

		top_pad->SetNumber(1);
		bottom_pad->SetNumber(2);

		top_pad->Draw();
		bottom_pad->Draw();

		can_stat->cd(1);
		TMultiGraph *mg_clone = (TMultiGraph*)mg->Clone(((string)mg->GetName() + "_Clone").data());
		mg_clone->GetYaxis()->SetTitle((stat_title[stat]).data());
//		mg_clone->SetTitle("Testhhher");
		mg_clone->GetYaxis()->SetTitleFont(43);
		mg_clone->GetYaxis()->SetTitleSize(20);
		mg_clone->GetYaxis()->SetLabelSize(20);
		if (stat == "mean") { mg_clone->GetYaxis()->SetTitleOffset(2.6); }
		else { mg_clone->GetYaxis()->SetTitleOffset(2.1); }
		mg_clone->Draw("AP");
		TLegend *leg_clone = (TLegend*)leg->Clone(((string)leg->GetName() + "_Clone").data());
		leg_clone->Draw();
		gPad->Update();
		leg_clone->SetX1NDC(0.55); leg_clone->SetX2NDC(0.85); leg_clone->SetY1NDC(0.05); leg_clone->SetY2NDC(0.25);
		gPad->Modified();

		can_stat->cd(2);
		TMultiGraph *mg_div_clone = (TMultiGraph*)mg_div->Clone(((string)mg_div->GetName() + "_Clone").data());
//		mg_div->GetYaxis()->SetTitle((stat_title[stat] + "_Raw  /  " + stat_title[stat] + "_Mix").data());
		mg_div_clone->GetYaxis()->SetTitle((stat_title[stat]).data());
		mg_div_clone->GetYaxis()->SetTitleFont(43);
		mg_div_clone->GetYaxis()->SetTitleSize(20);
		mg_div_clone->GetYaxis()->SetLabelSize(20);
		mg_div_clone->GetXaxis()->SetLabelSize(20);
		mg_div_clone->GetXaxis()->SetTitleSize(20);
		if (stat == "mean") { mg_div_clone->GetYaxis()->SetTitleOffset(2.6); }
		else { mg_div_clone->GetYaxis()->SetTitleOffset(2.1); }
		mg_div_clone->GetXaxis()->SetTitleOffset(2.2);
		mg_div_clone->Draw("AP");
		TLegend *leg_div_clone = (TLegend*)leg_div->Clone(((string)leg_div->GetName() + "_Clone").data());
		leg_div_clone->Draw();
		gPad->Update();
		leg_div_clone->SetX1NDC(0.55); leg_div_clone->SetX2NDC(0.85); leg_div_clone->SetY1NDC(0.15); leg_div_clone->SetY2NDC(0.3);
		gPad->Modified();

		if (1 < mg_div->GetYaxis()->GetXmax() && 1 > mg_div->GetYaxis()->GetXmin()) {
			TLine* one_line = new TLine(0, 1, 80, 1);
			one_line->SetLineStyle(2);
			one_line->Draw();
		}

		can_stat->Update();
		can_stat->Write();
		delete can_stat;

	}

	can->Update();
	can->Write();
	delete can;
}


void AzFigurePlotter::moments_vs_energy_cf_plot(vector<int> energies, map<string, map<string, plot_data>> bes1, map<string, map<string, plot_data>> ampt, map<string, map<string, plot_data>> cf, vector<string> stat_order, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	vector<double> energy_cf;
	for (double energy : energies) {
		energy_ampt.push_back(energy - 0.6);
		energy_bes1.push_back(energy);
		energy_cf.push_back(energy + 0.6);
	}

	map<string, int> color{ {"raw_bes", kRed}, {"raw_ampt", kBlue}, {"raw_cf", kGreen+2}, {"mix_bes", kRed}, {"mix_ampt", kBlue}, {"mix_cf", kGreen+2}, {"div_bes", kRed}, {"div_ampt", kBlue}, {"div_cf", kGreen+2} };
	map<string, int> marker_size{ {"raw", 2}, {"mix", 2}, {"divide", 2} };
//	map<string, int> marker_style{ {"raw_bes", 29}, {"raw_ampt", 22}, {"mix_bes", 30}, {"mix_ampt", 26}, {"div_bes", 29}, {"div_ampt", 22} };
	map<string, int> marker_style{ {"raw_bes", 29}, {"raw_ampt", 20}, {"raw_cf", 22}, {"mix_bes", 30}, {"mix_ampt", 24}, {"mix_cf", 26}, {"div_bes", 29}, {"div_ampt", 20}, {"div_cf", 22} };
	map<string, string> stat_title{ {"mean", "Mean"}, {"standard_deviation", "Standard Deviation"}, {"skewness", "Skewness"}, {"non_excess_kurtosis", "Kurtosis"} };
//	int bes_marker_style = 29;
//	int ampt_marker_style = 22;

	TCanvas* can = new TCanvas(("Moments_vs_energy_cf_can_" + type_name).data(), ("Moments vs Energy " + type_name).data(), 1250, 800);
	int stat_num = (int)stat_order.size();

	double y_top_pad = 0.01;
	double y_bot_pad = 0.14;
	double x_left_pad = 0.01;
	double x_right_pad = 0.01;
	double y_top_length = 0.6;
	double x_low = 0;
	double x_up = 0;
	double def_left_margin = 0.145;
	double def_right_margin = 0.01;

	double x_step = (1.0 - x_left_pad - x_right_pad) / stat_num;
//	cout << "x_step: " << x_step << endl;
	for (int x_index=0; x_index<stat_num; x_index++) {
		if (x_index == 0) { x_up = x_low + x_step + x_left_pad; }
		else if (x_index == stat_num - 1) { x_up = x_low + x_step + x_right_pad; }
		else { x_up = x_low + x_step; }
//		cout << "x_index: " << x_index << " x_low: " << x_low << " x_up: " << x_up << endl;

		int top_index = x_index * 2 + 1;
		string name_top = gPad->GetName() + to_string(top_index);
		TPad *top_pad = new TPad(name_top.data(), name_top.data(), x_low, 1 - y_top_length, x_up, 1.0);
		top_pad->SetTopMargin(y_top_pad);
		top_pad->SetBottomMargin(0);

		int bottom_index = x_index * 2 + 2;
		string name_bottom = gPad->GetName() + to_string(bottom_index);
		TPad *bottom_pad = new TPad(name_top.data(), name_top.data(), x_low, 0, x_up, 1 - y_top_length);
		bottom_pad->SetTickx();
		bottom_pad->SetBottomMargin(y_bot_pad);
		bottom_pad->SetTopMargin(0);

		if (x_index == 0) {
			top_pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin);
			bottom_pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin);
		} else { top_pad->SetLeftMargin(def_left_margin); bottom_pad->SetLeftMargin(def_left_margin); }
		if (x_index == stat_num - 1) {
			top_pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin);
			bottom_pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin);
		} else { top_pad->SetRightMargin(def_right_margin); bottom_pad->SetRightMargin(def_right_margin); }

		top_pad->SetNumber(top_index);
		bottom_pad->SetNumber(bottom_index);

		top_pad->Draw();
		bottom_pad->Draw();
		x_low = x_up;
	}

	int can_index = 1;
	for ( string stat : stat_order) { //pair<string, plot_data> stat : bes1["raw"]) {  // Iterate over stats, left to right on plot

		// Raw vs mixed on top plot
		can->cd(can_index * 2 - 1);

		TMultiGraph* mg = new TMultiGraph();
		mg->SetName((stat + "_raw_mixed").data());
//		mg->SetTitle((stat_title[stat]).data());

		TGraphErrors* bes1_ratio_raw_def_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][stat].val, energy_err, bes1["raw"][stat].stat);
		TGraphErrors* ampt_ratio_raw_def_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][stat].val, energy_err, ampt["raw"][stat].stat);
		TGraphErrors* cf_ratio_raw_def_g = graph_x_vs_y_err(energy_cf, cf["raw"][stat].val, energy_err, cf["raw"][stat].stat);
		bes1_ratio_raw_def_g->SetNameTitle(("bes1_raw_" + stat).data());
		ampt_ratio_raw_def_g->SetNameTitle(("ampt_raw_" + stat).data());
		cf_ratio_raw_def_g->SetNameTitle(("cf_raw_" + stat).data());
		TGraphErrors* bes1_ratio_raw_sys_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][stat].val, energy_err, bes1["raw"][stat].sys);
		TGraphErrors* ampt_ratio_raw_sys_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][stat].val, energy_err, ampt["raw"][stat].sys);
		TGraphErrors* cf_ratio_raw_sys_g = graph_x_vs_y_err(energy_cf, cf["raw"][stat].val, energy_err, cf["raw"][stat].sys);

		bes1_ratio_raw_def_g->SetMarkerStyle(marker_style["raw_bes"]);
		bes1_ratio_raw_def_g->SetMarkerColor(color["raw_bes"]);
		bes1_ratio_raw_def_g->SetLineColor(color["raw_bes"]);
		bes1_ratio_raw_def_g->SetMarkerSize(marker_size["raw"]);
		bes1_ratio_raw_sys_g->SetLineColor(color["raw_bes"]);

		ampt_ratio_raw_def_g->SetMarkerStyle(marker_style["raw_ampt"]);
		ampt_ratio_raw_def_g->SetMarkerColor(color["raw_ampt"]);
		ampt_ratio_raw_def_g->SetLineColor(color["raw_ampt"]);
		ampt_ratio_raw_def_g->SetMarkerSize(marker_size["raw"]);
		ampt_ratio_raw_sys_g->SetLineColor(color["raw_ampt"]);

		cf_ratio_raw_def_g->SetMarkerStyle(marker_style["raw_cf"]);
		cf_ratio_raw_def_g->SetMarkerColor(color["raw_cf"]);
		cf_ratio_raw_def_g->SetLineColor(color["raw_cf"]);
		cf_ratio_raw_def_g->SetMarkerSize(marker_size["raw"]);
		cf_ratio_raw_sys_g->SetLineColor(color["raw_cf"]);

		TGraphErrors* bes1_ratio_mix_def_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][stat].val, energy_err, bes1["mix"][stat].stat);
		TGraphErrors* ampt_ratio_mix_def_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][stat].val, energy_err, ampt["mix"][stat].stat);
		TGraphErrors* cf_ratio_mix_def_g = graph_x_vs_y_err(energy_cf, cf["mix"][stat].val, energy_err, cf["mix"][stat].stat);
		bes1_ratio_mix_def_g->SetNameTitle(("bes1_mix_" + stat).data());
		ampt_ratio_mix_def_g->SetNameTitle(("ampt_mix_" + stat).data());
		cf_ratio_mix_def_g->SetNameTitle(("cf_mix_" + stat).data());
		TGraphErrors* bes1_ratio_mix_sys_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][stat].val, energy_err, bes1["mix"][stat].sys);
		TGraphErrors* ampt_ratio_mix_sys_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][stat].val, energy_err, ampt["mix"][stat].sys);
		TGraphErrors* cf_ratio_mix_sys_g = graph_x_vs_y_err(energy_cf, cf["mix"][stat].val, energy_err, cf["mix"][stat].sys);

		bes1_ratio_mix_def_g->SetMarkerStyle(marker_style["mix_bes"]);
		bes1_ratio_mix_def_g->SetMarkerColor(color["mix_bes"]);
		bes1_ratio_mix_def_g->SetLineColor(color["mix_bes"]);
		bes1_ratio_mix_def_g->SetMarkerSize(marker_size["mix"]);
		bes1_ratio_mix_sys_g->SetLineColor(color["mix_bes"]);

		ampt_ratio_mix_def_g->SetMarkerStyle(marker_style["mix_ampt"]);
		ampt_ratio_mix_def_g->SetMarkerColor(color["mix_ampt"]);
		ampt_ratio_mix_def_g->SetLineColor(color["mix_ampt"]);
		ampt_ratio_mix_def_g->SetMarkerSize(marker_size["mix"]);
		ampt_ratio_mix_sys_g->SetLineColor(color["mix_ampt"]);

		cf_ratio_mix_def_g->SetMarkerStyle(marker_style["mix_cf"]);
		cf_ratio_mix_def_g->SetMarkerColor(color["mix_cf"]);
		cf_ratio_mix_def_g->SetLineColor(color["mix_cf"]);
		cf_ratio_mix_def_g->SetMarkerSize(marker_size["mix"]);
		cf_ratio_mix_sys_g->SetLineColor(color["mix_cf"]);

		mg->Add(cf_ratio_mix_def_g, "APZ");
		mg->Add(cf_ratio_mix_sys_g, "[]");
		mg->Add(cf_ratio_raw_def_g, "P");
		mg->Add(cf_ratio_raw_sys_g, "[]");
		mg->Add(ampt_ratio_mix_def_g, "P");
		mg->Add(ampt_ratio_mix_sys_g, "[]");
		mg->Add(ampt_ratio_raw_def_g, "P");
		mg->Add(ampt_ratio_raw_sys_g, "[]");
		mg->Add(bes1_ratio_mix_def_g, "P");
		mg->Add(bes1_ratio_mix_sys_g, "[]");
		mg->Add(bes1_ratio_raw_def_g, "P");
		mg->Add(bes1_ratio_raw_sys_g, "[]");

		mg->GetXaxis()->SetLimits(-2, 78);
		mg->GetXaxis()->SetRangeUser(-2, 78);
		mg->GetXaxis()->SetLabelFont(43);
		mg->GetXaxis()->SetLabelSize(def_text_size);
		mg->GetYaxis()->SetLabelFont(43);
		mg->GetYaxis()->SetLabelSize(def_text_size);
		if (type_name == "Ratio") {
			if (stat == "mean") {
				mg->GetYaxis()->SetLimits(0.3271, 0.3459);
				mg->GetYaxis()->SetRangeUser(0.3271, 0.3459);
				mg->GetYaxis()->SetDecimals(3);
			} else if (stat == "standard_deviation") {
				mg->GetYaxis()->SetLimits(0.061, 0.169);
				mg->GetYaxis()->SetRangeUser(0.061, 0.169);
				mg->GetYaxis()->SetDecimals(2);
			} else if (stat == "skewness") {
				mg->GetYaxis()->SetLimits(0.095, 0.295);
				mg->GetYaxis()->SetRangeUser(0.095, 0.295);
				mg->GetYaxis()->SetDecimals(2);
			} else if (stat == "non_excess_kurtosis") {
				mg->GetYaxis()->SetLimits(3.01, 3.24);
				mg->GetYaxis()->SetRangeUser(3.011, 3.239);
				mg->GetYaxis()->SetDecimals(2);
			}
		} else if (type_name == "Pull") {
			if (stat == "mean") {
				mg->GetYaxis()->SetLimits(-0.0059, 0.0129);
				mg->GetYaxis()->SetRangeUser(-0.0059, 0.0129);
				mg->GetYaxis()->SetDecimals(3);
			} else if (stat == "standard_deviation") {
				mg->GetYaxis()->SetLimits(0.941, 1.019);
				mg->GetYaxis()->SetRangeUser(0.941, 1.019);
				mg->GetYaxis()->SetDecimals(2);
			} else if (stat == "skewness") {
				mg->GetYaxis()->SetLimits(0.081, 0.249);
				mg->GetYaxis()->SetRangeUser(0.081, 0.249);
				mg->GetYaxis()->SetDecimals(2);
			} else if (stat == "non_excess_kurtosis") {
				mg->GetYaxis()->SetLimits(2.801, 2.989);
				mg->GetYaxis()->SetRangeUser(2.801, 2.989);
				mg->GetYaxis()->SetDecimals(2);
			}
		}

		mg->GetXaxis()->SetTitle("Energy (GeV)");
		mg->GetXaxis()->SetTitleFont(43);
		mg->GetXaxis()->SetTitleSize(def_text_size);

		mg->Draw("AP");

		TLegend* leg = new TLegend(0.3, 0.6, 0.9, 0.9);
		leg->SetTextFont(43);
		leg->SetTextSize(def_text_size);
		leg->AddEntry(bes1_ratio_raw_def_g, "STAR Raw", "p");
		leg->AddEntry(bes1_ratio_mix_def_g, "STAR Mixed", "p");
		leg->AddEntry(ampt_ratio_raw_def_g, "AMPT Raw", "p");
		leg->AddEntry(ampt_ratio_mix_def_g, "AMPT Mixed", "p");
		leg->AddEntry(cf_ratio_raw_def_g, "MUSIC + FIST Raw", "p");
		leg->AddEntry(cf_ratio_mix_def_g, "MUSIC + FIST Mixed", "p");
		leg->SetBorderSize(0);
		leg->SetFillStyle(0);
		if (stat == "mean") { leg->Draw(); }

		TText *title = new TText(0.55, 0.965, (stat_title[stat]).data());
		title->SetNDC();
		title->SetTextFont(43);
		title->SetTextSize(20);
		title->SetTextAlign(22);
		title->Draw();

		// Divided plot on bottom
		can->cd(can_index * 2);

		TMultiGraph* mg_div = new TMultiGraph();
		mg_div->SetName((stat + "_divided").data());

		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][stat].val, energy_err, bes1["divide"][stat].stat);
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][stat].val, energy_err, ampt["divide"][stat].stat);
		TGraphErrors* cf_ratio_div_def_g = graph_x_vs_y_err(energy_cf, cf["divide"][stat].val, energy_err, cf["divide"][stat].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + stat).data());
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + stat).data());
		cf_ratio_div_def_g->SetNameTitle(("cf_div_" + stat).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][stat].val, energy_err, bes1["divide"][stat].sys);
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][stat].val, energy_err, ampt["divide"][stat].sys);
		TGraphErrors* cf_ratio_div_sys_g = graph_x_vs_y_err(energy_cf, cf["divide"][stat].val, energy_err, cf["divide"][stat].sys);

		bes1_ratio_div_def_g->SetMarkerStyle(marker_style["div_bes"]);
		bes1_ratio_div_def_g->SetMarkerColor(color["div_bes"]);
		bes1_ratio_div_def_g->SetLineColor(color["div_bes"]);
		bes1_ratio_div_def_g->SetMarkerSize(marker_size["divide"]);
		bes1_ratio_div_sys_g->SetLineColor(color["div_bes"]);

		ampt_ratio_div_def_g->SetMarkerStyle(marker_style["div_ampt"]);
		ampt_ratio_div_def_g->SetMarkerColor(color["div_ampt"]);
		ampt_ratio_div_def_g->SetLineColor(color["div_ampt"]);
		ampt_ratio_div_def_g->SetMarkerSize(marker_size["divide"]);
		ampt_ratio_div_sys_g->SetLineColor(color["div_ampt"]);

		cf_ratio_div_def_g->SetMarkerStyle(marker_style["div_cf"]);
		cf_ratio_div_def_g->SetMarkerColor(color["div_cf"]);
		cf_ratio_div_def_g->SetLineColor(color["div_cf"]);
		cf_ratio_div_def_g->SetMarkerSize(marker_size["divide"]);
		cf_ratio_div_sys_g->SetLineColor(color["div_cf"]);

		mg_div->Add(cf_ratio_div_def_g, "APZ");
		mg_div->Add(cf_ratio_div_sys_g, "[]");
		mg_div->Add(ampt_ratio_div_def_g, "P");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");
		mg_div->Add(bes1_ratio_div_def_g, "P");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");

		mg_div->GetXaxis()->SetLimits(-2, 78);
		mg_div->GetXaxis()->SetRangeUser(-2, 78);
		mg_div->GetXaxis()->SetLabelFont(43);
		mg_div->GetXaxis()->SetLabelSize(def_text_size);
		mg_div->GetYaxis()->SetLabelFont(43);
		mg_div->GetYaxis()->SetLabelSize(def_text_size);
		if (type_name == "Ratio") {
			if (stat == "mean") {
				mg_div->GetYaxis()->SetLimits(0.991, 1.009);
				mg_div->GetYaxis()->SetRangeUser(0.991, 1.009);
				mg_div->GetYaxis()->SetDecimals(3);
			} else if (stat == "standard_deviation") {
				mg_div->GetYaxis()->SetLimits(0.941, 1.009);
				mg_div->GetYaxis()->SetRangeUser(0.941, 1.009);
				mg_div->GetYaxis()->SetDecimals(2);
			} else if (stat == "skewness") {
				mg_div->GetYaxis()->SetLimits(0.841, 1.019);
				mg_div->GetYaxis()->SetRangeUser(0.841, 1.019);
				mg_div->GetYaxis()->SetDecimals(2);
			} else if (stat == "non_excess_kurtosis") {
				mg_div->GetYaxis()->SetLimits(0.9961, 1.0119);
				mg_div->GetYaxis()->SetRangeUser(0.9961, 1.0119);
				mg_div->GetYaxis()->SetDecimals(3);
			}
		} else if (type_name == "Pull") {
			if (stat == "mean") {
				mg_div->GetYaxis()->SetLimits(-59, 59);
				mg_div->GetYaxis()->SetRangeUser(-59, 59);
//				mg_div->GetYaxis()->SetDecimals(3);
			} else if (stat == "standard_deviation") {
				mg_div->GetYaxis()->SetLimits(0.941, 1.009);
				mg_div->GetYaxis()->SetRangeUser(0.941, 1.009);
				mg_div->GetYaxis()->SetDecimals(2);
			} else if (stat == "skewness") {
				mg_div->GetYaxis()->SetLimits(0.841, 1.019);
				mg_div->GetYaxis()->SetRangeUser(0.841, 1.019);
				mg_div->GetYaxis()->SetDecimals(2);
			} else if (stat == "non_excess_kurtosis") {
				mg_div->GetYaxis()->SetLimits(0.9961, 1.0119);
				mg_div->GetYaxis()->SetRangeUser(0.9961, 1.0049);
				mg_div->GetYaxis()->SetDecimals(3);
			}
		}
		//mg_div->GetYaxis()->SetLimits(0.9955, 1.0145);
		//mg_div->GetYaxis()->SetRangeUser(0.9955, 1.0145);

		mg_div->GetXaxis()->SetTitle("Energy (GeV)");
		mg_div->GetXaxis()->SetTitleFont(43);
		mg_div->GetXaxis()->SetTitleSize(def_text_size);
		mg_div->GetXaxis()->SetTitleOffset(2.4);

		mg_div->Draw("AP");


		TLegend* leg_div = new TLegend(0.15, 0.75, 0.9, 0.95);
		leg_div->SetTextFont(43);
		leg_div->SetTextSize(def_text_size);
		leg_div->AddEntry(bes1_ratio_div_def_g, "STAR Raw/Mixed", "p");
		leg_div->AddEntry(ampt_ratio_div_def_g, "AMPT Raw/Mixed", "p");
		leg_div->AddEntry(cf_ratio_div_def_g, "MUSIC + FIST Raw/Mixed", "p");
		leg_div->SetBorderSize(0);
		leg_div->SetFillStyle(0);
		if (stat == "mean") { leg_div->Draw(); }

		if (1 < mg_div->GetYaxis()->GetXmax() && 1 > mg_div->GetYaxis()->GetXmin()) {
			TLine* one_line = new TLine(-2, 1, 78, 1);
			one_line->SetLineStyle(2);
			one_line->Draw();
		}

		can_index++;


		// Individual Canvases
		TCanvas *can_stat = new TCanvas((stat + "_vs_energy_cf_can_" + type_name).data(), (stat + " vs Energy " + type_name).data(), 1000, 1000);
		string name_top = gPad->GetName() + stat + "_top";
		TPad *top_pad = new TPad(name_top.data(), name_top.data(), 0, 1 - y_top_length, 1, 1);
		top_pad->SetTopMargin(y_top_pad);
		top_pad->SetBottomMargin(0);

		string name_bottom = gPad->GetName() + stat + "_bottom";
		TPad *bottom_pad = new TPad(name_top.data(), name_top.data(), 0, 0, 1, 1 - y_top_length);
		bottom_pad->SetTickx();
		bottom_pad->SetBottomMargin(y_bot_pad);
		bottom_pad->SetTopMargin(0);

		if (stat == "mean") {
			top_pad->SetLeftMargin(0.12);
			bottom_pad->SetLeftMargin(0.12);
		} else {
			top_pad->SetLeftMargin(0.08);
			bottom_pad->SetLeftMargin(0.08);
		}
		top_pad->SetRightMargin(0.01);
		bottom_pad->SetRightMargin(0.01);

		top_pad->SetNumber(1);
		bottom_pad->SetNumber(2);

		top_pad->Draw();
		bottom_pad->Draw();

		can_stat->cd(1);
		TMultiGraph *mg_clone = (TMultiGraph*)mg->Clone(((string)mg->GetName() + "_Clone").data());
		mg_clone->GetYaxis()->SetTitle((stat_title[stat]).data());
//		mg_clone->SetTitle("Testhhher");
		mg_clone->GetYaxis()->SetTitleFont(43);
		mg_clone->GetYaxis()->SetTitleSize(20);
		mg_clone->GetYaxis()->SetLabelSize(20);
		if (stat == "mean") { mg_clone->GetYaxis()->SetTitleOffset(2.6); }
		else { mg_clone->GetYaxis()->SetTitleOffset(2.1); }
		mg_clone->Draw("AP");
		TLegend *leg_clone = (TLegend*)leg->Clone(((string)leg->GetName() + "_Clone").data());
		leg_clone->Draw();
		gPad->Update();
		leg_clone->SetX1NDC(0.55); leg_clone->SetX2NDC(0.85); leg_clone->SetY1NDC(0.05); leg_clone->SetY2NDC(0.25);
		gPad->Modified();

		can_stat->cd(2);
		TMultiGraph *mg_div_clone = (TMultiGraph*)mg_div->Clone(((string)mg_div->GetName() + "_Clone").data());
//		mg_div->GetYaxis()->SetTitle((stat_title[stat] + "_Raw  /  " + stat_title[stat] + "_Mix").data());
		mg_div_clone->GetYaxis()->SetTitle((stat_title[stat]).data());
		mg_div_clone->GetYaxis()->SetTitleFont(43);
		mg_div_clone->GetYaxis()->SetTitleSize(20);
		mg_div_clone->GetYaxis()->SetLabelSize(20);
		mg_div_clone->GetXaxis()->SetLabelSize(20);
		mg_div_clone->GetXaxis()->SetTitleSize(20);
		if (stat == "mean") { mg_div_clone->GetYaxis()->SetTitleOffset(2.6); }
		else { mg_div_clone->GetYaxis()->SetTitleOffset(2.1); }
		mg_div_clone->GetXaxis()->SetTitleOffset(2.2);
		mg_div_clone->Draw("AP");
		TLegend *leg_div_clone = (TLegend*)leg_div->Clone(((string)leg_div->GetName() + "_Clone").data());
		leg_div_clone->Draw();
		gPad->Update();
		leg_div_clone->SetX1NDC(0.55); leg_div_clone->SetX2NDC(0.85); leg_div_clone->SetY1NDC(0.15); leg_div_clone->SetY2NDC(0.3);
		gPad->Modified();

		if (1 < mg_div->GetYaxis()->GetXmax() && 1 > mg_div->GetYaxis()->GetXmin()) {
			TLine* one_line = new TLine(0, 1, 80, 1);
			one_line->SetLineStyle(2);
			one_line->Draw();
		}

		can_stat->Update();
		can_stat->Write();
		delete can_stat;

	}

	can->Update();
	can->Write();
	delete can;
}


void AzFigurePlotter::kurt_vs_energy_w_rapid_plot(vector<int> energies, map<string, map<float, plot_data>> bes1, map<string, map<float, plot_data>> ampt, string type_name) {
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

	int x_num = 3;
	int y_num = 3;

	double y_top_pad = 0.01;
	double y_bot_pad = 0.04;
	double x_left_pad = 0.045;
	double x_right_pad = 0.01;
	double x_low = 0;
	double x_up = 0;
	double y_low = 0;
	double y_up = 1.0;
	double def_left_margin = 0.0;
	double def_right_margin = 0.0;
	double def_top_margin = 0.0;
	double def_bot_margin = 0.0;

	double x_step = (1.0 - x_left_pad - x_right_pad) / x_num;
	double y_step = (1.0 - y_top_pad - y_bot_pad) / y_num;
//	cout << "x_step: " << x_step << "  |  y_step: " << y_step << endl;
	for (int y_index=0; y_index<y_num; y_index++) {
		x_low = 0;
		if (y_index == 0) { y_low = y_up - y_step - y_top_pad; }
		else if (y_index == y_num - 1) { y_low = y_up - y_step - y_bot_pad; }
		else { y_low = y_up - y_step; }

		for (int x_index=0; x_index<x_num; x_index++) {
			int pad_index = x_index + x_num * y_index + 1;

			if (x_index == 0) { x_up = x_low + x_step + x_left_pad; }
			else if (x_index == x_num - 1) { x_up = x_low + x_step + x_right_pad; }
			else { x_up = x_low + x_step; }

			if (y_up > 1.0) { y_up = 1.0;}
			if (y_low < 0.0) { y_low = 0.0; }
			if (x_up > 1.0) { x_up = 1.0;}
			if (x_low < 0.0) { x_low = 0.0; }

//			cout << "x_index: " << x_index << " x_low: " << x_low << " x_up: " << x_up << endl;
//			cout << "y_index: " << y_index << " y_low: " << y_low << " y_up: " << y_up << endl;

			string pad_name = gPad->GetName() + to_string(pad_index);
			TPad *pad = new TPad(pad_name.data(), pad_name.data(), x_low, y_low, x_up, y_up);
			pad->SetTickx();
			pad->SetTicky();

			if (x_index == 0) {	pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin); }
			else { pad->SetLeftMargin(def_left_margin); }
			if (x_index == x_num - 1) {	pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin); }
			else { pad->SetRightMargin(def_right_margin); }
			if (y_index == 0) { pad->SetTopMargin(y_top_pad / (y_up - y_low) + def_top_margin); }
			else { pad->SetTopMargin(def_top_margin); }
			if (y_index == y_num -1) { pad->SetBottomMargin(y_bot_pad / (y_up - y_low) + def_bot_margin); }
			else { pad->SetBottomMargin(def_bot_margin); }

			pad->SetNumber(pad_index);

			pad->Draw();
			x_low = x_up;
		}
		y_up = y_low;
	}

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

		mg_div->Add(ampt_ratio_div_def_g, "APZ");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");
		mg_div->Add(bes1_ratio_div_def_g, "P");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");

		mg_div->GetXaxis()->SetLimits(-2, 78);
		mg_div->GetXaxis()->SetRangeUser(-2, 78);
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


		TLatex rapid_label;
		rapid_label.SetTextFont(43);
		rapid_label.SetTextAlign(22);
		rapid_label.SetTextSize(25);
		rapid_label.DrawLatex(65, 1.015, ("|#eta|<" + to_string(rapid.first).substr(0, 3)).data());


		TLine* one_line = new TLine(0, 1, 80, 1);
		one_line->SetLineStyle(2);
		one_line->Draw();

		if (can_index == 8) {
			can->cd(++can_index);
			vector<double> x_hold {-1};
			vector<double> y_hold {-1};
			TGraph *g_hold = new TGraph(1, x_hold.data(), y_hold.data());
			g_hold->SetTitle("");
			g_hold->GetXaxis()->SetLimits(-2, 78);
			g_hold->GetXaxis()->SetRangeUser(-2, 78);
			if (!zoom) {
				g_hold->GetYaxis()->SetLimits(0.9935, 1.0185);
				g_hold->GetYaxis()->SetRangeUser(0.9935, 1.0185);
			} else {
				g_hold->GetYaxis()->SetLimits(0.9975, 1.0035);
				g_hold->GetYaxis()->SetRangeUser(0.9975, 1.0035);
			}
			g_hold->Draw("AP");
			TLegend *leg2 = new TLegend(0.3, 0.4, 0.7, 0.6);
			leg2->AddEntry(bes1_ratio_div_def_g, "BES1", "p");
			leg2->AddEntry(ampt_ratio_div_def_g, "AMPT", "p");
			leg2->SetBorderSize(0);
			leg2->SetFillStyle(0);
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

		mg_div_bes->Add(bes1_ratio_div_def_g, "APZ");
		mg_div_bes->Add(bes1_ratio_div_sys_g, "[]");

		leg_bes->AddEntry(bes1_ratio_div_def_g, ("BES1 |#eta|<" + to_string(rapid.first).substr(0, 3)).data(), "p");
	}

	mg_div_bes->GetXaxis()->SetLimits(-2, 78);
	mg_div_bes->GetXaxis()->SetRangeUser(-2, 78);
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

		mg_div_ampt->Add(ampt_ratio_div_def_g, "P");
		mg_div_ampt->Add(ampt_ratio_div_sys_g, "[]");

		leg_ampt->AddEntry(ampt_ratio_div_def_g, ("AMPT |#eta|<" + to_string(rapid.first).substr(0, 3)).data(), "p");

//		can_index++;
	}

	mg_div_ampt->GetXaxis()->SetLimits(-2, 78);
	mg_div_ampt->GetXaxis()->SetRangeUser(-2, 78);
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


void AzFigurePlotter::kurt_vs_rapid_w_energy_plot(map<int, string> energies, vector<float> rapidities, map<string, map<int, plot_data>> bes1, map<string, map<int, plot_data>> ampt, string type_name) {
	vector<double> rapid_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> rapid_val(rapidities.begin(), rapidities.end());
	vector<double> rapid_bes1;
	vector<double> rapid_ampt;
	for (double rapid : rapidities) {
		rapid_bes1.push_back(rapid - 0.02);
		rapid_ampt.push_back(rapid + 0.02);
	}

	map<string, int> color{ {"7.7", kBlue}, {"11.5", kGreen + 3}, {"19.6", kRed}, {"27", kBlack}, {"39", kGray + 2}, {"62.4", kViolet} };
	map<string, int> marker_style{ {"bes", 20}, {"ampt", 24} };
	map<string, int> marker_size{ {"bes", 2}, {"ampt", 2} };
	map<string, int> marker_color{ {"bes", kBlack}, {"ampt", kRed} };

	TCanvas* can = new TCanvas(("Kurt_vs_Rapidity_can_" + type_name).data(), ("Kurtosis vs Rapidity " + type_name).data(), 955, 805);

	int x_num = 3;
	int y_num = 2;

	double y_top_pad = 0.01;
	double y_bot_pad = 0.045;
	double x_left_pad = 0.07;
	double x_right_pad = 0.01;
	double x_low = 0;
	double x_up = 0;
	double y_low = 0;
	double y_up = 1.0;
	double def_left_margin = 0.0;
	double def_right_margin = 0.0;
	double def_top_margin = 0.0;
	double def_bot_margin = 0.0;

	double x_step = (1.0 - x_left_pad - x_right_pad) / x_num;
	double y_step = (1.0 - y_top_pad - y_bot_pad) / y_num;
	//	cout << "x_step: " << x_step << "  |  y_step: " << y_step << endl;
	for (int y_index = 0; y_index < y_num; y_index++) {
		x_low = 0;
		if (y_index == 0) { y_low = y_up - y_step - y_top_pad; }
		else if (y_index == y_num - 1) { y_low = y_up - y_step - y_bot_pad; }
		else { y_low = y_up - y_step; }

		for (int x_index = 0; x_index < x_num; x_index++) {
			int pad_index = x_index + x_num * y_index + 1;

			if (x_index == 0) { x_up = x_low + x_step + x_left_pad; }
			else if (x_index == x_num - 1) { x_up = x_low + x_step + x_right_pad; }
			else { x_up = x_low + x_step; }

			if (y_up > 1.0) { y_up = 1.0; }
			if (y_low < 0.0) { y_low = 0.0; }
			if (x_up > 1.0) { x_up = 1.0; }
			if (x_low < 0.0) { x_low = 0.0; }

//			cout << endl << "pad_index: " << pad_index << endl;
//			cout << "x_index: " << x_index << " x_low: " << x_low << " x_up: " << x_up << endl;
//			cout << "y_index: " << y_index << " y_low: " << y_low << " y_up: " << y_up << endl;

			string pad_name = gPad->GetName() + to_string(pad_index);
			TPad* pad = new TPad(pad_name.data(), pad_name.data(), x_low, y_low, x_up, y_up);
			pad->SetTickx();
			pad->SetTicky();

			if (x_index == 0) { pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin); }
			else { pad->SetLeftMargin(def_left_margin); }
			if (x_index == x_num - 1) { pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin); }
			else { pad->SetRightMargin(def_right_margin); }
			if (y_index == 0) { pad->SetTopMargin(y_top_pad / (y_up - y_low) + def_top_margin); }
			else { pad->SetTopMargin(def_top_margin); }
			if (y_index == y_num - 1) { pad->SetBottomMargin(y_bot_pad / (y_up - y_low) + def_bot_margin); }
			else { pad->SetBottomMargin(def_bot_margin); }

			pad->SetNumber(pad_index);

			pad->Draw();
			x_low = x_up;
		}
		y_up = y_low;
	}

	int can_index = 0;

	for (pair<int, string> energy : energies) { // Iterate over energies, left to right on plot
		can->cd(++can_index);
//		cout << energy.second << " GeV can_index: " << can_index << endl;

		TMultiGraph* mg_div = new TMultiGraph();
		mg_div->SetName(("Energy_" + energy.second).data());

		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(rapid_bes1, bes1["divide"][energy.first].val, rapid_err, bes1["divide"][energy.first].stat);
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(rapid_ampt, ampt["divide"][energy.first].val, rapid_err, ampt["divide"][energy.first].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + energy.second).data());
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + energy.second).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(rapid_bes1, bes1["divide"][energy.first].val, rapid_err, bes1["divide"][energy.first].sys);
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(rapid_ampt, ampt["divide"][energy.first].val, rapid_err, ampt["divide"][energy.first].sys);

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

		mg_div->Add(ampt_ratio_div_def_g, "APZ");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");
		mg_div->Add(bes1_ratio_div_def_g, "P");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");

		mg_div->GetXaxis()->SetLimits(-0.1, 1.1);
		mg_div->GetXaxis()->SetRangeUser(-0.1, 1.1);
		mg_div->GetXaxis()->SetLabelFont(43);
		mg_div->GetXaxis()->SetTitleFont(43);
		mg_div->GetXaxis()->SetLabelSize(def_text_size);
		mg_div->GetXaxis()->SetTitleSize(def_text_size);
		mg_div->GetYaxis()->SetLabelFont(43);
		mg_div->GetYaxis()->SetTitleFont(43);
		mg_div->GetYaxis()->SetLabelSize(def_text_size);
		mg_div->GetYaxis()->SetTitleSize(def_text_size);
		mg_div->GetYaxis()->SetDecimals(3);
		mg_div->GetXaxis()->SetTitleOffset(1.9);
		mg_div->GetYaxis()->SetTitleOffset(4.4);
		if (!zoom) {
			mg_div->GetYaxis()->SetLimits(0.9935, 1.0185);
			mg_div->GetYaxis()->SetRangeUser(0.9935, 1.0185);
		}
		else {
			mg_div->GetYaxis()->SetLimits(0.9975, 1.0035);
			mg_div->GetYaxis()->SetRangeUser(0.9975, 1.0035);
		}

		mg_div->GetXaxis()->SetTitle("Rapidity");
		mg_div->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mix}");

		mg_div->Draw("AP");


		TLatex energy_label;
		energy_label.SetTextFont(43);
		energy_label.SetTextAlign(22);
		energy_label.SetTextSize(25);
		energy_label.DrawLatex(0.8, 1.017, (energy.second + " GeV").data());  //to_string(energy.first).substr(0, 3)


		TLine* one_line = new TLine(-0.1, 1, 1.1, 1);
		one_line->SetLineStyle(2);
		one_line->Draw();

		if (can_index == 1) {
			//can->cd(++can_index);
			//vector<double> x_hold{ -1 };
			//vector<double> y_hold{ -1 };
			//TGraph* g_hold = new TGraph(1, x_hold.data(), y_hold.data());
			//g_hold->SetTitle("");
			//g_hold->GetXaxis()->SetLimits(-2, 78);
			//g_hold->GetXaxis()->SetRangeUser(-2, 78);
			//if (!zoom) {
			//	g_hold->GetYaxis()->SetLimits(0.9935, 1.0185);
			//	g_hold->GetYaxis()->SetRangeUser(0.9935, 1.0185);
			//}
			//else {
			//	g_hold->GetYaxis()->SetLimits(0.9975, 1.0035);
			//	g_hold->GetYaxis()->SetRangeUser(0.9975, 1.0035);
			//}
			//g_hold->Draw("AP");
			float y1 = 0.5, y2 = 0.7;
			if (type_name == "Ratio") { y1 = 0.05; y2 = 0.25; }
			else if (type_name == "Pull") { y1 = 0.5; y2 = 0.7; }
			TLegend *leg2 = new TLegend(0.2, y1, 0.5, y2);
			leg2->AddEntry(bes1_ratio_div_def_g, "BES1", "p");
			leg2->AddEntry(ampt_ratio_div_def_g, "AMPT", "p");
			leg2->SetBorderSize(0);
			leg2->SetFillStyle(0);
			leg2->Draw();
		}
	}

	can->Update();
	can->Write();

	delete can;


	// Just BES1
	TCanvas* can_bes = new TCanvas(("BES_vs_Rapidity_can_" + type_name).data(), ("Kurtosis vs Rapidity " + type_name).data(), 955, 805);
	//	can_bes->Divide(3, 3, 0.001, 0.001);

	TMultiGraph* mg_div_bes = new TMultiGraph();
	mg_div_bes->SetName("Vs_Rapidities_divided");

	TLegend* leg_bes = new TLegend(0.3, 0.21, 0.3, 0.21);
	leg_bes->SetBorderSize(0);
	leg_bes->SetFillStyle(0);

	for (pair<int, string> energy : energies) {  // Iterate over energies, left to right on plot
		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(rapid_bes1, bes1["divide"][energy.first].val, rapid_err, bes1["divide"][energy.first].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + energy.second).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(rapid_bes1, bes1["divide"][energy.first].val, rapid_err, bes1["divide"][energy.first].sys);

		bes1_ratio_div_def_g->SetMarkerStyle(marker_style["bes"]);
		bes1_ratio_div_def_g->SetMarkerColor(color[energy.second]);
		bes1_ratio_div_def_g->SetLineColor(color[energy.second]);
		bes1_ratio_div_def_g->SetMarkerSize(marker_size["bes"]);
		bes1_ratio_div_sys_g->SetLineColor(color[energy.second]);

		mg_div_bes->Add(bes1_ratio_div_def_g, "APZ");
		mg_div_bes->Add(bes1_ratio_div_sys_g, "[]");

		leg_bes->AddEntry(bes1_ratio_div_def_g, ("BES1 " + energy.second + "GeV").data(), "p");
	}

	mg_div_bes->GetXaxis()->SetLimits(0, 1.1);
	mg_div_bes->GetXaxis()->SetRangeUser(0, 1.1);
	mg_div_bes->GetYaxis()->SetDecimals(3);
	if (!zoom) {
		mg_div_bes->GetYaxis()->SetLimits(0.9935, 1.0185);
		mg_div_bes->GetYaxis()->SetRangeUser(0.9935, 1.0185);
	}

	mg_div_bes->GetXaxis()->SetTitle("Rapidity");
	mg_div_bes->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mix}");

	mg_div_bes->Draw("AP");

	TLine* one_line_bes = new TLine(0, 1, 1.1, 1);
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
	TCanvas* can_ampt = new TCanvas(("AMPT_Vs_Rapidity_can_" + type_name).data(), ("Vs Rapidity " + type_name).data(), 955, 805);
	//	can_ampt->Divide(3, 3, 0.001, 0.001);

	TMultiGraph* mg_div_ampt = new TMultiGraph();
	mg_div_ampt->SetName("Vs_Rapidities_divided");

	TLegend* leg_ampt = new TLegend(0.3, 0.21, 0.3, 0.21);
	leg_ampt->SetBorderSize(0);
	leg_ampt->SetFillStyle(0);

	for (pair<int, string> energy : energies) {  // Iterate over rapidities, left to right on plot
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(rapid_ampt, ampt["divide"][energy.first].val, rapid_err, ampt["divide"][energy.first].stat);
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + energy.second).data());
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(rapid_ampt, ampt["divide"][energy.first].val, rapid_err, ampt["divide"][energy.first].sys);

		ampt_ratio_div_def_g->SetMarkerStyle(marker_style["ampt"]);
		ampt_ratio_div_def_g->SetMarkerColor(color[energy.second]);
		ampt_ratio_div_def_g->SetLineColor(color[energy.second]);
		ampt_ratio_div_def_g->SetMarkerSize(marker_size["ampt"]);
		ampt_ratio_div_sys_g->SetLineColor(color[energy.second]);

		mg_div_ampt->Add(ampt_ratio_div_def_g, "P");
		mg_div_ampt->Add(ampt_ratio_div_sys_g, "[]");

		leg_ampt->AddEntry(ampt_ratio_div_def_g, ("AMPT " + energy.second + "GeV").data(), "p");

		//		can_index++;
	}

	mg_div_ampt->GetXaxis()->SetLimits(0, 1.1);
	mg_div_ampt->GetXaxis()->SetRangeUser(0, 1.1);
	mg_div_ampt->GetYaxis()->SetDecimals(3);
	if (!zoom) {
		mg_div_ampt->GetYaxis()->SetLimits(0.9935, 1.0185);
		mg_div_ampt->GetYaxis()->SetRangeUser(0.9935, 1.0185);
	}

	mg_div_ampt->GetXaxis()->SetTitle("Rapidity");
	mg_div_ampt->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mix}");

	mg_div_ampt->Draw("AP");

	TLine* one_line_ampt = new TLine(0, 1, 1.1, 1);
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


void AzFigurePlotter::kurt_vs_rapid_w_energy_cf_plot(map<int, string> energies, vector<float> rapidities, map<string, map<int, plot_data>> bes1, map<string, map<int, plot_data>> ampt, map<string, map<int, plot_data>> cf, string type_name) {
	vector<double> rapid_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> rapid_val(rapidities.begin(), rapidities.end());
	vector<double> rapid_bes1;
	vector<double> rapid_ampt;
	vector<double> rapid_cf;
	for (double rapid : rapidities) {
		rapid_bes1.push_back(rapid);
		rapid_ampt.push_back(rapid - 0.02);
		rapid_cf.push_back(rapid + 0.02);
	}

	map<string, int> color{ {"7.7", kBlue}, {"11.5", kGreen + 3}, {"19.6", kRed}, {"27", kBlack}, {"39", kGray + 2}, {"62.4", kViolet} };
	map<string, int> marker_style{ {"bes", 20}, {"ampt", 24}, {"cf", 24} };
	map<string, int> marker_size{ {"bes", 2}, {"ampt", 2}, {"cf", 2} };
	map<string, int> marker_color{ {"bes", kBlack}, {"ampt", kRed}, {"cf", kBlue} };

	TCanvas* can = new TCanvas(("Kurt_vs_Rapidity_cf_can_" + type_name).data(), ("Kurtosis vs Rapidity " + type_name).data(), 955, 805);

	int x_num = 3;
	int y_num = 2;

	double y_top_pad = 0.01;
	double y_bot_pad = 0.05;
	double x_left_pad = 0.076;
	double x_right_pad = 0.01;
	double x_low = 0;
	double x_up = 0;
	double y_low = 0;
	double y_up = 1.0;
	double def_left_margin = 0.0;
	double def_right_margin = 0.0;
	double def_top_margin = 0.0;
	double def_bot_margin = 0.0;

	double x_step = (1.0 - x_left_pad - x_right_pad) / x_num;
	double y_step = (1.0 - y_top_pad - y_bot_pad) / y_num;
	//	cout << "x_step: " << x_step << "  |  y_step: " << y_step << endl;
	for (int y_index = 0; y_index < y_num; y_index++) {
		x_low = 0;
		if (y_index == 0) { y_low = y_up - y_step - y_top_pad; }
		else if (y_index == y_num - 1) { y_low = y_up - y_step - y_bot_pad; }
		else { y_low = y_up - y_step; }

		for (int x_index = 0; x_index < x_num; x_index++) {
			int pad_index = x_index + x_num * y_index + 1;

			if (x_index == 0) { x_up = x_low + x_step + x_left_pad; }
			else if (x_index == x_num - 1) { x_up = x_low + x_step + x_right_pad; }
			else { x_up = x_low + x_step; }

			if (y_up > 1.0) { y_up = 1.0; }
			if (y_low < 0.0) { y_low = 0.0; }
			if (x_up > 1.0) { x_up = 1.0; }
			if (x_low < 0.0) { x_low = 0.0; }

			//			cout << endl << "pad_index: " << pad_index << endl;
			//			cout << "x_index: " << x_index << " x_low: " << x_low << " x_up: " << x_up << endl;
			//			cout << "y_index: " << y_index << " y_low: " << y_low << " y_up: " << y_up << endl;

			string pad_name = gPad->GetName() + to_string(pad_index);
			TPad* pad = new TPad(pad_name.data(), pad_name.data(), x_low, y_low, x_up, y_up);
			pad->SetTickx();
			pad->SetTicky();

			if (x_index == 0) { pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin); }
			else { pad->SetLeftMargin(def_left_margin); }
			if (x_index == x_num - 1) { pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin); }
			else { pad->SetRightMargin(def_right_margin); }
			if (y_index == 0) { pad->SetTopMargin(y_top_pad / (y_up - y_low) + def_top_margin); }
			else { pad->SetTopMargin(def_top_margin); }
			if (y_index == y_num - 1) { pad->SetBottomMargin(y_bot_pad / (y_up - y_low) + def_bot_margin); }
			else { pad->SetBottomMargin(def_bot_margin); }

			pad->SetNumber(pad_index);

			pad->Draw();
			x_low = x_up;
		}
		y_up = y_low;
	}

	int can_index = 0;

	for (pair<int, string> energy : energies) { // Iterate over energies, left to right on plot
		can->cd(++can_index);
		//		cout << energy.second << " GeV can_index: " << can_index << endl;

		TMultiGraph* mg_div = new TMultiGraph();
		mg_div->SetName(("Energy_" + energy.second).data());

		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(rapid_bes1, bes1["divide"][energy.first].val, rapid_err, bes1["divide"][energy.first].stat);
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(rapid_ampt, ampt["divide"][energy.first].val, rapid_err, ampt["divide"][energy.first].stat);
		TGraphErrors* cf_ratio_div_def_g = graph_x_vs_y_err(rapid_cf, cf["divide"][energy.first].val, rapid_err, cf["divide"][energy.first].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + energy.second).data());
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + energy.second).data());
		cf_ratio_div_def_g->SetNameTitle(("cf_div_" + energy.second).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(rapid_bes1, bes1["divide"][energy.first].val, rapid_err, bes1["divide"][energy.first].sys);
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(rapid_ampt, ampt["divide"][energy.first].val, rapid_err, ampt["divide"][energy.first].sys);
		TGraphErrors* cf_ratio_div_sys_g = graph_x_vs_y_err(rapid_cf, cf["divide"][energy.first].val, rapid_err, cf["divide"][energy.first].sys);

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

		cf_ratio_div_def_g->SetMarkerStyle(marker_style["cf"]);
		cf_ratio_div_def_g->SetMarkerColor(marker_color["cf"]);
		cf_ratio_div_def_g->SetLineColor(marker_color["cf"]);
		cf_ratio_div_def_g->SetMarkerSize(marker_size["cf"]);
		cf_ratio_div_sys_g->SetLineColor(marker_color["cf"]);

		mg_div->Add(ampt_ratio_div_def_g, "APZ");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");
		mg_div->Add(cf_ratio_div_def_g, "P");
		mg_div->Add(cf_ratio_div_sys_g, "[]");
		mg_div->Add(bes1_ratio_div_def_g, "P");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");

		mg_div->GetXaxis()->SetLimits(-0.1, 1.1);
		mg_div->GetXaxis()->SetRangeUser(-0.1, 1.1);
		mg_div->GetXaxis()->SetLabelFont(43);
		mg_div->GetXaxis()->SetTitleFont(43);
		mg_div->GetXaxis()->SetLabelSize(def_text_size);
		mg_div->GetXaxis()->SetTitleSize(def_text_size);
		mg_div->GetYaxis()->SetLabelFont(43);
		mg_div->GetYaxis()->SetTitleFont(43);
		mg_div->GetYaxis()->SetLabelSize(def_text_size);
		mg_div->GetYaxis()->SetTitleSize(def_text_size);
		mg_div->GetYaxis()->SetDecimals(3);
		mg_div->GetXaxis()->SetTitleOffset(1.9);
		mg_div->GetYaxis()->SetTitleOffset(4.4);
		if (!zoom) {
			mg_div->GetYaxis()->SetLimits(0.9935, 1.0185);
			mg_div->GetYaxis()->SetRangeUser(0.9935, 1.0185);
		}
		else {
			mg_div->GetYaxis()->SetLimits(0.9975, 1.0035);
			mg_div->GetYaxis()->SetRangeUser(0.9975, 1.0035);
		}

		mg_div->GetXaxis()->SetTitle("|#eta|_{max}");
		mg_div->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mix}");

		mg_div->Draw("AP");


		TLatex energy_label;
		energy_label.SetTextFont(43);
		energy_label.SetTextAlign(22);
		energy_label.SetTextSize(25);
		energy_label.DrawLatex(0.8, 1.017, (energy.second + " GeV").data());  //to_string(energy.first).substr(0, 3)


		TLine* one_line = new TLine(-0.1, 1, 1.1, 1);
		one_line->SetLineStyle(2);
		one_line->Draw();

		if (energy.first == 11) {
			float y1 = 0.5, y2 = 0.7, x1 = 0.2, x2 = 0.5;
			if (type_name == "Ratio") { y1 = 0.05; y2 = 0.25; x1 = 0.0; x2 = 1.0; }
			else if (type_name == "Pull") { y1 = 0.5; y2 = 0.8; x1 = 0.2; x2 = 1.0; }
			TLegend* leg2 = new TLegend(x1, y1, x2, y2);
			leg2->AddEntry(bes1_ratio_div_def_g, "BES1", "p");
			leg2->AddEntry(ampt_ratio_div_def_g, "AMPT", "p");
			leg2->AddEntry(cf_ratio_div_def_g, "MUSIC + FIST", "p");
			if (type_name == "Ratio") { leg2->SetNColumns(3); }
			leg2->SetBorderSize(0);
			leg2->SetFillStyle(0);
			leg2->Draw();
		}
	}

	can->Update();
	can->Write();

	delete can;
}


void AzFigurePlotter::kurt_with_efficiency_plot(vector<int> energies, map<string, map<float, plot_data>> bes1, map<string, map<float, plot_data>> ampt, string type_name) {
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

		mg_bes->Add(bes1_ratio_mix_def_g, "APZ");
		mg_bes->Add(bes1_ratio_mix_sys_g, "[]");
		mg_bes->Add(bes1_ratio_raw_def_g, "P");
		mg_bes->Add(bes1_ratio_raw_sys_g, "[]");

		leg_bes->AddEntry(bes1_ratio_raw_def_g, ("Raw " + to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
		leg_bes->AddEntry(bes1_ratio_mix_def_g, ("Mixed " + to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
	}

	mg_bes->GetXaxis()->SetLimits(-2, 78);
	mg_bes->GetXaxis()->SetRangeUser(-2, 78);
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

		mg_div_bes->Add(bes1_ratio_div_def_g, "APZ");
		mg_div_bes->Add(bes1_ratio_div_sys_g, "[]");

		leg_div_bes->AddEntry(bes1_ratio_div_def_g, (to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
	}

	mg_div_bes->GetXaxis()->SetLimits(-2, 78);
	mg_div_bes->GetXaxis()->SetRangeUser(-2, 78);
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

		mg_ampt->Add(ampt_ratio_mix_def_g, "APZ");
		mg_ampt->Add(ampt_ratio_mix_sys_g, "[]");
		mg_ampt->Add(ampt_ratio_raw_def_g, "P");
		mg_ampt->Add(ampt_ratio_raw_sys_g, "[]");

		leg_ampt->AddEntry(ampt_ratio_raw_def_g, ("Raw " + to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
		leg_ampt->AddEntry(ampt_ratio_mix_def_g, ("Mixed " + to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
	}

	mg_ampt->GetXaxis()->SetLimits(-2, 78);
	mg_ampt->GetXaxis()->SetRangeUser(-2, 78);
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

		mg_div_ampt->Add(ampt_ratio_div_def_g, "APZ");
		mg_div_ampt->Add(ampt_ratio_div_sys_g, "[]");

		leg_div_ampt->AddEntry(ampt_ratio_div_def_g, (to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
	}

	mg_div_ampt->GetXaxis()->SetLimits(-2, 78);
	mg_div_ampt->GetXaxis()->SetRangeUser(-2, 78);
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



void AzFigurePlotter::kurt_vs_efficiency_plot(map<int, string> energies, vector<float> effs, map<string, map<int, plot_data>> bes1, map<string, map<int, plot_data>> ampt, map<string, map<int, plot_data>> cf, string type_name) {
	vector<double> eff_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> eff_bes1;
	vector<double> eff_ampt;
	vector<double> eff_cf;
	for (float eff : effs) {
		eff_ampt.push_back(eff - 0.005);
		eff_bes1.push_back(eff);
		eff_cf.push_back(eff + 0.005);
	}

	map<string, int> color{ {"7.7", kBlue}, {"11.5", kGreen + 3}, {"19.6", kRed}, {"27", kBlack}, {"39", kGray + 2}, {"62.4", kViolet} };
	map<string, int> marker_style{ {"bes", 20}, {"ampt", 24}, {"cf", 24} };
	map<string, int> marker_size{ {"bes", 2}, {"ampt", 2}, {"cf", 2} };
	map<string, int> marker_color{ {"bes", kBlack}, {"ampt", kRed}, {"cf", kBlue} };

	TCanvas* can = new TCanvas(("Kurt_vs_Efficiency_" + type_name).data(), ("Kurtosis vs Efficiency " + type_name).data(), 955, 805);

	int x_num = 3;
	int y_num = 2;

	double y_top_pad = 0.01;
	double y_bot_pad = 0.048;
	double x_left_pad = 0.075;
	double x_right_pad = 0.01;
	double x_low = 0;
	double x_up = 0;
	double y_low = 0;
	double y_up = 1.0;
	double def_left_margin = 0.0;
	double def_right_margin = 0.0;
	double def_top_margin = 0.0;
	double def_bot_margin = 0.0;

	double x_step = (1.0 - x_left_pad - x_right_pad) / x_num;
	double y_step = (1.0 - y_top_pad - y_bot_pad) / y_num;
	//	cout << "x_step: " << x_step << "  |  y_step: " << y_step << endl;
	for (int y_index = 0; y_index < y_num; y_index++) {
		x_low = 0;
		if (y_index == 0) { y_low = y_up - y_step - y_top_pad; }
		else if (y_index == y_num - 1) { y_low = y_up - y_step - y_bot_pad; }
		else { y_low = y_up - y_step; }

		for (int x_index = 0; x_index < x_num; x_index++) {
			int pad_index = x_index + x_num * y_index + 1;

			if (x_index == 0) { x_up = x_low + x_step + x_left_pad; }
			else if (x_index == x_num - 1) { x_up = x_low + x_step + x_right_pad; }
			else { x_up = x_low + x_step; }

			if (y_up > 1.0) { y_up = 1.0; }
			if (y_low < 0.0) { y_low = 0.0; }
			if (x_up > 1.0) { x_up = 1.0; }
			if (x_low < 0.0) { x_low = 0.0; }

//			cout << endl << "pad_index: " << pad_index << endl;
//			cout << "x_index: " << x_index << " x_low: " << x_low << " x_up: " << x_up << endl;
//			cout << "y_index: " << y_index << " y_low: " << y_low << " y_up: " << y_up << endl;

			string pad_name = gPad->GetName() + to_string(pad_index);
			TPad* pad = new TPad(pad_name.data(), pad_name.data(), x_low, y_low, x_up, y_up);
			pad->SetTickx();
			pad->SetTicky();

			if (x_index == 0) { pad->SetLeftMargin(x_left_pad / (x_up - x_low) + def_left_margin); }
			else { pad->SetLeftMargin(def_left_margin); }
			if (x_index == x_num - 1) { pad->SetRightMargin(x_right_pad / (x_up - x_low) + def_right_margin); }
			else { pad->SetRightMargin(def_right_margin); }
			if (y_index == 0) { pad->SetTopMargin(y_top_pad / (y_up - y_low) + def_top_margin); }
			else { pad->SetTopMargin(def_top_margin); }
			if (y_index == y_num - 1) { pad->SetBottomMargin(y_bot_pad / (y_up - y_low) + def_bot_margin); }
			else { pad->SetBottomMargin(def_bot_margin); }

			pad->SetNumber(pad_index);

			pad->Draw();
			x_low = x_up;
		}
		y_up = y_low;
	}

	int can_index = 0;

	for (pair<int, string> energy : energies) { // Iterate over energies, left to right on plot
		can->cd(++can_index);

		TMultiGraph* mg_div = new TMultiGraph();
		mg_div->SetName(("Energy_" + energy.second).data());

		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(eff_bes1, bes1["divide"][energy.first].val, eff_err, bes1["divide"][energy.first].stat);
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(eff_ampt, ampt["divide"][energy.first].val, eff_err, ampt["divide"][energy.first].stat);
		TGraphErrors* cf_ratio_div_def_g = graph_x_vs_y_err(eff_cf, cf["divide"][energy.first].val, eff_err, cf["divide"][energy.first].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + energy.second).data());
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + energy.second).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(eff_bes1, bes1["divide"][energy.first].val, eff_err, bes1["divide"][energy.first].sys);
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(eff_ampt, ampt["divide"][energy.first].val, eff_err, ampt["divide"][energy.first].sys);
		TGraphErrors* cf_ratio_div_sys_g = graph_x_vs_y_err(eff_cf, cf["divide"][energy.first].val, eff_err, cf["divide"][energy.first].sys);

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

		cf_ratio_div_def_g->SetMarkerStyle(marker_style["cf"]);
		cf_ratio_div_def_g->SetMarkerColor(marker_color["cf"]);
		cf_ratio_div_def_g->SetLineColor(marker_color["cf"]);
		cf_ratio_div_def_g->SetMarkerSize(marker_size["cf"]);
		cf_ratio_div_sys_g->SetLineColor(marker_color["cf"]);

		mg_div->Add(cf_ratio_div_def_g, "APZ");
		mg_div->Add(cf_ratio_div_sys_g, "[]");
		mg_div->Add(ampt_ratio_div_def_g, "P");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");
		mg_div->Add(bes1_ratio_div_def_g, "P");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");

		mg_div->GetXaxis()->SetLimits(-0.04, 0.34);
		mg_div->GetXaxis()->SetRangeUser(-0.04, 0.34);
		mg_div->GetXaxis()->SetLabelFont(43);
		mg_div->GetXaxis()->SetTitleFont(43);
		mg_div->GetXaxis()->SetLabelSize(def_text_size);
		mg_div->GetXaxis()->SetTitleSize(def_text_size);
		mg_div->GetYaxis()->SetLabelFont(43);
		mg_div->GetYaxis()->SetTitleFont(43);
		mg_div->GetYaxis()->SetLabelSize(def_text_size);
		mg_div->GetYaxis()->SetTitleSize(def_text_size);
		mg_div->GetYaxis()->SetDecimals(3);
		mg_div->GetXaxis()->SetTitleOffset(1.9);
		mg_div->GetYaxis()->SetTitleOffset(4.4);
		if (!zoom) {
			mg_div->GetYaxis()->SetLimits(0.9935, 1.0185);
			mg_div->GetYaxis()->SetRangeUser(0.9935, 1.0185);
		}
		else {
			mg_div->GetYaxis()->SetLimits(0.9975, 1.0035);
			mg_div->GetYaxis()->SetRangeUser(0.9975, 1.0035);
		}

		mg_div->GetXaxis()->SetTitle("Efficiency");
		mg_div->GetYaxis()->SetTitle("Kurtosis_{Raw}  /  Kurtosis_{Mix}");

		mg_div->Draw("AP");


		TLatex energy_label;
		energy_label.SetTextFont(43);
		energy_label.SetTextAlign(22);
		energy_label.SetTextSize(25);
		energy_label.DrawLatex(0.23, 1.017, (energy.second + " GeV").data());


		TLine* one_line = new TLine(-0.04, 1, 0.34, 1);
		one_line->SetLineStyle(2);
		one_line->Draw();

		if (energy.first == 11) {
			float y1 = 0.5, y2 = 0.7, x1 = 0.0, x2 = 1.0;
			if (type_name == "Ratio") { y1 = 0.6; y2 = 0.95; }
			else if (type_name == "Pull") { y1 = 0.6; y2 = 0.95; }
			TLegend *leg2 = new TLegend(x1, y1, x2, y2);
			leg2->AddEntry(bes1_ratio_div_def_g, "BES1", "p");
			leg2->AddEntry(ampt_ratio_div_def_g, "AMPT", "p");
			leg2->AddEntry(cf_ratio_div_def_g, "MUSIC + FIST", "p");
			leg2->SetBorderSize(0);
			leg2->SetFillStyle(0);
			leg2->Draw();
		}
	}

	can->Update();
	can->Write();

	delete can;


	// BES1
//	TCanvas* can_bes = new TCanvas(("BES1_Efficiency_vs_energy_can_" + type_name).data(), ("Efficiency vs Energy " + type_name).data(), 955, 805);
//	can_bes->SetPad(0.01, 0.01, 0.99, 0.99);
//	can_bes->Divide(1, 2, 0, 0);
//
//	can_bes->cd(1);
//	TMultiGraph* mg_bes = new TMultiGraph();
//	mg_bes->SetName("Efficiencies_Raw_Mixed_BES1");
//
//	TLegend* leg_bes = new TLegend(0.3, 0.21, 0.3, 0.21);
//	leg_bes->SetBorderSize(0);
//	leg_bes->SetFillStyle(0);
//
//	for (pair<float, plot_data> eff : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
//		TGraphErrors* bes1_ratio_raw_def_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][eff.first].val, energy_err, bes1["raw"][eff.first].stat);
//		bes1_ratio_raw_def_g->SetNameTitle(("bes1_raw_" + to_string(eff.first).substr(0, 3)).data());
//		TGraphErrors* bes1_ratio_raw_sys_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][eff.first].val, energy_err, bes1["raw"][eff.first].sys);
//
//		bes1_ratio_raw_def_g->SetMarkerStyle(marker_style[eff.first]);
//		bes1_ratio_raw_def_g->SetMarkerColor(color_raw[eff.first]);
//		bes1_ratio_raw_def_g->SetLineColor(color_raw[eff.first]);
//		bes1_ratio_raw_def_g->SetMarkerSize(marker_size[eff.first]);
//		bes1_ratio_raw_sys_g->SetLineColor(color_raw[eff.first]);
//
//		TGraphErrors* bes1_ratio_mix_def_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][eff.first].val, energy_err, bes1["mix"][eff.first].stat);
//		bes1_ratio_mix_def_g->SetNameTitle(("bes1_mix_" + to_string(eff.first).substr(0, 3)).data());
//		TGraphErrors* bes1_ratio_mix_sys_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][eff.first].val, energy_err, bes1["mix"][eff.first].sys);
//
//		bes1_ratio_mix_def_g->SetMarkerStyle(marker_style[eff.first]);
//		bes1_ratio_mix_def_g->SetMarkerColor(color_mix[eff.first]);
//		bes1_ratio_mix_def_g->SetLineColor(color_mix[eff.first]);
//		bes1_ratio_mix_def_g->SetMarkerSize(marker_size[eff.first]);
//		bes1_ratio_mix_sys_g->SetLineColor(color_mix[eff.first]);
//
//		mg_bes->Add(bes1_ratio_mix_def_g, "APZ");
//		mg_bes->Add(bes1_ratio_mix_sys_g, "[]");
//		mg_bes->Add(bes1_ratio_raw_def_g, "P");
//		mg_bes->Add(bes1_ratio_raw_sys_g, "[]");
//
//		leg_bes->AddEntry(bes1_ratio_raw_def_g, ("Raw " + to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
//		leg_bes->AddEntry(bes1_ratio_mix_def_g, ("Mixed " + to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
//	}
//
//	mg_bes->GetXaxis()->SetLimits(-2, 78);
//	mg_bes->GetXaxis()->SetRangeUser(-2, 78);
////	if (!zoom) {
////		mg_bes->GetYaxis()->SetLimits(0.9935, 1.0185);
////		mg_bes->GetYaxis()->SetRangeUser(0.9935, 1.0185);
////	}
//
////	mg->GetXaxis()->SetTitle("Energy (GeV)");
//	mg_bes->GetYaxis()->SetTitle("Kurtosis");
//
//	mg_bes->Draw("AP");
//
//	leg_bes->SetMargin(0.1); leg_bes->Draw();
//
//	//gPad->SetTopMargin(0.04);
//	//gPad->SetLeftMargin(0.12);
//	//gPad->SetRightMargin(0.04);
//
//	can_bes->cd(2);
//	TMultiGraph* mg_div_bes = new TMultiGraph();
//	mg_div_bes->SetName("Efficiencies_Divided_BES1");
//
//	TLegend* leg_div_bes = new TLegend(0.3, 0.21, 0.3, 0.21);
//	leg_div_bes->SetBorderSize(0);
//	leg_div_bes->SetFillStyle(0);
//
//	for (pair<float, plot_data> eff : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
//		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][eff.first].val, energy_err, bes1["divide"][eff.first].stat);
//		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + to_string(eff.first)).data());
//		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][eff.first].val, energy_err, bes1["divide"][eff.first].sys);
//
//		bes1_ratio_div_def_g->SetMarkerStyle(marker_style[eff.first]);
//		bes1_ratio_div_def_g->SetMarkerColor(color_div[eff.first]);
//		bes1_ratio_div_def_g->SetLineColor(color_div[eff.first]);
//		bes1_ratio_div_def_g->SetMarkerSize(marker_size[eff.first]);
//		bes1_ratio_div_sys_g->SetLineColor(color_div[eff.first]);
//
//		mg_div_bes->Add(bes1_ratio_div_def_g, "APZ");
//		mg_div_bes->Add(bes1_ratio_div_sys_g, "[]");
//
//		leg_div_bes->AddEntry(bes1_ratio_div_def_g, (to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
//	}
//
//	mg_div_bes->GetXaxis()->SetLimits(-2, 78);
//	mg_div_bes->GetXaxis()->SetRangeUser(-2, 78);
//	if (!zoom) {
//		mg_div_bes->GetYaxis()->SetLimits(0.9935, 1.0185);
//		mg_div_bes->GetYaxis()->SetRangeUser(0.9935, 1.0185);
//	}
//
//	mg_div_bes->GetXaxis()->SetTitle("Energy (GeV)");
//	mg_div_bes->GetYaxis()->SetTitle("Kurtosis");
//
//	mg_div_bes->Draw("AP");
//
//	TLine* one_line = new TLine(0, 1, 80, 1);
//	one_line->SetLineStyle(2);
//	one_line->Draw();
//
//	leg_div_bes->SetMargin(0.1); leg_div_bes->Draw();
//
//	//gPad->SetTopMargin(0.04);
//	//gPad->SetLeftMargin(0.12);
//	//gPad->SetRightMargin(0.04);
//
//	can_bes->Update();
//	can_bes->Write();
//
//	delete can_bes;
//
//
//	// AMPT
//	TCanvas* can_ampt = new TCanvas(("AMPT_Efficiency_vs_energy_can_" + type_name).data(), ("Rapidity vs Energy " + type_name).data(), 955, 805);
//	can_ampt->SetPad(0.01, 0.01, 0.99, 0.99);
//	can_ampt->Divide(1, 2, 0, 0);
//
//	can_ampt->cd(1);
//	TMultiGraph* mg_ampt = new TMultiGraph();
//	mg_ampt->SetName("Efficiencies_Raw_Mixed_AMPT");
//
//	TLegend* leg_ampt = new TLegend(0.3, 0.21, 0.3, 0.21);
//	leg_ampt->SetBorderSize(0);
//	leg_ampt->SetFillStyle(0);
//
//	for (pair<float, plot_data> eff : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
//		TGraphErrors* ampt_ratio_raw_def_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][eff.first].val, energy_err, ampt["raw"][eff.first].stat);
//		ampt_ratio_raw_def_g->SetNameTitle(("ampt_raw_" + to_string(eff.first).substr(0, 3)).data());
//		TGraphErrors* ampt_ratio_raw_sys_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][eff.first].val, energy_err, ampt["raw"][eff.first].sys);
//
//		ampt_ratio_raw_def_g->SetMarkerStyle(marker_style[eff.first]);
//		ampt_ratio_raw_def_g->SetMarkerColor(color_raw[eff.first]);
//		ampt_ratio_raw_def_g->SetLineColor(color_raw[eff.first]);
//		ampt_ratio_raw_def_g->SetMarkerSize(marker_size[eff.first]);
//		ampt_ratio_raw_sys_g->SetLineColor(color_raw[eff.first]);
//
//		TGraphErrors* ampt_ratio_mix_def_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][eff.first].val, energy_err, ampt["mix"][eff.first].stat);
//		ampt_ratio_mix_def_g->SetNameTitle(("ampt_mix_" + to_string(eff.first).substr(0, 3)).data());
//		TGraphErrors* ampt_ratio_mix_sys_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][eff.first].val, energy_err, ampt["mix"][eff.first].sys);
//
//		ampt_ratio_mix_def_g->SetMarkerStyle(marker_style[eff.first]);
//		ampt_ratio_mix_def_g->SetMarkerColor(color_mix[eff.first]);
//		ampt_ratio_mix_def_g->SetLineColor(color_mix[eff.first]);
//		ampt_ratio_mix_def_g->SetMarkerSize(marker_size[eff.first]);
//		ampt_ratio_mix_sys_g->SetLineColor(color_mix[eff.first]);
//
//		mg_ampt->Add(ampt_ratio_mix_def_g, "APZ");
//		mg_ampt->Add(ampt_ratio_mix_sys_g, "[]");
//		mg_ampt->Add(ampt_ratio_raw_def_g, "P");
//		mg_ampt->Add(ampt_ratio_raw_sys_g, "[]");
//
//		leg_ampt->AddEntry(ampt_ratio_raw_def_g, ("Raw " + to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
//		leg_ampt->AddEntry(ampt_ratio_mix_def_g, ("Mixed " + to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
//	}
//
//	mg_ampt->GetXaxis()->SetLimits(-2, 78);
//	mg_ampt->GetXaxis()->SetRangeUser(-2, 78);
////	if (!zoom) {
////		mg_ampt->GetYaxis()->SetLimits(0.9935, 1.0185);
////		mg_ampt->GetYaxis()->SetRangeUser(0.9935, 1.0185);
////	}
//
////	mg->GetXaxis()->SetTitle("Energy (GeV)");
//	mg_ampt->GetYaxis()->SetTitle("Kurtosis");
//
//	mg_ampt->Draw("AP");
//
//	leg_ampt->SetMargin(0.1); leg_ampt->Draw();
//
//	//gPad->SetTopMargin(0.04);
//	//gPad->SetLeftMargin(0.12);
//	//gPad->SetRightMargin(0.04);
//
//	can_ampt->cd(2);
//	TMultiGraph* mg_div_ampt = new TMultiGraph();
//	mg_div_ampt->SetName("Efficiencies_Raw_Mixed");
//
//	TLegend* leg_div_ampt = new TLegend(0.3, 0.21, 0.3, 0.21);
//	leg_div_ampt->SetBorderSize(0);
//	leg_div_ampt->SetFillStyle(0);
//
//	for (pair<float, plot_data> eff : bes1["raw"]) {  // Iterate over rapidities, left to right on plot
//		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][eff.first].val, energy_err, ampt["divide"][eff.first].stat);
//		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + to_string(eff.first)).data());
//		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][eff.first].val, energy_err, ampt["divide"][eff.first].sys);
//
//		ampt_ratio_div_def_g->SetMarkerStyle(marker_style[eff.first]);
//		ampt_ratio_div_def_g->SetMarkerColor(color_div[eff.first]);
//		ampt_ratio_div_def_g->SetLineColor(color_div[eff.first]);
//		ampt_ratio_div_def_g->SetMarkerSize(marker_size[eff.first]);
//		ampt_ratio_div_sys_g->SetLineColor(color_div[eff.first]);
//
//		mg_div_ampt->Add(ampt_ratio_div_def_g, "APZ");
//		mg_div_ampt->Add(ampt_ratio_div_sys_g, "[]");
//
//		leg_div_ampt->AddEntry(ampt_ratio_div_def_g, (to_string(eff.first*100).substr(0, 3) + "%").data(), "p");
//	}
//
//	mg_div_ampt->GetXaxis()->SetLimits(-2, 78);
//	mg_div_ampt->GetXaxis()->SetRangeUser(-2, 78);
////	if (!zoom) {
////		mg_div_ampt->GetYaxis()->SetLimits(0.9935, 1.0185);
////		mg_div_ampt->GetYaxis()->SetRangeUser(0.9935, 1.0185);
////	}
//
//	mg_div_ampt->GetXaxis()->SetTitle("Energy (GeV)");
//	mg_div_ampt->GetYaxis()->SetTitle("Kurtosis");
//
//	mg_div_ampt->Draw("AP");
//
//	TLine* one_line_ampt = new TLine(0, 1, 80, 1);
//	one_line_ampt->SetLineStyle(2);
//	one_line_ampt->Draw();
//
//	leg_div_ampt->SetMargin(0.1); leg_div_ampt->Draw();
//
//	//gPad->SetTopMargin(0.04);
//	//gPad->SetLeftMargin(0.12);
//	//gPad->SetRightMargin(0.04);
//
//	can_ampt->Update();
//	can_ampt->Write();
//
//	delete can_ampt;
}


void DividegPad(Int_t nx, Int_t ny,
                Float_t l, Float_t r, Float_t t, Float_t b)
{  // couet from ROOT forum
   Int_t ix, iy, n=0;
   Double_t x1, x2, y1, y2;
   Double_t dx = ((1-r)*(1-l))/((1-r)*(1-l)*(nx-2)-r+2-l);
   Double_t dl = dx/(1-l);
   Double_t dy = ((1-t)*(1-b))/((1-t)*(1-b)*(ny-2)-t+2-b);
   Double_t db = dy/(1-b);
   char *name  = new char [strlen(gPad->GetName())+6];

   y1 = 0;
   y2 = db;
   for (iy=0; iy<ny; iy++) {
      x1 = 0;
      x2 = dl;
      for (ix=0;ix<nx;ix++) {
         if (x1 > x2) continue;
         n++;
         sprintf(name,"%s_%d",gPad->GetName(),n);
         TPad *pad = new TPad(name,name,x1,y1,x2,y2,0);
         if (ix==0)    pad->SetLeftMargin(l);
         if (ix==nx-1) pad->SetRightMargin(r);
         if (iy==ny-1) pad->SetTopMargin(t);
         if (iy==0)    pad->SetBottomMargin(b);
         x1 = x2;
         if (ix==nx-2) x2 = 1;
         else          x2 = x1+dx;
         pad->SetNumber(n);
         pad->Draw();
      }
      y1 = y2;
      if (iy==ny-2) y2 = 1;
      else          y2 = y1+dy;
   }
}
