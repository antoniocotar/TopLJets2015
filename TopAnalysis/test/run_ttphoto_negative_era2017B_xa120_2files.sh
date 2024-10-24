#!/bin/bash

# Array of input file paths
input_files=(
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/07112F19-D118-6041-9389-774D807546C8.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/MINIAODSIM/default_ptmaxmatch_106X_mc2017_realistic_v9-v2/2820000/127EA2E6-F39E-184C-862C-C60A939101DA.root"
    # Add more file paths as needed
)

# Base directory for output files
output_base_dir="/eos/user/a/acotarod/Thesis/Updates/Updates_Antonio/august_2024/production_ttphoto_twphoto_era_xangle_local/era_2017B_xa120"

# Loop through the input files
for input_file in "${input_files[@]}"; do
    # Extract the tag from the input file path
    tag=$(basename "$input_file" .root)
    
    # Construct the output file path
    output_file="${output_base_dir}/test_ttphoto_negative_era2017B_xa120_${tag}.root"
    
    # Run the cmsRun command
    echo "Running cmsRun for input file: $input_file"
    cmsRun runMiniAnalyzer_cfg.py inputFile="$input_file" outFilename="$output_file" doPUProtons=False ListVars=ttbar_sys runProtonFastSim=120
    
    # Check if the cmsRun command was successful
    if [ $? -ne 0 ]; then
        echo "cmsRun failed for input file: $input_file"
        exit 1
    fi
done

echo "All cmsRun commands executed successfully."
