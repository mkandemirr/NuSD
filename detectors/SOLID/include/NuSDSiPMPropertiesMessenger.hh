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

#ifndef NuSDSiPMPropertiesMessenger_h
#define NuSDSiPMPropertiesMessenger_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4UImessenger.hh"

class NuSDSiPMProperties;
class G4UIcmdWithADoubleAndUnit;

class NuSDSiPMPropertiesMessenger: public G4UImessenger
{
  public:
    NuSDSiPMPropertiesMessenger(NuSDSiPMProperties* pmtProperties);
  virtual ~NuSDSiPMPropertiesMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    NuSDSiPMProperties                *fSiPMProperties;
    G4UIcmdWithADoubleAndUnit         *fPSSizeXCmd;
    G4UIcmdWithADoubleAndUnit         *fPSSizeYCmd;
    G4UIcmdWithADoubleAndUnit         *fPSSizeZCmd;
             
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
