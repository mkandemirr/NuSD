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

#include "NuSDPhotoSensorPropertiesMessenger.hh"
#include "NuSDVOptReadoutUnit.hh"
#include "NuSDPhotoSensorProperties.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhotoSensorPropertiesMessenger::NuSDPhotoSensorPropertiesMessenger(NuSDPhotoSensorProperties* psPropery)
 : G4UImessenger(), fPSProperties{psPropery}
   
{
 
  fOptReadoutDirectory = new G4UIdirectory("/NuSD/geometry/opticalReadOut/");
  fOptReadoutDirectory->SetGuidance("Optical readout construction control");
  
  fOptReadoutAttachedBothEndCmd = new G4UIcmdWithABool("/NuSD/geometry/opticalReadOut/isAttachedBothEnd",this);
  fOptReadoutAttachedBothEndCmd->SetGuidance("On/Off the Pmts on one side");
  fOptReadoutAttachedBothEndCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fOptReadoutAttachedBothEndCmd->SetDefaultValue(true); 
  fOptReadoutAttachedBothEndCmd->SetToBeBroadcasted(false);  
    
  fPSBinderCmd = new G4UIcmdWithABool("/NuSD/geometry/opticalReadOut/isPSBinderOn",this);
  fPSBinderCmd->SetGuidance("On/Off the Photo sensor binder");
  fPSBinderCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fPSBinderCmd->SetToBeBroadcasted(false); 
   
  fPSBinderLengthCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/geometry/opticalReadOut/psBinderLength",this);
  fPSBinderLengthCmd->SetGuidance("Set the length of optical grease ");
  fPSBinderLengthCmd->SetDefaultUnit("cm");
  fPSBinderLengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  
  fPSBinderLengthCmd->SetToBeBroadcasted(false);
  
  fPSQESpectCmd = new G4UIcmdWithABool("/NuSD/geometry/surface/photoCathode/isQuantumEfficiencyDependsEnergy",this);
  fPSQESpectCmd->SetGuidance("Decide if ps quantum efficiency depends on energy");
  fPSQESpectCmd->SetGuidance("if it is true, fill PSQE_spect.txt file located in /pathToBuild/Data/PhotoSensors/ ");
  fPSQESpectCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fPSQESpectCmd->SetToBeBroadcasted(false);
  
  fPSQECmd = new G4UIcmdWithADouble("/NuSD/geometry/surface/photoCathode/quantumEfficiency",this);
  fPSQECmd->SetGuidance("Set the quantum efficiency of the ps.");
  fPSQECmd->SetParameterName("quantumEfficiency",false);
  fPSQECmd->SetRange("quantumEfficiency>=0 && quantumEfficiency<=1");
  fPSQECmd->AvailableForStates(G4State_PreInit, G4State_Idle); 
  fPSQECmd->SetToBeBroadcasted(false);
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhotoSensorPropertiesMessenger::~NuSDPhotoSensorPropertiesMessenger()
{
  
  delete fOptReadoutDirectory;
  delete fOptReadoutAttachedBothEndCmd;
  delete fPSBinderCmd;
  delete fPSBinderLengthCmd;
  delete fPSQESpectCmd;
  delete fPSQECmd;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPhotoSensorPropertiesMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == fOptReadoutAttachedBothEndCmd )
  {
    fPSProperties->SetBothEnd(fOptReadoutAttachedBothEndCmd->GetNewBoolValue(newValue));
      
  }else if( command == fPSBinderCmd )
  {
    fPSProperties->SetPSBinder(fPSBinderCmd->GetNewBoolValue(newValue));
      
  }else if( command == fPSBinderLengthCmd )
  {
    fPSProperties->SetPSBinderLength(fPSBinderLengthCmd->GetNewDoubleValue(newValue));      
  }else if( command == fPSQESpectCmd )
  {
    fPSProperties->SetQEDependsEnergy(fPSQESpectCmd->GetNewBoolValue(newValue));
    
  }  else if( command == fPSQECmd )
  {
    fPSProperties->SetPSQE(fPSQECmd->GetNewDoubleValue(newValue));   
  }   
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


