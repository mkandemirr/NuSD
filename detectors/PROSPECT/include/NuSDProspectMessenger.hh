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

#ifndef NuSDProspectMessenger_h
#define NuSDProspectMessenger_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4UImessenger.hh"

class NuSDProspectDetConstruction;
class G4UIcmdWithADoubleAndUnit;

class NuSDProspectMessenger: public G4UImessenger
{
  public:
    NuSDProspectMessenger(NuSDProspectDetConstruction* );
  virtual ~NuSDProspectMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    NuSDProspectDetConstruction     *fDetConstruction;
    
    G4UIcmdWithADoubleAndUnit       *fReflectorThicknessCmd;
    G4UIcmdWithADoubleAndUnit       *fLiquidTankThicknessCmd;
               
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
