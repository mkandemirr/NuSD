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

#ifndef NuSDReflectorProperties_H
#define NuSDReflectorProperties_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class NuSDReflectorPropertiesMessenger;

class NuSDReflectorProperties
{
  public:
    NuSDReflectorProperties(G4double reflectivity, 
                            G4bool isReflectivityDependsEnergy
                            ); 
    virtual ~NuSDReflectorProperties();
     
  public:
  //UI commands  
    void SetReflectivity(G4double eff);
    void SetReflectivityDependsEnergy(G4bool b);
    
  //Getters  
  public: 
    G4double GetReflectivity()               const { return fReflectivity; }
    G4bool   IsReflectivityDependsEnergy()   const { return fIsReflectivityDependsEnergy; } 
    
  private:
    NuSDReflectorPropertiesMessenger  *fReflectorPropertiesMessenger; 
              
    G4double  fReflectivity;  //teoric  
    G4bool    fIsReflectivityDependsEnergy; 
           
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
