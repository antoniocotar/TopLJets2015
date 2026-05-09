// =================================================================================================
// BLOCK 01 — HEADERS, DOCUMENTATION, AND EXTERNAL DEPENDENCIES
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Load standard C++, ROOT, and TopLJets2015/CMS analysis headers.
//   - Document the intended usage of the executable.
//   - This program mixes proton candidates taken from data into MC events.
//   - It is designed for the 2017 exclusive/semi-exclusive top analysis with PPS information.
//
// Physics idea:
//   - Start from a central MC event.
//   - Draw pileup proton candidates from data control samples.
//   - Attach those protons to the MC event.
//   - Modify event weights to account for region probabilities, proton-tag probabilities,
//     pileup reweighting, PPS efficiencies, and trigger scale factors.
// =================================================================================================

#include <iostream>
#include <algorithm>

/*
Code was developed for exclusive top analysis by Enrico Robutti and Michael Pitt
Code mix pileup protons with MC simulation and it was used for 2017 data only (special strip treatment)

Usage:
mergePileupProtons /eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/mc/excl_ttbar_semilep_QED_xa150_era2017_preTS2.root /eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/data/
NOTE: for the events in /eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/mc/ 
  are already after the proton mixing was applied (be aware of wrong normalization if running twice)
*/

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TDirectory.h>
#include <TBranch.h>
#include <TRandom3.h>
#include <TMath.h>
#include "TSystem.h"

#include "TopLJets2015/TopAnalysis/interface/protonTrackRatios.h"
#include "TopLJets2015/TopAnalysis/interface/PPSEff.h"
#include "TopLJets2015/TopAnalysis/interface/EfficiencyScaleFactorsWrapper.h"

#include <cstdlib>  // For std::rand() and std::srand()
#include <time.h>

using namespace std;





// =================================================================================================
// BLOCK 02 — MAIN PROGRAM SETUP, RANDOM SEED, CMSSW PATHS, AND DEFAULT PARAMETERS
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Start the executable.
//   - Initialize random-number generators.
//   - Read CMSSW_BASE and construct the path to era2017 correction files.
//   - Define default control parameters:
//       * random seed,
//       * number of MC events to mix,
//       * number of MC events to skip.
//
// Main objects:
//   - TRandom3 rand_gen: ROOT random-number generator used for region choice, proton-pool choice,
//     and PPS efficiency accept/reject decisions.
//   - CMSSW_BASE: used to locate TopLJets2015 correction files.
//
// Physics role:
//   - Random sampling is essential because pileup protons are injected statistically from data.
// =================================================================================================




int main(int argc, char* argv[])
{

  // Seed the random number generator once
  std::srand(std::time(nullptr));	
	
  // CMSSW settings  
  const char* CMSSW_BASE = getenv("CMSSW_BASE");
  TString data_path = Form("%s/src/TopLJets2015/TopAnalysis/data/era2017/", CMSSW_BASE);

  // Parameters
  int rndSeed = 1234567890; 
  int nEventsToMix = -1;
  int nMCEventsToSkip = 0;
  
  TRandom3 *rand_gen = new TRandom3(rndSeed);
  

	// =================================================================================================
	// BLOCK 03 — COMMAND-LINE ARGUMENT PARSING AND INPUT VALIDATION
	// -------------------------------------------------------------------------------------------------
	// Purpose:
	//   - Check that the executable receives the correct number of arguments.
	//   - Validate that the input MC ROOT file exists.
	//   - Validate that the input data path exists.
	//
	// Expected usage:
	//   mergePileupProtons <inputMCFileName> <inputDataPath> [nMCEventsToMix] [nMCEventsToSkip]
	//
	// Required inputs:
	//   argv[1] = input MC ROOT file.
	//   argv[2] = directory containing hadded data files, e.g.
	//             SingleElectron_2017B.root, SingleMuon_2017B.root, etc.
	//
	// Physics role:
	//   - The MC file provides the central event.
	//   - The data path provides the pileup proton pools used for mixing.
	// =================================================================================================





  // Check arguments
  if (argc < 3 || argc > 5) {
    cout << "Two to four arguments required!" << endl
         << "Usage: " << argv[0] << " <inputMCFileName> <inputDataPath\
> [<nMCEventsToMix>=all] [<nMCEventsToSkip>=0]" << endl;
    cout << "Exampe: " << argv[0] << " /eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/mc/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8.root \
/eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/data/" << endl;
    return 1;
  }
  
  // Check input files
  if(gSystem->AccessPathName(argv[1])){
	  cout << "ERROR! Missing input file: " << argv[1] << endl;
	  return 0;
  }
  if(gSystem->AccessPathName(argv[2])){
	  cout << "ERROR! wrong path to data files: " << argv[2] << endl;
	  return 0;
  }  



// =================================================================================================
// BLOCK 04 — PHYSICS SAMPLE CLASSIFICATION AND OPTIONAL EVENT-RANGE SETTINGS
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Store input/output file names.
//   - Decide whether the input MC file is a signal sample.
//   - Parse optional arguments controlling how many MC events are processed and how many are skipped.
//
// Main variables:
//   - inMCFileName: input MC ROOT file.
//   - inPUFileName: path to data proton-pool files.
//   - outFileName: output enriched MC ROOT file.
//   - isSignal: true for photon/pomeron signal samples.
//   - nEventsToMix: number of MC events to process.
//   - nMCEventsToSkip: number of MC events to skip before processing.
//
// Physics role:
//   - Signal and background are treated differently during proton mixing.
//   - Signal can already contain real signal protons; background generally receives pileup protons.
// =================================================================================================










  
  string inMCFileName = argv[1];
  string inPUFileName = argv[2];
  string outFileName = inMCFileName.substr(inMCFileName.find_last_of('/') + 1, inMCFileName.find_last_of('.') - inMCFileName.find_last_of('/') - 1) + "_enriched.root";
  bool isSignal = TString(inMCFileName.c_str()).Contains("ntuples_ttphoto") ||
                  TString(inMCFileName.c_str()).Contains("ntuples_twphoto") ||
                  TString(inMCFileName.c_str()).Contains("ntuples_ttpomflux");  
  if (argc >= 4) {
    TString arg3 = TString(argv[3]);
    if (arg3.IsDec())
      nEventsToMix = arg3.Atoi();
    else
      cout << "Argument 3 must be an integer number. All input events will be mixed." << endl;
  }
  if (argc >= 5) { 
    TString arg4 = TString(argv[4]);
    if (arg4.IsDec())
      nMCEventsToSkip = arg4.Atoi();
    else
      cout << "Argument 4 must be an integer number. No pileup events will be s\
kipped." << endl;
  }
   

// =================================================================================================
// BLOCK 05 — LOAD PPS EFFICIENCIES AND LEPTON TRIGGER SCALE-FACTOR TOOLS
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Load PPS reconstruction-efficiency maps.
//   - Load electron trigger scale-factor tools for each 2017 era.
//
// Main correction tools:
//   - MultiRP_eff: PPS multi-RP efficiency.
//   - Strip_eff: PPS strip efficiency.
//   - lepEffH_eraB ... lepEffH_eraF: era-dependent electron trigger scale factors.
//
// Physics role:
//   - PPS efficiencies emulate whether a true signal proton is reconstructed or lost.
//   - Electron trigger scale factors correct MC to match data trigger efficiencies by era.
// =================================================================================================








   // Proton efficiency class
   PPSEff *MultiRP_eff = new PPSEff(Form("%s/pixelEfficiencies_multiRP.root",data_path.Data()));
   //PPSEff *Strip_eff = new PPSEff(Form("%s/PreliminaryEfficiencies_July132020_1D2DMultiTrack.root",data_path.Data()));
   //PPSEff *Strip_eff = new PPSEff(Form("%s/Alt2017ERestrictedRuns_PreliminaryEfficiencies_March122021_1D2DMultiTrack.root",data_path.Data()));
   PPSEff *Strip_eff = new PPSEff(Form("%s/PreliminaryEfficiencies_March302021_1D2DMultiTrack.root",data_path.Data()));
   
   // Electron trigger eff (calculated per era)
   EfficiencyScaleFactorsWrapper lepEffH_eraB(0,data_path,"B");
   EfficiencyScaleFactorsWrapper lepEffH_eraC(0,data_path,"C");
   EfficiencyScaleFactorsWrapper lepEffH_eraD(0,data_path,"D");
   EfficiencyScaleFactorsWrapper lepEffH_eraE(0,data_path,"E");
   EfficiencyScaleFactorsWrapper lepEffH_eraF(0,data_path,"F");
      
   // ---------------------------------------------------------------------------------------------------------------------------------- //
   // ---------------------------------------------------------------------------------------------------------------------------------- //
   // ---------------------------------------------------------------------------------------------------------------------------------- //
   // NEW code, create pools from the data files, calculate relative cross-sections and nvtx distributions (era,xangle)
   // Read data, prerare PU trees:






// =================================================================================================
// BLOCK 06 — DEFINE ERAS, CROSSING ANGLES, LUMINOSITIES, AND PILEUP REGIONS
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Define 2017 eras and their integrated luminosities.
//   - Define PPS crossing-angle regions.
//   - Define the full set of pileup regions as era × crossing angle.
//
// Main definitions:
//   - era_lumi[i]: luminosity of era i.
//   - era[i]: 2017B, 2017C, 2017D, 2017E, 2017F.
//   - xangle[j]: PPS crossing angle, e.g. 120, 130, 140, 150.
//   - n_PUregions = n_era × n_xa.
//
// Physics role:
//   - Pileup proton rates depend on data-taking era and LHC crossing angle.
//   - The MC event is randomly assigned to a data-like era/xangle region.
// =================================================================================================







   float era_lumi[] = {2.360910165,8.577154400,4.074834625,1.440661783,13.219864250};;
   TString era[] = {"2017B", "2017C", "2017D", "2017E", "2017F"}; int n_era = (sizeof(era)/sizeof(TString));
   int xangle[] = {120, 130, 140, 150}; int n_xa = (sizeof(xangle)/sizeof(int));
   
   const int n_PUregionsMAX = n_era * n_xa; // used in signal normalization
   





// =================================================================================================
// BLOCK 07 — SIGNAL-SPECIFIC ERA/XANGLE NORMALIZATION
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - For signal samples, compute extra normalization factors because signal MC is generated
//     separately for specific era/xangle configurations.
//   - Restrict the list of eras and crossing angles depending on the signal sample name.
//
// Main variables:
//   - signal_fraction_regions: data-derived fraction for each era/xangle region.
//   - extra_signal_normalization: final signal-specific normalization factor.
//   - isSignal: controls whether this block modifies the region setup.
//   - n_era and n_xa may be reduced for preTS2/postTS2 or fixed-xangle signal samples.
//
// Physics role:
//   - Signal samples are not generic inclusive MC.
//   - Dedicated signal files correspond to specific PPS optics/crossing-angle configurations.
//   - Their normalization must be corrected to represent the luminosity fractions in data.
// =================================================================================================







   // Extra normalization factor used for signal since we have dedicated signal samples per era/xangle
   float signal_fraction_regions[n_PUregionsMAX], extra_signal_normalization=0;
   float sig_total_event_per_era[n_PUregionsMAX];
   if(isSignal){
	   
	  //Get extra normalization
      for(int i_era=0;i_era<n_era;i_era++){
	    TString name_el = Form("%s/SingleElectron_%s.root",inPUFileName.c_str(),era[i_era].Data());
	    TString name_mu = Form("%s/SingleMuon_%s.root",inPUFileName.c_str(),era[i_era].Data());
	      for(int i_xa=0;i_xa<n_xa;i_xa++){ // create proton pools
		    // calculate fraction of Xangle at preselection fpr ALL REGIONS:
		    TChain * _ch2 = new TChain("tree"); _ch2->Add(name_el); _ch2->Add(name_mu);
		    signal_fraction_regions[i_era*n_xa+i_xa] = _ch2->GetEntries(Form("beamXangle==%d",xangle[i_xa]));
			sig_total_event_per_era[i_era*n_xa+i_xa]=_ch2->GetEntries();
	      }
	    // normalize properly per selected crossing-angle (sometimes data contains unselected values lke 141,142...)
	    for(int ii=0;ii<n_xa;ii++) signal_fraction_regions[i_era*n_xa+ii] *= (era_lumi[i_era]/(29.673425))/sig_total_event_per_era[i_era*n_xa+ii];
      }

	  // reduce the number of regions in case if the sample is a simulated signal
	  
	  // set crossing angle
	  int i_xa = 0;
	  if(TString(inMCFileName.c_str()).Contains("xa120")) {xangle[0] = 120; i_xa=0;}
	  if(TString(inMCFileName.c_str()).Contains("xa130")) {xangle[0] = 130; i_xa=1;}
	  if(TString(inMCFileName.c_str()).Contains("xa140")) {xangle[0] = 140; i_xa=2;}
	  if(TString(inMCFileName.c_str()).Contains("xa150")) {xangle[0] = 150; i_xa=3;}  

	  // set list of periods
	  if(TString(inMCFileName.c_str()).Contains("postTS2")){
		   n_era = 2;
		   era_lumi[0] = era_lumi[3]; era[0] = "2017E";
		   era_lumi[1] = era_lumi[4]; era[1] = "2017F";
		   for(int i_era=3;i_era<5;i_era++)
		     extra_signal_normalization += signal_fraction_regions[i_era*n_xa+i_xa];
	  }
	  else {
		  n_era = 3; // for preTS2 first 3 entries are the same n_era =1 for only run era B
		  for(int i_era=0;i_era<3;i_era++) // i_era<1 for only 2017B
		    extra_signal_normalization += signal_fraction_regions[i_era*n_xa+i_xa];
	  }
	  
	  // set to 1 the crossing angle counter
	  n_xa=1; 
	  cout << "Signal extra normalization = " << extra_signal_normalization << endl;
   }




// =================================================================================================
// BLOCK 08 — BUILD DATA-DRIVEN PILEUP-PROTON POOLS AND REGION PROBABILITIES
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Read hadded SingleElectron and SingleMuon data files.
//   - Build one pileup-proton tree for each era/xangle region.
//   - Compute data nvtx distributions for pileup reweighting.
//   - Compute proton-tag probabilities and their systematic variations.
//
// Main outputs:
//   - PUpr[i_reg]: proton pool tree for region i_reg.
//   - pu_weights[i_reg]: normalized nvtx distribution in data for region i_reg.
//   - fraction_regions[i_reg]: probability weight of each era/xangle region.
//   - norm_weight[i_reg]: probability of observing two pileup protons.
//   - norm_weight_1pRP0[i_reg]: probability of one proton in arm 0.
//   - norm_weight_1pRP1[i_reg]: probability of one proton in arm 1.
//   - norm_weight_0p[i_reg]: probability of zero pileup protons.
//   - norm_weight_err and related errors: systematic uncertainty proxies.
//
// Physics role:
//   - This block converts real 2017 data into empirical proton-pileup templates.
//   - Later, each MC event randomly samples one of these pools.
// =================================================================================================











   const int n_PUregions = n_era * n_xa;
   // puleup histograms (used for reweighting)
   TH1F * pu_weights[n_PUregions];
   TFile * _puweights = new TFile("pu_weights.root","recreate"); // dummy file for protons pools to be accosiated with.

   cout << "Reads data from " <<    inPUFileName << endl;
   TTree * PUpr[n_PUregions];  // puleup proton trees (for each era and xangle)
   
   // calculate fraction of the total luminosity
   float total_lumi=0;
   for(int i_era=0;i_era<n_era;i_era++) total_lumi+=era_lumi[i_era];
   
   float total_event_per_era[n_PUregions];


	// only for exlusive case
	float norm_weight[n_PUregions];
	float norm_weight_err[n_PUregions];

	float fraction_regions[n_PUregions];
	int counter_regions[n_PUregions]; 


	float norm_weight_0p[n_PUregions], norm_weight_0p_err[n_PUregions]; // for signal events
	float norm_weight_1pRP0[n_PUregions], norm_weight_1pRP0_err[n_PUregions]; // for signal events
	float norm_weight_1pRP1[n_PUregions], norm_weight_1pRP1_err[n_PUregions]; // for signal events








   
   // Modification for efficiency
   for(int i_era=0;i_era<n_era;i_era++){
	   printf ("\rProcessing %s ", era[i_era].Data());
	   TString name_el = Form("%s/SingleElectron_%s.root",inPUFileName.c_str(),era[i_era].Data());
	   TString name_mu = Form("%s/SingleMuon_%s.root",inPUFileName.c_str(),era[i_era].Data());
	   
	   TFile *f_el = TFile::Open(name_el);
	   TFile *f_mu = TFile::Open(name_mu);

	   if(!f_el || f_el->IsZombie()){
		   cout << "ERROR opening " << name_el << endl;
		   return 10;
	   }
	   if(!f_mu || f_mu->IsZombie()){
		   cout << "ERROR opening " << name_mu << endl;
		   return 11;
	   }

	   TH1F *evt_count_el = (TH1F*)f_el->Get("evt_count");
	   TH1F *evt_count_mu = (TH1F*)f_mu->Get("evt_count");
	   TH1F *pn_count_el  = (TH1F*)f_el->Get("pn_count");
	   TH1F *pn_count_mu  = (TH1F*)f_mu->Get("pn_count");

	   if(!evt_count_el || !evt_count_mu || !pn_count_el || !pn_count_mu){
		   cout << "ERROR: missing evt_count or pn_count histogram for era " << era[i_era] << endl;
		   return 12;
	   }

	   // Read 2 proton hits and total number of events
	   int n_p2_e = int(evt_count_el->GetBinContent(4));
	   int n_p2_m = int(evt_count_mu->GetBinContent(4));
	   int n_e    = int(evt_count_el->GetBinContent(3));
	   int n_m    = int(evt_count_mu->GetBinContent(3));
	   int n_p2 = (n_p2_e+n_p2_m), n = (n_e+n_m);
	   
	   // Get systematics for n_proton==2 event fraction from sub-selection of nBjet>0 events:
	   int n_e_sys = int(pn_count_el->GetBinContent(5));
	   int n_m_sys = int(pn_count_mu->GetBinContent(5));

	   TChain * _ch_protons = new TChain("protons");
	   _ch_protons->Add(name_el);
	   _ch_protons->Add(name_mu);
	   int n_sys = (n_e_sys+n_m_sys), n_p2_sys = _ch_protons->GetEntries("nBjets>0");

	   TChain * _ch_tree = new TChain("tree");
	   _ch_tree->Add(name_el);
	   _ch_tree->Add(name_mu);
	   Long64_t n_tree_all = _ch_tree->GetEntries();
	   
	   // Get 1 proton hits (depend on the arm)
	   int n_p1_eRP0 = int(pn_count_el->GetBinContent(2));
	   int n_p1_mRP0 = int(pn_count_mu->GetBinContent(2));
	   int n_p1_eRP1 = int(pn_count_el->GetBinContent(3));
	   int n_p1_mRP1 = int(pn_count_mu->GetBinContent(3));
	   int n_p1_RP0 = (n_p1_eRP0+n_p1_mRP0), n_p1_RP1 = (n_p1_eRP1+n_p1_mRP1);
		






	   
	   // events with exactly 0 pu tracks
	   int n_p0 = n - n_p1_RP0 - n_p1_RP1 - n_p2; 
	   
	   for(int i_xa=0;i_xa<n_xa;i_xa++){ // create proton pools
  		   _puweights->cd();

		   PUpr[i_era*n_xa+i_xa] = _ch_protons->CopyTree(Form("beamXangle==%d",xangle[i_xa]));

		   pu_weights[i_era*n_xa+i_xa] = new TH1F(Form("data_puw_%d",i_era*n_xa+i_xa),";nvtx;w",100,0,100);
		   PUpr[i_era*n_xa+i_xa]->Draw(Form("nvtx>>data_puw_%d",i_era*n_xa+i_xa));

		   counter_regions[i_era*n_xa+i_xa] = PUpr[i_era*n_xa+i_xa]->GetEntries();

		   if(counter_regions[i_era*n_xa+i_xa] > 0)
			   pu_weights[i_era*n_xa+i_xa]->Scale(1/float(counter_regions[i_era*n_xa+i_xa]));

		   // calculate fraction of Xangle at preselection:
		   fraction_regions[i_era*n_xa+i_xa] = _ch_tree->GetEntries(Form("beamXangle==%d",xangle[i_xa]));
		   total_event_per_era[i_era*n_xa+i_xa] = isSignal ? fraction_regions[i_era*n_xa+i_xa] : n_tree_all;
		   




		   norm_weight[i_era*n_xa+i_xa] = n_p2/float(n); // probability of 2 tracks
	       norm_weight_err[i_era*n_xa+i_xa] = (n_p2_sys/float(n_sys)) / norm_weight[i_era*n_xa+i_xa];
		   		   
		   // probabilities for 1 track in signal events
	       norm_weight_1pRP0[i_era*n_xa+i_xa] = n_p1_RP0/float(n); // probability of 0 tracks in RP0   P(1,0)
	       norm_weight_1pRP0_err[i_era*n_xa+i_xa] = 0.95; // 5% flat

	       norm_weight_1pRP1[i_era*n_xa+i_xa] = n_p1_RP1/float(n); // probability of 0 tracks in RP1   P(0,1)
	       norm_weight_1pRP1_err[i_era*n_xa+i_xa] = 0.95; // 5% flat







		   
		   // probabilities for 0 track in signal events
	       norm_weight_0p[i_era*n_xa+i_xa] = (n_p0)/float(n); // probability of 0 tracks in both arms
	       norm_weight_0p_err[i_era*n_xa+i_xa] = 0.95; // 5% flat
		   
	   }

	   // normalize properly per selected crossing-angle (sometimes data contains unselected values like 100,110,...)
	   for(int ii=0;ii<n_xa;ii++) fraction_regions[i_era*n_xa+ii] *= (era_lumi[i_era]/total_lumi)/total_event_per_era[i_era*n_xa+ii];

	   delete _ch_tree;
	   delete _ch_protons;

	   f_el->Close();
	   f_mu->Close();
	   delete f_el;
	   delete f_mu;
   }

   // Keep diagnostic uncertainty arrays and avoid -Werror=unused-but-set-variable.
   volatile float keep_norm_weight_errors = 0.0;
   for(int i=0; i<n_PUregions; i++){
	   keep_norm_weight_errors += norm_weight_err[i];
	   keep_norm_weight_errors += norm_weight_1pRP0_err[i];
	   keep_norm_weight_errors += norm_weight_1pRP1_err[i];
   }
   (void)keep_norm_weight_errors;





// =================================================================================================
// BLOCK 09 — REPORT REGION FRACTIONS AND ATTACH PILEUP-POOL BRANCHES
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Print the derived era/xangle region fractions.
//   - Attach branch addresses for each pileup-proton pool.
//   - Prepare fast access to run, p1_xi, and p2_xi for the randomly selected pileup event.
//
// Main variables:
//   - poll_run[i_reg]: run number from selected data proton-pool event.
//   - poll_p1_xi[i_reg]: proton xi in positive arm from pileup pool.
//   - poll_p2_xi[i_reg]: proton xi in negative arm from pileup pool.
//
// Physics role:
//   - The MC event inherits a run number and crossing angle from the randomly selected data region.
//   - Pileup proton xi values are sampled from real data.
// =================================================================================================











   // List obtained fractions:
   cout << "\nINFO List obtained fractions for all "<<n_PUregions<<" SRs, (for 120,130,140,150), including statistics:"<<endl;
   for(int i_era=0;i_era<n_era;i_era++){
	   cout << "era " << era[i_era]<<": ";
	   for(int ii=0;ii<n_xa-1;ii++) cout << fraction_regions[i_era*n_xa+ii]<<" ("<<counter_regions[i_era*n_xa+ii]<<"),";
	   cout << fraction_regions[i_era*n_xa+n_xa-1] <<" ("<<counter_regions[i_era*n_xa+n_xa-1]<<")"<< endl;
   }   
   
   // variables to be used to read from PU pools
    unsigned int poll_run[n_PUregions]; float poll_p1_xi[n_PUregions],poll_p2_xi[n_PUregions];
	_puweights->cd();
	for(int i=0;i<n_PUregions;i++){
		PUpr[i]->SetBranchAddress("run",&poll_run[i]);
		PUpr[i]->SetBranchAddress("p1_xi",&poll_p1_xi[i]);
		PUpr[i]->SetBranchAddress("p2_xi",&poll_p2_xi[i]);
	}
    cout << "done "<< endl;
   //
   // ---------------------------------------------------------------------------------------------------------------------------------- //
   // ---------------------------------------------------------------------------------------------------------------------------------- //
   // ---------------------------------------------------------------------------------------------------------------------------------- //
   
   //Save relative fractions of periods:
   //_puweights->cd();
   //cout << "write " <<   _puweights->GetName()  << endl;
   //for(int i=0;i<n_PUregions;i++) pu_weights[i]->Write();
   //_puweights->Write();
   //_puweights->Close();
   
   



// =================================================================================================
// BLOCK 10 — READ INPUT MC TREE AND ATTACH MC BRANCHES
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Open the input MC ROOT file.
//   - Read the central MC event tree.
//   - Build the MC nvtx distribution.
//   - Attach branch addresses for variables that will be modified or used during mixing.
//
// Main MC branches:
//   - run, beamXangle
//   - p1_xi, p2_xi
//   - weight, pu_wgt, ptag_wgt, ppsSF_wgt
//   - lepton kinematics and event category
//   - nvtx
//
// Physics role:
//   - The MC event provides the hard-scattering kinematics.
//   - This program modifies proton-related quantities and event weights.
//   - The MC nvtx distribution is used to reweight MC pileup to the data proton-pool nvtx profile.
// =================================================================================================









   
  // read MC file    
  TFile *oldfile = new TFile(inMCFileName.c_str());
  TTree * chMCEvents = (TTree *)oldfile->Get("tree");
  cout << "Store MC nvtx distribution " << endl;
  TH1F * mc_pu = new TH1F("mc_pu",";nvtx;w",100,0,100);
  chMCEvents->Draw("nvtx>>mc_pu","","norm");
  TH1F * evt_count = (TH1F*)oldfile->Get("evt_count"); // event counter with SumWeights
  
  //list of branches to update:
	unsigned int run; int nvtx;
	float beamXangle, p1_xi, p2_xi, weight, ppsSF_wgt, ppsSF_wgt_err, pu_wgt, ptag_wgt, ptag_wgt_err; 
	float lep_pt, lep_eta, lep_phi, lep_m, cat, triggerSF, triggerSF_err;
	float p1_x = 0, p1_y = 0, p2_x = 0, p2_y = 0;  






// =================================================================================================
// BLOCK 13 — PER-EVENT RESET, REGION WEIGHT, RUN/XANGLE REASSIGNMENT, AND PROBABILITY BOOKKEEPING
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Reset all per-event weight factors, probabilities, efficiencies, random numbers, and flags.
//   - Apply the era/xangle region weight.
//   - Replace the MC run and beamXangle with values consistent with the sampled data region.
//   - Store proton multiplicity probabilities and true-zero-track ratios.
//
// Main operations:
//   - weight *= w_region.
//   - run = poll_run[i_reg].
//   - beamXangle = xangle[i_reg % n_xa].
//   - Store Pr_2p, Pr_1_0, Pr_0_1, Pr_0_0.
//   - Store t1_run_thetax and t0_run_thetax from protonTrackRatios.
//
// Physics role:
//   - Makes the MC event look like it occurred in a realistic 2017 data-taking region.
//   - Encodes the probability of observing proton pileup in that region.
// =================================================================================================








	// new explicit stored weight factors
	float w_region    = 1.0;
	float w_etrig     = 1.0;
	float w_extra_sig = 1.0;

	// new stored physics/probability factors
	float Pr_2p  = 1.0;
	float Pr_1_0 = 1.0;
	float Pr_0_1 = 1.0;
	float Pr_0_0 = 1.0;

	float t1_run_thetax = 1.0;
	float t0_run_thetax = 1.0;

	float e_strip_0   = 1.0;
	float e_strip_1   = 1.0;
	float e_multiRP_0 = 1.0;
	float e_multiRP_1 = 1.0;

	int   delta_0 = 0;
	int   delta_1 = 0;

	float u_0 = -1.0;
	float u_1 = -1.0;

	float SF_eff_0 = 1.0;
	float SF_eff_1 = 1.0;
	
	int signal_protons = -99;

	// Important for the one-proton mixing logic.
	// Do not keep values read from the input MC tree.
	ptag_wgt        = 1.0;
	ptag_wgt_err    = 0.0;
	signal_protons  = -99;



  //float p1_220_x = 0, p1_220_y = 0, p2_220_x = 0, p2_220_y = 0;  
  chMCEvents->SetBranchAddress("run",&run);
  chMCEvents->SetBranchAddress("beamXangle",&beamXangle);
  chMCEvents->SetBranchAddress("p1_xi",&p1_xi);
  chMCEvents->SetBranchAddress("p2_xi",&p2_xi);
  if(isSignal){
	  chMCEvents->SetBranchAddress("p1_x",&p1_x);
	  chMCEvents->SetBranchAddress("p1_y",&p1_y);
	  chMCEvents->SetBranchAddress("p2_x",&p2_x);
	  chMCEvents->SetBranchAddress("p2_y",&p2_y);
	  //chMCEvents->SetBranchAddress("p1_220_x",&p1_220_x);
	  //chMCEvents->SetBranchAddress("p1_220_y",&p1_220_y);
	  //chMCEvents->SetBranchAddress("p2_220_x",&p2_220_x);
	  //chMCEvents->SetBranchAddress("p2_220_y",&p2_220_y);
  }
  chMCEvents->SetBranchAddress("weight",&weight);
  chMCEvents->SetBranchAddress("pu_wgt",&pu_wgt);
  chMCEvents->SetBranchAddress("EL_trigSF_wgt",&triggerSF);
  chMCEvents->SetBranchAddress("EL_trigSF_wgt_err",&triggerSF_err);
  chMCEvents->SetBranchAddress("ppsSF_wgt",&ppsSF_wgt);
  chMCEvents->SetBranchAddress("ppsSF_wgt_err",&ppsSF_wgt_err);
  chMCEvents->SetBranchAddress("ptag_wgt",&ptag_wgt);
  chMCEvents->SetBranchAddress("ptag_wgt_err",&ptag_wgt_err);
  chMCEvents->SetBranchAddress("nvtx",&nvtx);
  chMCEvents->SetBranchAddress("l_pt",&lep_pt);
  chMCEvents->SetBranchAddress("l_eta",&lep_eta);
  chMCEvents->SetBranchAddress("l_phi",&lep_phi);
  chMCEvents->SetBranchAddress("l_m",&lep_m);
  chMCEvents->SetBranchAddress("cat",&cat); // 4-el, 5-mu
  chMCEvents->SetBranchStatus("*",1); // activate all branches to copy
  
  int nMCEntries = chMCEvents->GetEntries();
  cout << nMCEntries << " events read from file(s) " << inMCFileName << endl;
  if (nEventsToMix == -1)
    nEventsToMix = nMCEntries;



// =================================================================================================
// BLOCK 11 — LOAD PROTON-RATIO CORRECTIONS AND CREATE OUTPUT ENRICHED TREE
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Load true-zero-track proton-ratio corrections from protonRatios_2017.dat.
//   - Open the output ROOT file.
//   - Clone the MC tree structure.
//   - Add new branches that expose the internal mixing weights, probabilities, efficiencies,
//     random decisions, and signal-proton categories.
//
// Main new branches:
//   - signal_protons: category describing whether protons are signal or injected pileup.
//   - w_region: era/xangle region weight.
//   - w_etrig: electron trigger scale factor.
//   - w_extra_sig: extra signal normalization.
//   - Pr_2p, Pr_1_0, Pr_0_1, Pr_0_0: proton multiplicity probabilities.
//   - e_strip_0/1 and e_multiRP_0/1: PPS efficiencies.
//   - delta_0/1: whether the signal proton survived PPS efficiency emulation.
//   - u_0/1: random numbers used for PPS accept/reject.
//   - SF_eff_0/1: total PPS efficiency factors per arm.
//
// Physics role:
//   - The output tree is the final MC enriched with data-driven pileup protons.
//   - The new branches make the mixing procedure auditable and debuggable.
// =================================================================================================








  // Get pileup proton ratios
  protonTrackRatios ptr;
  int nLines = ptr.readFromFile(Form("%s/protonRatios_2017.dat",data_path.Data()));
  if (nLines <= 0) {
    cout << "No true-zero-track ratio read from file! No mixing performed." << endl;
    return 4;
  }
  
  // Open output mixed file
  TFile* fOutMixed = new TFile(outFileName.c_str(), "RECREATE");
  if(!fOutMixed->IsOpen()) {
    cout<< "Error opening output file mixedPUProtons. Abort." << endl;
    return 5;
  }
 
	TTree* tMCMixed = chMCEvents->CloneTree(0);

	signal_protons = 0; // additional variables used for signal to indicate if all/part/none of the protons are from pileup
	tMCMixed->Branch("signal_protons",&signal_protons);

	// new branches weight factors
	tMCMixed->Branch("w_region",    &w_region,    "w_region/F");
	tMCMixed->Branch("w_etrig",     &w_etrig,     "w_etrig/F");
	tMCMixed->Branch("w_extra_sig", &w_extra_sig, "w_extra_sig/F");

	// new branches for probabilities and efficiencies
	tMCMixed->Branch("Pr_2p",  &Pr_2p,  "Pr_2p/F");
	tMCMixed->Branch("Pr_1_0", &Pr_1_0, "Pr_1_0/F");
	tMCMixed->Branch("Pr_0_1", &Pr_0_1, "Pr_0_1/F");
	tMCMixed->Branch("Pr_0_0", &Pr_0_0, "Pr_0_0/F");

	tMCMixed->Branch("t1_run_thetax", &t1_run_thetax, "t1_run_thetax/F");
	tMCMixed->Branch("t0_run_thetax", &t0_run_thetax, "t0_run_thetax/F");

	tMCMixed->Branch("e_strip_0",   &e_strip_0,   "e_strip_0/F");
	tMCMixed->Branch("e_strip_1",   &e_strip_1,   "e_strip_1/F");
	tMCMixed->Branch("e_multiRP_0", &e_multiRP_0, "e_multiRP_0/F");
	tMCMixed->Branch("e_multiRP_1", &e_multiRP_1, "e_multiRP_1/F");

	tMCMixed->Branch("delta_0", &delta_0, "delta_0/I");
	tMCMixed->Branch("delta_1", &delta_1, "delta_1/I");

	tMCMixed->Branch("u_0", &u_0, "u_0/F");
	tMCMixed->Branch("u_1", &u_1, "u_1/F");

	tMCMixed->Branch("SF_eff_0", &SF_eff_0, "SF_eff_0/F");
	tMCMixed->Branch("SF_eff_1", &SF_eff_1, "SF_eff_1/F");

   



// =================================================================================================
// BLOCK 12 — EVENT LOOP: RANDOMLY SELECT ERA/XANGLE REGION AND DATA PROTON-POOL EVENT
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Loop over MC events.
//   - Randomly select a pileup region i_reg.
//   - Randomly select one data proton-pool event from PUpr[i_reg].
//   - Load both the MC event and the selected data proton event.
//
// Region sampling:
//   - i_reg is sampled uniformly from the available regions.
//   - The correction factor w_region = fraction_regions[i_reg] × n_PUregions
//     restores the desired non-flat region distribution.
//
// Physics role:
//   - This is the core stochastic mixing step:
//       MC hard event + randomly sampled data pileup proton event.
// =================================================================================================






  // ===============================================================================================
  // FINAL COMPACT SUMMARY COUNTERS
  // ===============================================================================================

  Long64_t n_topo_before_00 = 0, n_topo_before_10 = 0, n_topo_before_01 = 0, n_topo_before_11 = 0;
  Long64_t n_topo_final_00  = 0, n_topo_final_10  = 0, n_topo_final_01  = 0, n_topo_final_11  = 0;

  Long64_t n_sig_cat_m99 = 0;
  Long64_t n_sig_cat_m1  = 0;
  Long64_t n_sig_cat_0   = 0;
  Long64_t n_sig_cat_1   = 0;
  Long64_t n_sig_cat_2   = 0;
  Long64_t n_sig_cat_3   = 0;
  Long64_t n_sig_cat_10  = 0;
  Long64_t n_sig_cat_11  = 0;
  Long64_t n_sig_cat_other = 0;

  Long64_t n_sampled_region[n_PUregions];
  for(int i=0; i<n_PUregions; i++) n_sampled_region[i] = 0;

  double sum_weight_initial      = 0.0;
  double sum_weight_after_region = 0.0;
  double sum_weight_after_ptag   = 0.0;
  double sum_weight_after_pu     = 0.0;
  double sum_weight_after_etrig  = 0.0;
  double sum_weight_final        = 0.0;

  double sum_w_region    = 0.0;
  double sum_ptag_wgt    = 0.0;
  double sum_pu_wgt      = 0.0;
  double sum_w_etrig     = 0.0;
  double sum_w_extra_sig = 0.0;

  double sum_Pr_0_0 = 0.0;
  double sum_Pr_1_0 = 0.0;
  double sum_Pr_0_1 = 0.0;
  double sum_Pr_2p  = 0.0;

  double sum_weight_cat_m99 = 0.0;
  double sum_weight_cat_m1  = 0.0;
  double sum_weight_cat_0   = 0.0;
  double sum_weight_cat_1   = 0.0;
  double sum_weight_cat_2   = 0.0;
  double sum_weight_cat_3   = 0.0;
  double sum_weight_cat_10  = 0.0;
  double sum_weight_cat_11  = 0.0;
  double sum_weight_cat_other = 0.0;


  // ===============================================================================================
  // Independent end-of-event audit sums.
  // These are computed at the end of each event from the stored factors:
  //
  //   w_initial
  //   w_region
  //   ptag_wgt
  //   pu_wgt
  //   w_etrig
  //   w_extra_sig
  //
  // They are independent of where the running cut-flow counters were placed.
  // ===============================================================================================

  double audit_sum_initial      = 0.0;
  double audit_sum_after_region = 0.0;
  double audit_sum_after_ptag   = 0.0;
  double audit_sum_after_pu     = 0.0;
  double audit_sum_after_etrig  = 0.0;
  double audit_sum_final        = 0.0;

  double audit_abs_diff_final   = 0.0;
  double audit_max_diff_final   = 0.0;

  double audit_sum_ptag_cat_m99 = 0.0;
  double audit_sum_ptag_cat_m1  = 0.0;
  double audit_sum_ptag_cat_0   = 0.0;
  double audit_sum_ptag_cat_1   = 0.0;
  double audit_sum_ptag_cat_2   = 0.0;
  double audit_sum_ptag_cat_3   = 0.0;
  double audit_sum_ptag_cat_10  = 0.0;
  double audit_sum_ptag_cat_11  = 0.0;
  double audit_sum_ptag_cat_other = 0.0;

  double audit_sum_final_cat_m99 = 0.0;
  double audit_sum_final_cat_m1  = 0.0;
  double audit_sum_final_cat_0   = 0.0;
  double audit_sum_final_cat_1   = 0.0;
  double audit_sum_final_cat_2   = 0.0;
  double audit_sum_final_cat_3   = 0.0;
  double audit_sum_final_cat_10  = 0.0;
  double audit_sum_final_cat_11  = 0.0;
  double audit_sum_final_cat_other = 0.0;

  Long64_t audit_n_after_eff_0 = 0;
  Long64_t audit_n_after_eff_1 = 0;
  Long64_t audit_n_after_eff_2 = 0;
  Long64_t audit_n_after_eff_other = 0;

  Long64_t audit_n_ptag_zero = 0;
  Long64_t audit_n_pu_zero = 0;
  Long64_t audit_n_final_zero_weight = 0;

  double audit_sum_prob_norm = 0.0;
  double audit_max_abs_prob_norm_minus_1 = 0.0;










  int times,timed;
  times=time(NULL);  
  cout << "Loop over MC entries and add pileup protons" << endl;
  // Loop over MC entries and mix pileup protons
  int iMCEntry;






  for (iMCEntry = 0; (iMCEntry < nEventsToMix) && (iMCEntry + nMCEventsToSkip < nMCEntries); iMCEntry++) {
    if(iMCEntry%1000==0) printf ("\r [%3.0f%%] done", 100.*(float)iMCEntry/(float)nEventsToMix);
    
	// ------------- old approach --------------------- //
	// sample region index using PDF and get protons from the pool
	//int i_reg=0; float cdf_=0, rndm = rand_gen->Rndm();
	//while (cdf_<rndm) {cdf_+= fraction_regions[i_reg];i_reg++;}
	//i_reg--; // region index should start from 0

    // Debugging the progress
    //std::cout << "\nProcessing event " << iMCEntry << "/" << nEventsToMix << std::endl;

	// ------------- new approach --------------------- //
	// samples region index using flat PDF, and asign extra weight to event
    int i_reg = rand_gen->Rndm() * n_PUregions;
    float _extra_weight = (fraction_regions[i_reg]*float(n_PUregions));

	// -------------------------------------------------- //

    //std::cout << "Selected region index: " << i_reg << ", Extra weight: " << _extra_weight << std::endl;


	int i_event = rand_gen->Rndm()*counter_regions[i_reg];  
	PUpr[i_reg]->GetEntry(i_event);
	
	// Add additional selection of proton pool here:
	// while(condition) {
	//	i_event = rand_gen->Rndm()*counter_regions[i_reg];
	//	PUpr[i_reg]->GetEntry(i_event);
	//}
	
	// asign the protons to the MC event
	chMCEvents->GetEntry(iMCEntry + nMCEventsToSkip);

	// Store the raw MC input weight before any enrichment factor is applied.
	double weight_initial_event = weight;

	// Count topology before any proton mixing / PPS-efficiency emulation.
	bool has_p1_before = (p1_xi > 0);
	bool has_p2_before = (p2_xi > 0);

	if(!has_p1_before && !has_p2_before) n_topo_before_00++;
	if( has_p1_before && !has_p2_before) n_topo_before_10++;
	if(!has_p1_before &&  has_p2_before) n_topo_before_01++;
	if( has_p1_before &&  has_p2_before) n_topo_before_11++;

	sum_weight_initial += weight;

	// reset stored factors every event
	w_region    = 1.0;



	w_etrig     = 1.0;
	w_extra_sig = 1.0;

	Pr_2p  = 1.0;
	Pr_1_0 = 1.0;
	Pr_0_1 = 1.0;
	Pr_0_0 = 1.0;

	t1_run_thetax = 1.0;
	t0_run_thetax = 1.0;

	e_strip_0   = 1.0;
	e_strip_1   = 1.0;
	e_multiRP_0 = 1.0;
	e_multiRP_1 = 1.0;

	delta_0 = 0;
	delta_1 = 0;

	u_0 = -1.0;
	u_1 = -1.0;

	SF_eff_0 = 1.0;
	SF_eff_1 = 1.0;

	// region weight
	w_region = _extra_weight;
	weight *= w_region;

	n_sampled_region[i_reg]++;
	sum_weight_after_region += weight;





	// fix run number and crossing-angle from proton pool
	run = poll_run[i_reg];
	beamXangle = xangle[i_reg % n_xa];

	// store region probabilities
	Pr_2p  = norm_weight[i_reg];
	Pr_1_0 = norm_weight_1pRP0[i_reg];
	Pr_0_1 = norm_weight_1pRP1[i_reg];
	Pr_0_0 = norm_weight_0p[i_reg];

	// store true-zero-track ratios
	t1_run_thetax = ptr.trueZeroTracksRatio(run, beamXangle, 1);
	t0_run_thetax = ptr.trueZeroTracksRatio(run, beamXangle, 0);
	




// =================================================================================================
// BLOCK 14 — PPS RECONSTRUCTION-EFFICIENCY EMULATION FOR SIGNAL PROTONS
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - For signal MC events with true protons, emulate the probability that the proton is reconstructed.
//   - Compute strip and multi-RP efficiencies per arm.
//   - Use a random number to decide whether each proton survives or is set to xi = 0.
//   - Accumulate PPS efficiency scale factor and uncertainty.
//
// Per arm:
//   - If p_i_xi > 0, compute:
//       SF_eff_i = e_strip_i × e_multiRP_i.
//   - Draw u_i uniformly in [0,1].
//   - If u_i > SF_eff_i, reject the proton and set p_i_xi = 0.
//   - Otherwise keep the proton and multiply ppsSF_wgt by SF_eff_i.
//
// Physics role:
//   - Signal MC may contain genuine forward protons.
//   - PPS detector inefficiency can make a true proton disappear from the reconstructed event.
// =================================================================================================







	// From Exclusive
	// proton efficiency implementation (xi of protons that fail reco. will be set to zero)
	ppsSF_wgt = 1.;
	ppsSF_wgt_err = 0.0;

	// -------------------- ARM 0 --------------------
	if(p1_xi > 0){
		e_strip_0   = Strip_eff->getEff(p1_x, p1_y, 0, run);
		e_multiRP_0 = MultiRP_eff->getEff(p1_x, p1_y, 0, run);

		SF_eff_0 = e_strip_0 * e_multiRP_0;

		u_0 = rand_gen->Rndm();

		if(u_0 > SF_eff_0){
			delta_0 = 0;
			p1_xi = 0;
		} else {
			delta_0 = 1;
			ppsSF_wgt *= SF_eff_0;
		}

		ppsSF_wgt_err += MultiRP_eff->getRelEffErrSq(p1_x, p1_y, 0, run);
	}
	else{
		e_strip_0   = 1.0;
		e_multiRP_0 = 1.0;
		SF_eff_0    = 1.0;
		u_0         = -1.0;
		delta_0     = 0;
	}

	// -------------------- ARM 1 --------------------
	if(p2_xi > 0){
		e_strip_1   = Strip_eff->getEff(p2_x, p2_y, 1, run);
		e_multiRP_1 = MultiRP_eff->getEff(p2_x, p2_y, 1, run);

		SF_eff_1 = e_strip_1 * e_multiRP_1;

		u_1 = rand_gen->Rndm();

		if(u_1 > SF_eff_1){
			delta_1 = 0;
			p2_xi = 0;
		} else {
			delta_1 = 1;
			ppsSF_wgt *= SF_eff_1;
		}

		ppsSF_wgt_err += MultiRP_eff->getRelEffErrSq(p2_x, p2_y, 1, run);
	}
	else{
		e_strip_1   = 1.0;
		e_multiRP_1 = 1.0;
		SF_eff_1    = 1.0;
		u_1         = -1.0;
		delta_1     = 0;
	}

	ppsSF_wgt_err = sqrt(ppsSF_wgt_err);

    //std::cout << "Proton efficiencies applied. ppsSF_wgt: " << ppsSF_wgt << ", ppsSF_wgt_err: " << ppsSF_wgt_err << std::endl;

	

	// =================================================================================================
	// PROTON MIXING LOGIC FOR ONE-PROTON SIGNAL PROCESS
	// -------------------------------------------------------------------------------------------------
	// Michael Pitt recommendation:
	//
	//   Signal with reconstructed signal proton:
	//       multiply by P(0 pileup protons)
	//
	//   Signal without reconstructed signal proton:
	//       multiply by P(1 pileup proton)
	//
	//   Background:
	//       multiply by P(1 pileup proton)
	//
	// Definitions:
	//   P00 = P(0,0) : exactly zero pileup protons
	//   P10 = P(1,0) : exactly one pileup proton in arm 0
	//   P01 = P(0,1) : exactly one pileup proton in arm 1
	//   P1  = P10 + P01
	//
	// Important:
	//   norm_weight[i_reg] = P(2) is kept only for monitoring.
	//   It is NOT the correct proton-tag probability for a one-proton analysis.
	//
	// Critical object-level correction:
	//   If the event is weighted with P1, the final stored proton object must really be a
	//   one-proton object, either (1,0) or (0,1). Therefore, when the code chooses arm 0,
	//   it must sample a data-pool event with p1_xi > 0 and p2_xi = 0. When it chooses arm 1,
	//   it must sample p1_xi = 0 and p2_xi > 0.
	//
	// Why the old version was wrong:
	//   The old code used ptag_wgt = P1 correctly, but then copied poll_p1_xi or poll_p2_xi
	//   from a random pool event without checking that the sampled event had a proton in the
	//   chosen arm. This could produce ptag_wgt = P1 but final p1_xi = 0 and p2_xi = 0.
	//   That is inconsistent for an exactly-one-proton tag.
	// =================================================================================================


	// -------------------------------------------------------------------------------------------------
	// 1) Reset proton-tag quantities.
	//    Do not trust values read from the input MC tree.
	// -------------------------------------------------------------------------------------------------
	ptag_wgt     = 1.0;
	ptag_wgt_err = 0.0;


	// -------------------------------------------------------------------------------------------------
	// 2) Read data-driven proton multiplicity probabilities for this selected era/xangle region.
	//    These probabilities were computed in BLOCK 08 from data.
	// -------------------------------------------------------------------------------------------------
	float P00 = norm_weight_0p[i_reg];      // P(0,0)
	float P10 = norm_weight_1pRP0[i_reg];   // P(1,0)
	float P01 = norm_weight_1pRP1[i_reg];   // P(0,1)
	float P1  = P10 + P01;                  // P(exactly one pileup proton)


	// -------------------------------------------------------------------------------------------------
	// 3) Protect against pathological negative values.
	// -------------------------------------------------------------------------------------------------
	if (P00 < 0.0) P00 = 0.0;
	if (P10 < 0.0) P10 = 0.0;
	if (P01 < 0.0) P01 = 0.0;
	if (P1  < 0.0) P1  = 0.0;


	// -------------------------------------------------------------------------------------------------
	// 4) Store probabilities in output branches.
	//    Pr_2p is stored only for monitoring. It is not used as one-proton tag weight.
	// -------------------------------------------------------------------------------------------------
	Pr_0_0 = P00;
	Pr_1_0 = P10;
	Pr_0_1 = P01;
	Pr_2p  = norm_weight[i_reg];   // P(2), monitoring only


	// -------------------------------------------------------------------------------------------------
	// 5) Determine signal-proton status after PPS efficiency emulation.
	//    At this stage p1_xi or p2_xi may already have been set to zero by the PPS efficiency block.
	// -------------------------------------------------------------------------------------------------
	bool hasSignalProtonArm0 = (p1_xi > 0.0);
	bool hasSignalProtonArm1 = (p2_xi > 0.0);

	int nSignalProtonsAfterEff =
		int(hasSignalProtonArm0) + int(hasSignalProtonArm1);

	if(nSignalProtonsAfterEff == 0) audit_n_after_eff_0++;
	else if(nSignalProtonsAfterEff == 1) audit_n_after_eff_1++;
	else if(nSignalProtonsAfterEff == 2) audit_n_after_eff_2++;
	else audit_n_after_eff_other++;


	// -------------------------------------------------------------------------------------------------
	// 6) Choose the pileup-proton arm if exactly one pileup proton is needed.
	//
	//    The arm is not chosen 50/50.
	//    It is chosen with the conditional probabilities:
	//
	//      P(arm 0 | exactly one pileup proton) = P10 / (P10 + P01)
	//      P(arm 1 | exactly one pileup proton) = P01 / (P10 + P01)
	//
	//    The event weight remains P1 = P10 + P01.
	//    Do NOT change ptag_wgt to P10 or P01, otherwise the arm probability is counted twice.
	// -------------------------------------------------------------------------------------------------
	bool injectArm0 = false;

	if (P1 > 0.0) {
		float r_arm = rand_gen->Rndm();
		injectArm0 = (r_arm < P10 / P1);
	}


	// -------------------------------------------------------------------------------------------------
	// 7) Helper: sample a one-proton pileup event consistent with the chosen arm.
	//
	//    The current pool may contain both one-proton topologies:
	//      (1,0) and (0,1)
	//
	//    Therefore, if the code chooses arm 0, this helper resamples until it finds:
	//      poll_p1_xi > 0 and poll_p2_xi = 0
	//
	//    If the code chooses arm 1, it resamples until it finds:
	//      poll_p1_xi = 0 and poll_p2_xi > 0
	//
	//    This guarantees consistency:
	//
	//      ptag_wgt = P1
	//      <=> final event really contains one proton object.
	//
	//    The helper also updates run and true-zero-track ratios to match the actual sampled
	//    pileup event.
	// -------------------------------------------------------------------------------------------------
	auto sampleOnePileupProtonFromArm = [&](bool wantArm0) -> bool {

		const int maxTries = 10000;

		if (counter_regions[i_reg] <= 0) {
			return false;
		}

		for (int itry = 0; itry < maxTries; ++itry) {

			int i_event_retry = int(rand_gen->Rndm() * counter_regions[i_reg]);
			PUpr[i_reg]->GetEntry(i_event_retry);

			bool hasPUArm0 = (poll_p1_xi[i_reg] > 0.0);
			bool hasPUArm1 = (poll_p2_xi[i_reg] > 0.0);

			// Need exactly one pileup proton in arm 0: (1,0)
			if (wantArm0 && hasPUArm0 && !hasPUArm1) {

				run        = poll_run[i_reg];
				beamXangle = xangle[i_reg % n_xa];

				t1_run_thetax = ptr.trueZeroTracksRatio(run, beamXangle, 1);
				t0_run_thetax = ptr.trueZeroTracksRatio(run, beamXangle, 0);

				return true;
			}

			// Need exactly one pileup proton in arm 1: (0,1)
			if (!wantArm0 && !hasPUArm0 && hasPUArm1) {

				run        = poll_run[i_reg];
				beamXangle = xangle[i_reg % n_xa];

				t1_run_thetax = ptr.trueZeroTracksRatio(run, beamXangle, 1);
				t0_run_thetax = ptr.trueZeroTracksRatio(run, beamXangle, 0);

				return true;
			}
		}

		// No valid pileup proton found after maxTries.
		// This should be rare if the data pool was prepared correctly.
		return false;
	};


	// -------------------------------------------------------------------------------------------------
	// CASE 1: SIGNAL SAMPLE
	// -------------------------------------------------------------------------------------------------
	if (isSignal) {

		// =============================================================================================
		// CASE 1A: Signal has exactly one reconstructed signal proton after PPS efficiency.
		//
		// Michael recommendation:
		//   weight by P(0 pileup protons)
		//
		// Physics:
		//   The event already has the signal proton.
		//   To keep an exactly-one-proton topology, require no additional pileup proton.
		//
		// Final category:
		//   signal_protons = 10  -> true signal proton survived in arm 0
		//   signal_protons = 1   -> true signal proton survived in arm 1
		// =============================================================================================
		if (nSignalProtonsAfterEff == 1) {

			ptag_wgt     = P00;
			ptag_wgt_err = norm_weight_0p_err[i_reg];

			if (hasSignalProtonArm0) {

				// Keep the true signal proton in arm 0.
				// Force the opposite arm to zero to preserve exactly-one-proton topology.
				p2_xi = 0.0;

				signal_protons = 10;
			}
			else if (hasSignalProtonArm1) {

				// Keep the true signal proton in arm 1.
				// Force the opposite arm to zero to preserve exactly-one-proton topology.
				p1_xi = 0.0;

				signal_protons = 1;
			}

			weight *= ptag_wgt;
			sum_weight_after_ptag += weight;
		}


		// =============================================================================================
		// CASE 1B: Signal has no reconstructed signal proton after PPS efficiency.
		//
		// Michael recommendation:
		//   weight by P(1 pileup proton)
		//
		// Physics:
		//   The true signal proton was lost, outside acceptance, or failed PPS efficiency.
		//   The event can still enter the exactly-one-proton category if exactly one pileup
		//   proton is present.
		//
		// Final category:
		//   signal_protons = 2   -> signal proton lost, pileup proton injected in arm 0
		//   signal_protons = 3   -> signal proton lost, pileup proton injected in arm 1
		//   signal_protons = -1  -> failed to find a valid one-proton pileup object
		// =============================================================================================
		else if (nSignalProtonsAfterEff == 0) {

			ptag_wgt     = P1;
			ptag_wgt_err = 0.95;  // 5% flat uncertainty; later converted to |1 - 0.95| = 0.05

			if (P1 <= 0.0) {

				// No probability to inject exactly one pileup proton.
				p1_xi = 0.0;
				p2_xi = 0.0;

				ptag_wgt = 0.0;
				signal_protons = -1;
			}
			else if (injectArm0) {

				// We selected arm 0 according to P10 / (P10 + P01).
				// Now force the sampled pileup object to really be (1,0).
				bool foundValidPU = sampleOnePileupProtonFromArm(true);

				if (foundValidPU) {

					p1_xi = poll_p1_xi[i_reg];
					p2_xi = 0.0;

					signal_protons = 2;
				}
				else {

					// Do not keep a P1-weighted event without a real one-proton object.
					p1_xi = 0.0;
					p2_xi = 0.0;

					ptag_wgt = 0.0;
					signal_protons = -1;
				}
			}
			else {

				// We selected arm 1 according to P01 / (P10 + P01).
				// Now force the sampled pileup object to really be (0,1).
				bool foundValidPU = sampleOnePileupProtonFromArm(false);

				if (foundValidPU) {

					p1_xi = 0.0;
					p2_xi = poll_p2_xi[i_reg];

					signal_protons = 3;
				}
				else {

					// Do not keep a P1-weighted event without a real one-proton object.
					p1_xi = 0.0;
					p2_xi = 0.0;

					ptag_wgt = 0.0;
					signal_protons = -1;
				}
			}

			weight *= ptag_wgt;
		}


		// =============================================================================================
		// CASE 1C: Signal has two reconstructed signal protons after PPS efficiency.
		//
		// For an exactly-one-proton analysis this should not contribute.
		// The event is kept in the tree for debugging, but its proton-tag weight is set to zero.
		//
		// Final category:
		//   signal_protons = 11  -> two true signal protons survived
		// =============================================================================================
		else {

			ptag_wgt     = 0.0;
			ptag_wgt_err = 0.0;

			signal_protons = 11;

			weight *= ptag_wgt;
		}
	}


	// -------------------------------------------------------------------------------------------------
	// CASE 2: BACKGROUND SAMPLE
	// -------------------------------------------------------------------------------------------------
	else {

		// Background has no true signal proton.
		// To enter an exactly-one-proton category, it must receive exactly one pileup proton.
		//
		// Michael recommendation:
		//   background one-proton tag weight = P(1) = P10 + P01
		//
		// Final category:
		//   signal_protons = 0   -> background; final proton comes from pileup
		// -------------------------------------------------------------------------------------------------

		ptag_wgt     = P1;
		ptag_wgt_err = 0.95;  // 5% flat uncertainty; later converted to |1 - 0.95| = 0.05

		if (P1 <= 0.0) {

			// No probability to inject exactly one pileup proton.
			p1_xi = 0.0;
			p2_xi = 0.0;

			ptag_wgt = 0.0;
			signal_protons = 0;
		}
		else if (injectArm0) {

			// We selected arm 0 according to P10 / (P10 + P01).
			// Now force the sampled pileup object to really be (1,0).
			bool foundValidPU = sampleOnePileupProtonFromArm(true);

			if (foundValidPU) {

				p1_xi = poll_p1_xi[i_reg];
				p2_xi = 0.0;

				signal_protons = 0;
			}
			else {

				// Do not keep a P1-weighted background event without a real proton object.
				p1_xi = 0.0;
				p2_xi = 0.0;

				ptag_wgt = 0.0;
				signal_protons = 0;
			}
		}
		else {

			// We selected arm 1 according to P01 / (P10 + P01).
			// Now force the sampled pileup object to really be (0,1).
			bool foundValidPU = sampleOnePileupProtonFromArm(false);

			if (foundValidPU) {

				p1_xi = 0.0;
				p2_xi = poll_p2_xi[i_reg];

				signal_protons = 0;
			}
			else {

				// Do not keep a P1-weighted background event without a real proton object.
				p1_xi = 0.0;
				p2_xi = 0.0;

				ptag_wgt = 0.0;
				signal_protons = 0;
			}
		}

		weight *= ptag_wgt;
	}



	

	// Michal proposal of background
	/*
	else { // no signal protons in the acceptance region
		// Not a signal => Allow injecting "pileup" 
		// or leave as (0,0). Example: inject 1 proton
		if (std::rand() % 2 == 0) {
			// Inject 1 proton into arm 0
			p1_xi = poll_p1_xi[i_reg]; 
			p2_xi = 0;

			// Assign weights for arm 0
			ptag_wgt = norm_weight_1pRP0[i_reg];
			ptag_wgt_err = norm_weight_1pRP0_err[i_reg];
		} else {
			// Inject 1 proton into arm 1
			p1_xi = 0;
			p2_xi = poll_p2_xi[i_reg];

			// Assign weights for arm 1
			ptag_wgt = norm_weight_1pRP1[i_reg];
			ptag_wgt_err = norm_weight_1pRP1_err[i_reg];
		}
		
		weight *= ptag_wgt;

		// Mark as 0 => not a signal
		signal_protons = 0; 
	}
	*/



// =================================================================================================
// BLOCK 16 — FINAL EVENT WEIGHTS, TREE FILLING, AND OUTPUT WRITING
// -------------------------------------------------------------------------------------------------
// Purpose:
//   - Convert ptag uncertainty into 1 ± error format.
//   - Apply nvtx pileup reweighting using data/MC nvtx distributions.
//   - Apply electron trigger scale factors for electron-channel events.
//   - Apply extra signal normalization when processing signal samples.
//   - Fill the enriched MC tree.
//   - Write the final output ROOT file.
//
// Final event weight structure:
//   weight_final = weight_initial
//                × w_region
//                × ptag_wgt
//                × pu_wgt
//                × w_etrig
//                × w_extra_sig.
//
// Physics role:
//   - This block finalizes the event weight used in the downstream cross-section or template analysis.
//   - The output tree contains central MC kinematics plus realistic data-driven pileup proton information.
// =================================================================================================



	// Fix ptag weight from w_sys/w_nom to 1 +/- err
	ptag_wgt_err = TMath::Abs(1 - ptag_wgt_err);

    // puleup reweighting
	float w_mc = mc_pu->GetBinContent(nvtx+1);
	pu_wgt = (w_mc) ? pu_weights[i_reg]->GetBinContent(nvtx+1)/w_mc : 0;
	weight *= pu_wgt;

	sum_weight_after_pu += weight;
	
    //std::cout << "Signal protons: " << signal_protons << ", p1_xi: " << p1_xi << ", p2_xi: " << p2_xi << std::endl;

    // Additional debug info for weights
    //std::cout << "Weight details - ptag_wgt: " << ptag_wgt << ", ptag_wgt_err: " << ptag_wgt_err << ", pu_wgt: " << pu_wgt << std::endl;



	// Electron trigger SF (Run dependent)
	if(cat==4) {  // Add opening brace

		// set electron kinematics
		std::vector<Particle> leptons={};
		TLorentzVector lp4;
		lp4.SetPtEtaPhiM(lep_pt, lep_eta, lep_phi, lep_m);
		leptons.push_back(Particle(lp4, 1, 11, 1, 0, 1.0, 0));

		EffCorrection_t trigSF;
		int era_i = i_reg / 4;

		if (0 == era_i) trigSF = lepEffH_eraB.getTriggerCorrection(leptons, {}, {}, "");
		if (1 == era_i) trigSF = lepEffH_eraC.getTriggerCorrection(leptons, {}, {}, "");
		if (2 == era_i) trigSF = lepEffH_eraD.getTriggerCorrection(leptons, {}, {}, "");
		if (3 == era_i) trigSF = lepEffH_eraE.getTriggerCorrection(leptons, {}, {}, "");
		if (4 == era_i) trigSF = lepEffH_eraF.getTriggerCorrection(leptons, {}, {}, "");  // Fixed typo here

		triggerSF = trigSF.first;
		triggerSF_err = trigSF.second;

		w_etrig = triggerSF;

		// update event weight
		weight *= w_etrig;
	}
	
	else {
		w_etrig = 1.0;
	}

	sum_weight_after_etrig += weight;


	// Add extra weight to signal since we have simulation for each era/xangle
	if(isSignal) {
		w_extra_sig = extra_signal_normalization;
		weight *= w_extra_sig;
	} else {
		w_extra_sig = 1.0;
	}

	sum_weight_final += weight;

	// ===============================================================================================
	// Independent audit calculated from stored per-event factors.
	// This should reproduce the final event weight:
	//
	//   weight_initial_event
	//   * w_region
	//   * ptag_wgt
	//   * pu_wgt
	//   * w_etrig
	//   * w_extra_sig
	// ===============================================================================================

	double audit_w_initial      = weight_initial_event;
	double audit_w_region       = audit_w_initial * double(w_region);
	double audit_w_ptag         = audit_w_region  * double(ptag_wgt);
	double audit_w_pu           = audit_w_ptag    * double(pu_wgt);
	double audit_w_etrig        = audit_w_pu      * double(w_etrig);
	double audit_w_final        = audit_w_etrig   * double(w_extra_sig);

	audit_sum_initial      += audit_w_initial;
	audit_sum_after_region += audit_w_region;
	audit_sum_after_ptag   += audit_w_ptag;
	audit_sum_after_pu     += audit_w_pu;
	audit_sum_after_etrig  += audit_w_etrig;
	audit_sum_final        += audit_w_final;

	double diff_final = TMath::Abs(double(weight) - audit_w_final);
	audit_abs_diff_final += diff_final;
	if(diff_final > audit_max_diff_final) audit_max_diff_final = diff_final;

	if(ptag_wgt == 0.0) audit_n_ptag_zero++;
	if(pu_wgt == 0.0) audit_n_pu_zero++;
	if(weight == 0.0) audit_n_final_zero_weight++;

	double prob_norm = double(Pr_0_0) + double(Pr_1_0) + double(Pr_0_1) + double(Pr_2p);
	audit_sum_prob_norm += prob_norm;

	double abs_prob_norm_minus_1 = TMath::Abs(prob_norm - 1.0);
	if(abs_prob_norm_minus_1 > audit_max_abs_prob_norm_minus_1)
		audit_max_abs_prob_norm_minus_1 = abs_prob_norm_minus_1;

	if(signal_protons == -99) {
		audit_sum_ptag_cat_m99 += audit_w_ptag;
		audit_sum_final_cat_m99 += audit_w_final;
	}
	else if(signal_protons == -1) {
		audit_sum_ptag_cat_m1 += audit_w_ptag;
		audit_sum_final_cat_m1 += audit_w_final;
	}
	else if(signal_protons == 0) {
		audit_sum_ptag_cat_0 += audit_w_ptag;
		audit_sum_final_cat_0 += audit_w_final;
	}
	else if(signal_protons == 1) {
		audit_sum_ptag_cat_1 += audit_w_ptag;
		audit_sum_final_cat_1 += audit_w_final;
	}
	else if(signal_protons == 2) {
		audit_sum_ptag_cat_2 += audit_w_ptag;
		audit_sum_final_cat_2 += audit_w_final;
	}
	else if(signal_protons == 3) {
		audit_sum_ptag_cat_3 += audit_w_ptag;
		audit_sum_final_cat_3 += audit_w_final;
	}
	else if(signal_protons == 10) {
		audit_sum_ptag_cat_10 += audit_w_ptag;
		audit_sum_final_cat_10 += audit_w_final;
	}
	else if(signal_protons == 11) {
		audit_sum_ptag_cat_11 += audit_w_ptag;
		audit_sum_final_cat_11 += audit_w_final;
	}
	else {
		audit_sum_ptag_cat_other += audit_w_ptag;
		audit_sum_final_cat_other += audit_w_final;
	}

	bool has_p1_final = (p1_xi > 0);
	bool has_p2_final = (p2_xi > 0);

	if(!has_p1_final && !has_p2_final) n_topo_final_00++;
	if( has_p1_final && !has_p2_final) n_topo_final_10++;
	if(!has_p1_final &&  has_p2_final) n_topo_final_01++;
	if( has_p1_final &&  has_p2_final) n_topo_final_11++;

	if(signal_protons == -99) {
		n_sig_cat_m99++;
		sum_weight_cat_m99 += weight;
	}
	else if(signal_protons == -1) {
		n_sig_cat_m1++;
		sum_weight_cat_m1 += weight;
	}
	else if(signal_protons == 0) {
		n_sig_cat_0++;
		sum_weight_cat_0 += weight;
	}
	else if(signal_protons == 1) {
		n_sig_cat_1++;
		sum_weight_cat_1 += weight;
	}
	else if(signal_protons == 2) {
		n_sig_cat_2++;
		sum_weight_cat_2 += weight;
	}
	else if(signal_protons == 3) {
		n_sig_cat_3++;
		sum_weight_cat_3 += weight;
	}
	else if(signal_protons == 10) {
		n_sig_cat_10++;
		sum_weight_cat_10 += weight;
	}
	else if(signal_protons == 11) {
		n_sig_cat_11++;
		sum_weight_cat_11 += weight;
	}
	else {
		n_sig_cat_other++;
		sum_weight_cat_other += weight;
	}

	sum_w_region    += w_region;
	sum_ptag_wgt    += ptag_wgt;
	sum_pu_wgt      += pu_wgt;
	sum_w_etrig     += w_etrig;
	sum_w_extra_sig += w_extra_sig;

	sum_Pr_0_0 += Pr_0_0;
	sum_Pr_1_0 += Pr_1_0;
	sum_Pr_0_1 += Pr_0_1;
	sum_Pr_2p  += Pr_2p;
	   
	// add extra weight in case of signal events:
    //pup_2tkRatio = ptr.twoTracksRatio(run, beamXangle);
    //for (int iArm = 0; iArm < 2; iArm++) {
      //pup_0tkRatio[iArm] = ptr.zeroTracksRatio(run, beamXangle, iArm);
      //pup_true0tkRatio[iArm] = ptr.trueZeroTracksRatio(run, beamXangle, iArm);
    //}
    tMCMixed->Fill();
  }


  cout << "\nDone. " << iMCEntry << " events processed." << endl;
  timed=time(NULL);
  times=timed-times;
  cout << "time from start to end = " << times << endl;

  double n_proc = (iMCEntry > 0) ? double(iMCEntry) : 1.0;

  cout << "\n======================================================================" << endl;
  cout << "FINAL COMPACT VALIDATION SUMMARY" << endl;
  cout << "======================================================================" << endl;

  cout << "Input MC file              : " << inMCFileName << endl;
  cout << "Output ROOT file           : " << outFileName << endl;
  cout << "isSignal                   : " << isSignal << endl;
  cout << "events processed           : " << iMCEntry << endl;
  cout << "n_PUregions                : " << n_PUregions << endl;
  cout << "extra_signal_normalization : " << extra_signal_normalization << endl;

  cout << "\nRegion table:" << endl;
  cout << "i_reg  era     xangle  counter_regions  fraction_regions  w_region_if_sampled  P00       P10       P01       P2p" << endl;
  for(int i=0; i<n_PUregions; i++){
	  int iera = i / n_xa;
	  int ixa  = i % n_xa;
	  cout << i << "      "
	       << era[iera] << "  "
	       << xangle[ixa] << "     "
	       << counter_regions[i] << "             "
	       << fraction_regions[i] << "          "
	       << fraction_regions[i]*float(n_PUregions) << "             "
	       << norm_weight_0p[i] << "  "
	       << norm_weight_1pRP0[i] << "  "
	       << norm_weight_1pRP1[i] << "  "
	       << norm_weight[i] << endl;
  }

  cout << "\nSampled regions:" << endl;
  cout << "i_reg  N_sampled" << endl;
  for(int i=0; i<n_PUregions; i++){
	  cout << i << "      " << n_sampled_region[i] << endl;
  }

  cout << "\nTopology before / final:" << endl;
  cout << "topology   before   final" << endl;
  cout << "(0,0)      " << n_topo_before_00 << "   " << n_topo_final_00 << endl;
  cout << "(1,0)      " << n_topo_before_10 << "   " << n_topo_final_10 << endl;
  cout << "(0,1)      " << n_topo_before_01 << "   " << n_topo_final_01 << endl;
  cout << "(1,1)      " << n_topo_before_11 << "   " << n_topo_final_11 << endl;

  cout << "\nsignal_protons categories:" << endl;
  cout << "category   N_events   audit_sum_after_ptag   audit_sum_final   meaning" << endl;
  cout << "-99        " << n_sig_cat_m99   << "   " << audit_sum_ptag_cat_m99   << "   " << audit_sum_final_cat_m99   << "   unset/problem" << endl;
  cout << "-1         " << n_sig_cat_m1    << "   " << audit_sum_ptag_cat_m1    << "   " << audit_sum_final_cat_m1    << "   invalid/no replacement" << endl;
  cout << "0          " << n_sig_cat_0     << "   " << audit_sum_ptag_cat_0     << "   " << audit_sum_final_cat_0     << "   background pileup proton" << endl;
  cout << "1          " << n_sig_cat_1     << "   " << audit_sum_ptag_cat_1     << "   " << audit_sum_final_cat_1     << "   true signal proton survived arm 1" << endl;
  cout << "2          " << n_sig_cat_2     << "   " << audit_sum_ptag_cat_2     << "   " << audit_sum_final_cat_2     << "   signal lost, pileup injected arm 0" << endl;
  cout << "3          " << n_sig_cat_3     << "   " << audit_sum_ptag_cat_3     << "   " << audit_sum_final_cat_3     << "   signal lost, pileup injected arm 1" << endl;
  cout << "10         " << n_sig_cat_10    << "   " << audit_sum_ptag_cat_10    << "   " << audit_sum_final_cat_10    << "   true signal proton survived arm 0" << endl;
  cout << "11         " << n_sig_cat_11    << "   " << audit_sum_ptag_cat_11    << "   " << audit_sum_final_cat_11    << "   two signal protons survived, zero weight" << endl;
  cout << "other      " << n_sig_cat_other << "   " << audit_sum_ptag_cat_other << "   " << audit_sum_final_cat_other << "   unexpected category" << endl;

  cout << "\nWeighted cut-flow audit:" << endl;
  cout << "stage              audit_sum_weight        relative_to_initial" << endl;
  cout << "initial            " << audit_sum_initial      << "        " << 1.0 << endl;
  cout << "after_region       " << audit_sum_after_region << "        " << (audit_sum_initial != 0 ? audit_sum_after_region/audit_sum_initial : 0.0) << endl;
  cout << "after_ptag         " << audit_sum_after_ptag   << "        " << (audit_sum_initial != 0 ? audit_sum_after_ptag/audit_sum_initial : 0.0) << endl;
  cout << "after_pu           " << audit_sum_after_pu     << "        " << (audit_sum_initial != 0 ? audit_sum_after_pu/audit_sum_initial : 0.0) << endl;
  cout << "after_etrig        " << audit_sum_after_etrig  << "        " << (audit_sum_initial != 0 ? audit_sum_after_etrig/audit_sum_initial : 0.0) << endl;
  cout << "final              " << audit_sum_final        << "        " << (audit_sum_initial != 0 ? audit_sum_final/audit_sum_initial : 0.0) << endl;

  cout << "\nInternal cut-flow counters currently stored in code:" << endl;
  cout << "stage              stored_sum_weight        stored/audit" << endl;
  cout << "initial            " << sum_weight_initial      << "        " << (audit_sum_initial != 0 ? sum_weight_initial/audit_sum_initial : 0.0) << endl;
  cout << "after_region       " << sum_weight_after_region << "        " << (audit_sum_after_region != 0 ? sum_weight_after_region/audit_sum_after_region : 0.0) << endl;
  cout << "after_ptag         " << sum_weight_after_ptag   << "        " << (audit_sum_after_ptag != 0 ? sum_weight_after_ptag/audit_sum_after_ptag : 0.0) << endl;
  cout << "after_pu           " << sum_weight_after_pu     << "        " << (audit_sum_after_pu != 0 ? sum_weight_after_pu/audit_sum_after_pu : 0.0) << endl;
  cout << "after_etrig        " << sum_weight_after_etrig  << "        " << (audit_sum_after_etrig != 0 ? sum_weight_after_etrig/audit_sum_after_etrig : 0.0) << endl;
  cout << "final              " << sum_weight_final        << "        " << (audit_sum_final != 0 ? sum_weight_final/audit_sum_final : 0.0) << endl;

  cout << "\nMean weight/probability checks:" << endl;
  cout << "mean w_region      : " << sum_w_region/n_proc << endl;
  cout << "mean ptag_wgt      : " << sum_ptag_wgt/n_proc << endl;
  cout << "mean pu_wgt        : " << sum_pu_wgt/n_proc << endl;
  cout << "mean w_etrig       : " << sum_w_etrig/n_proc << endl;
  cout << "mean w_extra_sig   : " << sum_w_extra_sig/n_proc << endl;
  cout << "mean P00           : " << sum_Pr_0_0/n_proc << endl;
  cout << "mean P10           : " << sum_Pr_1_0/n_proc << endl;
  cout << "mean P01           : " << sum_Pr_0_1/n_proc << endl;
  cout << "mean P1=P10+P01    : " << (sum_Pr_1_0 + sum_Pr_0_1)/n_proc << endl;
  cout << "mean P2p           : " << sum_Pr_2p/n_proc << endl;

  cout << "\nAdditional audit checks:" << endl;
  cout << "nSignalProtonsAfterEff = 0 : " << audit_n_after_eff_0 << endl;
  cout << "nSignalProtonsAfterEff = 1 : " << audit_n_after_eff_1 << endl;
  cout << "nSignalProtonsAfterEff = 2 : " << audit_n_after_eff_2 << endl;
  cout << "nSignalProtonsAfterEff other : " << audit_n_after_eff_other << endl;

  cout << "events with ptag_wgt == 0       : " << audit_n_ptag_zero << endl;
  cout << "events with pu_wgt == 0         : " << audit_n_pu_zero << endl;
  cout << "events with final weight == 0   : " << audit_n_final_zero_weight << endl;

  cout << "mean(P00+P10+P01+P2p)           : " << audit_sum_prob_norm/n_proc << endl;
  cout << "max abs(Psum - 1)               : " << audit_max_abs_prob_norm_minus_1 << endl;

  cout << "sum abs(weight - audit_final)   : " << audit_abs_diff_final << endl;
  cout << "max abs(weight - audit_final)   : " << audit_max_diff_final << endl;

  cout << "closure final/audit_final       : " << (audit_sum_final != 0 ? sum_weight_final/audit_sum_final : 0.0) << endl;
  cout << "closure stored_final/audit_final should be close to 1." << endl;

  cout << "======================================================================" << endl;
	
  // Write mixed tree to file and close everything






  fOutMixed->cd();
  cout << "Writes " << fOutMixed->GetName() << endl;
  tMCMixed->Write();
  if(nMCEventsToSkip==0) evt_count->Write();
  fOutMixed->Close();
  
  return iMCEntry;

}