#include "TTree.h"
#include "TFile.h"
#include "TStopwatch.h"

void skimForPromptRate() {
    TTree *tIn  = (TTree *) gFile->Get("tpTree/fitter_tree");
    Int_t muIdLooseMVA;
    tIn->SetBranchAddress("muIdLooseMVA", &muIdLooseMVA);

    TFile *fOut = new TFile("/tmp/hbrun/tnpZ_skimmedForPR.root", "RECREATE");
    fOut->mkdir("tpTree")->cd();
    TTree *tOut = tIn->CloneTree(0);


    int step = tIn->GetEntries()/1000;
    double evDenom = 100.0/double(tIn->GetEntries());
    TStopwatch timer; timer.Start();
    for (int i = 0, n = tIn->GetEntries(); i < n; ++i) {
        tIn->GetEntry(i);
        if (!(muIdLooseMVA)) continue;
        tOut->Fill();
        //if (i > 10000) break;
        if ((i+1) % step == 0) { 
            double totalTime = timer.RealTime()/60.; timer.Continue();
            double fraction = double(i+1)/double(n+1), remaining = totalTime*(1-fraction)/fraction;
            printf("Done %9d/%9d   %5.1f%%   (elapsed %5.1f min, remaining %5.1f min)\n", i, n, i*evDenom, totalTime, remaining); 
            fflush(stdout); 
        }
    }

    tOut->AutoSave(); // according to root tutorial this is the right thing to do
    fOut->Close();
}
