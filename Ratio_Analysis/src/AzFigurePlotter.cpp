
#include <vector>

#include "AzFigurePlotter.h"
#include "plotting.h"

#include <TFile.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>

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
	vector<int> divs{ 60, 72, 90, 120, 180 };
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

	moments_vs_energy_plot(energies, bes1_ratio, ampt_ratio, "Ratio");
	moments_vs_energy_plot(energies, bes1_pull, ampt_pull, "Pull");

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

	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_can_"+type_name).data(), "Kurtosis vs Energy", 955, 805);
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

	map<int, int> div_color{ {60, 1}, {72, 2}, {90, 3}, {120, 4}, {180, 5} };
	map<int, int> div_marker_style{ {60, 1}, {72, 2}, {90, 3}, {120, 4}, {180, 5} };
	map<int, int> div_marker_size{ {60, 2}, {72, 2}, {90, 2}, {120, 2}, {180, 2} };


	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_divs_can_" + type_name).data(), "Kurtosis vs Energy with Bin Width", 955, 805);
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

		bes1_ratio_def_g->SetMarkerStyle(div_marker_style[div.first]);
		bes1_ratio_def_g->SetMarkerColor(div_color[div.first]);
		bes1_ratio_def_g->SetLineColor(div_color[div.first]);
		bes1_ratio_def_g->SetMarkerSize(div_marker_size[div.first]);
		bes1_ratio_sys_g->SetLineColor(div_color[div.first]);

		ampt_ratio_def_g->SetMarkerStyle(div_marker_style[div.first]);
		ampt_ratio_def_g->SetMarkerColor(div_color[div.first]);
		ampt_ratio_def_g->SetLineColor(div_color[div.first]);
		ampt_ratio_def_g->SetMarkerSize(div_marker_size[div.first]);
		ampt_ratio_sys_g->SetLineColor(div_color[div.first]);

		mg->Add(bes1_ratio_def_g, "APLZ");
		mg->Add(ampt_ratio_def_g, "PL");
		mg->Add(bes1_ratio_sys_g, "[]");
		mg->Add(ampt_ratio_sys_g, "[]");

		leg->AddEntry(bes1_ratio_def_g, ("BES1_"+to_string(div.first)).data(), "lp");
		leg->AddEntry(ampt_ratio_def_g, ("AMPT_" + to_string(div.first)).data(), "lp");
	}

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);

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


void AzFigurePlotter::kurt_vs_energy_cents_plot(vector<int> energies, map<int, plot_data> bes1, map<int, plot_data> ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	for (double energy : energies) {
		energy_bes1.push_back(energy - 0.6);
		energy_ampt.push_back(energy + 0.6);
	}

	map<int, int> cent_color{ {8, 1}, {7, 2}, {6, 3}, {5, 4}, {4, 5}, {3, 6}, {2, 7}, {1, 8} };
	map<int, int> cent_marker_style{ {8, 1}, {7, 2}, {6, 3}, {5, 4}, {4, 5}, {3, 6}, {2, 7}, {1, 8} };
	map<int, int> cent_marker_size{ {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2} };


	TCanvas* can = new TCanvas(("Kurtosis_vs_energy_cents_can_" + type_name).data(), "Kurtosis vs Energy with Centrality", 955, 805);
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

		bes1_ratio_def_g->SetMarkerStyle(cent_marker_style[cent.first]);
		bes1_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetLineColor(cent_color[cent.first]);
		bes1_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		bes1_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		ampt_ratio_def_g->SetMarkerStyle(cent_marker_style[cent.first]);
		ampt_ratio_def_g->SetMarkerColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetLineColor(cent_color[cent.first]);
		ampt_ratio_def_g->SetMarkerSize(cent_marker_size[cent.first]);
		ampt_ratio_sys_g->SetLineColor(cent_color[cent.first]);

		mg->Add(bes1_ratio_def_g, "APLZ");
		mg->Add(ampt_ratio_def_g, "PL");
		mg->Add(bes1_ratio_sys_g, "[]");
		mg->Add(ampt_ratio_sys_g, "[]");

		leg->AddEntry(bes1_ratio_def_g, ("BES1_" + to_string(cent.first)).data(), "lp");
		leg->AddEntry(ampt_ratio_def_g, ("AMPT_" + to_string(cent.first)).data(), "lp");
	}

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);

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


void AzFigurePlotter::moments_vs_energy_plot(vector<int> energies, map<string, map<string, plot_data>> bes1, map<string, map<string, plot_data>> ampt, string type_name) {
	vector<double> energy_err{ 0, 0, 0, 0, 0, 0 };
	vector<double> energy_val(energies.begin(), energies.end());
	vector<double> energy_bes1;
	vector<double> energy_ampt;
	for (double energy : energies) {
		energy_bes1.push_back(energy - 0.6);
		energy_ampt.push_back(energy + 0.6);
	}

	map<string, int> color{ {"raw", kBlue}, {"mix", kGreen}, {"divide", kRed} };
	map<string, int> marker_style{ {"raw", 2}, {"mix", 3}, {"divide", 4} };
	map<string, int> marker_size{ {"raw", 2}, {"mix", 2}, {"divide", 2} };

	TCanvas* can = new TCanvas(("Moments_vs_energy_can" + type_name).data(), "Moments vs Energy", 955, 805);
	can->Divide((int)bes1["raw"].size(), 2, 0.001, 0.001);
	int can_index = 1;
	for (pair<string, plot_data> stat : bes1["raw"]) {  // Iterate over stats, left to right on plot

		// Raw vs mixed on top plot
		can->cd(can_index);
		TMultiGraph* mg = new TMultiGraph();
		mg->SetName((stat.first + "_raw_mixed").data());

		TGraphErrors* bes1_ratio_raw_def_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][stat.first].val, energy_err, bes1["raw"][stat.first].stat);
		TGraphErrors* ampt_ratio_raw_def_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][stat.first].val, energy_err, ampt["raw"][stat.first].stat);
		bes1_ratio_raw_def_g->SetNameTitle(("bes1_raw_" + stat.first).data());
		ampt_ratio_raw_def_g->SetNameTitle(("ampt_raw_" + stat.first).data());
		TGraphErrors* bes1_ratio_raw_sys_g = graph_x_vs_y_err(energy_bes1, bes1["raw"][stat.first].val, energy_err, bes1["raw"][stat.first].sys);
		TGraphErrors* ampt_ratio_raw_sys_g = graph_x_vs_y_err(energy_ampt, ampt["raw"][stat.first].val, energy_err, ampt["raw"][stat.first].sys);

		bes1_ratio_raw_def_g->SetMarkerStyle(marker_style["raw"]);
		bes1_ratio_raw_def_g->SetMarkerColor(color["raw"]);
		bes1_ratio_raw_def_g->SetLineColor(color["raw"]);
		bes1_ratio_raw_def_g->SetMarkerSize(marker_size["raw"]);
		bes1_ratio_raw_sys_g->SetLineColor(color["raw"]);

		ampt_ratio_raw_def_g->SetMarkerStyle(marker_style["raw"]);
		ampt_ratio_raw_def_g->SetMarkerColor(color["raw"]);
		ampt_ratio_raw_def_g->SetLineColor(color["raw"]);
		ampt_ratio_raw_def_g->SetMarkerSize(marker_size["raw"]);
		ampt_ratio_raw_sys_g->SetLineColor(color["raw"]);

		TGraphErrors* bes1_ratio_mix_def_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][stat.first].val, energy_err, bes1["mix"][stat.first].stat);
		TGraphErrors* ampt_ratio_mix_def_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][stat.first].val, energy_err, ampt["mix"][stat.first].stat);
		bes1_ratio_mix_def_g->SetNameTitle(("bes1_mix_" + stat.first).data());
		ampt_ratio_mix_def_g->SetNameTitle(("ampt_mix_" + stat.first).data());
		TGraphErrors* bes1_ratio_mix_sys_g = graph_x_vs_y_err(energy_bes1, bes1["mix"][stat.first].val, energy_err, bes1["mix"][stat.first].sys);
		TGraphErrors* ampt_ratio_mix_sys_g = graph_x_vs_y_err(energy_ampt, ampt["mix"][stat.first].val, energy_err, ampt["mix"][stat.first].sys);

		bes1_ratio_mix_def_g->SetMarkerStyle(marker_style["mix"]);
		bes1_ratio_mix_def_g->SetMarkerColor(color["mix"]);
		bes1_ratio_mix_def_g->SetLineColor(color["mix"]);
		bes1_ratio_mix_def_g->SetMarkerSize(marker_size["mix"]);
		bes1_ratio_mix_sys_g->SetLineColor(color["mix"]);

		ampt_ratio_mix_def_g->SetMarkerStyle(marker_style["mix"]);
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

		//TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		//leg->AddEntry(bes1_ratio_def_g, ("BES1_" + to_string(cent.first)).data(), "lp");
		//leg->AddEntry(ampt_ratio_def_g, ("AMPT_" + to_string(cent.first)).data(), "lp");
		//leg->SetBorderSize(0);
		//leg->SetFillStyle(0);	

		mg->GetXaxis()->SetLimits(0, 80);
		mg->GetXaxis()->SetRangeUser(0, 80);
		//mg->GetYaxis()->SetLimits(0.9955, 1.0145);
		//mg->GetYaxis()->SetRangeUser(0.9955, 1.0145);

		mg->GetXaxis()->SetTitle("Energy (GeV)");
		//mg->GetYaxis()->SetTitle("Kurtosis");

		mg->Draw("AP");


		// Divided plot on bottom
		can->cd(can_index + (int)bes1["raw"].size());
		TMultiGraph* mg_div = new TMultiGraph();
		mg_div->SetName((stat.first + "_divided").data());

		TGraphErrors* bes1_ratio_div_def_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][stat.first].val, energy_err, bes1["divide"][stat.first].stat);
		TGraphErrors* ampt_ratio_div_def_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][stat.first].val, energy_err, ampt["divide"][stat.first].stat);
		bes1_ratio_div_def_g->SetNameTitle(("bes1_div_" + stat.first).data());
		ampt_ratio_div_def_g->SetNameTitle(("ampt_div_" + stat.first).data());
		TGraphErrors* bes1_ratio_div_sys_g = graph_x_vs_y_err(energy_bes1, bes1["divide"][stat.first].val, energy_err, bes1["divide"][stat.first].sys);
		TGraphErrors* ampt_ratio_div_sys_g = graph_x_vs_y_err(energy_ampt, ampt["divide"][stat.first].val, energy_err, ampt["divide"][stat.first].sys);

		bes1_ratio_div_def_g->SetMarkerStyle(marker_style["divide"]);
		bes1_ratio_div_def_g->SetMarkerColor(color["divide"]);
		bes1_ratio_div_def_g->SetLineColor(color["divide"]);
		bes1_ratio_div_def_g->SetMarkerSize(marker_size["divide"]);
		bes1_ratio_div_sys_g->SetLineColor(color["divide"]);

		ampt_ratio_div_def_g->SetMarkerStyle(marker_style["divide"]);
		ampt_ratio_div_def_g->SetMarkerColor(color["divide"]);
		ampt_ratio_div_def_g->SetLineColor(color["divide"]);
		ampt_ratio_div_def_g->SetMarkerSize(marker_size["divide"]);
		ampt_ratio_div_sys_g->SetLineColor(color["divide"]);

		mg_div->Add(bes1_ratio_div_def_g, "APLZ");
		mg_div->Add(ampt_ratio_div_def_g, "PL");
		mg_div->Add(bes1_ratio_div_sys_g, "[]");
		mg_div->Add(ampt_ratio_div_sys_g, "[]");

		//TLegend* leg = new TLegend(0.3, 0.21, 0.3, 0.21);
		//leg->AddEntry(bes1_ratio_def_g, ("BES1_" + to_string(cent.first)).data(), "lp");
		//leg->AddEntry(ampt_ratio_def_g, ("AMPT_" + to_string(cent.first)).data(), "lp");
		//leg->SetBorderSize(0);
		//leg->SetFillStyle(0);	

		mg_div->GetXaxis()->SetLimits(0, 80);
		mg_div->GetXaxis()->SetRangeUser(0, 80);
		//mg_div->GetYaxis()->SetLimits(0.9955, 1.0145);
		//mg_div->GetYaxis()->SetRangeUser(0.9955, 1.0145);

		mg_div->GetXaxis()->SetTitle("Energy (GeV)");
		//mg_div->GetYaxis()->SetTitle("Kurtosis");

		mg_div->Draw("AP");

		TLine* one_line = new TLine(0, 1, 80, 1);
		one_line->SetLineStyle(2);
		one_line->Draw();

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