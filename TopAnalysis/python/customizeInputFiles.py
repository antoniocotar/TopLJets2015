import FWCore.ParameterSet.Config as cms

def customTestInputFiles(process,era,runOnData,runWithAOD):
    if '2016' in era:
        if runOnData:
            process.source.fileNames = cms.untracked.vstring('/store/data/Run2016B/SingleMuon/MINIAOD/17Jul2018_ver2-v1/00000/0219DD4A-7D8C-E811-B7EB-001E67248A43.root')
            if runWithAOD:
                print 'Adding secondary filenames'
                process.source.secondaryFileNames = cms.untracked.vstring(['/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/110000/B6479036-9486-E711-A6CF-003048FFD734.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/110001/0A8A7032-9086-E711-BD1B-0025905A60C6.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/110001/22FFA42A-9086-E711-9B49-0CC47A7C3472.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70000/767AE9A5-BA81-E711-9A8E-0CC47A745282.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70000/D67D18A5-BA81-E711-B6C2-0CC47A7C361E.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70000/E06164A7-BA81-E711-B612-0025905B858A.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/302E2840-C481-E711-8913-002618FDA259.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/687FA66D-BC81-E711-B41D-0CC47A745282.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/6890466C-BC81-E711-9C62-0CC47A4C8E1C.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/E64E346F-BC81-E711-93BC-0CC47A4C8F18.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/EC3632F7-C081-E711-B219-0CC47A7452DA.root',
                                                                           '/store/data/Run2016B/SingleMuon/AOD/07Aug17_ver2-v1/70001/F6499DE0-C281-E711-9835-0CC47A78A33E.root'])
            else:
                process.source.fileNames = cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv3/ST_t-channel_antitop_4f_mtop1715_inclusiveDecays_13TeV-powhegV2-madspin-pythia8/MINIAODSIM/PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/120000/16CEB785-3FE6-E811-AAE8-FA163E9D74F8.root')
    elif '2017' in era:
        if runOnData:
            process.source.fileNames = cms.untracked.vstring('/store/data/Run2017H/SingleMuon/MINIAOD/17Nov2017-v2/90000/FA9FA831-8B34-E811-BA1D-008CFAC93CFC.root')
            if runWithAOD:
                print 'Adding secondary filenames'
                process.source.secondaryFileNames = cms.untracked.vstring([
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/C223F6C8-EAD1-E711-BA08-02163E014142.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/261E9645-EDD1-E711-A59B-02163E01A5C6.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/383AE474-ECD1-E711-8869-02163E014206.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/24D6AED6-ECD1-E711-B698-02163E01A3DF.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/1E877022-EED1-E711-84A8-02163E01A473.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/10B7D572-E9D1-E711-89D3-02163E019BCA.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/020E8546-EBD1-E711-97E6-02163E014328.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/08A0ECBE-EBD1-E711-AD94-02163E011D0F.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/001B0186-E8D1-E711-BAB4-02163E019E63.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/7222AC6A-E4D1-E711-A16B-02163E01A2C6.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/7AE2E3E3-E8D1-E711-B6F9-02163E014159.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/3062E753-E0D1-E711-ACC3-02163E011A80.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/C41308C7-EDD1-E711-A257-02163E01A255.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/E871EBD4-EDD1-E711-874F-02163E0142C7.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/FE685172-E7D1-E711-827B-02163E0127CE.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/6097563B-EDD1-E711-9A3D-02163E011D03.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/400CA754-EDD1-E711-8947-02163E0134BF.root',
                    '/store/data/Run2017H/SingleMuon/RAW/v1/000/307/076/00000/3E805EF3-E8D1-E711-8FC4-02163E011B83.root'
                ])
        else:
            process.source.fileNames = cms.untracked.vstring('/store/mc/RunIISummer19UL17MiniAOD/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/106X_mc2017_realistic_v6-v2/270000/2927EC69-BA37-F64D-8057-040C81E11C06.root')
    elif '2018' in era:
        if runOnData:
            process.source.fileNames = cms.untracked.vstring('/store/data/Run2018C/SingleMuon/MINIAOD/17Sep2018-v1/00000/AB61FB4F-3A42-4B4F-93E2-78CD7E7CF0A4.root')
            if runWithAOD:
                print 'Adding secondary filenames'
                process.source.secondaryFileNames = cms.untracked.vstring([
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/C45A09EB-328E-E811-BFEC-FA163EDD3A25.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/0A3EDD77-128E-E811-8885-FA163E08D4C1.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/1E0CFF73-128E-E811-9B2B-FA163E6DCA76.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/9659EBAF-108E-E811-93CB-FA163E82172A.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/025/00000/F05577B5-568D-E811-A9FA-FA163E513903.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/026/00000/26D29DBA-5F8D-E811-9D7B-FA163E600F07.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/026/00000/9239C9D4-5F8D-E811-BD2C-FA163ECED2A8.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/065/00000/BEED33AC-328E-E811-8305-FA163EDE7DC6.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/025/00000/C662E202-578D-E811-8E1E-FA163E7FB452.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/024/00000/F4B08B5F-528D-E811-965A-FA163ED40F48.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/024/00000/0863DA3E-548D-E811-B8E1-A4BF01277823.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/009/00000/0218B251-F28C-E811-81CD-FA163ED486E3.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/010/00000/768D18BE-F98C-E811-A332-FA163E60ED2F.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/007/00000/D208B88E-E98C-E811-9B7F-FA163E61ADEA.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/007/00000/DAA30463-E98C-E811-918B-FA163EF70A72.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/010/00000/7A33E7D8-F78C-E811-AF32-FA163E3AFBF6.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/319/854/00000/A4D6882F-768A-E811-9978-FA163E11A4DA.root',
                    '/store/data/Run2018C/SingleMuon/RAW/v1/000/320/007/00000/B4B0ADA6-E88C-E811-B65F-FA163E105698.root'
                ])
        else:
            print 'FIXME: add a MC test file for 2018...'
