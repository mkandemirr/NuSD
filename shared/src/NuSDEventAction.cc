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
#include "NuSDGenericAnalysisManager.hh"
#else
#include "NuSDAnalysisManager.hh"
#endif 

#include "NuSDEventAction.hh"

#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDEventAction::NuSDEventAction()
: G4UserEventAction()

{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDEventAction::~NuSDEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDEventAction::BeginOfEventAction(const G4Event* anEvent )
{
#ifdef GENERIC_PRIMARY_GENERATOR
    NuSDGenericAnalysisManager* analysisManager = NuSDGenericAnalysisManager::GetInstance();
#else
    NuSDAnalysisManager* analysisManager = NuSDAnalysisManager::GetInstance();
#endif 

  analysisManager->SetEventID(anEvent->GetEventID());
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDEventAction::EndOfEventAction (const G4Event*)
{

#ifdef GENERIC_PRIMARY_GENERATOR
    NuSDGenericAnalysisManager* analysisManager = NuSDGenericAnalysisManager::GetInstance();
#else
    NuSDAnalysisManager* analysisManager = NuSDAnalysisManager::GetInstance();
#endif 		
 
  analysisManager->EndOfEvent();
    
}  


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
