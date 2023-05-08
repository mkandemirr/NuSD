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

#include "NuSDSiPMProperties.hh"
#include "NuSDSiPMPropertiesMessenger.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSiPMProperties::NuSDSiPMProperties( G4bool   isAttachedBothEnd, 
                                        G4bool   isPSBinderOn,
                                        G4double psBinderLength,
                                        G4double psQE,
                                        G4bool   isQEDependsEnergy,
                                        G4double psSizeX,
                                        G4double psSizeY,
                                        G4double psSizeZ
                                       )
: NuSDPhotoSensorProperties(isAttachedBothEnd, 
  isPSBinderOn,
  psBinderLength,
  psQE,
  isQEDependsEnergy
  ), 
           
fSiPMPropertiesMessenger{nullptr},
fPSSizeX{psSizeX},
fPSSizeY{psSizeY},
fPSSizeZ{psSizeZ}
{
  fSiPMPropertiesMessenger = new NuSDSiPMPropertiesMessenger(this); 
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSiPMProperties::~NuSDSiPMProperties()
{     
  delete fSiPMPropertiesMessenger;
}


G4double NuSDSiPMProperties::GetSiPMReadoutUnitLength() 
{ 
 //----------------SiPMReadOutUnit------------------------
  G4double psBinderLength = fPSBinderLength;

  if(!fIsPSBinderOn) psBinderLength = 0.;
  
  return psBinderLength + fPSLength;
   
}

//---------------UI commands----------------------------------------------

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSiPMProperties::SetPSSizeX(G4double x) 
{ 
  fPSSizeX = x; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSiPMProperties::SetPSSizeY(G4double y) 
{ 
  fPSSizeY = y; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
void NuSDSiPMProperties::SetPSSizeZ(G4double z) 
{ 
  fPSSizeZ = z; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
