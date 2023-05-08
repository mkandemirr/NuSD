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

#include "NuSDFiberProperties.hh"
#include "NuSDFiberPropertiesMessenger.hh"

#include "G4SystemOfUnits.hh"

#include "G4RunManager.hh"
#include "G4StateManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDFiberProperties::NuSDFiberProperties( G4double coreSizeX,
                                          G4double coreSizeY,
                                          G4double coreSizeZ,
                                          G4double clad1Thickness,
                                          G4double clad2Thickness,
                                          G4double gapThickness,
                                          G4double grooveDistToWall,
                                          G4double distBeweenGrooves
                                        ):
                                                                         
fFiberPropertiesMessenger{nullptr},
fCoreSizeX{coreSizeX}, fCoreSizeY{coreSizeY}, fCoreSizeZ{coreSizeZ},
fClad1Thickness{clad1Thickness}, fClad2Thickness{clad2Thickness},
fGapThickness{gapThickness},
fGrooveDistToWall{grooveDistToWall},
fDistBetweenGrooves{distBeweenGrooves}

{
  fFiberPropertiesMessenger = new NuSDFiberPropertiesMessenger(this);    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDFiberProperties::~NuSDFiberProperties()
{     
  if(fFiberPropertiesMessenger) delete fFiberPropertiesMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NuSDFiberProperties::GetGrooveSizeX() const
{
  return fCoreSizeX+2*fClad1Thickness+ 2*fClad2Thickness+ 2*fGapThickness;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NuSDFiberProperties::GetGrooveSizeY() const
{
  return fCoreSizeY+2*fClad1Thickness+ 2*fClad2Thickness+ 2*fGapThickness;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NuSDFiberProperties::GetGrooveSizeZ() const
{
  return fCoreSizeZ+2*fClad1Thickness+ 2*fClad2Thickness+ 2*fGapThickness;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//---------------------------UI commands--------------------------------------------    
void NuSDFiberProperties::SetCoreSizeX(G4double x) 
{
  fCoreSizeX =  x;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFiberProperties::SetCoreSizeY(G4double y) 
{
  fCoreSizeY =  y;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFiberProperties::SetCoreSizeZ(G4double z) 
{
  fCoreSizeZ =  z;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFiberProperties::SetClad1Thickness(G4double thick) 
{
  fClad1Thickness = thick;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFiberProperties::SetClad2Thickness(G4double thick) 
{
  fClad2Thickness = thick;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFiberProperties::SetGapThickness(G4double thick) 
{
  fGapThickness = thick;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFiberProperties::SetGrooveDistanceToWall(G4double dist) 
{
  fGrooveDistToWall = dist;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFiberProperties::SetDistanceBetweenGrooves(G4double dist) 
{
  fDistBetweenGrooves = dist;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



