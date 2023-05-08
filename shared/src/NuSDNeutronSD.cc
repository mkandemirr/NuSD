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

#if DETECTOR_TYPE == INHOMOGENEOUSCOMPOSITE

#include "NuSDNeutronSD.hh"

#ifdef GENERIC_PRIMARY_GENERATOR
#include "NuSDGenericAnalysisManager.hh"
#else
#include "NuSDAnalysisManager.hh"
#endif  

#include "G4SystemOfUnits.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh" 
#include "G4ParticleTypes.hh"  
#include "G4ProcessType.hh"
#include "G4HadronicProcess.hh"
#include "G4HadronInelasticProcess.hh"
#include "G4HadronicProcessType.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDNeutronSD::NuSDNeutronSD(const G4String& name, const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr)      
{
  collectionName.insert(hitsCollectionName);
     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDNeutronSD::~NuSDNeutronSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNeutronSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection = new NuSDNeutronSDHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NuSDNeutronSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{  
  G4Track * theTrack = step->GetTrack();

  if( theTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition() )
    return false;

  G4StepPoint *thePreStepPoint = step->GetPreStepPoint();
 	G4int histoDepth = thePreStepPoint->GetTouchable()->GetHistoryDepth();
 	       
 	G4int copyNo{};
  for(G4int depthNo=0;depthNo<=histoDepth;++depthNo)
  {
    //G4cout<<"PhysVol: "<<thePreStepPoint->GetTouchable()->GetVolume(depthNo)->GetName()<<G4endl;
    if(thePreStepPoint->GetTouchable()->GetVolume(depthNo)->GetName()=="UnitPV")
    {
      copyNo = thePreStepPoint->GetTouchable()->GetVolume(depthNo)->GetCopyNo();
      break;
    }      
  }
  
#if DETECTOR_NAME==CHANDLER //In chandler, NuSDNeutronSD is not inside unit. 
  copyNo = thePreStepPoint->GetTouchable()->GetVolume(0)->GetCopyNo();
#endif

  NuSDNeutronSDHit *hit  = new NuSDNeutronSDHit();
  hit->SetCopyNo(copyNo);
  hit->SetParticlePDG(theTrack->GetParticleDefinition()->GetPDGEncoding());
  hit->SetTrackID(theTrack->GetTrackID());
  hit->SetEdep(step->GetTotalEnergyDeposit()*(1./MeV));
  hit->SetTime(step->GetPreStepPoint()->GetGlobalTime()*(1./microsecond));
 
  fHitsCollection->insert(hit);
  
   
#ifndef GENERIC_PRIMARY_GENERATOR
  
  if( theTrack->GetParentID() == 0  && theTrack->GetDefinition() == G4Neutron::NeutronDefinition() )
  {

    const G4VProcess *vp = step->GetPostStepPoint()->GetProcessDefinedStep(); //burası cons olarak döndürüyor.
    
    if( vp->GetProcessSubType()==fHadronInelastic )
    {
      //G4cout<<"PROCESS NAME(NeutronSD): "<<vp->GetProcessName()<<"  Type: "<<vp->GetProcessSubType()<<G4endl;
      const G4HadronInelasticProcess *theNeutronInelasticProcess = dynamic_cast< const G4HadronInelasticProcess*>(vp);
      const std::vector<const G4Track*> *secondary = step->GetSecondaryInCurrentStep();
      
      if(theNeutronInelasticProcess==nullptr) 
      { G4Exception("NuSDNeutronSD::ProcessHits()", "Err0", FatalException, "Casting err !!"); }

      if(  (*secondary).size() == 2 && (*secondary)[0]->GetDefinition() == G4Triton::TritonDefinition() 
           && (*secondary)[1]->GetDefinition() == G4Alpha::AlphaDefinition() )   
      {

        G4int massNumber      = theNeutronInelasticProcess->GetTargetNucleus()->GetA_asInt(); //which nucleus absorb neutron
        G4double nct          = theTrack->GetGlobalTime(); 
        //G4double trackLength  = theTrack->GetTrackLength();
        
        NuSDAnalysisManager* analysisManager = NuSDAnalysisManager::GetInstance();
        
	      analysisManager->SetNeutronAbsNucleusA( massNumber );
	      analysisManager->SetNeutronCaptureTime( nct*(1/microsecond) ); 
	      analysisManager->GetNeutronCapturePosVec().push_back(theTrack->GetPosition().x());
        analysisManager->GetNeutronCapturePosVec().push_back(theTrack->GetPosition().y());
        analysisManager->GetNeutronCapturePosVec().push_back(theTrack->GetPosition().z());


      }

    }       



  }
   
#endif  

  return true;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNeutronSD::EndOfEvent(G4HCofThisEvent*)
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
    
    analysisManager->GetNdParticlePDGVec().push_back( hit->GetParticlePDG() );
    analysisManager->GetNdTrackIDVec().push_back( hit->GetTrackID() );
    analysisManager->GetNdCopyNoVec().push_back( hit->GetCopyNo() );
    analysisManager->GetNdEdepVec().push_back( hit->GetEdep() );
    analysisManager->GetNdTimeVec().push_back( hit->GetTime() );

  }
         
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNeutronSD::PrintAll()
{

  G4cout<<"-------------------NeutronSD Output---------------------------------------"<<G4endl;
  G4cout<<std::left<<std::setw(6+8)<<"CopyNo"<<std::setw(11+8)<<"ParticlePDG"<<std::setw(10+8)<<"TrackID"
        <<std::setw(11+8)<<"Energy(MeV)"<<std::setw(8+8)<<"Time(us)"<<G4endl;

  //G4cout<<std::setprecision(5)<<G4endl;
  for ( std::size_t i=0; i<fHitsCollection->entries(); ++i )
  {
    auto hit = (*fHitsCollection)[i];
    
    G4cout<<std::left<<std::setw(6+8)<<hit->GetCopyNo()<<std::setw(11+8)<<hit->GetParticlePDG()<<std::setw(10+8)<<hit->GetTrackID()
          <<std::setw(11+8)<<hit->GetEdep()<<std::setw(8+8)<<hit->GetTime()<<G4endl;
    
  }
 
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

