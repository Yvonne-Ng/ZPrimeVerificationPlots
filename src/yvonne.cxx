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

void doPlots(string expression, int nbin, float bin_low, float bin_hi, string outputName, string title="", bool logY=false);

void doPlots(string expression, int nbin, float bin_low, float bin_hi, string outputName, string title, bool logY){

    vector<string> rootfiles{"mR030_pcut100_evnt10000.root","mR040_pcut100_evnt10000.root","mR050_pcut100_evnt10000.root","mR060_pcut100_evnt10000.root","mR080_pcut100_evnt10000.root","mR100_pcut100_evnt10000.root","mR130_pcut100_evnt10000.root","mR160_pcut100_evnt10000.root","mR190_pcut100_evnt10000.root","mR220_pcut100_evnt10000.root"};//9
    vector<int> mR{30,40,50,60,80,100,130,160,190,220};
    vector<int> colors{1,2,3,4,5,6,7,8,9,11};

    //OUTPUT files
    //TFile *output_file = new TFile(outputName.c_str(),"Recreate");

    TLegend *tl = new TLegend(0.7,0.55,0.87,0.87);
    tl->SetFillColor(0);
    tl->SetBorderSize(0);

    TCanvas* canvas = new TCanvas((outputName+"_canvas").c_str(), "", 800, 600);
    canvas->cd();

    bool first = true;
    for (int j=0; j<rootfiles.size(); j++){
        TFile* file = TFile::Open(rootfiles.at(j).c_str());
        TTree* tree = (TTree*)file->Get("truth");
        int nEntries = tree->GetEntries();

        string histName=outputName+"_mR"+to_string(mR.at(j));//e.g: fjet_pt_mR30

        string Drawing=expression+">>"+histName + "(" + to_string(nbin) + "," + to_string(bin_low) + "," + to_string(bin_hi) + ")";
        cout << "Drawing expression: '" << Drawing << "'" << endl;
        tree->Draw(Drawing.c_str(),"","goff");

        cout << "grab hist name: " << histName << endl;
        TH1F* h = (TH1F*) gROOT->FindObject(histName.c_str());
        if (h == nullptr || h->Integral() == 0) {
            cout << "Error! Couldn't find histogram." << endl;
            continue;
        }

        //h->Scale(1./h->Integral());
        h->Scale(1./nEntries);

        h->SetLineColor(colors.at(j));

        if (first){
            h->Draw();
            h->SetTitle(title.c_str());
            ((TAxis*)h->GetYaxis())->SetTitleOffset(1.25);
            first = false;
        }
        else {
            h->Draw("Same");
        }

        tl->AddEntry(h,("mR"+to_string(mR.at(j))).c_str(),"LP");
    }
    
    tl->Draw();

    canvas->SetLogy(logY);

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

    int nBinMass=80;
    float xMinMass=0;
    float xMaxMass=250;

    int nBinPt=30;
    float xMinPt=250;
    float xMaxPt=900;

    doPlots("fjet_m[0]/1e3", nBinMass, xMinMass, xMaxMass, "fjet_m", ";large-R jet mass [GeV];");
    doPlots("fjet_pt[0]/1e3", nBinPt, xMinPt, xMaxPt, "fjet_pt", ";large-R jet p_{T} [GeV]", true);
 //   doPlots("jet_pt", 50, 0, 250e3,"test.root");
 //   doPlots("jet_m", 50, 0, 250e3, "test.root");
    doPlots("fjetTau_m[0]/1e3", nBinMass, xMinMass, xMaxMass, "fjetTau_m", ";large-R jet mass [GeV]");
    doPlots("fjetTau_pt[0]/1e3", 50, 0, 1.2e3, "fjetTau_pt","",false);
    doPlots("fjetTau_pt[0]/1e3", 50, 0, 1.2e3, "fjetTau_pt_log","",true);
    doPlots("jetNOleading_pt[0]", nBinPt, 0, xMaxPt,"jetNOleading_pt");
    doPlots("jetNOleading_m[0]", 50, 0, 250e3, "jetNOleading_m");
    doPlots("jetNOleading_pt[0]",nBinPt,0,xMaxPt, "jetNOleading_pt");
    doPlots("jetNOtau_pt[0]",nBinPt,0,xMaxPt,"jetNOtau_pt");
    doPlots("jetNOtau_m[0]",50,0,250e3,"jetNOtau_m");
}

