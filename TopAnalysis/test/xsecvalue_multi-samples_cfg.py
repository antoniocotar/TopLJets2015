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

        # =========================================================
        # TTToSemiLeptonic
        # =========================================================
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/00000/005708B7-331C-904E-88B9-189011E6C9DD.root',
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/00000/00623789-C012-474B-8860-3C18397A464F.root',
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/00000/0074902D-0DEC-7946-9901-309B04400A3F.root',

        # =========================================================
        # TTTo2L2Nu
        # =========================================================
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/00000/0004BE39-823E-4A4B-9727-C2544050C4C0.root',
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/00000/007382A7-8197-B147-8A03-0F0DEC82AF4B.root',
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/00000/035AC6FC-5D08-B74D-99A4-33A1BB6AB6DD.root',

        # =========================================================
        # ST_tW_top
        # =========================================================
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/120000/0FF75470-1C09-7045-9A19-A71A41BF98E6.root',
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/120000/13B12AF4-598C-D644-90A6-F072CFFA63EF.root',
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/120000/1A1DC724-6BAE-C547-B6B0-58FFAB36EE38.root',

        # =========================================================
        # ST_tW_antitop
        # =========================================================
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/110000/071FD53F-118E-7B44-A435-78801CC6F08D.root',
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/110000/0874B8B0-1C3D-5F4B-A340-EE1CE35AF08F.root',
        'root://cms-xrd-global.cern.ch//store/mc/RunIISummer20UL17MiniAODv2/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/106X_mc2017_realistic_v9-v1/110000/08C533BF-619B-0E4F-B5AC-A80B24DFA555.root'
    )
)

process.analyzer = cms.EDAnalyzer("GenXSecAnalyzer")

process.p = cms.Path(process.analyzer)