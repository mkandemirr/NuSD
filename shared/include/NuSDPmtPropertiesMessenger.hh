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

#ifndef NuSDPmtPropertiesMessenger_h
#define NuSDPmtPropertiesMessenger_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4UImessenger.hh"

class NuSDPmtProperties;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

class NuSDPmtPropertiesMessenger: public G4UImessenger
{
  public:
    NuSDPmtPropertiesMessenger(NuSDPmtProperties* pmtProperties);
  virtual ~NuSDPmtPropertiesMessenger();

  virtual void SetNewValue(G4UIcommand*, G4String);

  private:
    NuSDPmtProperties                 *fPmtProperties;
    G4UIcmdWithADoubleAndUnit         *fPSRadiusCmd;
    G4UIcmdWithABool                  *fLightGuideCmd;
    G4UIcmdWithADoubleAndUnit         *fLightGuideLengthCmd;  
    G4UIcmdWith3VectorAndUnit         *fLightGuideDimensionCmd;
    G4UIcmdWithABool                  *fLgBinderCmd; 
    G4UIcmdWithADoubleAndUnit         *fLgBinderLengthCmd;
         
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
