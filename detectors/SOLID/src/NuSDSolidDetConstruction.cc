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

#include "NuSDSolidDetConstruction.hh"
#include "NuSDSolidMessenger.hh"
#include "NuSDSiPMProperties.hh"
#include "NuSDFiberProperties.hh"
#include "NuSDReflectorProperties.hh"
#include "NuSDGroove.hh"
#include "NuSDSiPMReadoutUnit.hh"
#include "NuSDReflectorSurface.hh"

#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4ApplicationState.hh"

//assert works if this opens
#undef NDEBUG
#include <assert.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSolidDetConstruction::NuSDSolidDetConstruction(): 
NuSDVCompositeDetConstruction("Solid",NuSDDetType::InhomogeneousComposite),
fDetMessenger{nullptr},
fReflectorLogic{nullptr},
fReflectorSizeX{0.}, fReflectorSizeY{0.}, fReflectorSizeZ{0.}, 
fReflectorThick{0.},
fSiPMProperties{nullptr},
fFiberProperties{nullptr},
fReflectorProperties{nullptr}
{
  SetDefaults();
  
  fDetMessenger = new NuSDSolidMessenger(this);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSolidDetConstruction::~NuSDSolidDetConstruction()
{
  if(fDetMessenger)        delete fDetMessenger; 
  if(fSiPMProperties)      delete fSiPMProperties; 
  if(fFiberProperties)     delete fFiberProperties;
  if(fReflectorProperties) delete fReflectorProperties;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSolidDetConstruction::SetDefaults()
{ 
  //This initial values may be changed via user interface command with a macro file.  
  
  fNumberOfSegmentAlongX  = 16;  //16
  fNumberOfSegmentAlongY  = 9;   // 9
  fNumberOfSegmentAlongZ  = 16;  //16
  
  fNuScntSizeX            = 5*cm;
  fNuScntSizeY            = 5*cm;
  fNuScntSizeZ            = 5*cm;

  fOptBarrierThick        = 2*um;  //2*um
  fNeutronScntThick       = 0.32*mm;  //we use 0.32*mm. but in the experiment it is 262um
  fReflectorThick         = 186*um;  //186*um. we can observe photon behaviour clearly at the surfaces 
                                     //by increasing this thickness.
  fIsPSOn                 = false;                                        
                                         
  //Fiber properties 
  G4double coreSizeX          = 3*mm; //3*mm
  G4double coreSizeY          = 3*mm; //3*mm
  G4double coreSizeZ          = 3*mm; //3*mm
  
  G4double clad1Thickness     = 0.5*mm; //0.5*mm
  G4double clad2Thickness     = 0.5*mm; //0.5*mm
  G4double gapThickness       = 1*mm;   //1*mm
  G4double grooveDistToWall   = 0.0001*mm;  
  G4double distBetweenGrooves = 0.0001*mm;
                    
  fFiberProperties = new NuSDFiberProperties(coreSizeX,
                         coreSizeY,
                         coreSizeZ,
                         clad1Thickness,
                         clad2Thickness,
                         gapThickness,
                         grooveDistToWall,
                         distBetweenGrooves                         
                         );                                                  
  
  //SiPM properties                                    
  G4bool isPSAttachedBothEnd  = true;
  G4bool  isPSBinderOn        = true;
  G4double  psBinderLength    = 1*mm; 
  
  G4double psQE               = 1.0;
  G4bool isQEDependsenergy    = false;    

  //Mppc
  G4double  psSizeX           = coreSizeX;
  G4double  psSizeY           = coreSizeY;
  G4double  psSizeZ           = coreSizeZ;
  
  fSiPMProperties  = new  NuSDSiPMProperties(isPSAttachedBothEnd, 
                          isPSBinderOn,
                          psBinderLength,
                          psQE,
                          isQEDependsenergy,
                          psSizeX,
                          psSizeY,
                          psSizeZ
                          ); 

  //Reflector properties
  G4double reflectivity              = 0.96;
  G4bool isReflectivityDependsEnergy = false;//if it is true, 
                                             //fill data/surface/Reflector_spect.txt file
                        
  fReflectorProperties = new NuSDReflectorProperties(reflectivity,
                            isReflectivityDependsEnergy);      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* NuSDSolidDetConstruction::Construct()
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

void NuSDSolidDetConstruction::ConstructSDandField() 
{  
  NuSDVCompositeDetConstruction::ConstructSDs(); 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSolidDetConstruction::DefineDetCompDimensions()
{
  
  NuSDVDetConstruction::CheckNumberOfSegment();
 
  fOptBarrierSizeX = fNuScntSizeX;
  fOptBarrierSizeY = fOptBarrierThick;
  fOptBarrierSizeZ = fNuScntSizeZ;

  fNeutronScntSizeX = fNuScntSizeX;
  fNeutronScntSizeY = fNeutronScntThick;
  fNeutronScntSizeZ = fNuScntSizeZ;

  fReflectorSizeX = fNuScntSizeX + 2*fReflectorThick; 
  fReflectorSizeY = fNuScntSizeY + fOptBarrierSizeY + fNeutronScntSizeY + 2*fReflectorThick;
  fReflectorSizeZ = fNuScntSizeZ + 2*fReflectorThick;

  G4double distRefToUnit{0.*cm}; 

  //virtual volume
  fUnitSizeX = fReflectorSizeX + distRefToUnit;
  fUnitSizeY = fReflectorSizeY + distRefToUnit;
  fUnitSizeZ = fReflectorSizeZ + distRefToUnit;

  G4double gap{3.*cm};
  
  G4double length   = (fIsPSOn) ? fSiPMProperties->GetSiPMReadoutUnitLength() : 0.;
  
  //no SiPM along Y placedAxis
  fDetSizeX = fNumberOfSegmentAlongX*fUnitSizeX + 2*length + gap; 
  fDetSizeY = fNumberOfSegmentAlongY*fUnitSizeY + gap; 
  fDetSizeZ = fNumberOfSegmentAlongZ*fUnitSizeZ + 2*length + gap; 
  
  fWorldSizeX = fDetSizeX + gap;
  fWorldSizeY = fDetSizeY + gap;
  fWorldSizeZ = fDetSizeZ + gap;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSolidDetConstruction::DefineSolidsAndLogics()
{

  NuSDVCompositeDetConstruction::DefineSolidsAndLogics(); 
  
  G4Box* nuScntSolid      = new G4Box("NuScntSolid", 0.5*fNuScntSizeX, 0.5*fNuScntSizeY, 0.5*fNuScntSizeZ);
  fNuScntLogic            = new G4LogicalVolume(nuScntSolid, GetMaterial("EJ-200",true) , "NuScntLogic", 0, 0, 0);

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
 
  if(!fSiPMProperties->IsAttachedBothEnd() )
  {
    new G4LogicalSkinSurface("SkinSurface", fDetLogic , fReflectorSurface );  
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSolidDetConstruction::ConstructSingleDetUnit()
{

  //Place all components in Reflector(Ref).
  //reference point is Ref center. RefCenterVec(0,0,0)
  G4double distNuScntCenterToRefCenterAlongY     = fNeutronScntSizeY*0.5 + fOptBarrierSizeY*0.5;
  G4double distOptBarrierCenterToRefCenterAlongY = fNuScntSizeY*0.5-distNuScntCenterToRefCenterAlongY+fOptBarrierSizeY*0.5;
  G4double distNdCenterToRefCenterAlongY         = distOptBarrierCenterToRefCenterAlongY+fOptBarrierSizeY*0.5+fNeutronScntSizeY*0.5;

  //x and z axis center coincide
  G4ThreeVector nuScntPosVec(0, -distNuScntCenterToRefCenterAlongY, 0);
  G4ThreeVector optBarrierPosVec(0, distOptBarrierCenterToRefCenterAlongY , 0);
  G4ThreeVector ndPosVec(0, distNdCenterToRefCenterAlongY , 0);
   
  //place nuScnt in Reflector. First daughter of Reflector
  new G4PVPlacement(0,                     //no rotation
      nuScntPosVec,       //at (0,0,0)
      fNuScntLogic,         //its logical volume
      "NuScntPV",               //its name
      fReflectorLogic,                     //its mother  volume
      false,                 //no boolean operation
      0,                     //copy number
      fCheckOverlaps);        //overlaps checking 


  //place OptBarrier in Ref. Second daughter of Ref
  new G4PVPlacement(0,                     //no rotation
      optBarrierPosVec,       //at (0,0,0)
      fOptBarrierLogic,         //its logical volume
      "OptBarrierPV",               //its name
      fReflectorLogic,                     //its mother  volume
      false,                 //no boolean operation
      1,                     //copy number
      fCheckOverlaps);        //overlaps checking 


  //Place Nd in Ref. Third daugher of Ref.
  new G4PVPlacement(0,                     //no rotation
      ndPosVec,       //at (0,0,0)
      fNeutronScntLogic,         //its logical volume
      "NeutronScntPV",               //its name
      fReflectorLogic,                     //its mother  volume
      false,                 //no boolean operation
      2,                     //copy number
      fCheckOverlaps);        //overlaps checking 

  //Open a groove in Reflector. fourth and fifth daughter of Ref.
  new NuSDGroove(0, 
      G4ThreeVector(0,0,0),  //will be changed in NuSDGroove.cc class
      fReflectorLogic, //mother volume
      false, 
      3, 
      'x',
      this);

  new NuSDGroove(0, 
      G4ThreeVector(0,0,0), //will be changed in NuSDGroove.cc class
      fReflectorLogic, //mother volume
      false, 
      4, 
      'z',
      this);

  //Place Ref in Unit. Single daughter of Unit
  new G4PVPlacement(0,                     //no rotation
      G4ThreeVector(0,0,0),       //at (0,0,0)
      fReflectorLogic,         //its logical volume
      "ReflectorPV",               //its name
      fUnitLogic,                     //its mother  volume
      false,                 //no boolean operation
      0,                     //copy number
      fCheckOverlaps);        //overlaps checking 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSolidDetConstruction::ConstructMatrixDet()
{
     
  G4double x{}, y{}, z{};  //unit center position coordinates with respect to its mother. 

  //----------Place Units in Solid Detector--------------------    	 
  for(int i=0; i<fNumberOfSegmentAlongY; i++)
  {  

    y =  -(fUnitSizeY)*(fNumberOfSegmentAlongY-1)*0.5 + i*(fUnitSizeY);

    //xz layer       
    for(int j =0; j<fNumberOfSegmentAlongX; j++)
    {
   
      x =  -fUnitSizeX*(fNumberOfSegmentAlongX-1)*0.5 + j*fUnitSizeX;
    
      for(int k =0; k<fNumberOfSegmentAlongZ; k++)
      
      {      
	      
        z =  -fUnitSizeZ*(fNumberOfSegmentAlongZ-1)*0.5 + k*fUnitSizeZ; 
	        
        G4VPhysicalVolume* physUnit = new  G4PVPlacement(0, 
	                                         G4ThreeVector(x,y,z), 
	                                         fUnitLogic, 
	                                         "UnitPV", 
	                                         fDetLogic, 
	                                         false, 
	                                         fPhysUnitVector.size(), 
	                                         fCheckOverlaps);         
		         
	      fPhysUnitVector.push_back(physUnit);
	     
        if( fIsPSOn )     
        {
                
          if(j==0)
            AddOptReadoutUnit(physUnit,"-x");
            
          if(k==0)
            AddOptReadoutUnit(physUnit,"-z");
               
        }                 
                   
     } // k loop 
      
        
       
    
   } //j loop

 } //i loop
  
     
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSolidDetConstruction::AddOptReadoutUnit(const G4VPhysicalVolume *physUnit, G4String placedAxis)
{

  assert(physUnit);

  G4VPhysicalVolume* physOptReadoutUnit{nullptr};

  G4ThreeVector grooveCenterPosVec;
  G4ThreeVector siPMPosition; 
 
   
  if(placedAxis == "-x")
  {
   
    grooveCenterPosVec =  physUnit->GetTranslation() + 
                          physUnit->GetLogicalVolume()->GetDaughter(0)->
                          GetLogicalVolume()->GetDaughter(3)->GetTranslation();
                                        
    
    siPMPosition.setX( grooveCenterPosVec.getX()-fUnitSizeX*0.5-
    fSiPMProperties->GetSiPMReadoutUnitLength()*0.5 );
    
    siPMPosition.setY( grooveCenterPosVec.getY() );
    siPMPosition.setZ( grooveCenterPosVec.getZ() );
                                                

  }else if(placedAxis == "-z")
  {
   
    grooveCenterPosVec = physUnit->GetTranslation() + 
                         physUnit->GetLogicalVolume()->GetDaughter(0)->
                         GetLogicalVolume()->GetDaughter(4)->GetTranslation(); //wrt SolidDet

    siPMPosition.setX( grooveCenterPosVec.getX() );
    siPMPosition.setY( grooveCenterPosVec.getY() );
    siPMPosition.setZ( grooveCenterPosVec.getZ()-fUnitSizeZ*0.5-
    fSiPMProperties->GetSiPMReadoutUnitLength()*0.5 );
                                                 
       
  }
      
      
  physOptReadoutUnit = new NuSDSiPMReadoutUnit(0, //no rotation
                           siPMPosition, 
                           fDetLogic, 
                           false,  
                           fPhysOptReadoutUnitVec.size(), //copyNo
                           placedAxis,
                           fSiPMProperties
                           ); 
             
  fPhysOptReadoutUnitVec.push_back(physOptReadoutUnit);
    
  if( fSiPMProperties->IsAttachedBothEnd() )  
  {
    
    placedAxis.erase(0,1);
  
    if(placedAxis == "x")
      siPMPosition.setX(-siPMPosition.getX()); 
    else if(placedAxis == "z") 				 
      siPMPosition.setZ(-siPMPosition.getZ());
    

    physOptReadoutUnit = new  NuSDSiPMReadoutUnit(0, 
                              siPMPosition, 
                              fDetLogic, 
                              false,  
                              fPhysOptReadoutUnitVec.size(),  //copyNo
                              placedAxis,
                              fSiPMProperties
                              ); 
       
    fPhysOptReadoutUnitVec.push_back(physOptReadoutUnit);


  } 
		         
		  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSolidDetConstruction::SetVisProp()
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
void NuSDSolidDetConstruction::Print() const 
{ 
  NuSDVCompositeDetConstruction::Print();
  G4cout<<"Reflector thickness (mm) : "<<fReflectorThick*(1/mm)<<G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//----------------UI Commands------------------------------------------------------
void NuSDSolidDetConstruction::SetReflectorThickness(G4double thickness)
{
  fReflectorThick = thickness;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
   G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


