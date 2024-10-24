#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/second_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=190

# Array of unique parts of input file paths
inputFiles=(
    "B34D0904-419C-2842-A02C-A6057FF32C34.root"
    "130F9EFF-064F-8B47-B5DE-8BE9EBBA83B9.root"
    "E8B7ACB9-9F7F-B14D-A627-F7392697C204.root"
    "D1F2DB9E-FC53-2642-BF49-6E7BDF8C5F63.root"
    "2A5402E3-C2EA-C848-B284-1A814EB12F40.root"
    "56263BF8-7EC5-E04F-BB1B-06E57E6D9DD2.root"
    "C9AEFC92-933B-B84B-8E0B-B87702945783.root"
    "AF8F77C8-94FB-924D-9900-B781BBF88F00.root"
    "E66D5A24-5596-A441-AD1D-8A0E5DD6D2CA.root"
    "F41D3C9A-A672-BC4B-9F6A-D7215ACA2B69.root"
    "0ADB39B9-F17B-F341-9D0A-57EBACA8DB09.root"
    "CE0DC61D-87D8-B94E-8AE7-30AD6AAE291F.root"
    "1D2F80AC-0FA3-A045-B4F7-73B50C268242.root"
    "C6D981E8-A429-DA48-98FF-0E5777F18012.root"
    "0B1D6496-378B-D341-B1B7-6A1D45407E09.root"
    "2BE26915-01AF-1F4E-91D9-C35177812179.root"
    "4F080895-6094-9944-9F43-282E1CFE5BA0.root"
    "580E55C1-517F-8D46-835A-70F785E7E5E6.root"
    "9A92F533-4EE1-8D4E-9DB0-827F1D8A8DA9.root"
    "2503F4B4-CF1D-F44D-A431-723442620084.root"
    "61487C76-BF2B-BF43-A7C5-A02A228A8FB9.root"
    "F75DD914-58A6-374E-AAF6-A118EA532B71.root"
    "A628A9FF-2682-4740-B520-B997FEB941A3.root"
    "C15E641C-0E52-B44A-BD67-F79C490420D0.root"
    "BE84C64A-3D37-8E43-9026-B8E937CA4AA0.root"
    "4E953A93-33E9-5944-99A1-B9011583D3D0.root"
    "B8E82560-13F2-C440-8606-1A8C2E1CD65F.root"
    "0591EE46-33A8-C043-A037-B90A0A970842.root"
    "5CEE5F10-8DCB-6846-AF9F-13FC953EDFDE.root"
    "B33594D4-5061-EF46-BC49-D9D105B22100.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_second-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
