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

#ifndef NuSDReflectorSurface_H
#define NuSDReflectorSurface_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"

class NuSDReflectorProperties;

class NuSDReflectorSurface: public G4OpticalSurface 
{
  public:
    NuSDReflectorSurface(const G4String &name, 
                        G4OpticalSurfaceModel model, 
                        G4OpticalSurfaceFinish finish, 
                        G4SurfaceType type, 
                        NuSDReflectorProperties* refProp 
                        );

    virtual ~NuSDReflectorSurface();
  
  private:
    void SetReflectorSurface(); // for PFP
    void SetReflectorAndGapSurface(); //for PBP
  
  private:
    NuSDReflectorProperties *fReflectorProperties;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
