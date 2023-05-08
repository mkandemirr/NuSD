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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"

#include "NuSDPandaDetConstruction.hh"
#include "NuSDPandaMessenger.hh"
#include "NuSDPmtProperties.hh"
#include "NuSDReflectorProperties.hh"
#include "NuSDPmtReadoutUnit.hh"
#include "NuSDReflectorSurface.hh"

#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4ApplicationState.hh"

//assert works if this opens
#undef NDEBUG
#include <assert.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPandaDetConstruction::NuSDPandaDetConstruction(): 
NuSDVDetConstruction("Panda", NuSDDetType::Inhomogeneous),
fDetMessenger{nullptr},
fReflectorLogic{nullptr}, 
fGdLogic{nullptr},
fReflectorSizeX{0.}, fReflectorSizeY{0.}, fReflectorSizeZ{0.}, 
fGdSizeX{0.}, fGdSizeY{0.}, fGdSizeZ{0.},
fReflectorThick{0.}, fGdThick{0.},
fPmtProperties{nullptr},
fReflectorProperties{nullptr}
{  
  SetDefaults();

  fDetMessenger = new NuSDPandaMessenger(this);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPandaDetConstruction::~NuSDPandaDetConstruction()
{
  if(fDetMessenger)        delete fDetMessenger; 
  if(fPmtProperties)       delete fPmtProperties;
  if(fReflectorProperties) delete fReflectorProperties;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* NuSDPandaDetConstruction::Construct()
{
   
  if(fPhysWorld) 
  {
    CleanUpOldGeometry();     
  }
  
  //set-up full detector step by step
  DefineDetCompDimensions();
  DefineSolidsAndLogics();
  ConstructSingleDetUnit();
  ConstructMatrixDet(); 
  SetVisProp();
  PlaceDetInWorld();

  return fPhysWorld;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPandaDetConstruction::ConstructSDandField() 
{ 
  NuSDVDetConstruction::ConstructSDs(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPandaDetConstruction::SetDefaults()
{
  
  //This initial values may be changed via user interface command with a macro file.  
  //cannot be zero!
  fNumberOfSegmentAlongX        = 10; //10
  fNumberOfSegmentAlongY        = 10; //10
  fNumberOfSegmentAlongZ        = 1;  // 1

  fNuScntSizeX                  = 10*cm;
  fNuScntSizeY                  = 10*cm;
  fNuScntSizeZ                  = 100*cm;
   
  fOptBarrierThick              = 0.3*mm; //air 0.3mm
  fReflectorThick               = 250*um; //0.25mm = 250 um
  fGdThick                      = 50*um; //50um=0.05*mm 
      
  fIsPSOn                       = true;
          
  G4bool    isPSAttachedBothEnd = true;
  G4bool    isPSBinderOn        = true;
  G4double  psBinderLength      = 1*mm; 
  
  G4double psQE                 = 1.0;
  G4bool isQEDependsenergy      = false;    

  G4double  psRadius            = 2.3*cm; //4.6 cm photocathode diameter

  G4double  isLgOn              = true;

  G4double  lgPmtEndSizeX       = 10*cm;
  G4double  lgPmtEndSizeY       = 10*cm;
  G4double  lgPmtEndSizeZ       = 10*cm;
  G4double  lgLength            = 4*cm;

  G4bool    isLgBinderOn        = true;
  G4double  lgBinderLength      = 1*mm; 
   
  fPmtProperties  = new NuSDPmtProperties(isPSAttachedBothEnd, 
                        isPSBinderOn,
                        psBinderLength,
                        psQE,
                        isQEDependsenergy,
                        psRadius,
                        isLgOn,
                        lgPmtEndSizeX,
                        lgPmtEndSizeY,
                        lgPmtEndSizeZ,
                        lgLength,
                        isLgBinderOn,
                        lgBinderLength
                        );
 
 //Reflector properties
 G4double reflectivity              = 0.96;
 G4bool isReflectivityDependsEnergy = false;  //if it is true, 
                                             //fill data/surface/Reflector_spect.txt file
                        
 fReflectorProperties = new NuSDReflectorProperties(reflectivity,
                            isReflectivityDependsEnergy);                        
        
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPandaDetConstruction::DefineDetCompDimensions()
{
  
  NuSDVDetConstruction::CheckNumberOfSegment();
  
  fOptBarrierSizeX    = fNuScntSizeX + 2*fOptBarrierThick;
  fOptBarrierSizeY    = fNuScntSizeY + 2*fOptBarrierThick;
  fOptBarrierSizeZ    = fNuScntSizeZ;

  fReflectorSizeX     = fOptBarrierSizeX + 2*fReflectorThick;
  fReflectorSizeY     = fOptBarrierSizeY + 2*fReflectorThick;
  fReflectorSizeZ     = fNuScntSizeZ;

  fGdSizeX            = fReflectorSizeX + 2*fGdThick;
  fGdSizeY            = fReflectorSizeY + 2*fGdThick;
  fGdSizeZ            = fNuScntSizeZ;

  G4double distGdToUnit{0*cm};

  fUnitSizeX          = fGdSizeX + distGdToUnit;
  fUnitSizeY          = fGdSizeY + distGdToUnit;
  fUnitSizeZ          = fGdSizeZ + distGdToUnit;

  G4double gap{3.*cm};
  
  G4double pmtLength  = (fIsPSOn) ? fPmtProperties->GetPmtReadoutUnitLength() : 0.;

  fDetSizeX           = fNumberOfSegmentAlongX*fUnitSizeX + gap;
  fDetSizeY           = fNumberOfSegmentAlongY*fUnitSizeY + gap;
  fDetSizeZ           = fNumberOfSegmentAlongZ*fUnitSizeZ + 2*pmtLength + gap;
  
  fWorldSizeX         = fDetSizeX + gap;
  fWorldSizeY         = fDetSizeY + gap;
  fWorldSizeZ         = fDetSizeZ + gap;
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPandaDetConstruction::DefineSolidsAndLogics()
{  
  NuSDVDetConstruction::DefineSolidsAndLogics();
  
  G4Box* nuScntSolid      = new G4Box("NuScntSolid", 0.5*fNuScntSizeX, 0.5*fNuScntSizeY, 0.5*fNuScntSizeZ);
  fNuScntLogic            = new G4LogicalVolume(nuScntSolid, GetMaterial("EJ-200",true) , "NuScntLogic", 0, 0, 0);

  G4Box* optBarrierSolid  = new G4Box("OptBarrierSolid", 0.5*fOptBarrierSizeX, 0.5*fOptBarrierSizeY,0.5*fOptBarrierSizeZ);              
  fOptBarrierLogic        = new G4LogicalVolume(optBarrierSolid, GetMaterial("Air",true),"OptBarrierLogic",0,0,0);
  
  G4Box *reflectorSolid   = new G4Box("ReflectorSolid", 0.5*fReflectorSizeX, 0.5*fReflectorSizeY, 0.5*fReflectorSizeZ);
  fReflectorLogic         = new G4LogicalVolume(reflectorSolid, GetMaterial("AluminizedMylar"),"ReflectorLogic",0,0,0);
         
  G4Box *gdSolid          = new G4Box("GdSolid", 0.5*fGdSizeX, 0.5*fGdSizeY, 0.5*fGdSizeZ);
  fGdLogic                = new G4LogicalVolume(gdSolid, GetMaterial("GdCoating"), "GdLogic",0,0,0);
  
  //skin reflector logic
  if(!fIsPSOn) return;
  
  fReflectorSurface  =  new NuSDReflectorSurface("ReflectorName", //name
                            unified,                //model
                            polishedfrontpainted,  // surfaceFinish 
                            dielectric_dielectric, //type
                            fReflectorProperties
                            ); 

  new G4LogicalSkinSurface("SkinSurface", fReflectorLogic , fReflectorSurface );
 
  if(!fPmtProperties->IsAttachedBothEnd() )
  {
    new G4LogicalSkinSurface("SkinSurface", fDetLogic , fReflectorSurface );  
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPandaDetConstruction::ConstructSingleDetUnit()
{

  //Place NuScnt in OptBarrier
  new G4PVPlacement(0,                     //no rotation
      G4ThreeVector(0,0,0),       //at (0,0,0)
      fNuScntLogic,         //its logical volume
      "NuScntPV",               //its name
      fOptBarrierLogic,                     //its mother  volume
      false,                 //no boolean operation
      0,                     //copy number
      fCheckOverlaps);        //overlaps checking 
     
     
  //Place OptBarrier in Reflector
  new G4PVPlacement(0,                     //no rotation
      G4ThreeVector(0,0,0),       //at (0,0,0)
      fOptBarrierLogic,         //its logical volume
      "OptBarrierPV",               //its name
      fReflectorLogic,                     //its mother  volume
      false,                 //no boolean operation
      1,                     //copy number
      fCheckOverlaps);        //overlaps checking     


  //place Reflector in Gd
  new G4PVPlacement(0,                     //no rotation
      G4ThreeVector(0,0,0),       //at (0,0,0)
      fReflectorLogic,         //its logical volume
      "ReflectorPV",               //its name
      fGdLogic,                     //its mother  volume
      false,                 //no boolean operation
      2,                     //copy number
      fCheckOverlaps);        //overlaps checking 
     
     
  //place Gd in Unit
  new G4PVPlacement(0,                     //no rotation
      G4ThreeVector(0,0,0),       //at (0,0,0)
      fGdLogic,         //its logical volume
      "GdPV",               //dont change it!!look at stepping action
      fUnitLogic,                     //its mother  volume
      false,                 //no boolean operation
      3,                     //copy number
      fCheckOverlaps);        //overlaps checking

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//---------Common for 1d Light collection detector------------------------
void NuSDPandaDetConstruction::ConstructMatrixDet()
{
  
  G4double x{}, y{}, z{};  //unit center position coordinates with respect to its mother. 

  //----------Place Units in Panda Detector--------------------    	 

  for(G4int i=0; i<fNumberOfSegmentAlongY; ++i)
  { 

    y =  -fUnitSizeY*(fNumberOfSegmentAlongY-1)*0.5 + i*fUnitSizeY;

    //xz layer       
    for(G4int j =0; j<fNumberOfSegmentAlongX; ++j)
    {

      x = -fUnitSizeX*(fNumberOfSegmentAlongX-1)*0.5 + j*fUnitSizeX;

      for(G4int k =0; k<fNumberOfSegmentAlongZ; ++k)
      {      

        z = -fUnitSizeZ*(fNumberOfSegmentAlongZ-1)*0.5 + k*fUnitSizeZ; 

        G4VPhysicalVolume* physUnit = new G4PVPlacement(0, 
                                          G4ThreeVector(x,y,z), 
                                          fUnitLogic, 
                                          "UnitPV", 
                                          fDetLogic, 
                                          false, 
                                          fPhysUnitVector.size(), //copyNo 
                                          fCheckOverlaps);        

        fPhysUnitVector.push_back(physUnit);

        if( fIsPSOn && k==0 )  //z axis    
        {
          AddOptReadoutUnit( physUnit );  
        }


      } //k loop


    }//j loop

  }//i loop


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//---------Common for 1d Light collection detector------------------------
void NuSDPandaDetConstruction::AddOptReadoutUnit(const G4VPhysicalVolume *physUnit)
{
  
  assert(physUnit);

  G4VPhysicalVolume* physOptReadoutUnit{nullptr};
  
  G4ThreeVector pmtPosition( physUnit->GetTranslation().getX(), physUnit->GetTranslation().getY(),
                             physUnit->GetTranslation().getZ()-fUnitSizeZ*0.5-
                             fPmtProperties->GetPmtReadoutUnitLength()*0.5 );
     
  physOptReadoutUnit =  new  NuSDPmtReadoutUnit( 0, 
                             pmtPosition, 
                             fDetLogic, 
                             false,  
                             fPhysOptReadoutUnitVec.size(), //copyNo                             
                             "-z",
                             fPmtProperties 
                             ); 

  fPhysOptReadoutUnitVec.push_back(physOptReadoutUnit);

  
  if( fPmtProperties->IsAttachedBothEnd() )
  {
  
    pmtPosition.setZ(-pmtPosition.getZ());
    //+z axis
    physOptReadoutUnit = new  NuSDPmtReadoutUnit(0, 
                              pmtPosition, 
                              fDetLogic, 
                              false,  
                              fPhysOptReadoutUnitVec.size(), //copyNo
                              "z",
                              fPmtProperties 
                              ); 

    fPhysOptReadoutUnitVec.push_back(physOptReadoutUnit);	
 
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPandaDetConstruction::SetVisProp()
{
    
  NuSDVDetConstruction::SetVisProp();
  
  G4VisAttributes* visAttributes{nullptr};
  
  //Reflector
  visAttributes = new G4VisAttributes( G4Colour::Yellow());
  visAttributes->SetForceSolid(false);
  fReflectorLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  //Gd
  visAttributes = new G4VisAttributes( G4Colour::Brown());
  visAttributes->SetForceSolid(false);
  fGdLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//invoked by EndOfRunAction method of NuSDRunAction class
void NuSDPandaDetConstruction::Print() const 
{  
  NuSDVDetConstruction::Print();
  
  G4cout<<"Reflector thickness (um) : "<<fReflectorThick*(1/um)<<G4endl;
  G4cout<<"Gd thickness (um)        : "<<fGdThick*(1/um)<<G4endl;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//----------------UI Commands------------------------------------------------------

void NuSDPandaDetConstruction::SetReflectorThickness(G4double thickness)
{
  fReflectorThick = thickness;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    { 
      G4RunManager::GetRunManager()->ReinitializeGeometry();
    }
}

void NuSDPandaDetConstruction::SetGdThickness(G4double thickness)
{
  fGdThick = thickness;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
  {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

