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

#ifndef NuSDVCompositeDetConstruction_H
#define NuSDVCompositeDetConstruction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVDetConstruction.hh"
#include "G4ThreeVector.hh"
#include "NuSDDetType.hh"

class NuSDVCompositeDetMessenger;
class NuSDNeutronSD;

class NuSDVCompositeDetConstruction : public NuSDVDetConstruction
{

  public:
    NuSDVCompositeDetConstruction(const G4String&name, NuSDDetType type);
    virtual ~NuSDVCompositeDetConstruction(); 
  
  public:    
    void ConstructSDs();    
    
  //UI commands   
  public:
    void SetNeutronScntThickness(G4double thickness);
    
         
  public: 
    G4int GetNumberOfNeutronScnt() const { return fPhysNdVector.size(); }
      
    G4double GetNeutronScntSizeX() const { return fNeutronScntSizeX; }
    G4double GetNeutronScntSizeY() const { return fNeutronScntSizeY; }
    G4double GetNeutronScntSizeZ() const { return fNeutronScntSizeZ; }
     
    G4double GetNeutronScntThick() const  { return fNeutronScntThick; } 
       
  private:
    NuSDVCompositeDetMessenger* fDetMessenger;
    G4Cache<NuSDNeutronSD*> fNeutronSD;
       
  protected:
    void CleanUpOldGeometry();
    void DefineSolidsAndLogics();
    void SetVisProp();
    void Print() const;     
              
  protected:
    std::vector<G4VPhysicalVolume*> fPhysNdVector;
    G4LogicalVolume *fNeutronScntLogic;
        
  
    G4double fNeutronScntSizeX;
    G4double fNeutronScntSizeY;
    G4double fNeutronScntSizeZ;

    G4double fNeutronScntThick;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
