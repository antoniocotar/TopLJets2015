#include <iostream>
#include <algorithm>

/*
Code was developed for exclusive top analysis by Michael Pitt
Code creates QCD sample from data and MC folders the list of data and MC samples should be provided as a text file

Usage:
createQCDSample /eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/mc/excl_ttbar_semilep_QED_xa150_era2017_preTS2.root
*/

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TDirectory.h>
#include <TBranch.h>
#include <TMath.h>
#include "TSystem.h"

#include "TopLJets2015/TopAnalysis/data/era2017/GetFF_QCD.C"


#include <time.h>
using namespace std;

int main(int argc, char* argv[])
{
	
  // CMSSW settings  
  const char* CMSSW_BASE = getenv("CMSSW_BASE");
  TString data_path = Form("%s/src/TopLJets2015/TopAnalysis/data/era2017/", CMSSW_BASE);

  // Check arguments
  if (argc < 3 || argc > 5) {
    cout << "Missing data / MC folders " << endl
         << "Usage: " << argv[0] << " <dataFolder> <mcfolder>" << endl;
    cout << "Exampe: " << argv[0] << " /eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/data/ \
/eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/mc/" << endl;
    return 1;
  }
  
  // Check input files
  if(gSystem->AccessPathName(argv[1])){
	  cout << "ERROR! Missing path to folder with data: " << argv[1] << endl;
	  return 0;
  }
  if(gSystem->AccessPathName(argv[2])){
	  cout << "ERROR! Mising path to folder with MC: " << argv[2] << endl;
	  return 0;
  }  
  
  string DataPath = argv[1];
  string MCPath = argv[2];
  string outFileName = "QCD.root";
  
  // List of data files:
  float total_lumi = 29380.666084; // in ipb
  TString eras_str[5]={"2017B","2017C","2017D","2017E","2017F"};
  std::vector<TString> data;  
  for(int i=0;i<5;i++){
  for(int j=120;j<151;j+=10){
  data.push_back(Form("SingleElectron_%s_xa%d.root",eras_str[i].Data(),j)); 
  data.push_back(Form("SingleMuon_%s_xa%d.root",eras_str[i].Data(),j)); 
  }}
  
  // List of data files:
  std::vector<float> mc_xsec;
  std::vector<TString> mc; 

  mc.push_back("TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8.root"); mc_xsec.push_back(365.34);
  mc.push_back("TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8.root"); mc_xsec.push_back(88.29);
  mc.push_back("ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8.root"); mc_xsec.push_back(35.85);
  mc.push_back("ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8.root"); mc_xsec.push_back(35.85);
  mc.push_back("W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root"); mc_xsec.push_back(2793.0);
  mc.push_back("W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root"); mc_xsec.push_back(992.5);
  mc.push_back("W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root"); mc_xsec.push_back(544.3);
  mc.push_back("DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root"); mc_xsec.push_back(304.4);
  mc.push_back("DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root"); mc_xsec.push_back(111.5);
  mc.push_back("DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root"); mc_xsec.push_back(44.03);
   
  // read Data files
  TChain * chDataEvents = new TChain("tree");
  for(unsigned int k2=0;k2<data.size();k2++){
	  chDataEvents->Add(DataPath+"/"+data.at(k2));
  }
  float weight, l_pt, cat, nBjets, l_tight; 
  chDataEvents->SetBranchAddress("weight",&weight);
  chDataEvents->SetBranchAddress("l_pt",&l_pt);
  chDataEvents->SetBranchAddress("cat",&cat);
  chDataEvents->SetBranchAddress("nBjets",&nBjets);
  chDataEvents->SetBranchAddress("l_tight",&l_tight);
 
  int nDataEntries = chDataEvents->GetEntries();
  cout << nDataEntries << " events read from data file(s) in " << DataPath << endl;

  // Open output Data file
  TFile* fOutQCD = new TFile(Form("%s/QCD.root",MCPath.c_str()), "RECREATE");
  if(!fOutQCD->IsOpen()) {
    cout<< "Error opening output file. Abort." << endl;
    return 0;
  }

  // Create new output with updated QCD weights:
  TTree* tDataQCD = chDataEvents->CloneTree(0);
  
  float qcd_Up=1, qcd_Down=1; 
  tDataQCD->Branch("qcd_Up",&qcd_Up);
  tDataQCD->Branch("qcd_Down",&qcd_Down);
   
  int times,timed;
  times=time(NULL);  
  cout << "Loop over Data entries and apply QCD weights" << endl;
  // Loop over MC entries and mix pileup protons
  int iDataEntry;
  for (iDataEntry = 0; iDataEntry < nDataEntries; iDataEntry++) {
    if(iDataEntry%1000==0) printf ("\r [%3.0f%%] done", 100.*(float)iDataEntry/(float)nDataEntries);
    
	// asign the protons to the MC event
    chDataEvents->GetEntry(iDataEntry);
	if (nBjets<2) continue;
	if (l_tight) continue;
	
	weight *= GetFF_QCD(l_pt,cat);
	qcd_Up = GetFF_QCD_up(l_pt,cat)/GetFF_QCD(l_pt,cat) - 1;
	qcd_Down = 1 - GetFF_QCD_dn(l_pt,cat)/GetFF_QCD(l_pt,cat);

    tDataQCD->Fill();
  }

  cout << "\nDone. " << iDataEntry << " events processed." << endl;
      timed=time(NULL);
    times=timed-times;
    cout << "time from start to end = " << times << endl;
	
  // Write mixed tree to file and close everything
  fOutQCD->cd();
  cout << "Writes " << fOutQCD->GetName() << endl;
  tDataQCD->Write();
  fOutQCD->Close();
  
  
  
  // read MC files
for(unsigned int k2=0;k2<mc.size();k2++){
  TString ds=mc.at(k2);
  float _xsec = mc_xsec.at(k2);
  
  TFile *oldfile = new TFile(MCPath+"/"+ds);

  if(!oldfile) {cout << "bad file " << ds << endl; continue;}
  if(!oldfile->Get("evt_count")) {cout << "no \"evt_count\" in " << ds << endl; continue;}
  double norm=((TH1F*)oldfile->Get("evt_count"))->GetBinContent(2);

  TTree * chMCEvents = (TTree *)oldfile->Get("tree");
  
  float weight, l_pt, cat, nBjets, l_tight; 
  chMCEvents->SetBranchAddress("weight",&weight);
  chMCEvents->SetBranchAddress("l_pt",&l_pt);
  chMCEvents->SetBranchAddress("cat",&cat);
  chMCEvents->SetBranchAddress("nBjets",&nBjets);
  chMCEvents->SetBranchAddress("l_tight",&l_tight);
 
  int nMCEntries = chMCEvents->GetEntries();
  cout << nMCEntries << " events read from data file(s) in " << (MCPath+"/"+mc.at(k2)) << endl;
 
  // Open output mixed file
  ds.ReplaceAll(".root","_QCD.root");
  TFile* fOutMC = new TFile(Form("%s/%s",MCPath.c_str(),ds.Data()), "RECREATE");
  if(!fOutMC->IsOpen()) {
    cout<< "Error opening output file. Abort." << endl;
    return 0;
  }

  // Create new output with updated QCD weights:
  TTree* tMCQCD = chMCEvents->CloneTree(0);
  
  float qcd_Up=1, qcd_Down=1; 
  tMCQCD->Branch("qcd_Up",&qcd_Up);
  tMCQCD->Branch("qcd_Down",&qcd_Down);
   
  int times,timed;
  times=time(NULL);  
  cout << "Loop over MC and apply QCD weights" << endl;
  // Loop over MC entries and mix pileup protons
  int iDataEntry;
  for (iDataEntry = 0; iDataEntry < nDataEntries; iDataEntry++) {
    if(iDataEntry%1000==0) printf ("\r [%3.0f%%] done", 100.*(float)iDataEntry/(float)nDataEntries);
    
	// asign the protons to the MC event
    chMCEvents->GetEntry(iDataEntry);
	if (nBjets<2) continue;
	if (l_tight) continue;
	
	weight *= GetFF_QCD(l_pt,cat)*_xsec*(total_lumi/norm);
	qcd_Up = GetFF_QCD_up(l_pt,cat)/GetFF_QCD(l_pt,cat) - 1;
	qcd_Down = 1 - GetFF_QCD_dn(l_pt,cat)/GetFF_QCD(l_pt,cat);

    tMCQCD->Fill();
  }

  cout << "\nDone. " << iDataEntry << " events processed." << endl;
      timed=time(NULL);
    times=timed-times;
    cout << "time from start to end = " << times << endl;
	
  // Write mixed tree to file and close everything
  fOutMC->cd();
  cout << "Writes " << fOutMC->GetName() << endl;
  tMCQCD->Write();
  fOutMC->Close();  

}  

printf("hadd -f %s/QCD_MCSamples.root %s/*pythia8_QCD.root\n",MCPath.c_str(),MCPath.c_str());

return 1;

}
