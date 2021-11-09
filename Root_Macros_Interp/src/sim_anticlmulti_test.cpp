
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
	double amp = 0.5;
	double sig = 0.3;

	int n_protons = 60;

	if (n_protons > 0) {
		new_angle = sim_rand->Rndm() * 2 * M_PI;
		cout << new_angle / 2 / M_PI << endl;
	}

	int pdf_points = 999;  // Want multiple of 3 for this specific situation
	int cdf_points = pdf_points / 3;
	int x_index_low, x_index_up;
	double x_low = -2 * M_PI;
	double x_up = 4 * M_PI;
	double cdf_x_low = 0;
	double cdf_x_up = 2 * M_PI;
	double x_range = x_up - x_low;
	double cdf_x_range = cdf_x_up - cdf_x_low;
	vector<double> prob_vec(pdf_points, 1);
	double x, x_index, p_interp, x_val_low, x_val_up;
	int k = 1;
	while (k < n_protons) {
		k++;
		cout << new_angle << endl;
		int cdf_index = 0;
		vector<double> cdf(cdf_points + 1, 0);
		for (int i=0; i<pdf_points; i++) {
			x = x_low + (i + 0.5) * x_range / pdf_points;  // Generate prob points in middle of bins
			prob_vec[i] *= 1 - amp * gaus_kernel(x, new_angle, sig) -
					amp * gaus_kernel(x, new_angle - 2 * M_PI, sig) -
					amp * gaus_kernel(x, new_angle + 2 * M_PI, sig);
			if (x > cdf_x_low && x < cdf_x_up) {
				cdf[cdf_index + 1] = cdf[cdf_index] + prob_vec[i];  // cdf[0] = 0
				cdf_index++;
			}
		}

		vector<double> x_pdf_plot;
		vector<double> x_cdf_plot;
		for (int l = 0; l<pdf_points; l++) {
			x_pdf_plot.push_back(x_low + (l + 0.5) * x_range / pdf_points);
		}
		for (int l = 0; l<=cdf_points; l++) {
			x_cdf_plot.push_back(cdf_x_low + l * cdf_x_range / cdf_points);
		}

		double norm = cdf[cdf_points];  // Last point is max, full integral
		for (double &x : cdf) {
			x /= norm;
		}

		if (k == n_protons) {
			TCanvas *can_pdf = new TCanvas();
			TGraph *g_pdf = new TGraph(pdf_points, x_pdf_plot.data(), prob_vec.data());
			g_pdf->Draw();
			TCanvas *can_cdf = new TCanvas();
			TGraph *g_cdf = new TGraph(cdf_points+1, x_cdf_plot.data(), cdf.data());
			g_cdf->Draw();
		}

		double x_rand = sim_rand->Rndm();
		int i = 1;  // cdf[0] = 0 and rand > 0 so no need to check.
		while (cdf[i] < x_rand) { i++; }
		x_val_up = cdf_x_low + i * cdf_x_range / cdf_points;  // Upper bin edge
		new_angle = x_val_up - cdf_x_range / cdf_points / (cdf[i] - cdf[i - 1]) * (cdf[i] - x_rand);
		cout << x_rand << "  " << x_val_up << "  " << new_angle << endl;
	}
}
