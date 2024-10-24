#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/first_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/270000"
counter=91

# Array of unique parts of input file paths
inputFiles=(
    "31739B93-1B55-CF4D-B751-833EF6451F09.root"
    "317F9927-B1F4-7740-8321-E1CC37EAFE5D.root"
    "31DDB3C1-2607-D446-93D1-D9EEDD78D1A8.root"
    "31F1CB8E-12C9-414B-9ECF-C666C45CD180.root"
    "3234C105-AA44-CC40-923C-FA2A667748E7.root"
    "32A3D1FB-88DC-FA49-91BD-773D73C77C57.root"
    "32DC833D-5F59-C34E-AEF4-1090BA7BEA3B.root"
    "32F6DB63-AC36-A24B-80C2-A963F4229078.root"
    "33A5AA33-EA95-4049-A70A-C9B2F8965AE9.root"
    "33AA894B-E14A-AC4E-8420-1D72C82BC907.root"
    "3556FE54-F9D3-314D-8BF5-29BDA362AB23.root"
    "3653B9FB-0F21-F14F-B561-7772284A42BB.root"
    "365CF8CF-6DFB-5B4B-89DA-E73BCFEA6D1B.root"
    "36E30444-792A-784B-B6F3-30B82F905902.root"
    "36F99A82-D9FB-154D-960C-25D4DE83D566.root"
    "375925A4-6BAC-444B-8CEE-05E96FE94A8E.root"
    "37AFDA35-FA3F-6446-916F-FEC64C4E67CD.root"
    "37E86407-206E-D24C-9991-B3A90B55580C.root"
    "38F37631-7CDF-564C-BD1E-D7E177D559F0.root"
    "3912797B-CEBF-BD4F-97E8-D9BBE2406584.root"
    "39B2F11E-6078-1444-91BF-43967485A62B.root"
    "3B316716-7B9D-D54C-9E3F-AB5734326E3B.root"
    "3B8EEBC6-4D75-2044-924A-6EC737540722.root"
    "3BFD8206-FB63-9D41-98A2-4126A023439C.root"
    "3CB0ABC6-B192-E741-B4BD-083BF76A1FE5.root"
)




# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_first-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
