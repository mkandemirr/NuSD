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

#ifdef GENERIC_PRIMARY_GENERATOR

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDGenericPrimaryGeneratorMessenger.hh"
#include "NuSDGenericPrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDGenericPrimaryGeneratorMessenger::NuSDGenericPrimaryGeneratorMessenger(NuSDGenericPrimaryGeneratorAction* gPrimaryGen)
 : G4UImessenger(),
   fGenericPrimaryGenerator(gPrimaryGen)
{

  fParticleCmd = new G4UIcmdWithAString("/NuSD/gun/particleName",this);
  fParticleCmd->SetGuidance("Set particle to be generated.");
  fParticleCmd->SetGuidance(" (electron is default)");
  fParticleCmd->SetParameterName("particleName",false);
  fParticleCmd->SetDefaultValue("electron");
  
  fEnergyCmd = new G4UIcmdWithADoubleAndUnit("/NuSD/gun/particleEnergy",this);
  fEnergyCmd->SetGuidance("Set kinetic energy.");
  fEnergyCmd->SetParameterName("Energy",false);
  fEnergyCmd->SetDefaultUnit("MeV");
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDGenericPrimaryGeneratorMessenger::~NuSDGenericPrimaryGeneratorMessenger()
{ 
  delete fParticleCmd;
  delete fEnergyCmd; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDGenericPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,
                                               G4String newValue)
{
  if( command==fParticleCmd )
  { 
    fGenericPrimaryGenerator->SetParticleName(newValue); 
  }else if( command==fEnergyCmd )
  { 
    fGenericPrimaryGenerator->SetParticleEnergy(fEnergyCmd->GetNewDoubleValue(newValue)); 
  }
     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

