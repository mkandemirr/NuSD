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

#include "NuSDPhotonSD.hh"

#ifdef GENERIC_PRIMARY_GENERATOR
#include "NuSDGenericAnalysisManager.hh"
#else
#include "NuSDAnalysisManager.hh"
#endif 

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh" 
#include "G4ParticleTypes.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhotonSD::NuSDPhotonSD( const G4String& name)
 : G4VSensitiveDetector(name),
   fHitsCollection{nullptr}
{
  collectionName.insert("PhotonSDHitCol"); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhotonSD::~NuSDPhotonSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPhotonSD::Initialize(G4HCofThisEvent* hce)
{
	// Create hits collection
  fHitsCollection = new NuSDPhotonSDHitsCollection(SensitiveDetectorName, collectionName[0]); 

  if(fHCID<0) 
  { 
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }

  hce->AddHitsCollection( fHCID, fHitsCollection ); 
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NuSDPhotonSD::ProcessHits(G4Step*, G4TouchableHistory*)
{  
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NuSDPhotonSD::ProcessHits_constStep(const G4Step* step, G4TouchableHistory* )
{
  
  if (step == nullptr) return false;
  //Generates a hit and uses the postStepPoint; PostStepPoint because the hit
  //is generated manually when the photon hits the detector     
           
  //need to know if this is an optical photon
  if( step->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition() ) 
  	return false;   

  G4StepPoint *thePostStepPoint = step->GetPostStepPoint();
  G4int copyNo  = thePostStepPoint->GetTouchableHandle()->GetCopyNumber(0);  //pmt(0)--world(1)
  
  NuSDPhotonSDHit *hit  = new NuSDPhotonSDHit();
  hit->SetCopyNo(copyNo);
  hit->SetArrivalTime(thePostStepPoint->GetGlobalTime());
  
  
	fHitsCollection->insert(hit);
  
  return true;
                                 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPhotonSD::EndOfEvent(G4HCofThisEvent*)
{
  //PrintAll();
#ifdef GENERIC_PRIMARY_GENERATOR
  NuSDGenericAnalysisManager* analysisManager = NuSDGenericAnalysisManager::GetInstance();
#else
  NuSDAnalysisManager* analysisManager = NuSDAnalysisManager::GetInstance();
#endif 
  
  for ( std::size_t i=0; i<fHitsCollection->entries(); ++i )
  {
		auto hit = (*fHitsCollection)[i];
				
		analysisManager->GetPhotonDetCopyNoVec().push_back(hit->GetCopyNo());
		analysisManager->GetPhotonDetTimeVec().push_back(hit->GetArrivalTime());
    
  }
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPhotonSD::PrintAll()
{
  G4cout<<"-------------------PhotonSD Output---------------------------------------"<<G4endl;
  G4cout<<std::left<<std::setw(6+10)<<"CopyNo"<<std::setw(19)<<"Photon arrival time"<<G4endl;

  //G4cout<<std::setprecision(5)<<G4endl;
  
  for ( std::size_t i=0; i<fHitsCollection->entries(); ++i )
  {
    auto hit = (*fHitsCollection)[i];   
    G4cout<<std::left<<std::setw(6+10)<<hit->GetCopyNo()<<std::setw(19)<<hit->GetArrivalTime()<<G4endl;
    
  }

    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
