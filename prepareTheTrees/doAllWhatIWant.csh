#!/bin/csh

echo "fit jobs" | mail -s "Hello !!! the Data" hbrun@cern.ch
set LOCALDIR = `pwd`
setenv WORKINGDIR /afs/cern.ch/user/h/hbrun/CMSSW_5_3_2_patch2/src
setenv SCRAM_ARCH slc5_amd64_gcc462
setenv LOCAL_PATH TagAndProbeDirs/fitMuonsAll2012/latestStudies/addLeaf 
setenv FINAL_DIR /afs/cern.ch/work/h/hbrun/pogTnPr8
setenv TEMP_DIR /afs/cern.ch/work/h/hbrun/tempDir
setenv EOS_DIR root://eoscms//eos/cms/store/caf/user/gpetrucc/TnP/V5

set listFichier = (tnpZ_Run2012A.root tnpZ_Run2012B.root tnpZ_Run2012C.root tnpZ_Run2012D.root)
set listNomPart = (runA               runB               runC               runD)

cd $WORKINGDIR/$LOCAL_PATH
cmsenv

root -l -b -q $EOS_DIR/$listFichier[$1] loadMVAlibs.cxx addMVAIso.cxx+
mv /tmp/hbrun/tnpZ_withMVAIso.root $TEMP_DIR/tnpZ_withMVAIso$1.root
root -l -b -q $TEMP_DIR/tnpZ_withMVAIso$1.root addMuonID.cxx+
rm $TEMP_DIR/tnpZ_withMVAIso$1.root
mv /tmp/hbrun/tnpZ_withMuID.root $TEMP_DIR/tnpZ_withMuID$1.root
root -l -b -q $TEMP_DIR/tnpZ_withMuID$1.root createReducedTree.cxx+
rm $TEMP_DIR/tnpZ_withMuID$1.root
mv /tmp/hbrun/tnpZ_shortTree.root $FINAL_DIR/TnP_Data_$listNomPart[$1].root

