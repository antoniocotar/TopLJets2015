#!/bin/bash

# Navigate to the CMSSW base directory
cd /eos/user/a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src

# Setup CMSSW environment
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc820
eval `scramv1 runtime -sh`

# Ensure the correct version of libstdc++.so.6 is used
export LD_LIBRARY_PATH=/usr/lib64:$LD_LIBRARY_PATH

# Navigate to the directory containing the analysis script
cd /eos/user/a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test

# Run the cmsRun command
cmsRun runMiniAnalyzer_cfg.py inputFile=/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/07112F19-D118-6041-9389-774D807546C8.root outFilename=test_ttphoto_2017B_xa120.root doPUProtons=False ListVars=ttbar_sys runProtonFastSim=120
