

void anticluster_test_hold() {
	string gaus_wrap1_name = "gaus_wrap1_";
	string prob_name = "anti_clust_prob_";
	int num = 10000;
	double last_proton = 2.18;
	double spread = 0.3;
	TH1D* hist = new TH1D("test", "sample", 100, 0, 2 * M_PI);
	TF1 *gaus_wrap1 = new TF1(gaus_wrap1_name.data(), "gaus(0) + gaus(3) + gaus(6)", -2 * M_PI, 4 * M_PI);
	gaus_wrap1->SetParameters(1, last_proton, spread, 1, last_proton - 2 * M_PI, spread, 1, last_proton + 2 * M_PI, spread);
	double amp = gaus_wrap1->Eval(last_proton);
	TF1 *prob = new TF1(prob_name.data(), "1 - gaus(0) - gaus(3) - gaus(6)", 0, 2 * M_PI);
	prob->SetParameters(1 / amp, 0.7, spread, 1 / amp, 0.7 - 2 * M_PI, spread, 1 / amp, 0.7 + 2 * M_PI, spread);
	prob->GetRandom();
	prob->SetParameters(1 / amp, last_proton, spread, 1 / amp, last_proton - 2 * M_PI, spread, 1 / amp, last_proton + 2 * M_PI, spread);
	for (int i = 0; i < num; i++) {
		hist->Fill(prob->GetRandom());
	}
	cout << prob->Integral(0, 2 * M_PI) << " Norm is wrong between TF1 and TH1, couldn't figure it out fast" << endl;
	hist->Scale(100 / hist->Integral());
	//double group_angle = prob->GetRandom();
	hist->Draw();
	prob->Draw("same");
	//cin >> group_angle;
}


void anticluster_test() {
	string gaus_wrap1_name = "gaus_wrap1_";
	string prob_name = "anti_clust_prob_";
	int num = 10000;
	double last_proton = 2.18;
	double gen_center = M_PI;
	double spread = 0.3;
	TH1D* hist = new TH1D("test", "sample", 100, 0, 2 * M_PI);
	TF1* gaus_wrap1 = new TF1(gaus_wrap1_name.data(), "gaus(0) + gaus(3) + gaus(6)", -2 * M_PI, 4 * M_PI);
	gaus_wrap1->SetParameters(1, gen_center, spread, 1, gen_center - 2 * M_PI, spread, 1, gen_center + 2 * M_PI, spread);
	double amp = gaus_wrap1->Eval(gen_center);
	TF1* prob = new TF1(prob_name.data(), "1 - gaus(0) - gaus(3) - gaus(6)", 0, 2 * M_PI);
	prob->SetParameters(1 / amp, 0.7, spread, 1 / amp, 0.7 - 2 * M_PI, spread, 1 / amp, 0.7 + 2 * M_PI, spread);
	prob->GetRandom();
	prob->SetParameters(1 / amp, gen_center, spread, 1 / amp, gen_center - 2 * M_PI, spread, 1 / amp, gen_center + 2 * M_PI, spread);
	prob->GetRandom();
	for (int i = 0; i < num; i++) {
		double val = prob->GetRandom() - (M_PI - last_proton);
		val = fmod(val, 2 * M_PI);
		if (val < 0) { val += 2 * M_PI; }
		hist->Fill(val);
	}
	cout << prob->Integral(0, 2 * M_PI) << " Norm is wrong between TF1 and TH1, couldn't figure it out fast" << endl;
	hist->Scale(100 / hist->Integral());
	//double group_angle = prob->GetRandom();
	hist->Draw();
	prob->Draw("same");
	//cin >> group_angle;
}