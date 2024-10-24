#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/first_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/270000"
counter=24

# Array of unique parts of input file paths
inputFiles=(
    "0BB72E3E-CB6E-044D-BE10-EF75AF6E575B.root"
    "0BD03A6C-E3A0-0346-9AE1-F611DE9D25E5.root"
    "0C0DEE7A-C4F5-664A-AFB3-FAF8FEC24FF1.root"
    "0C4E0698-301F-424B-9141-F7DE17B61724.root"
    "0CEBEEF1-2639-2743-9F06-79B8E377FEDD.root"
    "0D20D694-B1AD-C848-8683-D8D90D9F6148.root"
    "0DBD5D15-9045-C84B-9852-35E18002B467.root"
    "0ECDC463-912D-7942-8AA4-93D580112FA4.root"
    "0FCFFD32-F01F-8741-99CF-0BA321381A1D.root"
)


# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_first-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
