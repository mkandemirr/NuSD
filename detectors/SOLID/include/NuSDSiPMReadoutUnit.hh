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

#ifndef NuSDSiPMReadoutUnit_h
#define NuSDSiPMReadoutUnit_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVOptReadoutUnit.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class NuSDSiPMProperties;

class NuSDSiPMReadoutUnit : public NuSDVOptReadoutUnit
{
  public:
    NuSDSiPMReadoutUnit(G4RotationMatrix *pRot,
                        const G4ThreeVector &tlate,
                        G4LogicalVolume *pMotherLogical,
                        G4bool pMany,
                        G4int pCopyNo,
                        const G4String& placedAxis,
                        NuSDSiPMProperties* siPMProperties
                        );
                        
    virtual ~NuSDSiPMReadoutUnit();


  public:
    static G4double GetLength();
    static void SetDefaults();  
    
  protected:
    void Construct() override;

  private:
    void DefineComponentsDimensions();
    void ConstructComponents();
    void SetVisProp();
       
  private:
    NuSDSiPMProperties* fSiPMProperties;
      
    G4double fPSBinderSizeX;
    G4double fPSBinderSizeY;
    G4double fPSBinderSizeZ;

    G4double fPSSizeX;
    G4double fPSSizeY;
    G4double fPSSizeZ;  
    
    G4double fUnitSizeX;
    G4double fUnitSizeY;
    G4double fUnitSizeZ; 
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
