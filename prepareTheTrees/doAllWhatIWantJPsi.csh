#!/bin/csh

echo "fit jobs" | mail -s "Hello !!! the Data" hbrun@cern.ch
set LOCALDIR = `pwd`
setenv WORKINGDIR /afs/cern.ch/user/h/hbrun/CMSSW_5_3_2_patch2/src
setenv SCRAM_ARCH slc5_amd64_gcc462
setenv LOCAL_PATH TagAndProbeDirs/fitMuonsAll2012/latestStudies/addLeaf 
setenv FINAL_DIR /afs/cern.ch/work/h/hbrun/pogTnPr7_JPsi
setenv TEMP_DIR /afs/cern.ch/work/h/hbrun/tempDir
setenv EOS_DIR root://eoscms//eos/cms/store/caf/user/gpetrucc/TnP/V4

#                  1                                2                               3                                   4                      5                                6                                   7
set listFichier = (tnpJPsi_run2012A_13Jul2012.root tnpJPsi_run2012B_13Jul2012.root tnpJPsi_run2012Cv1_24Aug2012.root tnpJPsi_run2012Cv2.root tnpJPsi_run2012D_upTo207898.root tnpJPsi_run2012D_207899-208686.root tnpJPsi_MC53X.root)
set listNomPart = (JpsirunA                        JpsirunB                        JpsirunCp1                        JpsirunCp2              JpsirunDp1                       JpsirunDp2                           MCjpsi)

cd $WORKINGDIR/$LOCAL_PATH
cmsenv

root -l -b -q $EOS_DIR/$listFichier[$1] loadMVAlibs.cxx addMVAIso.cxx+
mv /tmp/hbrun/tnpZ_withMVAIso.root $TEMP_DIR/tnpZ_withMVAIso$1.root
root -l -b -q $TEMP_DIR/tnpZ_withMVAIso$1.root addMuonID.cxx+
rm $TEMP_DIR/tnpZ_withMVAIso$1.root
if ($1<7) then
    echo "on est des data ! "
    cp /tmp/hbrun/tnpZ_withMuID.root $FINAL_DIR/TnP_Data_$listNomPart[$1].root
else
    echo "on est du MC" 
    cp /tmp/hbrun/tnpZ_withMuID.root $TEMP_DIR/tnpZ_withMuID$1.root
    root -l -b -q $TEMP_DIR/tnpZ_withMuID$1.root $FINAL_DIR/TnP_Data_$listNomPart[1].root $FINAL_DIR/TnP_Data_$listNomPart[2].root $FINAL_DIR/TnP_Data_$listNomPart[3].root $FINAL_DIR/TnP_Data_$listNomPart[4].root $FINAL_DIR/TnP_Data_$listNomPart[5].root $FINAL_DIR/TnP_Data_$listNomPart[6].root addNVtxWeight.cxx+
    cp /tmp/hbrun/TnP_MCwithPU.root $FINAL_DIR/TnP_MC_$listNomPart[$1].root
endif


