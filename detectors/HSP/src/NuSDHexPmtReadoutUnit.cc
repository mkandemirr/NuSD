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

#include "NuSDHexPmtReadoutUnit.hh"
#include "NuSDPhotoSensorSurface.hh"
#include "NuSDHexPmtProperties.hh"

#include "globals.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Polyhedra.hh"

#include "G4LogicalSkinSurface.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDHexPmtReadoutUnit::NuSDHexPmtReadoutUnit(G4RotationMatrix *pRot,
                                             const G4ThreeVector &tlate,
                                             G4LogicalVolume *pMotherLogic,
                                             G4bool pMany,
                                             G4int pCopyNo,
                                             const G4String& placedAxis,
                                             NuSDHexPmtProperties* hexPmtProperties
                                             )
: NuSDVOptReadoutUnit(pRot,
  tlate, 
  new G4LogicalVolume(new G4Box("temp",1,1,1), G4Material::GetMaterial("G4_Galactic"), "temp",0,0,0),
  "PmtPV", //PV name
  pMotherLogic,
  pMany,
  pCopyNo,
  placedAxis
  ),  
                     
  
fHexPmtProperties{hexPmtProperties}, 		                    
fLgRot{pRot},
fPhysLg{nullptr}, fPhysLgBinder{nullptr},  
fLgPos(), fLgBinderPos(),
fLgBinderSizeZ{0.}, fLgSizeZ{0.}, fUnitSizeZ{0.},
fLgTanDisToOuterSur{0.}, fLgBinderTanDisToOuterSur{0.}, fUnitTanDisToOuterSur{0.}
 
{ 
  Construct();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDHexPmtReadoutUnit::~NuSDHexPmtReadoutUnit()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHexPmtReadoutUnit::Construct()
{ 
 
  DefineOptReadoutCompDimensions(); 
  ConstructComponents();
  SetVisProp();
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHexPmtReadoutUnit::DefineOptReadoutCompDimensions()
{                		

  //copy values
  G4double lgLength       = fHexPmtProperties->GetLgLength();
  G4double lgBinderLength = fHexPmtProperties->GetLgBinderLength();
  G4double psBinderLength = fHexPmtProperties->GetPSBinderLength();
  G4double psLength       = fHexPmtProperties->GetPSLength();
  G4double unitLength     = fHexPmtProperties->GetHexPmtReadoutUnitLength();
  
  if(!fHexPmtProperties->IsLgBinderOn())   lgBinderLength  = 0.;
  if(!fHexPmtProperties->IsLgOn())         lgLength        = 0.;  
  if(!fHexPmtProperties->IsPSBinderOn())   psBinderLength  = 0.;    
   
  
  if(fPlacedAxis =="z" || fPlacedAxis == "-z")
  {
        
    fUnitTanDisToOuterSur 	  = fDetConstruction->GetUnitTanDisToOuterSur();
    fLgBinderTanDisToOuterSur = fDetConstruction->GetNuScntTanDisToOuterSur();
    fLgTanDisToOuterSur       = fLgBinderTanDisToOuterSur;
         
    fUnitSizeZ 			  = unitLength;
    fLgBinderSizeZ    = lgBinderLength;
    fLgSizeZ          = lgLength;  
          
    //Binder, Lg and ps position wrt to Pmt PhysicalVolume 
    G4ThreeVector unitStartingPos(0,0,fUnitSizeZ*0.5); //Wrt pmtunit center
    fLgBinderPos 		  = unitStartingPos - G4ThreeVector(0,0,lgBinderLength*0.5);
    fLgPos 					  = unitStartingPos - G4ThreeVector(0,0,lgBinderLength+lgLength*0.5);
    fPSBinderPos 	    = unitStartingPos - G4ThreeVector(0,0,lgBinderLength+lgLength+psBinderLength*0.5);
    fPSPos            = unitStartingPos - G4ThreeVector(0,0,lgBinderLength+lgLength+psBinderLength+   
                        psLength*0.5);
      
    if( fPlacedAxis == "z" ) //positive side
    {
      fLgBinderPos 	*=-1;
      fLgPos 				*=-1;
      fPSBinderPos 	*=-1;
      fPSPos	      *=-1;

      fLgRot = new G4RotationMatrix;
      fLgRot->rotateX(180*deg);   

    }

  }
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHexPmtReadoutUnit::ConstructComponents()
{	 
  
  //copy values
  G4double lgLength           = fHexPmtProperties->GetLgLength();
  G4double lgBinderLength     = fHexPmtProperties->GetLgBinderLength();
  G4double psBinderLength     = fHexPmtProperties->GetPSBinderLength();
  G4double psLength           = fHexPmtProperties->GetPSLength();
  G4double unitLength         = fHexPmtProperties->GetHexPmtReadoutUnitLength();
  G4double psTanDisToOuterSur = fHexPmtProperties->GetPSTanDisToOuterSur();
  
  const G4double rInner[2] 			   = { 0*cm, 0*cm };
  const G4double rOuterBinder[2]   = { fLgBinderTanDisToOuterSur, fLgBinderTanDisToOuterSur };
  const G4double rOuterBinder1[2]  = { psTanDisToOuterSur, psTanDisToOuterSur };
  const G4double rOuterPS[2]       = { psTanDisToOuterSur, psTanDisToOuterSur };
  const G4double rOuterPmt[2]      = { fUnitTanDisToOuterSur, fUnitTanDisToOuterSur };

  const G4double zPmt[2]           = {-0.5*unitLength, 0.5*unitLength};
  const G4double zLg[2]            = {-0.5*lgLength, 0.5*lgLength};
  const G4double zBinder[2]        = {-0.5*lgBinderLength, 0.5*lgBinderLength};
  const G4double zBinder1[2]       = {-0.5*psBinderLength, 0.5*psBinderLength};
  const G4double zPS[2]            = { 0.5*psLength, -0.5*psLength};
  
    
  //----------------------------------PMTREADOUTUNIT---------------------------------------

  G4Polyhedra *unitSolid     = new G4Polyhedra("PmtSolid",0*deg, 360*deg, 6, 2, zPmt, rInner, rOuterPmt);
  G4LogicalVolume* unitLogic = new G4LogicalVolume(unitSolid, GetMaterial("G4_Galactic"), "PmtReadOutUnitLogic",0,0,0);

  SetLogicalVolume(unitLogic); //this method inherited from G4PVPlacement->G4VPhysicalVolume
  
  new G4LogicalSkinSurface("HexaPmtTubeSkinSurface", unitLogic , fDetConstruction->GetReflectorSurface() );
  
  //----------------------------------LgBinder---------------------------------------
   if(fHexPmtProperties->IsLgBinderOn())
    {
      
      G4Polyhedra* lgBinderSolid      = new G4Polyhedra("LgBinderSolid",0*deg, 360*deg, 6, 2, zBinder, rInner, rOuterBinder);
      G4LogicalVolume* lgBinderLogic  = new G4LogicalVolume(lgBinderSolid, GetMaterial("Cement",true), "LgBinderLogic",0,0,0);
      
      fPhysLgBinder                   = new G4PVPlacement(0,  //no rotation
                                            fLgBinderPos,      
                                            lgBinderLogic,    //its logical volume
                                            "LgBinderPV",     //its name
                                            unitLogic,       //its mother  volume
                                            false,            //no boolean operation
                                            fCopyNo,          //copy number
                                            fCheckOverlaps);  //overlaps checking      
    }
    
  //----------------------------------LightGuide---------------------------------------
  if(fHexPmtProperties->IsLgOn())
  {
      
    G4Polyhedra *lgSolid      = new G4Polyhedra("LgSolid",0*deg, 360*deg, 6, 2, zLg, rInner, rOuterBinder);
    G4LogicalVolume *lgLogic  = new G4LogicalVolume(lgSolid, GetMaterial("Pmma",true), "LightGuideLogic",0,0,0);
    
    fPhysLg                   = new G4PVPlacement(fLgRot, 
                                    fLgPos,      //at (0,0,0)
                                    lgLogic,      //its logical volume
                                    "LightGuidePV",   //its name
                                    unitLogic,      //its mother  volume
                                    false,      //no boolean operation
                                    fCopyNo,  //copy number
                                    fCheckOverlaps);  //overlaps checking 
      
       
  }
     
  //----------------------------------(PSBinder)---------------------------------------
  if(fHexPmtProperties->IsPSBinderOn())
  {
	  	
    G4Polyhedra *psBinderSolid      = new G4Polyhedra("PSBinderSolid",0*deg, 360*deg, 6, 2, zBinder1, rInner, rOuterBinder1);
    G4LogicalVolume *psBinderLogic  = new G4LogicalVolume (psBinderSolid, GetMaterial("Grease",true), "PSBinderLogic",0,0,0); 

    fPhysPSBinder                   = new G4PVPlacement(fRot,                     
                                          fPSBinderPos,    
                                          psBinderLogic,         //its logical volume
                                          "PSBinderPV",               //its name
                                          unitLogic,                     //its mother  volume
                                          false,                 //no boolean operation
                                          fCopyNo,                     //copy number
                                          fCheckOverlaps);        //overlaps checking
	
  }
  
  //----------------------------------Photosensor(Photocatode)---------------------------------------
  	
  G4Polyhedra *psSolid      = new G4Polyhedra("PhotonDetSolid",0*deg, 360*deg, 6, 2, zPS, rInner, rOuterPS);
  fPSLogic                  = new G4LogicalVolume(psSolid, GetMaterial("G4_Al"), "PhotonDetLogic",0,0,0);
  
  fPhysPS                   = new  G4PVPlacement(fRot, 
                                   fPSPos,                     //no rotation
                                   fPSLogic,         //its logical volume
                                   "PhotonSDPV",               //its name
                                   unitLogic,                     //its mother  volume
                                   false,                 //no boolean operation
                                   fCopyNo,                     //copy number
                                   fCheckOverlaps);        //overlaps checking
                          
  //----------------------SETPSSurface--------------------------------------  
  if(!fPSSurface)
  {
		fPSSurface = new NuSDPhotoSensorSurface("PhotoCathode", //do not change name!!
					           glisur,   //model
		                 polished, // surfaceFinish 
		                 dielectric_metal, //type
		                 fHexPmtProperties
		                 );  
  }
  new G4LogicalSkinSurface("PSSentitiveSurface", fPSLogic, fPSSurface );                        
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDHexPmtReadoutUnit::SetVisProp()
{
 NuSDVOptReadoutUnit::SetVisProp(fHexPmtProperties);

	G4VisAttributes * visAttributes{nullptr};

  if(fHexPmtProperties->IsLgOn())
  {

    if(fPhysLg)
    {
      visAttributes = new G4VisAttributes(G4Colour::Cyan());      
      visAttributes->SetForceSolid(true);
      fPhysLg->GetLogicalVolume()->SetVisAttributes(visAttributes);
      fDetConstruction->GetVisVec().push_back(visAttributes);
    }

  }
  
  if(fHexPmtProperties->IsLgBinderOn())
  {
    if(fPhysLgBinder )
    {
      visAttributes = new G4VisAttributes(G4Colour::Magenta());  // magenta ));
      visAttributes->SetForceSolid(true);
      fPhysLgBinder->GetLogicalVolume()->SetVisAttributes(visAttributes);
      fDetConstruction->GetVisVec().push_back(visAttributes);
    }
  }     
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



