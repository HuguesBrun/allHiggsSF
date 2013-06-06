//#include "functions.h"

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
    
    for (int i = 0 ; i < 300; i++){
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

    

   RooPlot* xframe = modelTest->var("mass")->frame(Title("TnP model Passing"),Bins(40)) ;
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

	c0->Print("plots/fits.png");

}


evalSys(){
    
    int jobID=1;
    myFile = new TFile(Form("theOutFile_%i.root",jobID),"RECREATE");
   // loadTheWS();

    

    RooWorkspace* Tight2012_etaPOGlargeabseta_bin0_pt_bin0 = new RooWorkspace("Tight2012_etaPOGlargeabseta_bin0_pt_bin0");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("a0[1.89805,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("a1[3.24266e-08,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("a2[0.0580601,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("a3[2.91718e-07,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("a4[0.00148358,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("scale[1.0001,0.9,1.1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("largerResPass[0.998197,0.,2.]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("largerResFail[1.03273,0.,2.]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("effBkg[8.67639e-13,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("fSigAll[0.780512,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("numTot[8692,0,1e10]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("efficiency[0.645027,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("mean1p[90.6409,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("mean2p[83.2059,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("mean1f[89.7821,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("mean2f[91.9821,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("sigma1p[1.52661,1,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("sigma2p[4.94181,1,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("sigma1f[0.699778,0,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("sigma2f[2.00168e-05,0,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("vPropp[0.813748,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("vPropf[0.610732,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("lsp[-0.176987,-5,5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("lsf[-0.23634,-5,5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("ap0[88.4965,88,93]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("ap1[-5,-5,-0.5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("vFrac[0.65458,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("vPropTotp[0.877071,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("vPropTotf[0.642287,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("width1f[3,1,2]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("width2f[3,1,2]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::NewMean1p('mean1p*scale',mean1p,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::NewMean2p('mean2p*scale',mean2p,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::NewMean1f('mean1f*scale',mean1f,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::NewMean2f('mean2f*scale',mean2f,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::NewSigma1p('sigma1p*largerResPass',sigma1p,largerResPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::NewSigma2p('sigma2p*largerResPass',sigma2p,largerResPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::NewSigma1f('sigma1f*largerResFail',sigma1f,largerResFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("expr::NewSigma2f('sigma2f*largerResFail',sigma2f,largerResFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("Voigtian::theSig1p(mass, NewMean1p, width1p[2.495], NewSigma1p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("Voigtian::theSig2p(mass, NewMean2p, width2p[2.495], NewSigma2p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("SUM::sVoigP(vPropp*theSig1p,theSig2p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("Voigtian::theSig1f(mass, NewMean1f, width1f, NewSigma1f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("Voigtian::theSig2f(mass, NewMean2f, width2f, NewSigma2f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("SUM::sVoigF(vPropf*theSig1f,theSig2f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("Exponential::expP(mass, lsp)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("Exponential::expF(mass, lsf)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("ArgusBG::argF(mass, ap0, ap1)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("SUM::shapeF(vFrac*argF, expF)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("SUM::signalPass(vPropTotp*sVoigP,expP)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("SUM::signalFail( vPropTotf*sVoigF,shapeF)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("RooBernstein::backgroundPass(mass,{a0,a1,a2,a3,a4})");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("RooBernstein::backgroundFail(mass,{a0,a1,a2,a3,a4})");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("efficiency[0.9,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("signalFractionInPassing[0.9]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0->factory("SUM::tot(usl[1.0]*passing, failling)");

    
    RooWorkspace* Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN = new RooWorkspace("Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("a0[1.89805,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("a1[3.24266e-08,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("a2[0.0580601,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("a3[2.91718e-07,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("a4[0.00148358,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("scale[1.0001,0.9,1.1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("largerResPass[0.998197,0.,2.]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("largerResFail[1.03273,0.,2.]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("effBkg[8.67639e-13,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("fSigAll[0.780512,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("numTot[8692,0,1e10]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("efficiency[0.645027,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("mean1p[90.6409,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("mean2p[83.2059,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("mean1f[89.7821,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("mean2f[91.9821,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("sigma1p[1.52661,1,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("sigma2p[4.94181,1,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("sigma1f[0.699778,0,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("sigma2f[2.00168e-05,0,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("vPropp[0.813748,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("vPropf[0.610732,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("lsp[-0.176987,-5,5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("lsf[-0.23634,-5,5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("ap0[88.4965,88,93]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("ap1[-5,-5,-0.5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("vFrac[0.65458,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("vPropTotp[0.877071,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("vPropTotf[0.642287,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("width1f[3,1,2]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("width2f[3,1,2]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::NewMean1p('mean1p*scale',mean1p,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::NewMean2p('mean2p*scale',mean2p,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::NewMean1f('mean1f*scale',mean1f,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::NewMean2f('mean2f*scale',mean2f,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::NewSigma1p('sigma1p*largerResPass',sigma1p,largerResPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::NewSigma2p('sigma2p*largerResPass',sigma2p,largerResPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::NewSigma1f('sigma1f*largerResFail',sigma1f,largerResFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("expr::NewSigma2f('sigma2f*largerResFail',sigma2f,largerResFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("Voigtian::theSig1p(mass, NewMean1p, width1p[2.495], NewSigma1p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("Voigtian::theSig2p(mass, NewMean2p, width2p[2.495], NewSigma2p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("SUM::sVoigP(vPropp*theSig1p,theSig2p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("Voigtian::theSig1f(mass, NewMean1f, width1f, NewSigma1f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("Voigtian::theSig2f(mass, NewMean2f, width2f, NewSigma2f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("SUM::sVoigF(vPropf*theSig1f,theSig2f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("Exponential::expP(mass, lsp)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("Exponential::expF(mass, lsf)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("ArgusBG::argF(mass, ap0, ap1)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("SUM::shapeF(vFrac*argF, expF)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("SUM::signalPass(vPropTotp*sVoigP,expP)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("SUM::signalFail( vPropTotf*sVoigF,shapeF)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("RooBernstein::backgroundPass(mass,{a0,a1,a2,a3,a4})");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("RooBernstein::backgroundFail(mass,{a0,a1,a2,a3,a4})");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("efficiency[0.9,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("signalFractionInPassing[0.9]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_GEN->factory("SUM::tot(usl[1.0]*passing, failling)");

    
    RooWorkspace* Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3 = new RooWorkspace("Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("a0[15.0417,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("a1[2.04884e-06,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("a2[7.74655e-07,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("a3[4.76837e-05,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("scale[1.0001,0.9,1.1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("largerResPass[0.998197,0.,2.]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("largerResFail[1.03273,0.,2.]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("effBkg[8.67639e-13,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("fSigAll[0.780512,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("numTot[8692,0,1e10]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("efficiency[0.645027,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("mean1p[90.6409,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("mean2p[83.2059,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("mean1f[89.7821,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("mean2f[91.9821,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("sigma1p[1.52661,1,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("sigma2p[4.94181,1,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("sigma1f[0.699778,0,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("sigma2f[2.00168e-05,0,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("vPropp[0.813748,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("vPropf[0.610732,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("lsp[-0.176987,-5,5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("lsf[-0.23634,-5,5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("ap0[88.4965,88,93]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("ap1[-5,-5,-0.5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("vFrac[0.65458,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("vPropTotp[0.877071,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("vPropTotf[0.642287,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("width1f[3,1,2]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("width2f[3,1,2]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::NewMean1p('mean1p*scale',mean1p,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::NewMean2p('mean2p*scale',mean2p,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::NewMean1f('mean1f*scale',mean1f,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::NewMean2f('mean2f*scale',mean2f,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::NewSigma1p('sigma1p*largerResPass',sigma1p,largerResPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::NewSigma2p('sigma2p*largerResPass',sigma2p,largerResPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::NewSigma1f('sigma1f*largerResFail',sigma1f,largerResFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("expr::NewSigma2f('sigma2f*largerResFail',sigma2f,largerResFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("Voigtian::theSig1p(mass, NewMean1p, width1p[2.495], NewSigma1p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("Voigtian::theSig2p(mass, NewMean2p, width2p[2.495], NewSigma2p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("SUM::sVoigP(vPropp*theSig1p,theSig2p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("Voigtian::theSig1f(mass, NewMean1f, width1f, NewSigma1f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("Voigtian::theSig2f(mass, NewMean2f, width2f, NewSigma2f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("SUM::sVoigF(vPropf*theSig1f,theSig2f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("Exponential::expP(mass, lsp)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("Exponential::expF(mass, lsf)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("ArgusBG::argF(mass, ap0, ap1)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("SUM::shapeF(vFrac*argF, expF)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("SUM::signalPass(vPropTotp*sVoigP,expP)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("SUM::signalFail( vPropTotf*sVoigF,shapeF)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("RooBernstein::backgroundPass(mass,{a0,a1,a2,a3})");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("RooBernstein::backgroundFail(mass,{a0,a1,a2,a3})");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("efficiency[0.9,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("signalFractionInPassing[0.9]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3->factory("SUM::tot(usl[1.0]*passing, failling)");
   
    
    RooWorkspace* Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN = new RooWorkspace("Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("mass[40,60,120]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("a0[15.0417,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("a1[2.04884e-06,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("a2[7.74655e-07,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("a3[4.76837e-05,0,50]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("scale[1.0001,0.9,1.1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("largerResPass[0.998197,0.,2.]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("largerResFail[1.03273,0.,2.]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("effBkg[8.67639e-13,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("fSigAll[0.780512,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("numTot[8692,0,1e10]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("efficiency[0.645027,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("mean1p[90.6409,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("mean2p[83.2059,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("mean1f[89.7821,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("mean2f[91.9821,80,100]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("sigma1p[1.52661,1,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("sigma2p[4.94181,1,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("sigma1f[0.699778,0,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("sigma2f[2.00168e-05,0,20]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("vPropp[0.813748,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("vPropf[0.610732,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("lsp[-0.176987,-5,5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("lsf[-0.23634,-5,5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("ap0[88.4965,88,93]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("ap1[-5,-5,-0.5]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("vFrac[0.65458,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("vPropTotp[0.877071,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("vPropTotf[0.642287,0.5,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("width1f[3,1,2]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("width2f[3,1,2]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::NewMean1p('mean1p*scale',mean1p,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::NewMean2p('mean2p*scale',mean2p,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::NewMean1f('mean1f*scale',mean1f,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::NewMean2f('mean2f*scale',mean2f,scale)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::NewSigma1p('sigma1p*largerResPass',sigma1p,largerResPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::NewSigma2p('sigma2p*largerResPass',sigma2p,largerResPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::NewSigma1f('sigma1f*largerResFail',sigma1f,largerResFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("expr::NewSigma2f('sigma2f*largerResFail',sigma2f,largerResFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("Voigtian::theSig1p(mass, NewMean1p, width1p[2.495], NewSigma1p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("Voigtian::theSig2p(mass, NewMean2p, width2p[2.495], NewSigma2p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("SUM::sVoigP(vPropp*theSig1p,theSig2p)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("Voigtian::theSig1f(mass, NewMean1f, width1f, NewSigma1f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("Voigtian::theSig2f(mass, NewMean2f, width2f, NewSigma2f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("SUM::sVoigF(vPropf*theSig1f,theSig2f)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("Exponential::expP(mass, lsp)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("Exponential::expF(mass, lsf)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("ArgusBG::argF(mass, ap0, ap1)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("SUM::shapeF(vFrac*argF, expF)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("SUM::signalPass(vPropTotp*sVoigP,expP)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("SUM::signalFail( vPropTotf*sVoigF,shapeF)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("RooBernstein::backgroundPass(mass,{a0,a1,a2,a3})");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("RooBernstein::backgroundFail(mass,{a0,a1,a2,a3})");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("efficiency[0.9,0,1]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("signalFractionInPassing[0.9]");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)");
    Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN->factory("SUM::tot(usl[1.0]*passing, failling)");
    
    
    
    doTheToy(Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3_GEN, Tight2012_etaPOGlargeabseta_bin0_pt_bin0_poly3);

    
    
    myFile->Close();
    
    
}




