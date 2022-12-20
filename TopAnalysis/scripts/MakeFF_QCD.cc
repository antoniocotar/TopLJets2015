// run root -l -q -b MakeFF_QCD.cc

//ROOT includes
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TPaveText.h"
#include "TChain.h"
#include "TH2F.h"
#include "TCut.h"
#include "TH1F.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TFile.h"

//c++ includes
#include <iostream>

using namespace std;

void MakeFF_QCD(double Lumi = 29380.666084){

gROOT->ProcessLine(".x ~/rootlogon.C");
 


bool LogX=1;

TString fname=Form("GetFF_QCD");

TCut cut_year = "1"; 


//double Lumi = 29380.666084;	// luminosity in pb-1

TString weight_bkg = "weight";

gStyle->SetOptStat(0);
TString Atlas_label="Internal";
TString extra_suf = "";	// this one for Function names: GetTauFF2015_2016[extra_suf]
TPaveText *lumimc = new TPaveText(0.140228,0.80,0.556476,0.89,"NDC");
lumimc->SetFillColor(0); lumimc->SetLineColor(0); lumimc->SetFillStyle(0);


lumimc->AddText(Form("#sqrt{s}=13TeV, %1.1f fb^{-1}",Lumi/1000.0));
lumimc->SetTextFont(42); lumimc->SetTextSize(0.0499501); lumimc->SetBorderSize(0);


double m_atlasX=0.13, m_atlasY=0.89, m_atlasSize=0.06, m_ATLAS_width=2.4;
TLatex* text_ATLAS=new TLatex(); text_ATLAS->SetNDC(); text_ATLAS->SetTextFont(72); text_ATLAS->SetTextSize(m_atlasSize);

TString mc_path="/eos/user/m/mpitt/PPS_ttbar/MC/ntuples_forMatteo_QCD";
TString data_path="/eos/user/m/mpitt/PPS_ttbar/Data/ntuples_forMatteo_QCD/";

cout << "mc_path: " << mc_path << endl;
cout << "data_path: " << data_path << endl;

TChain * NOMINAL = new TChain("tree"); 
NOMINAL->Add(data_path+"/Single*_2017*_xa*.root"); cout << "Add "+data_path+"/Single*_2017*_xa*.root" << endl;


TString ds; std::vector<TString> mc; std::vector<float> xsec; TCut weight = "weight";

// ttbar sample
mc.push_back("TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8.root"); xsec.push_back(365.34);
mc.push_back("TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8.root"); xsec.push_back(88.29);

// Single top
mc.push_back("ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8.root"); xsec.push_back(35.85);
mc.push_back("ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8.root"); xsec.push_back(35.85);

// W+jets sample
mc.push_back("W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root"); xsec.push_back(2793.0);
mc.push_back("W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root"); xsec.push_back(992.5);
mc.push_back("W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root"); xsec.push_back(544.3);

//Z+jets
mc.push_back("DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root"); xsec.push_back(304.4);
mc.push_back("DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root"); xsec.push_back(111.5);
mc.push_back("DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root"); xsec.push_back(44.03);


//mc.clear();

const int bin_type = 2;	// electron or muon

// bin ws pt in each region
TH1F * FF[bin_type]; // matrix of histograms contain FF(pt) for different bins of type
TH1F * FFd[bin_type]; // matrix of histograms contain all loose taus for different bins of type
TH1F * FF_mc[bin_type]; // matrix of histograms contain FF(pt) for different bins of type
TH1F * FFd_mc[bin_type]; // matrix of histograms contain all loose taus for different bins of type
TH1F * FF_data[bin_type]; // matrix of histograms contain FF(pt) for different bins of type
TH1F * FFd_data[bin_type]; // matrix of histograms contain all loose taus for different bins of type

int nBinsFull[bin_type]; int type;


// Define FF histograms
type=0; float binsPT1[] = { 30, 35, 40, 45, 50, 60, 75, 90, 105, 120, 140, 160, 200, 1000}; nBinsFull[type]=sizeof(binsPT1)/sizeof(binsPT1[0])-1;
type=1; float binsPT2[] = { 30, 35, 40, 45, 50, 60, 70, 80, 100, 120, 140, 180, 1000}; nBinsFull[type]=sizeof(binsPT2)/sizeof(binsPT2[0])-1;

const float *binsPT[bin_type] = {binsPT1,binsPT2};
TCut type_cuts[] = {  "cat==4","cat==5"};
TString type_cuts_Str[] = {  "cat==4","cat==5"};

for(int i=0;i<bin_type;i++){
TCut cut = type_cuts[i];
cout << endl << "cut = "; cut.Print();

// Define FF histograms
FF[i] = new TH1F(Form("FF_%d",i),";p_{T} [GeV]; Fake Factors",nBinsFull[i],binsPT[i]);
FFd[i] = new TH1F(Form("FFd_%d",i),";p_{T} [GeV]; Fake Factors",nBinsFull[i],binsPT[i]);

FF_data[i] = new TH1F(Form("FF_data_%d",i),";p_{T} [GeV]; Fake Factors",nBinsFull[i],binsPT[i]);
FFd_data[i] = new TH1F(Form("FFd_data_%d",i),";p_{T} [GeV]; Fake Factors",nBinsFull[i],binsPT[i]);
NOMINAL->Draw( Form("l_pt >> FF_data_%d",i),(cut && cut_year && "l_tight"));
NOMINAL->Draw( Form("l_pt >> FFd_data_%d",i),(cut && cut_year && "!l_tight"));
 

cout << "TOTAL = " <<FF_data[i]->Integral() << " , " <<  FFd_data[i]->Integral() << endl;

// Subtrack all prompt leptons
FF_mc[i] = new TH1F(Form("FF_mc_%d",i),";p_{T} [GeV]; Fake Factors",nBinsFull[i],binsPT[i]);
FFd_mc[i] = new TH1F(Form("FFd_mc_%d",i),";p_{T} [GeV]; Fake Factors",nBinsFull[i],binsPT[i]);

TH1F * temp=new TH1F(Form("temp_%d",i),";p_{T} [GeV]; Fake Factors",nBinsFull[i],binsPT[i]);
TH1F * tempd=new TH1F(Form("tempd_%d",i),";p_{T} [GeV]; Fake Factors",nBinsFull[i],binsPT[i]);
double total_mc_tight=0, total_mc_loose=0;

 for(unsigned int k2=0;k2<mc.size();k2++){
 ds = mc.at(k2);
 float _xsec = xsec.at(k2);
   
   TFile * file = new TFile(mc_path+"/"+ds);
   if(!file) {cout << "bad file " << ds << endl; continue;}
   if(!file->Get("evt_count")) {cout << "no \"evt_count\" in " << ds << endl; continue;}
   double norm=((TH1F*)file->Get("evt_count"))->GetBinContent(2);
   //   cout<<"norm = "<<norm<<endl;
   file->Close();

   TChain * chain = new TChain("tree");  chain->Add(mc_path+"/"+ds); 
   weight = Form("(%s*%f)",weight_bkg.Data(),_xsec*Lumi/norm);
   chain->Draw(Form("l_pt >> temp_%d",i),(cut && "l_tight")  * weight );
   chain->Draw(Form("l_pt >> tempd_%d",i),(cut && "!l_tight ")  * weight );
   
   total_mc_tight+=temp->Integral(); total_mc_loose+=tempd->Integral();
   cout << "subtract " << ds.Data() << " = " << temp->Integral() << ", " <<  tempd->Integral() << endl;
   FF_mc[i]->Add(temp);
   FFd_mc[i]->Add(tempd);
 }
cout << "subtract TOTAL = " <<total_mc_tight << " , " <<  total_mc_loose << endl;
}

cout << "Finish obtaining the histograms, prepare the plots"<<endl;
Color_t color[bin_type]={kCyan+3, kMagenta};
Int_t markers[bin_type]={20, 21};
float markers_size[bin_type]={0.1, 0.1};

for(int i=0;i<bin_type;i++){
FF[i]->SetLineColor(color[i]); FF[i]->SetLineWidth(1); FF[i]->SetFillColor(color[i]);
FF[i]->SetMarkerColor(color[i]); FF[i]->SetMarkerSize(markers_size[i]); FF[i]->SetMarkerStyle(markers[i]);
FFd[i]->SetLineColor(color[i]); FFd[i]->SetLineWidth(1);
}

cout << "Writes output file with all histos"<<endl;
TFile * out2 = new TFile("output.root","recreate");
for(int i=0;i<bin_type;i++) { FF_mc[i]->Write(); FF_data[i]->Write();}
for(int i=0;i<bin_type;i++) { FFd_mc[i]->Write(); FFd_data[i]->Write();}
out2->Write();
out2->Close();


// devide the histograms and set errors
cout << "divide the histograms " << endl;
double r,rup, rdn, dr, nd, nmc, dd, dmc;
for(int i=0;i<bin_type;i++){
for(int bin=1;bin<=nBinsFull[i];bin++){
	nd  = FF_data[i]->GetBinContent(bin);
	nmc = FF_mc[i]->GetBinContent(bin);
	dd  = FFd_data[i]->GetBinContent(bin);
	dmc = FFd_mc[i]->GetBinContent(bin);
	cout << "i="<<i<<", r = (" << nd<<"-"<<nmc <<")/("<<dd<<"-"<<dmc<<")"<< endl;
	r=(nd-nmc)/(dd-dmc);
	rup = (nd-0.5*nmc)/(dd-0.5*dmc);
	rdn = (nd-1.5*nmc)/(dd-1.5*dmc);
	dr = 0.5*(rup - rdn) ;
	cout << "bin=" << bin <<", r =  " << r << "(+/-" << rup <<","<<rdn<<")"<< endl;
	FF[i]->SetBinContent(bin,r); FF[i]->SetBinError(bin,dr);
}}

// Draw histograms
cout << "draw " << endl;
TCanvas * c = new TCanvas("c","",600,600);
TLegend* lg1 = new TLegend(0.58336,0.75,0.932982,0.935279,"");
TString legend[bin_type]={"electrons","muons"};
for(int i=0;i<bin_type;i++) lg1->AddEntry(FF[i],legend[i],"f");
lg1->SetTextFont(22); lg1->SetTextFont(42);
lg1->SetBorderSize(0);
lg1->SetMargin(0.25);
lg1->SetFillColor(kWhite);
//lg1->SetNColumns(2);
FF[0]->GetXaxis()->SetTitleOffset(1.2);
FF[0]->GetXaxis()->SetNdivisions(513);
FF[0]->GetYaxis()->SetTitleSize(0.05);
FF[0]->GetXaxis()->SetTitleSize(0.05);
FF[0]->GetYaxis()->SetRangeUser(0.5,100);
FF[0]->GetYaxis()->SetTitleOffset(1.2);
//FF[j][0]->GetYaxis()->SetRangeUser(40,1000);
FF[0]->GetXaxis()->SetMoreLogLabels(LogX); 
FF[0]->Draw("E2");  FF[1]->Draw("E2same"); lumimc->Draw();
//FF[0]->Draw("Lsame");  FF[1]->Draw("Lsame"); 
text_ATLAS->DrawLatex(m_atlasX,m_atlasY,"CMS"); text_ATLAS->SetTextFont(42); text_ATLAS->DrawLatex(m_atlasX+m_ATLAS_width*m_atlasSize,m_atlasY,Atlas_label);  lg1->Draw(); 
c->SetTickx();
c->SetTicky();
c->SetLogx(LogX);
c->SetLogy();
c->Modified();


c->SaveAs(Form("Plots/root/%s.root",(fname).Data()));
TFile * file_c = TFile::Open(Form("Plots/root/%s.root",(fname).Data()),"update");
for(int i=0;i<bin_type;i++) FF[i]->Write();
file_c->Write();
file_c->Close();

c->SaveAs(Form("Plots/png/%s.png",(fname).Data()));
c->SaveAs(Form("Plots/eps/%s.eps",(fname).Data()));

// writes weight file
FILE * fp;
fp = fopen (Form("%s.C",(fname).Data()), "w+");
cout<< "Writes "<<(fname).Data()<<".C"<<endl;
fprintf(fp,"#include <iostream>\n");
fprintf(fp,"using namespace std;\n");
fprintf(fp,"float %s%s(float pt, int cat){\n",fname.Data(),extra_suf.Data());
for(int i=0;i<bin_type;i++){
if(i) fprintf(fp,"else if(%s){\n", type_cuts_Str[i].Data());
else fprintf(fp,"if(%s){\n", type_cuts_Str[i].Data());
for(int bin=1;bin<=nBinsFull[i];bin++){
  if(bin==1)
  fprintf(fp,"\t\tif(pt<%3.3f) return %6.6f;\n", FF[i]->GetBinCenter(bin)+0.5*FF[i]->GetBinWidth(bin) ,FF[i]->GetBinContent(bin));
  else if (bin==nBinsFull[i])
  fprintf(fp,"\t\telse return %6.6f;\n",FF[i]->GetBinContent(bin));
  else 
  fprintf(fp,"\t\telse if(pt<%3.3f) return %6.6f;\n", FF[i]->GetBinCenter(bin)+0.5*FF[i]->GetBinWidth(bin) ,FF[i]->GetBinContent(bin));
} // end pt bins
fprintf(fp,"\t\t}\n");
} // end if pantau
fprintf(fp,"else {cout << \"something wrong\" << endl; return 0;}\n");
fprintf(fp,"}\n"); 
fprintf(fp,"\n");

// writes weight file sys up
fprintf(fp,"float %s%s_up(float pt, int cat){\n",fname.Data(),extra_suf.Data());
for(int i=0;i<bin_type;i++){
if(i) fprintf(fp,"else if(%s){\n", type_cuts_Str[i].Data());
else fprintf(fp,"if(%s){\n", type_cuts_Str[i].Data());
for(int bin=1;bin<=nBinsFull[i];bin++){
  if(bin==1)
  fprintf(fp,"\t\tif(pt<%3.3f) return %6.6f;\n", FF[i]->GetBinCenter(bin)+0.5*FF[i]->GetBinWidth(bin) ,FF[i]->GetBinContent(bin)+FF[i]->GetBinError(bin));
  else if (bin==nBinsFull[i])
  fprintf(fp,"\t\telse return %6.6f;\n",FF[i]->GetBinContent(bin)+FF[i]->GetBinError(bin));
  else 
  fprintf(fp,"\t\telse if(pt<%3.3f) return %6.6f;\n", FF[i]->GetBinCenter(bin)+0.5*FF[i]->GetBinWidth(bin) ,FF[i]->GetBinContent(bin)+FF[i]->GetBinError(bin));
} // end pt bins
fprintf(fp,"\t}\n");
} // end if pantau
fprintf(fp,"else {cout << \"something wrong\" << endl; return 0;}\n");
fprintf(fp,"}\n"); 
fprintf(fp,"\n");

// writes weight file sys down
fprintf(fp,"float %s%s_dn(float pt, int cat){\n",fname.Data(),extra_suf.Data());
for(int i=0;i<bin_type;i++){
if(i) fprintf(fp,"else if(%s){\n", type_cuts_Str[i].Data());
else fprintf(fp,"if(%s){\n", type_cuts_Str[i].Data());
for(int bin=1;bin<=nBinsFull[i];bin++){
  if(bin==1)
  fprintf(fp,"\t\tif(pt<%3.3f) return %6.6f;\n", FF[i]->GetBinCenter(bin)+0.5*FF[i]->GetBinWidth(bin) ,FF[i]->GetBinContent(bin)-FF[i]->GetBinError(bin));
  else if (bin==nBinsFull[i])
  fprintf(fp,"\t\telse return %6.6f;\n",FF[i]->GetBinContent(bin)-FF[i]->GetBinError(bin));
  else 
  fprintf(fp,"\t\telse if(pt<%3.3f) return %6.6f;\n", FF[i]->GetBinCenter(bin)+0.5*FF[i]->GetBinWidth(bin) ,FF[i]->GetBinContent(bin)-FF[i]->GetBinError(bin));
} // end pt bins
fprintf(fp,"\t}\n");
} // end if pantau
fprintf(fp,"else {cout << \"something wrong\" << endl; return 0;}\n");
fprintf(fp,"}\n"); 
fprintf(fp,"\n");

// Close the file
fclose(fp);
cout << "Compile the files..."<<endl;
gROOT->ProcessLine(Form(".L %s.C++",(fname).Data()));

}
