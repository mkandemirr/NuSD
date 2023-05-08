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

#ifndef NuSDVPrimaryGeneratorAction_h
#define NuSDVPrimaryGeneratorAction_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSD_config.h"

#if DETECTOR_NAME == CHANDLER
#include "NuSDChandlerDetConstruction.hh"
#elif DETECTOR_NAME == SWEANY
#include "NuSDSweanyDetConstruction.hh"
#elif DETECTOR_NAME == SOLID
#include "NuSDSolidDetConstruction.hh"
#elif DETECTOR_NAME == NULAT
#include "NuSDNulatDetConstruction.hh"
#elif DETECTOR_NAME == PANDA
#include "NuSDPandaDetConstruction.hh"
#elif DETECTOR_NAME == PROSPECT
#include "NuSDProspectDetConstruction.hh"
#elif DETECTOR_NAME == HSP
#include "NuSDHspDetConstruction.hh"
#endif  

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"

class NuSDVPrimaryGeneratorMessenger;
class G4ParticleGun;
class G4Event;
class G4Box;

class NuSDVPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    NuSDVPrimaryGeneratorAction();
    virtual ~NuSDVPrimaryGeneratorAction();

   // virtual void GeneratePrimaries(G4Event*);

  private:
    G4ThreeVector SelectARndmUnitAndGetPos();
    G4ThreeVector GetNuScntPos();
    G4ThreeVector GetRandomPosInNuScnt();
  protected:
    G4ThreeVector GetEventInitPos();
    
  public:
    void SetPosInNuScnt(G4ThreeVector pos);
    void SetRndmFlag(G4bool flag);
  
  private:
    NuSDVPrimaryGeneratorMessenger* fMessenger;
    G4ThreeVector fPosInNuScnt;
    G4bool fRndmPosFlag;

#if DETECTOR_NAME == CHANDLER
    const NuSDChandlerDetConstruction *fDetConstruction;
#elif DETECTOR_NAME == SWEANY
    const NuSDSweanyDetConstruction *fDetConstruction;
#elif DETECTOR_NAME == SOLID
    const NuSDSolidDetConstruction *fDetConstruction;
#elif DETECTOR_NAME == NULAT
    const NuSDNulatDetConstruction *fDetConstruction;
#elif DETECTOR_NAME == PANDA
    const NuSDPandaDetConstruction *fDetConstruction;
#elif DETECTOR_NAME == PROSPECT
    const NuSDProspectDetConstruction *fDetConstruction;
#elif DETECTOR_NAME == HSP
    const NuSDHspDetConstruction *fDetConstruction;
#endif  


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
