#!/usr/bin/env python
import ROOT
from os import makedirs, path, rmdir
from shutil import rmtree, copyfile
from ShapeHelper import GetInfoFromFile
from optparse import OptionParser

#---------------- USER DEFINED INFO ------------------
uncorr_sys = [] # systematics uncorrelated among the signal regions
corr_sys = ['l1prefire', 'MUtrigSF', 'ELtrigSF', 'msel', 'esel', \
 'ppsSF', 'pdf_as', 'pdf_hs', 'scale_REN', 'scale_FAC',\
 'ISR_G2GG_muR', 'ISR_G2QQ_muR', 'ISR_Q2QG_muR', 'ISR_X2XG_muR',\
 'ISR_G2GG_cNS', 'ISR_G2QQ_cNS', 'ISR_Q2QG_cNS', 'ISR_X2XG_cNS',\
 'FSR_G2GG_muR', 'FSR_G2QQ_muR', 'FSR_Q2QG_muR', 'FSR_X2XG_muR',\
 'FSR_G2GG_cNS', 'FSR_G2QQ_cNS', 'FSR_Q2QG_cNS', 'FSR_X2XG_cNS',\
 'JEC_Absolute', 'JEC_Flavor', 'JEC_HighPt', 'JEC_Pileup', 'JEC_Relative',\
 'JEC_Time', 'JER', 'btaglf', 'btaghf', 'PPS56_Reco', 'PPS45_Reco','protonTag', 'UnclusteredMET'] 


#all other sys.
norm_sys = {'lumi':1.023}   # normalization unc set by the user
rate_param = {'ttbar' : 1 }
bkg_spec = {'Wt':1.05, 'Wjets':1.3,'Zjets':1.3} # background specific Xsec
#------------------------------------------------------

parser = OptionParser(usage="usage: %prog [options]\nrun with --help to get list of options")
parser.add_option("-f", "--file",   dest="filename", default="sl-shapes.root", type="string",  help="input file")
parser.add_option("-o", "--out",   dest="outfolder", default=".", type="string",  help="output folder")
parser.add_option("-k", "--ks",   dest="KSthreshold", default=0.95, type="float",  help="KS test threshold")
parser.add_option("-s", "--signal",   dest="signalname", default="excl_tt", type="string",  help="signal name")
parser.add_option("-e", "--minerr",   dest="minError", default=1.005, type="float",  help="minimal related uncertanty to store")
parser.add_option("-d", "--dataName",   dest="dataName", default="data_obs", type="string",  help="data obs. name")
parser.add_option("-w", "--withstats", dest="withStats", default=True, action="store_false", help="switch off bin statistical unc")
(options, args) = parser.parse_args()

filename=options.filename
outfolder=options.outfolder
KStest_threshold = options.KSthreshold
signal_name = options.signalname
min_error = options.minError
dataName = options.dataName
withStats = options.withStats

class SysClass:
    
  def __init__(self, region, backgrounds, shape_file):
    self.n_data = int(shape_file.Get(dataName+'_'+region).Integral())
    self.region = region
    self.processes = []
    self.rates = []
    self.shape_systematics = {}
    self.norm_systematics = {}
    self.n_sys=len(norm_sys)+len(bkg_spec)
    
    #signal
    hs = shape_file.Get(signal_name+'_'+region)
    self.processes.append(signal_name)
    self.rates.append(hs.Integral())
    
    #set indeces for background proceses:
    self.proc_indx={}
    self.proc_indx[signal_name]=0
    for i, bg in enumerate(backgrounds): self.proc_indx[bg]=i+1
	
    #calculate the total background yields (to discard insignificant backgrounds)
    Ntotal = 0.
    for bkg in backgrounds:
      hn=shape_file.Get(bkg+'_'+region)
      nevents=hn.Integral()
      Ntotal+=nevents
	  
    #backgrounds
    for bkg in backgrounds:
      hn=shape_file.Get(bkg+'_'+region)
      nevents=hn.Integral()
      if nevents<0.001*Ntotal: continue
      self.processes.append(bkg)
      self.rates.append(nevents)
    self.n_proc = len(self.processes)
      
    #systematic variations
    for sys in corr_sys+uncorr_sys:
      _shp_sys={}; _nrm_sys={}
      for pr in self.processes:
        hn=shape_file.Get(pr+'_'+region)
        hup=shape_file.Get(pr+'_'+region+'_'+sys+'Up')
        hdn=shape_file.Get(pr+'_'+region+'_'+sys+'Down')
        _isShape, _uncVal=self.AnalyseSystematics(hn, hup, hdn)
        _shp_sys[pr] = True if _isShape else False
        _nrm_sys[pr] = 1 if _isShape else _uncVal
      self.shape_systematics[sys]= _shp_sys
      self.norm_systematics[sys] = _nrm_sys

    #count systematics
    for sys in corr_sys+uncorr_sys:
      if not self.isPruned(sys): self.n_sys+=1
	
  def isShape(self, sys):
    return sum(self.shape_systematics[sys].values())

  def isPruned(self, sys):
    if self.isShape(sys): return False
    return sum([x>min_error for x in self.norm_systematics[sys].values()])==0

	
  def AnalyseSystematics(self, nom, up, dn):
    Nnom = nom.Integral()
    if not Nnom: return False, 1
    if 'toppt' in up.GetName() and 'ttbar' not in nom.GetName(): return False, 1 
    Nup = up.Integral()
    Ndn = dn.Integral()
    norm_err = 0.5*max(abs(Nnom - Nup),abs(Nnom - Ndn))/Nnom
    kup = nom.KolmogorovTest(up)
    kdn = nom.KolmogorovTest(dn)
    return (kup<KStest_threshold or kdn<KStest_threshold), 1+norm_err

  def WriteRates(self, fileout):
    fileout.write('\n')
    fileout.write('bin\t\t')
    for i in range(self.n_proc): fileout.write(self.region+'\t')
    fileout.write('\n')
    fileout.write('process\t\t')
    for pr in self.processes: fileout.write('%d\t\t'%self.proc_indx[pr])
    fileout.write('\n')
    fileout.write('process\t\t')
    for pr in self.processes: fileout.write('%s\t\t'%pr)
    fileout.write('\n')
    fileout.write('rate\t\t')
    for r in self.rates: fileout.write('%2.3f\t\t'%r)
    fileout.write('\n')
  
  def WriteSystematics(self, fileout):
  
    #normalization systematics (usually only lumi)
    for sys in norm_sys:
      fileout.write(sys+'\tlnN\t')
      for i in range(self.n_proc): fileout.write('%2.3f\t\t'%norm_sys[sys])
    fileout.write('\n')

    #background Xsec systematics
    for sys in bkg_spec:
      fileout.write(sys+'Xsec\tlnN\t')
      for i in range(self.n_proc): 
        if sys in self.processes[i]: fileout.write('%2.3f\t\t'%bkg_spec[sys])
        else: fileout.write('-  \t\t')
      fileout.write('\n')
	
    #systematics (shape/norm)
    for sys in corr_sys+uncorr_sys:
      if self.isPruned(sys): continue
      if sys in uncorr_sys: fileout.write(sys+region+'\t')
      else: fileout.write(sys+'\t')
      
      if self.isShape(sys): # shape systematics
        fileout.write('shape\t')
        for pr in self.processes:
          _str = '1' if (self.shape_systematics[sys][pr] or (self.norm_systematics[sys][pr]>min_error)) else '-'
          fileout.write(_str+'\t\t')
      else:                 # normalization systematics
        fileout.write('lnN\t')
        for pr in self.processes:
          _str = '-' if (self.norm_systematics[sys][pr]<min_error) else ('%2.3f'%self.norm_systematics[sys][pr])
          fileout.write(_str+'\t\t')
      fileout.write('\n')
    
  def WriteHeader(self, fileout):
    fileout.write('\n')
    fileout.write('imax 1  number of channels\n')
    fileout.write('jmax '+str(self.n_proc-1)+'  number of backgrounds\n')
    fileout.write('kmax '+str(self.n_sys)+' number of nuisance parameters (sources of systematical uncertainties)\n')
    fileout.write('-----------\n')
    fileout.write('shapes * * '+filename+' $PROCESS_'+self.region+' $PROCESS_'+self.region+'_$SYSTEMATIC\n')
    fileout.write('-----------\n')
    fileout.write('#\n')
    fileout.write('# here we write the total number of events observed in '+self.region+'\n')
    fileout.write('#\n')
    fileout.write('bin         %s\n'%self.region)
    fileout.write('observation %d\n'%self.n_data)
	
def WriteDataCard(region, backgrounds, shapes):
  data = SysClass(region, backgrounds, shapes)
  with open(outfolder+"/datacard_"+region+".dat","w") as fout:
    data.WriteHeader(fout)
    fout.write('-----------\n')
    fout.write('#\n')
    fout.write('# here we write the expected yields\n')
    fout.write('# for '+signal_name+' as we are fitting the cross section so we write A x Lumi in the SR and 0 otherwise\n')
    fout.write('#\n')
    data.WriteRates(fout)
    fout.write('-----------\n')
    data.WriteSystematics(fout)
    fout.write('-----------\n')
    for pr in rate_param:
      fout.write('mu_'+pr+'\trateParam * '+pr+'\t'+str(rate_param[pr])+'\t[0,10]\n')
    if(withStats): fout.write(region+' autoMCStats 0 0 1\n')
    fout.write('-----------\n')
	
print('Produce datacards from '+filename)

#read the input file
f=ROOT.TFile.Open(filename)
if f == None: 
  print("Cannot open file %s" % filename)
  quit()

#create output folder (if needed)
WriteHere = True if outfolder=='.' else False
if WriteHere: print('Writes datacards here')
else:
  if path.exists(outfolder):
    print('Warning: the path \''+outfolder+'\' exists, overwite the data')
    rmtree(outfolder)
  makedirs(outfolder)

#read regions and backgrounds
rgn_names, backgrounds, sys_debug = GetInfoFromFile(f)
backgrounds.remove(signal_name)
backgrounds.sort()

#Validate names of systematic variations
for sys in corr_sys+uncorr_sys:
  if sys not in sys_debug:
    print('ERROR: check the list of input systematics; the variation \''+
      sys+'\' is not present among the list of available systematics:')
    print(sys_debug)
    quit()

#Create datacards per regions	
#rgn_names=['SR2017F120']
for region in rgn_names:
  WriteDataCard(region,backgrounds,f)

if not WriteHere:
  print('Copy %s to %s'%(filename,outfolder) )
  copyfile(filename,outfolder+'/'+filename) 

print('done, to combine the datacards execute:')
combind_cmd='combineCards.py '
for region in rgn_names:
  combind_cmd+=region.replace('2017','')+'='+outfolder+"/datacard_"+region+".dat "
combind_cmd+="> sl_datacard.dat"
print(combind_cmd)
print('text2workspace.py sl_datacard.dat -o ws.root')
