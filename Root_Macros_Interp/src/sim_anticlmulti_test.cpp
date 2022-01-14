
#include <vector>

using namespace std;


template <typename T>
T gaus_kernel(T x, T m, T s)
{
    T a = (x - m) / s;

    return std::exp(-T(0.5) * a * a);
}


void sim_anticlmulti_test() {
	TRandom3 *sim_rand = new TRandom3(0);
	double new_angle;
	double amp = -0.04;
	double sig = 3.3;
	double base = 1.0;
	float wrap_sigmas = 8.0;

	int n_protons = 60;

	if (n_protons > 0) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		cout << new_angle / 2 / M_PI << endl;
	}

	int points = 1000;
	double x_low = 0;
	double x_up = 2 * M_PI;
	int wrap_num = ceil(wrap_sigmas * sig / (2 * M_PI));
	cout << "wrap_num: " << wrap_num << endl;
//	wrap_num = 1;
	double x_range = x_up - x_low;
	vector<double> prob_vec(points, 1);
	double x, x_val_up;
	int k = 1;
	while (k < n_protons) {
		k++;
		cout << new_angle << endl;
		int cdf_index = 0;
		vector<double> cdf(points + 1, 0);
		for (int i=0; i<points; i++) {
			x = x_low + (i + 0.5) * x_range / points;  // Generate prob points in middle of bins
			double prob_update = base + amp * gaus_kernel(x, new_angle, sig);
			for (int wrap_i = 1; wrap_i <= wrap_num; wrap_i++) {
				prob_update += amp * gaus_kernel(x, new_angle - 2 * M_PI * wrap_i, sig) +
						amp * gaus_kernel(x, new_angle + 2 * M_PI * wrap_i, sig);
			}
			prob_vec[i] *= prob_update;
//			prob_vec[i] *= base + amp * gaus_kernel(x, new_angle, sig) +
//					amp * gaus_kernel(x, new_angle - 2 * M_PI, sig) +
//					amp * gaus_kernel(x, new_angle + 2 * M_PI, sig);
			cdf[cdf_index + 1] = cdf[cdf_index] + prob_vec[i];  // cdf[0] = 0
			cdf_index++;
		}

		vector<double> x_pdf_plot;
		vector<double> x_cdf_plot;
		for (int l = 0; l<points; l++) {
			x_pdf_plot.push_back(x_low + (l + 0.5) * x_range / points);
		}
		for (int l = 0; l<=points; l++) {
			x_cdf_plot.push_back(x_low + l * x_range / points);
		}

		double norm = cdf[points];  // Last point is max, full integral
		for (double &x : cdf) {
			x /= norm;
		}

		if (k == n_protons) {
			TCanvas *can_pdf = new TCanvas();
			TGraph *g_pdf = new TGraph(points, x_pdf_plot.data(), prob_vec.data());
			g_pdf->GetYaxis()->SetRangeUser(0, g_pdf->GetYaxis()->GetXmax());
			g_pdf->Draw();
			TCanvas *can_cdf = new TCanvas();
			TGraph *g_cdf = new TGraph(points+1, x_cdf_plot.data(), cdf.data());
			g_cdf->Draw();
		}

		double cdf_rand = sim_rand->Rndm();
		int i = 1;  // cdf[0] = 0 and rand > 0 so no need to check.
		while (cdf[i] < cdf_rand) { i++; }
		x_val_up = x_low + i * x_range / points;  // Upper bin edge
		new_angle = x_val_up - x_range / points / (cdf[i] - cdf[i - 1]) * (cdf[i] - cdf_rand);
		cout << cdf_rand << "  " << x_val_up << "  " << new_angle << endl;
	}
}
