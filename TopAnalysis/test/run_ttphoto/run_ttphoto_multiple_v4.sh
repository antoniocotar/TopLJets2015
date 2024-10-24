#!/bin/bash

# Common parameters
runOnData="False"
era="era2017_F"
doPUProtons="False"
ListVars="ttbar"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/ttphoto"
inputBasePath="/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000"
counter=29

# Array of unique parts of input file paths
inputFiles=(
    "76F50AA8-0BC7-044A-B2C6-0D9E4CDD52E7.root"
    "7C71B6FE-20B8-0044-B043-7632E67ED7BD.root"
    "80675388-5C77-3F42-AFCE-4C4A9E587D11.root"
    "86C2343A-0943-724A-BF3C-0FA76AC586DC.root"
    "894FE509-764A-F54B-96E3-137AE813B8EB.root"
    "909D15C8-A870-3B40-9591-6D231833988F.root"
    "957F765B-D851-DC40-9F5D-CB64D37A78DB.root"
    "A2445811-351A-764D-B448-6B8BAF75DBC5.root"
    "AA668A25-7F5A-E94B-B384-2D9433DBE070.root"
    "ADCB8541-33AE-2042-8250-B592CC7B26D4.root"
    "B362D361-85EA-D44B-B628-77D6206B57BC.root"
    "C3832EF2-8FED-0844-85C2-099C5A72A0AF.root"
    "C9DE86A0-293C-5148-8A55-50D30DB14945.root"
    "CAD990D6-7752-0842-98C3-B86D3F000B8D.root"
    "D7895258-3F99-684F-B9E3-4C6EC339AB2D.root"
    "D936ED89-DCF2-0049-AE38-EAE4D2805411.root"
    "DA8A1055-D44F-FD4B-A180-64D62FD030D1.root"
    "E322084A-C786-8646-9F27-44F3FC0E0866.root"
)



# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_ttphoto_hf_ntrk_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile doPUProtons=$doPUProtons ListVars=$ListVars
    ((counter++))
done
