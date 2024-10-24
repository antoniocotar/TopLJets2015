#!/bin/bash

# Common parameters
runOnData="False"
era="era2017_F"
doPUProtons="False"
ListVars="ttbar"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/ttphoto"
inputBasePath="/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000"
counter=1

# Array of unique parts of input file paths
inputFiles=(
    "0823E980-A2E3-5747-8C36-CD1A880C5096.root"
    "0F957E96-ABC0-3843-9031-3485CF18F38A.root"
    "0FA7BCB6-7738-0D47-A700-CE401040ADA1.root"
    "12CD5F03-61AA-B648-98DD-440A0C03DBD3.root"
    "14C0BA57-BA6F-0249-B884-426BFAEC3C6C.root"
    "164DD1D1-2732-1D4E-B982-5D7D901639DD.root"
    "193E746B-747C-EF49-8DEF-001006827176.root"
    "1A6E8E2A-66B1-734F-A5A4-DC9A72313566.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_ttphoto_hf_ntrk_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile doPUProtons=$doPUProtons ListVars=$ListVars
    ((counter++))
done
