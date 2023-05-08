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

#ifndef NuSDPhotoSensorSurface_H
#define NuSDPhotoSensorSurface_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"

class NuSDPhotoSensorProperties;

class NuSDPhotoSensorSurface: public G4OpticalSurface 
{
  public:
    NuSDPhotoSensorSurface(const G4String &name, 
                           G4OpticalSurfaceModel model, 
                           G4OpticalSurfaceFinish finish, 
                           G4SurfaceType type, 
                           NuSDPhotoSensorProperties* psProp
                           );

    virtual ~NuSDPhotoSensorSurface();
    
  private:
    void SetPSSurface(G4String name);
    
  private:
    NuSDPhotoSensorProperties* fPSProperties;  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
