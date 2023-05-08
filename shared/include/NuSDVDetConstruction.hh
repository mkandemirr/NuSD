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

#ifndef NuSDVDetConstruction_H
#define NuSDVDetConstruction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

#include "G4Cache.hh"
#include "NuSDDetType.hh"

#include "G4OpticalSurface.hh"

class NuSDVDetMessenger;
class G4VisAttributes;
class G4OpticalSurface;
class G4Material;
class NuSDNeutrinoSD;
class NuSDPhotonSD;
class NuSDReflectorProperties;

class NuSDVDetConstruction : public G4VUserDetectorConstruction
{

  public:
    NuSDVDetConstruction(const G4String&name, NuSDDetType type);
    virtual ~NuSDVDetConstruction();
  
  public:    
    void ConstructSDs(); 
     
  //UI commands  
  public:
    void SetNumberOfSegmentAlongX(G4int x);
    void SetNumberOfSegmentAlongY(G4int y);
    void SetNumberOfSegmentAlongZ(G4int z);
    void SetNuScntDimensions(G4ThreeVector dimension); 
    void SetOptBarrierThickness(G4double thickness); 
    void SetOnFlag(G4bool bl);
  
  //Getters
  public:
    G4bool  IsPSOn() const { return fIsPSOn; }
    G4OpticalSurface* GetReflectorSurface() const { return fReflectorSurface; }
              
    const G4String& GetName() const { return fDetName; }
    
    std::vector<G4VPhysicalVolume*>   GetUnitPVVector()      const   { return fPhysUnitVector; }
    std::vector<G4VPhysicalVolume*>   GetOptReadoutUnitVec() const   { return fPhysOptReadoutUnitVec; }  
    std::vector<G4VisAttributes*>     GetVisVec()        const   { return fVisAttributes; }      

    G4LogicalVolume* GetLV()   const  { return fDetLogic; }  
    G4VPhysicalVolume* GetPV() const  {return fPhysDet;}  
   
    G4int GetNumberOfUnit() const    { return fPhysUnitVector.size(); }
    G4int GetNumberOfOptReadoutUnit() const    { return fPhysOptReadoutUnitVec.size(); }  
      
    G4double GetNuScntSizeX() const   { return fNuScntSizeX; }
    G4double GetNuScntSizeY() const   { return fNuScntSizeY; }
    G4double GetNuScntSizeZ() const   { return fNuScntSizeZ; }
    
    G4double GetOptBarrierSizeX()  const  { return fOptBarrierSizeX; }
    G4double GetOptBarrierSizeY()  const  { return fOptBarrierSizeY; }
    G4double GetOptBarrierSizeZ()  const  { return fOptBarrierSizeZ; } 
     
    G4double GetUnitSizeX() const   { return fUnitSizeX; }
    G4double GetUnitSizeY() const   { return fUnitSizeY; }
    G4double GetUnitSizeZ() const   { return fUnitSizeZ; }
    
    //full Detector
    G4double GetDetSizeX() const  { return fDetSizeX; }
    G4double GetDetSizeY() const  { return fDetSizeY; }
    G4double GetDetSizeZ() const  { return fDetSizeZ; }  
        
  protected:
    G4Material* GetMaterial(const G4String& name, G4bool enableOpticalProperty=false);
    
    void CleanUpOldGeometry();
    void CheckNumberOfSegment();
    void DefineSolidsAndLogics();
    void SetVisProp();
    void PlaceDetInWorld();
    void Print() const;
    
    void CleanVisAttributesVec();
    
  private:
    NuSDVDetMessenger *fDetMessenger; 
     
  protected:
    G4String    fDetName;
    NuSDDetType fDetType;
       
    G4OpticalSurface* fReflectorSurface;
    
    G4Cache<NuSDNeutrinoSD*>  fNeutrinoSD;
    G4Cache<NuSDPhotonSD*>    fPhotonSD;
    
    std::vector<G4VPhysicalVolume*> fPhysUnitVector;
    std::vector<G4VPhysicalVolume*> fPhysOptReadoutUnitVec;
    std::vector<G4VisAttributes*>   fVisAttributes;

    G4LogicalVolume   *fNuScntLogic;
    G4LogicalVolume   *fOptBarrierLogic;
    G4LogicalVolume   *fUnitLogic;
    G4LogicalVolume   *fDetLogic;
     
    G4VPhysicalVolume *fPhysDet;
    G4VPhysicalVolume *fPhysWorld;
    
    G4bool   fCheckOverlaps;
     
    //-----UI commands--------   
    G4int    fNumberOfSegmentAlongX;
    G4int    fNumberOfSegmentAlongY;
    G4int    fNumberOfSegmentAlongZ; 
    
    G4double fNuScntSizeX;
    G4double fNuScntSizeY;
    G4double fNuScntSizeZ; 
    
    G4double fOptBarrierSizeX;
    G4double fOptBarrierSizeY;
    G4double fOptBarrierSizeZ;
    
    G4double fOptBarrierThick;
      
    G4bool   fIsPSOn;  
    //UI---End---------------
     
    G4double fUnitSizeX;
    G4double fUnitSizeY;
    G4double fUnitSizeZ;
    
    G4double fDetSizeX;
    G4double fDetSizeY;
    G4double fDetSizeZ;

    G4double fWorldSizeX;
    G4double fWorldSizeY;
    G4double fWorldSizeZ;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
