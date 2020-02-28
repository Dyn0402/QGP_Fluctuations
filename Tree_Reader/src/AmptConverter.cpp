/*
 * AmptConvert.cpp
 *
 *  Created on: Feb 15, 2020
 *      Author: Dylan Neff
 */




#include "AmptConverter.h"

#include "file_io.h"



// AmptConverter

// Structors
AmptConverter::AmptConverter(string in_path, string out_path) {
	this->in_path = in_path;
	this->out_path = out_path;
}

AmptConverter::AmptConverter() {
	// pass
}

AmptConverter::~AmptConverter() {
	// pass
}


// Getters
string AmptConverter::get_in_path() {
	return(in_path);
}

string AmptConverter::get_out_path() {
	return(out_path);
}


// Setters
void AmptConverter::set_in_path(string path) {
	in_path = path;
}

void AmptConverter::set_out_path(string path) {
	out_path = path;
}


// Doers
void AmptConverter::convert_trees() {
	vector<string> in_files = get_files_in_dir(in_path, "root", "path");
	ROOT::EnableThreadSafety();
	{
		ThreadPool pool(threads);
		for(string path:in_files) {
			string out_file_path = out_path + get_name_from_path(path);
			TreeConverter converter(path, out_file_path);
			converter.convert_tree();
//			pool.enqueue(&TreeConverter::convert_tree, converter);
		}
	}
}


// ---------------------------------------------------------------------------------------------------


// TreeConverter

// Structors
TreeConverter::TreeConverter(string in_file_path, string out_file_path) {
	this->in_file_path = in_file_path;
	this->out_file_path = out_file_path;

	in_file = new TFile(in_file_path.data(), "READ");
	out_file = new TFile(out_file_path.data(), "RECREATE");

	in_tree = (TTree*)in_file->Get(tree_name.data());
	out_tree = new TTree(reader_pars.get_tree_name().data(),reader_pars.get_tree_name().data());

	event_hist = new TH1I(reader_pars.get_event_cut_hist_name().data(), reader_pars.get_event_cut_hist_name().data(), 8, -0.5, 7.5);
	track_hist = new TH1I(reader_pars.get_track_cut_hist_name().data(), reader_pars.get_track_cut_hist_name().data(), 12, -0.5, 11.5);
}

TreeConverter::~TreeConverter() {
	in_file->Close();
	out_file->Close();
//	delete event_hist;  This breaks. Figure out later.
//	delete track_hist;
//	delete in_tree;
//	delete out_tree;
//	delete in_file;
//	delete out_file;
}


// Doers
void TreeConverter::convert_tree() {
	cout << "Converting " << in_file_path << " to " << out_file_path << endl;
	generate_cut_hists();
	nsm_ampt_leaves leaves = get_nsm_ampt_leaves();
//	Event *event_pointer = new Event;
	Event event;
	out_tree->Branch("Event", &event);

	int event_index = 0;
	while(in_tree->GetEvent(event_index)) {
		event.clear();
		event.set_ref(leaves.mult->GetValue());
		get_proton_info(leaves, event);
		get_other_info(leaves, event);
		out_tree->Fill();
		++event_index;
	}
	out_file->Write();
}


// Set empty dummy event/track cut histograms.
void TreeConverter::generate_cut_hists() {
//	event_hist->SetName(reader_pars.get_event_cut_hist_name().data());
	event_hist->Write();
//	track_hist->SetName(reader_pars.get_track_cut_hist_name().data());
	track_hist->Write();
}


// Set tree leaves to appropriate addresses.
nsm_ampt_leaves TreeConverter::get_nsm_ampt_leaves() {
	nsm_ampt_leaves leaves;

	leaves.mult = in_tree->GetLeaf("Mult");
	leaves.pid = in_tree->GetLeaf("PID");
	leaves.px = in_tree->GetLeaf("Px");
	leaves.py = in_tree->GetLeaf("Py");
	leaves.pz = in_tree->GetLeaf("Pz");

	return(leaves);
}


// Iterate over each track in event, extract all relevant proton info, and add it to event.
void TreeConverter::get_proton_info(nsm_ampt_leaves leaves, Event &event) {
	vector<Track> protons;
	for(unsigned track_index=0; track_index<event.get_ref(); track_index++) {
		if(pid_codes[leaves.pid->GetValue(track_index)] == "proton") {
			TVector3 p(leaves.px->GetValue(track_index), leaves.py->GetValue(track_index), leaves.pz->GetValue(track_index));
			if(p.Perp() >= cut.p_min && p.Perp() >= cut.pt_min && p.Perp() <= cut.pt_max) {
				Track new_proton;
				new_proton.set_p(p.Mag());
				new_proton.set_pt(p.Perp());
				new_proton.set_phi(p.Phi());
				new_proton.set_eta(p.PseudoRapidity());
				new_proton.set_dca(track_default.dca);
				new_proton.set_nsigma(track_default.nsigma);
				new_proton.set_beta(track_default.beta);
				new_proton.set_charge(track_default.charge);
				protons.push_back(new_proton);
			}
		}
	}
	event.set_protons(protons);
}


// Fill event with other event level info
void TreeConverter::get_other_info(nsm_ampt_leaves leaves, Event &event) {
	event.set_vx(event_default.vx);
	event.set_vy(event_default.vy);
	event.set_vz(event_default.vz);

	event.set_run(event_default.run);
	event.set_refn(event.get_ref());
	event.set_btof(event.get_ref() * event_default.btof_ref_multi);
}
