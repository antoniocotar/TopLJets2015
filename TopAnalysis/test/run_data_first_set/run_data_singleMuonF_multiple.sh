#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/first_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/270000"
counter=2

# Array of unique parts of input file paths
inputFiles=(
    "0001C5EC-3DFC-CC40-BAAC-D990407DD675.root"
    "00593A83-723A-ED49-8AC3-8CD3F43DEE85.root"
    "008533DF-8810-E74A-B16A-2CA8F2C940FF.root"
    "0112FA47-530D-B442-9415-F2B190788F22.root"
    "019DFED7-2798-6544-9871-037D60ACD5BF.root"
    "022C187E-CEC4-4F48-AF86-61B71739B2F1.root"
    "02322DF4-4BF3-814D-982D-6146F6B2077C.root"
    "043109B7-B4B7-374C-A432-6F7192558B1D.root"
    "054A69EE-468F-0A4B-A623-B698DA0A60E3.root"
    "058F5B7D-990D-2146-8F7B-010A83BBDE32.root"
    "05A3FB9E-FC4F-1B48-9AB8-B5F41E283168.root"
    "05FC1783-2AD0-BA4A-8373-FE0C40328EA0.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_first-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
