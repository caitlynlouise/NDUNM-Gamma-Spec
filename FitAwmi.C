// Example to illustrate fitting function using AWMI algorithm.
// modified by Russell Leslie to illustrate GetArea issue - 2010-02-13
// To execute this example, do

// root > .x FitAwmi.C
#include "TCanvas.h"
#include "TFile.h"
#include "TROOT.h"
#include "TF1.h"
#include "TH1.h"
#include "TH1F.h"
#include "TPolyMarker.h"
#include "TSpectrum.h"
#include "TSpectrumFit.h"

void FitAwmi() {

   Double_t a;

   Int_t i,nfound=0,bin;

   Int_t xmin  = 420;

   Int_t xmax  = 540;

   Int_t nbins = xmax-xmin;

   Float_t * source = new float[nbins];

   Float_t * dest = new float[nbins];  

   TH1F *h = new TH1F("h","Fitting using AWMI algorithm",nbins,xmin,xmax);

   TH1F *d = new TH1F("d","",nbins,xmin,xmax);     

   TFile *f = new TFile("P1_se77tf1209_r04_239keV_G1.root");  // <----- TSpectrum.root replaced with example file to illustrate issue

   h=(TH1F*) f->Get("P1G1;1");  // <---- Pointer to histogram contained in example file

   for (i = 0; i < nbins; i++) source[i]=h->GetBinContent(i + 1);     

   TCanvas *Fit1 = (TCanvas*)gROOT->GetListOfCanvases()->FindObject("Fit1");

   if (!Fit1) Fit1 = new TCanvas("Fit1","Fit1",10,10,1000,700);

   h->Draw();

   TSpectrum *s = new TSpectrum();

   //searching for candidate peaks positions

   nfound = s->SearchHighRes(source, dest, nbins, 2, 0.1, kFALSE, 10000, kFALSE, 0);

   Bool_t *FixPos = new Bool_t[nfound];

   Bool_t *FixAmp = new Bool_t[nfound];     

   for(i = 0; i< nfound ; i++){

      FixPos[i] = kFALSE;

      FixAmp[i] = kFALSE;   

   }

   //filling in the initial estimates of the input parameters

   Float_t *PosX = new Float_t[nfound];        

   Float_t *PosY = new Float_t[nfound];

   PosX = s->GetPositionX();

   for (i = 0; i < nfound; i++) {

        a=PosX[i];

        bin = 1 + Int_t(a + 0.5);

        PosY[i] = h->GetBinContent(bin);

   }  

   TSpectrumFit *pfit=new TSpectrumFit(nfound);

   pfit->SetFitParameters(xmin, xmax-1, 1000, 0.1, pfit->kFitOptimChiCounts, pfit->kFitAlphaHalving, pfit->kFitPower2, pfit->kFitTaylorOrderFirst);  

   pfit->SetPeakParameters(2, kFALSE, PosX, (Bool_t *) FixPos, PosY, (Bool_t *) FixAmp);  

   pfit->FitAwmi(source);

   Double_t *CalcPositions = new Double_t[nfound];     

   Double_t *CalcAmplitudes = new Double_t[nfound];        

   Double_t *CalcArea = new Double_t[nfound];        

   Double_t *CalcAreaError = new Double_t[nfound];        

   CalcPositions=pfit->GetPositions();

   CalcAmplitudes=pfit->GetAmplitudes();  

   CalcArea[0] = (Double_t*)pfit->GetAreas();   // <-----Attempt to get the peak areas
   
   CalcAreaError[0] = (Double_t*)pfit->GetAreasErrors();    // <-----Attempt to get the peak area errors
   
   cout << CalcArea[0] << " - " << CalcAreaError[0] << endl;  // <----- Odd number results for CalcArea[0] and CalcAreaError[0] 

   
   for (i = 0; i < nbins; i++) d->SetBinContent(i + 1,source[i]);

   
   d->SetLineColor(kRed);  

   d->Draw("SAME"); 
 
   for (i = 0; i < nfound; i++) {

        a=CalcPositions[i];

        bin = 1 + Int_t(a + 0.5);               

        PosX[i] = d->GetBinCenter(bin);

        PosY[i] = d->GetBinContent(bin);

   }

   TPolyMarker * pm = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");

   if (pm) {

      h->GetListOfFunctions()->Remove(pm);

      delete pm;

   }

   pm = new TPolyMarker(nfound, PosX, PosY);

   h->GetListOfFunctions()->Add(pm);

   pm->SetMarkerStyle(23);

   pm->SetMarkerColor(kRed);

   pm->SetMarkerSize(1);  

}
