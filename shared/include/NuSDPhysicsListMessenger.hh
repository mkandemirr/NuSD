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

#ifndef NuSDPhysicsListMessenger_h
#define NuSDPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class NuSDPhysicsList;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NuSDPhysicsListMessenger: public G4UImessenger
{
  public:
  
    NuSDPhysicsListMessenger(NuSDPhysicsList* );
   ~NuSDPhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
  NuSDPhysicsList* fPhysicsList;
    
  G4UIdirectory*             fPhysDir;        
  G4UIcmdWithAString*        fPackageListCmd;
  G4UIcmdWithABool  *fOpticalPhysicsCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

