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

#include "NuSDHexPmtPropertiesMessenger.hh"
#include "NuSDHexPmtProperties.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDHexPmtPropertiesMessenger::NuSDHexPmtPropertiesMessenger(NuSDHexPmtProperties* pmtProperties)
 : G4UImessenger(), fHexPmtProperties{pmtProperties}
   
{ 
  fPSTanDisToOuterSurCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/psTanDisToOuterSur",this);
  fPSTanDisToOuterSurCmd ->SetGuidance("Set the psTanDisToOuterSur");
  fPSTanDisToOuterSurCmd ->SetParameterName("psTanDisToOuterSur",false);
  fPSTanDisToOuterSurCmd ->SetDefaultUnit("cm");
  fPSTanDisToOuterSurCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
   
  fLightGuideCmd = new G4UIcmdWithABool("/NuSD/geometry/opticalReadOut/isLightGuideOn",this);
  fLightGuideCmd->SetGuidance("On/Off the lightGuide");
  fLightGuideCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  
  fLightGuideLengthCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/lightGuideLength",this);
  fLightGuideLengthCmd->SetGuidance("Set the length of light guide ");
  fLightGuideLengthCmd->SetParameterName("lgLength",false);
  fLightGuideLengthCmd->SetDefaultUnit("cm");
  fLightGuideLengthCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
        
  fLgBinderCmd = new G4UIcmdWithABool("/NuSD/geometry/opticalReadOut/isLgBinderOn",this);
  fLgBinderCmd->SetGuidance("On/Off the Light guide binder");
  fLgBinderCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
   
  fLgBinderLengthCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/lgBinderLength",this);
  fLgBinderLengthCmd->SetGuidance("Set the length of optical cement");
  fLgBinderLengthCmd->SetDefaultUnit("cm");
  fLgBinderLengthCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDHexPmtPropertiesMessenger::~NuSDHexPmtPropertiesMessenger()
{ 
  delete fPSTanDisToOuterSurCmd;
  delete fLightGuideCmd;
  delete fLightGuideLengthCmd;
  delete fLgBinderCmd;
  delete fLgBinderLengthCmd;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHexPmtPropertiesMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  
  if (command == fPSTanDisToOuterSurCmd)
  {
    fHexPmtProperties->SetPSTanDisToOuterSur(fPSTanDisToOuterSurCmd->GetNewDoubleValue(newValue));
    
  }else if( command == fLightGuideCmd )
  {
    fHexPmtProperties->SetLightGuide(fLightGuideCmd->GetNewBoolValue(newValue));
       
  }else if( command == fLightGuideLengthCmd )
  {
    fHexPmtProperties->SetLgLength(fLightGuideLengthCmd->GetNewDoubleValue(newValue));
        
  }else if( command == fLgBinderCmd )
  {
    fHexPmtProperties->SetLgBinder(fLgBinderCmd->GetNewBoolValue(newValue));
       
  }else if( command == fLgBinderLengthCmd )
  {
    fHexPmtProperties->SetLgBinderLength(fLgBinderLengthCmd->GetNewDoubleValue(newValue));
       
  }
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



