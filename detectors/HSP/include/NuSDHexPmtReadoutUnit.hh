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

#ifndef NuSDHexPmtReadoutUnit_h
#define NuSDHexPmtReadoutUnit_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVOptReadoutUnit.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class NuSDHexPmtProperties;

class NuSDHexPmtReadoutUnit : public NuSDVOptReadoutUnit
{
  public:
    NuSDHexPmtReadoutUnit(G4RotationMatrix *pRot,
                                const G4ThreeVector &tlate,
                                G4LogicalVolume *pMotherLogical,
                                G4bool pMany,
                                G4int pCopyNo,
                                const G4String& placedAxis,
                                NuSDHexPmtProperties* hexPmtProperties
                                );

  virtual ~NuSDHexPmtReadoutUnit();
  
  protected:  
    void Construct() override;

  private: 
    void DefineOptReadoutCompDimensions();
    void ConstructComponents();
    void SetVisProp();  
  
  private:
    NuSDHexPmtProperties* fHexPmtProperties;
    
    G4RotationMatrix *fLgRot;

    G4VPhysicalVolume *fPhysLg;
    G4VPhysicalVolume *fPhysLgBinder;
    
    //position with respect to PmtVolume
    G4ThreeVector fLgPos;
    G4ThreeVector fLgBinderPos;
           
    G4double fLgBinderSizeZ;
    G4double fLgSizeZ;
    G4double fUnitSizeZ;
    
    G4double fLgTanDisToOuterSur;
    G4double fLgBinderTanDisToOuterSur;
    G4double fUnitTanDisToOuterSur;    
   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
