#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include <fstream>

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TDirectory.h>
#include <TBranch.h>
#include <TMath.h>

using namespace std;

class Selection{
	
	public:
	Selection(){
		_weight = "weight";
		user_lumi = 29380.666084;  // in ipb
		signal_name = "excl_tt"; nsignal = 1;
		eras_str = {"2017B","2017C","2017D","2017E","2017F"};
	};
	~Selection(){};
	int init();
	void process(int,int);
	
	// Initialization functions
	//int GetNRegions(){return _nregions;}
	//TString GetRegionName(int r){return isInit ? region_names.at(r) : "";}
	//int GetNsyst_shape(){return weight_syst_names.size();}
	//int GetNsyst_norm(){return 0;}
	void SetRange(int n,float x1,float x2){_NBins=n;_Xmin=x1;_Xmax=x2;_binWidth=(x2-x1)/float(n);}
	void setSysWeight(TString name,TString sys){weight_syst_names.push_back(name);syst_weights.push_back(sys);}
	void setSysSample(TString name,TString sys){sample_syst_names.push_back(name);syst_samples.push_back(sys);}
	void AddBkg(TString _n, std::vector<TString> _bkg, std::vector<float>_xs){bkg_name.push_back(_n); AllBKG.push_back(_bkg); BKG_XSEC.push_back(_xs);}
	void AddSig(TString _bkg, float _xs){sig_samples.push_back(_bkg); sig_xsec.push_back(_xs);}
	void AddData(TString _bkg){data_samples.push_back(_bkg);}
	void SetMCPath(TString path){_mc_path = path;}
	void SetMCSystPath(TString path){_mcsyst_path = path;}
	void SetDataPath(TString path){_data_path = path;}
	void SaveAs(TString,int);
	vector<TString> eras_str;
	
	private:
	int _nregions = 4; // 4 crossing-angles  = minimal number of regions
	bool isInit =false, withSyst=false;
	int _NBins, nsys_weights, nsys_samples, nbackgrounds, nsignal;
	float _Xmin, _Xmax, _binWidth, user_lumi;
	vector<TString> weight_syst_names, sample_syst_names, syst_weights, syst_samples;
	vector<vector<TH1F*>> shapes_nom; vector<TH1F*> shapes_data;
	vector<vector<vector<TH1F*>>> shapes_varUp;
	vector<vector<vector<TH1F*>>> shapes_varDn;
	vector<TString> region_names;
	std::vector<std::vector<float> > BKG_XSEC; std::vector<std::vector<TString> > AllBKG; vector<TString> bkg_name;
	std::vector<float> sig_xsec; vector<TString> sig_samples, data_samples;
	int regionIdx(int, float);
	void processSample(TString, float, int, int);
	void processData(TString);
	TString BackgroundName(int i){return (i<(int)bkg_name.size()) ? bkg_name.at(i) : "NULL";}
	TString _mc_path="", _data_path="", _mcsyst_path="", _weight, signal_name;
   
};
	

int main(int argc, char* argv[])
{ 
  
  int mode = 1; // mode = 0 blinded, 1 unblinded , 2 hybrid
  int processIdx=-1; // -1:all, 0:data, 1:signal, 2-...: backgrounds
  // Check arguments
  if (argc > 2) {
    cout << "up to one arguments required!" << endl
         << "Usage: " << argv[0] << " [mode]"<< endl;
    return 1;
  }
  else if(argc == 2) processIdx = atoi(argv[1]);
  
  Selection sel;
  sel.SetRange(22,-0.43,0.23); // Discriminator histogram range default (22,-0.43,0.23) 
   
  // Set signal cross-section (usually to 1pb to set the limit)
  float signal_xsec = 1 /* pb */; // 1.345354232E-004


  if(processIdx==2){
  const char* PWD = getenv("PWD");
  sel.SetMCPath(PWD);
  sel.SetMCSystPath(PWD);
  }
  else{
  sel.SetMCPath("/eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/mc/MVA");
  sel.SetMCSystPath("/eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/mc/MVA/syst");
  //sel.SetMCPath("/eos/user/m/mpitt/PPS_ttbar/MC/ntuples_forMatteo/MVA");
  //sel.SetMCSystPath("/eos/user/m/mpitt/PPS_ttbar/MC/ntuples_forMatteo/MVA");
  } 

  //sel.SetDataPath("/eos/user/m/mpitt/PPS_ttbar/Data/ntuples_forMatteo/MVA");
  sel.SetDataPath("/eos/cms/store/group/phys_top/TTbarCentralExclProd/ntuples/data/MVA");
  
  // List of samples
  std::vector<float> ttbar_xsec, wjets_xsec, zjets_xsec, stop_xsec, diboson_xsec;
  std::vector<TString> ttbar, wjets, zjets, stop, diboson; 
  std::vector<TString> exclusive_ttbar;
  std::vector<TString> data;  

  // ttbar
  //ttbar.push_back("TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8.root"); ttbar_xsec.push_back(832.0);
  ttbar.push_back("TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8.root"); ttbar_xsec.push_back(365.34);
  ttbar.push_back("TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8.root"); ttbar_xsec.push_back(88.29);

  
  // Single top
  stop.push_back("ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8.root"); stop_xsec.push_back(35.85);
  stop.push_back("ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8.root"); stop_xsec.push_back(35.85);

  // W+jets sample
  wjets.push_back("W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root"); wjets_xsec.push_back(2793.0);
  wjets.push_back("W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root"); wjets_xsec.push_back(992.5);
  wjets.push_back("W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root"); wjets_xsec.push_back(544.3);

  //Z+jets
  zjets.push_back("DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root"); zjets_xsec.push_back(304.4);
  zjets.push_back("DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root"); zjets_xsec.push_back(111.5);
  zjets.push_back("DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root"); zjets_xsec.push_back(44.03);

  //diboson
  diboson.push_back("WW_TuneCP5_13TeV-pythia8.root"); diboson_xsec.push_back(111.5);
  diboson.push_back("WZ_TuneCP5_13TeV-pythia8.root"); diboson_xsec.push_back(27.57);
  
  // set weights (histname, branch name)
  
  sel.setSysWeight("l1prefire","L1Prefire_wgt_err"); // L1Prefire
  sel.setSysWeight("MUtrigSF","MU_trigSF_wgt_err"); // muon trigger SF
  sel.setSysWeight("ELtrigSF","EL_trigSF_wgt_err"); // electron trigger SF
  sel.setSysWeight("msel","MU_SF_wgt_err"); // muon selection SF
  sel.setSysWeight("esel","EL_SF_wgt_err"); // electron selection SF
  sel.setSysWeight("protonTag","ptag_wgt_err"); // proton tagging uncertainty
  sel.setSysWeight("ppsSF","ppsSF_wgt_err"); // proton SF uncertainty
  sel.setSysWeight("pdf_as","pdf_as_err"); // PDF variation (coupling constant)
  sel.setSysWeight("pdf_hs","pdf_hs_err"); // PDF variation (hessian matrix)
  sel.setSysWeight("scale_REN","ren_err"); // renormalization scale variation
  sel.setSysWeight("scale_FAC","fac_err"); // factorization scale variation
  //sel.setSysWeight("toppt","toppt_wgt"); // top pT reweighting [SPECIAL TREATMENT]
  //sel.setSysWeight("PU_unc","pu_wgt"); // top pT reweighting [SPECIAL TREATMENT]
  
  // ISR FSR
  sel.setSysWeight("ISR_G2GG_muR","isr1_"); // ISR variation - asymmetric
  sel.setSysWeight("ISR_G2QQ_muR","isr2_"); // ISR variation - asymmetric
  sel.setSysWeight("ISR_Q2QG_muR","isr3_"); // ISR variation - asymmetric
  sel.setSysWeight("ISR_X2XG_muR","isr4_"); // ISR variation - asymmetric
  sel.setSysWeight("ISR_G2GG_cNS","isr5_"); // ISR variation - asymmetric
  sel.setSysWeight("ISR_G2QQ_cNS","isr6_"); // ISR variation - asymmetric
  sel.setSysWeight("ISR_Q2QG_cNS","isr7_"); // ISR variation - asymmetric
  sel.setSysWeight("ISR_X2XG_cNS","isr8_"); // ISR variation - asymmetric
  sel.setSysWeight("FSR_G2GG_muR","fsr1_"); // FSR variation - asymmetric
  sel.setSysWeight("FSR_G2QQ_muR","fsr2_"); // FSR variation - asymmetric
  sel.setSysWeight("FSR_Q2QG_muR","fsr3_"); // FSR variation - asymmetric
  sel.setSysWeight("FSR_X2XG_muR","fsr4_"); // FSR variation - asymmetric
  sel.setSysWeight("FSR_G2GG_cNS","fsr5_"); // FSR variation - asymmetric
  sel.setSysWeight("FSR_G2QQ_cNS","fsr6_"); // FSR variation - asymmetric
  sel.setSysWeight("FSR_Q2QG_cNS","fsr7_"); // FSR variation - asymmetric
  sel.setSysWeight("FSR_X2XG_cNS","fsr8_"); // FSR variation - asymmetric
  
  // set alternative samples (histname, sample sufix)
  sel.setSysSample("JEC_Absolute","JECABS"); // JEC
  sel.setSysSample("JEC_Flavor","JECFLAV"); // JEC
  sel.setSysSample("JEC_HighPt","JECHIGHPT"); // JEC
  sel.setSysSample("JEC_Pileup","JECPILEUP"); // JEC
  sel.setSysSample("JEC_Relative","JECREL"); // JEC
  sel.setSysSample("JEC_Time","JECTIME"); // JEC
  sel.setSysSample("JER","JER"); // JER
  sel.setSysSample("btaghf","BTAGHF"); // b-tagging HF SF uncertainty
  sel.setSysSample("btaglf","BTAGLF"); // b-tagging light quark SF uncertainty
  sel.setSysSample("UnclusteredMET","UNCLUSTEREDEN"); // b-tagging light quark SF uncertainty

  sel.setSysSample("PPS45_Reco","PPS45"); // proton reconstruction
  sel.setSysSample("PPS56_Reco","PPS56");

  // Add backgrounds
  sel.AddBkg("ttbar",ttbar,ttbar_xsec);
  sel.AddBkg("Wt",stop,stop_xsec);
  sel.AddBkg("Wjets",wjets,wjets_xsec);
  sel.AddBkg("Zjets",zjets,zjets_xsec);
  //sel.AddBkg("VV",diboson,diboson_xsec);
    
  // Add data
  if(mode){ 
	for(unsigned int i=0;i<sel.eras_str.size();i++){
	for(int j=120;j<151;j+=10){
		sel.AddData(Form("SingleElectron_%s_xa%d.root",sel.eras_str.at(i).Data(),j));
		sel.AddData(Form("SingleMuon_%s_xa%d.root",sel.eras_str.at(i).Data(),j));
    }}
  }
    
  // Add signal samples
  for(int j=120;j<151;j+=10){
	// era B, C, D
    sel.AddSig(Form("ExclusiveTTbar_SemiLep_TuneCP5_13TeV-FPMC-madspin-pythia8_xa%d_era2017_preTS2.root",j),signal_xsec * 0.421);
    sel.AddSig(Form("ExclusiveTTbar_LepLep_TuneCP5_13TeV-FPMC-madspin-pythia8_xa%d_era2017_preTS2.root",j),signal_xsec * 0.11085);
	// era E and F
    sel.AddSig(Form("ExclusiveTTbar_SemiLep_TuneCP5_13TeV-FPMC-madspin-pythia8_xa%d_era2017_postTS2.root",j),signal_xsec * 0.421);
    sel.AddSig(Form("ExclusiveTTbar_LepLep_TuneCP5_13TeV-FPMC-madspin-pythia8_xa%d_era2017_postTS2.root",j),signal_xsec * 0.11085);
  }
  
  // Initialize
  if(!sel.init()) return 0;
  
  // Process samples
  sel.process(mode,processIdx);
  
  // Save output
  sel.SaveAs("sl-shapes",processIdx);

  // Done
  return 1;  

}

int Selection::init(){
	std::cout << "Initializing:"<<endl;
	if(_mc_path==""){std::cout<<"ERROR: mc path not set"<<std::endl; return isInit;}
	if(_data_path==""){std::cout<<"ERROR: data path not set"<<std::endl; return isInit;}
	nbackgrounds = AllBKG.size();
	nsys_weights = weight_syst_names.size();
	nsys_samples = sample_syst_names.size();
	_nregions *= eras_str.size(); 	// update number of regions
	std::cout << "Total " << _nregions << "SR with "<<nbackgrounds << " backgrounds, with "
	<< nsys_weights << " systematics related to weights "
	<< nsys_samples << " systematics related to different samples "
	<< endl;
	

	// loop over all signal regions (different periods and crossing angles):
	for(int ir=0;ir<_nregions;ir++){
		
		// set names for the signal regions (Bin cannot start with a digit!)
		region_names.push_back(Form("SR%s%d",eras_str.at(ir/4).Data(),(ir%4)*10+120));
		
		// define data histograms:
		shapes_data.push_back(new TH1F("data_obs_"+region_names.at(ir),"",_NBins,_Xmin,_Xmax));

		// Set vectors:
		vector<TH1F*> _shapes_nom;
		vector<vector<TH1F*>> _shapes_varUp, _shapes_varDn;

		// define signal histograms
		vector<TH1F*> sig_sys_up; vector<TH1F*> sig_sys_dn;
		_shapes_nom.push_back(new TH1F(signal_name+"_"+region_names.at(ir),"",_NBins,_Xmin,_Xmax));
		for(int is=0;is<nsys_weights;is++){
			TString name_sys = signal_name+"_"+region_names.at(ir)+"_"+weight_syst_names.at(is);
			sig_sys_up.push_back(new TH1F(name_sys+"Up","",_NBins,_Xmin,_Xmax));
			sig_sys_dn.push_back(new TH1F(name_sys+"Down","",_NBins,_Xmin,_Xmax));
		}
		for(int is=0;is<nsys_samples;is++){
			TString name_sys = signal_name+"_"+region_names.at(ir)+"_"+sample_syst_names.at(is);
			sig_sys_up.push_back(new TH1F(name_sys+"Up","",_NBins,_Xmin,_Xmax));
			sig_sys_dn.push_back(new TH1F(name_sys+"Down","",_NBins,_Xmin,_Xmax));	
		}		
		_shapes_varUp.push_back(sig_sys_up);
		_shapes_varDn.push_back(sig_sys_dn);
		
		// loop over all backgounds and set histograms
		for(int ih=0;ih<nbackgrounds;ih++){
			TH1F * hist = new TH1F(bkg_name.at(ih)+"_"+region_names.at(ir),"",_NBins,_Xmin,_Xmax);
			_shapes_nom.push_back(hist);
			vector<TH1F*> sys_up; vector<TH1F*> sys_dn;
			for(int is=0;is<nsys_weights;is++){ // weight systematics
			  TString name_sys = bkg_name.at(ih)+"_"+region_names.at(ir)+"_"+weight_syst_names.at(is);
			  TH1F * histup = new TH1F(name_sys+"Up","",_NBins,_Xmin,_Xmax); sys_up.push_back(histup);
			  TH1F * histdn = new TH1F(name_sys+"Down","",_NBins,_Xmin,_Xmax); sys_dn.push_back(histdn);
			}
			for(int is=0;is<nsys_samples;is++){ // systematics due to different samples
			  TH1F * histup = new TH1F(bkg_name.at(ih)+"_"+region_names.at(ir)+"_"+sample_syst_names.at(is)+"Up","",_NBins,_Xmin,_Xmax); sys_up.push_back(histup);
			  TH1F * histdn = new TH1F(bkg_name.at(ih)+"_"+region_names.at(ir)+"_"+sample_syst_names.at(is)+"Down","",_NBins,_Xmin,_Xmax); sys_dn.push_back(histdn);
			}			
			_shapes_varUp.push_back(sys_up);
			_shapes_varDn.push_back(sys_dn);
		}
		
		// push_back histograms:
		shapes_nom.push_back(_shapes_nom);
		shapes_varUp.push_back(_shapes_varUp);
		shapes_varDn.push_back(_shapes_varDn);
	}
	isInit=true;
	std::cout << "Done"<<endl;
    return isInit;
}

void Selection::SaveAs(TString file, int processIdx){
	cout << "call SaveAs:" << endl;
	if(processIdx==0) file+="_data.root";
	else if(processIdx==1) file+="_signal.root";
	else if(processIdx>1) file+=("_"+BackgroundName(processIdx-2)+".root");
    else file+=".root";
	
	TFile * outfile = TFile::Open(file,"RECREATE");
	for(int ir=0;ir<_nregions;ir++){
		// save data 
		if(processIdx<1) shapes_data.at(ir)->Write();
		if(processIdx==0) continue;
		
		// save MC predictions			
		for(int ih=0;ih<(nbackgrounds+nsignal);ih++){
			if(processIdx>0 && processIdx!=(ih+1)) continue;
			shapes_nom.at(ir).at(ih)->Write();
			for(int is=0;is<(nsys_samples+nsys_weights);is++){
			  shapes_varUp.at(ir).at(ih).at(is)->Write();
			  shapes_varDn.at(ir).at(ih).at(is)->Write();
			}
		}
	} 
	cout << "Writes " << outfile->GetName() << endl;
	outfile->Write();
	outfile->Close();
	
}


void Selection::process(int _mode, int processIdx){
	std::cout << "Processing";
	if(_mode==1) std::cout << " the data unblinded"<<std::endl;
	else if(_mode==2) std::cout << " the data with hybrid sample"<<std::endl;
	else std::cout << " only MC"<<std::endl;
	
	TString ds; std::vector<TString> v_ds; std::vector<float> v_xsec; float xsec;	
	if(processIdx<0 || processIdx==1){
	cout << "process Signal"<<endl;
	// Process signal samples:
	for(unsigned int j=0;j<sig_samples.size();j++){ 
		
		  ds = sig_samples.at(j); xsec = sig_xsec.at(j); 
		  processSample(ds, xsec, 0 /* 0 for signal */, 2 /*SumAODbin*/);

	}
    } // end processIdx
	if(processIdx<0 || processIdx>1){
	cout << "process backgounds"<<endl;
	// Process all background samples:
	for(unsigned int i=0;i<AllBKG.size();i++){
	if(processIdx>1 && processIdx!=(int)(i+2)) continue;
    cout << "Obtain histograms for \"" << bkg_name.at(i) <<"\""<< endl;
		v_ds = AllBKG.at(i); v_xsec = BKG_XSEC.at(i); // loop over all samples:
	    for(unsigned int j=0;j<v_ds.size();j++){ 
		
		  ds = v_ds.at(j); xsec = v_xsec.at(j); 
		  processSample(ds, xsec, i+1 /* bkg histogram index */, 2 /*SumAODbin*/);

	}}
	} // end processIdx
	// process data / pseudo-data
	if(processIdx<1){
	if(_mode){
		
	cout << "process data"<<endl;
		for(unsigned int i=0;i<data_samples.size();i++){
			ds = data_samples.at(i);
			processData(ds);
		}
		
		if(_mode==2){ // replace significant bins by MC predictions
		for(int ibin=(_NBins/2);ibin<_NBins;ibin++){
		for(int ir=0;ir<_nregions;ir++){
		float r = 0;
		for(unsigned int i=1 /*skip signal*/;i<shapes_nom.at(ir).size();i++){
			r += shapes_nom.at(ir).at(i)->GetBinContent(ibin+1);
		}
		shapes_data.at(ir)->SetBinContent(ibin+1,r);
		}}}
	}
	else { // replace the data by sum of background components
		cout << "sum mu*s+b to mimic data (for statistical testing)"<<endl;
		for(int ir=0;ir<_nregions;ir++){
		  for(unsigned int i=1 /*skip signal*/;i<shapes_nom.at(ir).size();i++){
			shapes_data.at(ir)->Add(shapes_nom.at(ir).at(i));
		  }
		  // set Nevents to integer
		  for(int ibin=0;ibin<_NBins;ibin++){
			int r = round(shapes_data.at(ir)->GetBinContent(ibin+1));
			shapes_data.at(ir)->SetBinContent(ibin+1,r);
		  }
		}
	}
	} // end processIdx
}

int Selection::regionIdx(int run, float xangle){
	int xa = int(xangle)/10 - 12;
	if(run<297050) return -1;
    if(run<=299329) return xa;    //era2017B
    if(run<=300780) return 4+xa;  //era2017C1
    if(run<=302029) return 4+xa;  //era2017C2
    if(run<=302663) return 8+xa;  //era2017D
    if(run<=304797) return 12+xa; //era2017E
    if(run<=305114) return 16+xa; //era2017F1
    if(run<=305902) return 16+xa; //era2017F2
    if(run<=306460) return 16+xa; //era2017F3
	return -1;

}


void Selection::processSample(TString ds, float xsec, int component_idx, int SumAODbin){
	
    // load the file and get normalization 
	TFile * _file = TFile::Open(_mc_path+"/"+ds);
	if(!_file) {cout << "ERROR with " << ds << endl; return;}
	if(!_file->Get("evt_count")) {cout << "ERROR with evt_count in " << (_mc_path+"/"+ds) << endl; return;}
	float norm=((TH1F*)_file->Get("evt_count"))->GetBinContent(SumAODbin); 
	cout << "Read " << _mc_path+"/"+ds << endl;
	TTree * _tree = (TTree *) _file->Get("tree");
	int n = _tree->GetEntries(); // number of events

	// analysis variables:
	unsigned int run; Float_t beamXangle, BDT; Double_t weight;
	float weight_sys_Up[weight_syst_names.size()];
	float weight_sys_Dn[weight_syst_names.size()];
	float wnorm=1; int printProgress;
	
	float bugfix_var;
	
	_tree->SetBranchStatus("*",0); //disable all branches and enable only the relevant
	_tree->SetBranchStatus("run",1); _tree->SetBranchAddress("run",&run); 
	_tree->SetBranchStatus("beamXangle",1); _tree->SetBranchAddress("beamXangle",&beamXangle); 
	_tree->SetBranchStatus("wBDT",1); _tree->SetBranchAddress("wBDT",&BDT); 
	_tree->SetBranchStatus("EL_trigSF_wgt_err",1); _tree->SetBranchAddress("EL_trigSF_wgt_err",&bugfix_var); 
	_tree->SetBranchStatus(_weight,1); _tree->SetBranchAddress(_weight,&weight); 
	for(int is=0;is<nsys_weights;is++) {
	  TString sys_name_(syst_weights.at(is));
	  int sys_len = sys_name_.Length();
	  if((sys_len-1)==sys_name_.Last('_')){
		_tree->SetBranchStatus(sys_name_+"Up",1); 
		_tree->SetBranchStatus(sys_name_+"Down",1); 
		_tree->SetBranchAddress(sys_name_+"Up",&weight_sys_Up[is]);
		_tree->SetBranchAddress(sys_name_+"Down",&weight_sys_Dn[is]);
	  }
	  else{
		  _tree->SetBranchStatus(sys_name_,1); 
		  _tree->SetBranchAddress(sys_name_,&weight_sys_Up[is]);
		  weight_sys_Dn[is] = 0;

	  }
	}
	
	int signal_protons;
	if(component_idx==0){
	  _tree->SetBranchStatus("signal_protons",1); _tree->SetBranchAddress("signal_protons",&signal_protons); 
	}
	
    // fix event normalization before the event loop:
	wnorm = xsec*user_lumi/norm; printProgress=(n<1000) ? 1 : n/1000;
	// loop over all events in the tree
	printf ("\rprocessing %d events from %s [%3.0f%%] done",n,ds.Data(), 0.);
	for(int i_ev=0;i_ev<n;i_ev++){
	if(i_ev%printProgress==0) printf ("\rprocessing %d events from %s [%3.0f%%] done",n,ds.Data(), 100.*i_ev/(float)n);
	  _tree->GetEntry(i_ev);
	  int ireg = regionIdx(run,beamXangle);
	  if(ireg<0 || ireg>_nregions) {cout << "ERROR: ireg = " << ireg << " = regionIdx("<<run<<","<<beamXangle<<")"<< endl; continue;}
	  //if(component_idx==0 && signal_protons!=11) continue;
	  float var = BDT;
	  // over/under flow bins
	  if(var<_Xmin) var=_Xmin+0.5*_binWidth;
	  else if(var>_Xmax) var=_Xmax-0.5*_binWidth;	  
	  float weight_total = weight*wnorm;

	  //bug fix analusis using el_eta, SF derived for el_SCeta, weights set to -1 07/12
	  if(bugfix_var>0.5) weight_total=-weight_total;

	  shapes_nom.at(ireg).at(component_idx)->Fill(var,weight_total);
	  for(int is=0;is<nsys_weights;is++){
		  if(syst_weights.at(is).Contains("toppt_wgt")){
			  shapes_varUp.at(ireg).at(component_idx).at(is)->Fill(var,weight_total/weight_sys_Up[is]);
			  shapes_varDn.at(ireg).at(component_idx).at(is)->Fill(var,weight_total);
			  continue;
		  }
		  else if(fabs(weight_sys_Up[is])>0.99 ||fabs(weight_sys_Dn[is])>0.99 || weight_sys_Up[is]!=weight_sys_Up[is]|| weight_sys_Dn[is]!=weight_sys_Dn[is]){
			  shapes_varUp.at(ireg).at(component_idx).at(is)->Fill(var,weight_total);
			  shapes_varDn.at(ireg).at(component_idx).at(is)->Fill(var,weight_total);
			  continue;
		  }
		  float errUp = 1+weight_sys_Up[is];
		  float errDn = (weight_sys_Dn[is]) ? 1 - weight_sys_Dn[is] : 1 - weight_sys_Up[is];
		  
		  // protect from variations >100%
		  if(abs(errUp-1)>1) errUp=1;
		  if(abs(1-errDn)>1) errDn=1;
		  
		  //cout << "sys = " << syst_weights.at(is) << " +/"<<errUp<< " , " << errDn << endl;
		  shapes_varUp.at(ireg).at(component_idx).at(is)->Fill(var,errUp*weight_total);
		  shapes_varDn.at(ireg).at(component_idx).at(is)->Fill(var,errDn*weight_total);
	  }
    }
    printf ("\rprocessing %d events from %s [%3.0f%%] done\n",n,ds.Data(), 100.);
	
	// close the component file
	_file->Close();
	
	// processing alternative samples (if defined by Selection::setSysSample)
	for(int i_sys=0;i_sys<nsys_samples;i_sys++){
		TString ds_sys(ds);
		ds_sys=ds_sys.ReplaceAll(".root","_"+syst_samples.at(i_sys)+"up.root");
		cout << "Read " << _mcsyst_path+"/"+ds_sys << endl;
		_file = TFile::Open(_mcsyst_path+"/"+ds_sys);
		if(!_file) {cout << "ERROR: "<<_mcsyst_path<<"/"<<ds_sys<< " not exists" << endl; continue;}
		if(!_file->Get("evt_count") || !_file->Get("tree")){cout << "ERROR: "<<_file->GetName() << " is empty" << endl; continue;}
		norm=((TH1F*)_file->Get("evt_count"))->GetBinContent(SumAODbin); 
		_tree = (TTree *) _file->Get("tree");
		n = _tree->GetEntries();
		
        _tree->SetBranchStatus("*",0); //disable all branches and enable only the relevant
		_tree->SetBranchStatus("run",1); _tree->SetBranchAddress("run",&run); 
		_tree->SetBranchStatus("beamXangle",1); _tree->SetBranchAddress("beamXangle",&beamXangle); 
		_tree->SetBranchStatus("wBDT",1); _tree->SetBranchAddress("wBDT",&BDT); 
		_tree->SetBranchStatus(_weight,1); _tree->SetBranchAddress(_weight,&weight); 		

	    _tree->SetBranchStatus("EL_trigSF_wgt_err",1); _tree->SetBranchAddress("EL_trigSF_wgt_err",&bugfix_var); 

		int signal_protons;
		if(component_idx==0){
			_tree->SetBranchStatus("signal_protons",1); _tree->SetBranchAddress("signal_protons",&signal_protons); 
		}
	    
        // fix event normalization (before the event loop):
	    wnorm = xsec*user_lumi/norm; printProgress=(n<1000) ? 1 : n/1000;
		
		// loop over all events in the alternative tree
	    //printf ("\rprocessing %d events from %s [%3.0f%%] done",n,ds_sys.Data(), 0.);
        for(int i_ev=0;i_ev<n;i_ev++){
	        //if(i_ev%printProgress==0) printf ("\rprocessing %d events from %s [%3.0f%%] done",n,ds_sys.Data(), 100.*i_ev/(float)n);
			_tree->GetEntry(i_ev);
			int ireg = regionIdx(run,beamXangle);
			if(ireg<0 || ireg>_nregions) {cout << "ERROR: ireg = " << ireg << " = regionIdx("<<run<<","<<beamXangle<<")"<< endl; continue;}
            //if(component_idx==0 && signal_protons!=11) continue;
			float var = BDT;
	        // over/under flow bins
	        if(var<_Xmin) var=_Xmin+0.5*_binWidth;
	        else if(var>_Xmax) var=_Xmax-0.5*_binWidth;				
			float weight_total = weight*wnorm;

	  //bug fix analusis using el_eta, SF derived for el_SCeta, weights set to -1 07/12
	  if(bugfix_var>0.5) weight_total=-weight_total;

			shapes_varUp.at(ireg).at(component_idx).at(nsys_weights+i_sys)->Fill(var,weight_total);
		}
		//printf ("\rprocessing %d events from %s [%3.0f%%] done\n",n,ds_sys.Data(), 100.);
		_file->Close();
		
		ds_sys=ds_sys.ReplaceAll("up.root","dn.root");
		_file = TFile::Open(_mcsyst_path+"/"+ds_sys);
		if(!_file) {cout << "ERROR: "<<_mcsyst_path<<"/"<<ds_sys<< " not exists" << endl; return;}
		if(!_file->Get("evt_count") || !_file->Get("tree")){cout << "ERROR: "<<_file->GetName()<< " is empty" << endl; return;}
		norm=((TH1F*)_file->Get("evt_count"))->GetBinContent(SumAODbin); 
		_tree = (TTree *) _file->Get("tree");
		n = _tree->GetEntries();
		
        _tree->SetBranchStatus("*",0); //disable all branches and enable only the relevant
		_tree->SetBranchStatus("run",1); _tree->SetBranchAddress("run",&run); 
		_tree->SetBranchStatus("beamXangle",1); _tree->SetBranchAddress("beamXangle",&beamXangle); 
		_tree->SetBranchStatus("wBDT",1); _tree->SetBranchAddress("wBDT",&BDT); 
		_tree->SetBranchStatus(_weight,1); _tree->SetBranchAddress(_weight,&weight); 		

	    _tree->SetBranchStatus("EL_trigSF_wgt_err",1); _tree->SetBranchAddress("EL_trigSF_wgt_err",&bugfix_var); 
		
		// fix event normalization (before the event loop):
	    wnorm = xsec*user_lumi/norm; printProgress=(n<1000) ? 1 : n/1000;
		
		// loop over all events in the alternative tree
        for(int i_ev=0;i_ev<n;i_ev++){
	    //if(i_ev%printProgress==0) printf ("\rprocessing %d events from %s [%3.0f%%] done",n,ds_sys.Data(), 100.*i_ev/(float)n);
			_tree->GetEntry(i_ev);

			int ireg = regionIdx(run,beamXangle);
			if(ireg<0 || ireg>_nregions) {cout << "ERROR: ireg = " << ireg << " = regionIdx("<<run<<","<<beamXangle<<")"<< endl; continue;}
			float var = BDT;
			float weight_total = weight*wnorm;

	  //bug fix analusis using el_eta, SF derived for el_SCeta, weights set to -1 07/12
	  if(bugfix_var>0.5) weight_total=-weight_total;

	        // over/under flow bins
	        if(var<_Xmin) var=_Xmin+0.5*_binWidth;
	        else if(var>_Xmax) var=_Xmax-0.5*_binWidth;				
			shapes_varDn[ireg].at(component_idx).at(nsys_weights+i_sys)->Fill(var,weight_total);
		}
		//printf ("\rprocessing %d events from %s [%3.0f%%] done\n",n,ds_sys.Data(), 100.);
		_file->Close();
	}
	
}

void Selection::processData(TString ds){
	
    // load the file and get normalization 
	TFile * _file = TFile::Open(_data_path+"/"+ds);
	if(!_file) return;
	TTree * _tree = (TTree *) _file->Get("tree");
	int n = _tree->GetEntries(); // number of events
	// analysis variables:
	unsigned int run; Float_t beamXangle, BDT;

	_tree->SetBranchStatus("*",0); //disable all branches and enable only the relevant
	_tree->SetBranchStatus("run",1); _tree->SetBranchAddress("run",&run); 
	_tree->SetBranchStatus("beamXangle",1); _tree->SetBranchAddress("beamXangle",&beamXangle); 
	_tree->SetBranchStatus("wBDT",1); _tree->SetBranchAddress("wBDT",&BDT); 

	// loop over all events in the tree
	for(int i_ev=0;i_ev<n;i_ev++){
	if((n<1000) || (i_ev%(n/1000)==0)) printf ("\rprocessing %d events from %s [%3.0f%%] done",n,ds.Data(), 100.*i_ev/(float)n);
	  _tree->GetEntry(i_ev);
	  int ireg = regionIdx(run,beamXangle);
	  if(ireg<0 || ireg>_nregions) {cout << "ERROR: ireg = " << ireg << " = regionIdx("<<run<<","<<beamXangle<<")"<< endl; continue;}
	  float var = BDT;
	  // over/under flow bins
	  if(var<_Xmin) var=_Xmin+0.5*_binWidth;
	  else if(var>_Xmax) var=_Xmax-0.5*_binWidth;	  
	  
	  shapes_data[ireg]->Fill(var);
    }
    printf ("\rprocessing %d events from %s [%3.0f%%] done\n",n,ds.Data(), 100.);
	
	// close the component file
	_file->Close();
}
