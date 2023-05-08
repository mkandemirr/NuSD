/*
This macro file is used to generate reactor anti-neutrino spectrum using Huber and Muller model, which is used as an input in Geant4
*/
void writeFile()
{
  //Reactor antiNu emission spect. u5(u-235), u8(u-238), p9(Pu-239) p1(Pu-241)
	//Huber and Muller model
	TF1 fSigma("fSigma"," (9.42*1e-44)*(x-1.294)*TMath::Sqrt( TMath::Power(x-1.294,2)-TMath::Power(0.511,2) )");
	
	TF1 u5("u5","TMath::Exp(4.367 - 4.577*x + 2.1*TMath::Power(x,2) - 0.5294*TMath::Power(x,3) + 0.06186*TMath::Power(x,4) -         0.002777*TMath::Power(x,5))",1.806,10.5);
	
	TF1 u8("u8","TMath::Exp(4.833e-01+1.927e-01*x-1.283e-01*TMath::Power(x,2)-6.762e-03*TMath::Power(x,3)+2.233e-03*TMath::Power(x,4)-1.536e-04*TMath::Power(x,5) )",1.806,10.5);
	
	TF1 p9("p9","TMath::Exp(4.757-5.392*x+2.563*TMath::Power(x,2)-6.596e-01*TMath::Power(x,3)+7.820e-02*TMath::Power(x,4)-3.536e-03*TMath::Power(x,5))",1.806,10.5);
	
	TF1 p1("p1","TMath::Exp(2.990-2.882*x+1.278*TMath::Power(x,2)-3.343e-01*TMath::Power(x,3)+3.905e-02*TMath::Power(x,4)-1.754e-03*TMath::Power(x,5))",1.806,10.5);
	
	TF1 totalSpectMidCycle("totalc","(0.586*u5+0.075*u8+0.292*p9+0.047*p1)*fSigma",1.806,10.5);

 //diff cross section
  TF1 diffCrossSection("probFunc(cosTheta)","0.5*(1-0.1*x)",-1,1); //x=cosTheta.normalized
  double theta = diffCrossSection.GetRandom(); //theta is the angle between positron and antiNu

  TH1D* hSpect      = new TH1D("nuHisto","nuHisto", 100,0,10);	
  TH1D* hAngleSpect = new TH1D("angleHisto","angleHisto",100,-1,1);	  
    
  int nentries = 1000000;
    
  for(int i=0;i<nentries;++i){
	  hSpect->Fill(totalSpectMidCycle.GetRandom());
	  
    double theta = diffCrossSection.GetRandom();
    hAngleSpect->Fill(theta);
  }
  
  TFile* file = new TFile("nuSpect.root","RECREATE");  
  hSpect->Write();
  hAngleSpect->Write();
  file->Close();
  delete file;
  
  hAngleSpect->Draw();

}
