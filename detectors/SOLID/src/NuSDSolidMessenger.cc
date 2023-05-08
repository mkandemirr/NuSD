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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDSolidMessenger.hh"
#include "NuSDSolidDetConstruction.hh"

#include "G4StateManager.hh"
#include "G4ApplicationState.hh"
#include "G4RunManager.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSolidMessenger::NuSDSolidMessenger(NuSDSolidDetConstruction* det)
 : G4UImessenger(), fDetConstruction(det)
{
   
  fReflectorThicknessCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/reflectorThickness",this);
  fReflectorThicknessCmd->SetGuidance("Set the thickness of used reflector.");
  fReflectorThicknessCmd->SetParameterName("reflectorThickness",false);
  fReflectorThicknessCmd->SetDefaultUnit("mm");
  fReflectorThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  //fReflectorThicknessCmd->SetToBeBroadcasted(false);
  
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSolidMessenger::~NuSDSolidMessenger()
{
  delete fReflectorThicknessCmd;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSolidMessenger::SetNewValue(G4UIcommand* command, G4String newValue )
{
  
  if( command == fReflectorThicknessCmd )
  {
     
    fDetConstruction->SetReflectorThickness( fReflectorThicknessCmd->GetNewDoubleValue(newValue) );

  } 
  
}

