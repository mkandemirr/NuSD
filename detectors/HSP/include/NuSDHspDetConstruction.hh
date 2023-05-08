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

#ifndef NuSDHspDetConstruction_H
#define NuSDHspDetConstruction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVDetConstruction.hh"

class NuSDHspMessenger;
class G4VPhysicalVolume;
class NuSDHexPmtProperties;
class NuSDReflectorProperties;

class NuSDHspDetConstruction : public NuSDVDetConstruction
{
  public:
    NuSDHspDetConstruction();
    virtual ~NuSDHspDetConstruction();
         
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
  
  //UI commands  
  public:   
    void SetReflectorThickness(G4double thickness);
    void SetLiquidTankThickness(G4double thickness);
    void SetNuScntSideLength(G4double sl);
    
  public:   
    void Print() const ;
    G4double GetUnitTanDisToOuterSur()   const {return fUnitTanDisToOuterSur;}
    G4double GetNuScntTanDisToOuterSur() const {return fNuScntTanDisToOuterSur;}
        
  private: 
    void SetDefaults();
    void DefineDetCompDimensions();
    void DefineSolidsAndLogics();
    void ConstructSingleDetUnit(); 
    void ConstructMatrixDet();
    void AddOptReadoutUnit(const G4VPhysicalVolume *physUnit);
    void SetVisProp();
    
    void PlaceDetInWorld();

  public:
    NuSDHexPmtProperties* GetHexPmtProperties() const {return fHexPmtProperties;}
    
  private: 
    NuSDHspMessenger* fDetMessenger;
       
    G4LogicalVolume *fReflectorLogic;   
    G4LogicalVolume *fLiquidTankLogic;
    
    G4double fNuScntSideLength;
    G4double fNuScntTanDisToOuterSur;
    G4double fOptBarrierTanDisToOuterSur;
    G4double fReflectorTanDisToOuterSur;
    G4double fLiquidTankTanDisToOuterSur;
    G4double fUnitTanDisToOuterSur;
    G4double fDetTanDisToOuterSur;
    G4double fWorldTanDisToOuterSur;
    
    G4double fReflectorThick;
    G4double fLiquidTankThick;
    
    NuSDHexPmtProperties*       fHexPmtProperties;
    NuSDReflectorProperties*    fReflectorProperties;  
          
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
