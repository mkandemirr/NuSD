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

#include "NuSDHspDetConstruction.hh"
#include "NuSDHspMessenger.hh"
#include "NuSDHexPmtProperties.hh"
#include "NuSDReflectorProperties.hh"
#include "NuSDHexPmtReadoutUnit.hh"
#include "NuSDReflectorSurface.hh"

#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
//#include "G4Box.hh"
#include "G4Polyhedra.hh"
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

NuSDHspDetConstruction::NuSDHspDetConstruction(): 
NuSDVDetConstruction("Hsp",NuSDDetType::Homogeneous),
fDetMessenger{nullptr},
fReflectorLogic{nullptr}, 
fLiquidTankLogic{nullptr},
fNuScntSideLength{0.},
fNuScntTanDisToOuterSur{0.},
fOptBarrierTanDisToOuterSur{0.},
fReflectorTanDisToOuterSur{0.},
fLiquidTankTanDisToOuterSur{0.},
fUnitTanDisToOuterSur{0.},
fDetTanDisToOuterSur{0.},
fWorldTanDisToOuterSur{0.},
fReflectorThick{0.}, fLiquidTankThick{0.},
fHexPmtProperties{nullptr},
fReflectorProperties{nullptr}
{
  SetDefaults();
  
  fDetMessenger = new NuSDHspMessenger(this);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDHspDetConstruction::~NuSDHspDetConstruction()
{
  if(fDetMessenger)        delete fDetMessenger; 
  if(fHexPmtProperties)    delete fHexPmtProperties;  
  if(fReflectorProperties) delete fReflectorProperties;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHspDetConstruction::SetDefaults()
{ 
  //This initial values may be changed via user interface command with a macro file.  
  //cannot be zero!
  fNumberOfSegmentAlongX        = 11; //11
  fNumberOfSegmentAlongZ        = 1; //const

  fNuScntSideLength             = 6.0*cm; //6 cm
  fNuScntSizeZ                  = 120*cm; // 120 cm

  fOptBarrierThick              = 51*um; //0.051*mm= 51um Fluorinated ethylene propylene(FEP) 
  fReflectorThick               = 0.1*mm; //0.1*mm 3M DF 2000MA
  fLiquidTankThick              = 1.*cm;  //1*cm  
      
  fIsPSOn                       = false;
       
  //        
  G4bool isPSAttachedBothEnd    = true;
  G4bool  isPSBinderOn          = false;
  G4double  psBinderLength      = 1*mm; 
  
  G4double psQE                 = 1.0;
  G4bool isQEDependsenergy      = false;    

  G4double  psTanDisToOuterSur  = 0.5*70*mm; //R6235 76 mm Hexagonal.active 70 mm

  G4double  isLgOn              = true;

  G4double  lgLength            = 4*cm;

  G4bool    isLgBinderOn        = false;
  G4double  lgBinderLength      = 1*mm; 
  
   
  
  fHexPmtProperties  = new  NuSDHexPmtProperties(isPSAttachedBothEnd, 
                            isPSBinderOn,
                            psBinderLength,
                            psQE,
                            isQEDependsenergy,
                            psTanDisToOuterSur,
                            isLgOn,
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

G4VPhysicalVolume* NuSDHspDetConstruction::Construct()
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

void NuSDHspDetConstruction::ConstructSDandField() 
{ 
   NuSDVDetConstruction::ConstructSDs();    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHspDetConstruction::DefineDetCompDimensions()
{

  if( fNumberOfSegmentAlongX % 2 == 0)
    throw std::runtime_error("numberOfSegmentAlongX should be odd");
  
  fNuScntTanDisToOuterSur      =  (fNuScntSideLength/2.)*sqrt(3);
  fOptBarrierTanDisToOuterSur  =  fNuScntTanDisToOuterSur+fOptBarrierThick;
  fReflectorTanDisToOuterSur   =  fOptBarrierTanDisToOuterSur+fReflectorThick;
  fLiquidTankTanDisToOuterSur  =  fReflectorTanDisToOuterSur+fLiquidTankThick;
  
  G4double distLiquidTankToUnit{0.*cm};
  
  fUnitTanDisToOuterSur        = fLiquidTankTanDisToOuterSur+distLiquidTankToUnit;
  fUnitSizeZ                   = fNuScntSizeZ;

  G4double gap{3.*cm};
  
  fDetTanDisToOuterSur         =  fUnitTanDisToOuterSur*fNumberOfSegmentAlongX + gap;
  
  G4double pmtLength           = (fIsPSOn) ? fHexPmtProperties->GetHexPmtReadoutUnitLength() : 0.;
  
  fDetSizeZ                    = fNumberOfSegmentAlongZ*fUnitSizeZ + 2*pmtLength + gap;
  
  fWorldTanDisToOuterSur       =  fDetTanDisToOuterSur + gap; 
  fWorldSizeZ                  =  fDetSizeZ +gap;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHspDetConstruction::DefineSolidsAndLogics()
{
  G4double startAngle                 = 0.*deg;
  G4double totalAngle                 = 360.*deg;
  G4int numberOfSide                  = 6;
  G4int numberOfPlane                 = 2;

  const G4double rInner[2]            = { 0*cm, 0*cm };
  const G4double rOuterNuScnt[2]      = { fNuScntTanDisToOuterSur, fNuScntTanDisToOuterSur };
  const G4double rOuterOptBarrier[2]  = { fOptBarrierTanDisToOuterSur, fOptBarrierTanDisToOuterSur };
  const G4double rOuterReflector[2]   = { fReflectorTanDisToOuterSur, fReflectorTanDisToOuterSur };
  const G4double rOuterLiquidTank[2]  = { fLiquidTankTanDisToOuterSur, fLiquidTankTanDisToOuterSur };
  const G4double rOuterUnit[2]        = { fUnitTanDisToOuterSur, fUnitTanDisToOuterSur };
  const G4double rOuterDet[2]         = { fDetTanDisToOuterSur, fDetTanDisToOuterSur }; 

  const G4double zNuScnt[2]           = { -0.5*fNuScntSizeZ, 0.5*fNuScntSizeZ }; //position of zPlane
  const G4double zDet[2]              = { -0.5*fDetSizeZ, 0.5*fDetSizeZ };  
     
     
  G4VSolid *nuScntSolid       = new G4Polyhedra("NuScntSolid", startAngle, totalAngle, numberOfSide, numberOfPlane, 
                                               zNuScnt, rInner, rOuterNuScnt);
  fNuScntLogic             		= new G4LogicalVolume(nuScntSolid, GetMaterial("EJ-335",true),"NuScntLogic",0,0,0);

  G4VSolid *optBarrierSolid   = new G4Polyhedra("OptBarrierSolid",startAngle, totalAngle,numberOfSide, numberOfPlane, zNuScnt,
                                               rInner, rOuterOptBarrier); 
  fOptBarrierLogic           	= new G4LogicalVolume(optBarrierSolid, GetMaterial("Fep",true),"OptBarrierLogic",0,0,0);

  G4VSolid *reflectorSolid    = new G4Polyhedra("ReflectorSolid",startAngle, totalAngle,numberOfSide, numberOfPlane, zNuScnt,   
                                               rInner, rOuterReflector);
  fReflectorLogic             = new G4LogicalVolume(reflectorSolid, GetMaterial("3MSpecularReflector"),"ReflectorLogic",0,0,0);
   
  G4VSolid *liquidTankSolid   = new G4Polyhedra("LiquidTankSolid",startAngle, totalAngle,numberOfSide, numberOfPlane, 
                                               zNuScnt, rInner, rOuterLiquidTank);
  fLiquidTankLogic            = new G4LogicalVolume(liquidTankSolid, GetMaterial("Pmma",true), "LiquidTankLogic",0,0,0);

  G4VSolid *unitSolid         = new G4Polyhedra("UnitSolid",startAngle, totalAngle, numberOfSide, numberOfPlane, zNuScnt,   
                                               rInner, rOuterUnit);
  fUnitLogic                	= new G4LogicalVolume(unitSolid, GetMaterial("Air",true), "UnitLogic", 0, 0, 0);   

  G4VSolid *detSolid          = new G4Polyhedra("DetSolid",startAngle, totalAngle,numberOfSide, numberOfPlane, zDet, rInner, 
                                               rOuterDet);
  fDetLogic                   = new G4LogicalVolume(detSolid, GetMaterial("G4_Galactic"), "DetLogic", 0, 0, 0);
  
  if(!fIsPSOn) return;
  
  fReflectorSurface  =  new NuSDReflectorSurface("ReflectorName", //name
                            unified,                //model
                            polishedfrontpainted,  // surfaceFinish 
                            dielectric_dielectric, //type
                            fReflectorProperties
                            );  

  new G4LogicalSkinSurface("SkinSurface", fReflectorLogic , fReflectorSurface );
 
  if(!fHexPmtProperties->IsAttachedBothEnd() )
  {
    new G4LogicalSkinSurface("SkinSurface", fDetLogic , fReflectorSurface );  
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHspDetConstruction::ConstructSingleDetUnit()
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


  //place Reflector in Tank
  new G4PVPlacement(0,                     //no rotation
     G4ThreeVector(0,0,0),       //at (0,0,0)
     fReflectorLogic,         //its logical volume
     "ReflectorPV",               //its name
     fLiquidTankLogic,                     //its mother  volume
     false,                 //no boolean operation
     2,                     //copy number
     fCheckOverlaps);        //overlaps checking 
     
     
  //place Tank in Unit
  new G4PVPlacement(0,                     //no rotation
     G4ThreeVector(0,0,0),       //at (0,0,0)
     fLiquidTankLogic,         //its logical volume
     "LiquidTankPV",               //its name
     fUnitLogic,                     //its mother  volume
     false,                 //no boolean operation
     3,                     //copy number
     fCheckOverlaps);        //overlaps checking 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHspDetConstruction::ConstructMatrixDet()
{

  G4double x{}, y{}, z{};   // unit center coordinates wrt its mother(HspDet
  G4double unit_sideLength     = 2*fUnitTanDisToOuterSur/sqrt(3.);
  G4int initValue              = (fNumberOfSegmentAlongX+1)/2; //initial value of numberOfUnitAlongY. 
  //this is necessary for hexagonal symetry! 
  G4int numberOfUnitAlongY     = initValue;  //it changes along the x axis, initial value is given. 

  for(G4int i=0; i<fNumberOfSegmentAlongX; ++i)
  {
    x =  -1.5*unit_sideLength*(fNumberOfSegmentAlongX-1)*0.5 + i*1.5*unit_sideLength;  

    for ( G4int j = 0; j < numberOfUnitAlongY; j += 1)
    {

      y =  -2*fUnitTanDisToOuterSur*(numberOfUnitAlongY-1)*0.5 + j*2*fUnitTanDisToOuterSur;


      for(G4int k =0; k<fNumberOfSegmentAlongZ; k++)
      {      
        z = -fUnitSizeZ*(fNumberOfSegmentAlongZ-1)*0.5 + k*fUnitSizeZ;

        G4VPhysicalVolume* physUnit   =   new G4PVPlacement(0, 
                                              G4ThreeVector(x,y,z), 
                                              fUnitLogic, 
                                              "UnitPV",  
                                              fDetLogic, 
                                              false, 
                                              fPhysUnitVector.size(), 
                                              fCheckOverlaps //overlaps checking
                                              );        

        fPhysUnitVector.push_back(physUnit); 

        if( fIsPSOn && k==0 )  //z axis    
        {
          AddOptReadoutUnit( physUnit );  
        }

      }//k loop

    }//j loop

    if( i<(fNumberOfSegmentAlongX-1)/2 )
      numberOfUnitAlongY++;
    else
      numberOfUnitAlongY--;

  }//i loop  


}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//---------Common for 1d Light collection detector------------------------
void NuSDHspDetConstruction::AddOptReadoutUnit(const G4VPhysicalVolume *physUnit)
{
  
  assert(physUnit);

  G4VPhysicalVolume* physOptReadoutUnit{nullptr};
  
  G4ThreeVector pmtPosition( physUnit->GetTranslation().getX(), physUnit->GetTranslation().getY(),
                             physUnit->GetTranslation().getZ()-fUnitSizeZ*0.5-
                             fHexPmtProperties->GetHexPmtReadoutUnitLength()*0.5 );
     
  physOptReadoutUnit =  new  NuSDHexPmtReadoutUnit( 0, 
                             pmtPosition, 
                             fDetLogic, 
                             false,  
                             fPhysOptReadoutUnitVec.size(), //copyNo                             
                             "-z",
                             fHexPmtProperties 
                             ); 

  fPhysOptReadoutUnitVec.push_back(physOptReadoutUnit);

  
  if( fHexPmtProperties->IsAttachedBothEnd() )
  {
  
    pmtPosition.setZ(-pmtPosition.getZ());
    //+z axis
    physOptReadoutUnit = new  NuSDHexPmtReadoutUnit(0, 
                              pmtPosition, 
                              fDetLogic, 
                              false,  
                              fPhysOptReadoutUnitVec.size(), //copyNo
                              "z",
                              fHexPmtProperties 
                              ); 

    fPhysOptReadoutUnitVec.push_back(physOptReadoutUnit);	
 
  }
  
     

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHspDetConstruction::PlaceDetInWorld()
{

  const G4double zWorld[2]      = { -0.5*fWorldSizeZ, 0.5*fWorldSizeZ };
  const G4double rInner[2]      = { 0*cm, 0*cm };
  const G4double rOuterWorld[2] = { fWorldTanDisToOuterSur, fWorldTanDisToOuterSur }; 
 
  G4VSolid *worldSolid          = new G4Polyhedra("DetSolid",0.*deg, 360.*deg, 6, 2, zWorld, rInner, rOuterWorld);
  G4LogicalVolume *worldLogic   = new G4LogicalVolume(worldSolid, GetMaterial("Air",true), "WorldLogic", 0, 0, 0);
   
   //Place world
  fPhysWorld                    = new G4PVPlacement(0, G4ThreeVector(), worldLogic, "WorldPV", 0, false, fCheckOverlaps); 
   
  //-------------------PLACEMENT------------------------------------------
  fPhysDet                      =  new G4PVPlacement(0,                     //no rotation
                                       G4ThreeVector(0,0,0),       //at (0,0,0)
                                       fDetLogic,         //its logical volume
                                       "DetPV",               //its name
                                       worldLogic,                     //its mother  volume
                                       false,                 //no boolean operation
                                       0,                     //copy number
                                       fCheckOverlaps);        //overlaps checking   
  

 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHspDetConstruction::SetVisProp()
{ 
  NuSDVDetConstruction::SetVisProp();
  
  G4VisAttributes* visAttributes{nullptr};
  
  //Reflector
  visAttributes = new G4VisAttributes( G4Colour::Yellow());
  visAttributes->SetForceSolid(false);
  fReflectorLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
  //LiquidTank
  visAttributes = new G4VisAttributes( G4Colour::Brown());
  visAttributes->SetForceSolid(false);
  fLiquidTankLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//invoked by EndOfRunAction method of NuSDRunAction class
void NuSDHspDetConstruction::Print() const 
{ 
  
  NuSDVDetConstruction::Print();
  G4cout<<"Scnt side length (cm) : "<<fNuScntSideLength*(1/cm)<<G4endl;
  G4cout<<"Reflector thickness (um) : "<<fReflectorThick*(1/um)<<G4endl;
  G4cout<<"LiquidTank thickness (cm) : "<<fLiquidTankThick*(1/cm)<<G4endl;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//----------------UI Commands------------------------------------------------------
void NuSDHspDetConstruction::SetReflectorThickness(G4double thickness)
{
  fReflectorThick = thickness;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void NuSDHspDetConstruction::SetLiquidTankThickness(G4double thickness)
{
  fLiquidTankThick = thickness;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void NuSDHspDetConstruction::SetNuScntSideLength(G4double sl)
{
  fNuScntSideLength = sl;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  
}

