//***********************************************************************
//* Neutrino Segmented Detector (NuSD) Copyright 2022 Mustafa Kandemir  *
//*                                                                     *
//* This file is part of NuSD.                                          *
//*                                                                     *
//* NuSD is free software: you can redistribute it and/or modify        * 
//* it under the terms of the GNU General Public License as published   *
//* by the Free Software Foundation, either version 3 of the License,   *
//* or any later version.                                               *
//*                                                                     *
//* NuSD is distributed in the hope that it will be useful, but         *
//* WITHOUT ANY WARRANTY; without even the implied warranty of          *
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        * 
//* GNU General Public License for more details.                        *
//*                                                                     *
//* You should have received a copy of the GNU General Public License   *
//* along with NuSD. If not, see <https://www.gnu.org/licenses/>.       *
//*                                                                     *
//***********************************************************************


/*
If you run NuSD by activating the GENERIC_PRIMARY_GENERATOR 
preprocessor macro in NuSD_config.h file, you can use 
this file for analysis.
*/


#define fileName "sweany"

#define USEDEBUG

#include <iostream>
#include <vector>


//for each triggered photosensor in an event, we create object of photosensor class
class PhotoSensor{

  public:
    PhotoSensor(int copyNo) {fCopyNo = copyNo;}

    int GetCopyNo() const {return fCopyNo; }
    int GetDetectedPhotonNum() const { return fTimeSpectrum.size(); }
    std::vector<double>& GetTimeSpectrum() { return fTimeSpectrum; }

  private:
    std::vector<double> fTimeSpectrum;
    int fCopyNo;

};


//---------------------------------------------------------------------------
#ifdef USEDEBUG

void printEventResults(int eventID, 
					             double particleEnergy,
					             int    emittedScntPhotonNum,
					             const std::vector<double> *eventInitPosVec,
                       const std::map<int,double> &nuScntMap, 
                       const std::map<int,PhotoSensor*> &photoSensorMap 
                       );
   
//for MainSD          
void PrintSD(const string& name,
             std::vector <int> *particlePDGVec, 
             std::vector <int> *particleIDVec, 
             std::vector <int> *copyNoVec, 
             std::vector <double> *energyVec, 
             std::vector<double> *timeVec
             ); 
	           	                             
#endif
 
//--------------------------------------------------------------------------- 
void SetSDMap(std::vector<int> *copyNoVec, 
              std::vector<double> *energyVec, 
              std::vector<double> *timeVec, 
              std::map<int,double> &map, 
              double minEdepTimeCut, 
              double maxEdepTimeCut
             ); 
                    
void SetPhotoSensorMap(const std::vector <int>    *copyNoVec, 
                       const std::vector <double> *timeVec, 
                       std::map<int, PhotoSensor*> &map
                      ); 

//common for NeutrinoSD and PhotonSD                                       
void ApplyEnergyCutForEachUnit(std::map<int,double> &map, double energyCut);

void ApplyDetPhotonNumCutForEachPhotoSensor(std::map<int, PhotoSensor*> &map);


//------------------------------GLOBAL VARIABLES-----------------------------//
//NuScntCuts
//us
double nuScntMinEdepTimeCut   	= 0.;
double nuScntMaxEdepTimeCut   	= 0.1;

double nuScntUnitMinEnergyCut   = 0.;


//PhotonDetCuts
//ns
double minDetectedPhotonTimeCut = 0.;
double maxDetectedPhotonTimeCut = 0.1*100000;

int minDetectedPhotonNumberCut 	= 0;


//---------------------------------------------------------------------------//
void lce()
{
  gROOT->Reset();

#ifndef USEDEBUG
  TCanvas *c = new TCanvas("c1","c1");

  c->SetCanvasSize(1200, 1000);

  //TAttPad Class Reference

  gPad->SetRightMargin(0.1);
  gPad->SetLeftMargin(0.12);	
  gPad->SetBottomMargin(0.1);
  gPad->SetTopMargin(0.05);  
  //gPad->SetLogz();


  //TStyle Class Reference
  gStyle->SetTitleOffset(1.0, "x");
  gStyle->SetTitleOffset(0.9, "y");
  gStyle->SetTitleOffset(1.1, "z");

  gStyle->SetTitleSize(0.055,"xyz");
  gStyle->SetTitleFont(62,"xyz");

  gStyle->SetLabelSize(0.048,"xyz");
  //gStyle->SetLabelFont(62,"xyz");

  //gStyle->SetOptStat(0);

#endif

  //boşlukara dikkat et yoksa macro hata veriyor!!!
  TChain * myChain1 = new TChain("ntuple/Ntuple1");
	myChain1->Add(" rootFiles/" fileName"/NuSD.root");
	myChain1->Add(" rootFiles/" fileName"/NuSD_t0.root");
	myChain1->Add(" rootFiles/" fileName"/NuSD_t1.root");
	
	myChain1->Add(" rootFiles/" fileName"/NuSD_t2.root");
	myChain1->Add(" rootFiles/" fileName"/NuSD_t3.root");
	myChain1->Add(" rootFiles/" fileName"/NuSD_t4.root");
	myChain1->Add(" rootFiles/" fileName"/NuSD_t5.root");
  
	
	int     eventID{};
	double  particleEnergy{};
  int 		emittedScntPhotonNum{};
  std::vector<double> *eventInitPosVec{};	
  
  int     neutronAbsNucleusA{};
	double  neutronCaptureTime{};
	
	std::vector <int>     *nuScntParticlePDGVec{};
	std::vector <int>     *nuScntParticleIDVec{};
	std::vector <int>     *nuScntCopyNoVec{};
	std::vector <double>  *nuScntEdepVec{};
	std::vector <double>  *nuScntTimeVec{};
	
	std::vector <int>     *photonDetCopyNoVec{};
	std::vector <double>  *photonDetTimeVec{};

  myChain1->SetBranchAddress("EventID",              &eventID);
  myChain1->SetBranchAddress("ParticleEnergy", 		   &particleEnergy); //MeV
  myChain1->SetBranchAddress("EmittedScntPhotonNum", &emittedScntPhotonNum); 
  myChain1->SetBranchAddress("EventInitialPosVec", 	 &eventInitPosVec);
  
  myChain1->SetBranchAddress("NuScntParticlePDGVec", &nuScntParticlePDGVec);
  myChain1->SetBranchAddress("NuScntParticleIDVec",  &nuScntParticleIDVec);
	myChain1->SetBranchAddress("NuScntCopyNoVec",      &nuScntCopyNoVec);
  myChain1->SetBranchAddress("NuScntEdepVec", 	     &nuScntEdepVec);
  myChain1->SetBranchAddress("NuScntTimeVec", 	     &nuScntTimeVec); //us
  
  myChain1->SetBranchAddress("PhotonDetCopyNoVec", 	&photonDetCopyNoVec);
  myChain1->SetBranchAddress("PhotonDetTimeVec", 		&photonDetTimeVec); //ns
 
  //---------------------------------------------------------------------------//

 	std::map<int,double>        nuScntMap;
	std::map<int, PhotoSensor*> photoSensorMap;

#ifdef USEDEBUG
  std::size_t nentries = 10;
#else
  std::size_t nentries = myChain1->GetEntries();	
#endif
	
	TH1D *hLce                  = new TH1D("LCE distribution","LCE distribution",1000,0,100);
	TH1D *hEmittedPhotonNumDist = new TH1D("Emitted photon number distribution","Emitted photon number distribution",1000,0,20000);
  TH1D *hPhotoElectronDist    =	new TH1D("PhotoElectron distribution","PhotoElectron distribution",100,0,2000);
  	
  for (std::size_t i=0; i<nentries; ++i) 
  {
    myChain1->GetEntry(i);
     
    SetSDMap(nuScntCopyNoVec, nuScntEdepVec, nuScntTimeVec, nuScntMap, nuScntMinEdepTimeCut, nuScntMaxEdepTimeCut);
    ApplyEnergyCutForEachUnit(nuScntMap, nuScntUnitMinEnergyCut);  //MainSD

    SetPhotoSensorMap(photonDetCopyNoVec, photonDetTimeVec, photoSensorMap);
    ApplyDetPhotonNumCutForEachPhotoSensor(photoSensorMap);
		
		//----------------------------------PRINT---------------------------------------------------------
#ifdef USEDEBUG		
		printEventResults(eventID, 
		                  particleEnergy, 
		                  emittedScntPhotonNum, 
		                  eventInitPosVec,    
		                  nuScntMap, 
		                  photoSensorMap 
		                 );
			      
		  // PrintSD("NuScnt", nuScntParticlePDGVec, nuScntParticleIDVec, nuScntCopyNoVec, nuScntEdepVec, nuScntTimeVec);
		 				  		    
#endif

    //-----STARTTTT--------
     
    double promptEnergy 	= 0.;
	  for(auto el: nuScntMap)
		  promptEnergy += el.second;
    
    //calculate total detected photon num in all pmt satisfying the time and minDetectedPhoton condition
		int totalDetectedPhotonNum = 0; 

		for(auto el: photoSensorMap)
		{
			PhotoSensor * ps = el.second;
			  
			int detectedPhotonNum  = ps->GetDetectedPhotonNum();
			totalDetectedPhotonNum += detectedPhotonNum;   

		}
      
    double lce = static_cast<double>(totalDetectedPhotonNum)/emittedScntPhotonNum;
   	lce *=100;
    
    if(promptEnergy == 1.){
    
      hLce->Fill(lce);  
      hEmittedPhotonNumDist->Fill(emittedScntPhotonNum);
      hPhotoElectronDist->Fill(totalDetectedPhotonNum);
    
    }
    
    //----------FINISH----------------
    
		//dont forget to clear!!!
		nuScntCopyNoVec->clear();
		nuScntEdepVec->clear();
		nuScntTimeVec->clear();
	  nuScntMap.clear();
	  
	  eventInitPosVec->clear();
	  
    photonDetCopyNoVec->clear();
    photonDetTimeVec->clear();
        
    for(auto el: photoSensorMap)
    {
      PhotoSensor* ps = el.second;
      delete ps;  

    }
    
		photoSensorMap.clear();

	} //end of event loop

#ifndef USEDEBUG
  //hLce->Draw();
  //hEmittedPhotonNumDist->Draw();
 hPhotoElectronDist->Draw();
  
    
  //IntrinsicResolution
  double meanEmittedPhotonNum = hEmittedPhotonNumDist->GetMean();
  double stdEmittedPhotonNum  = hEmittedPhotonNumDist->GetStdDev();
  double intrinsicResolution  = (stdEmittedPhotonNum)/meanEmittedPhotonNum;
    
  //LCE resolution
  double meanLCE              = hLce->GetMean();
  double stdLCE               = hLce->GetStdDev();
  //meanLCE                   = 15.18;
  //stdLCE                    = 0.41;
  double lceResolution        = (stdLCE)/meanLCE;
    
  //PhotoElectron resolution
  double meanPe               = hPhotoElectronDist->GetMean();
  double stdPe                = hPhotoElectronDist->GetStdDev();
  double peResolution         = (stdPe)/meanPe;
    
  //Statistical resolution arise from photosensor
  double deltaStatistic       = 1./sqrt(meanPe);
  
  double resolutionTest         = sqrt( pow (intrinsicResolution,2)  + pow(lceResolution,2) );
  double totalEnergyResolution  = sqrt( pow (intrinsicResolution,2) + pow(lceResolution,2) + pow(deltaStatistic, 2) );
  double totalEnergyResolution2 = sqrt( pow(peResolution,2) + pow(deltaStatistic, 2) );
  
  cout<<std::setprecision(4)<<std::fixed<<endl;
  
  cout<<"Mean EmittedPhotonNum   : "<<meanEmittedPhotonNum<<endl;
  cout<<"Std of EmittedPhotonNum : "<<stdEmittedPhotonNum<<endl;
  cout<<"Intrinsic Resolution    : "<<intrinsicResolution<<endl;
  cout<<"------------------------------------------------------"<<endl;
  cout<<"Mean LCE                :  "<<meanLCE<<endl;
  cout<<"Std dev of LCE          :  "<<stdLCE<<endl;
  cout<<"LCE resolution          :  "<<lceResolution<<endl;
  cout<<"------------------------------------------------------"<<endl;
  cout<<"Mean of p.e             :  "<<meanPe<<endl;
  cout<<"Std of p.e              :  "<<stdPe<<endl;
  cout<<"P.e resolution          :  "<<peResolution<<endl;
  cout<<"------------------------------------------------------"<<endl;
  cout<<"deltaStatistic          :  "<<deltaStatistic<<endl;
  cout<<"------------------------------------------------------"<<endl;
  cout<<"resolutionTest (intrinsic+lce)                         :  "<<resolutionTest<<endl;
  cout<<"Total energy resolution (intrinsic+lce+deltaStatistic) :  "<<totalEnergyResolution<<endl;
  cout<<"Total energy resolution2 (pe+deltaStatistic)           :  "<<totalEnergyResolution2<<endl;
 
#endif

  cout<<"-----------------------------------"<<endl;
  cout<<"Detector name           : "<<fileName<<endl;
  cout<<"Total number of events  : "<<nentries<<endl;

}

//---------------------------SETSD-----------------------------------------------

void SetSDMap(std::vector<int>     *copyNoVec, 
              std::vector<double>  *energyVec, 
              std::vector<double>  *timeVec, 
              std::map<int,double> &map, 
              double minEdepTimeCut, 
              double maxEdepTimeCut
              )
{

  for(std::size_t i =0; i<energyVec->size(); i++)
  {

    int copyNo 			= copyNoVec->at(i);
    double energy 	= energyVec->at(i);
    double time			= timeVec->at(i);

    if( time>=minEdepTimeCut && time<maxEdepTimeCut ) //time window
    {
    
      auto it =  map.find(copyNo);
      if( it == map.end() ) //not found
        map[copyNo] = energy;
      else
        map.at(it->first) += energy;
	     				
    }

  }

 
}
             


//----------------------------Cut for NuScnt and NeutronDet---------------------------------------  
void ApplyEnergyCutForEachUnit(std::map<int,double> &map, double energyCut)
{

  for (auto it = map.cbegin(); it != map.cend(); ) // no "++"!
  {
  
    if (it->second <= energyCut) 
    {
      map.erase(it++);
    }
    else
    {
      ++it;
    }
  
  }

}



//------------------------------SETPHOTOSD-------------------------------------------------//

void SetPhotoSensorMap( const std::vector<int> *copyNoVec, const std::vector<double> *timeVec, std::map<int,PhotoSensor*> &map )
                     
{
		
  for(std::size_t i =0; i<timeVec->size(); ++i)
  {

    int copyNo 			= copyNoVec->at(i);
    double time			= timeVec->at(i);

    if( time>=minDetectedPhotonTimeCut && time<maxDetectedPhotonTimeCut ) //time unit is nanosecond
    {
      auto it = map.find(copyNo);
      if( it == map.end() )
      {
        PhotoSensor *ps = new PhotoSensor(copyNo);
        ps->GetTimeSpectrum().push_back(time);
        //map[copyNo] = ps;
        map.insert( std::make_pair (copyNo, ps ) );
        //cout<<"AAA"<<endl;
      }else
      {
        PhotoSensor *ps = map.at(it->first);
        ps->GetTimeSpectrum().push_back(time);
        // cout<<"BBBB"<<endl;
      } //not found

    }
    
  }  

}

//--------------------------CUT FOR PHOTONSD-------------------------------------------------

void ApplyDetPhotonNumCutForEachPhotoSensor(std::map<int, PhotoSensor*> &map)
{

  for (auto it = map.cbegin(); it != map.cend();  ) // no "++"!
  {
    PhotoSensor *ps = it->second;
    int detectedPhotonNum = ps->GetTimeSpectrum().size();

    if(detectedPhotonNum<minDetectedPhotonNumberCut)
    {
      delete ps;
      map.erase(it++);
    }else
    {
      ++it;
    }

  }

}



//---------------------------------PRINT--------------------------------------------

#ifdef USEDEBUG

void printEventResults(int eventID, 
					             double particleEnergy,
					             int emittedScntPhotonNum,
					             const std::vector<double> *eventInitPosVec, 
                       const std::map<int,double> &nuScntMap,           
                       const std::map<int,PhotoSensor*> &photoSensorMap 
	                     )
{
	cout<<"----------------------Event "<<eventID<<" Information----------------------------- "<<endl;
		 
	cout<<"Particle Energy(MeV)     : "<<particleEnergy<<endl;	 
	cout<<"Emitted scnt photon num  : "<<emittedScntPhotonNum<<endl;

	TVector3 pos_vec ( eventInitPosVec->at(0), eventInitPosVec->at(1), eventInitPosVec->at(2) );

	cout<<"Event initial pos x      :"<<pos_vec.x()<<endl;
	cout<<"Event initial pos y      :"<<pos_vec.y()<<endl;
	cout<<"Event initial pos z      :"<<pos_vec.z()<<endl;

	cout<<"-------------------NeutrinoSD map---------------------------------------"<<endl;
	cout<<"Applied selection cuts"<<endl;
	cout<<nuScntMinEdepTimeCut<<"<nuScntEdepTime(us)<"<<nuScntMaxEdepTimeCut<<endl;
	cout<<"Edep>="<<nuScntUnitMinEnergyCut<<" MeV (for each unit)"<<endl;
	//cout<<"NuScntMinEdepTimeCut(us)     : "<<nuScntMinEdepTimeCut<<endl;
	//cout<<"NuScntMaxEdepTimeCut(us)     : "<<nuScntMaxEdepTimeCut<<endl;
	//cout<<"NuScntUnitMinEnergyCut(MeV)  : "<<nuScntUnitMinEnergyCut<<endl;
	
	cout<<std::left<<std::setw(6+10)<<"CopyNo"<<std::setw(11)<<"Energy(MeV)"<<endl;
	
	for(auto it = nuScntMap.begin(); it != nuScntMap.end(); ++it)
	{
    cout<<std::left<<std::setw(6+10)<<it->first<<std::setw(11)<<it->second<<endl;
	}
	
	double promptEnergy 	= 0.;
	for(auto el: nuScntMap)
		promptEnergy += el.second;
	
	cout<<std::left<<std::setw(6+10)<<"Total"<<std::setw(11)<<promptEnergy<<endl;


	cout<<"-------------------PhotonSD map---------------------------------------"<<endl;
	
	cout<<"Applied selection cuts"<<endl;
	cout<<"MinDetectedPhotonTime(ns)  : "<<minDetectedPhotonTimeCut<<endl;
	cout<<"MaxDetectedPhotonTime(ns)  : "<<maxDetectedPhotonTimeCut<<endl;
	cout<<"MinDetectedPhotonNumber    : "<<minDetectedPhotonNumberCut<<" (for each photo sensor)"<<endl;
	
	cout<<std::left<<std::setw(6+10)<<"CopyNo"<<std::setw(22)<<"Detected photon number"<<endl;
	 
	int totalDetectedPhotonNum = 0; 
	  
  for(auto el: photoSensorMap)
  {
    PhotoSensor * ps = el.second;
    int copyNo             = ps->GetCopyNo();     
    int detectedPhotonNum  = ps->GetDetectedPhotonNum();
    
    cout<<std::left<<std::setw(6+10)<<copyNo<<std::setw(22)<<detectedPhotonNum<<endl;
    totalDetectedPhotonNum += detectedPhotonNum;   
    
  }
  	
  cout<<std::left<<std::setw(6+10)<<"Total"<<std::setw(11)<<totalDetectedPhotonNum<<endl;

  
}



//------------------------------FOR MainSD AND NeutronSD---------------------------------------------
void PrintSD(const string& name, std::vector <int> *particlePDGVec, std::vector <int> *particleIDVec, 
             std::vector <int> *copyNoVec, std::vector <double> *energyVec, std::vector<double> *timeVec)

{
	cout<<std::setprecision(10)<<std::fixed<<endl;
	cout<<"----------------------------------"<<name<<"------------------------------"<<endl;
	
	cout<<std::setw(10)<<"PDG"<<std::setw(25)<<"ID"<<std::setw(25)<<"CopyNo"
	    <<std::setw(25)<<"Time(us)"<<std::setw(25)<<"Energy(MeV)\n";
	for(std::size_t i =0; i<energyVec->size(); i++)
	{
		
		int particlePDG   = particlePDGVec->at(i);
		int particleID		= particleIDVec->at(i);
		int copyNo        = copyNoVec->at(i);    
		double energy  		= energyVec->at(i);
		double time       = timeVec->at(i);

      if(energy != 0)
      {
      
        cout<<std::setw(10)<<particlePDG<<std::setw(25)<<particleID<<std::setw(25)<<copyNo
		    <<std::setw(25)<<time<<std::setw(25)<<energy<<"\n";
      }
		

	} 
	
	
}











#endif
