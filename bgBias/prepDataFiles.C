

nbOfFit = 1;
//TFile *myFile = new TFile("TnP_ElecID_data_JSON_huguesTest_denom_pt.root");

//TFile *myOutFile = new TFile("thePlotFile.root","RECREATE");
TString thePath = ".";
TString kind;
prepDataFiles(){
//	TDirectory *theDr = (TDirectory*) myFile->Get("eleIDdir");///denom_pt/fit_eff_plots");
	int myIndex;	
	
	TSystemDirectory dir(thePath, thePath);
	TSystemFile *file;
	TString fname;
	TIter next(dir.GetListOfFiles());
	while ((file=(TSystemFile*)next())) {
		fname = file->GetName();
		if (fname.BeginsWith("TnP")&& fname.Contains("data")) {
	
			cout << "theName " << fname << endl;
			ofstream myfile;
	/*		myfile.open ("example.txt");
			myfile << "Writing this to a file.\n";
			myfile.close();*/
			TFile *myFile = new TFile(fname);
			TIter nextkey(myFile->GetListOfKeys());
			TKey *key;
			while (key = (TKey*)nextkey()) {
				TString theTypeClasse = key->GetClassName();
				TString theNomClasse = key->GetTitle();
				if ( theTypeClasse == "TDirectoryFile"){
					TDirectory *theDr = (TDirectory*) myFile->Get(theNomClasse);
					TIter nextkey2(theDr->GetListOfKeys());
					TKey *key2;
					while (key2 = (TKey*)nextkey2()) {
						TString theTypeClasse2 = key2->GetClassName();
						TString theNomClasse2 = key2->GetTitle();	
						myfile.open (theNomClasse2+"data.info");
						if ( theTypeClasse == "TDirectoryFile"){
							cout << "avant " << endl;
							TDirectory *theDr2 = (TDirectory*) myFile->Get(theNomClasse+"/"+theNomClasse2);
							cout << "apres " << endl;
							TIter nextkey3(theDr2->GetListOfKeys());
							TKey *key3;
							while (key3 = (TKey*)nextkey3()) {
								TString theTypeClasse3 = key3->GetClassName();
								TString theNomClasse3 = key3->GetTitle();	
								cout << "type = " << theTypeClasse3 << " nom = " << theNomClasse3 << endl;
								if ((theNomClasse3.Contains("TOGCPFTIPMVA"))) {
									/*cout << "coucou c nous " << endl;
									cout << "the nom classe 2 " << theNomClasse2 << endl;
									cout << "the proposed name = " << theName[theIndex] << endl;
									cout << "the nom classe 3 " << theNomClasse3 << endl;*/
									TString localClasse3 = theNomClasse3;
									localClasse3.ReplaceAll("__","%");
									cout << "apres " << localClasse3 << endl;
									TObjArray* listBin = localClasse3.Tokenize('%');
									TString first = ((TObjString*)listBin->At(0))->GetString();
									TString second = ((TObjString*)listBin->At(2))->GetString();
						//			if (second == "pt_bin5") continue;
									myfile << first;
									myfile << " " << second << " ";
									cout << "coucou la on va récupérer le rooFitResult " << endl;
								/*	TCanvas *theCanvas = (TCanvas*) myFile->Get(theNomClasse+"/"+theNomClasse2+"/"+theNomClasse3+"/fit_canvas");
									theCanvas->Print("fitPlots/"+kind+"_"+theName[theIndex]+"_"+theNomClasse3+".png");*/
							/*		TDirectory *theDr3 = (TDirectory*) myFile->Get(theNomClasse+"/"+theNomClasse2+"/"+theNomClasse3);
									TIter nextkey4(theDr3->GetListOfKeys());
									TKey *key4;
									while (key4 = (TKey*) nextkey4()) {
										TString theTypeClasse4 = key4->GetClassName();
										TString theNomClasse4 = key4->GetTitle();
										cout << " the Type classe 4 = " << theTypeClasse4 << endl;
										cout << " the Nom Classe 4 = " << theNomClasse4 << endl;
										if (theTypeClasse4="RooFitResult") {*/
									RooFitResult *theResults = (RooFitResult*) myFile->Get(theNomClasse+"/"+theNomClasse2+"/"+theNomClasse3+"/fitresults");
									theResults->Print();
									//RooArgList theParam = theResuts->floatParsFinal();
									RooArgList theParam = theResults->floatParsFinal();
									theParam.Print();
									int taille = theParam.getSize();
									for (int m = 0 ; m < taille ; m++){
										cout << "m=" << m << endl;
									RooAbsArg *theArg = (RooAbsArg*) theParam.at(m);
				//					theArg->Print();
//									oout << "root arg list = " << theParam.getSize() << endl;	
									RooAbsReal *theReal = (RooAbsReal*) theArg;
								//	cout << "the value " << theReal.getVal() << endl;
										myfile << theReal->getVal() << " " ;
									}		
								/*	RooAbsArg *fSigArg = (RooAbsArg*) theParam.at(4);
									RooAbsReal *fSig = (RooAbsReal*) fSigArg;
									
									RooAbsArg *feffBg = (RooAbsArg*) theParam.at(2);
									RooAbsReal *fSig = (RooAbsReal*) fSigArg;*/
									
									myfile << "\n";
/*										}
									}*/
									//delete myfile;
								}
							}
						}
						myfile.close();

					}
			
				}
			}
			delete myFile;
		}
	
	}

}
