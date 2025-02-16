#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
from array import array
import optparse
import sys

def saveExpectedBtagEff(opt):
    # Prepare data chain
    input_list = [opt.input]
    data = ROOT.TChain("analysis/tree")
    for i in xrange(0, min(5, len(input_list))):
        data.Add(input_list[i])
    print "Projecting tagging efficiency from {} events".format(data.GetEntries())

    # Define pT binning
    ptBins = [0, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 200, 250, 300, 400, 500, 600, 800, 1000]
    preTagH = ROOT.TH1F("preTagH", ";p_{T} [GeV];", len(ptBins)-1, array('d', ptBins))
    preTagH.Sumw2()
    tagH = preTagH.Clone("tagH")
    
    # Define taggers and jet-flavor conditions.
    # For this example:
    #   b jets: abs(j_hadflav)==5
    #   c jets: abs(j_hadflav)==4
    #   udsg jets: j_hadflav==0
    taggerList = [x for x in opt.taggers.split(',')]
    flavConds = [
        ("b", "abs(j_hadflav)==5"),
        ("c", "abs(j_hadflav)==4"),
        ("udsg", "j_hadflav==0")
    ]
    effgrs = {}

    for x in taggerList:
        tagger, op, opval = x.split(":")
        print "Starting tagger: {}".format(x)
        # Use the provided cut on j_deepjet for efficiency
        cut = "j_deepjet>{}".format(opval)
        effgrs[(tagger, op)] = {}

        for flav, cond in flavConds:
            print "  Processing flavour {} with condition {}".format(flav, cond)
            preTagH.Reset("ICE")
            tagH.Reset("ICE")
            
            # Fill histograms
            data.Draw("j_pt >> preTagH", cond, "goff")
            data.Draw("j_pt >> tagH", cond + " && " + cut, "goff")
            
            # Count empty bins in preTagH (bins with zero entries)
            nbins = preTagH.GetNbinsX()
            emptyBins = []
            for i in range(1, nbins+1):
                if preTagH.GetBinContent(i) == 0:
                    emptyBins.append(i)
            if emptyBins:
                print "    Warning: {} of {} bins are empty in preTagH (bins: {})".format(len(emptyBins), nbins, emptyBins)
            else:
                print "    All {} bins have entries in preTagH".format(nbins)
            
            # Create efficiency graph
            effGraph = ROOT.TGraphAsymmErrors()
            effGraph.Divide(tagH, preTagH)
            npoints = effGraph.GetN()
            if npoints != nbins:
                print "    Efficiency graph has {} points ({} bins skipped)".format(npoints, nbins - npoints)
            else:
                print "    Efficiency graph has {} points (all bins)".format(npoints)
            
            effgrs[(tagger, op)][flav] = effGraph

    # Jet energy response projection per flavour (minimal debug print)
    print "Projecting jet energy response per flavour"
    ptVar = "j_pt"
    genPtVar = "g_pt[j_g]"
    respgrs = []
    respH = ROOT.TH2F("respH", ";p_{T} [GeV]; p_{T,rec}/p_{T,gen};", len(ptBins)-1, array('d', ptBins), 1000, 0., 5)
    respH.Sumw2()
    for flav, cond in flavConds:
        respH.Reset("ICE")
        data.Draw("{0}/{1}:{0} >> respH".format(ptVar, genPtVar), cond + " && j_g>=0", "goff")
        gr = ROOT.TGraphAsymmErrors(respH.ProfileX())
        gr.SetName("jresp_{}".format(flav))
        respgrs.append(gr)
    
    # Save results to file
    fOut = ROOT.TFile.Open(opt.output, "RECREATE")
    for key in effgrs:
        outDir = fOut.mkdir("{}_{}".format(key[0], key[1]))
        outDir.cd()
        for flav in effgrs[key]:
            effgrs[key][flav].Write(flav)
        fOut.cd()
    for gr in respgrs:
        gr.Write()
    fOut.Close()

def main():
    usage = "usage: %prog [options]"
    parser = optparse.OptionParser(usage)
    parser.add_option("-i", "--in", dest="input",
                      default="/store/cmst3/group/top/RunIIReReco/f93b8d8/MC13TeV_2017_TTJets",
                      help="Input ROOT file or directory")
    parser.add_option("-o", "--out", dest="output",
                      default="expectedBtagEff.root",
                      help="Output ROOT file")
    parser.add_option("--taggers", dest="taggers",
                      default="DeepJet:loose:0.0532,DeepJet:medium:0.3040,DeepJet:tight:0.7476",
                      help="Comma-separated list of tagger:workingPoint:cut")
    (opt, args) = parser.parse_args()
    saveExpectedBtagEff(opt)

if __name__ == "__main__":
    sys.exit(main())
