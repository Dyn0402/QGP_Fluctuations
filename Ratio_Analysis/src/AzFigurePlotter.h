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

void DividegPad(Int_t nx, Int_t ny, Float_t l, Float_t r, Float_t t, Float_t b);

class AzFigurePlotter : public AzimuthBinAnalyzer {
public:
	// Structors
	AzFigurePlotter();
	~AzFigurePlotter();

	// Getters

	// Setters
	void set_zoom(bool zoom);

	// Doers
	void plot_paper_figs();
	void kurt_vs_energy();
	void kurt_vs_divs_w_energy();
	void kurt_vs_energy_cents();
	void sd_vs_energy_cents();
	void moments_vs_energy();
	void kurt_vs_energy_w_rapidity();
	void kurt_vs_rapidity_w_energy();
	void kurt_with_efficiency();
	void kurt_vs_efficiency();

private:
	// Attributes
	bool zoom = false;
	float def_text_size = 18;

	// Doers
	void kurt_vs_energy_plot(vector<int> energies, plot_data bes1, plot_data ampt, string type_name);
	void kurt_vs_energy_cf_plot(vector<int> energies, plot_data bes1, plot_data ampt, plot_data cf, string type_name);
	void kurt_vs_divs_w_energy_plot(map<int, string> energies, vector<int> divs, map<string, map<int, plot_data>> bes1, map<string, map<int, plot_data>> ampt, map<string, map<int, plot_data>> cf, string type_name);
	void kurt_vs_energy_cents_plot(vector<int> energies, map<int, plot_data> bes1, map<int, plot_data> ampt, string type_name);
	void sd_vs_energy_cents_plot(vector<int> energies, map<int, plot_data> bes1, map<int, plot_data> ampt, string type_name);
	void sd_vs_energy_cf_plot(vector<int> energies, plot_data bes1, plot_data ampt, plot_data cf, string type_name);
	void moments_vs_energy_plot(vector<int> energies, map<string, map<string, plot_data>> bes1, map<string, map<string, plot_data>> ampt, vector<string> stat_order, string type_name);
	void kurt_vs_energy_w_rapid_plot(vector<int> energies, map<string, map<float, plot_data>> bes1, map<string, map<float, plot_data>> ampt, string type_name);
	void kurt_vs_rapid_w_energy_plot(map<int, string> energies, vector<float> rapidities, map<string, map<int, plot_data>> bes1, map<string, map<int, plot_data>> ampt, string type_name);
	void kurt_vs_rapid_w_energy_cf_plot(map<int, string> energies, vector<float> rapidities, map<string, map<int, plot_data>> bes1, map<string, map<int, plot_data>> ampt, map<string, map<int, plot_data>> cf, string type_name);
	void kurt_with_efficiency_plot(vector<int> energies, map<string, map<float, plot_data>> bes1, map<string, map<float, plot_data>> ampt, string type_name);
	void kurt_vs_efficiency_plot(map<int, string> energies, vector<float> effs, map<string, map<int, plot_data>> bes1, map<string, map<int, plot_data>> ampt, map<string, map<int, plot_data>> cf, string type_name);

};
