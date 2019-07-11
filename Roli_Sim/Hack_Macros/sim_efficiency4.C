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
#include "TRandom3.h"
#include "TMath.h"

using namespace std;

int getPhiBin(double, int);
int factorial(int);
void makeMixedPairs(int);
void copyCurrentToBuffer(int);
void DeclareHistograms();


//----- mixing variables ------

int CurrentEvent_nProton;
float CurrentEvent_ProtonPhi[25000];

int BufferPointer;

int BufferEvent_NEvents[250];

int BufferEvent_Full[250];

int BufferEvent_nProton[25000];
float BufferEvent_ProtonPhi[25000][25000];

int iran ;

// ------ define variables -------
int nevents = 10000000; //Total number of events to run on

int nParticle, nParticle1, phibin, check=0, check1=0, nglob, a, c;

double xevent, xphi, phi, efficiency, fm, x;
double pratio;
double twoPi = 2*3.14159;
//double mean = 25; ///// <--- mean of the Poisson

const int nPhiBin = 4+1;
int num_particle[nPhiBin] = {0,};
int num_mixed[nPhiBin] = {0,};

char name[60];

// ------ define histograms ---------
TH1F* hTrue;
TH1F* hMult;
TH1F* hMultM;
TH1F* hPileup;
TH1F* hReject;
TH1F* hEfficiency;
TH1F* hphi;
TH1F* hmphi;
TH1F* hbin;

TH1F *hratio[nPhiBin];
TH1F *hmixed[nPhiBin];

TProfile *hp[13];
TProfile *hParticle;
TProfile *hMixed;


void sim_efficiency2(TString outputFile, double pc, double xcluster, double mean) {

    // ------ define histograms -------

    TFile *n = new TFile(outputFile,"RECREATE");
    if(!n){
        cout << "Output file cannot be opened" << endl;
        exit(0);
    }

    DeclareHistograms();


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
        CurrentEvent_nProton = 0;

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

                phi = (xphi-1) + 1.0 * phi_bin.Rndm();
                if(phi < 0) phi = phi + twoPi;
                if(phi > twoPi) phi = phi - twoPi;

                phibin = getPhiBin(phi,nPhiBin-1);
                if(phibin > (nPhiBin-1) || phibin < 1) {cout << "check1" << endl; continue;}
                hphi->Fill(phi);
                hbin->Fill(phibin);
                num_particle[phibin]++;

                CurrentEvent_ProtonPhi[CurrentEvent_nProton] = phi;
                CurrentEvent_nProton++;
            }
        }

        for(int j = 0; j < (nParticle-nParticle1); j++){

            phi = twoPi * phi_bin.Rndm();
            hphi->Fill(phi);
            phibin = getPhiBin(phi,nPhiBin-1);
            if(phibin > (nPhiBin-1) || phibin < 1) {cout << "check2" << endl; continue;}

            num_particle[phibin]++;

            CurrentEvent_ProtonPhi[CurrentEvent_nProton] = phi;
            CurrentEvent_nProton++;
        }

        if(CurrentEvent_nProton != nParticle) cout << "What is this ??" << endl;

        for(int num = 1; num < nPhiBin; num++){

            pratio = (double)num_particle[num]/nParticle;
            hratio[num]->Fill(pratio);
            hratio[0]->Fill(pratio);

            for(int k = 0; k < 10; k++){
                hParticle->Fill(k,pow(pratio,k));
            }

        }

        BufferPointer = 1;

        copyCurrentToBuffer(BufferPointer);
        makeMixedPairs(BufferPointer);
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

int factorial(int N){

    int fact = 1;

    if(N > 0){

        for(int i = 1; i <= N; i++){

            fact = fact*i;
        }
    }

    return fact;

}

void makeMixedPairs(int bufferPointer){

    TRandom ran(0);

    nParticle = 0;

    for(int num = 0; num < nPhiBin; num++){
        num_mixed[num] = 0;
    }

    int nmax = CurrentEvent_nProton;

    while(nParticle != nmax){

        phi = -999;

        while(phi == -999){

            int k = ran.Rndm() * BufferEvent_NEvents[bufferPointer];
            int j = ran.Rndm() * BufferEvent_nProton[k+bufferPointer];

            phi = BufferEvent_ProtonPhi[k+bufferPointer][j];

        }

        nParticle++ ;

        phibin = getPhiBin(phi,nPhiBin-1);
        if(phibin > (nPhiBin-1) || phibin < 1) {cout << "check3" << endl; continue;}
        hmphi->Fill(phi);
        num_mixed[phibin]++;


    }

    hMultM->Fill(nParticle);

    if(nParticle != nmax) cout << "Tragedy!!" << endl;

    for(int num = 1; num < nPhiBin; num++){

        pratio = (double)num_mixed[num]/nParticle;
        hmixed[num]->Fill(pratio);
        hmixed[0]->Fill(pratio);

        for(int k = 0; k < 10; k++){
            hMixed->Fill(k,pow(pratio,k));
        }
    }

}

void copyCurrentToBuffer(int bufferPointer){

    if(BufferEvent_NEvents[bufferPointer] >= 250) BufferEvent_Full[bufferPointer] = 1;

    TRandom3 *gRandom = new TRandom3(iran++);
    int eventPointer = -1;

    if(BufferEvent_Full[bufferPointer]){ // full - random rewrite one

        do{ double rrr = gRandom->Rndm();
            eventPointer = (int)(250.*(1.0 - rrr));
        } while(eventPointer<0||eventPointer>=250);
    }
    else { // not full
        eventPointer = BufferEvent_NEvents[bufferPointer];
    }
    delete gRandom;

    //BufferEvent_Psi[bufferPointer+100*eventPointer] = CurrentEvent_Psi;

    BufferEvent_nProton[bufferPointer+eventPointer] = CurrentEvent_nProton;

    for(int i = 0; i < CurrentEvent_nProton; i++){

        BufferEvent_ProtonPhi[bufferPointer+eventPointer][i]=CurrentEvent_ProtonPhi[i];

    }

    if(BufferEvent_NEvents[bufferPointer]<250) {
        BufferEvent_NEvents[bufferPointer]+=1;
    }
}

void DeclareHistograms(){
    hTrue = new TH1F("hTrue","true particle multiplicity  distribution ",250,0,250);
    hMult = new TH1F("hMult","total multiplicity  distribution ",250,0,250);
    hMultM = new TH1F("hMultM","mixed multiplicity  distribution ",250,0,250);

    hPileup = new TH1F("hPileup","pileup multiplicity  distribution ",50,0,50);
    hReject = new TH1F("hReject","rejection distribution ",250,0,250);

    hEfficiency = new TH1F("hEfficiency", "efficiency of particle detection", 200, 0, 1);

    hphi  = new TH1F("hphi","phi distribution",100,-10,10);
    hmphi  = new TH1F("hmphi","phi distribution for mixed",100,-10,10);
    hbin = new TH1F("hbin","phi bin distribution ",10,0,10);

    for(int i = 0; i < nPhiBin; i++){
        sprintf(name,"hratio_Phi_%d",i);
        hratio[i] = new TH1F(name,"",24,-0.05,1.15);

        sprintf(name,"hmixed_Phi_%d",i);
        hmixed[i] = new TH1F(name,"",24,-0.05,1.15);
    }


    for(int i = 0; i <= 12; i++){
        sprintf(name,"hp_%d",i);
        hp[i] = new TProfile(name,"",100,0,100);
    }

    hParticle = new TProfile("hParticle","moments of ratio (particle)",20,-0.5,19.5);
    hMixed = new TProfile("hMixed","moments of ratio (mixed)",20,-0.5,19.5);
}

