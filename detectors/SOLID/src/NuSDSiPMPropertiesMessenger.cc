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

#include "NuSDSiPMPropertiesMessenger.hh"
#include "NuSDSiPMProperties.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSiPMPropertiesMessenger::NuSDSiPMPropertiesMessenger(NuSDSiPMProperties* pmtProperties)
 : G4UImessenger(), fSiPMProperties{pmtProperties}
   
{  
  fPSSizeXCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/mppcSizeX",this);
  fPSSizeXCmd ->SetGuidance("Set the x dimendion of mppc");
  fPSSizeXCmd ->SetParameterName("psSizeX",false);
  fPSSizeXCmd ->SetDefaultUnit("cm");
  fPSSizeXCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fPSSizeXCmd->SetToBeBroadcasted(false);
  
  fPSSizeYCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/mppcSizeY",this);
  fPSSizeYCmd ->SetGuidance("Set the y dimendion of mppc");
  fPSSizeYCmd ->SetParameterName("psSizeY",false);
  fPSSizeYCmd ->SetDefaultUnit("cm");
  fPSSizeYCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fPSSizeYCmd->SetToBeBroadcasted(false);
  
  fPSSizeZCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/mppcSizeZ",this);
  fPSSizeZCmd ->SetGuidance("Set the z dimendion of mppc");
  fPSSizeZCmd ->SetParameterName("psSizeZ",false);
  fPSSizeZCmd ->SetDefaultUnit("cm");
  fPSSizeZCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fPSSizeZCmd->SetToBeBroadcasted(false);
       
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSiPMPropertiesMessenger::~NuSDSiPMPropertiesMessenger()
{ 
  delete fPSSizeXCmd;
  delete fPSSizeYCmd;
  delete fPSSizeZCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSiPMPropertiesMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  
  if (command == fPSSizeXCmd)
  {
    fSiPMProperties->SetPSSizeX(fPSSizeXCmd->GetNewDoubleValue(newValue));  
    
  }else if (command == fPSSizeYCmd)
  {
    fSiPMProperties->SetPSSizeY(fPSSizeYCmd->GetNewDoubleValue(newValue));
    
  }else if (command == fPSSizeZCmd)
  {
    fSiPMProperties->SetPSSizeZ(fPSSizeZCmd->GetNewDoubleValue(newValue));  
  }
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

