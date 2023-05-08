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

#ifndef NuSDFileManager_h
#define NuSDFileManager_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "globals.hh"
#include <vector>


//singleton class
class NuSDFileManager
{
  public:
    virtual ~NuSDFileManager();
    
    static NuSDFileManager* GetInstance();

  public:
    void ReadSpectrumFromFile(const G4String &filename, std::vector<G4double> &quantity);
    void ReadSpectrumFromFile(const G4String &filename, std::vector<G4double> &quantity, G4double unit);
    void ReadSpectrumFromFile(const G4String &filename, 
                              std::vector<G4double> &photonEnergy, 
                              std::vector<G4double> &value, 
                              G4double unit1=1., 
                              G4double unit2=1.
                              );

//    void ReadSpectrumFromFile(const G4String &filename, std::vector<G4double> &fX, std::vector<G4double> &fY); 
	
  private:
    // Private constructor so that no objects can be created.
    NuSDFileManager();     
  
  private:
    static NuSDFileManager* fInstance;  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif 
