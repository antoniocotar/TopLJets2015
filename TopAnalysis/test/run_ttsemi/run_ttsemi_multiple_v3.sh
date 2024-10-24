#!/bin/bash

# Common parameters
runOnData="False"
era="era2017_F"
doPUProtons="False"
ListVars="ttbar"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/ttsemi"
inputBasePath="/store/mc/RunIISummer20UL17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/00000"
counter=24

# Array of unique parts of input file paths
inputFiles=(
    "0416277A-C8C5-DF47-A203-01F94AD10B1A.root"
    "0515A5C1-3BD2-4A4D-8735-5F987C5C5AC9.root"
    "059EBD05-3225-DD4C-8C04-EFA4FFD237A1.root"
    "05D5C506-D4BE-5A41-BE26-33C811D5AD15.root"
    "061FFC08-F7C9-5243-A757-24B32D5FD598.root"
    "06CCC7CE-D5B5-BB45-BB79-8C5CD2281800.root"
    "07835FD6-C024-2942-910C-24159F5454BE.root"
    "07A90435-A5FF-5047-A059-C73875C47897.root"
    "07B8C6F9-417A-0343-94DB-6B0A5884BACC.root"
    "083F268B-79A6-564F-B7D9-EB1AD15FB3E8.root"
    "0892F769-1AD2-404F-A977-EC7655F00403.root"
    "08DB9DF7-8F80-D347-8139-6924C3AAF643.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_TTToSemiLeptonic_hf_ntrk_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile doPUProtons=$doPUProtons ListVars=$ListVars
    ((counter++))
done
