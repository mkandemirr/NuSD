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

#ifndef NuSDVPrimaryGeneratorMessenger_h
#define NuSDVPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"

class NuSDVPrimaryGeneratorAction;
class G4UIcmdWithABool;
class G4UIcmdWith3VectorAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NuSDVPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    NuSDVPrimaryGeneratorMessenger(NuSDVPrimaryGeneratorAction* vPrimaryGenerator);
    virtual ~NuSDVPrimaryGeneratorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    NuSDVPrimaryGeneratorAction*    fVPrimaryGenerator;
    G4UIdirectory*                  fDirectory;
    G4UIcmdWithABool*               fRndmPosFlagCmd;
    G4UIcmdWith3VectorAndUnit*      fEventInitPosCmd;
};

#endif
