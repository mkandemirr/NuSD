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

#include "G4SystemOfUnits.hh"

#include "NuSDSteppingAction.hh"
#include "NuSDPhotonSD.hh"

#include "G4NeutronCaptureProcess.hh"
#include "G4Step.hh"
#include "G4LogicalVolume.hh"
#include "G4VProcess.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4ParticleDefinition.hh" 
#include "G4ParticleTypes.hh"  //G4Gamma için
#include "G4SDManager.hh"
#include "G4OpBoundaryProcess.hh"
#include  "G4ProcessManager.hh"
#ifndef GENERIC_PRIMARY_GENERATOR
#include "NuSDAnalysisManager.hh"
#endif


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSteppingAction::NuSDSteppingAction()
: G4UserSteppingAction(), fOpProcess{nullptr} 
  
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSteppingAction::~NuSDSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSteppingAction::UserSteppingAction(const G4Step* step)
{

  G4LogicalVolumeStore *lvs{ G4LogicalVolumeStore::GetInstance() };
  G4LogicalVolume *lv{ lvs->GetVolume("NuScntLogic") };

  G4Track *theTrack             = step->GetTrack();
  G4StepPoint *thePostStepPoint = step->GetPostStepPoint(); 
  
  
  if( theTrack->GetParentID() == 0 && theTrack->GetLogicalVolumeAtVertex() != lv )
    { G4Exception("NuSDSteppingAction::UserSteppingAction()", "stepActionErr0", FatalException, 
    "Initial particles should be generated inside desired volume!!!"); }

#ifndef GENERIC_PRIMARY_GENERATOR    
  
  //for neutron captured  in Gd layer
#if DETECTOR_NAME == PANDA

    if( theTrack->GetParentID() == 0 &&  theTrack->GetDefinition() == G4Neutron::NeutronDefinition()  ) //primary neutron
    {
      if( step->GetPreStepPoint()->GetPhysicalVolume()->GetName() =="GdPV")
      {
        const G4VProcess *vp = thePostStepPoint->GetProcessDefinedStep(); 
        
        if( vp->GetProcessName() == "nCapture"  )  //should be work only once for each event
        {
        
          const G4NeutronCaptureProcess *neutronCaptureProcess = dynamic_cast< const G4NeutronCaptureProcess*>(vp);
          
          if(neutronCaptureProcess==nullptr) 
            { G4Exception("NuSDNeutrinoSD::ProcessHits()", "Err0", FatalException, "Casting err !!"); }
          
          G4int massNo      		= neutronCaptureProcess->GetTargetNucleus()->GetA_asInt(); //which nucleus absorb neutron
          G4double nct          = theTrack->GetGlobalTime(); //Neutron capture time.
            
          NuSDAnalysisManager* analysisManager = NuSDAnalysisManager::GetInstance();  
                   
          analysisManager->SetNeutronAbsNucleusA( massNo );
          analysisManager->SetNeutronCaptureTime( nct*(1/microsecond) ); 
          analysisManager->GetNeutronCapturePosVec().push_back(theTrack->GetPosition().x());
          analysisManager->GetNeutronCapturePosVec().push_back(theTrack->GetPosition().y());
          analysisManager->GetNeutronCapturePosVec().push_back(theTrack->GetPosition().z());
         
        }       
      
      }

    }

#endif
  
#endif
  //--------------------------------------OPTIC PHOTONS----------------------------------------------------------------//

  if(theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ) //particle is photon
  {
    
    // Retrieve the status of the photon
    G4OpBoundaryProcessStatus theStatus = Undefined;

    G4ProcessManager* opManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

    if (opManager) 
    {
      G4int maxOfPostStepLoops = opManager->GetPostStepProcessVector()->entries();
      G4ProcessVector* fPostStepDoItVector  = opManager->GetPostStepProcessVector(typeDoIt);

      for ( G4int i=0; i<maxOfPostStepLoops; i++) 
      {
        G4VProcess* currentProcess = (*fPostStepDoItVector)[i];
        fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(currentProcess);
        if (fOpProcess) { theStatus = fOpProcess->GetStatus(); break;}
      }
    }
    
      // if ( thePostStepPoint->GetPhysicalVolume()->GetName() == "PhotonSDPV" ) 
      //{ G4cout<<"SOLIDDDDDDDDddd"<<G4endl; }
    if(theStatus == Detection)
    {
        
      if ( thePostStepPoint->GetPhysicalVolume()->GetName() == "PhotonSDPV" ) 
      {  
        //Triger sensitive detector manually since photon is
        //absorbed but status was Detection
        G4SDManager* sdMan     = G4SDManager::GetSDMpointer();
        NuSDPhotonSD* photonSD = dynamic_cast<NuSDPhotonSD*>( sdMan->FindSensitiveDetector("PhotonSD") );

        if(photonSD)
          photonSD->ProcessHits_constStep(step, nullptr);
        else
          G4Exception("NuSDSteppingAction::UserSteppingAction()", "stepActionErr2", FatalException, "PhotonSD returns NULL!");

        // Stop Tracking when it hits the detector's surface
        theTrack->SetTrackStatus(fStopAndKill);
        return;
      }
       
    } //Detection check

  }//optPhoton check   



}//stepping action



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

