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

#ifndef NuSDPhysicsList_h
#define NuSDPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class NuSDPhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NuSDPhysicsList: public G4VModularPhysicsList
{
  public:
    NuSDPhysicsList();
    virtual ~NuSDPhysicsList();

    void ConstructParticle() override;
    void ConstructProcess() override;

  //UI commands
  public: 
    void AddPackage(const G4String& name);
    void SetOpticalPhysicsOn(G4bool bl);
    
  //Getters
  public:
    G4bool IsOpticalPhysicsOn() const {return fIsOpticalPhysicsOn; }

  
  private:
    NuSDPhysicsListMessenger    *fMessenger;
    G4VPhysicsConstructor       *fOptPhysicsList;
    
    G4bool                      fIsOpticalPhysicsOn;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
