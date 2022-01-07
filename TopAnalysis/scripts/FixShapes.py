#!/usr/bin/env python
import re
from sys import argv, stdout, stderr, exit
from optparse import OptionParser
import ROOT
from ShapeHelper import *
from os import makedirs, path, rmdir
from shutil import rmtree, copyfile


dataName='data_obs'

parser = OptionParser(usage="usage: %prog [-f shapes.root]\nrun with --help to get list of options")
parser.add_option("-f", "--file",   dest="filename", default="sl-shapes.root", type="string",  help="input file")
parser.add_option("-o", "--folder",   dest="outfolder", default="test", type="string",  help="outfolder")
parser.add_option("-t", "--sigtest", dest="sigtest", default=False, action="store_true", help="switch SignificanceTest on")
parser.add_option("-k", "--KStest", dest="KStest", default=0.95, type="float", help="set KS shape Test threshold")
parser.add_option("-u", "--symm", dest="symm", default=False, action="store_true", help="switch Symmetrization on")
parser.add_option("-s", "--smooth", dest="smooth", default=1, type="int", help="set SmoothShape")
parser.add_option("-b", "--bare", dest="bare", default=False, action="store_true", help="don't apply any fix")
parser.add_option("-a", "--average", dest="average", default=True, action="store_true", help="averate shape variation among SR")
parser.add_option("-n", "--ttbarnorm", dest="ttbarNorm", default=True, action="store_false", help="Keep only shape variation for ttbar")
(options, args) = parser.parse_args()
outfolder=options.outfolder

print('Read from '+options.filename+' with the following corrections:')
if options.bare:
  print('WARNING: using bare option \'TRUE\', no corrections will be used')
else:
  if options.sigtest: print('Apply SignificanceTest()')
  if options.smooth: print('Apply SmoothShape(%d)'%options.smooth)
  if options.symm: print('Apply Symmetrizing')
  if options.KStest: print('Reject shape variation using KS Test with threshold = %2.3f'%options.KStest)
  if options.average: print('Apply Shape averaging')
  if options.ttbarNorm: print('Keep only shape variation for ttbar')
#  print('Apply Symmetrization on toppt')


f=ROOT.TFile.Open(options.filename)
if f == None: 
  print("Cannot open file %s" % options.filename)
  exit(1)

#get all processes and systematic variations from the file
rgn_names, bkg_names, sys_names = GetInfoFromFile(f)

#set output folder:
if path.exists(outfolder):
  print('Warning: the path \''+outfolder+'\' exists, overwite the data')
  rmtree(outfolder)
makedirs(outfolder)
#loop over all regions/backgrounds/systematics and fix the shapes
outifle=ROOT.TFile(outfolder+'/'+options.filename, 'RECREATE' )

# obtain shape variations averaged among all signal regions:
if options.average:
  hup_ratio_average, hdn_ratio_average = {}, {}
  for bkg in bkg_names:
    nbins=f.Get(bkg+'_'+rgn_names[0]).GetNbinsX()
    hup_ratio_average[bkg]={}
    hdn_ratio_average[bkg]={}
    for sys in sys_names:
      hup_ratio_average[bkg][sys] = [ 1 ] * (nbins+1)
      hdn_ratio_average[bkg][sys] = [ 1 ] * (nbins+1)
  GetAverageShapeRatio(f,hup_ratio_average,hdn_ratio_average)


for rgn in rgn_names:
  #saves the data histograms for each signal region
  h=f.Get(dataName+'_'+rgn)
  h.Write()
  
  # loop over the backgorunds and write the nominal shape
  for bkg in bkg_names:
    h=f.Get(bkg+'_'+rgn)
    #loop over all variations, fix and save the modified version
    for sys in sys_names:	
      hup=f.Get(bkg+'_'+rgn+'_'+sys+'Up')
      hdn=f.Get(bkg+'_'+rgn+'_'+sys+'Down')

      if options.bare:
        hup.Write()
        hdn.Write()
        continue
		
      if options.average:
        hup=ScaleToRatio(h,hup,hup_ratio_average[bkg][sys])
        hdn=ScaleToRatio(h,hdn,hdn_ratio_average[bkg][sys])

      #if options.smooth: hup, hdn = SmoothShape(h,hup,hdn,options.smooth)
      if options.sigtest: hup, hdn = SignificanceTest(h,hup,hdn)
      #if options.KStest: 
      #  for hvar in [hup, hdn]: hvar=Significance_KS_Test(h,hvar,options.KStest)
      if options.symm: hup, hdn = SymmetrizeShape(h,hup,hdn)
      h, hup, hdn = FixNegatives(h,hup,hdn)

      #apply summetrization on toppt reweighting
      if not options.symm and 'toppt' in sys and 'ttbar' in bkg: hup, hdn = SymmetrizeShape(h,hup,hdn)
      #normalize ttbar variation to nominal
      if 'ttbar' in bkg and options.ttbarNorm: hup, hdn = NormalizeShape(h,hup,hdn)

      hup.Write()
      hdn.Write()

    #save nominal shape:
    h.Write()

print('Writes: '+outifle.GetName())
print('To produce the datacards:\ncd %s\n../ProduceDatacards.py -f %s'%(outfolder,options.filename))
outifle.Write()
outifle.Close()

