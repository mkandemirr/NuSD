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

#ifndef NuSDFiberProperties_H
#define NuSDFiberProperties_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class NuSDFiberPropertiesMessenger;

class NuSDFiberProperties
{

  public:
    NuSDFiberProperties(G4double coreSizeX,
                        G4double coreSizeY,
                        G4double coreSizeZ,
                        G4double clad1Thickness,
                        G4double clad2Thickness,
                        G4double gapThickness,
                        G4double grooveDistToWall,
                        G4double distBeweenGrooves
                        );
                             
    virtual ~NuSDFiberProperties();
  
    
  public:
  //Fiber UI comand
    void SetCoreSizeX(G4double x);
    void SetCoreSizeY(G4double y);
    void SetCoreSizeZ(G4double z);
    void SetClad1Thickness(G4double thick);
    void SetClad2Thickness(G4double thick);
    void SetGapThickness(G4double thick);
    void SetGrooveDistanceToWall(G4double dist);
    void SetDistanceBetweenGrooves(G4double dist);
    
  //Getters  
  public: 
    G4double GetCoreSizeX()        const { return fCoreSizeX; }
    G4double GetCoreSizeY()        const { return fCoreSizeY; }
    G4double GetCoreSizeZ()        const { return fCoreSizeZ; }
    
    G4double GetClad1Thickness()   const { return fClad1Thickness; }
    G4double GetClad2Thickness()   const { return fClad2Thickness; }
    G4double GetGapThickness()     const { return fGapThickness; }
    
    G4double GetGrooveDistanceToWall()    const { return fGrooveDistToWall; }
    G4double GetDistanceBetweenGrooves()  const { return fDistBetweenGrooves; } //AlongY
    G4double GetGrooveSizeX()      const;
    G4double GetGrooveSizeY()      const;
    G4double GetGrooveSizeZ()      const;
    
    
  private:
    NuSDFiberPropertiesMessenger  *fFiberPropertiesMessenger; 
    
    G4double fCoreSizeX;
    G4double fCoreSizeY;
    G4double fCoreSizeZ;
    G4double fClad1Thickness;
    G4double fClad2Thickness;
    G4double fGapThickness; 
    
    G4double fGrooveDistToWall;
    G4double fDistBetweenGrooves; //along Y axis
    
        
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
