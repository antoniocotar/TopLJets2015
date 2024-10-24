#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/first_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/270000"
counter=43

# Array of unique parts of input file paths
inputFiles=(
    "181A23BE-28A9-7D47-A3EF-5E1892F1DE08.root"
    "18E29BF8-BA8B-3C48-B4BF-1C4552DB5D50.root"
    "199C3050-D0CA-674F-B423-BA873726BC3C.root"
    "19C7DFC5-E8D7-154A-B543-F069484C6EC8.root"
    "19D32168-B92E-D742-B2EE-FA95A841F044.root"
    "1A5294C4-EF5F-8D42-BF5F-D3CB208DADEC.root"
    "1ADF4174-D335-3546-823A-0F463AC07E4C.root"
    "1BCC488C-79FB-4F49-A5FF-6D554B64CDFA.root"
)



# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_first-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
