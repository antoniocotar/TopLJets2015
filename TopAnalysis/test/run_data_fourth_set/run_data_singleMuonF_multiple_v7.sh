#!/bin/bash

# Common parameters
runOnData="True"
era="era2017_F"
ListVars="ttbar_data"
outputBasePath="/eos/home-a/acotarod/Thesis/Updates/Updates_Antonio/february_2024/minievents_v1/v1_hf_ntrk/data/fourth_set"
inputBasePath="/store/data/Run2017F/SingleMuon/MINIAOD/UL2017_MiniAODv2-v1/280000"
counter=750

# Array of unique parts of input file paths
inputFiles=(
"82D940B8-7C9B-AA47-8D2B-5475DABEEA8C.root"
"FEEFA6C3-CCDD-7240-9AED-4D0A36F9CE87.root"
"AEC84004-D35B-024E-8DAA-7BC253005D86.root"
"FF8A3BA6-B0D8-1B4D-AEFB-E67386C1C357.root"
"34C37559-AC19-2147-8B30-027180F2413B.root"
"499C343F-D507-F740-85A5-A0CD6AECDCF2.root"
"EC4F64D7-E4F9-7846-A78A-54B9254B38D6.root"
"926FFF05-4A68-8041-83D2-15A2104E8076.root"
"A312E916-261C-BD40-A14A-DC03681D61E4.root"
"49BD0021-F29E-7C44-B54C-D1659D9DB1CE.root"
"EBAFBB45-A6BC-4041-A923-8F35A3B363BD.root"
"9865DD6A-B642-E142-B0BB-B15E12CDB135.root"
"E58CCDC8-4579-F04B-8C48-97C17A1A946A.root"
"712209F7-E4DB-4043-85EA-D12467556ADD.root"
"18551567-C69F-024C-B9E4-AD684AB6E510.root"
"4955C18B-3FD0-BA41-8153-E7F8292D0979.root"
"B3F37C37-877B-E44F-AB5A-86227EA48A38.root"
"229173E7-5073-574C-8D9D-7FD6F2040019.root"
"F26BE374-F55F-3845-A924-208B0F3F8474.root"
"E693B153-09C4-544C-844B-AB4679A8ECF1.root"
"0B39B064-4FB1-D745-9575-0FE2946C2155.root"
"BCC12CED-BA2B-1D4E-829C-EBF658C762B4.root"
"E76DF8D1-34CB-384E-A458-AF697E33B261.root"
"4529150D-7527-7349-A2EA-10F8B25404EF.root"
"8D920B50-C8E9-9B46-B92B-107B82FF9AC5.root"
"7CBB9C3E-8614-634C-8EB9-C710329CBEC1.root"
"2EF03317-8FFB-DF4B-876B-6D2D2C7E743F.root"
"A89FC36C-4615-3E47-BFF4-1A5C280E148A.root"
"3559066F-F67D-B948-9F75-7D7FD2130492.root"
"9E87D23F-0F77-8B4D-94EE-F4DA07538F01.root"
"CDDB9F56-B33D-3243-B9B5-5C4E216D2C2C.root"
"B63DCD6F-DEAC-4B44-A750-6A5B6E65E7EA.root"
"3511DF11-8535-7946-B061-9788B8C30217.root"
"A2F0B70D-5183-9B41-B54F-A059EC6C0FD9.root"
"9F5C3EB7-D3B2-6D4D-8E7E-2CCC32733D62.root"
"50E230B0-AFF7-1F48-9B20-CB6FAC47236A.root"
"9D2463A8-A111-634B-92CD-4E487103B964.root"
"16D7BC5A-1AA6-8B4F-8A32-14D45D651A6F.root"
"2E576764-F970-5349-9682-652B1F886F00.root"
"8A2BF2E2-9EB2-7247-89E6-8747D785934A.root"
"BBEEEBA5-C6B4-F24E-AB0B-4FCF1259E927.root"
)

# Loop through the input files
for file in "${inputFiles[@]}"; do
    inputFile="${inputBasePath}/${file}"
    outputFile="${outputBasePath}/test_SingleMuon_B_hf_ntrk_fourth-set_v${counter}.root"
    cmsRun /eos/home-a/acotarod/Thesis/CMSSW_MP_PROTON/CMSSW_10_6_X/CMSSW_10_6_20/src/TopLJets2015/TopAnalysis/test/runMiniAnalyzer_cfg.py runOnData=$runOnData era=$era inputFile=$inputFile outFilename=$outputFile ListVars=$ListVars
    ((counter++))
done
