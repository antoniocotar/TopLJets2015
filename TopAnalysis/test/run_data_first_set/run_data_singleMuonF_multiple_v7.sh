#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/first_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/270000"
counter=71

# Array of unique parts of input file paths
inputFiles=(
    "28D0470D-CAA0-6741-B870-7CF1AAF5A25C.root"
    "28EB9BFD-9A16-2B48-A927-214A1356380F.root"
    "28FB371C-3D58-0F45-9DC9-1A2B58E24C24.root"
    "2900D829-94F5-9342-9642-806B2EEDFCAF.root"
    "2A4A95AD-7AD1-084F-8122-EB8819C074E0.root"
    "2A67A8AE-E27A-F94E-A674-8FAB4715C01E.root"
    "2B4214A3-3065-DF44-B2C2-CD4E83CD803D.root"
    "2C5B8809-92D0-2145-B0E8-D11F074459D9.root"
    "2D804FC8-6D07-5247-A851-6F166F8C3540.root"
    "2DFF44F4-7BB9-7E4B-BD1E-E53E5E6D0118.root"
    "2E073C3E-8D45-ED43-8851-24B77132BE5F.root"
    "2E353D10-2C39-314D-ADA9-3D0A27CCAF1F.root"
    "2E7ABD0F-118E-E14D-9C59-7FCA7A4D642C.root"
    "2EFC5503-2502-F14D-95EC-1CF63905F896.root"
    "2FC056DD-D7F3-7B4B-8B72-FDE99E788225.root"
    "2FE4030D-A5EB-CE44-8430-8D9EBEBB4276.root"
    "2FEC096B-36AA-F345-BCF8-6565538CD395.root"
    "30246CB3-817F-A84E-BE0A-471674B740D0.root"
    "30E64CB3-B2F4-4D40-8DEE-DBD20CF955C6.root"
    "3169CFA5-CDC7-D345-9F67-3EC689B68DAE.root"
)




# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_first-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
