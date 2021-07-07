#pragma once

#include "AzimuthBinAnalyzer.h"

//template <typename T>
//struct plot_data {
//	vector<T> def;
//	vector<T> stat;
//	vector<T> sys;
//};

struct plot_data {
	vector<double> val;
	vector<double> stat;
	vector<double> sys;
};

class AzFigurePlotter : public AzimuthBinAnalyzer {
public:
	// Structors
	AzFigurePlotter();
	~AzFigurePlotter();

	// Getters

	// Setters

	// Doers
	void plot_paper_figs();
	void kurt_vs_energy();
	void kurt_vs_energy_divs();
	void kurt_vs_energy_cents();
	void moments_vs_energy();
	void kurt_vs_rapidity();

private:
	// Attributes

	// Doers
	void kurt_vs_energy_plot(vector<int> energies, plot_data bes1, plot_data ampt, string type_name);
	void kurt_vs_energy_divs_plot(vector<int> energies, map<int, plot_data> bes1, map<int, plot_data> ampt, string type_name);
	void kurt_vs_energy_cents_plot(vector<int> energies, map<int, plot_data> bes1, map<int, plot_data> ampt, string type_name);
	void moments_vs_energy_plot(vector<int> energies, map<string, map<string, plot_data>> bes1, map<string, map<string, plot_data>> ampt, string type_name);
	void kurt_vs_rapid_plot(vector<int> energies, map<string, map<float, plot_data>> bes1, map<string, map<float, plot_data>> ampt, string type_name);

};
