#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/second_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=70

# Array of unique parts of input file paths
inputFiles=(
    "9D1D6F57-CADD-AA4A-8A22-0F344621429A.root"
    "C9A5DFFD-5340-444F-918B-3E497FBBEE50.root"
    "08BD4A67-0B01-AB4C-9E9C-F9FD8D5B9651.root"
    "18D29DAF-E308-BB42-B62D-54D1976D5D28.root"
    "28219A4B-EC16-5A4D-A207-95F58A9D3194.root"
    "24D79F27-B3FF-0746-85E7-E4B24F3DB60E.root"
    "C8A2F53C-5621-EF45-8A92-7DF26D99475B.root"
    "16830C51-63D9-324D-B5A5-54911FB42AC5.root"
    "8A76CF92-2072-D24B-B601-A975B1566D83.root"
    "DF452ADD-B1ED-934F-8F04-4923F15978D5.root"
    "ECFFFFD0-C573-4849-8378-5A21D132BC8B.root"
    "34FDEC05-85C5-6949-9851-513F0D50ABFF.root"
    "E15FA11F-2146-9641-B3F2-507EC7D31012.root"
    "8FA2A3BE-A904-3045-9164-D35D93179E1A.root"
    "212E0A8C-A1CF-744C-9C67-4892A9A26DC7.root"
    "16D36409-A086-7D47-BA73-79265CCE4054.root"
    "09F4BE3D-F52F-B044-BFB9-B2CA68805582.root"
    "C925D66D-BF2A-BD47-9B29-8BE87B08A482.root"
    "053E1D06-09EE-744A-901F-3C692165AF5F.root"
    "C0FAEF52-6ACD-DF48-BCFA-C493534FFA5E.root"
    "4DC22FEB-9F5E-5641-9274-CB47691F2FD2.root"
    "65813329-0F37-F645-B673-41298C6AF307.root"
    "1794B251-84C5-8347-81D8-5F9943BC33EB.root"
    "DB5A32A7-C81E-6F4A-B747-EF5754FDF649.root"
    "5CBC4CC2-E978-3846-A864-E24B219C5AD7.root"
    "E0B3AF95-27CF-7A41-965F-29FEE57ADA93.root"
    "96E7F7A1-4658-4841-825B-2C20673C26E3.root"
    "031D31EC-5479-A641-B29C-6B06666C4BE6.root"
    "79F8A3AA-CD40-EF43-A177-9A82F53749C1.root"
    "5D18506F-8BBB-C746-B570-0A854E159FF0.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_second-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
