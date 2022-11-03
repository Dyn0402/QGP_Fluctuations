
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

#include <TGraphErrors.h>
#include <TPad.h>

using namespace std;


class Measure {
private:
	double val, err;
public:
	// Constructors
	Measure() {
		val = err = 0.0;
	}

	Measure(double val_in) {
		val = val_in;
		err = 0.0;
	}

	Measure(double val_in, double err_in) {
		val = val_in;
		err = err_in;
	}

	// Destructor
	~Measure() {
		// Nothing
	}


	// Operators
	Measure operator + (Measure const &obj) const {
		Measure result;
		result.set_val(val + obj.val);
		result.set_err(std::pow(std::pow(err, 2) + std::pow(obj.err, 2), 0.5));

		return result;
	}

	Measure operator + (double d) const {
		Measure result;
		result.set_val(val + d);
		result.set_err(err);

		return result;
	}

	Measure operator - (Measure const &obj) const {
		Measure result;
		result.set_val(val - obj.val);
		result.set_err(std::pow(std::pow(err, 2) + std::pow(obj.err, 2), 0.5));

		return result;
	}

	Measure operator - (double d) const {
		Measure result;
		result.set_val(val - d);
		result.set_err(err);

		return result;
	}

	Measure operator * (Measure const &obj) const {
		Measure result;
		result.set_val(val * obj.val);
		result.set_err(fabs(result.get_val()) * std::pow(std::pow(err/val, 2) + std::pow(obj.err/obj.val, 2), 0.5));

		return result;
	}

	Measure operator * (double d) const {
		Measure result;
		result.set_val(val * d);
		result.set_err(err * fabs(d));

		return result;
	}

	Measure operator / (Measure const &obj) const {
		Measure result;
		result.set_val(val / obj.val);
		result.set_err(fabs(result.get_val()) * std::pow(std::pow(err/val, 2) + std::pow(obj.err/obj.val, 2), 0.5));

		return result;
	}

	Measure operator / (double d) const {
		Measure result;
		result.set_val(val / d);
		result.set_err(err / fabs(d));

		return result;
	}

	Measure pow(double exp) const {
		Measure result;
		result.set_val(std::pow(val, exp));
		result.set_err(fabs(result.get_val() * exp * err / val));

		return result;
	}

	operator std::string() const {
		std::ostringstream os("");
		os << std::setprecision(15) << val << " ±" << std::setprecision(15) << err << std::ends;
		return os.str();
	}


	// Getters
	double get_val() {
		return val;
	}

	double get_err() {
		return err;
	}


	// Setters
	void set_val(double val_in) {
		val = val_in;
	}

	void set_err(double err_in) {
		err = err_in;
	}

};


struct plot_data {
	vector<double> val;
	vector<double> stat;
	vector<double> sys;
};


// Emulation of Python split function. Split string into vector of strings on delim.
vector<string> split(string main, char delim) {
	if(main.size() == 0) { return {}; }

	vector<string> split_strings {""};
	for(char x:main) {
		if(x == delim) {
			if(split_strings.back() != "") {
				split_strings.push_back("");
			}
		} else {
			split_strings.back() += x;
		}
	}
	return(split_strings);
}


// Emulation of Python split function. Split string into vector of strings on delim.
vector<string> split(string main, string delim) {
	if(delim.size() <= 0) { return {}; }
	size_t index = main.find(delim);

	vector<string> split_strings {};
	do {
		split_strings.push_back(main.substr(0, index));
		main = main.substr(index + delim.size());
		index = main.find(delim);
	}
	while(index != string::npos);

	if(main.size() > 0) { split_strings.push_back(main); }

	return(split_strings);
}


TGraphErrors* graph_x_vs_y_err(vector<double> x, vector<double> y, vector<double> x_err, vector<double> y_err) {
	TGraphErrors *graph = new TGraphErrors((int)x.size(), x.data(), y.data(), x_err.data(), y_err.data());
	return(graph);
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
