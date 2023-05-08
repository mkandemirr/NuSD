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

#include "NuSDVDetMessenger.hh"
#include "NuSDVDetConstruction.hh"

#include "G4StateManager.hh"
#include "G4ApplicationState.hh"
#include "G4RunManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVDetMessenger::NuSDVDetMessenger(NuSDVDetConstruction* det)
 : G4UImessenger(), fDetConstruction(det)
{
 
  fDetConstructionDirectory = new G4UIdirectory("/NuSD/geometry/");
  fDetConstructionDirectory->SetGuidance("Detector construction control");
    
  fNumberOfSegmentAlongXCmd = new G4UIcmdWithAnInteger("/NuSD/geometry/numberOfSegmentAlongX", this);
  fNumberOfSegmentAlongXCmd->SetGuidance("Number of segment along x" "\n  0 or negative values mean <<Don't change it!>>");
  fNumberOfSegmentAlongXCmd->SetParameterName("numberOfSegmentAlongX", false);
  fNumberOfSegmentAlongXCmd->SetDefaultValue(3);
  fNumberOfSegmentAlongXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNumberOfSegmentAlongXCmd->SetToBeBroadcasted(false);
  
  fNumberOfSegmentAlongYCmd = new G4UIcmdWithAnInteger("/NuSD/geometry/numberOfSegmentAlongY", this);
  fNumberOfSegmentAlongYCmd->SetGuidance("Number of segment along y" "\n  0 or negative values mean <<Don't change it!>>");
  fNumberOfSegmentAlongYCmd->SetParameterName("numberOfSegmentAlongY", false);
  fNumberOfSegmentAlongYCmd->SetDefaultValue(3);
  fNumberOfSegmentAlongYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNumberOfSegmentAlongYCmd->SetToBeBroadcasted(false);
  
  fNumberOfSegmentAlongZCmd = new G4UIcmdWithAnInteger("/NuSD/geometry/numberOfSegmentAlongZ", this);
  fNumberOfSegmentAlongZCmd->SetGuidance("Number of segment along z" "\n  0 or negative values mean <<Don't change it!>>");
  fNumberOfSegmentAlongZCmd->SetParameterName("numberOfSegmentAlongZ", false);
  fNumberOfSegmentAlongZCmd->SetDefaultValue(3);
  fNumberOfSegmentAlongZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNumberOfSegmentAlongZCmd->SetToBeBroadcasted(false);
  
  
  fNuScntDimensionCmd = new G4UIcmdWith3VectorAndUnit("/NuSD/geometry/neutrinoScntDimensions",this);
  fNuScntDimensionCmd->SetGuidance("Set the dimensions of nuScnt vol");
  fNuScntDimensionCmd->SetParameterName("nuScntSizeX","nuScntSizeY","nuScntSizeZ",false);
  fNuScntDimensionCmd->SetDefaultUnit("cm");
  fNuScntDimensionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fNuScntDimensionCmd->SetToBeBroadcasted(false);

  fOptBarrierThicknessCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/optBarrierThickness",this);
  fOptBarrierThicknessCmd->SetGuidance("Set the thickness of ROL optBarrier.");
  fOptBarrierThicknessCmd->SetParameterName("optBarrierThick",false);
  fOptBarrierThicknessCmd->SetDefaultUnit("mm");
  fOptBarrierThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fOptBarrierThicknessCmd->SetToBeBroadcasted(false);
  
  fOptReadoutCmd = new G4UIcmdWithABool("/NuSD/geometry/isOpticalReadoutOn",this);
  fOptReadoutCmd->SetGuidance("On/Off the opticalReadOut system");
  fOptReadoutCmd->SetParameterName("OpticalSystemFlag",false);
  fOptReadoutCmd->AvailableForStates(G4State_PreInit,G4State_Init, G4State_Idle);
  //fOptReadoutCmd->SetDefaultValue(true); 
  fOptReadoutCmd->SetToBeBroadcasted(false);
  
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVDetMessenger::~NuSDVDetMessenger()
{
  delete fDetConstructionDirectory;
  
  delete fNumberOfSegmentAlongXCmd;
  delete fNumberOfSegmentAlongYCmd;
  delete fNumberOfSegmentAlongZCmd;  
  
  delete fNuScntDimensionCmd;
  delete fOptBarrierThicknessCmd;
  
  delete fOptReadoutCmd;
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  
  if( command == fNumberOfSegmentAlongXCmd )
  {
    G4int numOfSegmentAlongX = fNumberOfSegmentAlongXCmd->GetNewIntValue(newValue); 
    fDetConstruction->SetNumberOfSegmentAlongX(numOfSegmentAlongX);
    
  }else if( command == fNumberOfSegmentAlongYCmd )  
  {
    G4int numOfSegmentAlongY = fNumberOfSegmentAlongYCmd->GetNewIntValue(newValue); 
    fDetConstruction->SetNumberOfSegmentAlongY(numOfSegmentAlongY);

  } else if( command == fNumberOfSegmentAlongZCmd )
  {
    G4int numOfSegmentAlongZ = fNumberOfSegmentAlongZCmd->GetNewIntValue(newValue); 
    fDetConstruction->SetNumberOfSegmentAlongZ(numOfSegmentAlongZ);

  } else if( command == fNuScntDimensionCmd )
  {
    fDetConstruction->SetNuScntDimensions(fNuScntDimensionCmd->GetNew3VectorValue(newValue));
  } else if( command == fOptBarrierThicknessCmd )
  {
    G4double thickness = fOptBarrierThicknessCmd->GetNewDoubleValue(newValue); 
    fDetConstruction->SetOptBarrierThickness(thickness);

  }else if( command == fOptReadoutCmd )
  {
    fDetConstruction->SetOnFlag(fOptReadoutCmd->GetNewBoolValue(newValue));
       
  }
    
  
}




