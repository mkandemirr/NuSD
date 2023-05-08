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

#if DETECTOR_NAME != SOLID && DETECTOR_NAME != HSP 

#include "NuSDPmtPropertiesMessenger.hh"
#include "NuSDPmtProperties.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPmtPropertiesMessenger::NuSDPmtPropertiesMessenger(NuSDPmtProperties* pmtProperties)
 : G4UImessenger(), fPmtProperties{pmtProperties}
   
{ 
  
  fPSRadiusCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/photoCathodeRadius",this);
  fPSRadiusCmd ->SetGuidance("Set the radius of photocathode");
  fPSRadiusCmd ->SetParameterName("photoCathodeRadius",false);
  fPSRadiusCmd ->SetDefaultUnit("cm");
  fPSRadiusCmd ->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fPSRadiusCmd->SetToBeBroadcasted(false);
   
  fLightGuideCmd = new G4UIcmdWithABool("/NuSD/geometry/opticalReadOut/isLightGuideOn",this);
  fLightGuideCmd->SetGuidance("On/Off the lightGuide");
  fLightGuideCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fLightGuideCmd->SetToBeBroadcasted(false);
  
  fLightGuideLengthCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/lightGuideLength",this);
  fLightGuideLengthCmd->SetGuidance("Set the length of light guide ");
  fLightGuideLengthCmd->SetParameterName("lgLength",false);
  fLightGuideLengthCmd->SetDefaultUnit("cm");
  fLightGuideLengthCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fLightGuideLengthCmd->SetToBeBroadcasted(false);
  
  fLightGuideDimensionCmd = new G4UIcmdWith3VectorAndUnit("/NuSD/geometry/opticalReadOut/lightGuidePmtEndDimension",this);
  fLightGuideDimensionCmd->SetGuidance("Set the dimensions of light guide on the pmt side");
  fLightGuideDimensionCmd->SetParameterName("lgSizeX","lgSizeY","lgSizeZ",false);
  fLightGuideDimensionCmd->SetDefaultUnit("cm");
  fLightGuideDimensionCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fLightGuideDimensionCmd->SetToBeBroadcasted(false);    
      
  fLgBinderCmd = new G4UIcmdWithABool("/NuSD/geometry/opticalReadOut/isLgBinderOn",this);
  fLgBinderCmd->SetGuidance("On/Off the Light guide binder");
  fLgBinderCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fLgBinderCmd->SetToBeBroadcasted(false); 
   
  fLgBinderLengthCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/lgBinderLength",this);
  fLgBinderLengthCmd->SetGuidance("Set the length of optical cement");
  fLgBinderLengthCmd->SetDefaultUnit("cm");
  fLgBinderLengthCmd->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fLgBinderLengthCmd->SetToBeBroadcasted(false);      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPmtPropertiesMessenger::~NuSDPmtPropertiesMessenger()
{ 
  delete fPSRadiusCmd;
  
  delete fLightGuideCmd;
  delete fLightGuideLengthCmd;
  delete fLightGuideDimensionCmd;
  delete fLgBinderCmd;
  delete fLgBinderLengthCmd;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPmtPropertiesMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  
  if (command == fPSRadiusCmd)
  {
    fPmtProperties->SetPSRadius(fPSRadiusCmd->GetNewDoubleValue(newValue));
    
  }else if( command == fLightGuideCmd )
  {
    fPmtProperties->SetLightGuide(fLightGuideCmd->GetNewBoolValue(newValue));
       
  }else if( command == fLightGuideLengthCmd )
  {
    fPmtProperties->SetLgLength(fLightGuideLengthCmd->GetNewDoubleValue(newValue));
        
  }else if (command == fLightGuideDimensionCmd)
  {
  
    G4int x = fLightGuideDimensionCmd->GetNew3VectorValue(newValue).x();
    G4int y = fLightGuideDimensionCmd->GetNew3VectorValue(newValue).y();
    G4int z = fLightGuideDimensionCmd->GetNew3VectorValue(newValue).z();
    
    fPmtProperties->SetLgPmtEndSizeX(x);
    fPmtProperties->SetLgPmtEndSizeY(y);
    fPmtProperties->SetLgPmtEndSizeZ(z);
    
       
  } else if( command == fLgBinderCmd )
  {
    fPmtProperties->SetLgBinder(fLgBinderCmd->GetNewBoolValue(newValue));
       
  }else if( command == fLgBinderLengthCmd )
  {
    fPmtProperties->SetLgBinderLength(fLgBinderLengthCmd->GetNewDoubleValue(newValue));
       
  }
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


