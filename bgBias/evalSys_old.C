using namespace RooFit ;

TFile *myFile;

doTheToy(RooWorkspace *modelTarget, RooWorkspace *modelTest ){
    float ratioEff, errorEff;
    TTree *values = new TTree("values","");
    values->Branch("ratioEff",&ratioEff,"ratioEff/F");
    values->Branch("errorEff",&errorEff,"errorEff/F");

    
    int numTot = modelTarget->var("numTot")->getVal();
    float fSigAll = modelTarget->var("fSigAll")->getVal();
    float efficiency = modelTarget->var("efficiency")->getVal();
    float effBkg = modelTarget->var("effBkg")->getVal();
    
    
    int nbPass = (int) (efficiency*fSigAll*numTot+effBkg*(1-fSigAll)*numTot);
    int nbFail = (int) ((1-efficiency)*fSigAll*numTot + (1-effBkg)*(1-fSigAll)*numTot);

    
    
    RooCategory TnPcat("TnP","TnP") ;
    TnPcat.defineType("passing") ;
    TnPcat.defineType("failling") ;

    


    float efficiencyCalc, errorCalc;

    
    cout << "Toys avec numTot=" << numTot << " et fSigAll=" << fSigAll << endl;
    
    for (int i = 0 ; i < 2000; i++){
        cout << "i = " << i <<  endl;
        
      ////  *localWS = *modelTest;
        
        cout << "construct the pdf" << endl;
        RooSimultaneous simPdf("simPdf","simultaneous pdf",TnPcat) ;
        simPdf.addPdf(*modelTest->pdf("passing"),"passing") ;
        simPdf.addPdf(*modelTest->pdf("failling"),"failling") ;
       
       // cout << "effTest " << modelTest->var("efficiency")->getVal() << endl;

        cout << "generate the pseudo-data " << endl;
        modelTarget->var("mass")->setBins(40);
        RooDataHist* dataPass = modelTarget->pdf("passing")->generateBinned(*modelTarget->var("mass"),nbPass) ;
        RooDataHist* dataFail = modelTarget->pdf("failling")->generateBinned(*modelTarget->var("mass"),nbFail) ;
        RooDataHist* dataAll = modelTarget->pdf("tot")->generateBinned(*modelTarget->var("mass"),nbFail+nbPass) ;
        RooDataHist combData("combData","combined data",*modelTarget->var("mass"),Index(TnPcat),Import("passing",*dataPass),Import("failling",*dataFail)) ;

        simPdf.fitTo(combData) ;
        efficiencyCalc = modelTest->var("efficiency")->getVal();
        errorCalc =  modelTest->var("efficiency")->getError();
        
        ratioEff = efficiencyCalc/efficiency;
        errorEff = errorCalc/efficiency;
        values->Fill();
        
    }
    myFile->cd();
    values->Write();

    

   /*RooPlot* xframe = modelTest->var("mass")->frame(Title("TnP model Passing"),Bins(40)) ;
    dataPass->plotOn(xframe);
    modelTest->pdf("passing")->plotOn(xframe,LineColor(kGreen));
    modelTest->pdf("passing")->plotOn(xframe,Components(*modelTest->pdf("backgroundPass")), LineStyle(kDashed),LineColor(kGreen));
    
    RooPlot* xframe2 = modelTest->var("mass")->frame(Title("TnP model Failling"),Bins(40)) ;
    dataFail->plotOn(xframe2);
    modelTest->pdf("failling")->plotOn(xframe2,LineColor(kRed));
    modelTest->pdf("failling")->plotOn(xframe2,Components(*modelTest->pdf("backgroundFail")), LineStyle(kDashed),LineColor(kRed));


    RooPlot* xframe3 = modelTest->var("mass")->frame(Title("TnP model Total"),Bins(40)) ;
    dataAll->plotOn(xframe3);
    modelTest->pdf("tot")->plotOn(xframe3,LineColor(kBlue));
 

    TCanvas *c0 = new TCanvas("c0","coucou",800,800);
	c0->SetFillColor(0);
    c0->Divide(2,2);

    c0->cd(1);
    xframe->Draw();
    
    
    c0->cd(2);
    xframe2->Draw();
    
    c0->cd(3);
    xframe3->Draw();

	c0->Print("plots/fits.png");*/

}


evalSys(int jobID){
    
    myFile = new TFile(Form("theOutFile_%i.root",jobID),"RECREATE");
    
    RooWorkspace* Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN = new RooWorkspace("Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("a0[0.10196,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("a1[0.0044549,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("a2[0.00868245,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("a3[0.00298549,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("alpha[0.983606, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("alphaF[1.10303, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("effBkg[3.94918e-05,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("efficiency[0.335154,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("fSigAll[0.273675,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("largerResFail[1.16541,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("largerResPass[1.02226,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("numTot[61655,0,1e10]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("scaleTf[0.994391,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("scaleTp[0.994307,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("lef[-0.123679]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("lep[-0.0953175]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("mean[91.5941]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("n[2.20355]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("scale[3.68072e-07]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("sigma[2.10038]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("sigmaF[2.46496]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("vFrac[0.897881]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("expr::NewMean1p('mean*scaleTp',mean,scaleTp)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("expr::NewMean1f('mean*scaleTf',mean,scaleTf)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("expr::NewSigma1p('sigma*largerResPass',sigma,largerResPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("expr::NewSigma1f('sigmaF*largerResFail',sigmaF,largerResFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("CBShape::cbs(mass, scale, NewSigma1p, alpha, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("CBShape::cbsF(mass, scale, NewSigma1f, alphaF, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("RooBreitWigner::vs(mass, NewMean1p, NewSigma1p)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("RooBreitWigner::vsF(mass, NewMean1f, NewSigma1f)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("FCONV::convPass(mass,vs,cbs)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("FCONV::convFail(mass,vsF,cbsF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("Exponential::expP(mass, lep)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("Exponential::expF(mass, lef)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("SUM::signalPass(vFrac*convPass, expP)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("SUM::signalFail(vFrac*convFail, expF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("RooBernstein::backgroundPass(mass,{a0,a1,a2,a3})");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("RooBernstein::backgroundFail(mass,{a0,a1,a2,a3})");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("efficiency");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("signalFractionInPassing");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN->factory("SUM::tot(usl[1.0]*passing, failling)");
    
    
    RooWorkspace* Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3 = new RooWorkspace("Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("a0[0.10196,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("a1[0.0044549,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("a2[0.00868245,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("a3[0.00298549,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("alpha[0.983606, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("alphaF[1.10303, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("effBkg[3.94918e-05,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("efficiency[0.335154,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("fSigAll[0.273675,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("largerResFail[1.16541,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("largerResPass[1.02226,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("numTot[61655,0,1e10]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("scaleTf[0.994391,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("scaleTp[0.994307,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("lef[-0.123679]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("lep[-0.0953175]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("mean[91.5941]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("n[2.20355]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("scale[3.68072e-07]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("sigma[2.10038]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("sigmaF[2.46496]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("vFrac[0.897881]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("expr::NewMean1p('mean*scaleTp',mean,scaleTp)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("expr::NewMean1f('mean*scaleTf',mean,scaleTf)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("expr::NewSigma1p('sigma*largerResPass',sigma,largerResPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("expr::NewSigma1f('sigmaF*largerResFail',sigmaF,largerResFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("CBShape::cbs(mass, scale, NewSigma1p, alpha, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("CBShape::cbsF(mass, scale, NewSigma1f, alphaF, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("RooBreitWigner::vs(mass, NewMean1p, NewSigma1p)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("RooBreitWigner::vsF(mass, NewMean1f, NewSigma1f)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("FCONV::convPass(mass,vs,cbs)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("FCONV::convFail(mass,vsF,cbsF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("Exponential::expP(mass, lep)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("Exponential::expF(mass, lef)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("SUM::signalPass(vFrac*convPass, expP)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("SUM::signalFail(vFrac*convFail, expF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("RooBernstein::backgroundPass(mass,{a0,a1,a2,a3})");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("RooBernstein::backgroundFail(mass,{a0,a1,a2,a3})");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("efficiency");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("signalFractionInPassing");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3->factory("SUM::tot(usl[1.0]*passing, failling)");
    
    
    RooWorkspace* Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN = new RooWorkspace("Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("a0[0.102667,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("a1[0.0341287,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("a2[0.0000000360673,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("a3[0.0096667,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("a4[0.00237659,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("alpha[0.983606, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("alphaF[1.10303, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("effBkg[3.94918e-05,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("efficiency[0.335154,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("fSigAll[0.273675,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("largerResFail[1.16541,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("largerResPass[1.02226,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("numTot[61655,0,1e10]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("scaleTf[0.994391,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("scaleTp[0.994307,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("lef[-0.123679]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("lep[-0.0953175]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("mean[91.5941]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("n[2.20355]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("scale[3.68072e-07]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("sigma[2.10038]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("sigmaF[2.46496]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("vFrac[0.897881]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("expr::NewMean1p('mean*scaleTp',mean,scaleTp)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("expr::NewMean1f('mean*scaleTf',mean,scaleTf)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("expr::NewSigma1p('sigma*largerResPass',sigma,largerResPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("expr::NewSigma1f('sigmaF*largerResFail',sigmaF,largerResFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("CBShape::cbs(mass, scale, NewSigma1p, alpha, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("CBShape::cbsF(mass, scale, NewSigma1f, alphaF, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("RooBreitWigner::vs(mass, NewMean1p, NewSigma1p)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("RooBreitWigner::vsF(mass, NewMean1f, NewSigma1f)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("FCONV::convPass(mass,vs,cbs)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("FCONV::convFail(mass,vsF,cbsF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("Exponential::expP(mass, lep)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("Exponential::expF(mass, lef)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("SUM::signalPass(vFrac*convPass, expP)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("SUM::signalFail(vFrac*convFail, expF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("RooBernstein::backgroundPass(mass,{a0,a1,a2,a3,a4})");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("RooBernstein::backgroundFail(mass,{a0,a1,a2,a3,a4})");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("efficiency");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("signalFractionInPassing");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN->factory("SUM::tot(usl[1.0]*passing, failling)");

    RooWorkspace* Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4 = new RooWorkspace("Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("a0[0.10266,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("a1[0.0341287,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("a2[0.0000000360673,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("a3[0.0096667,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("a4[0.00237659,0,50]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("alpha[0.983606, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("alphaF[1.10303, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("effBkg[3.94918e-05,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("efficiency[0.335154,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("fSigAll[0.273675,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("largerResFail[1.16541,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("largerResPass[1.02226,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("numTot[61655,0,1e10]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("scaleTf[0.994391,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("scaleTp[0.994307,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("lef[-0.123679]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("lep[-0.0953175]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("mean[91.5941]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("n[2.20355]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("scale[3.68072e-07]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("sigma[2.10038]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("sigmaF[2.46496]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("vFrac[0.897881]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("expr::NewMean1p('mean*scaleTp',mean,scaleTp)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("expr::NewMean1f('mean*scaleTf',mean,scaleTf)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("expr::NewSigma1p('sigma*largerResPass',sigma,largerResPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("expr::NewSigma1f('sigmaF*largerResFail',sigmaF,largerResFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("CBShape::cbs(mass, scale, NewSigma1p, alpha, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("CBShape::cbsF(mass, scale, NewSigma1f, alphaF, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("RooBreitWigner::vs(mass, NewMean1p, NewSigma1p)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("RooBreitWigner::vsF(mass, NewMean1f, NewSigma1f)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("FCONV::convPass(mass,vs,cbs)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("FCONV::convFail(mass,vsF,cbsF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("Exponential::expP(mass, lep)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("Exponential::expF(mass, lef)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("SUM::signalPass(vFrac*convPass, expP)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("SUM::signalFail(vFrac*convFail, expF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("RooBernstein::backgroundPass(mass,{a0,a1,a2,a3,a4})");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("RooBernstein::backgroundFail(mass,{a0,a1,a2,a3,a4})");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("efficiency");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("signalFractionInPassing");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4->factory("SUM::tot(usl[1.0]*passing, failling)");
    
    
    RooWorkspace* Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN = new RooWorkspace("Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("alpha[0.983606, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("alphaF[1.10303, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("effBkg[3.94918e-05,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("efficiency[0.335154,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("fSigAll[0.273675,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("largerResFail[1.16541,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("largerResPass[1.02226,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("numTot[61655,0,1e10]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("scaleTf[0.994391,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("scaleTp[0.994307,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("lef[-0.123679]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("lep[-0.0953175]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("mean[91.5941]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("n[2.20355]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("scale[3.68072e-07]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("sigma[2.10038]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("sigmaF[2.46496]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("vFrac[0.897881]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("expr::NewMean1p('mean*scaleTp',mean,scaleTp)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("expr::NewMean1f('mean*scaleTf',mean,scaleTf)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("expr::NewSigma1p('sigma*largerResPass',sigma,largerResPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("expr::NewSigma1f('sigmaF*largerResFail',sigmaF,largerResFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("CBShape::cbs(mass, scale, NewSigma1p, alpha, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("CBShape::cbsF(mass, scale, NewSigma1f, alphaF, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("RooBreitWigner::vs(mass, NewMean1p, NewSigma1p)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("RooBreitWigner::vsF(mass, NewMean1f, NewSigma1f)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("FCONV::convPass(mass,vs,cbs)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("FCONV::convFail(mass,vsF,cbsF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("Exponential::expP(mass, lep)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("Exponential::expF(mass, lef)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("SUM::signalPass(vFrac*convPass, expP)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("SUM::signalFail(vFrac*convFail, expF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("Exponential::backgroundFail(mass, lf[-0.0585468,-5,5])");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("Exponential::backgroundPass(mass, lp[0.114545,-8,8])");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("efficiency");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("signalFractionInPassing");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN->factory("SUM::tot(usl[1.0]*passing, failling)");
    
    RooWorkspace* Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp = new RooWorkspace("Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("alpha[0.983606, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("alphaF[1.10303, 0.5, 5.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("effBkg[3.94918e-05,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("efficiency[0.335154,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("fSigAll[0.273675,0,1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("largerResFail[1.16541,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("largerResPass[1.02226,0.,2.]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("numTot[61655,0,1e10]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("scaleTf[0.994391,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("scaleTp[0.994307,0.9,1.1]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("lef[-0.123679]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("lep[-0.0953175]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("mean[91.5941]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("n[2.20355]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("scale[3.68072e-07]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("sigma[2.10038]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("sigmaF[2.46496]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("vFrac[0.897881]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("expr::NewMean1p('mean*scaleTp',mean,scaleTp)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("expr::NewMean1f('mean*scaleTf',mean,scaleTf)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("expr::NewSigma1p('sigma*largerResPass',sigma,largerResPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("expr::NewSigma1f('sigmaF*largerResFail',sigmaF,largerResFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("CBShape::cbs(mass, scale, NewSigma1p, alpha, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("CBShape::cbsF(mass, scale, NewSigma1f, alphaF, n)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("RooBreitWigner::vs(mass, NewMean1p, NewSigma1p)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("RooBreitWigner::vsF(mass, NewMean1f, NewSigma1f)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("FCONV::convPass(mass,vs,cbs)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("FCONV::convFail(mass,vsF,cbsF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("Exponential::expP(mass, lep)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("Exponential::expF(mass, lef)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("SUM::signalPass(vFrac*convPass, expP)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("SUM::signalFail(vFrac*convFail, expF)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("Exponential::backgroundFail(mass, lf[-0.0585468,-5,5])");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("Exponential::backgroundPass(mass, lp[0.114545,-8,8])");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("efficiency");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("signalFractionInPassing");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp->factory("SUM::tot(usl[1.0]*passing, failling)");
    
     //doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3);

    
    
    switch (jobID)
    {
        case 1: {
            doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3);
            break;
        }
        case 2: {
            doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4);
            break;
        }
        case 3: {
            doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp);
            break;
        }
        case 4: {
            doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4);
            break;
        }
        case 5: {
            doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3);
            break;
        }
        case 6: {
            doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp);
            break;
        }
        case 7: {
            doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp);
            break;
        }
        case 8: {
            doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly3);
            break;
        }
        case 9: {
            doTheToy(Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_exp_GEN, Tight2012_etaPOGlargeabsSCeta_bin0_pt_bin0_poly4);
            break;
        }
    
    }
    myFile->Close();
    
    
}




