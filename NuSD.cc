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

// --------------------------------------------------------------
//                NuSD (Neutrino Segmented Detector)
// --------------------------------------------------------------
//
// Code developed by Mustafa Kandemir
//
//    *******************************
//    *                             *
//    *         NuSD.cc             *
//    *                             *
//    *******************************



#include "NuSD_config.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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

#include "NuSDPhysicsList.hh"
//#include "G4OpticalPhysics.hh"
//#include "FTFP_BERT_HP.hh"
//#include "QGSP_BERT_HP.hh"
//#include "QGSP_BIC_HP.hh"

#include "NuSDActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#include "G4VisExecutive.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() 
  {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " NuSD [-b batch ] [-i interactive] [-t nThreads] [-r seed] "
           << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// argc holds the number of arguments (including the name) on the command line
// -> it is ONE when only the name is  given !!!
// argv[0] is always the name of the program
// argv[1] points to the first argument, and so on

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
  // Evaluate arguments
  if ( argc > 9 ) {
    PrintUsage();
    return 1;
  }

  G4String batchMacro;
  G4String interactiveMacro;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  G4long myseed = 345354;
  for ( G4int i=1; i<argc; i=i+2 ) 
  {
    if      ( G4String(argv[i]) == "-b" ) batchMacro       = argv[i+1];
    else if ( G4String(argv[i]) == "-i" ) interactiveMacro = argv[i+1];
    else if ( G4String(argv[i]) == "-r" ) myseed           = atoi(argv[i+1]);
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) 
    {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
      G4cout<<"Number of threads: "<<nThreads<<G4endl;
    }
#endif
    else 
    {
      PrintUsage();
      return 1;
    }
  }

  // Instantiate G4UIExecutive if interactive mode
  G4UIExecutive* ui = nullptr;
  if ( batchMacro.size() == 0 ) 
  {
    ui = new G4UIExecutive(argc, argv);
  }
  
  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
#ifdef G4MULTITHREADED
  G4cout << "Using the MT Run Manager (G4MULTITHREADED=ON)" << G4endl;  
  G4MTRunManager* runManager = new G4MTRunManager();
  if ( nThreads > 0 ) runManager->SetNumberOfThreads(nThreads);      
#else
  G4RunManager * runManager = new G4RunManager;
  G4cout << "Using the sequential Run Manager" << G4endl;
#endif

  // Seed the random number generator manually
  G4Random::setTheSeed(myseed);
    
#if DETECTOR_NAME == CHANDLER
  runManager->SetUserInitialization(new NuSDChandlerDetConstruction());
#elif DETECTOR_NAME == SWEANY
  runManager->SetUserInitialization(new NuSDSweanyDetConstruction());
#elif DETECTOR_NAME == SOLID
  runManager->SetUserInitialization(new NuSDSolidDetConstruction() );
#elif DETECTOR_NAME == NULAT
  runManager->SetUserInitialization(new NuSDNulatDetConstruction());
#elif DETECTOR_NAME == PANDA
  runManager->SetUserInitialization(new NuSDPandaDetConstruction());
#elif DETECTOR_NAME == PROSPECT
  runManager->SetUserInitialization(new NuSDProspectDetConstruction());
#elif DETECTOR_NAME == HSP
  runManager->SetUserInitialization(new NuSDHspDetConstruction());
#endif  
  
  runManager->SetUserInitialization(new NuSDPhysicsList());
  runManager->SetUserInitialization(new NuSDActionInitialization());
  
  // Initialize visualization
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // get the pointer to the UI manager and set verbosities
  G4UImanager* uImanager = G4UImanager::GetUIpointer();
  uImanager->ApplyCommand("/control/execute macros/common.mac"); 
  
  if ( batchMacro.size() ) // Batch mode
  {   
    G4String command = "/control/execute macros/";
    uImanager->ApplyCommand(command+batchMacro);
    uImanager->ApplyCommand("/control/execute macros/run.mac");
  }
  else if ( interactiveMacro.size() )// Define UI session Macro for interactive mode
  {
    runManager->SetNumberOfThreads(1);
    G4String command = "/control/execute macros/";
    uImanager->ApplyCommand(command+interactiveMacro);
    uImanager->ApplyCommand("/control/execute macros/vis.mac");
    ui->SessionStart();
    delete ui;
  }else // Define UI session Macro for default interactive mode
  {
    uImanager->ApplyCommand("/run/initialize");
    uImanager->ApplyCommand("/control/execute macros/vis.mac");
    
    ui->SessionStart();
    delete ui;   
  }

  delete visManager;
  delete runManager;

  return 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
