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

#include "NuSDGenericPrimaryGeneratorAction.hh"
#include "NuSDGenericPrimaryGeneratorMessenger.hh"

#include "NuSDGenericAnalysisManager.hh"  

#include "Randomize.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "G4PhysicalConstants.hh" //added to use twopi 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDGenericPrimaryGeneratorAction::NuSDGenericPrimaryGeneratorAction(): 
NuSDVPrimaryGeneratorAction(), 
fParticleGun{nullptr},
fParticleName{"e-"},
fParticleEnergy{1*MeV}

{
  fMessenger = new NuSDGenericPrimaryGeneratorMessenger(this);
  
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDGenericPrimaryGeneratorAction::~NuSDGenericPrimaryGeneratorAction()
{
  delete fMessenger;  
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDGenericPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  G4ParticleTable* particleTable          	= G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition  = particleTable->FindParticle(fParticleName);
  fParticleGun->SetParticleDefinition(particleDefinition); 
	
	//energy
	fParticleGun->SetParticleEnergy(fParticleEnergy);
  //position
  G4ThreeVector eventInitPosition = GetEventInitPos();
  fParticleGun->SetParticlePosition( eventInitPosition );
    
	//Get random direcion
	G4double cosTheta       = -1.0+2.0*G4UniformRand();
  G4double phi            = twopi * G4UniformRand();
  G4double sinTheta       = sqrt(1-cosTheta*cosTheta);

  G4double ux             = -sinTheta * cos(phi);
  G4double uy             = -sinTheta * sin(phi);
  G4double uz             = -cosTheta;
  
  fParticleGun->SetParticleMomentumDirection( G4ThreeVector(ux,uy,uz) );
  
  fParticleGun->GeneratePrimaryVertex(anEvent);
     
  NuSDGenericAnalysisManager* analysisManager = NuSDGenericAnalysisManager::GetInstance();
  
  analysisManager->SetParticleEnergy(fParticleEnergy);
  analysisManager->GetEventInitPosVec().push_back( eventInitPosition.x()*(1./cm) );
  analysisManager->GetEventInitPosVec().push_back( eventInitPosition.y()*(1./cm) );
  analysisManager->GetEventInitPosVec().push_back( eventInitPosition.z()*(1./cm) );
 	 	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  NuSDGenericPrimaryGeneratorAction::SetParticleName(const G4String& name) 
{ 
  fParticleName = name; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDGenericPrimaryGeneratorAction::SetParticleEnergy(G4double energy) 
{ 
  fParticleEnergy = energy; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
