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

#ifndef NuSDPrimaryGeneratorAction_h
#define NuSDPrimaryGeneratorAction_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class TFile;
class NuSDPrimaryGeneratorMessenger;

class NuSDPrimaryGeneratorAction : public NuSDVPrimaryGeneratorAction
{
  public:
    NuSDPrimaryGeneratorAction();
    virtual ~NuSDPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);
  
  public:  
    //UI commands
    void SetNuSpectFlag(G4bool useSpectFlag);
    void SetPositronMomentumDirection(G4ThreeVector posMomDirec);
    void SetNeutronMomentumDirection(G4ThreeVector neutronMomDirec);
    void SetPositronEnergy(G4double positronEnegy);
    void SetNeutronEnergy(G4double neutronEnergy);

  private:
    G4double GetRandomEnergyFromNuSpectrum();
    G4double GetRandomAngle();
    
    void SetPositronAndNeutronEnergy(G4double cosTheta);
    void SetPositronAndNeutronMomentumDirection(G4double cosTheta);
    
  private:
    NuSDPrimaryGeneratorMessenger* fMessenger;
    static TFile* fInputFile;
    
    G4ParticleGun* fParticlePositronGun;
    G4ParticleGun* fParticleNeutronGun;
   
    G4ThreeVector fNuMomentumDirection;
    G4ThreeVector fPositronMomentumDirection;
    G4ThreeVector fNeutronMomentumDirection;
    
    G4double fNuEnergy;
    G4double fPositronEnergy;
    G4double fNeutronEnergy;
    
    G4bool fUseNuSpect;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
