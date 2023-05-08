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

#include "NuSDReflectorPropertiesMessenger.hh"
#include "NuSDReflectorProperties.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh" 
#include "G4UIcmdWithADouble.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDReflectorPropertiesMessenger::NuSDReflectorPropertiesMessenger(NuSDReflectorProperties* refProp)
 : G4UImessenger(), fReflectorProperties{refProp}
   
{ 
  fReflectorSurfaceDirectory = new G4UIdirectory("/NuSD/geometry/surface/");
  fReflectorSurfaceDirectory->SetGuidance("Optical readout construction control");
  
  fReflectorRefSpectCmd = new G4UIcmdWithABool("/NuSD/geometry/surface/reflector/isReflectivityDependsEnergy",this);
  fReflectorRefSpectCmd->SetGuidance("Decide if reflector reflectivity depends on energy");
  fReflectorRefSpectCmd->SetGuidance("if it is true, fill Reflector_spect.txt file located in /pathToBuild/Data/Reflector/ ");
  fReflectorRefSpectCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fReflectorRefSpectCmd->SetToBeBroadcasted(false);
  
  fReflectorRefValueCmd = new G4UIcmdWithADouble("/NuSD/geometry/surface/reflector/reflectivity",this);
  fReflectorRefValueCmd->SetGuidance("Set the reflectivity of the reflector.");
  fReflectorRefValueCmd->SetParameterName("reflectivity",false);
  fReflectorRefValueCmd->SetRange("reflectivity>=0 && reflectivity<=1");
  fReflectorRefValueCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fReflectorRefValueCmd->SetToBeBroadcasted(false); 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDReflectorPropertiesMessenger::~NuSDReflectorPropertiesMessenger()
{
  delete fReflectorSurfaceDirectory;
  delete fReflectorRefSpectCmd;
  delete fReflectorRefValueCmd;
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDReflectorPropertiesMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
 
  if( command == fReflectorRefSpectCmd )
  {
    
    fReflectorProperties->SetReflectivityDependsEnergy(fReflectorRefSpectCmd->GetNewBoolValue(newValue));
   
  }else if( command == fReflectorRefValueCmd )
  {
    
    fReflectorProperties->SetReflectivity(fReflectorRefValueCmd->GetNewDoubleValue(newValue));
    
  } 
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

