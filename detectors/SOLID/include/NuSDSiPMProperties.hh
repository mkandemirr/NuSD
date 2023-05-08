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

#ifndef NuSDSiPMProperties_H
#define NuSDSiPMProperties_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDPhotoSensorProperties.hh"
#include "globals.hh"

class NuSDSiPMPropertiesMessenger;

class NuSDSiPMProperties: public NuSDPhotoSensorProperties 
{

  public:
    NuSDSiPMProperties(G4bool isAttachedBothEnd, 
                      G4bool isPSBinderOn,
                      G4double psBinderLength,
                      G4double psQE,
                      G4bool isQEDependsEnergy,
                      G4double psSizeX,
                      G4double psSizeY,
                      G4double psSizeZ
                     );
    
    ~NuSDSiPMProperties();
  
  public:
    G4double GetSiPMReadoutUnitLength(); 
  
  public:
    //UI commands   
    void SetPSSizeX(G4double x);
    void SetPSSizeY(G4double y);
    void SetPSSizeZ(G4double z);
         
  //Gettters      
  public:
    G4double GetPSSizeX() const { return fPSSizeX; }
    G4double GetPSSizeY() const { return fPSSizeY; }
    G4double GetPSSizeZ() const { return fPSSizeZ; }
    
  private:
    NuSDSiPMPropertiesMessenger* fSiPMPropertiesMessenger;      
         
  private:              
    G4double fPSSizeX;
    G4double fPSSizeY;
    G4double fPSSizeZ;
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
