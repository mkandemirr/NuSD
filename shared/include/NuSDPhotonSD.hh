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

#ifndef NuSDPhotonSD_h
#define NuSDPhotonSD_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VSensitiveDetector.hh"
#include "NuSDPhotonSDHit.hh"

class G4Step;
class G4HCofThisEvent;

class NuSDPhotonSD : public G4VSensitiveDetector
{
  public:
    NuSDPhotonSD(const G4String& name);
    virtual ~NuSDPhotonSD();

    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);

    //A version of processHits that keeps aStep constant
    G4bool ProcessHits_constStep(const G4Step*, G4TouchableHistory* );

    virtual void  EndOfEvent(G4HCofThisEvent* hitCollection);
    virtual void PrintAll() override;
  
  private:
    NuSDPhotonSDHitsCollection* fHitsCollection;
    G4int fHCID;
     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

