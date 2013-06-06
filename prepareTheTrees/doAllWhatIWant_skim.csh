#!/bin/csh

setenv EOS_DIR /afs/cern.ch/work/h/hbrun/pogTnPr8/

set listNomPart = (runA runB_part0 runB_part1 runC_part0 runC_part1 runD_part0 runD_part1 runD_part2)


foreach i  ($listNomPart)
    echo $i
    root -l -b -q $EOS_DIR/TnP_Data_$i.root skimForPromptRate.cxx+
    mv /tmp/hbrun/tnpZ_skimmedForPR.root $EOS_DIR/TnP_Data_${i}_skimed.root
end





