#include "TTree.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include <cstdlib>
#include <cmath>


void createReducedTree() {
    TTree *tIn  = (TTree *) gFile->Get("tpTree/fitter_tree");
    Float_t mass, pt, abseta, pair_probeMultiplicity, tag_nVertices;
    Int_t TrackerOrGlobal, Tight2012, TOGCPFTIPMVA, mcTrue, muIdLooseMVA;


    tIn->SetBranchAddress("mass", &mass);
    tIn->SetBranchAddress("pt", &pt);
    tIn->SetBranchAddress("abseta", &abseta);
    tIn->SetBranchAddress("pair_probeMultiplicity", &pair_probeMultiplicity);
    tIn->SetBranchAddress("tag_nVertices", &tag_nVertices);
    tIn->SetBranchAddress("TrackerOrGlobal", &TrackerOrGlobal);
    tIn->SetBranchAddress("Tight2012", &Tight2012);
    tIn->SetBranchAddress("muIdLooseMVA", &muIdLooseMVA);
    tIn->SetBranchAddress("TOGCPFTIPMVA", &TOGCPFTIPMVA);
    tIn->SetBranchAddress("mcTrue", &mcTrue);

    TFile *fOut = new TFile("/tmp/hbrun/tnpZ_shortTree.root", "RECREATE");
    fOut->mkdir("tpTree")->cd();
    TTree *tOut = new TTree("fitter_tree","");
    tOut->Branch("TrackerOrGlobal", &TrackerOrGlobal, "TrackerOrGlobal/I");
    tOut->Branch("Tight2012", &Tight2012, "Tight2012/I");
    tOut->Branch("muIdLooseMVA", &muIdLooseMVA, "muIdLooseMVA/I");
    tOut->Branch("TOGCPFTIPMVA", &TOGCPFTIPMVA, "TOGCPFTIPMVA/I");
    tOut->Branch("mcTrue", &mcTrue, "mcTrue/I");
    tOut->Branch("tag_nVertices", &tag_nVertices, "tag_nVertices/F");
    tOut->Branch("pair_probeMultiplicity", &pair_probeMultiplicity, "pair_probeMultiplicity/F");
    tOut->Branch("pt", &pt, "pt/F");
    tOut->Branch("abseta", &abseta, "abseta/F");
    tOut->Branch("mass", &mass, "mass/F");

    int step = tIn->GetEntries()/1000;
    double evDenom = 100.0/double(tIn->GetEntries());
    TStopwatch timer; timer.Start();
    for (int i = 0, n = tIn->GetEntries(); i < n; ++i) {
        tIn->GetEntry(i);

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