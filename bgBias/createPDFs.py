import re, os

TemplateSignal_AllBin = [
                         #"scale[1,0.9,1.1]",
                         #"largerResPass[1,0.,2.]",
                         #"largerResFail[1,0.,2.]",
                         "expr::NewMean1p('mean1p*scale',mean1p,scale)",
                         "expr::NewMean2p('mean2p*scale',mean2p,scale)",
                         "expr::NewMean1f('mean1f*scale',mean1f,scale)",
                         "expr::NewMean2f('mean2f*scale',mean2f,scale)",
                         "expr::NewSigma1p('sigma1p*largerResPass',sigma1p,largerResPass)",
                         "expr::NewSigma2p('sigma2p*largerResPass',sigma2p,largerResPass)",
                         "expr::NewSigma1f('sigma1f*largerResFail',sigma1f,largerResFail)",
                         "expr::NewSigma2f('sigma2f*largerResFail',sigma2f,largerResFail)",
                         "Voigtian::theSig1p(mass, NewMean1p, width1p[2.495], NewSigma1p)",
                         "Voigtian::theSig2p(mass, NewMean2p, width2p[2.495], NewSigma2p)",
                         "SUM::sVoigP(vPropp*theSig1p,theSig2p)",
                         "Voigtian::theSig1f(mass, NewMean1f, width1f, NewSigma1f)",
                         "Voigtian::theSig2f(mass, NewMean2f, width2f, NewSigma2f)",
                         "SUM::sVoigF(vPropf*theSig1f,theSig2f)",
                         "Exponential::expP(mass, lsp)",
                         "Exponential::expF(mass, lsf)",
                         "ArgusBG::argF(mass, ap0, ap1)",
                         "SUM::shapeF(vFrac*argF, expF)",
                         "SUM::signalPass(vPropTotp*sVoigP,expP)",
                         "SUM::signalFail( vPropTotf*sVoigF,shapeF)",
                         #"Exponential::backgroundPass(mass, lp[0,-5,5])",
                         #"Exponential::backgroundFail(mass, lf[0,-5,5])",
                         "RooBernstein::backgroundPass(mass,{a0,a1,a2,a3,a4})",
                         "RooBernstein::backgroundFail(mass,{a0,a1,a2,a3,a4})",
                         "efficiency[0.9,0,1]",
                         "signalFractionInPassing[0.9]"
                                    ]

TemplateSignal_HighBin = [
                          #  "scale[1,0.9,1.1]",
                          #"largerResPass[1,0.,2.]",
                          "expr::NewMean('mean*scale',mean,scale)",
                          "expr::NewSigma('sigma*largerResPass',sigma,largerResPass)",
                          "CBShape::crystal(mass, NewMean, NewSigma,alpha, n)",
                          "RooLandau::pLandau(mass, Lmp,wp)",
                          "ArgusBG::fArg(mass, ap0, ap1)",
                          "RooLandau::fLandau(mass, Lmf,wf)",
                          "SUM::argLand(vFrac*fLandau, fArg)",
                          "SUM::signalPass(vProp*crystal,pLandau)",
                          "SUM::signalFail(vProp*crystal,argLand)",
                          "RooBernstein::backgroundPass(mass,{a0,a1,a2,a3,a4})",
                          "RooBernstein::backgroundFail(mass,{a0,a1,a2,a3,a4})",
                          "efficiency[0.9,0,1]",
                          "signalFractionInPassing[0.9]"
                          ]


Template_beginingOfThePDF = ["expr::nSignalPass('efficiency*fSigAll*numTot', efficiency, fSigAll,numTot)",
                             "expr::nSignalFail('(1-efficiency)*fSigAll*numTot', efficiency, fSigAll,numTot)",
                             "expr::nBkgPass('effBkg*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)",
                             "expr::nBkgFail('(1-effBkg)*(1-fSigAll)*numTot', effBkg,fSigAll,numTot)"]

Template_endOfThePDF = ["SUM::passing(nSignalPass*signalPass,nBkgPass*backgroundPass)",
                        "SUM::failling(nSignalFail*signalFail,nBkgFail*backgroundFail)",
                        "SUM::tot(usl[1.0]*passing, failling)"]
                        


file = open("TOGCPFTIPMVA_higgsSFdata.info","r")
files = file.readlines()
file.close()

file = open("TOGCPFTIPMVA_higgsSF.info","r")
filesMC = file.readlines()
file.close()

compteur=0 
for line in files:
    ligneSplitted = re.split(" ",line)
    ligneMCSplitted = re.split(" ",filesMC[compteur])
    if ("pt_bin5" in ligneSplitted[1]):
        localPDF = []
        localPDF.extend(["a0["+str(ligneSplitted[2])+",0,50]"])
        localPDF.extend(["a1["+str(ligneSplitted[3])+",0,50]"])
        localPDF.extend(["a2["+str(ligneSplitted[4])+",0,50]"])
        localPDF.extend(["a3["+str(ligneSplitted[5])+",0,50]"])
        localPDF.extend(["a4["+str(ligneSplitted[6])+",0,50]"])
        localPDF.extend(["scale["+str(ligneSplitted[12])+",0.9,1.1]"])
        localPDF.extend(["largerResPass["+str(ligneSplitted[10])+",0.,2.]"])
        localPDF.extend(["effBkg["+str(ligneSplitted[7])+",0,1]"])
        localPDF.extend(["fSigAll["+str(ligneSplitted[9])+",0,1]"])
        localPDF.extend(["numTot["+str(ligneSplitted[11])+",0,1e10]"])
        localPDF.extend(["efficiency["+str(ligneSplitted[8])+",0,1]"])
        localPDF.extend(["Lmf["+str(ligneMCSplitted[2])+",90,100]"])
        localPDF.extend(["Lmp["+str(ligneMCSplitted[3])+",90,100]"])
        localPDF.extend(["alpha["+str(ligneMCSplitted[4])+",0.5, 5.]"])
        localPDF.extend(["ap0["+str(ligneMCSplitted[5])+",88,93]"])
        localPDF.extend(["ap1["+str(ligneMCSplitted[6])+",-6,-0.5]"])
        localPDF.extend(["mean["+str(ligneMCSplitted[10])+",80,100]"])
        localPDF.extend(["n["+str(ligneMCSplitted[11])+",0., 100.]"])
        localPDF.extend(["sigma["+str(ligneMCSplitted[13])+",1,20]"])
        localPDF.extend(["vFrac["+str(ligneMCSplitted[14])+",0.5,1]"])
        localPDF.extend(["vProp["+str(ligneMCSplitted[15])+",0.5,1]"])
        localPDF.extend(["wf["+str(ligneMCSplitted[16])+",0,10]"])
        localPDF.extend(["wp["+str(ligneMCSplitted[17])+",0,10]"])
        
        compteur+=1
        localPDF+=Template_beginingOfThePDF
        localPDF+=TemplateSignal_AllBin
        localPDF+=Template_endOfThePDF
        nomPDF="Tight2012_etaPOGlarge"+ligneSplitted[0]+"_"+ligneSplitted[1]
        print 'RooWorkspace* '+nomPDF+' = new RooWorkspace("'+nomPDF+'");'
        for wLine in localPDF:
            print nomPDF+'->factory("'+wLine+'");'
        continue
    # print "on est dans ",ligneSplitted[0]," ",ligneSplitted[1]

    localPDF = []
    localPDF.extend(["a0["+str(ligneSplitted[2])+",0,50]"])
    localPDF.extend(["a1["+str(ligneSplitted[3])+",0,50]"])
    localPDF.extend(["a2["+str(ligneSplitted[4])+",0,50]"])
    localPDF.extend(["a3["+str(ligneSplitted[5])+",0,50]"])
    localPDF.extend(["a4["+str(ligneSplitted[6])+",0,50]"])
    localPDF.extend(["scale["+str(ligneSplitted[13])+",0.9,1.1]"])
    localPDF.extend(["largerResPass["+str(ligneSplitted[11])+",0.,2.]"])
    localPDF.extend(["largerResFail["+str(ligneSplitted[10])+",0.,2.]"])
    localPDF.extend(["effBkg["+str(ligneSplitted[7])+",0,1]"])
    localPDF.extend(["fSigAll["+str(ligneSplitted[9])+",0,1]"])
    localPDF.extend(["numTot["+str(ligneSplitted[12])+",0,1e10]"])
    localPDF.extend(["efficiency["+str(ligneSplitted[8])+",0,1]"])
    localPDF.extend(["mean1p["+str(ligneMCSplitted[10])+",80,100]"])
    localPDF.extend(["mean2p["+str(ligneMCSplitted[12])+",80,100]"])
    localPDF.extend(["mean1f["+str(ligneMCSplitted[9])+",80,100]"])
    localPDF.extend(["mean2f["+str(ligneMCSplitted[11])+",80,100]"])
    localPDF.extend(["sigma1p["+str(ligneMCSplitted[15])+",1,20]"])
    localPDF.extend(["sigma2p["+str(ligneMCSplitted[17])+",1,20]"])
    localPDF.extend(["sigma1f["+str(ligneMCSplitted[14])+",0,20]"])
    localPDF.extend(["sigma2f["+str(ligneMCSplitted[16])+",0,20]"])
    localPDF.extend(["vPropp["+str(ligneMCSplitted[22])+",0,1]"])
    localPDF.extend(["vPropf["+str(ligneMCSplitted[21])+",0,1]"])
    localPDF.extend(["lsp["+str(ligneMCSplitted[8])+",-5,5]"])
    localPDF.extend(["lsf["+str(ligneMCSplitted[7])+",-5,5]"])
    localPDF.extend(["ap0["+str(ligneMCSplitted[2])+",88,93]"])
    localPDF.extend(["ap1["+str(ligneMCSplitted[3])+",-5,-0.5]"])
    localPDF.extend(["vFrac["+str(ligneMCSplitted[18])+",0.5,1]"])
    localPDF.extend(["vPropTotp["+str(ligneMCSplitted[20])+",0.5,1]"])
    localPDF.extend(["vPropTotf["+str(ligneMCSplitted[19])+",0.5,1]"])
    localPDF.extend(["width1f["+str(ligneMCSplitted[23])+",1,2]"])
    localPDF.extend(["width2f["+str(ligneMCSplitted[24])+",1,2]"])


    compteur+=1
#if (("pt_bin0" in ligneSplitted[1]) or ("pt_bin1" in ligneSplitted[1]) or ("pt_bin2" in ligneSplitted[1])):
    #localPDF+=TemplateSignal_LowPt
        #else:
    localPDF+=Template_beginingOfThePDF
    localPDF+=TemplateSignal_AllBin
    localPDF+=Template_endOfThePDF
    nomPDF="Tight2012_etaPOGlarge"+ligneSplitted[0]+"_"+ligneSplitted[1]
    print 'RooWorkspace* '+nomPDF+' = new RooWorkspace("'+nomPDF+'");'
    for wLine in localPDF:
        print nomPDF+'->factory("'+wLine+'");'
