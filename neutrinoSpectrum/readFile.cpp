//read neutrino spectrum from file

void readFile()
{
  TH1D* hNewSpect       = new TH1D("Neutrino spectrum","Neutrino spectrum", 100,0,10);
  TH1D* hNewAngleSpect  = new TH1D("IBD diff cross section","IBD diff cross section", 100,-1,1);	
  
  TFile* inputFile   = new TFile("nuSpect.root","READ");
  
  TH1D * hSpect      = (TH1D*)inputFile->Get("nuHisto");
  TH1D*  hAngleSpect = (TH1D*)inputFile->Get("angleHisto");
  
  int nentries  = hSpect->GetEntries();
  int nentries2 = hAngleSpect->GetEntries();
  
  //cout<<"size: "<<nentries<<endl;
  
  for(int i=0;i<nentries;++i){  
    hNewSpect->Fill(hSpect->GetRandom());
  }
  
  for(int i=0;i<nentries2;++i){  
    hNewAngleSpect->Fill(hAngleSpect->GetRandom());
  }
  
  //hSpect->Draw();
  hNewSpect->Draw();
  //hNewAngleSpect->Draw();

}
