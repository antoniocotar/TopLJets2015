#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/second_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=160

# Array of unique parts of input file paths
inputFiles=(
    "2271EAAF-DF6A-7B4B-9D9D-AA618BDFD8C8.root"
    "55DAE913-60FF-AC49-833B-7361817B89AA.root"
    "07505977-2801-3049-B73B-47816E89B2A5.root"
    "BABEEF4C-56DF-004C-B23A-23F9BB3B6D7F.root"
    "40988508-2EA2-7F44-ADE1-DFA0181361FB.root"
    "D6C354B9-11E1-A547-BA16-99AF665D9F06.root"
    "630D2CA1-381A-8D4D-9A0B-BCC78571E5DC.root"
    "4F05153D-CA79-1B41-B80C-2C8A459A1FBB.root"
    "E5666CE8-7833-DA49-A90B-22A22052EAA9.root"
    "0866E75F-F5F5-B94B-A715-15FB0EE814F5.root"
    "801F1DA4-F053-C042-B38F-C429839A922C.root"
    "9AD676AA-CB78-1F44-BFA0-9C7E8BB6EBA2.root"
    "A579BA4F-8ED5-4542-B173-77D897DB2241.root"
    "14C96E30-F70E-1F49-ABD4-757BDE3BCE99.root"
    "75791CD3-09EC-C347-8AEC-8DE265F7785E.root"
    "8C9082A3-B2F1-8646-96E7-F07C67FCFF10.root"
    "A0C6AF8E-B4D5-ED42-B55F-07C7A22B4845.root"
    "2A3E989A-4EA8-EA4D-8E3D-AE34B3ADC829.root"
    "31CDE531-8F3F-EA4D-9C6A-670316EDB440.root"
    "8B78D534-4041-9649-851B-4F48744A1A8A.root"
    "C4092BFE-F64E-0B4E-8569-CA57D4F7ABD0.root"
    "B84D8A08-8BEE-4547-8C03-7A58DC027906.root"
    "D07C60B8-93C6-7741-8E67-621D6CB4670F.root"
    "6F788EB1-4E71-584C-98C4-25B224A6618C.root"
    "E07531F1-D5CE-9448-9196-B8F865977486.root"
    "2DC31779-331A-EA4E-ADF8-9FDE206F11BA.root"
    "922B1B94-14D4-2545-AF33-DF267D087AAB.root"
    "299F6777-DCD9-D844-A7DD-87F125D0BCAB.root"
    "2F2E5A69-BF0B-0E4B-AACD-FC1E92B70D25.root"
    "0F45CB54-D365-2249-8009-9DBAF7DB5BC9.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_second-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
