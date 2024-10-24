#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/first_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/270000"
counter=14

# Array of unique parts of input file paths
inputFiles=(
    "0607B0B1-DD86-354C-8FAD-F9BC4D6A2385.root"
    "069E1557-DAB9-7247-BDAF-3759CA12079B.root"
    "06FCF48C-884C-EB47-A320-55E33791DB9A.root"
    "083FA4C6-5737-A74B-BF56-0FF8F9DBD00F.root"
    "08A8B80A-3506-F041-BB68-2D9FD500A6F7.root"
    "08BFD0E1-7523-2849-8C8A-C1DA3BD61391.root"
    "09E325EE-0774-AD43-ABDD-B1310CE4BC99.root"
    "09EC74E9-E303-4848-A089-264CAE82EF8D.root"
    "09F25F85-356C-4647-8464-0C526896DCC6.root"
    "0ABFE2E4-AE06-4F41-9463-6E16E9C9B1A5.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_first-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
