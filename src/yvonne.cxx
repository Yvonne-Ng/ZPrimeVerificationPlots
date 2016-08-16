#include <string>
#include <vector>
#include <math.h>
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include <iostream>
#include "TCanvas.h"

using namespace std;

void plotVariables(void){

   int nBinMass=90;
   int xMinMass=0;
   int xMaxMass=450000;

   int nBinPt=90;    
   int xMinPt=0;
   int xMaxPt=1800000;

   //Initialize the input variable and root file names and mR masses
    vector<string> variables{"fjet_pt","fjet_m","jetNOleading_pt","jetNOleading_m","fjetTau_pt","fjet_m","jetNOtau_pt","jetNOtau_m"};
    vector<string> rootfiles{"mR030_pcut100_evnt10000.root","mR040_pcut100_evnt10000.root","mR050_pcut100_evnt10000.root","mR060_pcut100_evnt10000.root","mR080_pcut100_evnt10000.root","mR0100_pcut100_evnt10000.root","mR0130_pcut100_evnt10000.root","mR0160_pcut100_evnt10000.root","mR0190_pcut100_evnt10000.root","mR0220_pcut100_evnt10000.root"};
    vector<int> mR{30,40,50,60,80,100,130,160,190,220};

   //Declaring rootfile size of Canvas
    vector<TCanvas*> c;

   //Declaring Variable.size()*rootfile.size()# of histograms 
    vector<TH1F*> hist;

   //String names of OUTPUT file 
    string output_filename="mR30-mR200_pcut100.root";
        
   //OUTPUT files
    TFile *output_file = new TFile(output_filename.c_str(),"Recreate");


   //Declaring and opening the INPUT rootfiles as TFile objects
    vector<TFile*> File;
    vector<TTree*> Tree;

    for (int i=0; i<rootfiles.size(); i++){
        File.push_back(TFile::Open((rootfiles.at(i)).c_str()));
        Tree.push_back((TTree*)File.at(i)->Get("truth"));
        string CanvasName="c"+to_string(i);
        c.push_back(new TCanvas(CanvasName.c_str(),"Graph",200,100,700,500));

        for (int j=0; j<variables.size(); j++){
                   
                string HistName=variables.at(j)+"_mR"+to_string(mR.at(j));//e.g: fjet_pt_mR30
                if (j%2==0){
                        hist.push_back(new TH1F(HistName.c_str(),HistName.c_str(),nBinPt,xMinPt,xMaxPt));
                }
                else {
                        hist.push_back(new TH1F(HistName.c_str(),HistName.c_str(),nBinMass,xMinMass,xMaxMass));
                }
                string Drawing=variables.at(i)+">>"+HistName;
                Tree.at(i)->Draw(Drawing.c_str(),"","goff");
                hist.at(i*variables.size()+j)->Scale(1./hist.at(i*variables.size()+j)->Integral());
                c.at(i)->cd();
                hist.at(i*variables.size()+j)->Draw();
    

        }


    }





}
