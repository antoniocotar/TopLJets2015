#!/bin/bash

# Common parameters
runOnData="False"
era="era2017_F"
doPUProtons="False"
ListVars="ttbar"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/ttphoto"
inputBasePath="/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000"
counter=9

# Array of unique parts of input file paths
inputFiles=(
    "1A8F4242-632E-CF47-A6A9-364EB6DDE5D9.root"
    "21962C86-9C36-774F-8E05-9905277B6448.root"
    "222A1597-611E-5043-BEDE-31C830559D9D.root"
    "29EA70CE-630F-2347-8A43-EC55EA4E165D.root"
    "2F737BB7-D630-0A43-B51E-15DBF4908C73.root"
    "31F92A70-087E-5845-B596-6A13EDDBD98F.root"
    "3FF16226-CE6F-654F-8C8A-C30D8A5484A4.root"
    "401CE172-60C2-6F4D-935A-F38275F163E6.root"
    "42E7C8E5-42C1-4848-B711-DD7511DC443D.root"
    "480D388A-7F98-404D-95A9-D654AAE341C6.root"
)


# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_ttphoto_hf_ntrk_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile doPUProtons=$doPUProtons ListVars=$ListVars
    ((counter++))
done
