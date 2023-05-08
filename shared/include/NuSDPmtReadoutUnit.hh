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

#ifndef NuSDPmtReadoutUnit_h
#define NuSDPmtReadoutUnit_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVOptReadoutUnit.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class NuSDPmtProperties;

class NuSDPmtReadoutUnit : public NuSDVOptReadoutUnit
{
  public:
    NuSDPmtReadoutUnit(G4RotationMatrix *pRot,
                        const G4ThreeVector &tlate,
                        G4LogicalVolume *pMotherLogical,
                        G4bool pMany,
                        G4int pCopyNo,
                        const G4String& placedAxis,
                        NuSDPmtProperties* pmtProperties
                        );

    virtual ~NuSDPmtReadoutUnit();
    
  protected:  
    void Construct() override;

  private: 
    void DefineComponentsDimensions();
    void ConstructComponents();
    void SetVisProp();
    
      
  private:
    NuSDPmtProperties* fPmtProperties; 
    
    G4RotationMatrix *fLgRot;

    G4VPhysicalVolume *fPhysLg;
    G4VPhysicalVolume *fPhysLgBinder;
     
    G4ThreeVector fLgPos;
    G4ThreeVector fLgBinderPos;
    
    G4double fLgSizeX;
    G4double fLgSizeY;
    G4double fLgSizeZ;    
       
    G4double fLgBinderSizeX;
    G4double fLgBinderSizeY;
    G4double fLgBinderSizeZ;
    
    G4double fUnitSizeX;
    G4double fUnitSizeY;
    G4double fUnitSizeZ;

};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
