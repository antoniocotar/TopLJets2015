#!/bin/bash

# Common parameters
runOnData="False"
era="era2017_F"
doPUProtons="False"
ListVars="ttbar"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/ttsemi"
inputBasePath="/store/mc/RunIISummer20UL17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/00000"
counter=12

# Array of unique parts of input file paths
inputFiles=(
    "005708B7-331C-904E-88B9-189011E6C9DD.root"
    "00623789-C012-474B-8860-3C18397A464F.root"
    "0074902D-0DEC-7946-9901-309B04400A3F.root"
    "00B2D4E3-FF37-9F4F-A724-7E0DBC1B8329.root"
    "013C3A2D-BC7D-F643-AE9F-2CD8AF149F5D.root"
    "0158F46B-96BD-8244-AC41-E317C05025DF.root"
    "01598D9D-2C3B-2B40-A63B-144C1E6DAA8E.root"
    "01B0B418-DB7F-324B-B678-5CED3A5CE179.root"
    "02071E82-445C-2647-B4F6-3E8C0546381C.root"
    "02FC1590-93CE-2E4C-97F5-B6E35F272720.root"
    "03494230-667F-4042-8DE6-4F3933F7F0C7.root"
    "036A87AC-A9E8-9B4D-8F56-65AF574A892C.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_TTToSemiLeptonic_hf_ntrk_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile doPUProtons=$doPUProtons ListVars=$ListVars
    ((counter++))
done
