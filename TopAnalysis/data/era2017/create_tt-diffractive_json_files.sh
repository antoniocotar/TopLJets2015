#!/bin/bash

# Arrays for eras and xa categories
eras=("era2017B" "era2017C" "era2017D" "era2017E" "era2017F")
xa_categories=("xa120" "xa130" "xa140" "xa150")

# Loop over each era and xa category
for era in "${eras[@]}"; do
  for xa in "${xa_categories[@]}"; do
    # Construct the JSON content
    json_content="{
    \"TT-pomflux-H1FitB_TuneCP5_13TeV-pythia8_${era}_${xa}\"                     : [5.14, 0,     \"/TT-pomflux-H1FitB_TuneCP5_13TeV-pythia8/RunIISummer20UL17MiniAODv2-106X_mc2017_realistic_v9-v2/MINIAODSIM\", \"ttbar_diffractive\", 920, true, false, 1]
}"

    # Write the content to a JSON file
    json_filename="mc_tt-diffractive_${era}_${xa}.json"
    echo "$json_content" > "$json_filename"

    # Output file creation
    echo "Created $json_filename"
  done
done
