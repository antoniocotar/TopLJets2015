#!/bin/bash

# Define the array of input and output files
declare -a inputs=(
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/1A8F4242-632E-CF47-A6A9-364EB6DDE5D9.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/21962C86-9C36-774F-8E05-9905277B6448.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/222A1597-611E-5043-BEDE-31C830559D9D.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/29EA70CE-630F-2347-8A43-EC55EA4E165D.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/2F737BB7-D630-0A43-B51E-15DBF4908C73.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/31F92A70-087E-5845-B596-6A13EDDBD98F.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/3FF16226-CE6F-654F-8C8A-C30D8A5484A4.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/401CE172-60C2-6F4D-935A-F38275F163E6.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/42E7C8E5-42C1-4848-B711-DD7511DC443D.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/480D388A-7F98-404D-95A9-D654AAE341C6.root"
    "/store/mc/RunIISummer20UL17MiniAODv2/APtoTT_4f_TuneCP5_13TeV_positivePhotonDirection-amcatnlo-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2810000/4BD27E2B-8A35-D245-B971-50686EDAB822.root"
)

declare -a outputs=(
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v4.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v5.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v6.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v7.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v8.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v9.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v10.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v11.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v12.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v13.root"
    "/eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/mc_ttsemi_hf/test_ttphoto_hf_v14.root"
)

# Common parameters
runOnData="False"
era="era2017_B"
doPUProtons="False"
ListVars="ttbar"

# Loop through the arrays
for i in "${!inputs[@]}"; do
    cmsRun runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=${inputs[$i]} outFilename=${outputs[$i]} doPUProtons=$doPUProtons ListVars=$ListVars
done
