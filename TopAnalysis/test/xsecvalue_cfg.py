import FWCore.ParameterSet.Config as cms

process = cms.Process("XSEC")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("GeneratorInterface.Core.genFilterSummary_cff")
process.load("GeneratorInterface.Core.genXSecAnalyzer_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/TT-pomflux-H1FitB_TuneCP5_13TeV-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v2/2820000/0C93E2EC-27FE-1540-982D-16065D1E48B8.root'
    )
)

process.analyzer = cms.EDAnalyzer("GenXSecAnalyzer")

process.p = cms.Path(process.analyzer)


