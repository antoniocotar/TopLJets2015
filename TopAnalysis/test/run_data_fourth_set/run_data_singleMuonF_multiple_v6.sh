#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/fourth_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=710

# Array of unique parts of input file paths
inputFiles=(
"9434ACA8-DCB7-B148-8AA4-9889D3C08178.root"
"BC2022B1-CA3B-5B4D-A6B1-359998349188.root"
"6ECC2196-E141-9D41-B06A-4CF3C43B6CBD.root"
"6F45CB90-DF2E-B84F-A4DB-C5806E68F67C.root"
"E4999E2B-A855-D744-8252-F3764E6920E6.root"
"5C17041C-AF5E-834D-8EC5-33B71BEB66BC.root"
"951B05FD-56B9-2341-8D94-E76E95798EDF.root"
"8EC8D27C-C393-034A-A19C-8654BEBAD51E.root"
"5489AB91-8692-6F4C-BFC4-CF17B400D94B.root"
"A70C903A-E7DA-A24C-8201-101FEBF2FB0C.root"
"A021373C-AFE5-D642-B026-2C8D37638634.root"
"7406F9E0-E98C-B542-B8D7-E07854ABF8EF.root"
"0FF45866-6EB5-1943-92A3-D91F2C125453.root"
"233E386E-5B98-7B46-A6E5-33F36B41B4DC.root"
"AF28F7A2-73DA-F940-B3FC-879FA899BB33.root"
"69FC19C7-9CFB-6146-8FBC-E4EADC3E43F2.root"
"2BC6007C-32A6-584F-BC19-2F5241892A1D.root"
"309498A4-CDB5-6D47-B36D-A768A6223869.root"
"7B612DDD-F743-994B-AD65-6001893AC9CA.root"
"973C36B4-95C2-A14A-8A4B-56321D6D2F61.root"
"2F2393DA-3946-844F-93A4-F3AECA3359BA.root"
"B40FA640-96A8-5D42-8F92-455C94319190.root"
"08EE5E40-FC35-B645-89B4-0C1575786F9F.root"
"A1597D6C-FCFE-764E-A6D0-2F40D51B1AC0.root"
"8DF43DA5-0424-034F-81F0-0679214B9D17.root"
"BB36486C-9108-2443-AC88-308E53517AA8.root"
"D151BBBB-E102-4549-ACC4-6AD6E9BAC392.root"
"631C2A7A-C2D3-FF44-A418-2AA0873E53D6.root"
"58652075-6AB6-7444-971C-E1BA55329A9A.root"
"5B79B8E3-6995-B643-91D8-C2A5BE8D15B8.root"
"687D7FB5-0793-EA4B-AD66-FAF2CDDCBF3A.root"
"E821FDC1-D03F-9C45-91C8-4A983A9B0408.root"
"2184CEAD-8619-2F41-A524-30A17700CD29.root"
"24C0CCFC-F4DA-D947-B17B-B5E544CE6A5F.root"
"65E04C5C-BB8E-E84A-AE32-FC6DA9863D3E.root"
"C258BFB7-9D8C-5A42-9297-2ABDD573EB50.root"
"B4E86D35-1096-8C4B-B4FA-F6AE1E1AC5AA.root"
"AAB767CE-AC00-2D4A-B9C6-CFD0BDD49148.root"
"31B213AF-558E-C84C-B3A1-25C2F91F6BB7.root"
"6B394CBD-19CA-1648-BA03-C697363CFCDA.root"
"82D940B8-7C9B-AA47-8D2B-5475DABEEA8C.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_fourth-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
