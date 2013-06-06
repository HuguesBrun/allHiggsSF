TFile *theFile = new TFile("myFile.root","RECREATE");


histo(TString nom,TString nomHist){
    cout << "nom histo" << nomHist << endl;

    TChain *chain = new TChain("values");
    chain->Add("toyFiles/"+nom);
    cout << "nbEntries = " << chain->GetEntries() << endl;
    TH1F *h = new TH1F(nomHist,"",50,0.8,1.2);
    chain->Draw("ratioEff>>"+nomHist);
    cout << "h " << h->GetEntries() << endl;
    //h->Draw();
    h->Write();
    delete chain;
}


plotThePull(){
    gStyle->SetOptStat(0);
    
    histo("theOutFile_1.root","histo1");
    histo("theOutFile_2.root","histo2");
    histo("theOutFile_3.root","histo3");
    histo("theOutFile_4.root","histo4");
    histo("theOutFile_5.root","histo5");
    histo("theOutFile_6.root","histo6");
    histo("theOutFile_7.root","histo7");
    histo("theOutFile_8.root","histo8");
    histo("theOutFile_9.root","histo9");
    histo("theOutFile_10.root","histo10");
    histo("theOutFile_11.root","histo11");
    histo("theOutFile_12.root","histo12");
    histo("theOutFile_13.root","histo13");
    histo("theOutFile_14.root","histo14");
    histo("theOutFile_15.root","histo15");
    histo("theOutFile_16.root","histo16");

    
 //   theFile->Close();
  //  TCanvas *c0 = new TCanvas("c0","coucou",600,600);
  //  c0->SetFillColor(0);
  //  TChain *chain = new TChain("values");
  //  chain->Add("theOutFile_1.root");
  //  TH
  //  TH1F theHisto0 = histo("theOutFile_1.root","histo1");
    
  /*  TH1F *theHisto0 = new TH1F();
    *theHisto0 = *theHisto;
    delete theHisto;*/
    
 //   theHisto0.SetLineColor(kRed);
 //   theHisto0.Draw();
    
 //   TH1F theHisto2 = histo("theOutFile_2.root","histo2");
    //theHisto2.SetLineColor(kBlue);
   // theHisto2.Draw("same");
    
  /*  TH1F *theHisto3 = histo("theOutFile_3.root");
    theHisto3->SetLineColor(kGreen);
    theHisto3->Draw("same");*/
}
