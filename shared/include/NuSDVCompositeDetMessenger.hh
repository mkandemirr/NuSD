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

#ifndef NuSDVCompositeDetMessenger_h
#define NuSDVCompositeDetMessenger_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4UImessenger.hh"

class NuSDVCompositeDetConstruction;
class G4UIdirectory;

class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;


class NuSDVCompositeDetMessenger: public G4UImessenger
{
  public:
    NuSDVCompositeDetMessenger(NuSDVCompositeDetConstruction* );
  virtual ~NuSDVCompositeDetMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    NuSDVCompositeDetConstruction       *fDetConstruction;
    
    G4UIdirectory                     *fDetConstructionDirectory;
    
    G4UIcmdWithADoubleAndUnit         *fNeutronScntThicknessCmd;
    
                
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
