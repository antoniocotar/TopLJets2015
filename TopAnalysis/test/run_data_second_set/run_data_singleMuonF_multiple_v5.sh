#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/second_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=100

# Array of unique parts of input file paths
inputFiles=(
    "75C96F26-F0DC-CB41-8E3E-68B0E6EDA0DA.root"
    "A8A7F1F0-63A3-3C47-9752-2BAF155194B5.root"
    "C6843799-E1B2-8E41-8DB9-0082DF40F364.root"
    "FD0BC958-8C86-4048-A61A-81E242A12AB3.root"
    "C0C1827F-3937-8541-83AA-36BC3DB8EEA9.root"
    "15018F88-7EEE-144F-8832-20BF69A31B01.root"
    "00B841F5-84FC-CC45-A565-E47AC7EA855B.root"
    "A3A1E161-B13D-DC46-A447-7E6724AD7748.root"
    "198A0B1A-68A1-C745-A5D4-9A9223790AE4.root"
    "0A102B07-8FE1-9A46-9D4E-92FA1D8B3D7A.root"
    "04CCCD39-1078-704D-81D6-357041A3122E.root"
    "42C583A1-E465-9F42-8CDA-CA9597A558F1.root"
    "6857D8D1-511E-E14A-AB9E-4AF0EF22776E.root"
    "11558C6F-A327-734B-913C-FFB4CC8C536F.root"
    "4F5EFE67-A975-7444-9524-3176E233DDFC.root"
    "9AA06033-639D-E94A-BE99-84C91006E85D.root"
    "CE6BF072-4585-8649-BDC8-9FCCCD29A27B.root"
    "B2C0511B-E82F-4D47-8CDC-DF5904792E05.root"
    "F06C343F-3348-4C41-BD93-A09F3EF6AFD0.root"
    "EB96294F-3907-E845-B3B5-5F388BEF729E.root"
    "79758B37-B75D-B447-9806-85B7E3494D4A.root"
    "6C6092CD-0738-F844-86A2-F48CCBAB9822.root"
    "1CC39C12-C6ED-6041-9861-A39CD3D50B0C.root"
    "B8E9A7E2-5FB8-EC47-ABE5-2594C8D00E4C.root"
    "882CE17C-1122-9E4B-81A4-B03D36E3F89A.root"
    "32D3E4C4-134E-3B4A-8830-DCC8E88C9D2B.root"
    "1080DAB2-453C-FC45-9964-5569E71EAAE8.root"
    "5A9A6EB7-8789-B344-AFB1-88DBA2090152.root"
    "C3FA13C1-B2B0-BF48-BD87-C559EF169488.root"
    "62A71695-7D20-2F4B-BBA4-F17D1EF3A59F.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_second-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
