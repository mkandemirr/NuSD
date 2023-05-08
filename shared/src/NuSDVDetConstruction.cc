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

#include "NuSDVDetConstruction.hh"
#include "NuSDVDetMessenger.hh"
#include "NuSDMaterialFactory.hh"
#include "NuSDNeutrinoSD.hh"
#include "NuSDPhotonSD.hh"

#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4SDManager.hh"

//for cleanup old geometry
#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalSkinSurface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVDetConstruction::NuSDVDetConstruction(const G4String& detName, NuSDDetType detType): 
G4VUserDetectorConstruction(),
fDetMessenger{nullptr},  
fDetName{detName},
fDetType{detType},
fReflectorSurface{nullptr},
fPhysUnitVector(), 
fPhysOptReadoutUnitVec(),
fVisAttributes(),
fNuScntLogic{nullptr}, fOptBarrierLogic{nullptr}, fUnitLogic{nullptr}, fDetLogic{nullptr}, 
fPhysDet{nullptr}, fPhysWorld{nullptr},
fCheckOverlaps{true},
fNumberOfSegmentAlongX{0}, fNumberOfSegmentAlongY{0}, fNumberOfSegmentAlongZ{0},
fNuScntSizeX{0.}, fNuScntSizeY{0.}, fNuScntSizeZ{0.},
fOptBarrierSizeX{0.}, fOptBarrierSizeY{0.}, fOptBarrierSizeZ{0.}, fOptBarrierThick{0.},
fIsPSOn{false},
fUnitSizeX{0.}, fUnitSizeY{0.}, fUnitSizeZ{0.},
fDetSizeX{0.}, fDetSizeY{0.}, fDetSizeZ{0.},
fWorldSizeX{0.}, fWorldSizeY{0.}, fWorldSizeZ{0.}

{
  fDetMessenger = new NuSDVDetMessenger(this);   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVDetConstruction::~NuSDVDetConstruction()
{
  delete fDetMessenger;
   
  CleanVisAttributesVec();
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::CleanVisAttributesVec()
{
  for (auto visAttributes: fVisAttributes) 
  {
    delete visAttributes;
  } 
  fVisAttributes.clear();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* NuSDVDetConstruction::GetMaterial(const G4String& name, G4bool enableOpticalProperty) 
{
  G4Material* material = NuSDMaterialFactory::GetInstance()->GetMaterial(name,enableOpticalProperty);
  return material;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::CleanUpOldGeometry()
{ 
  G4GeometryManager::GetInstance()->OpenGeometry();

  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  CleanVisAttributesVec();
  //G4OpticalSurface::CleanSurfacePropertyTable();
   
  fPhysUnitVector.clear();
  fPhysOptReadoutUnitVec.clear();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::ConstructSDs() 
{
  //---------NeutrinoSD--------------  
  if(!fNeutrinoSD.Get())
  {
    NuSDNeutrinoSD *mainSD = new NuSDNeutrinoSD("NeutrinoSD");       
    G4SDManager::GetSDMpointer()->AddNewDetector(mainSD);
    fNeutrinoSD.Put(mainSD);
    
  }
  
  SetSensitiveDetector("NuScntLogic", fNeutrinoSD.Get(), false);
  //SetSensitiveDetector(fNuScntLogic,fNeutrinoSD.Get());
  
  //---------PhotonSD--------------
  if(fIsPSOn) 
  { 
      
    if(!fPhotonSD.Get())
    {    
      NuSDPhotonSD *photonSD = new NuSDPhotonSD("PhotonSD");
      G4SDManager::GetSDMpointer()->AddNewDetector(photonSD); 
      fPhotonSD.Put(photonSD);      
    
    }
  
  }
  
  //is is trigered manually in stepping action 
  //SetSensitiveDetector("PhotonDetLogic", fPhotonSD.Get(),true);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::CheckNumberOfSegment()
{
  
  if( fNumberOfSegmentAlongX == 0 || fNumberOfSegmentAlongY == 0 || fNumberOfSegmentAlongZ ==0 )
    { G4Exception("NuSDVDetConstruction::DefineDetCompDimensions())", "Err0", FatalException, 
    "Number of segment cant be zero!!"); }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::DefineSolidsAndLogics()
{

  G4Box* unitSolid        = new G4Box("UnitSolid", 0.5*fUnitSizeX, 0.5*fUnitSizeY, 0.5*fUnitSizeZ);
  fUnitLogic              = new G4LogicalVolume(unitSolid, GetMaterial("Air",true) , "UnitLogic", 0, 0, 0);

  G4Box *detSolid         = new G4Box("DetSolid", 0.5*fDetSizeX, 0.5*fDetSizeY, 0.5*fDetSizeZ);
  fDetLogic               = new G4LogicalVolume(detSolid, GetMaterial("Air",true), "DetLogic", 0, 0, 0);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::SetVisProp()
{
 
  G4VisAttributes* visAttributes{nullptr};
  
  //NuScnt
  visAttributes = new G4VisAttributes( G4Colour::Blue());
  visAttributes->SetForceSolid(true);
  fNuScntLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
  //OptBarrier  
  visAttributes = new G4VisAttributes( G4Colour::White() );
  visAttributes->SetForceSolid(false);
  fOptBarrierLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
  //Unit
  visAttributes = new G4VisAttributes( G4Colour::White() );
  visAttributes->SetForceSolid(false);
  fUnitLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  //Full detector  
  visAttributes = new G4VisAttributes( G4Colour::White() );
  visAttributes->SetForceSolid(false);
  fDetLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::Print() const 
{ 
  G4cout<<"-----------------Detector properties------------------- "<<G4endl;
  G4cout<<"Detector Name                     : "<<fDetName<<G4endl; 
  G4cout<<"Detector Type                     : "<<static_cast<G4int>(fDetType)<<G4endl; 
  G4cout<<"Number of segment along x         : "<<fNumberOfSegmentAlongX<<G4endl;
  G4cout<<"Number of segment along y         : "<<fNumberOfSegmentAlongY<<G4endl;
  G4cout<<"Number of segment along z         : "<<fNumberOfSegmentAlongZ<<G4endl;
  G4cout<<"Neutrino scintillator size x (cm) : "<<fNuScntSizeX*(1/cm)<<G4endl;
  G4cout<<"Neutrino scintillator size y (cm) : "<<fNuScntSizeY*(1/cm)<<G4endl;
  G4cout<<"Neutrino scintillator size z (cm) : "<<fNuScntSizeZ*(1/cm)<<G4endl;
  G4cout<<"OptBarrier thickness (um)         : "<<fOptBarrierThick*(1/um)<<G4endl;
  G4cout<<"Active vol(not include Nd)        : "<<GetNumberOfUnit()*(fNuScntSizeX*fNuScntSizeY*fNuScntSizeZ)*
                                              (1./m3)<<"m3"<<G4endl;
  G4cout<<"Number of unit                    : "<<GetNumberOfUnit()<<G4endl;
  G4cout<<"Number of PMT                     : "<<fPhysOptReadoutUnitVec.size()<<G4endl;
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::PlaceDetInWorld()
{
  //World
  G4Box *worldSolid               = new G4Box("WorldSolid", 0.5*fWorldSizeX, 0.5*fWorldSizeY, 0.5*fWorldSizeZ);
  G4LogicalVolume *worldLogic     = new G4LogicalVolume(worldSolid, GetMaterial("Air",true), "WorldLogic");
   
  //Place world
  fPhysWorld                      = new G4PVPlacement(0, G4ThreeVector(), worldLogic, "WorldPV", 0, false, 0); 
   
  
  fPhysDet                        =  new G4PVPlacement(0,                     //no rotation
                                         G4ThreeVector(0,0,0),       //at (0,0,0)
                                         fDetLogic,         //its logical volume
                                         "DetPV",               //its name
                                         worldLogic,                     //its mother  volume
                                         false,                 //no boolean operation
                                         0,                     //copy number
                                         fCheckOverlaps);        //overlaps checking 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//--------------------USER INTERFACE COMMAND------------------------
void NuSDVDetConstruction::SetNumberOfSegmentAlongX(G4int x)
{
  
  fNumberOfSegmentAlongX = x;  
 
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();

  //G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  //G4RunManager::GetRunManager()->GeometryHasBeenModified();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::SetNumberOfSegmentAlongY(G4int y)
{
  fNumberOfSegmentAlongY = y;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
   G4RunManager::GetRunManager()->ReinitializeGeometry();
     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::SetNumberOfSegmentAlongZ(G4int z)
{
  fNumberOfSegmentAlongZ = z;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
   G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::SetNuScntDimensions(G4ThreeVector dimension)
{
  
  fNuScntSizeX = dimension.getX();
  fNuScntSizeY = dimension.getY();
  fNuScntSizeZ = dimension.getZ();
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
   G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::SetOptBarrierThickness(G4double thickness)
{
  fOptBarrierThick = thickness;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
   G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVDetConstruction::SetOnFlag(G4bool bl) 
{ 
  fIsPSOn= bl; 
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
  {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
    
}


