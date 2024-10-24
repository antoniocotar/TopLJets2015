#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/first_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/270000"
counter=33

# Array of unique parts of input file paths
inputFiles=(
    "10B712B6-C698-8541-86ED-71E5C1557F78.root"
    "10FD77E1-CC21-5C43-B0B8-119FF78929C0.root"
    "12361C73-BD9B-4041-A152-F85475ECC2FA.root"
    "1393F891-31F2-FA43-AFA2-BF6B213132BD.root"
    "13CE38AE-3B73-B640-BCB9-816D3C817092.root"
    "142C6254-FF87-2F40-8ECF-84F25FAAE4A3.root"
    "151BDC30-161B-8847-9B61-1FDF0DDC95AC.root"
    "158C37AB-8A60-DF46-B6D6-A21567FE24DD.root"
    "16655F15-D7A2-FA43-97A4-65845EC366EB.root"
    "17335D4B-72A3-4747-9417-B6F34C8D6610.root"
)



# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_first-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
