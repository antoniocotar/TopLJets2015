#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/second_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=10 

# Array of unique parts of input file paths
inputFiles=(
    "27CD5E53-D30C-6040-8372-05F47B8ED676.root"
    "1088F9CC-3F1A-7646-9348-ECF5FA4D4FE6.root"
    "E755BCE0-709C-E240-9AFF-385362848C7D.root"
    "9E012DD9-379E-5440-83AC-F569A4823948.root"
    "0804C0C1-0272-FD4E-90D2-824D01A97DBA.root"
    "20DDBBCA-3C97-E849-BCF3-D4292B6BDFE8.root"
    "8AEA3A82-75FD-C545-961B-0D71860DF86A.root"
    "1C3A5EE1-8ACE-E94E-8796-1E110860C959.root"
    "6DC7A4AD-EAB2-3948-9EAC-14734B5B1EF9.root"
    "C1D308D2-69DC-0E44-8B41-E24D0B438D9F.root"
    "00009621-E0F4-534D-BEBA-41E9E1E5D1DE.root"
    "D46FB8E6-5230-5044-A9F3-6DED9E7A58CA.root"
    "70094930-3056-E448-8646-FF0D0DCBC9C0.root"
    "4BB64AAA-DE1D-5A41-898C-236A69CE31DD.root"
    "FC68C9B0-AFA1-BC46-9B98-6797C81F1003.root"
    "945B8DBE-6044-5443-909A-6A3DB70ED4ED.root"
    "816CABA8-542F-1549-8BA7-42FD90D77299.root"
    "7D521320-95F5-6544-9A5A-92376E5787B9.root"
    "02AFF491-5769-DB4B-B3E7-4801672257DC.root"
    "A48662CA-D0A4-7746-BACE-C8BB45809050.root"
    "79658FBC-83FF-AC42-8189-7D304DDB1221.root"
    "0F6B8ABA-BA7F-1643-B07B-90A05CD75942.root"
    "E43E8452-6E52-4545-8FF0-DE6E243781BB.root"
    "AC13AE15-6F7E-A347-B918-75D570E3390B.root"
    "2AE9EAC6-E79A-AC4A-A284-B691EAE375B2.root"
    "7412C543-F59A-6145-8EE2-758EF1726B95.root"
    "7A13390D-73F8-5445-A09B-B7BE51FABA3B.root"
    "B83C733C-3EA7-3B40-9FAA-146093A8E2CD.root"
    "3B7E239E-B007-334B-A0C2-F073C08E78D9.root"
    "E886292C-6913-A146-A3D5-E93179DC2629.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_second-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
