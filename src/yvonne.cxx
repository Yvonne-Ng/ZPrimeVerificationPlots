#include <string>
#include <vector>
#include <math.h>
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TRoot.h"
#include "TStyle.h"
#include <iostream>
#include "TCanvas.h"
#include "TLegend.h"

using namespace std;

void doPlots(std::string expression, int nbin, float bin_low, float bin_hi, std::string outputName, std::string title="");

void doPlots(std::string expression, int nbin, float bin_low, float bin_hi, std::string outputName, std::string title){
    vector<string> rootfiles{"mR030_pcut100_evnt10000.root","mR040_pcut100_evnt10000.root","mR050_pcut100_evnt10000.root","mR060_pcut100_evnt10000.root","mR080_pcut100_evnt10000.root","mR100_pcut100_evnt10000.root","mR130_pcut100_evnt10000.root","mR160_pcut100_evnt10000.root","mR190_pcut100_evnt10000.root","mR220_pcut100_evnt10000.root"};//9
    vector<int> mR{30,40,50,60,80,100,130,160,190,220};

   //Declaring Variable.size()*rootfile.size()# of histograms 
    vector<TH1F*> histograms;

   //OUTPUT files
    TFile *output_file = new TFile(outputName.c_str(),"Recreate");

   //Declaring and opening the INPUT rootfiles as TFile objects
    vector<TFile*> File;
    vector<TTree*> Tree;
    int histcount=-1;
      
      TLegend *tl = new TLegend(0.7,0.55,0.87,0.87);
      tl->SetFillColor(0);
      tl->SetBorderSize(0);

      bool first = true;
    for (int j=0; j<rootfiles.size(); j++){
           cout<<"Rootfiles.at(j)"<<rootfiles.at(j)<<endl;
            File.push_back(TFile::Open((rootfiles.at(j)).c_str()));
            Tree.push_back((TTree*) (File.at(j)->Get("truth")) );

            string HistName=expression+"_mR"+to_string(mR.at(j));//e.g: fjet_pt_mR30

            string Drawing=expression+">>"+HistName + "(" + to_string(nbin) + "," + to_string(bin_low) + "," + to_string(bin_hi) + ")";
            cout << "Drawing expression: '" << Drawing << "'" << endl;
            Tree.at(j)->Draw(Drawing.c_str(),"","goff");

            cout << "grab hist name: " << HistName << endl;
            TH1F* h = (TH1F*) gROOT->FindObject(HistName.c_str());
            if (h == nullptr) {
                cout << "Error! Couldn't find histogram." << endl;
                continue;
            }

            histograms.push_back(h);
            h->SetLineColor(j+1);
            if (first){
                first = false;
                h->Draw();
                h->SetTitle(title.c_str());
                ((TAxis*)h->GetYaxis())->SetTitleOffset(1.25);
            }
            else {
                h->Draw("Same");
            }
            tl->AddEntry(h,("mR"+to_string(mR.at(j))).c_str(),"LP");
           
    
    }
    
    TCanvas* canvas = (TCanvas*)gROOT->FindObject("c1");
    tl->Draw();

    canvas->Update();
    canvas->SaveAs((outputName+"foo.pdf").c_str());
    canvas->SaveAs((outputName+"_canvas.root").c_str());

    /*
//c.at(i)->SaveAs((variables.at(i)+".pdf").c_str());

cout<<"TFile_pushback: "<<TFile_pushback<<endl;
cout<<"TTree_pushback: "<<TFile_pushback<<endl;
cout<<"hist_pushback: "<<hist_pushback<<endl;
cout<<"canvas_pushback: "<<canvas_pushback<<endl;
cout<<endl;
cout<<"TFile_Access: "<<TFile_Access<<endl;
cout<<"TTree_Access: "<<TTree_Access<<endl;
cout<<"hist_Access: "<<hist_Access<<endl;
cout<<"canvas_Access: "<<canvas_Access<<endl;


output_file->cd();
histcount=0;
for (int i=0; i< variables.size(); i++){
        for (int j=0; j<rootfiles.size();j++){
                hist.at(histcount)->Write();
                histcount++;
        }

}
output_file->Write();
output_file->Close();
*/

}


void plotVariables(void) {
    gStyle->SetOptStat(0);

    int nBinPt=90;
    int xMaxPt=900e3;

    doPlots("fjet_m[0]/1e3", 50, 0, 250, "fjet_m", ";large-R jet mass [GeV];y-axis label");
    doPlots("fjet_pt[0]", nBinPt, 0, xMaxPt, "fjet_pt");
 //   doPlots("jet_pt", 50, 0, 250e3,"test.root");
 //   doPlots("jet_m", 50, 0, 250e3, "test.root");
    doPlots("fjetTau_m[0]", 50, 0, 250e3, "fjetTau_m");
    doPlots("fjetTau_pt[0]", nBinPt, 0, xMaxPt, "fjetTau_pt");
    doPlots("jetNOleading_pt[0]", nBinPt, 0, xMaxPt,"jetNOleading_pt");
    doPlots("jetNOleading_m[0]", 50, 0, 250e3, "jetNOleading_m");
    doPlots("jetNOleading_pt[0]",nBinPt,0,xMaxPt, "jetNOleading_pt");
    doPlots("jetNOtau_pt[0]",nBinPt,0,xMaxPt,"jetNOtau_pt");
    doPlots("jetNOtau_m[0]",50,0,250e3,"jetNOtau_m");
}

