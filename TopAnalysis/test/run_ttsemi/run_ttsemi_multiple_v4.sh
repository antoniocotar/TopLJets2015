#!/bin/bash

# Common parameters
runOnData="False"
era="era2017_F"
doPUProtons="False"
ListVars="ttbar"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/ttsemi"
inputBasePath="/store/mc/RunIISummer20UL17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/00000"
counter=36

# Array of unique parts of input file paths
inputFiles=(
    "093D4539-B082-1B47-82C7-18C9D6F15B5A.root"
    "098E1E26-6108-654E-B170-3C183CE1CCD0.root"
    "09CF080C-D800-C04A-8671-12D5DEA9BA1B.root"
    "09FC43CC-F7A2-B846-9146-B400DB3E8228.root"
    "09FD4891-4597-3148-B066-144BEA80533E.root"
    "0A368404-E886-DD49-9FC0-EAC2D2C9069A.root"
    "0A764635-8474-6E42-AB04-FED8DFF66F7F.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_TTToSemiLeptonic_hf_ntrk_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile doPUProtons=$doPUProtons ListVars=$ListVars
    ((counter++))
done
