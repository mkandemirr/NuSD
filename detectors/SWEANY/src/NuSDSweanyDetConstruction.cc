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

#include "NuSDSweanyDetConstruction.hh"
#include "NuSDSweanyMessenger.hh"
#include "NuSDPmtProperties.hh"
#include "NuSDReflectorProperties.hh"
#include "NuSDPmtReadoutUnit.hh"
#include "NuSDReflectorSurface.hh"

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

NuSDSweanyDetConstruction::NuSDSweanyDetConstruction(): 
NuSDVCompositeDetConstruction("Sweany",NuSDDetType::InhomogeneousComposite),
fDetMessenger{nullptr},
fReflectorLogic{nullptr},
fReflectorSizeX{0.}, fReflectorSizeY{0.}, fReflectorSizeZ{0.},
fReflectorThick{0.},
fPmtProperties{nullptr},
fReflectorProperties{nullptr}
{
  SetDefaults();
  
  fDetMessenger = new NuSDSweanyMessenger(this);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSweanyDetConstruction::~NuSDSweanyDetConstruction()
{
  if(fDetMessenger)        delete fDetMessenger; 
  if(fPmtProperties)       delete fPmtProperties;
  if(fReflectorProperties) delete fReflectorProperties;    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSweanyDetConstruction::SetDefaults()
{
  //This initial values may be changed via user interface command with a macro file.  
  //cannot be zero!
  fNumberOfSegmentAlongX        = 7;
  fNumberOfSegmentAlongY        = 7;
  fNumberOfSegmentAlongZ        = 1; 

  fNuScntSizeX                  = 12.7*cm;
  fNuScntSizeY                  = 12.7*cm;
  fNuScntSizeZ                  = 120*cm;

  fOptBarrierThick              = 0.3*mm; //air 0.3mm
  fNeutronScntThick             = 0.32*mm; //this should be member, 0.32*mm
  
  fReflectorThick               = 0.25*mm; //250um
     
  fIsPSOn                       = false;
          
  G4bool isPSAttachedBothEnd    = true;
  G4bool  isPSBinderOn          = false;
  G4double  psBinderLength      = 1*mm; 
  
  G4double psQE                 = 1.0;
  G4bool isQEDependsenergy      = false;    

  G4double  psRadius            = 12.7*0.5*cm; //5 inch-12.7 cm diameter pmts

  G4double  isLgOn              = false;

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

G4VPhysicalVolume* NuSDSweanyDetConstruction::Construct()
{
   
  if(fPhysWorld) 
  {
    NuSDVCompositeDetConstruction::CleanUpOldGeometry();       
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

void NuSDSweanyDetConstruction::ConstructSDandField() 
{ 
  NuSDVCompositeDetConstruction::ConstructSDs();   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSweanyDetConstruction::DefineDetCompDimensions()
{
  
  NuSDVDetConstruction::CheckNumberOfSegment();
  
  fOptBarrierSizeX = fNuScntSizeX + 2*fOptBarrierThick;
  fOptBarrierSizeY = fNuScntSizeY + 2*fOptBarrierThick;
  fOptBarrierSizeZ = fNuScntSizeZ;

  fNeutronScntSizeX = fOptBarrierSizeX + 2*fNeutronScntThick;
  fNeutronScntSizeY = fOptBarrierSizeY + 2*fNeutronScntThick;
  fNeutronScntSizeZ = fOptBarrierSizeZ;

  fReflectorSizeX   = fNeutronScntSizeX + 2*fReflectorThick;
  fReflectorSizeY   = fNeutronScntSizeY + 2*fReflectorThick;
  fReflectorSizeZ   = fNeutronScntSizeZ;

  G4double distReflectorToUnit{0*cm};

  fUnitSizeX        = fReflectorSizeX + distReflectorToUnit;
  fUnitSizeY        = fReflectorSizeY + distReflectorToUnit;
  fUnitSizeZ        = fReflectorSizeZ + distReflectorToUnit;

  G4double gap{3.*cm};
  
  G4double length   = (fIsPSOn) ? fPmtProperties->GetPmtReadoutUnitLength() : 0.;

  fDetSizeX         = fNumberOfSegmentAlongX*fUnitSizeX + gap;
  fDetSizeY         = fNumberOfSegmentAlongY*fUnitSizeY + gap;
  fDetSizeZ         = fNumberOfSegmentAlongZ*fUnitSizeZ + 2*length + gap;
  
  fWorldSizeX       = fDetSizeX + gap;
  fWorldSizeY       = fDetSizeY + gap;
  fWorldSizeZ       = fDetSizeZ + gap;
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSweanyDetConstruction::DefineSolidsAndLogics()
{  
  
  NuSDVCompositeDetConstruction::DefineSolidsAndLogics(); 
  
  G4Box* nuScntSolid      = new G4Box("NuScntSolid", 0.5*fNuScntSizeX, 0.5*fNuScntSizeY, 0.5*fNuScntSizeZ);
  fNuScntLogic            = new G4LogicalVolume(nuScntSolid, GetMaterial("EJ-260",true) , "NuScntLogic", 0, 0, 0);

  G4Box* optBarrierSolid  = new G4Box("OptBarrierSolid", 0.5*fOptBarrierSizeX, 0.5*fOptBarrierSizeY,0.5*fOptBarrierSizeZ);              
  fOptBarrierLogic        = new G4LogicalVolume(optBarrierSolid, GetMaterial("Air",true),"OptBarrierLogic",0,0,0);
  
  G4Box *reflectorSolid   = new G4Box("ReflectorSolid", 0.5*fReflectorSizeX, 0.5*fReflectorSizeY, 0.5*fReflectorSizeZ);
  fReflectorLogic         = new G4LogicalVolume(reflectorSolid, GetMaterial("AluminizedMylar"),"ReflectorLogic",0,0,0);
  
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

void NuSDSweanyDetConstruction::ConstructSingleDetUnit()
{
  
   //Place NuScnt in optBarrier
   new   G4PVPlacement(0,                     //no rotation
         G4ThreeVector(0,0,0),       //at (0,0,0)
         fNuScntLogic,         //its logical volume
         "NuScntPV",               //its name
         fOptBarrierLogic,                     //its mother  volume
         false,                 //no boolean operation
         0,                     //copy number
         fCheckOverlaps);        //overlaps checking 


   //place OptBarrier in NeutronScntector
   new   G4PVPlacement(0,                     //no rotation
         G4ThreeVector(0,0,0),       //at (0,0,0)
         fOptBarrierLogic,         //its logical volume
         "OptBarrierPV",               //its name
         fNeutronScntLogic,                     //its mother  volume
         false,                 //no boolean operation
         1,                     //copy number
         fCheckOverlaps);        //overlaps checking 


   //place  NeutronScntector in Reflector
   new   G4PVPlacement(0,                     //no rotation
         G4ThreeVector(0,0,0),       //at (0,0,0)
         fNeutronScntLogic,         //its logical volume
         "NeutronScntPV",               //its name
         fReflectorLogic,                     //its mother  volume
         false,                 //no boolean operation
         2,                     //copy number
         fCheckOverlaps);        //overlaps checking 


   //Place Reflector in Unit
   new   G4PVPlacement(0,                     //no rotation
         G4ThreeVector(0,0,0),       //at (0,0,0)
         fReflectorLogic,         //its logical volume
         "ReflectorPV",               //its name
         fUnitLogic,                     //its mother  volume
         false,                 //no boolean operation
         3,                     //copy number
         fCheckOverlaps);        //overlaps checking 

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//----------Common for 1d light collection detector-------------------------
void NuSDSweanyDetConstruction::ConstructMatrixDet()
{

  G4double x{}, y{}, z{};  //unit center position coordinates with respect to its mother. 

  //----------Place Units in Sweany Detector--------------------    	 

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

//---------Common for 1d segmented detector------------------------
void NuSDSweanyDetConstruction::AddOptReadoutUnit(const G4VPhysicalVolume *physUnit)
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
  
  if(  fPmtProperties->IsAttachedBothEnd() )
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

void NuSDSweanyDetConstruction::SetVisProp()
{
    
  NuSDVCompositeDetConstruction::SetVisProp();
  
  G4VisAttributes* visAttributes{nullptr};
  
  //Reflector
  visAttributes = new G4VisAttributes( G4Colour::Yellow());
  visAttributes->SetForceSolid(false);
  fReflectorLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//invoked by EndOfRunAction method of NuSDRunAction class
void NuSDSweanyDetConstruction::Print() const 
{ 
  NuSDVCompositeDetConstruction::Print();
  G4cout<<"Reflector thickness (mm) : "<<fReflectorThick*(1/mm)<<G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//----------------UI Commands------------------------------------------------------
void NuSDSweanyDetConstruction::SetReflectorThickness(G4double thickness)
{
  fReflectorThick = thickness;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


