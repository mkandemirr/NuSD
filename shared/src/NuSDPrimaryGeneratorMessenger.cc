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

#ifndef GENERIC_PRIMARY_GENERATOR

#include "NuSDPrimaryGeneratorMessenger.hh"
#include "NuSDPrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPrimaryGeneratorMessenger::NuSDPrimaryGeneratorMessenger(NuSDPrimaryGeneratorAction* primaryGenerator)
 : G4UImessenger(),
   fPrimaryGenerator(primaryGenerator)   
{  
  fDirectory = new G4UIdirectory("/NuSD/gun/");
  fDirectory->SetGuidance("Particle Gun control commands.");  
  
  fUseNuSpectFlag = new G4UIcmdWithABool("/NuSD/gun/useNeutrinoSpectrum",this);
  fUseNuSpectFlag->SetGuidance("read neutrino spectrum from root file or not");
  fUseNuSpectFlag->SetGuidance("Default: true.");
  fUseNuSpectFlag->SetParameterName("UseNuSpect",false);
  fUseNuSpectFlag->SetDefaultValue(true);
  fUseNuSpectFlag->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);
  
  fPositronMomentumDirectionCmd = new G4UIcmdWith3Vector("/NuSD/gun/positronMomentumDirection",this);
  fPositronMomentumDirectionCmd->SetGuidance("To activate this command set UseNuSpect to false");
  fPositronMomentumDirectionCmd->SetGuidance("Set the momentum direction of positron");
  fPositronMomentumDirectionCmd->SetGuidance("Default: (0,0,1)");
  fPositronMomentumDirectionCmd->SetParameterName("momDirecX","momDirecY","momDirecZ", false);
  fPositronMomentumDirectionCmd->SetDefaultValue( G4ThreeVector(0,0,1) );
  fPositronMomentumDirectionCmd->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);
  
  fNeutronMomentumDirectionCmd = new G4UIcmdWith3Vector("/NuSD/gun/neutronMomentumDirection",this);
  fNeutronMomentumDirectionCmd->SetGuidance("To activate this command set the UseNuSpect to false");
  fNeutronMomentumDirectionCmd->SetGuidance("Set the momentum direction of neutron");
  fNeutronMomentumDirectionCmd->SetGuidance("Default: (0,0,1)");
  fNeutronMomentumDirectionCmd->SetParameterName("momDirecX","momDirecY","momDirecZ", false);
  fNeutronMomentumDirectionCmd->SetDefaultValue( G4ThreeVector(0,0,1) );
  fNeutronMomentumDirectionCmd->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);
  
  fPositronEnergyCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/gun/positronEnergy",this);
  fPositronEnergyCmd->SetGuidance("To activate this command set UseNuSpect to false.");
  fPositronEnergyCmd->SetGuidance("Set positron energy.");
  fPositronEnergyCmd->SetGuidance("Default: 2 MeV");
  fPositronEnergyCmd->SetParameterName("positronEnergy",false);
  fPositronEnergyCmd->SetDefaultValue(2*MeV);
  fPositronEnergyCmd->SetDefaultUnit("MeV");
  fPositronEnergyCmd->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);
  
  fNeutronEnergyCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/gun/neutronEnergy",this);
  fNeutronEnergyCmd->SetGuidance("To activate this command set UseNuSpect to false.");
  fNeutronEnergyCmd->SetGuidance("Set neutron energy.");
  fNeutronEnergyCmd->SetGuidance("Default: 1 keV");
  fNeutronEnergyCmd->SetParameterName("neutronEnergy",false);
  fNeutronEnergyCmd->SetDefaultValue(1*keV);
  fNeutronEnergyCmd->SetDefaultUnit("keV");
  fNeutronEnergyCmd->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPrimaryGeneratorMessenger::~NuSDPrimaryGeneratorMessenger()
{
  delete fDirectory;
  delete fUseNuSpectFlag;
  delete fPositronMomentumDirectionCmd;
  delete fNeutronMomentumDirectionCmd;
  delete fPositronEnergyCmd;
  delete fNeutronEnergyCmd;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command, G4String newValue)
{
  if( command == fUseNuSpectFlag )
  {  
    fPrimaryGenerator->SetNuSpectFlag(fUseNuSpectFlag->GetNewBoolValue(newValue));
  }else if( command == fPositronMomentumDirectionCmd )
  { 
    fPrimaryGenerator->SetPositronMomentumDirection(fPositronMomentumDirectionCmd->GetNew3VectorValue(newValue));
  }else if( command == fNeutronMomentumDirectionCmd )
  { 
    fPrimaryGenerator->SetNeutronMomentumDirection(fNeutronMomentumDirectionCmd->GetNew3VectorValue(newValue));
  }else if( command == fPositronEnergyCmd )
  { 
    fPrimaryGenerator->SetPositronEnergy(fPositronEnergyCmd->GetNewDoubleValue(newValue)); 
  }else if( command == fNeutronEnergyCmd )
  { 
    fPrimaryGenerator->SetNeutronEnergy(fNeutronEnergyCmd->GetNewDoubleValue(newValue)); 
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

