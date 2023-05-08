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

#ifndef NuSDHexPmtPropertiesMessenger_h
#define NuSDHexPmtPropertiesMessenger_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4UImessenger.hh"

class NuSDHexPmtProperties;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

class NuSDHexPmtPropertiesMessenger: public G4UImessenger
{
  public:
    NuSDHexPmtPropertiesMessenger(NuSDHexPmtProperties* pmtProperties);
  virtual ~NuSDHexPmtPropertiesMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    NuSDHexPmtProperties              *fHexPmtProperties;
    G4UIcmdWithADoubleAndUnit         *fPSTanDisToOuterSurCmd;
    G4UIcmdWithABool                  *fLightGuideCmd;
    G4UIcmdWithADoubleAndUnit         *fLightGuideLengthCmd;  
    G4UIcmdWithABool                  *fLgBinderCmd; 
    G4UIcmdWithADoubleAndUnit         *fLgBinderLengthCmd;
         
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
