#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/second_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=130

# Array of unique parts of input file paths
inputFiles=(
    "61C2FC93-E959-1649-9857-E3FBE4F645E2.root"
    "8968099C-BE8F-1747-BE66-A6EEDAFC22D2.root"
    "55AF35B3-DBDF-C345-B638-E46BA97D4B9B.root"
    "4F1C35C6-699C-5640-AA36-1A3F9934EEBC.root"
    "B26D7B1B-3F49-E149-BC56-61906121A578.root"
    "4E2D0CEE-8F8B-2A41-B4B5-6C3C34DDCAB0.root"
    "83AE3430-264A-5D40-868D-A95299FC91C1.root"
    "1BAB8C99-B003-0949-B767-D3C9E58145D4.root"
    "3BE7EBD0-303C-6442-B4E3-6AB419B5BF0E.root"
    "938670FC-784F-B044-871E-6E3848A6E589.root"
    "D6212F3A-B28B-6B47-88FB-DDF0DE8F2E65.root"
    "D9C90C93-BC7D-4442-8038-C68A1CB91190.root"
    "8B72D265-AD8A-3440-8CF1-B8F02177C10B.root"
    "0EE51888-5C54-B640-9879-49E1025E59A2.root"
    "37DDB1D9-45F2-534D-B957-2A753BDC9711.root"
    "1544EC68-22F6-E342-ABF6-B80466620AA6.root"
    "7B44FE24-4490-4D41-9FB2-AD24A147A6F6.root"
    "5FDBF6C7-1F5B-DD49-AC94-85C20F7A4FDF.root"
    "0BF3B17C-5ECF-D14C-8DF9-F32B54A4F623.root"
    "97C39EDC-95A2-6D40-A921-064E16A65B5F.root"
    "D4918C09-6794-3A4B-B12B-21FE19996536.root"
    "424BC179-A1E0-0146-B291-52ACD81D1697.root"
    "4CA5D201-2025-4145-9127-068D4767F0D2.root"
    "A7E1C1BF-20D9-524C-80FD-BA410CED98AE.root"
    "DA904AF6-533D-9347-B2DF-687CC7E14C1C.root"
    "C16E6E16-3312-D54C-9489-057B6456AA6E.root"
    "D9390A39-E548-BE48-90A9-136A6C48763E.root"
    "C4351EEA-F18D-1D4B-9E53-410694D7199A.root"
    "35FCB9FC-6DF0-7146-8652-0CA983AA736F.root"
    "EF55882E-0948-B247-A325-97243D8CAB26.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_second-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
