#!/bin/bash
# Define the target directory (TopAnalysis)
target_dir="/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis"
# Define the output directory
output_dir="/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/script_for_tree_information/test_1"

# Create output directory if it doesn't exist
mkdir -p "$output_dir"

# Loop through each subdirectory in the target directory
for dir in "$target_dir"/*/; do
    # Get the base name of the directory (e.g., "bin", "data")
    base_dir=$(basename "$dir")
    # Save the tree structure to the output directory
    tree "$dir" > "$output_dir/${base_dir}_tree.txt"
done
