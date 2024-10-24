#!/bin/bash

# Base content for JSON files
base_content='{
    "APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8_%s_xa%s"                     : [0.70, 0,     "/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM", "ttbar_pos", 920, true, false, 1],
    "APtoTT-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8_%s_xa%s"                     : [0.70, 0,     "/APtoTT-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM", "ttbar_neg", 920, true, false, 1],
    "APtoTW-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8_%s_xa%s"                     : [0.51, 0,     "/APtoTW-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM", "tw_pos", 920, true, false, 1],
    "APtoTW-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8_%s_xa%s"                     : [0.51, 0,     "/APtoTW-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM", "tw_neg", 920, true, false, 1]
}'

# Arrays for the different eras and xa values
eras=("era2017C" "era2017D" "era2017F")
xa_values=("120" "130" "140" "150")

# Loop through each era and xa value to create the corresponding JSON files
for era in "${eras[@]}"; do
  for xa in "${xa_values[@]}"; do
    filename="mc_signal_samples_antonio_${era}_xa${xa}.json"
    content=$(printf "$base_content" "$era" "$xa" "$era" "$xa" "$era" "$xa" "$era" "$xa")
    echo "$content" > "$filename"
    echo "Created $filename"
  done
done
