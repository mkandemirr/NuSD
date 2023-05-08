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

#ifndef NuSDGenericPrimaryGeneratorAction_h
#define NuSDGenericPrimaryGeneratorAction_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"

class NuSDGenericPrimaryGeneratorMessenger;
class G4ParticleGun;
class G4Event;

class NuSDGenericPrimaryGeneratorAction : public NuSDVPrimaryGeneratorAction
{
  public:
    NuSDGenericPrimaryGeneratorAction();
    virtual ~NuSDGenericPrimaryGeneratorAction();
    
  public:
    //UI commands
    void SetParticleName(const G4String& name);
    void SetParticleEnergy(G4double energy);

  public:
    virtual void GeneratePrimaries(G4Event*);

  private:
    NuSDGenericPrimaryGeneratorMessenger* fMessenger;
    
    G4ParticleGun* fParticleGun;
    G4String fParticleName;
    G4double fParticleEnergy;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
