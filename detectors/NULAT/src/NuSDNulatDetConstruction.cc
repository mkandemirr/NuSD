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

#include "NuSDNulatDetConstruction.hh"
#include "NuSDNulatMessenger.hh"
#include "NuSDPmtProperties.hh"
#include "NuSDReflectorProperties.hh"
#include "NuSDPmtReadoutUnit.hh"
#include "NuSDReflectorSurface.hh"
#include "NuSDPhotoSensorSurface.hh"

#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4VSolid.hh"
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

NuSDNulatDetConstruction::NuSDNulatDetConstruction(): 
NuSDVDetConstruction("Nulat", NuSDDetType::Homogeneous),
fDetMessenger{nullptr},
fContainerLogic{nullptr}, 
fContainerSizeX{0.}, fContainerSizeY{0.}, fContainerSizeZ{0.}, 
fContainerThick{0.},
fPmtProperties{nullptr},
fReflectorProperties{nullptr}
{
  SetDefaults();
  
  fDetMessenger = new NuSDNulatMessenger(this);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDNulatDetConstruction::~NuSDNulatDetConstruction()
{
  if(fDetMessenger)        delete fDetMessenger; 
  if(fPmtProperties)       delete fPmtProperties;
  if(fReflectorProperties) delete fReflectorProperties;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNulatDetConstruction::SetDefaults()
{
  //This initial values may be changed via user interface command with a macro file.  
  fNumberOfSegmentAlongX        = 15;  //15
  fNumberOfSegmentAlongY        = 15;  //15
  fNumberOfSegmentAlongZ        = 15;  //15

  fNuScntSizeX                  = 6.3*cm; //6.3 cm
  fNuScntSizeY                  = 6.3*cm; //6.3 cm
  fNuScntSizeZ                  = 6.3*cm; //6.3 cm
   
  fOptBarrierThick              = 0.127*mm;  //0.127*mm. distance cell to cell is 0.254mm=0.01 inch
  fContainerThick               = 0.16*mm;  // 0.16*mm. acrylic container (1./16)*2.54 cm 
      
  fIsPSOn                       = false;
          
  G4bool isPSAttachedBothEnd    = true;
  G4bool  isPSBinderOn          = false;
  G4double  psBinderLength      = 1*mm; 
  
  G4double psQE                 = 1.0;
  G4bool isQEDependsenergy      = false;    

  G4double  psRadius            = 0.5*4.6*cm; //2-inch R10533 Hamamatsu PMT. Effective dia is 4.6cm

  G4double  isLgOn              = true;

  G4double  lgPmtEndSizeX       = 2*psRadius;
  G4double  lgPmtEndSizeY       = 2*psRadius;
  G4double  lgPmtEndSizeZ       = 2*psRadius;
  G4double  lgLength            = 4*cm;

  G4bool    isLgBinderOn        = false;
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

G4VPhysicalVolume* NuSDNulatDetConstruction::Construct()
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

void NuSDNulatDetConstruction::ConstructSDandField() 
{ 
  NuSDVDetConstruction::ConstructSDs();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNulatDetConstruction::DefineDetCompDimensions()
{
  
  NuSDVDetConstruction::CheckNumberOfSegment();
  
  fOptBarrierSizeX    =  fNuScntSizeX  + 2*fOptBarrierThick;
  fOptBarrierSizeY    =  fNuScntSizeY  + 2*fOptBarrierThick;
  fOptBarrierSizeZ    =  fNuScntSizeZ  + 2*fOptBarrierThick;

  fContainerSizeX     = fOptBarrierSizeX + 2*fContainerThick;
  fContainerSizeY     = fOptBarrierSizeY + 2*fContainerThick;
  fContainerSizeZ     = fOptBarrierSizeZ + 2*fContainerThick;

  G4double distOptBarrierToUnit{0.*mm}; //just for visualization
  
  fUnitSizeX          = fContainerSizeX + distOptBarrierToUnit;
  fUnitSizeY          = fContainerSizeY + distOptBarrierToUnit;
  fUnitSizeZ          = fContainerSizeZ + distOptBarrierToUnit;

  G4double gap{3.*cm};

  G4double length     = (fIsPSOn) ? fPmtProperties->GetPmtReadoutUnitLength() : 0.;

  fDetSizeX           = fNumberOfSegmentAlongX*fUnitSizeX + 2*length + gap;
  fDetSizeY           = fNumberOfSegmentAlongY*fUnitSizeY + 2*length + gap;
  fDetSizeZ           = fNumberOfSegmentAlongZ*fUnitSizeZ + 2*length + gap;
  
  fWorldSizeX         = fDetSizeX + gap;
  fWorldSizeY         = fDetSizeY + gap;
  fWorldSizeZ         = fDetSizeZ + gap;
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNulatDetConstruction::DefineSolidsAndLogics()
{

  NuSDVDetConstruction::DefineSolidsAndLogics();
  
  G4Box* nuScntSolid      = new G4Box("NuScntSolid", 0.5*fNuScntSizeX, 0.5*fNuScntSizeY, 0.5*fNuScntSizeZ);
  fNuScntLogic            = new G4LogicalVolume(nuScntSolid, GetMaterial("CustomEJ-254",true) , "NuScntLogic", 0, 0, 0);

  G4Box* optBarrierSolid  = new G4Box("OptBarrierSolid", 0.5*fOptBarrierSizeX, 0.5*fOptBarrierSizeY,0.5*fOptBarrierSizeZ);              
  fOptBarrierLogic        = new G4LogicalVolume(optBarrierSolid, GetMaterial("Air",true),"OptBarrierLogic",0,0,0);
  
  G4Box *containerSolid   = new G4Box("ContainerSolid", 0.5*fContainerSizeX, 0.5*fContainerSizeY, 0.5*fContainerSizeZ);
  fContainerLogic         = new G4LogicalVolume(containerSolid, GetMaterial("Pmma",true),"ContainerLogic", 0,0,0); 
  
  if(!fIsPSOn) return;
  
  fReflectorSurface  =  new NuSDReflectorSurface("ReflectorName", //name
                            unified,                //model
                            polishedfrontpainted,  // surfaceFinish 
                            dielectric_dielectric, //type
                            fReflectorProperties
                            );  
 
  new G4LogicalSkinSurface("SkinSurface", fDetLogic , fReflectorSurface );

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNulatDetConstruction::ConstructSingleDetUnit()
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
      
  //Place OptBarrier in Container
  new G4PVPlacement(0,                     //no rotation
      G4ThreeVector(0,0,0),       //at (0,0,0)
      fOptBarrierLogic,         //its logical volume
      "OptBarrierPV",               //its name
      fContainerLogic,                     //its mother  volume
      false,                 //no boolean operation
      0,                     //copy number
      fCheckOverlaps);        //overlaps checking     


  //place Container in Unit
  new G4PVPlacement(0,                     //no rotation
      G4ThreeVector(0,0,0),       //at (0,0,0)
      fContainerLogic,         //its logical volume
      "ContainerPV",               //its name
      fUnitLogic,                     //its mother  volume
      false,                 //no boolean operation
      1,                     //copy number
      fCheckOverlaps);        //overlaps checking 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNulatDetConstruction::ConstructMatrixDet()
{
  G4double x{}, y{}, z{};  //unit center position coordinates with respect to its mother. 

  //----------Place Unit in Nulat Detector--------------------    	 

  for(int i=0; i<fNumberOfSegmentAlongY; i++)
  { 

    y =  -fUnitSizeY*(fNumberOfSegmentAlongY-1)*0.5 + i*fUnitSizeY;

    //xz layer       
    for(int j =0; j<fNumberOfSegmentAlongX; j++)
    {

      x = -fUnitSizeX*(fNumberOfSegmentAlongX-1)*0.5 + j*fUnitSizeX;

      for(int k =0; k<fNumberOfSegmentAlongZ; k++)
      {      

        z = -fUnitSizeZ*(fNumberOfSegmentAlongZ-1)*0.5 + k*fUnitSizeZ; 

        G4VPhysicalVolume* physUnit = new G4PVPlacement(0, 
                                          G4ThreeVector(x,y,z), 
                                          fUnitLogic, 
                                          "UnitPV", 
                                          fDetLogic, 
                                          false, 
                                          fPhysUnitVector.size(), //copyNo 
                                          fCheckOverlaps); //overlaps checking 

        fPhysUnitVector.push_back(physUnit);

        if(fIsPSOn)     
        {
          if(i==0)
            AddOptReadoutUnit(physUnit,"-y");

          if(j==0)
            AddOptReadoutUnit(physUnit,"-x");

          if(k==0)
            AddOptReadoutUnit(physUnit,"-z");

        }   


      } //k loop


    }//j loop

  }//i loop


      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNulatDetConstruction::AddOptReadoutUnit(const G4VPhysicalVolume *physUnit, G4String placedAxis)
{

  assert(physUnit);

  G4VPhysicalVolume* physOptReadoutUnit{nullptr};

  G4ThreeVector pmtPosition(physUnit->GetTranslation().getX(), physUnit->GetTranslation().getY(), 
  physUnit->GetTranslation().getZ() );

  //update 
  if(placedAxis == "-y")
  { 

    pmtPosition.setY( physUnit->GetTranslation().getY()-fUnitSizeY*0.5-
    fPmtProperties->GetPmtReadoutUnitLength()*0.5 );

  }else if(placedAxis == "-x") 
  { 

    pmtPosition.setX( physUnit->GetTranslation().getX()-fUnitSizeX*0.5-
    fPmtProperties->GetPmtReadoutUnitLength()*0.5 );

  }else if(placedAxis == "-z") 
  { 

    pmtPosition.setZ( physUnit->GetTranslation().getZ()-fUnitSizeZ*0.5-
    fPmtProperties->GetPmtReadoutUnitLength()*0.5 );

  }

  physOptReadoutUnit = new  NuSDPmtReadoutUnit(0, 
                            pmtPosition, 
                            fDetLogic, 
                            false, 
                            fPhysOptReadoutUnitVec.size(), //copyNo
                            placedAxis,
                            fPmtProperties 
                            ); 
       
  fPhysOptReadoutUnitVec.push_back(physOptReadoutUnit);


  if( fPmtProperties->IsAttachedBothEnd() ) 
  {
    placedAxis.erase(0,1);

    if(placedAxis == "y")              
      pmtPosition.setY(-pmtPosition.getY());
    else if(placedAxis == "x")
      pmtPosition.setX(-pmtPosition.getX());
    else if(placedAxis == "z") 				 
      pmtPosition.setZ(-pmtPosition.getZ() ); 				 
			                
    physOptReadoutUnit =  new NuSDPmtReadoutUnit(0, 
                              pmtPosition,  
                              fDetLogic, 
                              false, 
                              fPhysOptReadoutUnitVec.size(), //copyNo
                              placedAxis,
                              fPmtProperties 
                              ); 

    fPhysOptReadoutUnitVec.push_back(physOptReadoutUnit);
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDNulatDetConstruction::SetVisProp()
{  
  NuSDVDetConstruction::SetVisProp();
  
  G4VisAttributes* visAttributes{nullptr};
  
  visAttributes = new G4VisAttributes( G4Colour::Brown());
  visAttributes->SetForceSolid(false);
  fContainerLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//invoked by EndOfRunAction method of NuSDRunAction class
void NuSDNulatDetConstruction::Print() const 
{ 
  
  NuSDVDetConstruction::Print();
  G4cout<<"Container thickness (mm): "<<fContainerThick*(1/mm)<<G4endl;
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//----------------UI Commands------------------------------------------------------
void NuSDNulatDetConstruction::SetContainerThickness(G4double thickness)
{
  fContainerThick = thickness;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
