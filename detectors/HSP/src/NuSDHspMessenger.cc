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

#include "NuSDHspMessenger.hh"
#include "NuSDHspDetConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDHspMessenger::NuSDHspMessenger(NuSDHspDetConstruction* det)
 : G4UImessenger(), fDetConstruction(det)
{
  
  fNuScntSideLengthCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/nuScntSideLength",this);
  fNuScntSideLengthCmd->SetGuidance("Set the side length of neutrino scintillator");
  fNuScntSideLengthCmd->SetParameterName("NuScntSideLength",false);
  fNuScntSideLengthCmd->SetDefaultUnit("cm");
  fNuScntSideLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  fReflectorThicknessCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/reflectorThickness",this);
  fReflectorThicknessCmd->SetGuidance("Set the thickness of used reflector.");
  fReflectorThicknessCmd->SetParameterName("reflectorThickness",false);
  fReflectorThicknessCmd->SetDefaultUnit("mm");
  fReflectorThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  //fReflectorThicknessCmd->SetToBeBroadcasted(false);
  
  fLiquidTankThicknessCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/liquidTankThickness",this);
  fLiquidTankThicknessCmd->SetGuidance("Set the thickness of liquid tank.");
  fLiquidTankThicknessCmd->SetParameterName("liquidTankThickness",false);
  fLiquidTankThicknessCmd->SetDefaultUnit("mm");
  fLiquidTankThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  //fReflectorThicknessCmd->SetToBeBroadcasted(false);
     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDHspMessenger::~NuSDHspMessenger()
{
  delete fNuScntSideLengthCmd;
  delete fReflectorThicknessCmd;  
  delete fLiquidTankThicknessCmd;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHspMessenger::SetNewValue(G4UIcommand* command , G4String newValue )
{

  if( command == fNuScntSideLengthCmd )
  {
     
    fDetConstruction->SetNuScntSideLength( fNuScntSideLengthCmd->GetNewDoubleValue(newValue) );

  }else if( command == fReflectorThicknessCmd )
  {
     
    fDetConstruction->SetReflectorThickness( fReflectorThicknessCmd->GetNewDoubleValue(newValue) );

  } else if( command == fLiquidTankThicknessCmd )
  {
     
    fDetConstruction->SetLiquidTankThickness( fLiquidTankThicknessCmd->GetNewDoubleValue(newValue) );

  }  
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
