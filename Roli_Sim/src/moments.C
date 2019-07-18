//____________________________________________________________
//
// Ref: Code of N.R Sahoo (psn0572) and X. Luo (psn0571)::::::
//____________________________________________________________

#include "math.h"
#include "string.h"
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TMath.h"

#define binSize 650

double Kurtosis(double,double,double,double,double);
double Skewness(double,double,double,double);
double Variance(double,double,double);
double Mean(double,double);

void moments(TString inputfilename_profile, int energy)
{
    int ref_beg;
    int check = 0;
    
    TFile *pro_file = new TFile(inputfilename_profile);
    
    if (energy == 7){
        ref_beg = 165;
        ofstream datafilep("data7.txt");
    }
    
    else if (energy == 11){
        ref_beg = 206;
        ofstream datafilep("data11.txt");
    }
    
    else if(energy == 14){
        ref_beg = 225;
        ofstream datafilep("data14.txt");
    }
    
    else if(energy == 19){
        ref_beg = 258;
        ofstream datafilep("data19.txt");
    }
    
    else if(energy == 27){
        ref_beg = 284;
        ofstream datafilep("data27.txt");
    }
    
    else if(energy == 39){
        ref_beg = 307;
        ofstream datafilep("data39.txt");
    }
    
    else if(energy == 62){
        ref_beg = 334;
        ofstream datafilep("data62.txt");
    }
    
    else if(energy == 200){
        ref_beg = 421;
        ofstream datafilep("data200.txt");
    }
    
    else{
        cout << "Please use valid file with a valid energy." << endl;
    }
    
    char name[60];
    
    //ref_beg = 0;
    // ==== define arrays ====
    
    double nevent[binSize];
    
    double netq1[binSize];
    double netq2[binSize];
    double netq3[binSize];
    double netq4[binSize];
    double netq5[binSize];
    double netq6[binSize];
    double netq7[binSize];
    double netq8[binSize];
    
    double mu1[binSize];
    double mu2[binSize];
    double mu3[binSize];
    double mu4[binSize];
    double mu5[binSize];
    double mu6[binSize];
    double mu7[binSize];
    double mu8[binSize];
    
    double m3[binSize];
    double m4[binSize];
    double m5[binSize];
    double m6[binSize];
    double m7[binSize];
    double m8[binSize];
    
    double Err_mean[binSize];
    double Err_sigma[binSize];
    double Err_skew[binSize];
    double Err_kurt[binSize];
    
    double mean_n[binSize];
    double sigma_n[binSize];
    double skew_n[binSize];
    double kurt_n[binSize];
    
    
    for(int l=0; l<binSize; l++){
        
        nevent[l]=0.;
        
        netq1[l]=0.;
        netq2[l]=0.;
        netq3[l]=0.;
        netq4[l]=0.;
        netq5[l]=0.;
        netq6[l]=0.;
        netq7[l]=0.;
        netq8[l]=0.;
        
        mu1[l]=0.;
        mu2[l]=0.;
        mu3[l]=0.;
        mu4[l]=0.;
        mu5[l]=0.;
        mu6[l]=0.;
        mu7[l]=0.;
        mu8[l]=0.;
        
        m3[l]=0.;
        m4[l]=0.;
        m5[l]=0.;
        m6[l]=0.;
        m7[l]=0.;
        m8[l]=0.;
        
        Err_mean[l]=0.;
        Err_sigma[l]=0.;
        Err_skew[l]=0.;
        Err_kurt[l]=0.;
        
        mean_n[l]=0.;
        sigma_n[l] = 0.;
        skew_n[l]  = 0.;
        kurt_n[l]  = 0.;
        
    }
    
    
    
    char name[60];
    
    cout << "Starting actual work !! " << endl;
    ///// ===== Getting profile from input file =====
    
    for(int con = 1; con < 5; con++){
        
        for( int i=1; i< binSize; i++){
            
            sprintf(name,"p1_%d",con);
            int ir =((TProfile *)pro_file->Get(name))->GetBinCenter(i-1);
            if(ir < 0)continue;
            
            sprintf(name,"p1_%d",con);
            nevent[ir] = ((TProfile *)pro_file->Get(name))->GetBinEntries(i-1); //Ni (# of event with i multiplicity)
            double check_error = ((TProfile *)pro_file->Get(name))->GetBinError(i-1);
            if (check_error < pow(10,-7)) continue;
            
            if(nevent[ir] ==0)continue;
            
            sprintf(name,"p1_%d",con);
            netq1[ir] = nevent[ir]*((TProfile *)pro_file->Get(name))->GetBinContent(i-1);
            double check_error = ((TProfile *)pro_file->Get(name))->GetBinError(i-1);
            if (check_error < pow(10,-7)) continue;
            
            sprintf(name,"p2_%d",con);
            netq2[ir] = nevent[ir]*((TProfile *)pro_file->Get(name))->GetBinContent(i-1);
            double check_error = ((TProfile *)pro_file->Get(name))->GetBinError(i-1);
            if (check_error < pow(10,-7)) continue;
            
            sprintf(name,"p3_%d",con);
            netq3[ir] = nevent[ir]*((TProfile *)pro_file->Get(name))->GetBinContent(i-1);
            double check_error = ((TProfile *)pro_file->Get(name))->GetBinError(i-1);
            if (check_error < pow(10,-7)) continue;
            
            sprintf(name,"p4_%d",con);
            netq4[ir] = nevent[ir]*((TProfile *)pro_file->Get(name))->GetBinContent(i-1);
            double check_error = ((TProfile *)pro_file->Get(name))->GetBinError(i-1);
            if (check_error < pow(10,-7)) continue;
            
            sprintf(name,"p5_%d",con);
            netq5[ir] = nevent[ir]*((TProfile *)pro_file->Get(name))->GetBinContent(i-1);
            double check_error = ((TProfile *)pro_file->Get(name))->GetBinError(i-1);
            if (check_error < pow(10,-7)) continue;
            
            sprintf(name,"p6_%d",con);
            netq6[ir] = nevent[ir]*((TProfile *)pro_file->Get(name))->GetBinContent(i-1);
            double check_error = ((TProfile *)pro_file->Get(name))->GetBinError(i-1);
            if (check_error < pow(10,-7)) continue;
            
            sprintf(name,"p7_%d",con);
            netq7[ir] = nevent[ir]*((TProfile *)pro_file->Get(name))->GetBinContent(i-1);
            double check_error = ((TProfile *)pro_file->Get(name))->GetBinError(i-1);
            if (check_error < pow(10,-7)) continue;
            
            sprintf(name,"p8_%d",con);
            netq8[ir] = nevent[ir]*((TProfile *)pro_file->Get(name))->GetBinContent(i-1);
            double check_error = ((TProfile *)pro_file->Get(name))->GetBinError(i-1);
            if (check_error < pow(10,-7)) continue;
            
            
            
            ///// will be need for error calculation
            int evt = nevent[ir];
            if (evt == 0) continue;
            
            mu1[ir] = netq1[ir]/evt;
            mu2[ir] = netq2[ir]/evt - pow(netq1[ir]/evt,2);
            mu3[ir] = netq3[ir]/evt - 3*(netq1[ir]/evt)*(netq2[ir]/evt)+ 2*pow(netq1[ir]/evt,3);
            mu4[ir] = netq4[ir]/evt - 4*(netq1[ir]/evt)*(netq3[ir]/evt) + 6*pow(netq1[ir]/evt,2)*netq2[ir]/evt - 3*pow(netq1[ir]/evt,4);
            
            mu5[ir] = netq5[ir]/evt - 10*(netq2[ir]/evt)*pow(netq1[ir]/evt,3) + 10*(netq3[ir]/evt)*pow(netq1[ir]/evt,2) - 5*(netq4[ir]/evt)*(netq1[ir]/evt)+ 4*pow(netq1[ir]/evt,5);
            
            mu6[ir] = netq6[ir]/evt + 15*(netq2[ir]/evt)*pow(netq1[ir]/evt,4) - 20*(netq3[ir]/evt)*pow(netq1[ir]/evt,3) +15*(netq4[ir]/evt)*pow(netq1[ir]/evt,2)
            - 6*(netq5[ir]/evt)*(netq1[ir]/evt) - 5*pow(netq1[ir]/evt,6);
            
            mu7[ir] = netq7[ir]/evt - 21*(netq2[ir]/evt)*pow(netq1[ir]/evt,5) + 35*(netq3[ir]/evt)*pow(netq1[ir]/evt,4) - 35*(netq4[ir]/evt)*pow(netq1[ir]/evt,3)
            + 21*(netq5[ir]/evt)*pow(netq1[ir]/evt,2) - 7*(netq6[ir]/evt)*(netq1[ir]/evt) + 6*pow(netq1[ir]/evt,7);
            
            mu8[ir] = netq8[ir]/evt + 28*(netq2[ir]/evt)*pow(netq1[ir]/evt,6) - 56*(netq3[ir]/evt)*pow(netq1[ir]/evt,5) + 70*(netq4[ir]/evt)*pow(netq1[ir]/evt,4)
            - 56*(netq5[ir]/evt)*pow(netq1[ir]/evt,3)+28*(netq6[ir]/evt)*pow(netq1[ir]/evt,2) - 8*(netq7[ir]/evt)*(netq1[ir]/evt) - 7*pow(netq1[ir]/evt,8);
            
            
            double den = Variance(netq1[ir],netq2[ir],evt);
            if(den<pow(10,-7)) continue; //go back if sigma is model_zero
            
            
            //::::::::Delta Theorem for error:::::::::
            m3[ir]    =  (mu3[ir])/pow(den,3);
            m4[ir]    =  (mu4[ir])/pow(den,4);
            m5[ir]    =  (mu5[ir])/pow(den,5);
            m6[ir]    =  (mu6[ir])/pow(den,6);
            m7[ir]    =  (mu7[ir])/pow(den,7);
            m8[ir]    =  (mu8[ir])/pow(den,8);
            
            double check_sigma = ((m4[ir] - 1)*den*den)/(4*evt);
            
            double check_skew = (9 - 6*m4[ir] + m3[ir]*m3[ir]*(35 + 9*m4[ir])/4 - 3*m3[ir]*m5[ir] + m6[ir])/(evt);
            
            double check_kurt = (-m4[ir]*m4[ir] + 4*m4[ir]*m4[ir]*m4[ir] + 16*m3[ir]*m3[ir]*( 1 + m4[ir] ) - 8*m3[ir]*m5[ir] - 4*m4[ir]*m6[ir] + m8[ir])/(evt);
            
            
            if(check_sigma < 0 || check_skew < 0 || check_kurt < 0) {cout << "DANGER in refmult =  " << ir << endl; continue;}
            
            Err_mean[ir] = den/sqrt(evt);
            Err_sigma[ir] = sqrt(check_sigma);
            Err_skew[ir]  = sqrt(check_skew);
            Err_kurt[ir]  = sqrt(check_kurt);
            
            
            ////calculate moments for each refmult bin
            
            mean_n[ir] = Mean(netq1[ir],evt);
            sigma_n[ir] = Variance(netq1[ir],netq2[ir],evt);
            skew_n[ir]  = Skewness(netq1[ir],netq2[ir],netq3[ir],evt);
            kurt_n[ir]  = Kurtosis(netq1[ir],netq2[ir],netq3[ir],netq4[ir],evt);
            
            
        }//refmult bin
        
        
        cout << "Puhh" << endl;
        
        ////////calculate cumulant for each centrality bin and phi bin
        
        double sum_mean, sum_sigma, sum_skew, sum_kurt, sum_Sigma_err, sum_skew_err, sum_kurt_err, event_cen;
        
        
        for( int i=ref_beg; i<= 650; i++){// i corresponds to refmult
            
            sum_mean   += mean_n[i]*nevent[i];
            sum_sigma  += sigma_n[i]*nevent[i];
            sum_skew   += skew_n[i]*nevent[i];
            sum_kurt   += kurt_n[i]*nevent[i];
            
            //for error calculation
            sum_Sigma_err  += pow(nevent[i],2)*pow(Err_sigma[i],2);
            sum_skew_err  += pow(nevent[i],2)*pow(Err_skew[i],2);
            sum_kurt_err  += pow(nevent[i],2)*pow(Err_kurt[i],2);
            
            event_cen += nevent[i]; //total weight
            
        }//end of i loop
        
        if(event_cen == 0) continue;
        
        //:::::::::: final values of moments :::::::::
        double Mean_n    = sum_mean/event_cen;
        double St_dev_n  = sum_sigma/event_cen;
        double Skewness_n = sum_skew/event_cen;
        double Kurtosis_n = sum_kurt/event_cen;
        
        
        //:::::::::: final  error on moments :::::::::
        
        double err_mean  = St_dev_n/sqrt(event_cen);
        double err_sigma = TMath::Sqrt(sum_Sigma_err/pow(event_cen,2)) ;
        double err_skew  = TMath::Sqrt(sum_skew_err/pow(event_cen,2)) ;
        double err_kurto = TMath::Sqrt(sum_kurt_err/pow(event_cen,2)) ;
        
        
        //write in a ascii file
        
        datafilep<<con<<"\t"<<Mean_n<<"\t"<<err_mean<<"\t"<<St_dev_n<<"\t"<<err_sigma<<"\t"<<Skewness_n<<"\t"<<err_skew<<"\t"<<Kurtosis_n<<"\t"<<err_kurto<<endl;
        
        Mean_n = 0; St_dev_n = 0; Skewness_n = 0; Kurtosis_n = 0;
        err_mean = 0; err_sigma = 0; err_skew = 0; err_kurto = 0;
    }
    
} // end of main function




double Kurtosis(double sum,double sum2, double sum3,double sum4, double Nevents){
    
    // cout<<"Invoking the kurtosis function....."<<endl;
    double fourth_moment=6*pow(sum/Nevents,2)*(sum2/Nevents)-4*pow(sum/Nevents,1)*(sum3/Nevents)+(sum4/Nevents)-3*pow(sum/Nevents,4);
    double sec_moment=pow(sum2/Nevents,1)-pow(sum/Nevents,2);
    double kurt=fourth_moment/pow(sec_moment,2)-3.0;
    return kurt;
    
}

double Skewness(double sum,double sum2, double sum3, double Nevents){
    
    // cout<<"Invoking the skewness function....."<<endl;
    double three_moment=(sum3/Nevents)-3*pow(sum/Nevents,1)*(sum2/Nevents)+2*pow(sum/Nevents,3);
    double sec_moment=pow(sum2/Nevents,1)-pow(sum/Nevents,2);
    double skew=three_moment/pow(sec_moment,1.5);
    return skew;
}

double Variance(double sum,double sum2, double Nevents){
    
    //  cout<<"Invoking the variance function....."<<endl;
    double sec_moment=pow(sum2/Nevents,1)-pow(sum/Nevents,2);
    if(sec_moment<0) sec_moment = 0;
    return sqrt(sec_moment);
}

double Mean(double sum,double Nevents){
    // cout<<"Invoking the mean function....."<<endl;
    return sum/Nevents;
}
