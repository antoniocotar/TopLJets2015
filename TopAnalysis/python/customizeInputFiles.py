import FWCore.ParameterSet.Config as cms
import subprocess
import json

def getListAOD(filename):
  print 'collect AODs for ',filename
  recotag=filename.split('/')[6]
  #Get list of runs / lumis for the input file
  p = subprocess.Popen(['dasgoclient --query="lumi file=%s" -json'%filename], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
  out, err = p.communicate()
  data = json.loads(out)
  miniaodruns=[]
  miniaodlumis=[]
  for i in range(len(data)):
    miniaodruns.append(data[i]['lumi'][0]['run_number'])
    miniaodlumis.append(data[i]['lumi'][0]['lumi_section_num'])
	
  #Get list of RAW files
  dascmd='dasgoclient --query="parent file=%s"'%filename
  p = subprocess.Popen([dascmd],stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
  out, err = p.communicate()
  rawfiles = out.split()
  
  #Loop over all RAW files to locate AODs that match miniAOD lumiblocks  
  AODfiles=[]
  for rawfile in rawfiles:
    dascmd='dasgoclient --query="child file=%s" | grep /AOD/%s'%(rawfile,recotag)
    p = subprocess.Popen([dascmd],stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    out, err = p.communicate()
    
    #Check AOD files to match with at least one lumiblock
    if len(out.split()) == 1: #if only one AOD child, skip the lumi-check
      if out.split()[0] not in AODfiles: AODfiles.append(out.split()[0])	
    else:  
      for aodfile in out.split():
        if aodfile in AODfiles: continue
        if(MatchedAOD(aodfile,miniaodruns,miniaodlumis)): AODfiles.append(aodfile);

  return AODfiles

def MatchedAOD(aodfile,miniaodruns,miniaodlumis):
  
  aodruns=json.loads(subprocess.check_output(['dasgoclient --query="run file=%s"'%aodfile], shell=True).strip())
  if len(aodruns)==1 and aodruns[0] not in miniaodruns: return False
  aodlumis=[json.loads(x) for x in subprocess.check_output(['dasgoclient --query="lumi file=%s"'%aodfile], shell=True).split()]
  for idx, aodrun in enumerate(aodruns): 
    # check location of the run in the miniaodruns list
    i_run=-1
    for ii, miniaodrun in enumerate(miniaodruns):
      if aodrun == miniaodrun: i_run=ii; break
    if(i_run<0): continue # run not in the list of miniaodruns
	
	#check if have at least a single lumiblock in list of miniaodlumis
    for lumi in aodlumis[idx]:
      if lumi in miniaodlumis[i_run]: return True
	  
  # if faled to return true -> AOD is not matched to the miniAODds
  return False

def customTestInputFiles(process,era,runOnData,runWithAOD):
    if '2016' in era:
        if runOnData:
            process.source.fileNames = cms.untracked.vstring('/store/data/Run2016B/SingleMuon/MINIAOD/17Jul2018_ver2-v1/00000/0219DD4A-7D8C-E811-B7EB-001E67248A43.root')
            if runWithAOD:
                print 'Adding secondary filenames'
                process.source.secondaryFileNames = cms.untracked.vstring(['/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/110000/B6479036-9486-E711-A6CF-003048FFD734.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/110001/0A8A7032-9086-E711-BD1B-0025905A60C6.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/110001/22FFA42A-9086-E711-9B49-0CC47A7C3472.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70000/767AE9A5-BA81-E711-9A8E-0CC47A745282.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70000/D67D18A5-BA81-E711-B6C2-0CC47A7C361E.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70000/E06164A7-BA81-E711-B612-0025905B858A.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/302E2840-C481-E711-8913-002618FDA259.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/687FA66D-BC81-E711-B41D-0CC47A745282.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/6890466C-BC81-E711-9C62-0CC47A4C8E1C.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/E64E346F-BC81-E711-93BC-0CC47A4C8F18.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/EC3632F7-C081-E711-B219-0CC47A7452DA.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/F6499DE0-C281-E711-9835-0CC47A78A33E.root'])
            else:
                process.source.fileNames = cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv3/ST_t-channel_antitop_4f_mtop1715_inclusiveDecays_13TeV-powhegV2-madspin-pythia8/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/120000/16CEB785-3FE6-E811-AAE8-FA163E9D74F8.root')
    elif '2017' in era:
        if runOnData:
            process.source.fileNames = cms.untracked.vstring('file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/MINIAOD/09Aug2019_UL2017-v1/30000/E4D20966-5F71-5C47-867A-21164955708A.root')
            if runWithAOD:
                print 'Adding secondary filenames'
                process.source.secondaryFileNames = cms.untracked.vstring([
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/486/00000/4C120193-C559-E711-B7EA-02163E01A3BE.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/485/00000/44179606-B659-E711-8412-02163E019C43.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/225/00000/2A3968F8-6F56-E711-848C-02163E013649.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/660/00000/2CB8DF01-4F5C-E711-9A06-02163E0146E8.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/659/00000/5C4E58B9-4E5C-E711-B789-02163E01383A.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/620/00000/D4C1D535-0F5C-E711-B5AF-02163E019CA1.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/620/00000/301BEF34-0F5C-E711-9774-02163E01476C.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/488/00000/F0D2C985-FF59-E711-A700-02163E01A212.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/488/00000/AA8B6BBC-F559-E711-B767-02163E012401.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/488/00000/3260EAAE-FF59-E711-ADAE-02163E01461B.root',
				'file:/eos/cms/store/cmst3/user/mpitt/ttbar/Run2017B/MuonEG/RAW/v1/000/297/488/00000/2602B391-F559-E711-82AD-02163E014259.root'
                ])
        else:
            process.source.fileNames = cms.untracked.vstring('/store/mc/RunIISummer19UL17MiniAOD/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/106X_mc2017_realistic_v6-v2/270000/2927EC69-BA37-F64D-8057-040C81E11C06.root')
    elif '2018' in era:
        if runOnData:
            process.source.fileNames = cms.untracked.vstring('/store/data/Run2018C/SingleMuon/MINIAOD/17Sep2018-v1/00000/AB61FB4F-3A42-4B4F-93E2-78CD7E7CF0A4.root')
            if runWithAOD:
                print 'Adding secondary filenames'
                process.source.secondaryFileNames = cms.untracked.vstring([
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/C45A09EB-328E-E811-BFEC-FA163EDD3A25.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/0A3EDD77-128E-E811-8885-FA163E08D4C1.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/1E0CFF73-128E-E811-9B2B-FA163E6DCA76.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/9659EBAF-108E-E811-93CB-FA163E82172A.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/025/00000/F05577B5-568D-E811-A9FA-FA163E513903.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/026/00000/26D29DBA-5F8D-E811-9D7B-FA163E600F07.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/026/00000/9239C9D4-5F8D-E811-BD2C-FA163ECED2A8.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/BEED33AC-328E-E811-8305-FA163EDE7DC6.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/025/00000/C662E202-578D-E811-8E1E-FA163E7FB452.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/024/00000/F4B08B5F-528D-E811-965A-FA163ED40F48.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/024/00000/0863DA3E-548D-E811-B8E1-A4BF01277823.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/009/00000/0218B251-F28C-E811-81CD-FA163ED486E3.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/010/00000/768D18BE-F98C-E811-A332-FA163E60ED2F.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/007/00000/D208B88E-E98C-E811-9B7F-FA163E61ADEA.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/007/00000/DAA30463-E98C-E811-918B-FA163EF70A72.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/010/00000/7A33E7D8-F78C-E811-AF32-FA163E3AFBF6.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/319/854/00000/A4D6882F-768A-E811-9978-FA163E11A4DA.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/007/00000/B4B0ADA6-E88C-E811-B65F-FA163E105698.root'
                ])
        else:
            print 'FIXME: add a MC test file for 2018...'
