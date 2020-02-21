///*
// * AmptConvert.cpp
// *
// *  Created on: Feb 15, 2020
// *      Author: Dylan Neff
// */
//
//
//
//
//#include "AmptConverter.h"
//
//
//
//// AmptConverter
//
//// Structors
//AmptConverter::AmptConverter() {
//	// pass
//}
//
//AmptConverter::~AmptConverter() {
//	// pass
//}
//
//
//// Getters
//string AmptConverter::get_in_path() {
//	return(in_path);
//}
//
//string AmptConverter::get_out_path() {
//	return(out_path);
//}
//
//
//// Setters
//void AmptConverter::set_in_path(string path) {
//	in_path = path;
//}
//
//void AmptConverter::set_out_path(string path) {
//	out_path = path;
//}
//
//
//// Doers
//void AmptConverter::convert_trees() {
//	vector<string> in_files = get_files_in_dir(in_path, "root", "path");
//	ROOT::EnableThreadSafety();
//	{
//		ThreadPool pool(threads);
//		for(string path:in_files) {
//			string out_file_path = out_path+get_name_from_path(path);
//			TreeConverter converter(path, out_file_path);
//			TFile *in_file = new TFile(path.data, "READ");
//			TTree *in_tree = (TTree*)in_file->Get(tree_name.data());
//			in_file->Close();
//			delete in_file;
//		}
//	}
//}
//
//
//// ---------------------------------------------------------------------------------------------------
//
//
//// TreeConverter
//
//// Structors
//TreeConverter::TreeConverter(string in_file_path, string out_file_path) {
//	this->in_file_path = in_file_path;
//	this->out_file_path = out_file_path;
//
//	in_file = new TFile(in_file_path.data, "READ");
//	out_file = new TFile(out_file_path.data(), "RECREATE");
//
//	in_tree = (TTree*)in_file->Get(tree_name.data());
//	out_tree = new TTree(reader_pars.get_tree_name());
//
//	event_hist = new TH1I();
//	track_hist = new TH1I();
//}
//
//TreeConverter::~TreeConverter() {
//	in_file->Close();
//	out_file->Close();
//	delete event_hist;
//	delete track_hist;
//	delete in_tree;
//	delete out_tree;
//	delete in_file;
//	delete out_file;
//}
//
//
//// Doers
//void TreeConverter::convert_tree() {
//	generate_cut_hists();
//}
//
//
//// Set empty dummy event/track cut histograms.
//void TreeConverter::generate_cut_hists() {
//	event_hist->SetName(reader_pars.get_event_cut_hist_name().data());
//	event_hist->Write();
//	track_hist->SetName(reader_pars.get_track_cut_hist_name().data());
//	track_hist->Write();
//}
