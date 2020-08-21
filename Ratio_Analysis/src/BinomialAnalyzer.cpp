/*
 * BinomialAnalyzer.cpp
 *
 *  Created on: Jan 30, 2020
 *      Author: Dylan Neff
 */




#include <vector>

#include <TStyle.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TLine.h>
#include <TPad.h>

#include "BinomialAnalyzer.h"

using namespace std;


// Structors

BinomialAnalyzer::BinomialAnalyzer() {
	out_root = NULL;
}

BinomialAnalyzer::~BinomialAnalyzer() {
	out_root->Close();
	delete out_root;
}


// Getters

string BinomialAnalyzer::get_out_root_name() {
	return out_root_name;
}


// Setters

void BinomialAnalyzer::set_out_root_name(string name) {
	out_root_name = name;
}

void BinomialAnalyzer::set_in_path(string in_path) {
	this->in_path = in_path;
}

void BinomialAnalyzer::set_in_mix_path(string in_mix_path) {
	this->in_mix_path = in_mix_path;
}

void BinomialAnalyzer::set_out_path(string out_path) {
	this->out_path = out_path;
}

void BinomialAnalyzer::set_energies(vector<int> energies) {
	energy_list = energies;
}

void BinomialAnalyzer::set_divs(vector<int> divs) {
	this->divs = divs;
}

void BinomialAnalyzer::set_sets(map<string, vector<int>> set_name) {
	this->sets = set_name;
}

void BinomialAnalyzer::set_set_combos(map<string, vector<string>> combos) {
	this->set_combos = combos;
}

void BinomialAnalyzer::set_centralities(vector<int> centralities) {
	this->centralities = centralities;
}

void BinomialAnalyzer::set_can_wh(int can_width, int can_height) {
	canvas_width = can_width;
	canvas_height = can_height;
}

// Doers

void BinomialAnalyzer::analyze() {
	out_root = new TFile((out_path+out_root_name).data(), "RECREATE");
	analyze_sets();
}


void BinomialAnalyzer::analyze_sets() {
	for(pair<string, vector<int>> set:sets) {
		analyze_set(set.first, set.second);
	}
	cout << "Combining Set Combos" << endl;
	combine_sets();
}


void BinomialAnalyzer::combine_sets() {
	TDirectory *combos_dir = out_root->mkdir("Combined_Sets");
	for(auto &set:set_combos) {
		TDirectory *combo_dir = combos_dir->mkdir(set.first.data());
		plot_combo_sets(set.second, combo_dir);
	}
}


void BinomialAnalyzer::plot_combo_sets(vector<string> set_names, TDirectory *dir) {
//	plot_all_stats(sys_stats[set_name]["slice_stats"], dir);
//	plot_all_stat_ratios(sys_stats[set_name]["slice_stat_ratios"], dir);
	map<string, map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>>> slice_stat_combos;
	for(auto &set_name:set_names) {
		for(auto &slice_stat:sys_stats[set_name]) {
			for(auto &data_type:slice_stat.second) {
				for(auto &energy:data_type.second) {
					for(auto &div:energy.second) {
						for(auto &cent:div.second) {
							for(auto &total_part:cent.second) {
								for(auto &stat:total_part.second) {
									slice_stat_combos[slice_stat.first][set_name+"_"+data_type.first][energy.first][div.first][cent.first][total_part.first][stat.first] = stat.second;
								}
							}
						}
					}
				}
			}
		}
	}
	plot_all_divided_stats(slice_stat_combos["slice_divided_stats"], slice_stat_combos["slice_divided_stat_ratios"], dir);
	plot_mix_divided_stats(slice_stat_combos["slice_mix_divided_stats"], dir);
}


void BinomialAnalyzer::analyze_set(string set_name, vector<int> set_nums) {
//	ROOT::EnableThreadSafety();
//	{
//		ThreadPool pool(1);//thread::hardware_concurrency());
//		TDirectory *set_dir = out_root->mkdir((set_name.data()));
//		for(int set_num = set_nums[0]; set_num <= set_nums[1]; set_num++) {
//			pool.enqueue(&BinomialAnalyzer::analyze_subset, this, set_num, set_dir);
////			analyze_subset(set_name, set_num, set_dir);
//		}
//	}
	TDirectory *set_dir = out_root->mkdir((set_name.data()));
	for(int set_num = set_nums[0]; set_num <= set_nums[1]; set_num++) {
		analyze_subset(set_name, set_num, set_dir);
	}
	cout << "Combining " << set_name << " sets" << endl;
	combine_subsets(set_name);
	TDirectory *combined_dir = set_dir->mkdir("Combined");
	plot_set_combo(set_name, combined_dir);
}


void BinomialAnalyzer::combine_subsets(string set_name) {
	for(auto &slice_stat:combined_stats[set_name]) {
		for(auto &data_type:slice_stat.second) {
			for(auto &energy:data_type.second) {
				for(auto &div:energy.second) {
					for(auto &cent:div.second) {
						for(auto &total_part:cent.second) {
							for(auto &stat:total_part.second) {
								sys_stats[set_name][slice_stat.first][data_type.first][energy.first][div.first][cent.first][total_part.first][stat.first] = {median(stat.second), sample_sd(stat.second)};
							}
						}
					}
				}
			}
		}
	}
}


void BinomialAnalyzer::plot_set_combo(string set_name, TDirectory *dir) {
	plot_all_stats(sys_stats[set_name]["slice_stats"], dir);
	plot_all_stat_ratios(sys_stats[set_name]["slice_stat_ratios"], dir);
	plot_all_divided_stats(sys_stats[set_name]["slice_divided_stats"], sys_stats[set_name]["slice_divided_stat_ratios"], dir);
	plot_mix_divided_stats(sys_stats[set_name]["slice_mix_divided_stats"], dir);
}


void BinomialAnalyzer::analyze_subset(string set_name, int set_num, TDirectory *set_dir) {
	cout << "Starting Set " + set_name + to_string(set_num) << endl << endl;
	string path = in_path + set_name + to_string(set_num) + "/";
	string path_mix = in_mix_path + set_name + to_string(set_num) + "/";

	map<int, map<int, map<int, AzimuthBinData>>> data = get_data(path);
	map<int, map<int, map<int, AzimuthBinData>>> data_mix = get_data(path_mix);

	TDirectory *set_num_dir = set_dir->mkdir((set_name + to_string(set_num)).data());

	TDirectory *slice_dir = set_num_dir->mkdir("Slice Distributions");
	plot_slices(data, slice_dir->mkdir("raw"));
	plot_slices(data_mix, slice_dir->mkdir("mix"));

	map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> slice_stats {{"raw", get_slice_stats(data)}, {"mix", get_slice_stats(data_mix)}};
	plot_all_stats(slice_stats, set_num_dir);
//	map<string, map<int, map<int, map<int,map<int, map<string, Measure>>>>>> diff_slice_stats {{"raw", get_diff_slice_stats(data)}, {"mix", get_diff_slice_stats(data_mix)}};

	map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> slice_stat_ratios {{"raw", get_stat_ratios(slice_stats["raw"])}, {"mix", get_stat_ratios(slice_stats["mix"])}};
	plot_all_stat_ratios(slice_stat_ratios, set_num_dir);

	map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> slice_divided_stats {{"raw", divide_binomial(slice_stats["raw"])}, {"mix", divide_binomial(slice_stats["mix"])}};
	map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> slice_divided_stat_ratios {{"raw", divide_binomial(slice_stat_ratios["raw"])}, {"mix", divide_binomial(slice_stat_ratios["mix"])}};
	plot_all_divided_stats(slice_divided_stats, slice_divided_stat_ratios, set_num_dir);

	map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> slice_mix_divided_stats {{"raw/mix", divide_mix(slice_stats)}};
	plot_mix_divided_stats(slice_mix_divided_stats, set_num_dir);

	// maps are copied in line below, clean up for optimization
	map<string, map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>>> slice_stats_combined = {{"slice_stats", slice_stats}, {"slice_stat_ratios", slice_stat_ratios}, {"slice_divided_stats", slice_divided_stats}, {"slice_divided_stat_ratios", slice_divided_stat_ratios}, {"slice_mix_divided_stats", slice_mix_divided_stats}};

	for(auto &slice_stat:slice_stats_combined) {
		for(auto &data_type:slice_stat.second) {
			for(auto &energy:data_type.second) {
				for(auto &div:energy.second) {
					for(auto &cent:div.second) {
						for(auto &total_part:cent.second) {
							for(auto &stat:total_part.second) {
								combined_stats[set_name][slice_stat.first][data_type.first][energy.first][div.first][cent.first][total_part.first][stat.first].push_back(stat.second);
							}
						}
					}
				}
			}
		}
	}
}


map<int, map<int, map<int, AzimuthBinData>>> BinomialAnalyzer::get_data(string path, int min_num_events) {
	map<int, map<int, map<int, AzimuthBinData>>> data;

	for(int const &energy:energy_list) {

		string energy_path = path + to_string(energy) + "GeV/";

		for(int const &div:divs) {

			// Get centralities found in file names of given directory path.
			for(int const &cent:get_centrals(energy_path, div)) {
				if(find(centralities.begin(), centralities.end(), cent) == centralities.end()) { continue; }
				AzimuthBinData az_data(div);
				az_data.read_data_from_dir(energy_path, div, cent);  // Read azimuthal bin data from file path

				if(az_data.get_num_bins() <= min_num_events) {
					if(div == divs[0]) {
						cout << "Centrality " << cent << " with only " << az_data.get_num_bins() << " entries. Skipping." << endl;
					}
				} else {
					data[energy][div][cent] = az_data;  // Store azimuthal bin data in data under corresponding centrality (refmult2) value
				}
			}
		}
	}

	return data;
}


// Calculate stats for each cumulant_order for each centrality for each number of divisions for each energy.
map<int, map<int, map<int, map<int, map<string, Measure>>>>> BinomialAnalyzer::get_slice_stats(map<int, map<int, map<int, AzimuthBinData>>> &data) {
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> stats;
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(stat_threads);  // Threading currently doesn't work, set to 1 thread
		for(auto &energy:data) {
			for(auto &div:energy.second) {
				for(auto &cent:div.second) {
					for(auto &total_particle:cent.second.get_bin_data()) {
						int events = 0;
						for(auto &bin_particles:total_particle.second) { events += bin_particles.second; }
						if(events > min_events) {
							pool.enqueue(&BinomialAnalyzer::calc_stat, this, total_particle.second, total_particle.first, energy.first, div.first, cent.first, &stats);
						}
					}
				}
			}
		}
	}
	return stats;
}



// Calculate diff stats for each cumulant_order for each centrality for each number of divisions for each energy.
//map<int, map<int, map<int, map<int, map<string, Measure>>>>> BinomialAnalyzer::get_diff_slice_stats(map<int, map<int, map<int, AzimuthBinData>>> &data) {
//	map<int, map<int, map<int, map<int, map<string, Measure>>>>> stats;
//	ROOT::EnableThreadSafety();
//	{
//		ThreadPool pool(stat_threads);  // Threading currently doesn't work, set to 1 thread
//		for(auto &energy:data) {
//			for(auto &div:energy.second) {
//				for(auto &cent:div.second) {
//					for(auto &total_proton:cent.second.get_diff_slice_hist()) {
//						int events = 0;
//						for(auto &bin_protons:total_proton.second) { events += bin_protons.second; }
//						if(events > min_events) {
//							pool.enqueue(&BinomialAnalyzer::calc_stat, this, total_proton.second, total_proton.first, energy.first, div.first, cent.first, &stats);
//						}
//					}
//				}
//			}
//		}
//	}
//	return stats;
//}


// Divide each statistic by the corresponding statistic for binomial distribution.
map<int, map<int, map<int, map<int, map<string, Measure>>>>> BinomialAnalyzer::divide_binomial(map<int, map<int, map<int, map<int, map<string, Measure>>>>> &slice_stats) {
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> result;
	for(auto &energy:slice_stats) {
		for(auto &div:energy.second) {
			double p = (double)div.first / 360;
			double q = 1.0 - p;
			for(auto &cent:div.second) {
				for(auto &total_particle:cent.second) {
					int n = total_particle.first;
					for(auto &stat:total_particle.second) {
						if(stat.first == "mean") {
							result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second / (n*p);
						} else if(stat.first == "standard_deviation") {
							result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second / pow(n*p*q,0.5);
						} else if(stat.first == "skewness") {
							result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second / ( (q - p) / pow(n*p*q,0.5) );
						} else if(stat.first == "skewness_sd") {
							result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second / ( q - p );
						} else if(stat.first == "kurtosis") {
							result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second / ( (1 - 6*p*q) / (n*p*q) );
						} else if(stat.first == "kurtosis_variance") {
							result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second / (1 - 6*p*q);
						} else if(stat.first == "variance/mean") {
							result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second / q;
						} else if(stat.first == "sd/mean") {
							result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second / pow(q/(n*p),0.5);
						} else { // Placeholder for higher orders
							result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second * 0.0;
						}
					}
				}
			}
		}
	}
	return(result);
}


// Divide each statistic by the corresponding statistic for binomial distribution.
map<int, map<int, map<int, map<int, map<string, Measure>>>>> BinomialAnalyzer::divide_mix(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats) {
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> result;
	for(auto &energy:slice_stats["raw"]) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &total_particle:cent.second) {
					for(auto &stat:total_particle.second) {
						result[energy.first][div.first][cent.first][total_particle.first][stat.first] = stat.second / slice_stats["mix"][energy.first][div.first][cent.first][total_particle.first][stat.first];
//						string raw = (string)stat.second;
//						string mix = (string)slice_stats["mix"][energy.first][div.first][cent.first][total_particle.first][stat.first];
//						string ratio = (string)result[energy.first][div.first][cent.first][total_particle.first][stat.first];
//						cout << "raw: " << raw << endl;
//						cout << "mix: " << mix << endl;
//						cout << "ratio: " << ratio << endl;
//						this_thread::sleep_for(chrono::seconds(1));
					}
				}
			}
		}
	}
	return(result);
}


// Divide each statistic by the corresponding statistic for binomial distribution.
map<int, map<int, map<int, map<int, map<string, Measure>>>>> BinomialAnalyzer::get_stat_ratios(map<int, map<int, map<int, map<int, map<string, Measure>>>>> &slice_stats) {
	map<int, map<int, map<int, map<int, map<string, Measure>>>>> result;
	for(auto &energy:slice_stats) {
		for(auto &div:energy.second) {
			for(auto &cent:div.second) {
				for(auto &total_particle:cent.second) {
					Measure sd = total_particle.second["standard_deviation"];
					Measure mean = total_particle.second["mean"];
					result[energy.first][div.first][cent.first][total_particle.first]["variance/mean"] = sd*sd / mean;
					result[energy.first][div.first][cent.first][total_particle.first]["sd/mean"] = sd / mean;
				}
			}
		}
	}
	return(result);
}

// Calculate statistics for total_particle slice histogram data.
void BinomialAnalyzer::calc_stat(map<int, int> &slice_data, int particles, int energy, int div, int cent, map<int, map<int, map<int, map<int, map<string, Measure>>>>> *stats) {
	Stats stat(slice_data);
//	cout << "Getting energy " << to_string(energy) << " div " << to_string(div) << " cent " << to_string(cent) << " particle " << to_string(particles) << endl;
	(*stats)[energy][div][cent][particles]["mean"] = stat.get_mean();
	(*stats)[energy][div][cent][particles]["standard_deviation"] = stat.get_standard_deviation();
	(*stats)[energy][div][cent][particles]["skewness"] = stat.get_skewness();
	(*stats)[energy][div][cent][particles]["skewness_sd"] = stat.get_skew_sd();
	(*stats)[energy][div][cent][particles]["kurtosis"] = stat.get_kurtosis();
	(*stats)[energy][div][cent][particles]["kurtosis_variance"] = stat.get_kurt_var();
	(*stats)[energy][div][cent][particles]["non_excess_kurtosis"] = stat.get_non_excess_kurtosis();
}


// Calculate statistics for total_proton slice histogram data.
//void BinomialAnalyzer::calc_stat(map<double, int> &slice_data, int particles, int energy, int div, int cent, map<int, map<int, map<int, map<int, map<string, Measure>>>>> *stats) {
//	Stats stat(slice_data);
////	cout << "Getting energy " << to_string(energy) << " div " << to_string(div) << " cent " << to_string(cent) << " proton " << to_string(particles) << endl;
//	(*stats)[energy][div][cent][particles]["mean"] = stat.get_mean();
//	(*stats)[energy][div][cent][particles]["standard_deviation"] = stat.get_standard_deviation();
//	(*stats)[energy][div][cent][particles]["skewness"] = stat.get_skewness();
//	(*stats)[energy][div][cent][particles]["kurtosis"] = stat.get_kurtosis();
//	(*stats)[energy][div][cent][particles]["non_excess_kurtosis"] = stat.get_non_excess_kurtosis();
//}



// Plotters

void BinomialAnalyzer::draw_proton_bin_plots(map<int, map<int, map<int, AzimuthBinData>>> &data, TDirectory *dir) {
	TDirectory *out_dir = dir->mkdir("Particle_Bin_Plots");

	for(int energy:energy_list) {
		TDirectory *energy_dir = out_dir->mkdir((to_string(energy)+"GeV").data());

		for(int cent:centralities) {
			TDirectory *cent_dir = energy_dir->mkdir((to_string(cent)+"_Cent").data());
			cent_dir->cd();

			for(int div:divs) {
				data[energy][div][cent].canvas_2d_dist("Particle_Bin_Dist " + to_string(energy) + "GeV, " + to_string(cent) + " cent, " + to_string(div) + " degree bins");
			}
		}
	}
}


void BinomialAnalyzer::plot_slices(map<int, map<int, map<int, AzimuthBinData>>> &data, TDirectory *dir) {

	for(auto &div:divs) {
		TDirectory* div_dir = dir->mkdir((to_string(div)+" Degree Bins").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &energy:energy_list) {
				TDirectory *energy_dir = cent_dir->mkdir((to_string(energy)+"GeV").data());
				energy_dir->cd();
				for(auto &total_particle:data[energy][div][cent].get_bin_data()) {
					slice_dist_plot(total_particle.second, total_particle.first, div, "Slice Dist " + to_string(total_particle.first) + " particles " + to_string(energy) + "GeV " + to_string(cent) + " cent " + to_string(div) + " degree bins");
				}
//				for(auto &total_particle:data[energy][div][cent].get_diff_slice_hist()) {
//					slice_diff_dist_plot(total_proton.second, total_proton.first, div, "Diff Slice Dist " + to_string(total_particle.first) + " particles " + to_string(energy) + "GeV " + to_string(cent) + " cent " + to_string(div) + " divs");
//				}
			}
		}
	}
}


void BinomialAnalyzer::plot_all_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, TDirectory *dir) {
	TDirectory* stats_dir = dir->mkdir("Stats_vs_Total_Particles");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_dir->mkdir((to_string(div)+" Degree Bins").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &stat:stats) {
//				cout << "all " << div << " div " << cent << " cent " << stat << endl;
				slice_stats_plot(slice_stats, stat, energy_list, cent, div, stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
		}
	}
}


void BinomialAnalyzer::plot_all_stats(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, TDirectory *dir) {
	TDirectory* stats_dir = dir->mkdir("Stats_vs_Total_Particles");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_dir->mkdir((to_string(div)+" Degree Bins").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &stat:stats) {
//				cout << "all " << div << " div " << cent << " cent " << stat << endl;
				slice_stats_plot(slice_stats, stat, energy_list, cent, div, stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
		}
	}
}


void BinomialAnalyzer::plot_all_stat_ratios(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, TDirectory *dir) {
	TDirectory* stats_dir = dir->mkdir("Stat_Ratios_vs_Total_Particles");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_dir->mkdir((to_string(div)+" Degree Bins").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(string &stat:stat_ratios) {
				slice_stat_ratios_plot(slice_stats, stat, energy_list, cent, div, stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
		}
	}
}


void BinomialAnalyzer::plot_all_stat_ratios(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, TDirectory *dir) {
	TDirectory* stats_dir = dir->mkdir("Stat_Ratios_vs_Total_Particles");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_dir->mkdir((to_string(div)+" Degree Bins").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(string &stat:stat_ratios) {
				slice_stat_ratios_plot(slice_stats, stat, energy_list, cent, div, stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
		}
	}
}


void BinomialAnalyzer::plot_all_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stat_ratios, TDirectory *dir) {
	TDirectory* stats_divided_dir = dir->mkdir("Stats_Divided_vs_Total_Particles");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_divided_dir->mkdir((to_string(div)+" Degree Bins").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &stat:stats) {
				auto fits = slice_stats_divided_plot(slice_stats, stat, energy_list, cent, div, "Divided "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
				plot_fit(fits, cent, div, "Fits "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
			for(auto &stat:stat_ratios) {
				auto fits = slice_stats_divided_plot(slice_stat_ratios, stat, energy_list, cent, div, "Divided "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
				plot_fit(fits, cent, div, "Fits "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
		}
	}
}


void BinomialAnalyzer::plot_all_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stat_ratios, TDirectory *dir) {
	TDirectory* stats_divided_dir = dir->mkdir("Stats_Divided_vs_Total_Particles");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_divided_dir->mkdir((to_string(div)+" Degree Bins").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &stat:stats) {
				auto fits = slice_stats_divided_plot(slice_stats, stat, energy_list, cent, div, "Divided "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
				plot_fit(fits, cent, div, "Fits "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
			for(auto &stat:stat_ratios) {
				auto fits = slice_stats_divided_plot(slice_stat_ratios, stat, energy_list, cent, div, "Divided "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
				plot_fit(fits, cent, div, "Fits "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
		}
	}
}


void BinomialAnalyzer::plot_mix_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, TDirectory *dir) {
	TDirectory* stats_divided_dir = dir->mkdir("Stats_Mix_Divided_vs_Total_Particles");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_divided_dir->mkdir((to_string(div)+" Degree Bins").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &stat:stats) {
				auto fits = slice_stats_divided_plot(slice_stats, stat, energy_list, cent, div, "Mix_Divided "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
				plot_fit(fits, cent, div, "Fits "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
		}
	}
}


void BinomialAnalyzer::plot_mix_divided_stats(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, TDirectory *dir) {
	TDirectory* stats_divided_dir = dir->mkdir("Stats_Mix_Divided_vs_Total_Particles");
	for(auto &div:divs) {
		TDirectory* div_dir = stats_divided_dir->mkdir((to_string(div)+" Degree Bins").data());
		for(auto &cent:centralities) {
			TDirectory* cent_dir = div_dir->mkdir((to_string(cent)+" Centrality").data());
			cent_dir->cd();
			for(auto &stat:stats) {
				auto fits = slice_stats_divided_plot(slice_stats, stat, energy_list, cent, div, "Divided "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
				plot_fit(fits, cent, div, "Fits "+stat+" "+to_string(div)+" degree bins "+to_string(cent)+" cent");
			}
		}
	}
}


void BinomialAnalyzer::slice_stats_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name) {
	double y_max = numeric_limits<double>::min();
	double y_min = numeric_limits<double>::max();
	int x_max = 0;
	for(int energy:energies) {
		for(auto &data_set:slice_stats) {
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name];
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				if(particle_data.first > x_max) { x_max = particle_data.first; }
			}
		}
	}
	double y_range = y_max - y_min;


	TCanvas can(name.data(), name.data(), canvas_width, canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	gStyle->SetLegendTextSize(legend_text_size);
	pair<int, int> can_div = get_canvas_div(energies.size());
	can.Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;

	vector<TMultiGraph*> mgs;
	vector<TGraphErrors*> graphs;
	vector<TLegend*> legends;
	vector<TF1*> binoms;

	for(int energy:energies) {
		can.cd(can_index);
		mgs.push_back(new TMultiGraph());
		mgs.back()->SetNameTitle((to_string(energy) + "GeV").data(), (to_string(energy) + "GeV").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		double p = (double)div / 360;
		legends.push_back(new TLegend(0.3, 0.21, 0.3, 0.21));
		if(stat_name == "mean") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "[0]*x", plot_x_range.first, x_max + 2)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "standard_deviation") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "TMath::Sqrt([0]*(1-[0])*x)", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "skewness") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "(1 - 2*[0]) / TMath::Sqrt([0]*(1-[0])*x)", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "skewness_sd") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "1 - 2*[0]", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "kurtosis") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "(1 - 6*[0]*(1-[0])) / ([0]*(1-[0])*x)", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "kurtosis_variance") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "(1 - 6*[0]*(1-[0]))", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else { binoms.push_back(new TF1()); }
		for(auto &data_set:slice_stats) {
			vector<double> stat_vals, particle_val, stat_err, particle_err;
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name];
//				if(stat_name == "skewness_sd") { cout << (string) stat_meas << endl; }
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				particle_val.push_back(particle_data.first);
				particle_err.push_back(0.0);
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
//				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
//				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			graphs.push_back(new TGraphErrors((int)particle_val.size(), particle_val.data(), stat_vals.data(), particle_err.data(), stat_err.data()));
			graphs.back()->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
			graphs.back()->SetMarkerStyle(get_marker_style(data_set.first));
			graphs.back()->SetMarkerColor(get_marker_color(data_set.first));
			graphs.back()->SetMarkerSize(get_marker_size(data_set.first));
			graphs.back()->SetLineColor(get_marker_color(data_set.first));
			mgs.back()->Add(graphs.back(), "APZ");
			if(can_index == 1) {
				legends.back()->SetBorderSize(legend_border_width);
				legends.back()->SetFillStyle(0);
				legends.back()->AddEntry(graphs.back(), (data_set.first + " " + stat_name + " " + to_string(div) + "#circ bins").data(), "p");

			}
		}
//		double y_range = y_max - y_min;
		mgs.back()->GetXaxis()->SetLimits(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetRangeUser(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetLabelSize(0.06);
		mgs.back()->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mgs.back()->GetXaxis()->SetTitle("Total Particles"); mgs.back()->GetXaxis()->SetTitleSize(0.06); mgs.back()->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		if(can_index % can_div.first == 1) {
			if(stat_name == "mean") { mgs.back()->GetYaxis()->SetTitle("Mean"); }
			else if(stat_name == "standard_deviation") { mgs.back()->GetYaxis()->SetTitle("Standard Deviation"); }
			else if(stat_name == "skewness") { mgs.back()->GetYaxis()->SetTitle("Skewness"); }
			else if(stat_name == "skewness_sd") { mgs.back()->GetYaxis()->SetTitle("Skewness*Standard Deviation"); }
			else if(stat_name == "kurtosis") { mgs.back()->GetYaxis()->SetTitle("Kurtosis"); }
			else if(stat_name == "kurtosis_variance") { mgs.back()->GetYaxis()->SetTitle("Kurtosis*Variance"); }
			mgs.back()->GetYaxis()->SetTitleSize(0.06); mgs.back()->GetYaxis()->SetTitleOffset(0.95); gPad->SetLeftMargin(0.12); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.03);
		mgs.back()->Draw("AP");
		binoms.back()->Draw("same");
		if(can_index == 1) { legends.back()->SetMargin(0.1); legends.back()->Draw(); }
		can_index++;
	}
	can.Update();
	can.Write(name.data());

	for(auto leg:legends) { delete leg; }
	for(auto graph:graphs) { delete graph; }
	for(auto mg:mgs) { delete mg; }
}


void BinomialAnalyzer::slice_stats_plot(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name) {
	double y_max = numeric_limits<double>::min();
	double y_min = numeric_limits<double>::max();
	int x_max = 0;
	for(int energy:energies) {
		for(auto &data_set:slice_stats) {
//			vector<double> stat_vals, particle_val, stat_err, particle_err, stat_sys;
			Measure stat_meas;
			double stat_sys;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name].first;
				stat_sys = particle_data.second[stat_name].second;
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				if(stat_meas.get_val() + stat_sys > y_max) { y_max = stat_meas.get_val() + stat_sys; }
				if(stat_meas.get_val() - stat_sys < y_min) { y_min = stat_meas.get_val() - stat_sys; }
				if(particle_data.first > x_max) { x_max = particle_data.first; }
			}
		}
	}
	double y_range = y_max - y_min;


	TCanvas can(name.data(), name.data(), canvas_width, canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	gStyle->SetLegendTextSize(legend_text_size);
	pair<int, int> can_div = get_canvas_div(energies.size());
	can.Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;

	vector<TMultiGraph*> mgs;
	vector<TGraphErrors*> graphs;
	vector<TLegend*> legends;
	vector<TF1*> binoms;

	for(int energy:energies) {
		can.cd(can_index);
		mgs.push_back(new TMultiGraph());
		mgs.back()->SetNameTitle((to_string(energy) + "GeV").data(), (to_string(energy) + "GeV").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		double p = (double)div / 360;
		legends.push_back(new TLegend(0.3, 0.21, 0.3, 0.21));
		if(stat_name == "mean") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "[0]*x", plot_x_range.first, x_max + 2)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "standard_deviation") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "TMath::Sqrt([0]*(1-[0])*x)", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "skewness") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "(1 - 2*[0]) / TMath::Sqrt([0]*(1-[0])*x)", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "skewness_sd") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "1 - 2*[0]", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "kurtosis") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "(1 - 6*[0]*(1-[0])) / ([0]*(1-[0])*x)", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "kurtosis_variance") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "(1 - 6*[0]*(1-[0]))", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else { binoms.push_back(new TF1()); }
		for(auto &data_set:slice_stats) {
			vector<double> stat_vals, particle_val, stat_err, particle_err, stat_sys;
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name].first;
//				if(stat_name == "skewness_sd") { cout << (string) stat_meas << endl; }
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				particle_val.push_back(particle_data.first);
				particle_err.push_back(0.0);
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				stat_sys.push_back(particle_data.second[stat_name].second);
//				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
//				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			graphs.push_back(new TGraphErrors((int)particle_val.size(), particle_val.data(), stat_vals.data(), particle_err.data(), stat_err.data()));
			graphs.back()->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
			graphs.back()->SetMarkerStyle(get_marker_style(data_set.first));
			graphs.back()->SetMarkerColor(get_marker_color(data_set.first));
			graphs.back()->SetMarkerSize(get_marker_size(data_set.first));
			graphs.back()->SetLineColor(get_marker_color(data_set.first));
			mgs.back()->Add(graphs.back(), "APZ");
			if(can_index == 1) {
				legends.back()->SetBorderSize(legend_border_width);
				legends.back()->SetFillStyle(0);
				legends.back()->AddEntry(graphs.back(), (data_set.first + " " + stat_name + " " + to_string(div) + "#circ bins").data(), "p");
			}
			graphs.push_back(new TGraphErrors((int)particle_val.size(), particle_val.data(), stat_vals.data(), particle_err.data(), stat_sys.data()));
			graphs.back()->SetLineColor(get_marker_color(data_set.first));
			mgs.back()->Add(graphs.back(), "[]");
		}
//		double y_range = y_max - y_min;
		mgs.back()->GetXaxis()->SetLimits(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetRangeUser(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetLabelSize(0.06);
		mgs.back()->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mgs.back()->GetXaxis()->SetTitle("Total Particles"); mgs.back()->GetXaxis()->SetTitleSize(0.06); mgs.back()->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		if(can_index % can_div.first == 1) {
			if(stat_name == "mean") { mgs.back()->GetYaxis()->SetTitle("Mean"); }
			else if(stat_name == "standard_deviation") { mgs.back()->GetYaxis()->SetTitle("Standard Deviation"); }
			else if(stat_name == "skewness") { mgs.back()->GetYaxis()->SetTitle("Skewness"); }
			else if(stat_name == "skewness_sd") { mgs.back()->GetYaxis()->SetTitle("Skewness*Standard Deviation"); }
			else if(stat_name == "kurtosis") { mgs.back()->GetYaxis()->SetTitle("Kurtosis"); }
			else if(stat_name == "kurtosis_variance") { mgs.back()->GetYaxis()->SetTitle("Kurtosis*Variance"); }
			mgs.back()->GetYaxis()->SetTitleSize(0.06); mgs.back()->GetYaxis()->SetTitleOffset(0.95); gPad->SetLeftMargin(0.12); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.03);
		mgs.back()->Draw("AP");
		binoms.back()->Draw("same");
		if(can_index == 1) { legends.back()->SetMargin(0.1); legends.back()->Draw(); }
		can_index++;
	}
	can.Update();
	can.Write(name.data());

	for(auto leg:legends) { delete leg; }
	for(auto graph:graphs) { delete graph; }
	for(auto mg:mgs) { delete mg; }
}


void BinomialAnalyzer::slice_stat_ratios_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name) {
	double y_max = numeric_limits<double>::min();
	double y_min = numeric_limits<double>::max();
	int x_max = 0;
	for(int energy:energies) {
		for(auto &data_set:slice_stats) {
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name];
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				if(particle_data.first > x_max) { x_max = particle_data.first; }
			}
		}
	}
	double y_range = y_max - y_min;


	TCanvas can(name.data(), name.data(), canvas_width, canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	gStyle->SetLegendTextSize(legend_text_size);
	pair<int, int> can_div = get_canvas_div(energies.size());
	can.Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;

	vector<TMultiGraph*> mgs;
	vector<TGraphErrors*> graphs;
	vector<TLegend*> legends;
	vector<TF1*> binoms;

	for(int energy:energies) {
		can.cd(can_index);
		mgs.push_back(new TMultiGraph());
		mgs.back()->SetNameTitle((to_string(energy) + "GeV").data(), (to_string(energy) + "GeV").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		double p = (double)div / 360;
		legends.push_back(new TLegend(0.3, 0.21, 0.3, 0.21));
		if(stat_name == "variance/mean") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "1-[0]", plot_x_range.first, x_max + 2)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "sd/mean") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "TMath::Sqrt((1-[0])/([0]*x))", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else { binoms.push_back(new TF1()); }
		for(auto &data_set:slice_stats) {
			vector<double> stat_vals, particle_val, stat_err, particle_err;
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name];
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				particle_val.push_back(particle_data.first);
				particle_err.push_back(0.0);
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
//				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
//				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			graphs.push_back(new TGraphErrors((int)particle_val.size(), particle_val.data(), stat_vals.data(), particle_err.data(), stat_err.data()));
			graphs.back()->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
			graphs.back()->SetMarkerStyle(get_marker_style(data_set.first));
			graphs.back()->SetMarkerColor(get_marker_color(data_set.first));
			graphs.back()->SetMarkerSize(get_marker_size(data_set.first));
			graphs.back()->SetLineColor(get_marker_color(data_set.first));
			mgs.back()->Add(graphs.back(), "APZ");
			if(can_index == 1) {
				legends.back()->SetBorderSize(legend_border_width);
				legends.back()->SetFillStyle(0);
				legends.back()->AddEntry(graphs.back(), (data_set.first + " " + stat_name + " " + to_string(div) + "#circ bins").data(), "p");

			}
		}
//		double y_range = y_max - y_min;
		mgs.back()->GetXaxis()->SetLimits(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetRangeUser(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetLabelSize(0.06);
		mgs.back()->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mgs.back()->GetXaxis()->SetTitle("Total Particles"); mgs.back()->GetXaxis()->SetTitleSize(0.06); mgs.back()->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		if(can_index % can_div.first == 1) {
			if(stat_name == "variance/mean") { mgs.back()->GetYaxis()->SetTitle("Vaiance Divided By Mean"); }
			else if(stat_name == "sd/mean") { mgs.back()->GetYaxis()->SetTitle("Standard Deviation Divided By Mean"); }
			mgs.back()->GetYaxis()->SetTitleSize(0.06); mgs.back()->GetYaxis()->SetTitleOffset(0.95); gPad->SetLeftMargin(0.12); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.03);
		mgs.back()->Draw("AP");
		binoms.back()->Draw("same");
		if(can_index == 1) { legends.back()->SetMargin(0.1); legends.back()->Draw(); }
		can_index++;
	}
	can.Update();
	can.Write(name.data());

	for(auto leg:legends) { delete leg; }
	for(auto graph:graphs) { delete graph; }
	for(auto mg:mgs) { delete mg; }
}


void BinomialAnalyzer::slice_stat_ratios_plot(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_stats, string stat_name, vector<int> &energies, int cent, int div, string name) {
	double y_max = numeric_limits<double>::min();
	double y_min = numeric_limits<double>::max();
	int x_max = 0;
	for(int energy:energies) {
		for(auto &data_set:slice_stats) {
			Measure stat_meas;
			double stat_sys;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name].first;
				stat_sys = particle_data.second[stat_name].second;
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
				if(stat_meas.get_val() + stat_sys > y_max) { y_max = stat_meas.get_val() + stat_sys; }
				if(stat_meas.get_val() - stat_sys < y_min) { y_min = stat_meas.get_val() - stat_sys; }
				if(particle_data.first > x_max) { x_max = particle_data.first; }
			}
		}
	}
	double y_range = y_max - y_min;


	TCanvas can(name.data(), name.data(), canvas_width, canvas_height);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	gStyle->SetLegendTextSize(legend_text_size);
	pair<int, int> can_div = get_canvas_div(energies.size());
	can.Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;

	vector<TMultiGraph*> mgs;
	vector<TGraphErrors*> graphs;
	vector<TLegend*> legends;
	vector<TF1*> binoms;

	for(int energy:energies) {
		can.cd(can_index);
		mgs.push_back(new TMultiGraph());
		mgs.back()->SetNameTitle((to_string(energy) + "GeV").data(), (to_string(energy) + "GeV").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		double p = (double)div / 360;
		legends.push_back(new TLegend(0.3, 0.21, 0.3, 0.21));
		if(stat_name == "variance/mean") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "1-[0]", plot_x_range.first, x_max + 2)); binoms.back()->SetParameter(0, p); }
		else if(stat_name == "sd/mean") { binoms.push_back(new TF1((to_string(energy) + "GeV Binomial").data(), "TMath::Sqrt((1-[0])/([0]*x))", plot_x_range.first, x_max)); binoms.back()->SetParameter(0, p); }
		else { binoms.push_back(new TF1()); }
		for(auto &data_set:slice_stats) {
			vector<double> stat_vals, particle_val, stat_err, particle_err, stat_sys;
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name].first;
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				particle_val.push_back(particle_data.first);
				particle_err.push_back(0.0);
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				stat_sys.push_back(particle_data.second[stat_name].second);
//				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
//				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			graphs.push_back(new TGraphErrors((int)particle_val.size(), particle_val.data(), stat_vals.data(), particle_err.data(), stat_err.data()));
			graphs.back()->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
			graphs.back()->SetMarkerStyle(get_marker_style(data_set.first));
			graphs.back()->SetMarkerColor(get_marker_color(data_set.first));
			graphs.back()->SetMarkerSize(get_marker_size(data_set.first));
			graphs.back()->SetLineColor(get_marker_color(data_set.first));
			mgs.back()->Add(graphs.back(), "APZ");
			if(can_index == 1) {
				legends.back()->SetBorderSize(legend_border_width);
				legends.back()->SetFillStyle(0);
				legends.back()->AddEntry(graphs.back(), (data_set.first + " " + stat_name + " " + to_string(div) + "#circ bins").data(), "p");
			}
			graphs.push_back(new TGraphErrors((int)particle_val.size(), particle_val.data(), stat_vals.data(), particle_err.data(), stat_sys.data()));
			graphs.back()->SetLineColor(get_marker_color(data_set.first));
			mgs.back()->Add(graphs.back(), "[]");
		}
//		double y_range = y_max - y_min;
		mgs.back()->GetXaxis()->SetLimits(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetRangeUser(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetLabelSize(0.06);
		mgs.back()->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mgs.back()->GetXaxis()->SetTitle("Total Particles"); mgs.back()->GetXaxis()->SetTitleSize(0.06); mgs.back()->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		if(can_index % can_div.first == 1) {
			if(stat_name == "variance/mean") { mgs.back()->GetYaxis()->SetTitle("Vaiance Divided By Mean"); }
			else if(stat_name == "sd/mean") { mgs.back()->GetYaxis()->SetTitle("Standard Deviation Divided By Mean"); }
			mgs.back()->GetYaxis()->SetTitleSize(0.06); mgs.back()->GetYaxis()->SetTitleOffset(0.95); gPad->SetLeftMargin(0.12); }
		gPad->SetTopMargin(0.09);
		gPad->SetRightMargin(0.03);
		mgs.back()->Draw("AP");
		binoms.back()->Draw("same");
		if(can_index == 1) { legends.back()->SetMargin(0.1); legends.back()->Draw(); }
		can_index++;
	}
	can.Update();
	can.Write(name.data());

	for(auto leg:legends) { delete leg; }
	for(auto graph:graphs) { delete graph; }
	for(auto mg:mgs) { delete mg; }
}


map<string, map<int, TF1*>> BinomialAnalyzer::slice_stats_divided_plot(map<string, map<int, map<int, map<int, map<int, map<string, Measure>>>>>> &slice_divided_stats, string stat_name, vector<int> &energies, int cent, int div, string name) {
	double y_max = numeric_limits<double>::min();
	double y_min = numeric_limits<double>::max();
	int x_max = 0;
	for(int energy:energies) {
		for(auto &data_set:slice_divided_stats) {
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name];
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				if(stat_meas.get_val() > y_max) { y_max = stat_meas.get_val(); }
				if(stat_meas.get_val() < y_min) { y_min = stat_meas.get_val(); }
				if(particle_data.first > x_max) { x_max = particle_data.first; }
			}
		}
	}
	double y_range = y_max - y_min;


	TCanvas can(name.data(), name.data(), canvas_width, canvas_height);
	gPad->SetFillColor(0);
	gPad->SetBorderMode(0);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	gStyle->SetLegendTextSize(legend_text_size);
	pair<int, int> can_div = get_canvas_div(energies.size());
	can.Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;

	vector<TMultiGraph*> mgs;
	vector<TGraphErrors*> graphs;
	vector<TLegend*> legends;
	vector<TLine*> lines;

	map<string, map<int, TF1*>> linear_fits;

	for(int energy:energies) {
		can.cd(can_index);
		mgs.push_back(new TMultiGraph());
		mgs.back()->SetNameTitle((to_string(energy) + "GeV").data(), (to_string(energy) + "GeV").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		legends.push_back(new TLegend(0.3, 0.21, 0.3, 0.21));
		lines.push_back(new TLine(plot_x_range.first, 1, x_max + 2, 1));
		lines.back()->SetLineColor(kBlack);
		int set_num = 0;
		for(auto &data_set:slice_divided_stats) {
			vector<double> stat_vals, particle_val, stat_err, particle_err;
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name];
//				if(in_string(data_set.first, "raw/mix") && stat_name == "standard_deviation") { cout << (string)stat_meas << endl; }
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				particle_val.push_back(particle_data.first);
				particle_err.push_back(0.0);
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
//				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
//				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
//			if(in_string(data_set.first, "raw/mix") && stat_name == "standard_deviation") { for(auto &val:particle_val) { cout << val << endl; } }
			graphs.push_back(new TGraphErrors((int)particle_val.size(), particle_val.data(), stat_vals.data(), particle_err.data(), stat_err.data()));
			graphs.back()->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
			graphs.back()->SetMarkerStyle(get_marker_style(data_set.first, set_num));
			graphs.back()->SetMarkerColor(get_marker_color(data_set.first, set_num));
			graphs.back()->SetMarkerSize(get_marker_size(data_set.first, set_num));
			graphs.back()->SetLineColor(get_marker_color(data_set.first, set_num));

			linear_fits[data_set.first][energy] = new TF1((data_set.first+to_string(energy)+stat_name).data(), "[0]*(x-1)+1");
			linear_fits[data_set.first][energy]->SetLineColor(get_marker_color(data_set.first, set_num));
			graphs.back()->Fit(linear_fits[data_set.first][energy], "Q");

			mgs.back()->Add(graphs.back(), "APZ");
			if(can_index == 1) {
				legends.back()->SetBorderSize(legend_border_width);
				legends.back()->SetFillStyle(0);
				legends.back()->AddEntry(graphs.back(), (data_set.first + " " + stat_name + " " + to_string(div) + "#circ bins").data(), "p");
			}
			set_num++;
		}
//		double y_range = y_max - y_min;
		int x_pos = (can_index - 1) % can_div.first;
		int y_pos = (can_index - 1) / can_div.first;
		double left_margin = 0.18;
		double x_len = 0.99 - left_margin/can_div.first;
		double x_low = left_margin/can_div.first + x_pos*(x_len / can_div.first);
		double x_up = x_low + x_len / can_div.first;
		if(x_pos == 0) { x_low = 0; }
		double y_low = (can_div.second - y_pos -1) * (1.0 / can_div.second);
		double y_up = y_low + 1.0 / can_div.second;
		gPad->SetPad(x_low, y_low, x_up, y_up);

		mgs.back()->GetXaxis()->SetLimits(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetRangeUser(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetLabelSize(0.06);
		mgs.back()->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mgs.back()->GetYaxis()->SetLimits(0.8, 1.15);
//		mgs.back()->GetYaxis()->SetRangeUser(0.8, 1.15);
		mgs.back()->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mgs.back()->GetXaxis()->SetTitle("Total Particles"); mgs.back()->GetXaxis()->SetTitleSize(0.06); mgs.back()->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		if(can_index % can_div.first == 1) {
			if(stat_name == "mean") { mgs.back()->GetYaxis()->SetTitle("Mean_{data} / Mean_{binomial}                     "); }
			else if(stat_name == "standard_deviation") { mgs.back()->GetYaxis()->SetTitle("#sigma_{data} / #sigma_{binomial}                     "); }
			else if(stat_name == "skewness") { mgs.back()->GetYaxis()->SetTitle("skewness_{data} / skewness_{binomial}                     "); }
			else if(stat_name == "skewness_sd") { mgs.back()->GetYaxis()->SetTitle("skewness*sd_{data} / skewness*sd_{binomial}                     "); }
			else if(stat_name == "kurtosis") { mgs.back()->GetYaxis()->SetTitle("kurtosis_{data} / kurtosis_{binomial}                     "); }
			else if(stat_name == "kurtosis_variance") { mgs.back()->GetYaxis()->SetTitle("kurtosis*variance_{data} / kurtosis*variance_{binomial}                     "); }
			else if(stat_name == "variance/mean") { mgs.back()->GetYaxis()->SetTitle("#sigma^2_{data}/#mu{data} / #sigma^2_{binomial}/mu{binomial}    "); }
			else if(stat_name == "sd/mean") { mgs.back()->GetYaxis()->SetTitle("#sigma_{data}/#mu{data} / #sigma_{binomial}/mu{binomial}    "); }
			mgs.back()->GetYaxis()->SetTitleSize(0.06); mgs.back()->GetYaxis()->SetTitleOffset(1.5); gPad->SetLeftMargin(left_margin); }
		else { gPad->SetLeftMargin(0.001); }
		if(can_index % can_div.first == 0) { gPad->SetRightMargin(0.005); }
		else { gPad->SetRightMargin(0.0001); }
		gPad->SetTopMargin(0.02);
		mgs.back()->Draw("AP");
		lines.back()->Draw("same");
		if(can_index == 1) { legends.back()->SetMargin(0.1); legends.back()->Draw(); }
		can_index++;
	}
	can.Update();
	can.Write(name.data());

	for(auto line:lines) { delete line; }
	for(auto leg:legends) { delete leg; }
	for(auto graph:graphs) { delete graph; }
	for(auto mg:mgs) { delete mg; }

	return(linear_fits);
}


map<string, map<int, TF1*>> BinomialAnalyzer::slice_stats_divided_plot(map<string, map<int, map<int, map<int, map<int, map<string, pair<Measure, double>>>>>>> &slice_divided_stats, string stat_name, vector<int> &energies, int cent, int div, string name) {
	double y_max = numeric_limits<double>::min();
	double y_min = numeric_limits<double>::max();
	int x_max = 0;
	for(int energy:energies) {
		for(auto &data_set:slice_divided_stats) {
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name].first;
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				if(stat_meas.get_val() > y_max) { y_max = stat_meas.get_val(); }
				if(stat_meas.get_val() < y_min) { y_min = stat_meas.get_val(); }
				if(particle_data.first > x_max) { x_max = particle_data.first; }
			}
		}
	}
	double y_range = y_max - y_min;


	TCanvas can(name.data(), name.data(), canvas_width, canvas_height);
	gPad->SetFillColor(0);
	gPad->SetBorderMode(0);
	gStyle->SetTitleFontSize(0.09);
	gStyle->SetTitleOffset(1.2);
	gStyle->SetLegendTextSize(legend_text_size);
	pair<int, int> can_div = get_canvas_div(energies.size());
	can.Divide(can_div.first, can_div.second, 0.001, 0.001);
	int can_index = 1;

	vector<TMultiGraph*> mgs;
	vector<TGraphErrors*> graphs;
	vector<TLegend*> legends;
	vector<TLine*> lines;

	map<string, map<int, TF1*>> linear_fits;

	for(int energy:energies) {
		can.cd(can_index);
		mgs.push_back(new TMultiGraph());
		mgs.back()->SetNameTitle((to_string(energy) + "GeV").data(), (to_string(energy) + "GeV").data());
//		double y_max = numeric_limits<double>::min();
//		double y_min = numeric_limits<double>::max();
		legends.push_back(new TLegend(0.3, 0.21, 0.3, 0.21));
		lines.push_back(new TLine(plot_x_range.first, 1, x_max + 2, 1));
		lines.back()->SetLineColor(kBlack);
		int set_num = 0;
		for(auto &data_set:slice_divided_stats) {
			vector<double> stat_vals, particle_val, stat_err, particle_err, stat_sys;
			Measure stat_meas;

			for(auto &particle_data:data_set.second[energy][div][cent]) {
				stat_meas = particle_data.second[stat_name].first;
				if(stat_meas.get_err() == 0 || isnan(stat_meas.get_err())) { continue; }
				particle_val.push_back(particle_data.first);
				particle_err.push_back(0.0);
				stat_vals.push_back(stat_meas.get_val());
				stat_err.push_back(stat_meas.get_err());
				stat_sys.push_back(particle_data.second[stat_name].second);
//				if(stat_meas.get_val() + stat_meas.get_err() > y_max) { y_max = stat_meas.get_val() + stat_meas.get_err(); }
//				if(stat_meas.get_val() - stat_meas.get_err() < y_min) { y_min = stat_meas.get_val() - stat_meas.get_err(); }
			}
			graphs.push_back(new TGraphErrors((int)particle_val.size(), particle_val.data(), stat_vals.data(), particle_err.data(), stat_err.data()));
			graphs.back()->SetNameTitle((data_set.first + " " + to_string(div) + " degree bins").data());
			graphs.back()->SetMarkerStyle(get_marker_style(data_set.first, set_num));
			graphs.back()->SetMarkerColor(get_marker_color(data_set.first, set_num));
			graphs.back()->SetMarkerSize(get_marker_size(data_set.first, set_num));
			graphs.back()->SetLineColor(get_marker_color(data_set.first, set_num));

			linear_fits[data_set.first][energy] = new TF1((data_set.first+to_string(energy)+stat_name).data(), "[0]*(x-1)+1");
			linear_fits[data_set.first][energy]->SetLineColor(get_marker_color(data_set.first, set_num));
			graphs.back()->Fit(linear_fits[data_set.first][energy], "Q");

			mgs.back()->Add(graphs.back(), "APZ");

			if(can_index == 1) {
				legends.back()->SetBorderSize(legend_border_width);
				legends.back()->SetFillStyle(0);
				legends.back()->AddEntry(graphs.back(), (data_set.first + " " + stat_name + " " + to_string(div) + "#circ bins").data(), "p");
			}

			graphs.push_back(new TGraphErrors((int)particle_val.size(), particle_val.data(), stat_vals.data(), particle_err.data(), stat_sys.data()));
			graphs.back()->SetLineColor(get_marker_color(data_set.first, set_num));
			mgs.back()->Add(graphs.back(), "[]");
			set_num++;
		}
//		double y_range = y_max - y_min;
		int x_pos = (can_index - 1) % can_div.first;
		int y_pos = (can_index - 1) / can_div.first;
		double left_margin = 0.18;
		double x_len = 0.99 - left_margin/can_div.first;
		double x_low = left_margin/can_div.first + x_pos*(x_len / can_div.first);
		double x_up = x_low + x_len / can_div.first;
		if(x_pos == 0) { x_low = 0; }
		double y_low = (can_div.second - y_pos -1) * (1.0 / can_div.second);
		double y_up = y_low + 1.0 / can_div.second;
		gPad->SetPad(x_low, y_low, x_up, y_up);

		mgs.back()->GetXaxis()->SetLimits(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetRangeUser(plot_x_range.first, x_max + 2);
		mgs.back()->GetXaxis()->SetLabelSize(0.06);
		mgs.back()->GetYaxis()->SetLimits(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
		mgs.back()->GetYaxis()->SetRangeUser(y_min - 0.1 * y_range, y_max + 0.1 * y_range);
//		mgs.back()->GetYaxis()->SetLimits(0.8, 1.15);
//		mgs.back()->GetYaxis()->SetRangeUser(0.8, 1.15);
		mgs.back()->GetYaxis()->SetLabelSize(0.06);
		if(can_index > can_div.first*(can_div.second-1)) { mgs.back()->GetXaxis()->SetTitle("Total Particles"); mgs.back()->GetXaxis()->SetTitleSize(0.06); mgs.back()->GetXaxis()->SetTitleOffset(0.85); gPad->SetBottomMargin(0.12); }
		else { gPad->SetBottomMargin(0.07); }
		if(can_index % can_div.first == 1) {
			if(stat_name == "mean") { mgs.back()->GetYaxis()->SetTitle("Mean_{data} / Mean_{binomial}                     "); }
			else if(stat_name == "standard_deviation") { mgs.back()->GetYaxis()->SetTitle("#sigma_{data} / #sigma_{binomial}                     "); }
			else if(stat_name == "skewness") { mgs.back()->GetYaxis()->SetTitle("skewness_{data} / skewness_{binomial}                     "); }
			else if(stat_name == "skewness_sd") { mgs.back()->GetYaxis()->SetTitle("skewness*sd_{data} / skewness*sd_{binomial}                     "); }
			else if(stat_name == "kurtosis") { mgs.back()->GetYaxis()->SetTitle("kurtosis_{data} / kurtosis_{binomial}                     "); }
			else if(stat_name == "kurtosis_variance") { mgs.back()->GetYaxis()->SetTitle("kurtosis*variance_{data} / kurtosis*variance_{binomial}                     "); }
			else if(stat_name == "variance/mean") { mgs.back()->GetYaxis()->SetTitle("#sigma^2_{data}/#mu{data} / #sigma^2_{binomial}/mu{binomial}    "); }
			else if(stat_name == "sd/mean") { mgs.back()->GetYaxis()->SetTitle("#sigma_{data}/#mu{data} / #sigma_{binomial}/mu{binomial}    "); }
			mgs.back()->GetYaxis()->SetTitleSize(0.06); mgs.back()->GetYaxis()->SetTitleOffset(1.5); gPad->SetLeftMargin(left_margin); }
		else { gPad->SetLeftMargin(0.001); }
		if(can_index % can_div.first == 0) { gPad->SetRightMargin(0.005); }
		else { gPad->SetRightMargin(0.0001); }
		gPad->SetTopMargin(0.02);
		mgs.back()->Draw("AP");
		lines.back()->Draw("same");
		if(can_index == 1) { legends.back()->SetMargin(0.1); legends.back()->Draw(); }
		can_index++;
	}
	can.Update();
	can.Write(name.data());

	for(auto line:lines) { delete line; }
	for(auto leg:legends) { delete leg; }
	for(auto graph:graphs) { delete graph; }
	for(auto mg:mgs) { delete mg; }

	return(linear_fits);
}


void BinomialAnalyzer::plot_fit(map<string, map<int, TF1*>> &fits, int cent, int div, string name) {
	TCanvas can(name.data(), name.data(), canvas_width/2, canvas_height);
//	gStyle->SetTitleFontSize(0.09);
//	gStyle->SetTitleOffset(1.2);

	auto slope_mg = new TMultiGraph();
	slope_mg->SetNameTitle("Slope", "Slope");
	double y_max_slope = numeric_limits<double>::min();
	double y_min_slope = numeric_limits<double>::max();

	auto slope_leg = new TLegend();
	slope_leg->SetBorderSize(legend_border_width);
	slope_leg->SetFillStyle(0);
	slope_leg->SetMargin(0.1);

	auto zero_line = new TLine(0, 0, 80, 0);
	zero_line->SetLineColor(kBlack);

	vector<TGraphErrors*> graphs;

	int set_num = 0;
	for(auto &data_set:fits) {
		vector<double> slope_vals, energy_val, slope_err, energy_err;

		for(auto &energy:data_set.second) {
			slope_vals.push_back(energy.second->GetParameter(0));
			energy_val.push_back(energy_match[energy.first] + 0.15*set_num);
			slope_err.push_back(energy.second->GetParError(0));
			energy_err.push_back(0.0);
			if(slope_vals.back() + slope_err.back() > y_max_slope) { y_max_slope = slope_vals.back() + slope_err.back(); }
			if(slope_vals.back() - slope_err.back() < y_min_slope) { y_min_slope = slope_vals.back() - slope_err.back(); }
		}

		graphs.push_back(new TGraphErrors((int)energy_val.size(), energy_val.data(), slope_vals.data(), energy_err.data(), slope_err.data()));
		graphs.back()->SetNameTitle((data_set.first + " slope").data());
		graphs.back()->SetMarkerStyle(get_marker_style(data_set.first, set_num));
		graphs.back()->SetMarkerColor(get_marker_color(data_set.first, set_num));
		graphs.back()->SetLineColor(get_marker_color(data_set.first, set_num));
		graphs.back()->SetMarkerSize(1.5);
		slope_mg->Add(graphs.back(), "APZ");
		slope_leg->AddEntry(graphs.back(), data_set.first.data(), "p");
		set_num++;
	}

	double y_range_slope = y_max_slope - y_min_slope;
	slope_mg->GetXaxis()->SetLimits(0, 80);
	slope_mg->GetXaxis()->SetRangeUser(0, 80);
//	slope_mg->GetXaxis()->SetLabelSize(0.06);
	slope_mg->GetYaxis()->SetLimits(y_min_slope - 0.1 * y_range_slope, y_max_slope + 0.1 * y_range_slope);
	slope_mg->GetYaxis()->SetRangeUser(y_min_slope - 0.1 * y_range_slope, y_max_slope + 0.1 * y_range_slope);
//	slope_mg->GetYaxis()->SetLabelSize(0.06);
	slope_mg->GetXaxis()->SetTitle("Energy (GeV)");

	slope_mg->Draw("AP");
	zero_line->Draw("same");
	slope_leg->Draw();

	can.Update();
	can.Write(name.data());

	for(auto graph:graphs) { delete graph; }
	delete zero_line;
	delete slope_mg;
}


void BinomialAnalyzer::slice_dist_plot(map<int, int> &slice_data, int total_particles, int div, string name) {
	TCanvas can((name + " canvas").data(), (name + " canvas").data(), canvas_width, canvas_height);
	gStyle->SetOptStat("niMRSK");
	TH1D dist((name + " hist").data(), (name + " hist").data(), total_particles+1, -0.5, total_particles+0.5);
	TH1D binom((name + " hist_binom").data(), (name + " binom").data(), total_particles+1, -0.5, total_particles+0.5);
	binom.SetLineColor(kRed);

	for(auto &bin_particles:slice_data) {
		dist.Fill(bin_particles.first, bin_particles.second);
	}
	int norm = dist.Integral();
	double p = (double)div / 360;
	double q = 1 - p;
	int n = total_particles;
	for(int k = 0; k <= n; k++) {
		binom.Fill(k, norm * TMath::Binomial(n, k) * pow(p, k) * pow(q, n - k));
	}

	dist.Draw("HIST");
	binom.Draw("HISTSAMES");
	can.Update();
	can.Write(name.data());
}


void BinomialAnalyzer::slice_diff_dist_plot(map<double, int> &slice_data, int total_particles, int div, string name) {
	TCanvas can((name + " canvas").data(), (name + " canvas").data(), canvas_width, canvas_height);
	gStyle->SetOptStat("niMRSK");
	double p = (double)div / 360;
	double q = 1 - p;
	TH1D dist((name + " hist").data(), (name + " hist").data(), total_particles+1, -(float)total_particles * p, (float)total_particles*q);
	TH1D binom((name + " hist_binom").data(), (name + " binom").data(), total_particles+1, -(float)total_particles * p, (float)total_particles*q);
	binom.SetLineColor(kRed);

	for(auto &bin_particles:slice_data) {
		dist.Fill(bin_particles.first, bin_particles.second);
	}
	int norm = dist.Integral();
	int n = total_particles;
	for(int k = 0; k <= n; k++) {
		binom.Fill(k - (double)n * p, norm * TMath::Binomial(n, k) * pow(p, k) * pow(q, n - k));
	}

	dist.Draw("HIST");
	binom.Draw("HISTSAMES");
	can.Update();
	can.Write(name.data());
}


pair<int, int> BinomialAnalyzer::get_canvas_div(int plots) {
	pair<int, int> divs;
	divs.first = ceil(pow(plots, 0.5));
	divs.second = ceil((float)plots / divs.first);

	return(divs);
}


int BinomialAnalyzer::get_marker_style(string key, int alt_key) {
	if(marker_style.find(key) != marker_style.end()) {
		return marker_style[key];
	} else {
		return marker_style_def[alt_key];
	}
}

int BinomialAnalyzer::get_marker_color(string key, int alt_key) {
	if(marker_style.find(key) != marker_style.end()) {
		return marker_color[key];
	} else {
		return marker_color_def[alt_key];
	}
}

double BinomialAnalyzer::get_marker_size(string key, int alt_key) {
	if(marker_style.find(key) != marker_style.end()) {
		return marker_size[key];
	} else {
		return marker_size_def[alt_key];
	}
}
