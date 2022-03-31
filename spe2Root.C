/*
 * Here we will take the .Spe gamma spec files and convert into a .root file.
 * I will also just plot the graph so we can see what is being saved.
 * A. Clark 03.18.2022
 */

//This code will be run by
//      root -l spe2Root.C
// or if already in a root session:
// .X spe2Root.C
///////////////////////////////////////////////
#include <stdio.h>
#include <iostream>
#include <fstream>

void spe2Root(){
//Enter the name of the file into this line
	char filename[50] = "507_sampleore_MiVitaMine_Utah_uraninite";
  char str[80]; //will be used for using the name of the files


//Call load of a separate Plot formating file
	gROOT->ProcessLine(".L FormatScanPlots.C"); //load function for formatting each plot

//Use the file name above to call the .Spe file
  	sprintf(str,"%s.Spe", filename); // takes filename and adds .Spe to the end
cout << "Opening file ... " << str << endl;
	 		ifstream fSpec;
			fSpec.open(str); //opens file


	 sprintf(str,"%s.root", filename); // takes filename and adds .Spe to the end
	 TFile *fRoot = new TFile(str, "RECREATE");
		cout << "Creating file ... " << str << endl;
//Test that the root file was properly make, if not you will get an error message
	if(!fRoot){
		cout << "Root file not Generated!" << endl;
		exit(1);
	}

//Define variables that will be needed for the code
 	int n = 8192; //total number of channels
  double bin[8192]; //x-> Channel but can convert to Energy
	double counts[8192]; ////y->Counts in channel

	double livetime =0;
	double realtime =0;
	string line; //line will be used to pars past junk lines in the file

//Here we will skip the first 9 lines since we do not need them.
for(int i=0; i<9; i++){
	getline(fSpec, line);
	cout << line <<endl; //output the lines we grabbed to screen to check that we counted right
}

//Grab the times from the file and compute the deadtime.
	fSpec >> livetime >> realtime;
	cout << "Live Time was = " << livetime << "s" << endl;
	cout << "Real Time was = " << realtime << "s" << endl;
	cout << "The deadtime was = " << (1.0-(livetime/realtime))*100 << "%." << endl;

//Skip 3 more lines to get to the data
for(int i=0; i<3; i++){
	getline(fSpec, line);
		cout << line << endl;
}

//Generate Canvas
  TCanvas *c1 = new TCanvas("c1", "Cool Plots",800, 800);

//Grab the data and store in arrays
for(int j=0; j<n; j++){
	bin[j] = j;
	fSpec >> counts[j];
}
//Generate TGraph with the data pulled into the arrays
	TGraph *h1 = new TGraph(n,bin,counts);
		h1->Draw("AL");

//The following will Write all what we need into the .root file
//So we save the realtime, livetime, and TGraph
TObject live_time;
live_time.SetUniqueID(livetime);
live_time.Write("livetime");
TObject real_time;
real_time.SetUniqueID(realtime);
real_time.Write("realtime");

h1->Write("tgraph");
fRoot->Write();

}
