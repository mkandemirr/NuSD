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

#include "NuSDFiberPropertiesMessenger.hh"
#include "NuSDFiberProperties.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDFiberPropertiesMessenger::NuSDFiberPropertiesMessenger(NuSDFiberProperties* fiberProperties)
 : G4UImessenger(), fFiberProperties{fiberProperties}
   
{  
  fCoreSizeXCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/fiber/coreSizeX",this);
  fCoreSizeXCmd ->SetGuidance("fiber core size x");
  fCoreSizeXCmd ->SetParameterName("coreSizeX",false);
  fCoreSizeXCmd ->SetDefaultUnit("mm");
  fCoreSizeXCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fCoreSizeXCmd->SetToBeBroadcasted(false);
  
  fCoreSizeYCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/fiber/coreSizeY",this);
  fCoreSizeYCmd ->SetGuidance("fiber core size y");
  fCoreSizeYCmd ->SetParameterName("coreSizeY",false);
  fCoreSizeYCmd ->SetDefaultUnit("mm");
  fCoreSizeYCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fCoreSizeYCmd->SetToBeBroadcasted(false);
  
  fCoreSizeZCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/fiber/coreSizeZ",this);
  fCoreSizeZCmd ->SetGuidance("fiber core size z");
  fCoreSizeZCmd ->SetParameterName("coreSizeZ",false);
  fCoreSizeZCmd ->SetDefaultUnit("mm");
  fCoreSizeZCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fCoreSizeZCmd->SetToBeBroadcasted(false);
  
  fClad1ThicknessCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/fiber/clad1Thickness",this);
  fClad1ThicknessCmd ->SetGuidance("set the clad1 thickness");
  fClad1ThicknessCmd ->SetParameterName("clad1Thickness",false);
  fClad1ThicknessCmd ->SetDefaultUnit("mm");
  fClad1ThicknessCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fClad1ThicknessCmd->SetToBeBroadcasted(false);
  
  fClad2ThicknessCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/fiber/clad2Thickness",this);
  fClad2ThicknessCmd ->SetGuidance("set the clad2 thickness");
  fClad2ThicknessCmd ->SetParameterName("clad2Thickness",false);
  fClad2ThicknessCmd ->SetDefaultUnit("mm");
  fClad2ThicknessCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fClad2ThicknessCmd->SetToBeBroadcasted(false);
  
  fGapThicknessCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/fiber/gapThickness",this);
  fGapThicknessCmd ->SetGuidance("set the distance clad2 to groove");
  fGapThicknessCmd ->SetParameterName("gapThickness",false);
  fGapThicknessCmd ->SetDefaultUnit("mm");
  fGapThicknessCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fGapThicknessCmd->SetToBeBroadcasted(false);
  
  fGrooveDistToWallCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/fiber/grooveDistanceToWall",this);
  fGrooveDistToWallCmd ->SetGuidance("set the distance groove to wall");
  fGrooveDistToWallCmd ->SetParameterName("grooveDistToWall",false);
  fGrooveDistToWallCmd ->SetDefaultUnit("mm");
  fGrooveDistToWallCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fGrooveDistToWallCmd->SetToBeBroadcasted(false);
  
  fDistBetweenGroovesCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/fiber/distanceBetweenGrooves",this);
  fDistBetweenGroovesCmd->SetGuidance("set the distance groove to groove.");
  fDistBetweenGroovesCmd->SetParameterName("distanceBetweenGrooves",false);
  fDistBetweenGroovesCmd->SetDefaultUnit("mm");
  fDistBetweenGroovesCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fDistBetweenGroovesCmd->SetToBeBroadcasted(false);
         
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDFiberPropertiesMessenger::~NuSDFiberPropertiesMessenger()
{ 
  delete fCoreSizeXCmd;
  delete fCoreSizeYCmd;
  delete fCoreSizeZCmd;
  delete fClad1ThicknessCmd;
  delete fClad2ThicknessCmd;
  delete fGapThicknessCmd;
  delete fGrooveDistToWallCmd;
  delete fDistBetweenGroovesCmd;
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFiberPropertiesMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{  
  if (command == fCoreSizeXCmd)
  {
    fFiberProperties->SetCoreSizeX(fCoreSizeXCmd->GetNewDoubleValue(newValue));
    
  } else if (command == fCoreSizeYCmd)
  {
    fFiberProperties->SetCoreSizeY(fCoreSizeYCmd->GetNewDoubleValue(newValue));
    
  } else if (command == fCoreSizeZCmd)
  {
    fFiberProperties->SetCoreSizeZ(fCoreSizeZCmd->GetNewDoubleValue(newValue));
    
  } else if (command == fClad1ThicknessCmd)
  {
    fFiberProperties->SetClad1Thickness(fClad1ThicknessCmd->GetNewDoubleValue(newValue));
    
  } else if (command == fClad2ThicknessCmd)
  {
    fFiberProperties->SetClad2Thickness(fClad2ThicknessCmd->GetNewDoubleValue(newValue));
    
  } else if (command == fGapThicknessCmd)
  {
    fFiberProperties->SetGapThickness(fGapThicknessCmd->GetNewDoubleValue(newValue));
    
  } else if (command == fGrooveDistToWallCmd)
  {
    fFiberProperties->SetGrooveDistanceToWall(fGrooveDistToWallCmd->GetNewDoubleValue(newValue));
    
  } else if (command == fDistBetweenGroovesCmd)
  {
    fFiberProperties->SetDistanceBetweenGrooves(fDistBetweenGroovesCmd->GetNewDoubleValue(newValue));
    
  } 
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

