#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/second_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/270000"
counter=1

# Array of unique parts of input file paths
inputFiles=(
    "4D3C1882-A53B-9749-976A-DBF9FE22F905.root"
    "4DD44983-45E7-424E-8FC7-94FCAD154E57.root"
    "4E2348BF-1D71-C84E-969E-B22A258D84D1.root"
    "4EEB1D77-BA6C-874B-8720-3173C71C5E9A.root"
    "4F074D9C-6E75-1946-958E-0F28583C2615.root"
    "4F6DA0CD-495C-1441-88E8-5567976AB48C.root"
    "4F7C4FDA-77BF-F741-AEED-546DF34C9501.root"
    "4FED29F3-6245-B444-9CFE-AC31CD9005CC.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_second-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
