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

#include "NuSDPhotoSensorProperties.hh"
#include "NuSDPhotoSensorPropertiesMessenger.hh"

#include "G4RunManager.hh"
#include "G4StateManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhotoSensorProperties::NuSDPhotoSensorProperties(G4bool isAttachedBothEnd, 
                                                     G4bool isPSBinderOn,
                                                     G4double psBinderLength,
                                                     G4double psQE,
                                                     G4bool isQEDependsEnergy 
                                                     ):
fPSPropertiesMessenger{nullptr},
fIsAttachedBothEnd{isAttachedBothEnd},
fIsPSBinderOn{isPSBinderOn},
fPSBinderLength{psBinderLength},
fQE{psQE},
fIsQEDependsEnergy{isQEDependsEnergy},
fPSLength{4*cm}
{
   fPSPropertiesMessenger = new NuSDPhotoSensorPropertiesMessenger(this);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhotoSensorProperties::~NuSDPhotoSensorProperties()
{
  if(fPSPropertiesMessenger)
    delete fPSPropertiesMessenger;     
}

G4double NuSDPhotoSensorProperties::GetPSBinderLength() const
{
  return (fIsPSBinderOn) ? fPSBinderLength : 0.;  
}

//---------------------------UI commands--------------------------------------------

void NuSDPhotoSensorProperties::SetBothEnd(G4bool bl) 
{ 
  fIsAttachedBothEnd = bl; 
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
  {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
    
}


void NuSDPhotoSensorProperties::SetPSBinder(G4bool bl ) 
{ 
  fIsPSBinderOn = bl; 
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
  {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
}

void NuSDPhotoSensorProperties::SetPSBinderLength(G4double length) 
{
  fPSBinderLength = length; 
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
  {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
} 


void NuSDPhotoSensorProperties::SetPSQE(G4double eff) 
{ 
  fQE = eff; 
 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  
  
} 
    
void NuSDPhotoSensorProperties::SetQEDependsEnergy(G4bool b) 
{
  fIsQEDependsEnergy = b;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();

    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
