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

#ifndef NuSDPrimaryGeneratorMessenger_h
#define NuSDPrimaryGeneratorMessenger_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"

class NuSDPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWith3Vector;
class G4UIcmdWithADoubleAndUnit;

class NuSDPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    NuSDPrimaryGeneratorMessenger(NuSDPrimaryGeneratorAction* primaryGenerator);
    virtual ~NuSDPrimaryGeneratorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    NuSDPrimaryGeneratorAction*     fPrimaryGenerator;
    
    G4UIdirectory*                  fDirectory; 
    G4UIcmdWithABool*               fUseNuSpectFlag;
    G4UIcmdWith3Vector*             fPositronMomentumDirectionCmd;
    G4UIcmdWith3Vector*             fNeutronMomentumDirectionCmd;
    G4UIcmdWithADoubleAndUnit*      fPositronEnergyCmd; 
    G4UIcmdWithADoubleAndUnit*      fNeutronEnergyCmd;   

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
