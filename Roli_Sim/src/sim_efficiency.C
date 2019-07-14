#include <math.h>
#include <string.h>
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TRandom3.h>
#include <TMath.h>

using namespace std;

int getPhiBin(double, int);


void sim_efficiency(TString outputFile, double pc, double xcluster, double mean, int divs) {

	// ------ define variables -------
	int nevents = 10000000; //Total number of events to run on

	int nParticle, nParticle1, phibin, check=0, check1=0, nglob, a, c;

	double xevent, xphi, phi, efficiency, fm, x;
	double pratio;
	double twoPi = 2*3.14159;
	//double mean = 25; ///// <--- mean of the Poisson

	const int max_nPhiBin = 10;
	int nPhiBin = divs+1;
	int num_particle[max_nPhiBin] = {0,};
	int num_mixed[max_nPhiBin] = {0,};

	char name[60];

    // ------ define histograms -------

    TFile *n = new TFile(outputFile,"RECREATE");
    if(!n){
        cout << "Output file cannot be opened" << endl;
        exit(0);
    }

    TH1F *hTrue = new TH1F("hTrue","true particle multiplicity  distribution ",101,-0.5,100.0);
    TH1F *hMult = new TH1F("hMult","total multiplicity  distribution ",250,0,250);
    TH1F *hMultM = new TH1F("hMultM","mixed multiplicity  distribution ",250,0,250);

    TH1F *hPileup = new TH1F("hPileup","pileup multiplicity  distribution ",50,0,50);
    TH1F *hReject = new TH1F("hReject","rejection distribution ",250,0,250);

    TH1F *hEfficiency = new TH1F("hEfficiency", "efficiency of particle detection", 200, 0, 1);

    TH1F *hphi  = new TH1F("hphi","phi distribution",100,-10,10);
    TH1F *hmphi  = new TH1F("hmphi","phi distribution for mixed",100,-10,10);
    TH1F *hbin = new TH1F("hbin","phi bin distribution ",10,0,10);

    TH1F *hratio[max_nPhiBin];
    TH1F *hmixed[max_nPhiBin];
	for(int i = 0; i < nPhiBin; i++){
		sprintf(name,"hratio_Phi_%d",i);
		hratio[i] = new TH1F(name,"",22,-0.05,1.05);

		sprintf(name,"hmixed_Phi_%d",i);
		hmixed[i] = new TH1F(name,"",24,-0.05,1.15);
	}

	TProfile *hp[13];
	for(int i = 0; i <= 12; i++){
		sprintf(name,"hp_%d",i);
		hp[i] = new TProfile(name,"",100,0,100);
	}

	TProfile *hParticle = new TProfile("hParticle","moments of ratio (particle)",20,-0.5,19.5);
	TProfile *hMixed = new TProfile("hMixed","moments of ratio (mixed)",20,-0.5,19.5);


    //Initialize random number generators.

    TRandom3 particle_generator(0);
    TRandom3 eff(0);
    TRandom3 overlap(0);
    TRandom3 event_cluster(0);
    TRandom3 particle_cluster(0);
    TRandom3 phi_bin(0);
    TRandom3 myRandom(0);



    for (int i = 0; i < nevents; i++){

        if(i%50000==0)cout << "Working on event  " << i << endl;

        nParticle = 0;
        nParticle1 = 0;

        for(int num = 0; num < nPhiBin; num++){
            num_particle[num] = 0;
        }

        nParticle = particle_generator.Poisson(mean);
        hTrue->Fill(nParticle);




        //// introducing overlap in this block ///////////

         /*
         x = overlap.Rndm();
         if(x < 0.1){ //Define probability of events overlapping
         x = overlap.Rndm()*10;
         nParticle1 = particle_generator.Poisson(x);
         hPileup->Fill(nParticle1);

         nParticle = nParticle + nParticle1;
         }
        */
        ///////////////////////////////////////////////////


        ///// introducing efficiency in this block /////////
        /*
        efficiency = 0.8;// - 0.0003*(nParticle-25);
        nParticle1 = eff.Binomial(nParticle,efficiency);
        efficiency = (double) nParticle1/nParticle;

        hEfficiency->Fill(efficiency);

        nParticle = nParticle1;
        */
        ////////////////////////////////////////////////////


        ///// introducing rejection from tail in this block /////////
        /*
         x = overlap.Rndm();
         if(nParticle > 40 && x < 0.1){ //Define probability of events overlapping

         hReject->Fill(nParticle);
         nParticle = 0;
         }
        */

        ////////////////////////////////////////////////////

        hMult->Fill(nParticle);

        /////////// cumulant analysis ////////////////////

        nglob = myRandom.Rndm()*50;

        for(int order = 0; order <= 8; order++){

            fm = 1;

            for(int j = 0; j <= order-1; j++){

                fm = fm * (nParticle - j);

            }

            hp[order]->Fill(nglob, fm);
        }

        //////////////////////////////////////////////////

        nParticle1 = 0;
        if(nParticle < 2) continue;

        ///// density fluctuations ///////////////////

        xevent = event_cluster.Rndm();

        if(xevent < pc){ //Define probability of clustering in event


            /// ***** change percentage here ******
//            xcluster = 0.2; //Define percentage of particle clustering
            // xcluster = particle_cluster.Rndm()*(0.1) // Varying particle cluster from 0 to 10%


            nParticle1 = xcluster * nParticle;
            xphi = twoPi*phi_bin.Rndm();

            ////// introduce a scale of 2 units in phi

            for(int j = 0; j < nParticle1; j++){

                phi = xphi;//(xphi-1) + 1.0 * phi_bin.Rndm();
                if(phi < 0) phi = phi + twoPi;
                if(phi > twoPi) phi = phi - twoPi;

                phibin = getPhiBin(phi,nPhiBin-1);
                if(phibin > (nPhiBin-1) || phibin < 1) {cout << "check1" << endl; continue;}
                hphi->Fill(phi);
                hbin->Fill(phibin);
                num_particle[phibin]++;
            }
        }

        for(int j = 0; j < (nParticle-nParticle1); j++){

            phi = twoPi * phi_bin.Rndm();
            hphi->Fill(phi);
            phibin = getPhiBin(phi,nPhiBin-1);
            if(phibin > (nPhiBin-1) || phibin < 1) {cout << "check2" << endl; continue;}

            num_particle[phibin]++;
        }

        for(int num = 1; num < nPhiBin; num++){

            pratio = (double)num_particle[num]/nParticle;
            hratio[num]->Fill(pratio);
            hratio[0]->Fill(pratio);

            for(int k = 0; k < 10; k++){
                hParticle->Fill(k,pow(pratio,k));
            }

        }
    }

    n->cd();
    n->Write();
    n->Close();

}

int getPhiBin(double phi, int N){

    int bin=0;
    double twoPi = 2*3.14159;

    if(phi > 0 && phi <= twoPi/N) bin=1;
    if(phi > twoPi/N && phi <= 2*twoPi/N) bin=2;
    if(phi > 2*twoPi/N && phi <= 3*twoPi/N) bin=3;
    if(phi > 3*twoPi/N && phi <= 4*twoPi/N) bin=4;
    if(phi > 4*twoPi/N && phi <= 5*twoPi/N) bin=5;
    if(phi > 5*twoPi/N && phi <= 6*twoPi/N) bin=6;
    return bin;

}

