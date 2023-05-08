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
>>Possible detector name:sweany, chandler, solid, hsp, prospect, panda, nulat.
>>Enable or disable the preprocessor COMPOSITESCNT depending on 
the selected detector type. activate it for sweany, chandler, and solid. 
>>if you activate the USEDEBUG macro, event summary will be printed for a few
events.
*/

#define detName "sweany"
#define COMPOSITESCNT
//#define USEDEBUG
//#define ENABLE_PHOTOSENSOR


#include <iostream>
#include <vector>


class PhotoSensor
{
  public:
    PhotoSensor(int copyNo) {fCopyNo = copyNo;}
    //Getters
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
                      double neutrinoEnergy,
                      double positronEnergy, 
                      double neutronEnergy,
                      const std::vector<double> *neutrinoMomentumDirection,
                      const std::vector<double> *positronMomentumDirection,
                      const std::vector<double> *neutronMomentumDirection,
                      const std::vector<double> *eventInitPosVec,

                      int neutronAbsNucleusA, 
                      double neutronCaptureTime, 
                      int    emittedScntPhotonNum,

                      const std::map<int,double> &nuScntMap, 
                      const std::map<int,double> &neutronScntMap, 
                      const std::map<int,PhotoSensor*> &photoSensorMap 
                      );
	   
//for both MainSD and NeutronSD           
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
     
#ifdef ENABLE_PHOTOSENSOR
                    
void SetPhotoSensorMap(const std::vector <int> *copyNoVec, 
                       const std::vector <double> *timeVec, 
                       std::map<int, PhotoSensor*> &map
                       ); 

#endif

//common for NuScnt and NeutronScnt                                        
void ApplyEnergyCutForEachUnit(std::map<int,double> &map, double energyCut);
void ApplyDetPhotonNumCutForEachPhotoSensor(std::map<int, PhotoSensor*> &map);

//------------------------------GLOBAL VARIABLES-----------------------------//
//NuScntCuts
double nuScntMinEdepTimeCut   	   = 0.;
double nuScntMaxEdepTimeCut   	   = 0.1;

double nuScntUnitMinEnergyCut      = 0.2;

#ifdef COMPOSITESCNT
//NeutronScntCuts
double neutronScntMinEdepTimeCut   = 0;
double neutronScntMaxEdepTimeCut   = 500.;

double neutronScntUnitMinEnergyCut = 0.2;

#else
//for delayed energy in neutrino scnt
double nuScntMinEdepTimeCut1   	   = 1;
double nuScntMaxEdepTimeCut1   	   = 500;

#endif

//promt and delayed cuts.Cuts for anti-neutrino detection efficiency calculation

double promptEnergyCut  = 2;
double delayedEnergyCut = 3;


#ifdef ENABLE_PHOTOSENSOR
///PhotonDetCuts
double minDetectedPhotonTimeCut    = 0.;
double maxDetectedPhotonTimeCut    = 0.1*1000000;

int minDetectedPhotonNumberCut 	   = 0;

#endif

//---------------------------------------------------------------------------//
void ade()
{
  gROOT->Reset();

  //boşlukara dikkat et yoksa macro hata veriyor!!!
  TChain * myChain1 = new TChain("ntuple/Ntuple1");
	myChain1->Add(" ../rootFiles/" detName"/NuSD.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t0.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t1.root");
	/*
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t2.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t3.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t4.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t5.root");
 */
	int     eventID{};
	double  neutrinoEnergy{};
  double  positronEnergy{};
	double  neutronEnergy{};
		
	std::vector<double> *neutrinoMomentumDirection{};
	std::vector<double> *positronMomentumDirection{};
  std::vector<double> *neutronMomentumDirection{};
  
  std::vector<double> *eventInitPosVec{};	
  
  int     neutronAbsNucleusA{};
	double  neutronCaptureTime{};
	int 		emittedScntPhotonNum{};
	
	std::vector <int>     *nuScntParticlePDGVec{};
	std::vector <int>     *nuScntParticleIDVec{};
	std::vector <int>     *nuScntCopyNoVec{};
	std::vector <double>  *nuScntEdepVec{};
	std::vector <double>  *nuScntTimeVec{};
	
  myChain1->SetBranchAddress("EventID",           &eventID);
  myChain1->SetBranchAddress("NeutrinoEnergy", 		&neutrinoEnergy); //MeV
  myChain1->SetBranchAddress("PositronEnergy", 		&positronEnergy); //MeV
  myChain1->SetBranchAddress("NeutronEnergy", 		&neutronEnergy); //keV
  
  myChain1->SetBranchAddress("NeutrinoMomentumDirectionVec", 	&neutrinoMomentumDirection);
  myChain1->SetBranchAddress("PositronMomentumDirectionVec", 	&positronMomentumDirection);
  myChain1->SetBranchAddress("NeutronMomentumDirectionVec", 	&neutronMomentumDirection);
    
  myChain1->SetBranchAddress("EventInitialPosVec", 	&eventInitPosVec);
  
  myChain1->SetBranchAddress("NeutronAbsNucleusA", 	&neutronAbsNucleusA);
  myChain1->SetBranchAddress("NeutronCaptureTime", 	&neutronCaptureTime); //us
  
#ifdef ENABLE_PHOTOSENSOR
  myChain1->SetBranchAddress("EmittedScntPhotonNum",&emittedScntPhotonNum); //us
#endif
  
  myChain1->SetBranchAddress("NuScntParticlePDGVec", &nuScntParticlePDGVec);
  myChain1->SetBranchAddress("NuScntParticleIDVec",  &nuScntParticleIDVec);
	myChain1->SetBranchAddress("NuScntCopyNoVec",      &nuScntCopyNoVec);
  myChain1->SetBranchAddress("NuScntEdepVec", 	     &nuScntEdepVec);
  myChain1->SetBranchAddress("NuScntTimeVec", 	     &nuScntTimeVec); //ns
  
#ifdef COMPOSITESCNT
  std::vector <int>     *neutronScntParticlePDGVec{};
	std::vector <int>     *neutronScntParticleIDVec{};
	std::vector <int>     *neutronScntCopyNoVec{};
	std::vector <double>  *neutronScntEdepVec{};
	std::vector <double>  *neutronScntTimeVec{};
	
	myChain1->SetBranchAddress("NeutronScntParticlePDGVec", &neutronScntParticlePDGVec);
  myChain1->SetBranchAddress("NeutronScntParticleIDVec",  &neutronScntParticleIDVec);
	myChain1->SetBranchAddress("NeutronScntCopyNoVec",      &neutronScntCopyNoVec);
  myChain1->SetBranchAddress("NeutronScntEdepVec", 	      &neutronScntEdepVec);
  myChain1->SetBranchAddress("NeutronScntTimeVec", 	      &neutronScntTimeVec); //ns
#endif

#ifdef ENABLE_PHOTOSENSOR  
  std::vector <int>     *photonDetCopyNoVec{};
	std::vector <double>  *photonDetTimeVec{};
	
  myChain1->SetBranchAddress("PhotonDetCopyNoVec", 	&photonDetCopyNoVec);
  myChain1->SetBranchAddress("PhotonDetTimeVec", 		&photonDetTimeVec); //ns
#endif
 
//---------------------------------------------------------------------//
 	std::map<int,double>        nuScntMap;
//#ifdef COMPOSITESCNT
	std::map<int,double>        neutronScntMap;
//#else
#ifndef COMPOSITESCNT
	std::map<int,double>        nuScntMap1; //for delayed energy
#endif

	std::map<int, PhotoSensor*> photoSensorMap;

	
#ifdef USEDEBUG
  std::size_t nentries = 10;
#else
  std::size_t nentries = myChain1->GetEntries();	
#endif
	
	
 
  int counter = 0;
		
  for (std::size_t i=0; i<nentries; ++i) 
  {
		myChain1->GetEntry(i);
		     
		SetSDMap(nuScntCopyNoVec, 
		         nuScntEdepVec, 
		         nuScntTimeVec, 
		         nuScntMap, 
		         nuScntMinEdepTimeCut, 
		         nuScntMaxEdepTimeCut
		         );
    ApplyEnergyCutForEachUnit(nuScntMap, nuScntUnitMinEnergyCut);  //MainSD
    
#ifdef COMPOSITESCNT		
		SetSDMap(neutronScntCopyNoVec, 
		         neutronScntEdepVec, 
		         neutronScntTimeVec, 
		         neutronScntMap, 
		         neutronScntMinEdepTimeCut,  
		         neutronScntMaxEdepTimeCut
		         );
		ApplyEnergyCutForEachUnit(neutronScntMap, neutronScntUnitMinEnergyCut); //NeutronSD
#else
    SetSDMap(nuScntCopyNoVec, 
		         nuScntEdepVec, 
		         nuScntTimeVec, 
		         nuScntMap1, 
		         nuScntMinEdepTimeCut1, 
		         nuScntMaxEdepTimeCut1
		         );
    ApplyEnergyCutForEachUnit(nuScntMap, nuScntUnitMinEnergyCut);  //MainSD

#endif	


	
				
#ifdef ENABLE_PHOTOSENSOR
		SetPhotoSensorMap(photonDetCopyNoVec, photonDetTimeVec, photoSensorMap);
		ApplyDetPhotonNumCutForEachPhotoSensor(photoSensorMap);
#endif

		//----------------------------------PRINT---------------------------------------------------------
#ifdef USEDEBUG		
		printEventResults(eventID, 
		                  neutrinoEnergy, 
		                  positronEnergy, 
		                  neutronEnergy, 
		                  neutrinoMomentumDirection, 
		                  positronMomentumDirection,    
		                  neutronMomentumDirection, 
		                  eventInitPosVec, 
		                  neutronAbsNucleusA, 
		                  neutronCaptureTime, 
		                  emittedScntPhotonNum, 
		                  nuScntMap, 
		                  neutronScntMap, 
		                  photoSensorMap 
		                  );
			      
    PrintSD("NuScnt", nuScntParticlePDGVec, nuScntParticleIDVec, nuScntCopyNoVec, nuScntEdepVec, nuScntTimeVec);

#ifdef COMPOSITESCNT    
		PrintSD("NeutronScnt", neutronScntParticlePDGVec, neutronScntParticleIDVec, neutronScntCopyNoVec, neutronScntEdepVec, neutronScntTimeVec);				    
#endif

#endif

    //-----STARTTTT---------------------------------------
     
    double promptEnergy 	= 0.;
		double delayedEnergy  = 0.;
		
		for(auto el: nuScntMap)
			promptEnergy += el.second;
			
#ifdef COMPOSITESCNT	
		for(auto el: neutronScntMap)
			delayedEnergy += el.second;	
#else
    for(auto el: nuScntMap1)
			delayedEnergy += el.second;
#endif
	
		//hPromptEnergy->Fill(promptEnergy);
		//hDelayedEnergy->Fill(delayedEnergy);	

#ifndef USEDEBUG 
		//time cut is already applied in SetSDMap function.you can test by changing the time and energy cut
		if(promptEnergy>promptEnergyCut && delayedEnergy>delayedEnergyCut ) 
		{
			counter++;				
		}
#endif  
    
    //----------FINISH---------------------------------------------

		neutrinoMomentumDirection->clear();
	  positronMomentumDirection->clear();
	  neutronMomentumDirection->clear();
	  eventInitPosVec->clear();
		

		//dont forget to clear!!!
		nuScntParticlePDGVec->clear();
		nuScntParticleIDVec->clear();
		nuScntCopyNoVec->clear();
		nuScntEdepVec->clear();
		nuScntTimeVec->clear();
	  nuScntMap.clear();
	  
#ifdef COMPOSITESCNT
    neutronScntParticlePDGVec->clear();	
    neutronScntParticleIDVec->clear();		  
	  neutronScntCopyNoVec->clear();
		neutronScntEdepVec->clear();
		neutronScntTimeVec->clear();
	  neutronScntMap.clear();
#else
    nuScntMap1.clear();
#endif	  

#ifdef ENABLE_PHOTOSENSOR	  
    photonDetCopyNoVec->clear();
    photonDetTimeVec->clear();
   
    for(auto el: photoSensorMap)
    {
      PhotoSensor* ps = el.second;
        delete ps;  

    }
    
		photoSensorMap.clear();
#endif
  
  } //end of event loop
  	
	cout<<"-----------------------------------"<<endl;
	cout<<"Detector name         : "<<detName<<endl;
	cout<<"Total number of events: "<<nentries<<endl;
	
#ifndef USEDEBUG
	cout<<std::setprecision(1)<<std::fixed<<endl;

  cout<<"Applied selection cuts"<<endl;
	cout<<nuScntMinEdepTimeCut<<"<=nuScntEdepTime(us)<"<<nuScntMaxEdepTimeCut<<" (promptSignal)"<<endl;
	
#ifdef COMPOSITESCNT
	cout<<neutronScntMinEdepTimeCut<<"<=ndEdepTime(us)<"<<neutronScntMaxEdepTimeCut<<" (delayedSignal)"<<endl;
#else
  cout<<nuScntMinEdepTimeCut1<<"<=nuScntEdepTime(us)<"<<nuScntMaxEdepTimeCut1<<" (delayedSignal)"<<endl;	
#endif
	
	cout<<"promptEnergy  cut: >"<<promptEnergyCut<<"MeV"<<endl;
	cout<<"delayedEnergy cut: >"<<delayedEnergyCut<<"MeV"<<endl;
	
	cout<<std::setprecision(0)<<std::fixed<<endl;
	/*kfNormal ile aynı sonucu veriyor.*/
	double binomEff	= static_cast<double>(counter)/nentries;
  cout<<"ade (%): "<<100*binomEff<<endl;
#endif

} //macro end

//---------------------------SETSD-----------------------------------------------

void SetSDMap(std::vector<int> *copyNoVec, 
              std::vector<double> *energyVec, 
              std::vector<double> *timeVec, 
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
             
//----------------------------Cut for NuScnt and NeutronScnt---------------------------------------  
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

#ifdef ENABLE_PHOTOSENSOR

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

#endif



//--------------------------CUT FOR PHOTONSD-------------------------------------------------
#ifdef ENABLE_PHOTOSENSOR
void ApplyDetPhotonNumCutForEachPhotoSensor(std::map<int, PhotoSensor*> &map)
{

	for (auto it = map.cbegin(); it != map.cend();  ) // no "++"!
		{
		  PhotoSensor *ps = it->second;
		  int detectedPhotonNum = ps->GetTimeSpectrum().size();
		  
		  if( detectedPhotonNum<minDetectedPhotonNumberCut )
		  {
		    delete ps;
		    map.erase(it++);
		  }else
		  {
		    ++it;
		  }
		  
	}

}
#endif


//---------------------------------PRINT--------------------------------------------

#ifdef USEDEBUG

void printEventResults(int eventID, 
					             double neutrinoEnergy,
					             double positronEnergy, 
					             double neutronEnergy,
					             const std::vector<double> *neutrinoMomentumDirection,
					             const std::vector<double> *positronMomentumDirection,
					             const std::vector<double> *neutronMomentumDirection,
					             const std::vector<double> *eventInitPosVec,
					              
					             int neutronAbsNucleusA, 
					             double neutronCaptureTime, 
					             int emittedScntPhotonNum,
                        
                       const std::map<int,double> &nuScntMap, 
                       const std::map<int,double> &neutronScntMap, 
                       const std::map<int,PhotoSensor*> &photoSensorMap 
	                    )
{
	cout<<"----------------------Event "<<eventID<<" Information----------------------------- "<<endl;
		 
	cout<<"Incoming Neutrino Energy(MeV)     : "<<neutrinoEnergy<<endl;	 
	cout<<"Primary Positron Energy(MeV)      : "<<positronEnergy<<endl;
	cout<<"Primary Neutron Energy(keV)       : "<<neutronEnergy<<endl;
	
	TVector3 amd_vec ( neutrinoMomentumDirection->at(0), neutrinoMomentumDirection->at(1), neutrinoMomentumDirection->at(2) );
	TVector3 pmd_vec( positronMomentumDirection->at(0), positronMomentumDirection->at(1), positronMomentumDirection->at(2) );
	TVector3 nmd_vec( neutronMomentumDirection->at(0), neutronMomentumDirection->at(1), neutronMomentumDirection->at(2) );
	/*
	cout<<"NeutrinoMomentumDirection         : "<<amd_vec<<endl;
	cout<<"PositronMomentumDirection         : "<<pmd_vec<<endl;
	cout<<"NeutronMomentumDirection          : "<<nmd_vec<<endl;
	*/
	double posCosScatterAngle     = amd_vec*pmd_vec;
  double neutronCosScatterAngle = amd_vec*nmd_vec;
  double posScatterAngle        = acos(posCosScatterAngle)*(180./TMath::Pi());
  double neutronScatterAngle    = acos(neutronCosScatterAngle)*(180./TMath::Pi());
	
	cout<<"AngleBetweenPosAndAntiNu(deg)     : "<<posScatterAngle<<endl;
	cout<<"AngleBetweenNeutronAndAntiNu(deg) : "<<neutronScatterAngle<<endl;
	
	cout<<"Neutron capture nucleus A         : "<<neutronAbsNucleusA<<endl;
	cout<<"Neutron capture time(us)          : "<<neutronCaptureTime<<endl;
	cout<<"Emitted scnt photon num           : "<<emittedScntPhotonNum<<endl;

	cout<<"-------------------NuScnt map---------------------------------------"<<endl;
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

#ifdef COMPOSITESCNT
	cout<<"-------------------NeutronScnt map---------------------------------------"<<endl;
	
	cout<<"Applied selection cuts"<<endl;
	cout<<neutronScntMinEdepTimeCut<<"<ndEdepTime(us)<"<<neutronScntMaxEdepTimeCut<<endl;
	cout<<"Edep>="<<neutronScntUnitMinEnergyCut<<" MeV (for each unit) "<<endl;
	
	cout<<std::left<<std::setw(6+10)<<"CopyNo"<<std::setw(11)<<"Energy(MeV)"<<endl;
	for(auto it = neutronScntMap.begin(); it != neutronScntMap.end(); ++it)
	{
	 cout<<std::left<<std::setw(6+10)<<it->first<<std::setw(11)<<it->second<<endl;	    
	}
	
	double delayedEnergy  = 0.;		
	for(auto el: neutronScntMap)
		delayedEnergy += el.second;
	
	cout<<std::left<<std::setw(6+10)<<"Total"<<std::setw(11)<<delayedEnergy<<endl;
#endif	
	
	
#ifdef ENABLE_PHOTOSENSOR	
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
#endif
  
}



//------------------------------FOR MainSD AND NeutronSD---------------------------------------------
void PrintSD(const string& name, std::vector <int> *particlePDGVec, std::vector <int> *particleIDVec, 
             std::vector <int> *copyNoVec, std::vector <double> *energyVec, std::vector<double> *timeVec)

{
	cout<<std::setprecision(10)<<std::fixed<<endl;
	cout<<"-----------------"<<name<<" step information----------------------"<<endl;
	
	cout<<std::setw(15)<<"PDG"<<std::setw(25)<<"ID"<<std::setw(25)<<"CopyNo"
	    <<std::setw(25)<<"Time(us)"<<std::setw(25)<<"Energy(MeV)"<<endl;
	
	for(std::size_t i =0; i<energyVec->size(); i++)
	{
		
		int particlePDG   = particlePDGVec->at(i);
		int particleID		= particleIDVec->at(i);
		int copyNo        = copyNoVec->at(i);    
		double energy  		= energyVec->at(i);
		double time       = timeVec->at(i);

    if(energy != 0.)
    {
      cout<<std::setw(15)<<particlePDG<<std::setw(25)<<particleID<<std::setw(25)<<copyNo
	    <<std::setw(25)<<time<<std::setw(25)<<energy<<endl;
    }
		

	} 
		
}


#endif



