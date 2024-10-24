#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/second_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=210

# Array of unique parts of input file paths
inputFiles=(
    "447F83C0-FB28-FF4E-B81A-9360DD0FF21A.root"
    "27A5A418-9450-4B48-87FA-A7F03869EC32.root"
    "3C968CAC-F010-694B-8826-B38051AB9052.root"
    "954FABD9-0206-8E49-A532-4F340991CA04.root"
    "3359B285-E61B-F34D-B71A-7ACC9019E37A.root"
    "E78B7B3B-7BE8-9B47-BBEC-1283F2F6EEBA.root"
    "B1CAC940-A933-8F45-B731-9DA87D269C3A.root"
    "C1DDB0EC-371A-FB4D-B959-ABA3303AE3EC.root"
    "B69F069C-A6DE-5E4D-B01D-5F6B1CF0C8F9.root"
    "06AA898F-B18C-8046-A838-5C0F85AEB781.root"
    "5B11A2E8-C928-A545-B7F1-FDC2365F1584.root"
    "033594E9-0C8F-2D42-A8DA-34CED242C3AB.root"
    "73117211-A10B-0449-B8BB-4EA0454BF92F.root"
    "2E1FD28F-35EC-7F40-8FB4-98E0C173411C.root"
    "689A0530-47C5-504C-B35D-B99E7C3B6CBD.root"
    "77D826E4-FDBB-2A4C-BD9F-C00EAFAAC568.root"
    "9FAB370C-2BE9-9D40-9D71-7CD1CA87A82F.root"
    "19E6488B-9600-5741-BF86-CEFDFB3619DB.root"
    "818A6A3D-678B-CC46-B532-2D89E20E9EF4.root"
    "41FBC306-A5D9-2D4E-ACB5-7A0DCE1C6C63.root"
    "BAE6BF4A-A545-CC4B-97E2-9EC49C93E49A.root"
    "21148418-DBA4-F44B-9EA4-A338388921B7.root"
    "8CA98E8E-59BB-AB4E-BCD5-4D15B24858B4.root"
    "E4774D66-3066-C546-9315-D54D77040F8F.root"
    "9F464B6F-97C6-CD49-BDB8-9F57E8950F1F.root"
    "50C15117-1D84-FE4E-9AE3-CCAC8ADB98A6.root"
    "5ACE0176-897A-D84E-B234-8B5D4D5079C5.root"
    "E7C4EC5A-0960-8142-9A51-AE4797779473.root"
    "471EE8A4-5053-CE4F-8529-843155A32490.root"
    "F694B01F-8F8F-D345-BD7B-82425C7A38CB.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_second-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
