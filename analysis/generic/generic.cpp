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
>>Possible detector name: sweany, chandler, solid, hsp, prospect, panda, nulat.
>>Enable or disable the preprocessor COMPOSITESCNT depending on 
the selected detector type. activate it for sweany, chandler, and solid. 
>>if you activate the USEDEBUG macro, event summary will be printed for a few
events.
*/

#define detName "cc"
#define COMPOSITESCNT
#define USEDEBUG
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
                       double particleEnergy,
                       const std::vector<double> *eventInitPosVec, 
                       int    emittedScntPhotonNum,
                       const std::map<int,double> &nuScntMap, 
                       const std::map<int,double> &neutronScntMap, 
                       const std::map<int,PhotoSensor*> &photoSensorMap 
                       );
	   
//for both MainSD and NeutronSD           
void PrintSD(const string& name,
            std::vector <int> *particlePDGVec, 
            std::vector <int> *trackIDVec, 
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
                    
void SetPhotoSensorMap(const std::vector <int> *copyNoVec, 
                       const std::vector <double> *timeVec, 
                       std::map<int, PhotoSensor*> &map
                       ); 

//common for NuScnt and NeutronScnt                                        
void ApplyEnergyCutForEachUnit(std::map<int,double> &map, double energyCut);
void ApplyDetPhotonNumCutForEachPhotoSensor(std::map<int, PhotoSensor*> &map);

//------------------------------GLOBAL VARIABLES-----------------------------//
//NuScntCuts
double nuScntMinEdepTimeCut   	   = 0.;
double nuScntMaxEdepTimeCut   	   = 0.1;

double nuScntUnitMinEnergyCut      = 0.;

#ifdef COMPOSITESCNT
//NeutronScntCuts
double neutronScntMinEdepTimeCut   = 1.;
double neutronScntMaxEdepTimeCut   = 600.;

double neutronScntUnitMinEnergyCut = 0.;

#endif

///PhotonDetCuts
double minDetectedPhotonTimeCut    = 0.;
double maxDetectedPhotonTimeCut    = 0.1*1000000;

int minDetectedPhotonNumberCut 	   = 0;


//---------------------------------------------------------------------------//
void generic()
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
//	myChain1->Add(" ../rootFiles/" detName"/NuSD.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t0.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t1.root");
	/*
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t2.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t3.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t4.root");
	myChain1->Add(" ../rootFiles/" detName"/NuSD_t5.root");
 */
	int     eventID{};
	double  particleEnergy{};
 
  std::vector<double> *eventInitPosVec{};	
	int 		emittedScntPhotonNum{};
	
	std::vector <int>     *nuScntParticlePDGVec{};
	std::vector <int>     *nuScntTrackIDVec{};
	std::vector <int>     *nuScntCopyNoVec{};
	std::vector <double>  *nuScntEdepVec{};
	std::vector <double>  *nuScntTimeVec{};
	
  myChain1->SetBranchAddress("EventID",           &eventID);
  myChain1->SetBranchAddress("ParticleEnergy", 		&particleEnergy); //MeV
    
  myChain1->SetBranchAddress("EventInitialPosVec", 	&eventInitPosVec);
    
#ifdef ENABLE_PHOTOSENSOR
  myChain1->SetBranchAddress("EmittedScntPhotonNum", &emittedScntPhotonNum); //us
#endif
  
  myChain1->SetBranchAddress("NuScntParticlePDGVec", &nuScntParticlePDGVec);
  myChain1->SetBranchAddress("NuScntTrackIDVec",  &nuScntTrackIDVec);
	myChain1->SetBranchAddress("NuScntCopyNoVec",      &nuScntCopyNoVec);
  myChain1->SetBranchAddress("NuScntEdepVec", 	     &nuScntEdepVec);
  myChain1->SetBranchAddress("NuScntTimeVec", 	     &nuScntTimeVec); //ns
  
#ifdef COMPOSITESCNT
  std::vector <int>     *neutronScntParticlePDGVec{};
	std::vector <int>     *neutronScntTrackIDVec{};
	std::vector <int>     *neutronScntCopyNoVec{};
	std::vector <double>  *neutronScntEdepVec{};
	std::vector <double>  *neutronScntTimeVec{};
	
	myChain1->SetBranchAddress("NeutronScntParticlePDGVec", &neutronScntParticlePDGVec);
  myChain1->SetBranchAddress("NeutronScntTrackIDVec",  &neutronScntTrackIDVec);
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
	std::map<int,double>        neutronScntMap;
	std::map<int, PhotoSensor*> photoSensorMap;
	
#ifdef USEDEBUG
  std::size_t nentries = 50;
#else
  std::size_t nentries = myChain1->GetEntries();	
#endif
	
	/*
	  Create histograms
	
	*/	
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
#endif		
				
#ifdef ENABLE_PHOTOSENSOR
		SetPhotoSensorMap(photonDetCopyNoVec, photonDetTimeVec, photoSensorMap);
		ApplyDetPhotonNumCutForEachPhotoSensor(photoSensorMap);
#endif

		//----------------------------------PRINT---------------------------------------------------------
#ifdef USEDEBUG		
		printEventResults(eventID, 
		                  particleEnergy, 
		                  eventInitPosVec, 
		                  emittedScntPhotonNum, 
		                  nuScntMap, 
		                  neutronScntMap, 
		                  photoSensorMap 
		                  );
			      
    PrintSD("NuScnt", nuScntParticlePDGVec, nuScntTrackIDVec, nuScntCopyNoVec, nuScntEdepVec, nuScntTimeVec);

#ifdef COMPOSITESCNT    
		PrintSD("NeutronScnt", neutronScntParticlePDGVec, neutronScntTrackIDVec, neutronScntCopyNoVec, neutronScntEdepVec, neutronScntTimeVec);				    
#endif

#endif

    //-----STARTTTT---------------------------------------
     
     //MAKE YOUR ANALYSIS 
    
    //----------FINISH---------------------------------------------

	  eventInitPosVec->clear();
		

		//dont forget to clear!!!
		nuScntParticlePDGVec->clear();
		nuScntTrackIDVec->clear();
		nuScntCopyNoVec->clear();
		nuScntEdepVec->clear();
		nuScntTimeVec->clear();
	  nuScntMap.clear();
	  
#ifdef COMPOSITESCNT
    neutronScntParticlePDGVec->clear();	
    neutronScntTrackIDVec->clear();		  
	  neutronScntCopyNoVec->clear();
		neutronScntEdepVec->clear();
		neutronScntTimeVec->clear();
	  neutronScntMap.clear();
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



//---------------------------------PRINT--------------------------------------------

#ifdef USEDEBUG

void printEventResults(int eventID, 
					             double particleEnergy,
					             const std::vector<double> *eventInitPosVec, 
					             int emittedScntPhotonNum,
                       const std::map<int,double> &nuScntMap, 
                       const std::map<int,double> &neutronScntMap, 
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
void PrintSD(const string& name, std::vector <int> *particlePDGVec, std::vector <int> *trackIDVec, 
             std::vector <int> *copyNoVec, std::vector <double> *energyVec, std::vector<double> *timeVec)

{
	cout<<std::setprecision(10)<<std::fixed<<endl;
	cout<<"-----------------"<<name<<" step information----------------------"<<endl;
	
	cout<<std::setw(15)<<"PDG"<<std::setw(25)<<"TrackID"<<std::setw(25)<<"CopyNo"
	    <<std::setw(25)<<"Time(us)"<<std::setw(25)<<"Energy(MeV)"<<endl;
	
	for(std::size_t i =0; i<energyVec->size(); i++)
	{
		
		int particlePDG   = particlePDGVec->at(i);
		int trackID		    = trackIDVec->at(i);
		int copyNo        = copyNoVec->at(i);    
		double energy  		= energyVec->at(i);
		double time       = timeVec->at(i);

    if(energy != 0.)
    {
      cout<<std::setw(15)<<particlePDG<<std::setw(25)<<trackID<<std::setw(25)<<copyNo
	    <<std::setw(25)<<time<<std::setw(25)<<energy<<endl;
    }
		

	} 
		
}


#endif



