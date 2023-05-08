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

#include "NuSDChandlerDetConstruction.hh"
#include "NuSDChandlerMessenger.hh"
#include "NuSDPmtProperties.hh"
#include "NuSDReflectorProperties.hh"
#include "NuSDPmtReadoutUnit.hh"
#include "NuSDReflectorSurface.hh"

#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
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

NuSDChandlerDetConstruction::NuSDChandlerDetConstruction(): 
NuSDVCompositeDetConstruction("Chandler",NuSDDetType::InhomogeneousComposite),
fDetMessenger{nullptr},
fPmtProperties{nullptr},
fReflectorProperties{nullptr}
{
  SetDefaults();
  
  fDetMessenger = new NuSDChandlerMessenger(this);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDChandlerDetConstruction::~NuSDChandlerDetConstruction()
{  
  if(fDetMessenger)        delete fDetMessenger; 
  if(fPmtProperties)       delete fPmtProperties;
  if(fReflectorProperties) delete fReflectorProperties;    	     	      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDChandlerDetConstruction::SetDefaults()
{
  //This initial values may be changed via user interface command with a macro file.  
  fNumberOfSegmentAlongX        = 16; //16
  fNumberOfSegmentAlongY        = 16; //16
  fNumberOfSegmentAlongZ        = 16; //16

  fNuScntSizeX                  = 6.2*cm; //6.2
  fNuScntSizeY                  = 6.2*cm; //6.2
  fNuScntSizeZ                  = 6.2*cm; //6.2

  fNeutronScntThick             = 0.32*mm; //320um = 0.32 mm, bigger than 0.15mm.
  fOptBarrierThick              = 0.15*mm; //ai4 0.15mm+0.15=0.3mm
     
  fIsPSOn                       = false;
     
  //pmt parameters        
  G4bool isPSAttachedBothEnd    = true;
  G4bool  isPSBinderOn          = false;
  G4double  psBinderLength      = 1*mm; 
  
  G4double psQE                 = 1.0;
  G4bool isQEDependsEnergy      = false;    

  G4double  psRadius            = 51*0.5*mm; //51 mm diameter

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
                        isQEDependsEnergy,
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

G4VPhysicalVolume* NuSDChandlerDetConstruction::Construct()
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

void NuSDChandlerDetConstruction::ConstructSDandField() 
{ 
  NuSDVCompositeDetConstruction::ConstructSDs();      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDChandlerDetConstruction::DefineDetCompDimensions()
{
  NuSDVDetConstruction::CheckNumberOfSegment();

  fOptBarrierSizeX  =  fNuScntSizeX  + 2*fOptBarrierThick;
  fOptBarrierSizeY  =  fNuScntSizeY  + 2*fOptBarrierThick;
  fOptBarrierSizeZ  =  fNuScntSizeZ  + 2*fOptBarrierThick;

  G4double distOptBarrierToUnit{0.*mm}; //just for visualization

  fUnitSizeX        = fOptBarrierSizeX + distOptBarrierToUnit;
  fUnitSizeY        = fOptBarrierSizeY + distOptBarrierToUnit;
  fUnitSizeZ        = fOptBarrierSizeZ + distOptBarrierToUnit;

  fNeutronScntSizeX = fNumberOfSegmentAlongX*fUnitSizeX;
  fNeutronScntSizeY = fNeutronScntThick;
  fNeutronScntSizeZ = fNumberOfSegmentAlongZ*fUnitSizeZ;

  G4double gap{3.*cm};
   
  G4double length   = (fIsPSOn) ? fPmtProperties->GetPmtReadoutUnitLength() : 0.;
    
  fDetSizeX         = fNumberOfSegmentAlongX*fUnitSizeX + 2*length + gap;
  fDetSizeY         = fNumberOfSegmentAlongY*fUnitSizeY + (fNumberOfSegmentAlongY+1)*fNeutronScntSizeY + gap; 
  fDetSizeZ         = fNumberOfSegmentAlongZ*fUnitSizeZ + 2*length+ gap;

  fWorldSizeX       = fDetSizeX + gap;
  fWorldSizeY       = fDetSizeY + gap;
  fWorldSizeZ       = fDetSizeZ + gap;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDChandlerDetConstruction::DefineSolidsAndLogics()
{
  NuSDVCompositeDetConstruction::DefineSolidsAndLogics();
  
  G4Box* nuScntSolid      = new G4Box("NuScntSolid", 0.5*fNuScntSizeX, 0.5*fNuScntSizeY, 0.5*fNuScntSizeZ);
  fNuScntLogic            = new G4LogicalVolume(nuScntSolid, GetMaterial("EJ-260",true) , "NuScntLogic", 0, 0, 0);

  G4Box* optBarrierSolid  = new G4Box("OptBarrierSolid", 0.5*fOptBarrierSizeX, 0.5*fOptBarrierSizeY,0.5*fOptBarrierSizeZ);              
  fOptBarrierLogic        = new G4LogicalVolume(optBarrierSolid, GetMaterial("Air",true),"OptBarrierLogic",0,0,0);
  
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

void NuSDChandlerDetConstruction::ConstructSingleDetUnit()
{
  //Place NuScnt in OptBarrier
  new  G4PVPlacement(0,                     //no rotation
       G4ThreeVector(0,0,0),       //at (0,0,0)
       fNuScntLogic,         //its logical volume
       "NuScntPV",               //its name
       fOptBarrierLogic,                     //its mother  volume
       false,                 //no boolean operation
       0,                     //copy number
       fCheckOverlaps);        //overlaps checking 


  //place OptBarrier in Unit
  new  G4PVPlacement(0,                     //no rotation
       G4ThreeVector(0,0,0),       //at (0,0,0)
       fOptBarrierLogic,         //its logical volume
       "OptBarrierPV",               //its name
       fUnitLogic,                     //its mother  volume
       false,                 //no boolean operation
       1,                     //copy number
       fCheckOverlaps);        //overlaps checking 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDChandlerDetConstruction::ConstructMatrixDet()
{     
  
  G4double nd_center_y{};  //y coordinate of the Nd center wrt its mother(ChandlerDet)
  G4double x{}, y{}, z{};  // unit center coordinates wrt its mother(ChandlerDet)

  //----------Place Units in ChandlerDet--------------------    	 

  for(int i=0; i<fNumberOfSegmentAlongY+1; i++)     //added +1 to insert last nd layer
  {  

    nd_center_y =  -(fUnitSizeY+fNeutronScntSizeY)*(fNumberOfSegmentAlongY)*0.5 + i*(fUnitSizeY+fNeutronScntSizeY);
    //place Nd in Chandler
    G4VPhysicalVolume* physNd = new G4PVPlacement(0, 
                                G4ThreeVector(0,nd_center_y,0), 
                                fNeutronScntLogic, 
                                "NeutronScntPV", 
                                fDetLogic, 
                                false, 
                                fPhysNdVector.size(), 
                                fCheckOverlaps);        //overlaps checking

    fPhysNdVector.push_back(physNd);
    G4cout<<"neutronSayac: "<<fPhysNdVector.size()<<"  "<<i<<G4endl;    

    if( i == fNumberOfSegmentAlongY ) {break;}   //After the last nd layer inserted, it exits the loop within it


    //xz layer       
    for(int j =0; j<fNumberOfSegmentAlongX; j++)
    {
      x =  -fUnitSizeX*(fNumberOfSegmentAlongX-1)*0.5 + j*fUnitSizeX;

      for(int k =0; k<fNumberOfSegmentAlongZ; k++)
      {      
        z =  -fUnitSizeZ*(fNumberOfSegmentAlongZ-1)*0.5 + k*fUnitSizeZ; 
        //include unit  
        G4VPhysicalVolume* physUnit = new G4PVPlacement(0, 
                                          G4ThreeVector(x, y=nd_center_y+(fUnitSizeY+fNeutronScntSizeY)*0.5, z), 
                                          fUnitLogic, 
                                          "UnitPV", 
                                          fDetLogic, 
                                          false, 
                                          fPhysUnitVector.size(),  //copyNo
                                          fCheckOverlaps);      

        fPhysUnitVector.push_back(physUnit);
      
        if( fIsPSOn )     
        {        
          if(j==0)
            AddOptReadoutUnit(physUnit,"-x");

          if(k==0)
            AddOptReadoutUnit(physUnit,"-z");

        }                 

      } //k loop 

    } // j loop

  }//i loop
              
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDChandlerDetConstruction::AddOptReadoutUnit(const G4VPhysicalVolume *physUnit, G4String placedAxis)
{  
  assert(physUnit);
      
  G4VPhysicalVolume* physOptReadoutUnit{nullptr};
  
  G4ThreeVector pmtPosition( physUnit->GetTranslation().getX(), physUnit->GetTranslation().getY(), 
                             physUnit->GetTranslation().getZ() );

  //update 
  if(placedAxis == "-x")
  {
    pmtPosition.setX( physUnit->GetTranslation().getX()-fUnitSizeX*0.5-
    fPmtProperties->GetPmtReadoutUnitLength()*0.5 );
  }
  else if(placedAxis == "-z")
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
       
    if(placedAxis == "x")
      pmtPosition.setX(-pmtPosition.getX());
    else if(placedAxis == "z") 				 
      pmtPosition.setZ(-pmtPosition.getZ() ); 				 

    
    physOptReadoutUnit = new NuSDPmtReadoutUnit(0, 
                             pmtPosition,  
                             fDetLogic, //mother volume
                             false, //pMany
                             fPhysOptReadoutUnitVec.size(), //copyNo
                             placedAxis,
                             fPmtProperties 
                             ); 

    fPhysOptReadoutUnitVec.push_back(physOptReadoutUnit);
        
  }
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDChandlerDetConstruction::SetVisProp()
{
  NuSDVCompositeDetConstruction::SetVisProp();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//invoked by EndOfRunAction method of NuSDRunAction class
void NuSDChandlerDetConstruction::Print() const 
{ 
  NuSDVCompositeDetConstruction::Print();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

