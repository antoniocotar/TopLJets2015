#!/bin/bash

# Define the dataset
dataset="/APtoTT-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM"

# Define the destination directory
destination_dir="/eos/user/a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/xdrp_root_files"

# Define the log file for failed downloads
log_file="$destination_dir/failed_files.log"

# Create the destination directory if it does not exist
mkdir -p $destination_dir

# Clear the log file
> $log_file

# Get the list of files in the dataset
files=$(dasgoclient -query="file dataset=${dataset}")

# Function to check file existence on the server
check_file_existence() {
    xrd eoscms ls $1 > /dev/null 2>&1
    return $?
}

# Function to copy file with retries
copy_file() {
    local file=$1
    local dest=$2
    local retries=3
    local count=0

    while [ $count -lt $retries ]; do
        xrdcp root://eoscms.cern.ch//$file $dest
        if [ $? -eq 0 ]; then
            echo "Successfully copied $file to $dest"
            return 0
        else
            echo "Failed to copy $file. Retrying... ($((count+1))/$retries)"
            count=$((count+1))
            sleep 2
        fi
    done

    echo "Failed to copy file $file after multiple attempts." >> $log_file
    return 1
}

# Loop over each file and copy it to the destination directory
for file in $files; do
    echo "Processing file: $file"
    
    # Extract the filename from the full file path
    filename=$(basename $file)
    dest="$destination_dir/$filename"

    # Verify if the file exists on the server
    if check_file_existence $file; then
        # Copy the file to the destination directory with retries
        copy_file $file $dest
    else
        echo "File $file does not exist or cannot be accessed." >> $log_file
    fi
done

echo "Script finished. Check $log_file for any failed downloads."
