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
#include "NuSDNulatMessenger.hh"
#include "NuSDNulatDetConstruction.hh"

#include "G4StateManager.hh"
#include "G4ApplicationState.hh"
#include "G4RunManager.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDNulatMessenger::NuSDNulatMessenger(NuSDNulatDetConstruction* det)
 : G4UImessenger(), fDetConstruction(det)
{
   
  fContainerThicknessCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/containerThickness",this);
  fContainerThicknessCmd->SetGuidance("Set the thickness of scnt container");
  fContainerThicknessCmd->SetParameterName("containerThickness",false);
  fContainerThicknessCmd->SetDefaultUnit("mm");
  fContainerThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  //fContainerThicknessCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDNulatMessenger::~NuSDNulatMessenger()
{
  delete fContainerThicknessCmd;  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNulatMessenger::SetNewValue(G4UIcommand* command, G4String newValue )
{
  
  if( command == fContainerThicknessCmd )
  {
     
    fDetConstruction->SetContainerThickness( fContainerThicknessCmd->GetNewDoubleValue(newValue) );

  } 
  
}

