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

#ifndef NuSDProspectDetConstruction_H
#define NuSDProspectDetConstruction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVDetConstruction.hh"

class NuSDProspectMessenger;
class G4VPhysicalVolume;
class NuSDPmtProperties;
class NuSDReflectorProperties;

class NuSDProspectDetConstruction : public NuSDVDetConstruction
{
  public:
    NuSDProspectDetConstruction();
    virtual ~NuSDProspectDetConstruction();
         
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
  
  //UI commands  
  public:  
    void SetReflectorThickness(G4double thickness);
    void SetLiquidTankThickness(G4double thickness);
    
  public:   
    void Print() const ;
    
  private: 
    void SetDefaults();
    void DefineDetCompDimensions();
    void DefineSolidsAndLogics();
    void ConstructSingleDetUnit(); 
    void ConstructMatrixDet();
    void AddOptReadoutUnit(const G4VPhysicalVolume *physUnit);
    void SetVisProp();

  public:
    NuSDPmtProperties* GetPmtProperties() const {return fPmtProperties;}

  private: 
    NuSDProspectMessenger* fDetMessenger;
        
    G4LogicalVolume *fReflectorLogic;
    G4LogicalVolume *fLiquidTankLogic;
         
    G4double fReflectorSizeX;
    G4double fReflectorSizeY;
    G4double fReflectorSizeZ;
    
    G4double fLiquidTankSizeX;
    G4double fLiquidTankSizeY;
    G4double fLiquidTankSizeZ;
      
    G4double fReflectorThick;
    G4double fLiquidTankThick;
    
    NuSDPmtProperties*       fPmtProperties;
    NuSDReflectorProperties* fReflectorProperties;
    
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
