#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/second_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=40

# Array of unique parts of input file paths
inputFiles=(
    "71F46733-EFB8-8649-9720-BE06DCCAE523.root"
    "3CC8B6ED-6F6B-8446-8B74-E9111FE50C3D.root"
    "24DAD100-AB92-0248-BEB5-54848EA1CBF3.root"
    "FAE7343F-9268-2947-B40A-28154F4D6E26.root"
    "9C4FF50F-5D2E-C54D-A54F-938E708010CD.root"
    "0CCA7C1F-2826-8443-BC08-FC791226F78D.root"
    "17B03F12-2580-5047-B075-FCE67FF4BEFC.root"
    "6EF8648F-61B1-2D4C-9BFB-DEBC8F3B4E89.root"
    "B092289F-7070-2D41-AE46-7572197BEC4B.root"
    "2226AA27-5B5A-B640-8AA1-34A23ECEC81B.root"
    "B1CA3417-245D-254B-B1C9-FBD478950EAF.root"
    "31161CF0-ED6C-5640-8FA6-6583FD7F7509.root"
    "15DACBEC-6E8E-8A46-BE54-33B10D104B22.root"
    "958653ED-8CE4-4945-86FB-6369A88DBCAF.root"
    "58A5821A-B610-FC4E-8EB1-FC04E8225EFF.root"
    "BB8E6F16-9E6F-E44F-8EBA-C651DC28A811.root"
    "F1CB1F2A-50E9-6843-B0F0-E82054A96CC7.root"
    "158D30D8-D56D-FB40-808B-782D227EDEF8.root"
    "99F40DCA-651A-0844-9AC7-3C630D8283E8.root"
    "15348649-9786-F149-B1A7-953A6E1A8299.root"
    "0E51BC87-EA97-5D4B-A099-E36B3D5F8F2E.root"
    "A46AADC2-C4B7-734E-B4A1-FBD1FC443A38.root"
    "5BB7AAEC-3A4B-C44C-9AA5-22818699F302.root"
    "533FA556-8B9D-A14B-AD63-CF4B6546EE83.root"
    "FAEAA2D5-FDA4-8D4A-B7A1-AA783FCFE593.root"
    "27B39BF2-AEAB-974F-B59C-FD6B8CB235BD.root"
    "A1C0F26F-211E-6240-87C2-F72C7F78CC43.root"
    "CF8DE6EB-2F98-6F4A-BDAA-1886EC74B671.root"
    "920C8E4F-8A22-8F43-AF72-09E35909400D.root"
    "3BEC1245-CCE9-1B40-9017-E727B1F47F0E.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_second-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
