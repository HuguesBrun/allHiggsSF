TFile *myFile = new TFile("myFile.root");

float theMax(float entree1, float entree2, float entree3){
    float myMax;
    
    if (entree1>entree2) myMax = entree1;
    else myMax = entree2;
    
    if (myMax>entree3) return myMax;
    else return entree3;
}


doThePlot(TString histo1, TString histo2, TString histo3){
    TH1F *h1 = (TH1F*) myFile->Get(histo1);
    TH1F *h2 = (TH1F*) myFile->Get(histo2);
    TH1F *h3 = (TH1F*) myFile->Get(histo3);
    
    
    
    TCanvas *c0 = new TCanvas("c0","coucou",600,300);
    c0->SetFillColor(0);
    
    h3->GetXaxis()->SetTitle("eff_{measured}/eff_{model}");
    h3->GetYaxis()->SetTitle("toys/0.004");
    
    float theMaxi = theMax(h1->GetMaximum(), h2->GetMaximum(), h3->GetMaximum());
    
    
    h3->SetMaximum(theMaxi*4.0/3);
    
    h1->SetFillColor(kGreen-9);
    h1->SetFillStyle(3002);
    h1->SetLineColor(kGreen-9);
    h1->SetLineWidth(3);
  //  h1->Draw();
    
    h2->SetFillColor(kRed-7);
    h2->SetFillStyle(3002);
    h2->SetLineColor(kRed-7);
    h2->SetLineWidth(3);
   // h2->Draw("same");
    
    h3->SetFillColor(kBlue-7);
    h3->SetFillStyle(3002);
    h3->SetLineColor(kBlue-7);
    h3->SetLineWidth(3);
    h3->Draw();
    h1->Draw("same");
    h2->Draw("same");
    
    TLegend *t = new TLegend(0.15,0.65,0.35,0.85);
    t->SetFillColor(0);
    t->SetLineColor(0);
    t->AddEntry(h1,"poly 3^{rd} deg","F");
    t->AddEntry(h2,"poly 4^{th} deg","F");
    t->AddEntry(h3,"exp deg","F");
    t->Draw();
    
    c0->Print(histo1+".png");


    printf("mean = %4.3f +- %4.2e \n",h1->GetMean(), h1->GetMeanError());
    printf("mean = %4.3f +- %4.2e \n",h2->GetMean(), h2->GetMeanError());
    printf("mean = %4.3f +- %4.2e \n",h3->GetMean(), h3->GetMeanError());
    
}

drawThePlot(int i ){
    doThePlot(Form("histo%i",3*i+1),Form("histo%i",3*i+2),Form("histo%i",3*i+3));

    
}




