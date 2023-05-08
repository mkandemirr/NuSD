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

#include "NuSDRunAction.hh"

#if DETECTOR_NAME == CHANDLER
#include "NuSDChandlerDetConstruction.hh"
#elif DETECTOR_NAME == SWEANY
#include "NuSDSweanyDetConstruction.hh"
#elif DETECTOR_NAME == SOLID
#include "NuSDSolidDetConstruction.hh"
#elif DETECTOR_NAME == NULAT
#include "NuSDNulatDetConstruction.hh"
#elif DETECTOR_NAME == PANDA
#include "NuSDPandaDetConstruction.hh"
#elif DETECTOR_NAME == PROSPECT
#include "NuSDProspectDetConstruction.hh"
#elif DETECTOR_NAME == HSP
#include "NuSDHspDetConstruction.hh"
#endif 

#ifdef GENERIC_PRIMARY_GENERATOR
#include "NuSDGenericAnalysisManager.hh"
#else
#include "NuSDAnalysisManager.hh"
#endif

#include "G4RunManager.hh"
#include "G4Run.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo................oooOO0OOooo........oooOO0OOooo......

NuSDRunAction::NuSDRunAction()
 : G4UserRunAction()
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  //first call. 
#ifdef GENERIC_PRIMARY_GENERATOR
  NuSDGenericAnalysisManager::GetInstance();
#else
  NuSDAnalysisManager::GetInstance();
#endif   
       
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDRunAction::~NuSDRunAction()
{   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDRunAction::BeginOfRunAction(const G4Run* aRun )
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
#ifdef CREATE_ROOT_FILE

#ifdef GENERIC_PRIMARY_GENERATOR
  NuSDGenericAnalysisManager* analysisManager = NuSDGenericAnalysisManager::GetInstance();
#else
  NuSDAnalysisManager* analysisManager = NuSDAnalysisManager::GetInstance();
#endif   

  analysisManager->Book();
#endif
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDRunAction::EndOfRunAction(const G4Run* run)
{

#ifdef CREATE_ROOT_FILE
#ifdef GENERIC_PRIMARY_GENERATOR
  NuSDGenericAnalysisManager* analysisManager = NuSDGenericAnalysisManager::GetInstance();
#else
  NuSDAnalysisManager* analysisManager = NuSDAnalysisManager::GetInstance();
#endif   
  analysisManager->Save();
#endif

  if(!IsMaster())
    G4cout<< "EndOfRunAction Started" <<G4endl;

  G4int nofEvents = run->GetNumberOfEvent();

  if (nofEvents == 0) return;
  
  if ( IsMaster() ) 
  {
    G4cout<< "\n--------------------End of Global Runnn-----------------------"<<G4endl;

    const G4VUserDetectorConstruction* vdc = G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  
#if DETECTOR_NAME == CHANDLER
    const NuSDChandlerDetConstruction* dc  = static_cast<const NuSDChandlerDetConstruction*>(vdc);
#elif DETECTOR_NAME == SWEANY
    const NuSDSweanyDetConstruction* dc    = static_cast<const NuSDSweanyDetConstruction*>(vdc);
#elif DETECTOR_NAME == SOLID
    const NuSDSolidDetConstruction* dc     = static_cast<const NuSDSolidDetConstruction*>(vdc);
#elif DETECTOR_NAME == NULAT
    const NuSDNulatDetConstruction* dc     = static_cast<const NuSDNulatDetConstruction*>(vdc);
#elif DETECTOR_NAME == PANDA
    const NuSDPandaDetConstruction* dc     = static_cast<const NuSDPandaDetConstruction*>(vdc);
#elif DETECTOR_NAME == PROSPECT
    const NuSDProspectDetConstruction* dc  = static_cast<const NuSDProspectDetConstruction*>(vdc);
#elif DETECTOR_NAME == HSP
    const NuSDHspDetConstruction* dc       = static_cast<const NuSDHspDetConstruction*>(vdc);
#endif  

    dc->Print();

  }
  else 
  {
    G4cout<< "\n--------------------End of Local Run------------------------"<< G4endl;
  }

  G4cout<< "\n The run consists of " << nofEvents << " event"<< G4endl;
                
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
