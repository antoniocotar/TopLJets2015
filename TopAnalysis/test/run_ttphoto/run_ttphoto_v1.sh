#!/bin/bash

# Define the array of input and output files
declare -a inputs=(
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/164DD1D1-2732-1D4E-B982-5D7D901639DD.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/193E746B-747C-EF49-8DEF-001006827176.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/1A6E8E2A-66B1-734F-A5A4-DC9A72313566.root"
)

declare -a outputs=(
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v1.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v2.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v3.root"
)

# Common parameters
runOnData="False"
era="era2017_B"
doPUProtons="False"
ListVars="ttbar"

# Loop through the arrays
for i in "${!inputs[@]}"; do
    cmsRun runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=${inputs[$i]} outFilename=${outputs[$i]} doPUProtons=$doPUProtons ListVars=$ListVars
done
