#!/bin/bash

# Common parameters
runOnData="False"
era="era2017_F"
doPUProtons="False"
ListVars="ttbar"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/ttphoto"
inputBasePath="/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000"
counter=19

# Array of unique parts of input file paths
inputFiles=(
    "4BD27E2B-8A35-D245-B971-50686EDAB822.root"
    "4BE302CC-3855-6E46-852D-EFBE3BDE833B.root"
    "4D9DA040-FADD-F347-BF97-35155804B286.root"
    "4FFDD2CA-CEAD-4C4C-9352-E2DAD9A282FA.root"
    "59B72842-60CA-8542-A607-C6B184AC8A46.root"
    "5FDF48D7-E6B8-7749-A509-D7E31957EA75.root"
    "60FB4AB3-6275-8845-8330-534689CAA148.root"
    "69F8969C-C21F-B044-860B-AC0B4E7F7C60.root"
    "6B5CA18D-F7FA-A442-A928-C4F173FB2249.root"
    "6F38EBDF-4250-1346-94D3-300D10749FBB.root"
)



# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_ttphoto_hf_ntrk_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile doPUProtons=$doPUProtons ListVars=$ListVars
    ((counter++))
done
