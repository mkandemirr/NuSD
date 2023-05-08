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

#include "NuSD_config.h"

#ifndef GENERIC_PRIMARY_GENERATOR

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDAnalysisManager.hh"
#include "NuSDSimParameters.hh"
//#undef NDEBUG
//#include <assert.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//this is a thread-local singleton class
NuSDAnalysisManager* NuSDAnalysisManager::GetInstance() 
{
  static G4ThreadLocalSingleton<NuSDAnalysisManager> instance;
  return instance.Instance();
}

NuSDAnalysisManager::NuSDAnalysisManager() : 
	fFactoryOn(false),
	fEventID{-1},
	fNeutrinoEnergy{0.},
	fPositronEnergy{0.},
	fNeutronEnergy{0.},
	fNeutronAbsNucleusA{-1},
	fNeutronCaptureTime{0.},
	fEmittedScntPhotonNum{-1},
	
	fNeutrinoMomentumDirectionVec(),
	fPositronMomentumDirectionVec(),
	fNeutronMomentumDirectionVec(),
	fEventInitPosVec(),
	fNeutronCapturePosVec(),

	fNuScntParticlePDGVec(),
	fNuScntTrackIDVec(),
	fNuScntCopyNoVec(),
	fNuScntEdepVec(),
	fNuScntTimeVec(),
	
	fNeutronScntParticlePDGVec(),
	fNeutronScntTrackIDVec(),
	fNeutronScntCopyNoVec(),
	fNeutronScntEdepVec(),
	fNeutronScntTimeVec(),
	
	fPhotonDetCopyNoVec(),
	fPhotonDetTimeVec()
	
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDAnalysisManager::~NuSDAnalysisManager()
{
}

#ifdef CREATE_ROOT_FILE

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//will be called in BeginOfRunAction method of NuSDRunAction
void NuSDAnalysisManager::Book()
{
  NuSDSimParameters *simPar = NuSDSimParameters::GetInstance();

    // Create or get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  if(!fFactoryOn)
  {
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetVerboseLevel(1);
    //analysisManager->SetNtupleMerging(true);
    // Create directories 
    analysisManager->SetNtupleDirectoryName("ntuple");
  }

  // Open an output file
  G4bool fileOpen = analysisManager->OpenFile("output/NuSD");
  
	if (! fileOpen) 
  {
    G4cerr << "\n---> NuSDAnalysisManager::Book(): cannot open " 
           << analysisManager->GetFileName() << G4endl;
    return;
  }
   
  if(!fFactoryOn)
  {
    
    // Create ntuples.
    // Ntuples ids are generated automatically starting from 0.
    // The start value can be changed by:
    // analysisManager->SetFirstMtupleId(1);  
   
    // Create 1st ntuple (id = 0)
	  analysisManager->CreateNtuple("Ntuple1", "EventResult"); //TTree = NTuple 
	  analysisManager->CreateNtupleIColumn(0,"EventID"); // column Id = 0
	  analysisManager->CreateNtupleDColumn(0,"NeutrinoEnergy"); // column Id = 1
	  analysisManager->CreateNtupleDColumn(0,"PositronEnergy"); // column Id = 2
	  analysisManager->CreateNtupleDColumn(0,"NeutronEnergy"); // column Id = 3
	  analysisManager->CreateNtupleIColumn(0,"NeutronAbsNucleusA"); // column Id = 5
	  analysisManager->CreateNtupleDColumn(0,"NeutronCaptureTime"); // column Id = 6
	  
	  if( simPar->IsOpticalPhysicsOn() )
	    analysisManager->CreateNtupleIColumn(0,"EmittedScntPhotonNum");
	  
	  //3d vector
	  analysisManager->CreateNtupleDColumn(0,"NeutrinoMomentumDirectionVec", fNeutrinoMomentumDirectionVec); // column Id = 4
	  analysisManager->CreateNtupleDColumn(0,"PositronMomentumDirectionVec", fPositronMomentumDirectionVec); // column Id = 4
	  analysisManager->CreateNtupleDColumn(0,"NeutronMomentumDirectionVec",  fNeutronMomentumDirectionVec); // column Id = 4
	  analysisManager->CreateNtupleDColumn(0,"EventInitialPosVec",           fEventInitPosVec); // column Id = 4
	  analysisManager->CreateNtupleDColumn(0,"NeutronCapturePosVec",         fNeutronCapturePosVec); // column Id = 4
	  
	  analysisManager->CreateNtupleIColumn(0,"NuScntParticlePDGVec",   fNuScntParticlePDGVec);  // column Id = 11
	  analysisManager->CreateNtupleIColumn(0,"NuScntTrackIDVec",    fNuScntTrackIDVec);  // column Id = 11
	  analysisManager->CreateNtupleIColumn(0,"NuScntCopyNoVec",        fNuScntCopyNoVec);  // column Id = 11
	  analysisManager->CreateNtupleDColumn(0,"NuScntEdepVec",          fNuScntEdepVec);  // column Id = 12
	  analysisManager->CreateNtupleDColumn(0,"NuScntTimeVec",          fNuScntTimeVec);  // column Id = 13
	  
#if DETECTOR_TYPE == INHOMOGENEOUSCOMPOSITE
    analysisManager->CreateNtupleIColumn(0,"NeutronScntParticlePDGVec", fNeutronScntParticlePDGVec);  // column Id = 11
    analysisManager->CreateNtupleIColumn(0,"NeutronScntTrackIDVec",  fNeutronScntTrackIDVec);  // column Id = 11
    analysisManager->CreateNtupleIColumn(0,"NeutronScntCopyNoVec",      fNeutronScntCopyNoVec);  // column Id = 14
    analysisManager->CreateNtupleDColumn(0,"NeutronScntEdepVec",        fNeutronScntEdepVec);  // column Id = 15
    analysisManager->CreateNtupleDColumn(0,"NeutronScntTimeVec",        fNeutronScntTimeVec);  // column Id = 16
#endif
    
    if( simPar->IsOpticalPhysicsOn() )
    {  
      analysisManager->CreateNtupleIColumn(0,"PhotonDetCopyNoVec",  fPhotonDetCopyNoVec);  // column Id = 14
      analysisManager->CreateNtupleDColumn(0,"PhotonDetTimeVec",    fPhotonDetTimeVec);  // column Id = 14
    }  
      
	  analysisManager->FinishNtuple();
    
    fFactoryOn = true;       
  
  }
  
  G4cout << "\n----> Output file is open in " 
         << analysisManager->GetFileName() << "." 
         << analysisManager->GetFileType() << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//will be called in EndOfRunAction method of NuSDRunAction
void NuSDAnalysisManager::Save()
{
  if (! fFactoryOn) return;
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();    
  analysisManager->Write();
  analysisManager->CloseFile(); 
   
  G4cout << "\n----> Histograms and ntuples are saved\n" << G4endl;
      
  //delete G4AnalysisManager::Instance();
  //fFactoryOn = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDAnalysisManager::FillNtuple()
{
  NuSDSimParameters *simPar = NuSDSimParameters::GetInstance();
   
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
	analysisManager->FillNtupleIColumn(0, 0, fEventID);
	analysisManager->FillNtupleDColumn(0, 1, fNeutrinoEnergy);
	analysisManager->FillNtupleDColumn(0, 2, fPositronEnergy);
	analysisManager->FillNtupleDColumn(0, 3, fNeutronEnergy);
	analysisManager->FillNtupleIColumn(0, 4, fNeutronAbsNucleusA);
	analysisManager->FillNtupleDColumn(0, 5, fNeutronCaptureTime);
	
	if( simPar->IsOpticalPhysicsOn() )
	  analysisManager->FillNtupleIColumn(0, 6, fEmittedScntPhotonNum);
	
	//after all information are gathered, add row to ntuple
	analysisManager->AddNtupleRow(0); //parameter is ntupleId

} 

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//called in EndOfEventAction method of NuSDEventAction
void NuSDAnalysisManager::EndOfEvent()
{

#ifdef NuSD_DEBUG  
  PrintEventResults();
#endif

#ifdef CREATE_ROOT_FILE
  FillNtuple();
#endif  

  Reset();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Reset all values in the begining of the event.
void NuSDAnalysisManager::Reset()
{   
  fEventID = fNeutronAbsNucleusA  = -1;
  fEmittedScntPhotonNum = 0;
  fNeutrinoEnergy = fNeutronEnergy = fPositronEnergy = fNeutronCaptureTime = 0.;
    
  //3d vector
  fNeutrinoMomentumDirectionVec.clear();
  fPositronMomentumDirectionVec.clear();
  fNeutronMomentumDirectionVec.clear();
  fEventInitPosVec.clear();
  fNeutronCapturePosVec.clear();
  
  fNuScntParticlePDGVec.clear();	
  fNuScntTrackIDVec.clear();
  fNuScntCopyNoVec.clear();
  fNuScntEdepVec.clear();
  fNuScntTimeVec.clear();
     
  fNeutronScntParticlePDGVec.clear();	
  fNeutronScntTrackIDVec.clear();
  fNeutronScntCopyNoVec.clear();
  fNeutronScntEdepVec.clear();
  fNeutronScntTimeVec.clear();
  
  fPhotonDetCopyNoVec.clear();
  fPhotonDetTimeVec.clear();
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDAnalysisManager::PrintEventResults() const
{
  G4cout<<"\n----------------------Event "<<fEventID<<" Information----------------------------- ";
  G4cout<<"\nInverse Beta Decay";

  G4cout<<"\nAntineutrino Energy(MeV)         :"<<fNeutrinoEnergy; //not ready yet!!!return 0!!
  G4cout<<"\nPrimary Positron Energy(MeV)     :"<<fPositronEnergy;
  G4cout<<"\nPrimary Neutron Energy(keV)      :"<<fNeutronEnergy;
  G4cout<<"\nNeutron capture nucleus A        :"<<fNeutronAbsNucleusA;
  G4cout<<"\nNeutron capture time(us)         :"<<fNeutronCaptureTime;
  G4cout<<"\nEmitted Photon number            :"<<fEmittedScntPhotonNum;
 
  G4ThreeVector eip( fEventInitPosVec.at(0), fEventInitPosVec.at(1), fEventInitPosVec.at(2) );
  
  G4cout<<"\nEvent vertex pos(cm)             :"<<eip;
  
  if(fNeutrinoMomentumDirectionVec.size()!=0) //it can be zero when studying background particles, not IBD
  {
    G4ThreeVector amd( fNeutrinoMomentumDirectionVec.at(0), fNeutrinoMomentumDirectionVec.at(1), fNeutrinoMomentumDirectionVec.at(2) );
    G4ThreeVector pmd( fPositronMomentumDirectionVec.at(0), fPositronMomentumDirectionVec.at(1), fPositronMomentumDirectionVec.at(2) );
    G4ThreeVector nmd( fNeutronMomentumDirectionVec.at(0), fNeutronMomentumDirectionVec.at(1), fNeutronMomentumDirectionVec.at(2) );

    
    G4cout<<"\nNeutrino Momentum direction      :"<<amd;
    G4cout<<"\nPositron Momentum direction      :"<<pmd;
    G4cout<<"\nNeutron Momentum direction       :"<<nmd;
  }
  
  PrintNeutrinoSDOutput(); 
    
#if DETECTOR_TYPE == INHOMOGENEOUSCOMPOSITE
  PrintNeutronSDOutput();
#endif  
            
  PrintPhotonSDOutput();	     
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDAnalysisManager::PrintNeutrinoSDOutput() const
{

	G4cout<<G4endl;
	G4cout<<"-------------------NeutrinoSD Output----------------------------------------"<<G4endl;
  
  G4cout<<std::left<<std::setw(6+8)<<"CopyNo"<<std::setw(11+8)<<"ParticlePDG"<<std::setw(10+8)<<"TrackID"
        <<std::setw(11+8)<<"Energy(MeV)"<<std::setw(8+8)<<"Time(us)"<<G4endl;

  //G4cout<<std::setprecision(5)<<G4endl;
  
  for ( std::size_t i=0; i<fNuScntCopyNoVec.size(); ++i )
  {
    G4cout<<std::left<<std::setw(6+8)<<fNuScntCopyNoVec.at(i)<<
    std::setw(11+8)<<fNuScntParticlePDGVec.at(i)<<
    std::setw(10+8)<<fNuScntTrackIDVec.at(i)<<
    std::setw(11+8)<<fNuScntEdepVec.at(i)<<
    std::setw(8+8)<<fNuScntTimeVec.at(i)<<G4endl;
    
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDAnalysisManager::PrintNeutronSDOutput() const
{
	G4cout<<G4endl;
	G4cout<<"-------------------NeutronSD Output---------------------------------------"<<G4endl;
	
  G4cout<<std::left<<std::setw(6+8)<<"CopyNo"<<
  std::setw(11+8)<<"ParticlePDG"<<std::setw(10+8)<<"TrackID"<<
  std::setw(11+8)<<"Energy(MeV)"<<
  std::setw(8+8)<<"Time(us)"<<G4endl;

  //G4cout<<std::setprecision(5)<<G4endl;
  
	for ( std::size_t i=0; i<fNeutronScntCopyNoVec.size(); ++i )
	{
		  
		G4cout<<std::left<<std::setw(6+8)<<fNeutronScntCopyNoVec.at(i)<<
		std::setw(11+8)<<fNeutronScntParticlePDGVec.at(i)<<
		std::setw(10+8)<<fNeutronScntTrackIDVec.at(i)<<
		std::setw(11+8)<<fNeutronScntEdepVec.at(i)<<
		std::setw(8+8)<<fNeutronScntTimeVec.at(i)<<G4endl;

	} 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDAnalysisManager::PrintPhotonSDOutput() const
{
	G4cout<<G4endl;
	G4cout<<"-------------------PhotonSD Output---------------------------------------"<<G4endl;
	 
	//calculate total number of detected photons in each unit of the detector without considering time
	  
	std::map<int,int> map;
	
	for(std::size_t i =0; i<fPhotonDetCopyNoVec.size(); ++i)
  {
	  
		int copyNo = fPhotonDetCopyNoVec.at(i);
		    
		auto it = map.find(copyNo);
		if( it == map.end() ) //not found
		  map[copyNo] = 1;
		else
		  map.at(it->first) += 1;    
							  
  }  

	G4cout<<std::left<<std::setw(6+10)<<"CopyNo"<<std::setw(22)<<"Detected photon number"<<G4endl;
	
	for(auto el: map)
		G4cout<<std::left<<std::setw(6+10)<<el.first<<std::setw(22)<<el.second<<G4endl;	

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

