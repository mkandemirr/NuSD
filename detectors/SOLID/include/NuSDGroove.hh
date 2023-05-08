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

#ifndef NuSDGroove_H
#define NuSDGroove_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4PVPlacement.hh"

class G4LogicalVolume;
class G4Material;
class NuSDSolidDetConstruction;

class NuSDGroove : public G4PVPlacement
{
  public:
    NuSDGroove(G4RotationMatrix *pRot,
               const G4ThreeVector &tlate,
               G4LogicalVolume *pMotherLogical,
               G4bool pMany,
               G4int pCopyNo,
               char axis,
               NuSDSolidDetConstruction* dc
               );

  private:
    void Construct();
    void DefineComponentsDimensions();
    void ConstructComponens();
    void SetVisProp();
    
    G4Material* GetMaterial(const G4String& name, G4bool enableOpticalProperty=false);
   
  private:  
    G4ThreeVector fGroovePosVec; //wrt to mother neutrinoScnt
    char fPlacedAxis;
     
    G4LogicalVolume* fCoreLogic;
    G4LogicalVolume* fClad1Logic;
    G4LogicalVolume* fClad2Logic; 
    G4LogicalVolume* fGrooveLogic;

    G4double fGrooveSizeX;
    G4double fGrooveSizeY;
    G4double fGrooveSizeZ;

    G4double fCoreSizeX;
    G4double fCoreSizeY;
    G4double fCoreSizeZ;

    G4double fClad1SizeX;
    G4double fClad1SizeY;
    G4double fClad1SizeZ;

    G4double fClad2SizeX;
    G4double fClad2SizeY;
    G4double fClad2SizeZ;

    NuSDSolidDetConstruction* fSolidDet;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
