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

//these detectors do not use pmt
#if DETECTOR_NAME != SOLID && DETECTOR_NAME != HSP 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDPmtProperties.hh"
#include "NuSDPmtPropertiesMessenger.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPmtProperties::NuSDPmtProperties( G4bool   isAttachedBothEnd, 
                                      G4bool   isPSBinderOn,
                                      G4double psBinderLength,
                                      G4double psQE,
                                      G4bool   isQEDependsEnergy,
                                      G4double psRadius,
                                      G4bool   isLgOn,
                                      G4double lgPmtEndSizeX,
                                      G4double lgPmtEndSizeY,
                                      G4double lgPmtEndSizeZ,
                                      G4double lgLength,
                                      G4bool   isLgBinderOn,
                                      G4double lgBinderLength
                                     )
: NuSDPhotoSensorProperties(isAttachedBothEnd, 
  isPSBinderOn,
  psBinderLength,
  psQE,
  isQEDependsEnergy
  ), 
           
fPmtPropertiesMessenger{nullptr},
fPSRadius{psRadius},
fIsLgOn{isLgOn},
fLgPmtEndSizeX{lgPmtEndSizeX},
fLgPmtEndSizeY{lgPmtEndSizeY},
fLgPmtEndSizeZ{lgPmtEndSizeZ},
fLgLength{lgLength},
fIsLgBinderOn{isLgBinderOn},
fLgBinderLength{lgBinderLength}
{
  fPmtPropertiesMessenger = new NuSDPmtPropertiesMessenger(this); 
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPmtProperties::~NuSDPmtProperties()
{     
  delete fPmtPropertiesMessenger;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NuSDPmtProperties::GetLgLength() const
{
  return (fIsLgOn) ? fLgLength : 0.; 
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NuSDPmtProperties::GetLgBinderLength() const
{
  return (fIsLgBinderOn) ? fLgBinderLength : 0.;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NuSDPmtProperties::GetPmtReadoutUnitLength() 
{ 
 //----------------PmtReadOutUnit------------------------
  G4double lgLength                 = fLgLength;
  G4double lgBinderLength           = fLgBinderLength;
  G4double psBinderLength           = fPSBinderLength;

  if(!fIsLgOn)       lgLength       = 0;
  if(!fIsLgBinderOn) lgBinderLength = 0.;
  if(!fIsPSBinderOn) psBinderLength = 0.;

  return lgLength + lgBinderLength + psBinderLength + fPSLength;
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//---------------UI commands----------------------------------------------
void NuSDPmtProperties::SetPSRadius(G4double pcr) 
{ 
  fPSRadius = pcr; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
void NuSDPmtProperties::SetLightGuide(G4bool bl )    
{ 
  fIsLgOn = bl; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
        
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPmtProperties::SetLgPmtEndSizeX(G4double x) 
{ 
  fLgPmtEndSizeX = x;
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPmtProperties::SetLgPmtEndSizeY(G4double y) 
{ 
  fLgPmtEndSizeY = y;
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPmtProperties::SetLgPmtEndSizeZ(G4double z) 
{ 
  fLgPmtEndSizeZ = z;
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
void NuSDPmtProperties::SetLgLength(G4double length) 
{ 
  fLgLength = length;
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
void NuSDPmtProperties::SetLgBinder(G4bool bl ) 
{ 
  fIsLgBinderOn = bl; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPmtProperties::SetLgBinderLength(G4double length) 
{
  fLgBinderLength = length; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
