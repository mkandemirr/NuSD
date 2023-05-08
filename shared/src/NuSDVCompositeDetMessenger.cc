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

#if DETECTOR_TYPE == INHOMOGENEOUSCOMPOSITE

#include "NuSDVCompositeDetMessenger.hh"
#include "NuSDVCompositeDetConstruction.hh"

#include "G4StateManager.hh"
#include "G4ApplicationState.hh"
#include "G4RunManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh" 
#include "G4UIcmdWithADouble.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVCompositeDetMessenger::NuSDVCompositeDetMessenger(NuSDVCompositeDetConstruction* det)
 : G4UImessenger(), fDetConstruction(det)
{
 
  fDetConstructionDirectory = new G4UIdirectory("/NuSD/geometry/");
  fDetConstructionDirectory->SetGuidance("Detector construction control");
      
  fNeutronScntThicknessCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/neutronScntThickness",this);
  fNeutronScntThicknessCmd->SetGuidance("Set the thickness of neutron scintillator.");
  fNeutronScntThicknessCmd->SetParameterName("neutronDetThick",false);
  fNeutronScntThicknessCmd->SetDefaultUnit("mm");
  fNeutronScntThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  //fNeutronScntThicknessCmd->SetToBeBroadcasted(false);
       
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVCompositeDetMessenger::~NuSDVCompositeDetMessenger()
{
  //delete fDetConstructionDirectory;
  delete fNeutronScntThicknessCmd;
  
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVCompositeDetMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  
  if( command == fNeutronScntThicknessCmd )
  {
    G4double thickness = fNeutronScntThicknessCmd->GetNewDoubleValue(newValue); 
    fDetConstruction->SetNeutronScntThickness(thickness);

  } 
    
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
