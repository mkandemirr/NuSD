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

#include "NuSDHexPmtProperties.hh"
#include "NuSDHexPmtPropertiesMessenger.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDHexPmtProperties::NuSDHexPmtProperties( G4bool   isAttachedBothEnd, 
                                            G4bool   isPSBinderOn,
                                            G4double psBinderLength,
                                            G4double psQE,
                                            G4bool   isQEDependsEnergy,
                                            G4double psTanDisToOuterSur,
                                            G4bool   isLgOn,
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
           
fHexPmtPropertiesMessenger{nullptr},
fPSTanDisToOuterSur{psTanDisToOuterSur},
fIsLgOn{isLgOn},
fLgLength{lgLength},
fIsLgBinderOn{isLgBinderOn},
fLgBinderLength{lgBinderLength}
{
  fHexPmtPropertiesMessenger = new NuSDHexPmtPropertiesMessenger(this); 
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDHexPmtProperties::~NuSDHexPmtProperties()
{     
  delete fHexPmtPropertiesMessenger;
}


G4double NuSDHexPmtProperties::GetHexPmtReadoutUnitLength() 
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



//---------------UI commands----------------------------------------------
void NuSDHexPmtProperties::SetPSTanDisToOuterSur(G4double psTanDisToOuterSur) 
{ 
  fPSTanDisToOuterSur = psTanDisToOuterSur;
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
void NuSDHexPmtProperties::SetLightGuide(G4bool bl )    
{ 
  fIsLgOn = bl; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
        
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
void NuSDHexPmtProperties::SetLgLength(G4double length) 
{ 
  fLgLength = length;
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
void NuSDHexPmtProperties::SetLgBinder(G4bool bl ) 
{ 
  fIsLgBinderOn = bl; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHexPmtProperties::SetLgBinderLength(G4double length) 
{
  fLgBinderLength = length; 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
