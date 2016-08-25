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
#include "TLine.h"

using namespace std;

void doPlots(string expression, int nbin, float bin_low, float bin_hi, string outputName, string title="", bool logY=false);
void doCutPlots(string expression, string ptCutExpression, string outputName,double plusminus=5.);
void doPlotsforFWHM(void);

void doPlotsforFWHM(void){
 vector<string> rootfiles{"mR030_pcut100_evnt10000.root","mR040_pcut100_evnt10000.root","mR050_pcut100_evnt10000.root","mR060_pcut100_evnt10000.root","mR080_pcut100_evnt10000.root","mR100_pcut100_evnt10000.root","mR130_pcut100_evnt10000.root","mR160_pcut100_evnt10000.root","mR190_pcut100_evnt10000.root","mR220_pcut100_evnt10000.root"};//9
    vector<int> mR{30,40,50,60,80,100,130,160,190,220};


for (int j=0; j<rootfiles.size(); j++){
     TFile* file = TFile::Open(rootfiles.at(j).c_str());
     TTree* tree = (TTree*)file->Get("truth");
     int nEntries = tree->GetEntries();

     string histName="mR"+to_string(mR.at(j));//e.g: fjet_pt_mR30

     double bin_low=mR.at(j)-30;
     double bin_hi=mR.at(j)+30;
     double nbin=80;

    // string Drawing="fjetTau_m[0]>>"+histName;
     
string Drawing="fjetTau_m[0]/1e3>>"+histName + "(" + to_string(nbin) + "," + to_string(bin_low) + "," + to_string(bin_hi) + ")";

     string CavasName="C"+rootfiles.at(j);
     string FileName="F"+rootfiles.at(j)+"pdf";

     tree->Draw(Drawing.c_str(),"","goff");

     //Declare canvas draw histogram
     TCanvas* canvas = new TCanvas((histName+"_canvas").c_str(), "", 800, 600);
     canvas->cd();
     TH1F* h = (TH1F*) gROOT->FindObject(histName.c_str());
   
  //   h->Scale(1./nEntries);
     h->Draw();
     //Declare a line and draw it.
     double max= h->GetMaximum();
     TLine *Line= new TLine(0,max/2, 270, max/2);
     Line->Draw();

//Save as a pdf
    canvas->Update();
     canvas->SaveAs((rootfiles.at(j)+"mass.pdf").c_str());
}  
}
    

void doCutPlots(string expression, string ptCutExpression, string outputName,double plusminus){
    vector<string> rootfiles{"mR030_pcut100_evnt10000.root","mR040_pcut100_evnt10000.root","mR050_pcut100_evnt10000.root","mR060_pcut100_evnt10000.root","mR080_pcut100_evnt10000.root","mR100_pcut100_evnt10000.root","mR130_pcut100_evnt10000.root","mR160_pcut100_evnt10000.root","mR190_pcut100_evnt10000.root","mR220_pcut100_evnt10000.root"};//9
    vector<int> mR{30,40,50,60,80,100,130,160,190,220};
 //   vector<int> pmLower{29,38,48,58,78,95,122,151,179,206};//Method 1
 //   vector<double> pmUpper{32,42,51,62,82,102,132,162,193,222};//Method1
    vector<int> pmLower{29,38,47,58,75,93,120,147,174,194};
    vector<int> pmUpper{31,42,52,62,82,102,132,162,192,223};
    
cout<<"mR[GeV]      CutPT Eff        CutPlusMinusMass Eff        Total Eff"<<endl;
    for (int j=0; j<rootfiles.size(); j++){
        TFile* file = TFile::Open(rootfiles.at(j).c_str());
        TTree* tree = (TTree*)file->Get("truth");
        int nEntries = tree->GetEntries();

        string histName=outputName+"_mR"+to_string(mR.at(j));//e.g: fjet_pt_mR30
        string histNameCut=outputName+"_mR"+to_string(mR.at(j))+"Cut";
        string histNameCutPt=outputName+"_mR"+to_string(mR.at(j))+"CutPt";
        string histNameCutPlusMinus=outputName+"_mR"+to_string(mR.at(j))+"CutPlusMinus";

        string Drawing=expression+">>"+histName;
        string DrawingCut=expression+">>"+histNameCut;
        string DrawingCutPt=expression+">>"+histNameCutPt;
        string DrawingCutPlusMinus=expression+">>"+histNameCutPlusMinus;

        string CutOption="("+expression+">"+to_string(pmLower.at(j))+")&&("+expression+"<"+to_string(pmUpper.at(j))+")&&("+ptCutExpression+")";
        string CutOptionPt="("+ptCutExpression+")";
        string CutOptionPlusMinus="("+expression+">"+to_string((mR.at(j)-plusminus))+")&&("+expression+"<"+to_string((mR.at(j)+plusminus))+")";

/*
        cout << "Drawing expression: '" << Drawing << "'" << endl;
        cout<<"Drawing cut expression '"<<DrawingCut<<"'"<<endl;
        cout<<"the Cut option: '"<<CutOption<<"'"<<endl;
        cout<<"---"<<endl;
        cout<<"Drawing cut PT expression '" <<DrawingCutPt<<"'"<<endl;
        cout<<"The CutOption PT: '"<<CutOptionPt<<"'"<<endl;
        cout<<"---"<<endl;
        cout<<"Drawing cut Plus Minus Expression '"<<DrawingCutPlusMinus<<"'"<<endl;
        cout<<"The cut option plus minus expression '"<<CutOptionPlusMinus<<"'"<<endl;
*/

        tree->Draw(Drawing.c_str(),"","goff");
        tree->Draw(DrawingCut.c_str(),CutOption.c_str(),"goff");
        tree->Draw(DrawingCutPt.c_str(),CutOptionPt.c_str(),"goff");
        tree->Draw(DrawingCutPlusMinus.c_str(),CutOptionPlusMinus.c_str(),"goff");

    //    cout << "grab hist name: " << histName << endl;
        TH1F* h = (TH1F*) gROOT->FindObject(histName.c_str());

     //   cout<<"grab hist cut name: "<<histNameCut<<endl;
        TH1F* hCut= (TH1F*) gROOT->FindObject(histNameCut.c_str());

       // cout<<"Grab hist cut pt name: "<<histNameCutPt<<endl;
        TH1F* hCutPt= (TH1F*) gROOT->FindObject(histNameCutPt.c_str());

     //   cout<<"Grab hist cut plus minus name: "<<histNameCutPlusMinus<<endl;
        TH1F* hCutPlusMinus=(TH1F*) gROOT->FindObject(histNameCutPlusMinus.c_str());

        if (h == nullptr || h->Integral() == 0) {
            cout << "Error! Couldn't find histogram." << endl;
            continue;
        }

        if (hCut==nullptr || hCut->Integral()==0){
            cout<<"Error! Couldn't find the cut histogram!"<<endl;
        }

        if (hCutPt==nullptr || hCut->Integral()==0){
            cout<<"Error! Couldn't find the cut PT histogram!"<<endl;
        }

        if (hCutPlusMinus==nullptr || hCut->Integral()==0){
            cout<<"Error! Couldn't find the cut plus minus histogram!"<<endl;
        }

        double TotalEntries=(double)h->GetEntries();
        double CutEntries=(double)hCut->GetEntries();
        double CutPtEntries=(double)hCutPt->GetEntries();
        double CutPlusMinusEntries=(double) hCutPlusMinus->GetEntries();

    //    cout<<"Res Mass: "<<to_string(mR.at(j))<<" Total Entries: "<<TotalEntries<<" Cut Entries: "<<CutEntries<<" Cut PT Entries: "<<CutPtEntries<<" Cut Plus Minus Entries: "<<CutPlusMinusEntries<<endl;
    //        cout<<" Res Mass: "<<to_string(mR.at(j))<<" PT Cut eff: "<<CutPtEntries/TotalEntries<<" Mass plus minus Efficiency: "<<CutPlusMinusEntries/TotalEntries<< " Total Efficiency: "<<CutEntries/TotalEntries<<endl;

            cout<<to_string(mR.at(j))<<"  "<<CutPtEntries/TotalEntries<<"  "<<CutPlusMinusEntries/TotalEntries<<"  "<<CutEntries/TotalEntries<<endl;
    }


}
void doPlots(string expression, int nbin, float bin_low, float bin_hi, string outputName, string title, bool logY){

    vector<string> rootfiles{"mR030_pcut100_evnt10000.root","mR040_pcut100_evnt10000.root","mR050_pcut100_evnt10000.root","mR060_pcut100_evnt10000.root","mR080_pcut100_evnt10000.root","mR100_pcut100_evnt10000.root","mR130_pcut100_evnt10000.root","mR160_pcut100_evnt10000.root","mR190_pcut100_evnt10000.root","mR220_pcut100_evnt10000.root"};//9
    vector<int> mR{30,40,50,60,80,100,130,160,190,220};
    vector<int> colors{1,2,3,4,5,6,7,8,9,11};

    //OUTPUT files
    //TFile *output_file = new TFile(outputName.c_str(),"Recreate");

    TLegend *tl = new TLegend(0.7,0.55,0.87,0.87);
    tl->SetFillColor(0);
    tl->SetBorderSize(0);
    tl->SetTextSize(0.03);

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
    float xMaxPt=1800;

    doPlots("fjet_m[0]/1e3", nBinMass, xMinMass, xMaxMass, "fjet_m", ";large-R jet mass [GeV];");
    doPlots("fjet_pt[0]/1e3", nBinPt, xMinPt, xMaxPt, "fjet_pt", ";large-R jet p_{T} [GeV]", false);
    doPlots("fjet_pt[0]/1e3", nBinPt, xMinPt, xMaxPt, "fjet_pt_log", ";large-R jet p_{T} [GeV]", true);
 //   doPlots("jet_pt", 50, 0, 250e3,"test.root");
 //   doPlots("jet_m", 50, 0, 250e3, "test.root");
    doPlots("fjetTau_m[0]/1e3", nBinMass, xMinMass, xMaxMass, "fjetTau_m", ";large-R jet mass [GeV]");
    doPlots("fjetTau_pt[0]/1e3", 50, 0, 1.8e3, "fjetTau_pt",";large-R jet p_{T} [GeV]",false);
    doPlots("fjetTau_pt[0]/1e3", 50, 0, 1.8e3, "fjetTau_pt_log",";large-R jet p_{T} [GeV]",true);
    doPlots("jetNOleading_pt[0]/1e3", nBinPt, 0, xMaxPt,"jetNOleading_pt",";probe jet p_{T} [GeV]",false);
    doPlots("jetNOleading_pt[0]/1e3", nBinPt, 0, xMaxPt,"jetNOleading_pt_log",";probe jet p_{T} [GeV]",true);
    doPlots("jetNOleading_m[0]/1e3", 50, 0, 250, "jetNOleading_m_log",";probe jet mass [GeV]",true);

    doPlots("jetNOleading_m[0]/1e3", 50, 0, 250, "jetNOleading_m",";probe jet mass [GeV]",false);
    doPlots("jetNOleading_pt[0]/1e3",nBinPt,0,xMaxPt, "jetNOleading_pt", ";probe jet p_{T}[GeV]",false);
    doPlots("jetNOleading_pt[0]/1e3",nBinPt,0,xMaxPt, "jetNOleading_pt_log", ";probe jet p_{T}[GeV]",true);
    doPlots("jetNOtau_pt[0]/1e3",nBinPt,0,xMaxPt,"jetNOtau_pt",";probe jet p_{T}[GeV]",false);
    doPlots("jetNOtau_pt[0]/1e3",nBinPt,0,xMaxPt,"jetNOtau_pt_log",";probe jet p_{T}[GeV]",true);
    doPlots("jetNOtau_m[0]/1e3",50,0,250,"jetNOtau_m",";probe jet mass [GeV]",false);
    doPlots("jetNOtau_m[0]/1e3",50,0,250,"jetNOtau_m_log",";probe jet mass [GeV]",true);

  //  */

    doCutPlots("fjet_m[0]/1e3", "jetNOleading_pt[0]/1e3>400","fjet_m_cut");

    doCutPlots("fjetTau_m[0]/1e3", "jetNOtau_pt[0]/1e3>400","fjettau_m_cut");


    doPlotsforFWHM();
}

