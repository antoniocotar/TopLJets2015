#!/bin/bash

# Define the base directory of the source JSON file
base_dir="/eos/user/a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/data/era2017"
source_file="mc_signal_samples_antonio_era2017B_xa120.json"

# Define the target eras and the sequences for xangle
eras=("2017B" "2017E")
sequences=(120 130 140 150)

# Loop through the eras and sequences to create new JSON files with the modified content
for era in "${eras[@]}"
do
    for seq in "${sequences[@]}"
    do
        target_file="${base_dir}/mc_signal_samples_antonio_era${era}_xa${seq}.json"
        
        cat <<EOF > "$target_file"
{
    "APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8_era${era}_xa${seq}"                     : [0.70, 0,     "/APtoTT-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM", "ttbar_pos", 920, true, false, 1],
    "APtoTT-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8_era${era}_xa${seq}"                     : [0.70, 0,     "/APtoTT-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM", "ttbar_neg", 920, true, false, 1],
    "APtoTW-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8_era${era}_xa${seq}"                     : [0.51, 0,     "/APtoTW-positivePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM", "tw_pos", 920, true, false, 1],
    "APtoTW-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8_era${era}_xa${seq}"                     : [0.51, 0,     "/APtoTW-negativePhotonDirection_TuneCP5_13TeV-amcatnlo-pythia8/RunIISummer20UL17MiniAODv2-default_ptmaxmatch_106X_mc2017_realistic_v9-v2/MINIAODSIM", "tw_neg", 920, true, false, 1]
}
EOF

        echo "Created $target_file"
    done
done

echo "All JSON files created successfully."
