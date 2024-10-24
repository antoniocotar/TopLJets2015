#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/first_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/270000"
counter=52

# Array of unique parts of input file paths
inputFiles=(
    "1C38EAE8-CA88-4141-878A-5CDE919E5595.root"
    "1C51D552-F126-164B-91EE-33D41831A8A3.root"
    "1D9AEDD8-B576-F64F-97A2-58AC9C24F296.root"
    "1DC14BA1-A035-664F-B237-9111C7D8D2B8.root"
    "1F4976EF-461E-5E4C-BFD1-AEF7D895884B.root"
    "1FC4D582-1258-3A4F-9826-0AF46FE2DE71.root"
    "20A7FA03-B904-8A40-87E3-CE83CB5E95EE.root"
    "2138B79E-8262-3E42-9E3A-5B1945C7599E.root"
    "21B5014B-38E8-2C43-929A-526D56200EBB.root"
    "21FC48A2-E922-C94F-AECE-6B8C79172000.root"
    "22C864BF-AF8C-6749-9F64-44A05520D723.root"
    "22D94420-F5C7-674F-9DF2-984072C47154.root"
    "22ECD97F-16D2-7543-90E8-B9429E75D765.root"
    "254A2FB4-AA60-9A4F-89E1-8171B309DDDA.root"
    "260B4B40-9945-084A-BE9B-7954D52D8788.root"
    "263478AC-B346-614B-8161-5560B35E26C1.root"
    "26EF2F7B-2427-3348-B885-F5081DFD8AEF.root"
    "27D492B1-F9DA-6D4E-B598-E8EB0FAA4263.root"
    "285EF4C9-AB88-5E4C-A9A5-919127967139.root"
)




# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_first-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
