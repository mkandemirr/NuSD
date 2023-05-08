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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//#include <TF1.h>

#include <TFile.h>
#include <TH1D.h>



#include "NuSDPrimaryGeneratorAction.hh"
#include "NuSDPrimaryGeneratorMessenger.hh"
#include "NuSDAnalysisManager.hh"   
#include "Randomize.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh" //added to use twopi 
#include "G4AutoLock.hh"

namespace { G4Mutex lowEnergyPrimGenMutex = G4MUTEX_INITIALIZER; }

TFile* NuSDPrimaryGeneratorAction::fInputFile = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPrimaryGeneratorAction::NuSDPrimaryGeneratorAction():
NuSDVPrimaryGeneratorAction(), 
fMessenger{nullptr},
fParticlePositronGun{nullptr},
fParticleNeutronGun{nullptr},      
fNuMomentumDirection(),
fPositronMomentumDirection(0,0,1),
fNeutronMomentumDirection(0,0,1),
fNuEnergy{0.},
fPositronEnergy{3*MeV},
fNeutronEnergy{1*keV},
fUseNuSpect{true}

{    

  fMessenger = new NuSDPrimaryGeneratorMessenger(this);  
  
  G4int n_particle = 1;
  fParticleNeutronGun   = new G4ParticleGun(n_particle);
  fParticlePositronGun  = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable          = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleNeutron   = particleTable->FindParticle("neutron");
  G4ParticleDefinition* particlePositron  = particleTable->FindParticle("e+");

  fParticleNeutronGun->SetParticleDefinition(particleNeutron);
  fParticlePositronGun->SetParticleDefinition(particlePositron); 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPrimaryGeneratorAction::~NuSDPrimaryGeneratorAction()
{ 
  delete fMessenger;
  delete fParticleNeutronGun;
  delete fParticlePositronGun;
  
  G4AutoLock lock(&lowEnergyPrimGenMutex);
  if(fInputFile) 
  {
    delete fInputFile; 
    fInputFile = nullptr; //dont forget this! 
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
	
void NuSDPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{  	
  //position
  G4ThreeVector eventInitPos = GetEventInitPos();
  fParticlePositronGun->SetParticlePosition( eventInitPos );
  fParticleNeutronGun->SetParticlePosition( eventInitPos );
   
  if(fUseNuSpect)
  {
    //theta is the angle between positron and antiNu
    G4double cosTheta = GetRandomAngle(); 
    
    SetPositronAndNeutronEnergy(cosTheta);
    SetPositronAndNeutronMomentumDirection(cosTheta);
    
  } 
  
  //Energy
  fParticlePositronGun->SetParticleEnergy(fPositronEnergy);
  fParticleNeutronGun->SetParticleEnergy(fNeutronEnergy);
   
  //Momentum direction 
  fParticlePositronGun->SetParticleMomentumDirection(fPositronMomentumDirection);
  fParticleNeutronGun->SetParticleMomentumDirection(fNeutronMomentumDirection);

  fParticlePositronGun->GeneratePrimaryVertex(anEvent);
  fParticleNeutronGun->GeneratePrimaryVertex(anEvent);
  	 
  NuSDAnalysisManager* analysisManager = NuSDAnalysisManager::GetInstance();	 
  
  analysisManager->SetNeutrinoEnergy( fNuEnergy );
  analysisManager->SetPositronEnergy( fPositronEnergy ); //MeV!
  analysisManager->SetNeutronEnergy( fNeutronEnergy*(1./keV) ); 
  
  //3d vectors, three elements 
  
  if(fUseNuSpect)
  {
    analysisManager->GetNeutrinoMomentumDirectionVec().push_back( fNuMomentumDirection.x() );	
    analysisManager->GetNeutrinoMomentumDirectionVec().push_back( fNuMomentumDirection.y() );	
    analysisManager->GetNeutrinoMomentumDirectionVec().push_back( fNuMomentumDirection.z() );
  } 
  
  analysisManager->GetPositronMomentumDirectionVec().push_back( fPositronMomentumDirection.x() );	
  analysisManager->GetPositronMomentumDirectionVec().push_back( fPositronMomentumDirection.y() );	
  analysisManager->GetPositronMomentumDirectionVec().push_back( fPositronMomentumDirection.z() );
  
  analysisManager->GetNeutronMomentumDirectionVec().push_back( fNeutronMomentumDirection.x() );	
  analysisManager->GetNeutronMomentumDirectionVec().push_back( fNeutronMomentumDirection.y() );	
  analysisManager->GetNeutronMomentumDirectionVec().push_back( fNeutronMomentumDirection.z() );	
  
  analysisManager->GetEventInitPosVec().push_back( eventInitPos.x()*(1./cm) );
  analysisManager->GetEventInitPosVec().push_back( eventInitPos.y()*(1./cm) );
  analysisManager->GetEventInitPosVec().push_back( eventInitPos.z()*(1./cm) );
 	 	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NuSDPrimaryGeneratorAction::GetRandomAngle()
{
  G4AutoLock lock(&lowEnergyPrimGenMutex);
 
  if(!fInputFile)
    fInputFile = new TFile("neutrinoSpectrum/nuSpect.root","READ");
    
  TH1D *angleSpect = (TH1D*)fInputFile->Get("angleHisto");
  G4double theta = angleSpect->GetRandom();
  
  return theta; 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double NuSDPrimaryGeneratorAction::GetRandomEnergyFromNuSpectrum()
{
  //Reactor nu emission spect. u5(u-235), u8(u-238), p9(Pu-239) p1(Pu-241)
	//Huber and Muller model
	
  //ROOT::EnableThreadSafety();
  G4AutoLock lock(&lowEnergyPrimGenMutex);
  
  if(!fInputFile)
    fInputFile = new TFile("neutrinoSpectrum/nuSpect.root","READ");
    
  TH1D * nuSpect = (TH1D*)fInputFile->Get("nuHisto");
  G4double initNuEnegy = nuSpect->GetRandom();
  
  return initNuEnegy;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NuSDPrimaryGeneratorAction::SetPositronAndNeutronEnergy(G4double cosTheta)
{
  G4double delta  = 1.293*MeV; //neutron-proton mass
  G4double M_n    = 939.6*MeV; //neutron mass
  G4double M_e    = 0.511*MeV; //electron mass
  
  fNuEnergy       = GetRandomEnergyFromNuSpectrum();
  //IBD kinematics 
  fPositronEnergy = (fNuEnergy-delta)*(1- (fNuEnergy/M_n)*(1-cosTheta) ) - (delta*delta-M_e*M_e)/(2*M_n);
  fNeutronEnergy  = fNuEnergy*(fNuEnergy-delta)*(1./M_n)*(1-cosTheta) + (delta*delta-M_e*M_e)/(2*M_n);    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPrimaryGeneratorAction::SetPositronAndNeutronMomentumDirection(G4double cosTheta)
{
  /*
	Angular correlation between positron and nu. we assume azimutal angle(xy plane) is the same for 
	both momentum direction of positron and nu. This is not exact!! 
	*/
  
  //angle between z-axis and nu momentum direction.    
  G4double nuTheta     = G4UniformRand()*( pi-acos(cosTheta) );  //nuTheta<=pi-theta
  
  G4double nuCosTheta  = cos(nuTheta);
  G4double nuSinTheta  = sqrt(1-nuCosTheta*nuCosTheta);
  G4double phi         = twopi*G4UniformRand(); //the same for both nu and positron momentum vec.
  
  G4double ux          = -nuSinTheta*cos(phi);
  G4double uy          = -nuSinTheta*sin(phi);
  G4double uz          = -nuCosTheta;
    
  fNuMomentumDirection = G4ThreeVector(ux,uy,uz);
  
  //angle between z-axis and positron momentum direction.
  G4double positronTheta    = acos(cosTheta)+nuTheta;
  G4double positronCosTheta = cos(positronTheta); 
  G4double positronSinTheta = sqrt(1-positronCosTheta*positronCosTheta);
  
  ux = -positronSinTheta*cos(phi);
  uy = -positronSinTheta*sin(phi);
  uz = -positronCosTheta;
  
  fPositronMomentumDirection = G4ThreeVector(ux,uy,uz);
  fNeutronMomentumDirection  = fNuMomentumDirection-fPositronMomentumDirection; //momentum conversation
  fNeutronMomentumDirection  = fNeutronMomentumDirection.unit(); //normalized								
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//UI commands

void NuSDPrimaryGeneratorAction::SetNuSpectFlag(G4bool useSpectFlag)
{
  fUseNuSpect = useSpectFlag;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPrimaryGeneratorAction::SetPositronMomentumDirection(G4ThreeVector posMomDirec)
{
  fPositronMomentumDirection = posMomDirec;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPrimaryGeneratorAction::SetNeutronMomentumDirection(G4ThreeVector neutronMomDirec)
{
  fNeutronMomentumDirection = neutronMomDirec;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPrimaryGeneratorAction::SetPositronEnergy(G4double positronEnergy)
{
  fPositronEnergy = positronEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPrimaryGeneratorAction::SetNeutronEnergy(G4double neutronEnergy)
{
  fNeutronEnergy = neutronEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
