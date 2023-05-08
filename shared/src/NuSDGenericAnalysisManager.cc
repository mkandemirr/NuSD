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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef GENERIC_PRIMARY_GENERATOR

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4AnalysisManager.hh"
#include "NuSDGenericAnalysisManager.hh"
#include "G4RunManager.hh"
#include "NuSDSimParameters.hh"

//#undef NDEBUG
//#include <assert.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//this is thread-local singleton class
NuSDGenericAnalysisManager* NuSDGenericAnalysisManager::GetInstance() 
{
  static G4ThreadLocalSingleton<NuSDGenericAnalysisManager> instance;
  return instance.Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDGenericAnalysisManager::NuSDGenericAnalysisManager() : 
	
	fFactoryOn(false),
	fEventID{-1},
	fParticleEnergy{0.},
	fEmittedScntPhotonNum{-1},
	
	fEventInitPosVec(),
	
	//common
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

NuSDGenericAnalysisManager::~NuSDGenericAnalysisManager()
{
}

#ifdef CREATE_ROOT_FILE

//will be called in BeginOfRunAction method of G1RunAction
void NuSDGenericAnalysisManager::Book()
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
    G4cerr << "\n---> NuSDGenericAnalysisManager::Book(): cannot open " 
           << analysisManager->GetFileName() << G4endl;
    return;
  }
  
  if(!fFactoryOn)
  {
    //analysisManager->SetNtupleDirectoryName("output");
    // Create ntuples.
    // Ntuples ids are generated automatically starting from 0.
    // The start value can be changed by:
    // analysisManager->SetFirstMtupleId(1);  
   
    // Create 1st ntuple (id = 0)
	  analysisManager->CreateNtuple("Ntuple1", "EventResult"); //TTree = NTuple 
	  analysisManager->CreateNtupleIColumn(0,"EventID"); // column Id = 0
	  analysisManager->CreateNtupleDColumn(0,"ParticleEnergy"); // column Id = 1
		  
	  if( simPar->IsOpticalPhysicsOn() )
	    analysisManager->CreateNtupleIColumn(0,"EmittedScntPhotonNum");
		  
	  analysisManager->CreateNtupleDColumn(0,"EventInitialPosVec",     fEventInitPosVec); // column Id = 4
		  
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

//will be called in EndOfRunAction method of G1RunAction
void NuSDGenericAnalysisManager::Save()
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

void NuSDGenericAnalysisManager::FillNtuple()
{
  NuSDSimParameters *simPar = NuSDSimParameters::GetInstance();
  
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
	analysisManager->FillNtupleIColumn(0, 0, fEventID);
	analysisManager->FillNtupleDColumn(0, 1, fParticleEnergy);
	
	if( simPar->IsOpticalPhysicsOn() )
	  analysisManager->FillNtupleIColumn(0, 2, fEmittedScntPhotonNum);
	
	//after all information are gathered, add row to ntuple
	analysisManager->AddNtupleRow(0); //parameter is ntupleId

} 

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDGenericAnalysisManager::EndOfEvent()
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

void NuSDGenericAnalysisManager::PrintEventResults() const
{
  G4cout<<"\n----------------------Event "<<fEventID<<" Information----------------------------- ";
  G4cout<<"\nGeneric Event";

  G4cout<<"\nParticle Energy(MeV)             :"<<fParticleEnergy; //not ready yet!!!return 0!!
  G4cout<<"\nEmitted Photon number            :"<<fEmittedScntPhotonNum;
 
  G4ThreeVector eip( fEventInitPosVec.at(0), fEventInitPosVec.at(1), fEventInitPosVec.at(2) );
  
  G4cout<<"\nEvent vertex pos(cm)             :"<<eip;
  
  PrintNeutrinoSDOutput(); 
  
#if DETECTOR_TYPE == INHOMOGENEOUSCOMPOSITE
  PrintNeutronSDOutput();
#endif  
      
  PrintPhotonSDOutput();	     
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//---------Reset all values in the begining of the event.---------
void NuSDGenericAnalysisManager::Reset()
{
  fEventID = -1;
  fEmittedScntPhotonNum = 0;
  fParticleEnergy = 0.;
  
  //3d vector
  fEventInitPosVec.clear();
  
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

#ifdef NuSD_DEBUG 	
	G4cout<<"All the parameters are reseted"<<G4endl;
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDGenericAnalysisManager::PrintNeutrinoSDOutput() const
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

void NuSDGenericAnalysisManager::PrintNeutronSDOutput() const
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

void NuSDGenericAnalysisManager::PrintPhotonSDOutput() const
{ 
	G4cout<<G4endl;
	G4cout<<"-------------------PhotonSD Output---------------------------------------"<<G4endl;

  G4cout<<std::left<<std::setw(6+10)<<"CopyNo"<<std::setw(4+10)<<"Time"<<G4endl;		
  for(std::size_t i=0; i<fPhotonDetCopyNoVec.size(); ++i)
  {
    G4cout<<std::left<<std::setw(6+10)<<fPhotonDetCopyNoVec.at(i)<<std::setw(4+10)<<fPhotonDetTimeVec.at(i)<<G4endl;  
  }
	  
	//calculate total number of detected photons in each unit of the detector without considering time
	  
	std::map<int,int> map;
	
	for(std::size_t i =0; i<fPhotonDetCopyNoVec.size(); ++i)
  {
	  
		int copyNo 			= fPhotonDetCopyNoVec.at(i);
		    
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

