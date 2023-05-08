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

#ifndef NuSDSolidDetConstruction_H
#define NuSDSolidDetConstruction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVCompositeDetConstruction.hh"

class NuSDSolidMessenger;
class G4VPhysicalVolume;
class NuSDSiPMProperties;
class NuSDFiberProperties;
class NuSDReflectorProperties;

class NuSDSolidDetConstruction : public NuSDVCompositeDetConstruction
{
  public:
    NuSDSolidDetConstruction();
    virtual ~NuSDSolidDetConstruction();
         
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
  
  //UI commands  
  public:  
    void SetReflectorThickness(G4double thickness);
    
  public:   
    void Print() const ;
      
  private: 
    void SetDefaults();
    void DefineDetCompDimensions();
    void DefineSolidsAndLogics();
    void ConstructSingleDetUnit(); 
    void ConstructMatrixDet();
    void AddOptReadoutUnit(const G4VPhysicalVolume *physUnit, G4String placedAxis);
    void SetVisProp();
  
  public:
    NuSDSiPMProperties*  GetSiPMProperties()  const {return fSiPMProperties;}
    NuSDFiberProperties* GetFiberProperties() const {return fFiberProperties;}
  
  private: //Members
    NuSDSolidMessenger* fDetMessenger;
    G4LogicalVolume *fReflectorLogic;

  private:
    G4double fReflectorSizeX;
    G4double fReflectorSizeY;
    G4double fReflectorSizeZ;
    
    G4double fReflectorThick;
    
    NuSDSiPMProperties*      fSiPMProperties;
    NuSDFiberProperties*     fFiberProperties;
    NuSDReflectorProperties* fReflectorProperties;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
