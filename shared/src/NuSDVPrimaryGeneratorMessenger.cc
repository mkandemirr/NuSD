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

#include "NuSDVPrimaryGeneratorMessenger.hh"
#include "NuSDVPrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVPrimaryGeneratorMessenger::NuSDVPrimaryGeneratorMessenger(NuSDVPrimaryGeneratorAction* vPrimaryGenerator)
 : G4UImessenger(),
   fVPrimaryGenerator(vPrimaryGenerator),
   fRndmPosFlagCmd(0),
   fEventInitPosCmd()
{  
  fDirectory = new G4UIdirectory("/NuSD/gun/");
  fDirectory->SetGuidance("Particle Gun control commands.");  
    
  fRndmPosFlagCmd = new G4UIcmdWithABool("/NuSD/gun/isRandomInitPos",this);
  fRndmPosFlagCmd->SetGuidance("Generate event in a random point inside the neutrino scintillator or not");
  fRndmPosFlagCmd->SetParameterName("RndmPos",false);
  fRndmPosFlagCmd->SetDefaultValue(true);
  fRndmPosFlagCmd->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);
 
  fEventInitPosCmd = new G4UIcmdWith3VectorAndUnit("/NuSD/gun/eventInitialPositon",this);
  fEventInitPosCmd->SetGuidance("To activate this command first set the RndmPos to false.");
  fEventInitPosCmd->SetGuidance("Set the initial position of the event in a randomly selected neutrino scintillator unit.");
  fEventInitPosCmd->SetGuidance("Default: (0,0,0)");
  fEventInitPosCmd->SetParameterName("posX","posY","posZ", false);
  fEventInitPosCmd->SetDefaultValue( G4ThreeVector(0,0,0) );
  fEventInitPosCmd->SetDefaultUnit("cm");
  //fEventInitPosCmd->AvailableForStates(G4State_PreInit,G4State_Init);
  //fEventInitPosCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fEventInitPosCmd->AvailableForStates(G4State_PreInit,G4State_Init,G4State_Idle);
  //fEventInitPosCmd->AvailableForStates(G4State_Idle);
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVPrimaryGeneratorMessenger::~NuSDVPrimaryGeneratorMessenger()
{
  delete fDirectory;
  delete fRndmPosFlagCmd;
  delete fEventInitPosCmd;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command, G4String newValue)
{

  if( command == fRndmPosFlagCmd )
  {  
    fVPrimaryGenerator->SetRndmFlag(fRndmPosFlagCmd->GetNewBoolValue(newValue));
  }else if( command == fEventInitPosCmd )
  { 
    fVPrimaryGenerator->SetPosInNuScnt(fEventInitPosCmd->GetNew3VectorValue(newValue));
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
