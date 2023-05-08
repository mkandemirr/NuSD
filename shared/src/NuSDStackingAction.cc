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
#include "NuSDStackingAction.hh"

#ifdef GENERIC_PRIMARY_GENERATOR
#include "NuSDGenericAnalysisManager.hh"
#else
#include "NuSDAnalysisManager.hh"
#endif

#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDStackingAction::NuSDStackingAction()
  : G4UserStackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDStackingAction::~NuSDStackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack NuSDStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{

  if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
  {
  	
    if(aTrack->GetCreatorProcess()->GetProcessName()=="Scintillation")
    {
    
#ifdef GENERIC_PRIMARY_GENERATOR
      NuSDGenericAnalysisManager* analysisManager = NuSDGenericAnalysisManager::GetInstance();
#else
      NuSDAnalysisManager* analysisManager = NuSDAnalysisManager::GetInstance();
#endif 
      analysisManager->IncEmittedPhotonNum();
      
    }else if( aTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov" )
    {
      return fKill;
    }

  }

  return fUrgent;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDStackingAction::NewStage()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//invoked at the begining of each event..
void NuSDStackingAction::PrepareNewEvent()
{
}


