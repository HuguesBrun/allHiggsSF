//TString nomPlot[9] = {"denom_pt","TOGCPF_from_TrackerOrGlobal","TOGCPFT_from_TrackerOrGlobal","TOGCPFTIP_from_TrackerOrGlobal","TOGCPFTIPMVA_from_TrackerOrGlobal","TOGCPF_from_TOGclean","TOGCPFT_from_TOGCPF","TOGCPFTIP_from_TOGCPFT","TOGCPFTIPMVA_from_TOGCPFTIP"};
//TString cuts[8] = {"pt", "pt_fine", "eta_sup", "eta_inf", "pt_real_etabin0", "pt_real_etabin1", "vtx", "pt_latinos"};


TFile *myFile = new TFile("theEfficiencyPlots_data.root");
TFile *myFileMC = new TFile("theEfficiencyPlots_MC.root");



TGraphAsymmErrors *giveGraphRatio(TGraphAsymmErrors *MCgraph, TGraphAsymmErrors *DATAgraph){
    TGraphAsymmErrors *theRatioGraph = (TGraphAsymmErrors*) MCgraph->Clone("graphRatio");
    
    int inPoints = theRatioGraph->GetN();
    double x,y;
    double x2, y2; 
    double xr, yr; 
    double ratioError, dataError, mcError;
    
    for (int i = 0 ; i < inPoints; i++){
        MCgraph->GetPoint(i, x, y);
        DATAgraph->GetPoint(i, x2, y2);
        yr = y2/y;
        dataError = DATAgraph->GetErrorY(i);
        mcError = MCgraph->GetErrorY(i);
        ratioError = yr*sqrt(dataError*dataError + mcError*mcError);
        theRatioGraph->SetPoint(i, x, yr);
        theRatioGraph->SetPointEYhigh(i, ratioError);
        theRatioGraph->SetPointEYlow(i, ratioError);
        
    }
    return theRatioGraph;
}




drawMyPlot(TString nameCut, TString theText, TString title, float yMin, float yMax){
    
    cout << theText << endl;
	
    
    
	
	TGraphAsymmErrors *theGraph4 = (TGraphAsymmErrors*) myFile->Get(nameCut);
    int inPointsDATA = theGraph4->GetN();
    for (int i = 0 ; i < inPointsDATA; i++){
        cout << "err+" << theGraph4->GetErrorYhigh(i) << "err-" << theGraph4->GetErrorYlow(i) << endl;
        if ( theGraph4->GetErrorYhigh(i)> theGraph4->GetErrorYlow(i)){
            theGraph4->SetPointEYhigh(i,theGraph4->GetErrorYlow(i));
            theGraph4->SetPointEYlow(i,theGraph4->GetErrorYlow(i));
        }
        else {
            theGraph4->SetPointEYhigh(i,theGraph4->GetErrorYhigh(i));
            theGraph4->SetPointEYlow(i,theGraph4->GetErrorYhigh(i));
        }
    }
	TGraphAsymmErrors *theGraph4MC = (TGraphAsymmErrors*) myFileMC->Get(nameCut+"_&_mcTrue_true");
    
	TGraphAsymmErrors *ratio = (TGraphAsymmErrors*) giveGraphRatio(theGraph4MC, theGraph4);
    
    int inPoints = theGraph4->GetN();
    double x,y;
    double x2, y2;    
    theGraph4->GetPoint(0, x, y);
    theGraph4->GetPoint(inPoints-1, x2, y2);
    
    float lowLim = x-theGraph4->GetErrorXlow(0);
    float highLim = x2+theGraph4->GetErrorXhigh(inPoints-1);
    
    
	TCanvas *c0 = new TCanvas("c0","coucou",600, 600);
	c0->SetFillColor(0);
	theGraph4->SetMinimum(yMin);
	theGraph4->SetMaximum(yMax);
    
	theGraph4->SetMarkerStyle(22);
	theGraph4->SetMarkerColor(kGreen-6);
	theGraph4->GetYaxis()->SetTitle("#epsilon");
	theGraph4->SetMarkerSize(1.5);
    theGraph4->GetXaxis()->SetRangeUser(lowLim,highLim);
	theGraph4->Draw("AP");
	theGraph4MC->SetMarkerStyle(22);
	theGraph4MC->SetMarkerColor(kMagenta-6);
	theGraph4MC->SetMarkerSize(1.5);
	theGraph4MC->Draw("P:same");
    
    ratio->SetMarkerStyle(22);
	ratio->SetMarkerColor(kBlue-4);
	ratio->SetMarkerSize(1.5);
	ratio->Draw("P:same");

    
	TLatex theLatex;
	theLatex.SetNDC();
	theLatex.SetTextSize(0.03);
	theLatex.DrawLatex(0.06,0.92, title);
	
	theLatex.DrawLatex(0.55,0.92, theText);
    
	TLegend *t = new TLegend(0.66,0.16,0.87,0.35);
	t->SetFillColor(0);
	t->SetLineColor(0);
	t->AddEntry(theGraph4,"data","P");
	t->AddEntry(theGraph4MC,"MC","P");
    t->AddEntry(ratio,"DATA/MC ","P");
    
	t->Draw();
    
    TLine *line = new TLine(lowLim,1,highLim,1);
    line->SetLineStyle(2);
    line->Draw("same");
    
	
	c0->Print("effPlots/plot_"+nameCut+"MC.png");
    
}

doADraw3bins(TString name, TString title){
    cout << "on va plotter le plot " << name << endl;
    drawMyPlot(name, "|#eta| <0.9",title,0,1.2);
    name.ReplaceAll("bin0","bin1");
	drawMyPlot(name, "0.9 <|#eta| < 1.2",title,0,1.2);   
    name.ReplaceAll("bin1","bin2");
	drawMyPlot(name, "1.2 <|#eta| < 2.4",title,0,1.2);
    
}


doADraw4bins(TString name, TString title){
    cout << "on va plotter le plot " << name << endl;
    drawMyPlot(name, "|#eta| < 0.8",title,0,1.2);
    name.ReplaceAll("bin0","bin1");
	drawMyPlot(name, "0.8 <|#eta| < 1.479",title,0,1.2);   
    name.ReplaceAll("bin1","bin2");
	drawMyPlot(name, "1.479 <|#eta| < 2.0",title,0,1.2);
    name.ReplaceAll("bin2","bin3");
	drawMyPlot(name, "2.0 <|#eta| < 2.5",title,,0,1.2);
    
}

doADraw2bins(TString name, TString title){
    drawMyPlot(name, "|#eta| < 1.48",title,0,1.2);
    name.ReplaceAll("bin0","bin1");
	drawMyPlot(name, "1.48 <|#eta| < 2.4",title,0,1.2);       
}


doADraw2binsPOG(TString name, TString title){
    drawMyPlot(name, "|#eta| < 1.2",title,0.8,1.1);
    name.ReplaceAll("bin0","bin1");
	drawMyPlot(name, "1.2 <|#eta| < 2.4",title,0.8,1.1);       
}

doThePlotsMC(){
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);


    
    doADraw3bins("TOGCPFTIPMVA_higgsSF_pt_PLOT_abseta_bin0_&_TrackerOrGlobal_pass","Hww selection");
     
}

//TOGCPFTIPMVA_higgsSF_pt_PLOT_abseta_bin0_&_TrackerOrGlobal_pass
//TOGCPFTIPMVA_higgsSF_pt_PLOT_abseta_bin0_&_TrackerOrGlobal_pass_&_mcTrue_true


