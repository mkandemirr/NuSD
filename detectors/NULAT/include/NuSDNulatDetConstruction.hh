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

#ifndef NuSDNulatDetConstruction_H
#define NuSDNulatDetConstruction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVDetConstruction.hh"

class NuSDNulatMessenger;
class G4VPhysicalVolume;
class NuSDPmtProperties;
class NuSDReflectorProperties;

class NuSDNulatDetConstruction : public NuSDVDetConstruction
{
  public:
    NuSDNulatDetConstruction();
    virtual ~NuSDNulatDetConstruction();
         
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
  
  //UI commands  
  public:  
    void SetContainerThickness(G4double thickness);
        
  public:   
    void Print() const;
    
  private: 
    void SetDefaults();
    void DefineDetCompDimensions();
    void DefineSolidsAndLogics();
    void ConstructSingleDetUnit(); 
    void ConstructMatrixDet();
    void AddOptReadoutUnit(const G4VPhysicalVolume *physUnit, G4String placedAxis);
    void SetVisProp();  
  
  public:
    NuSDPmtProperties* GetPmtProperties() const {return fPmtProperties;}  
    
  private: 
    NuSDNulatMessenger* fDetMessenger;
        
    G4LogicalVolume *fContainerLogic;
        
    G4double fContainerSizeX;
    G4double fContainerSizeY;
    G4double fContainerSizeZ;
    
    G4double fContainerThick;
    
    NuSDPmtProperties*       fPmtProperties;
    NuSDReflectorProperties* fReflectorProperties;
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
