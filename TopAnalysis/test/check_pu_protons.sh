#!/bin/bash

# Define the dataset
dataset="/APtoTT-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM"

# Get the list of files in the dataset
files=$(dasgoclient -query="file dataset=${dataset}")

# Loop over each file and check for the presence of genPUProtons
for file in $files; do
    echo "Checking file: $file"
    
    # Verify if the file exists on the server
    if ! xrd eoscms ls $file > /dev/null 2>&1; then
        echo "File $file does not exist or cannot be accessed."
        continue
    fi

    # Copy the file locally
    xrdcp root://eoscms.cern.ch//$file /tmp/test.root
    if [ $? -ne 0 ]; then
        echo "Failed to copy file $file."
        continue
    fi

    # Check for genPUProtons
    edmDumpEventContent /tmp/test.root | grep genPUProtons
    
    if [ $? -eq 0 ]; then
        echo "File $file contains genPUProtons"
    else
        echo "File $file does NOT contain genPUProtons"
    fi
    
    # Remove the local copy of the file
    rm /tmp/test.root
done
