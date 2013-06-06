#include "TTree.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include <cstdlib>
#include <cmath>


#include "../../../../Muon/MuonAnalysisTools/interface/MuonEffectiveArea.h"

//void addMuonID(TString NameFile) {
void addMuonID() {
	using namespace std;
    TTree *tIn  = (TTree *) gFile->Get("tpTree/fitter_tree");
    Float_t pt, eta, chHad, nHad, phot, rho;
	Int_t Glb, TM, TMLST, PF, mvaIsoCut_HWW2012, mvaIsoCut_HWW2012Loose, TMOST;
	Float_t glbChi2, glbValidMuHits, numberOfMatches, tkKink, tkTrackerLay, tkPixelLay, tkChi2,numberOfMatchedStations ;
	Float_t tkValidPixelHits, tkPtError, dB, dzPV, mvaIso;
    tIn->SetBranchAddress("pt", &pt);
    tIn->SetBranchAddress("eta", &eta);
    tIn->SetBranchAddress("chargedHadIso04", &chHad);
    tIn->SetBranchAddress("neutralHadIso04", &nHad);
    tIn->SetBranchAddress("photonIso04",     &phot);
    tIn->SetBranchAddress("kt6RhoNeu05",     &rho);
	tIn->SetBranchAddress("Glb", &Glb); 
	tIn->SetBranchAddress("glbChi2", &glbChi2); 
	tIn->SetBranchAddress("glbValidMuHits", &glbValidMuHits); 
	tIn->SetBranchAddress("numberOfMatches", &numberOfMatches); 
	tIn->SetBranchAddress("TM", &TM); 
	tIn->SetBranchAddress("TMLST", &TMLST); 
	tIn->SetBranchAddress("PF", &PF); 
	tIn->SetBranchAddress("tkKink", &tkKink); 
	tIn->SetBranchAddress("tkTrackerLay", &tkTrackerLay); 
	tIn->SetBranchAddress("tkValidPixelHits", &tkValidPixelHits); 
	tIn->SetBranchAddress("tkPtError", &tkPtError); 
	tIn->SetBranchAddress("dB", &dB); 
	tIn->SetBranchAddress("dzPV", &dzPV); 
	tIn->SetBranchAddress("mvaIsoCut_HWW2012", &mvaIsoCut_HWW2012); 
	tIn->SetBranchAddress("mvaIsoCut_HWW2012Loose", &mvaIsoCut_HWW2012Loose); 
	tIn->SetBranchAddress("tkPixelLay", &tkPixelLay); 
	tIn->SetBranchAddress("tkChi2", &tkChi2); 
	tIn->SetBranchAddress("TMOST", &TMOST);
	tIn->SetBranchAddress("mvaIso", &mvaIso);
	tIn->SetBranchAddress("numberOfMatchedStations", &numberOfMatchedStations);
	

	

    TFile *fOut = new TFile("/tmp/hbrun/tnpZ_withMuID.root", "RECREATE");
    //TFile *fOut = new TFile("/afs/cern.ch/work/h/hbrun/pogTnPr7/TnP_"+NameFile+".root", "RECREATE");
    fOut->mkdir("tpTree")->cd();
    TTree *tOut = tIn->CloneTree(0);
    Float_t pfCombRelIso04EACorr;
	Int_t TrackerOrGlobal, TOGclean, TOGCPF, TOGCPFT, TOGCPFTIP, TOGCPFTIPMVA, muIdLooseMVA, muTightId, muSoftId, fullWP0, fullWP1,  fullWP2,  fullWP3, muonIDbad, alternHwwID;
    tOut->Branch("TrackerOrGlobal", &TrackerOrGlobal, "TrackerOrGlobal/I");
    tOut->Branch("TOGclean", &TOGclean, "TOGclean/I");
    tOut->Branch("TOGCPF", &TOGCPF, "TOGCPF/I");
    tOut->Branch("TOGCPFT", &TOGCPFT, "TOGCPFT/I");
    tOut->Branch("TOGCPFTIP", &TOGCPFTIP, "TOGCPFTIP/I");
    tOut->Branch("TOGCPFTIPMVA", &TOGCPFTIPMVA, "TOGCPFTIPMVA/I");
    tOut->Branch("muIdLooseMVA", &muIdLooseMVA, "muIdLooseMVA/I");
    tOut->Branch("muTightId", &muTightId, "muTightId/I");
    tOut->Branch("muSoftId", &muSoftId, "muSoftId/I");
    tOut->Branch("fullWP0", &fullWP0, "fullWP0/I");
    tOut->Branch("fullWP1", &fullWP1, "fullWP1/I");
    tOut->Branch("fullWP2", &fullWP2, "fullWP2/I");
    tOut->Branch("fullWP3", &fullWP3, "fullWP3/I");
    tOut->Branch("muonIDbad", &muonIDbad, "muonIDbad/I");
    tOut->Branch("alternHwwID", &alternHwwID, "alternHwwID/I");
	
	
  
    MuonEffectiveArea::MuonEffectiveAreaTarget effAreaTarget = MuonEffectiveArea::kMuEAData2012; // or 2011
    MuonEffectiveArea::MuonEffectiveAreaType   effAreaType   = MuonEffectiveArea::kMuGammaAndNeutralHadronIso04;

    int step = tIn->GetEntries()/1000;
    double evDenom = 100.0/double(tIn->GetEntries());
    TStopwatch timer; timer.Start();
	
	bool goodGlobalMuon, goodTrackerMuon, niceTrack, IPcuts, IPcutsLoose;
    for (int i = 0, n = tIn->GetEntries(); i < n; ++i) {
	//for (int i = 0, n = 100; i < n; ++i) {
        tIn->GetEntry(i);
		/// the cut on the Global Muon
	//	printf("global muons cuts isGLM =%i  glbChi2=%f nbOfMatchs=%f nbValidMuonHits=%f \n",Glb, glbChi2,numberOfMatches, glbValidMuHits); 
		goodGlobalMuon = Glb && (glbChi2<10) && (numberOfMatches>1) && (glbValidMuHits>0);
	//	printf("isGoodGlobalMuon=%i\n", goodGlobalMuon);
		
		/// the cut on the tracker muon 
	//	printf("TM=%i, TMLST=%i\n",TM,TMLST);
		goodTrackerMuon = TM && TMLST;
	//	printf("goodTrackerMuon=%i\n",goodGlobalMuon);
		
		///do we have a PF muon ? 
	//	printf("PFmuon=%i", PF);
		
		/// the cut on the muon's track 
	//	printf("tkKink=%f, tkTrackerLay=%f, tkValidPixelHits=%f, tkPtError/pt=%f\n",tkKink, tkTrackerLay, tkValidPixelHits, 1.0*tkPtError/pt);
		niceTrack = (tkKink<20) && (tkTrackerLay>5) && (tkValidPixelHits>0) &&(1.0*tkPtError/pt < 0.10); 
	//	printf("niceTrack=%i\n",niceTrack);
		
	//	printf("db=%f, dzPV=%f\n",dB, dzPV);
		IPcuts = ((pt>20&&fabs(dB)<0.02)||(pt<=20&&fabs(dB)<0.01))&&fabs(dzPV)<0.1;
		IPcutsLoose = (fabs(dB)<0.2)&&fabs(dzPV)<0.1;
	//	printf("IPcuts=%i\n", IPcuts);
		
		
		TrackerOrGlobal = (Glb||TM);
		TOGclean = (goodGlobalMuon||goodTrackerMuon);
		TOGCPF = TOGclean && PF;
		TOGCPFT = TOGCPF && niceTrack;
		TOGCPFTIP = TOGCPFT && IPcuts;		
		TOGCPFTIPMVA = TOGCPFTIP && mvaIsoCut_HWW2012;
        
        alternHwwID = ((Glb && (glbChi2<10) && (numberOfMatchedStations>1) && (glbValidMuHits>0))||goodTrackerMuon) && PF && niceTrack && IPcuts && mvaIsoCut_HWW2012;
		
		muIdLooseMVA = TOGCPFT && IPcutsLoose && mvaIsoCut_HWW2012Loose;
		
		muSoftId = TMOST && (tkTrackerLay>5) &&(tkPixelLay > 1) && (tkChi2 < 1.8)&&(fabs(dB)<3)&&(fabs(dzPV)<30);
		
		muTightId = Glb && (glbChi2 < 10) && (glbValidMuHits>0) && (numberOfMatchedStations>1) && (fabs(dB)<0.2)&&(fabs(dzPV)<0.5) && (tkValidPixelHits > 0) && (tkTrackerLay>5);
		
        fullWP0 = TOGCPFTIP && (mvaIso>0.6);
        fullWP1 = TOGCPFTIP && (mvaIso>0.90);
        fullWP2 = TOGCPFTIP && (mvaIso>0.95);
        fullWP3 = TOGCPFTIP && (mvaIso>0.97);
        
        
        bool badMVA;
        if ((pt >= 20) && (abs(eta)<1.479))  badMVA = (mvaIso > 0.86);
		if ((pt >= 20) && (abs(eta)>1.479) && (abs(eta)<2.4))  badMVA = (mvaIso > 0.82);
		if ((pt < 20) && (abs(eta)<1.479))  badMVA = (mvaIso > 0.82);
		if ((pt < 20) && (abs(eta)>1.479) && (abs(eta)<2.4))  badMVA = (mvaIso > 0.86);
        muonIDbad = TOGCPFTIP && badMVA;
        
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
