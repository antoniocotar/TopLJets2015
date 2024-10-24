#!/bin/bash

# Base output directory
output_base_dir="/ttphoto_local"

# List of input files
input_files=(
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/01F501AB-5A7A-D645-89FF-8EE25158CBEB.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/05594CFE-2E95-AD4B-B4BF-D9C524C74FB2.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/05CF322A-34C5-6B41-9420-502D2FBBD1A3.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/0CA736B3-E16C-DF46-B44E-F0D45848F5E6.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/0FDCE1DF-A694-C440-8D5F-342A130B1D4F.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/1734B3AE-6D35-0842-95EA-547062797649.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/1AD2A0EF-2218-7A4F-873C-EDE72B20800B.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/1C20B87B-E0A7-3045-8A6A-E8DB2EE14AA9.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/1CFEFF8E-9D21-6F43-ACC4-98B564CFEC68.root"
)

# Version counter
version_counter=1

# Loop over input files
for input_file in "${input_files[@]}"; do
    # Construct the output filename with "positive" tag
    output_filename="${output_base_dir}/test_ttphoto_2017B_xa120_positive_v${version_counter}.root"

    # Run cmsRun with the specified input file and output filename
    cmsRun runMiniAnalyzer_cfg.py inputFile="${input_file}" outFilename="${output_filename}" doPUProtons=False ListVars=ttbar_sys runProtonFastSim=120

    # Increment the version counter
    version_counter=$((version_counter + 1))
done

echo "All jobs have been submitted successfully."
