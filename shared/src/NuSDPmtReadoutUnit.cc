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

#include "NuSD_config.h"

#if DETECTOR_NAME != SOLID && DETECTOR_NAME != HSP 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDPmtReadoutUnit.hh"
#include "NuSDPmtProperties.hh"
#include "NuSDPhotoSensorSurface.hh"
#include "globals.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"

#include "G4LogicalSkinSurface.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPmtReadoutUnit::NuSDPmtReadoutUnit(G4RotationMatrix *pRot,
                                       const G4ThreeVector &tlate,
                                       G4LogicalVolume *pMotherLogic,
                                       G4bool pMany,
                                       G4int pCopyNo,
                                       const G4String& placedAxis,
                                       NuSDPmtProperties* pmtProperties
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
fPmtProperties{pmtProperties},  
fLgRot{pRot},
fPhysLg{nullptr}, fPhysLgBinder{nullptr},   
fLgPos(), fLgBinderPos(),
fLgSizeX{0.}, fLgSizeY{0.}, fLgSizeZ{0.},
fLgBinderSizeX{0.}, fLgBinderSizeY{0.}, fLgBinderSizeZ{0.},
fUnitSizeX{0.}, fUnitSizeY{0.}, fUnitSizeZ{0.}
 
{       
  Construct();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPmtReadoutUnit::~NuSDPmtReadoutUnit()
{      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPmtReadoutUnit::Construct()
{ 
  DefineComponentsDimensions(); 
  ConstructComponents();
  SetVisProp();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPmtReadoutUnit::DefineComponentsDimensions()
{
  //copy values
  G4double lgLength       = fPmtProperties->GetLgLength();
  G4double lgBinderLength = fPmtProperties->GetLgBinderLength();
  G4double psBinderLength = fPmtProperties->GetPSBinderLength();
  G4double psLength       = fPmtProperties->GetPSLength();
  G4double unitLength     = fPmtProperties->GetPmtReadoutUnitLength();
                       		
  if(fPlacedAxis == "y" || fPlacedAxis == "-y") 
  {
    fUnitSizeX      = fDetConstruction->GetUnitSizeX(); //Pmt ağzı ayarı!!!
    fUnitSizeY      = unitLength;
    fUnitSizeZ      = fDetConstruction->GetUnitSizeZ(); //Pmt ağzı ayarı!!!
       
    fLgBinderSizeX  = fDetConstruction->GetNuScntSizeX(); //binder scin'e göre ayarlanıyor! 
    fLgBinderSizeY  = lgBinderLength; 
    fLgBinderSizeZ  = fDetConstruction->GetNuScntSizeZ();
    
    fLgSizeX        = fDetConstruction->GetNuScntSizeX(); 
    fLgSizeY        = lgLength;
    fLgSizeZ        = fDetConstruction->GetNuScntSizeZ();
           
    //Binder, Lg, Binder1 and ps position wrt to Pmt PhysicalVolume
    G4ThreeVector unitStartingPos(0,fUnitSizeY*0.5,0); //Wrt pmt center!!
    
    
    fLgBinderPos    = unitStartingPos - G4ThreeVector(0,lgBinderLength*0.5,0);
    fLgPos          = unitStartingPos - G4ThreeVector(0,lgBinderLength+lgLength*0.5,0);
    fPSBinderPos    = unitStartingPos - G4ThreeVector(0,lgBinderLength+lgLength+psBinderLength*0.5,0);
    fPSPos          = unitStartingPos - G4ThreeVector(0,lgBinderLength+lgLength+psBinderLength +    
                      psLength*0.5, 0);   
     
    fRot = new G4RotationMatrix;
    fRot->rotateX(90*deg);
    
    fLgRot = new G4RotationMatrix;
    fLgRot->rotateX(90*deg); 

    if(fPlacedAxis == "y") 
    {
      fLgBinderPos 	 *=-1;
      fLgPos 				 *=-1;
      fPSBinderPos 	 *=-1;
      fPSPos	       *=-1;

      fLgRot = new G4RotationMatrix;
      fLgRot->rotateX(-90*deg);

    }


  }else if(fPlacedAxis == "x" || fPlacedAxis == "-x") 
  {    
    fUnitSizeX      = unitLength;
    fUnitSizeY      = fDetConstruction->GetUnitSizeY(); //Pmt ağzı ayarı!!!
    fUnitSizeZ      = fDetConstruction->GetUnitSizeZ(); //Pmt ağzı ayarı!!!

    fLgBinderSizeX  = lgBinderLength; 
    fLgBinderSizeY  = fDetConstruction->GetNuScntSizeY(); //binder scin'e göre ayarlanıyor!
    fLgBinderSizeZ  = fDetConstruction->GetNuScntSizeZ();

    fLgSizeX        =  lgLength;
    fLgSizeY        =  fDetConstruction->GetNuScntSizeY();
    fLgSizeZ        =  fDetConstruction->GetNuScntSizeZ();
      
    //Binder, Lg and ps position wrt to Pmt 
    G4ThreeVector unitStartingPos(fUnitSizeX*0.5,0,0); //Wrt pmt center!!
    fLgBinderPos 		= unitStartingPos - G4ThreeVector(lgBinderLength*0.5,0,0);
    fLgPos 					= unitStartingPos - G4ThreeVector(lgBinderLength+lgLength*0.5,0,0);
    fPSBinderPos    = unitStartingPos - G4ThreeVector(lgBinderLength+lgLength+psBinderLength*0.5,0,0);
    fPSPos          = unitStartingPos - G4ThreeVector(lgBinderLength+lgLength+psBinderLength + 
                      psLength*0.5,0,0);
    
    fRot = new G4RotationMatrix;
    fRot->rotateY(90*deg);
    
    fLgRot = new G4RotationMatrix;
    fLgRot->rotateY(-90*deg); 

    if(fPlacedAxis == "x") //positive side
    {

      fLgBinderPos 	*=-1;
      fLgPos 				*=-1;
      fPSBinderPos 	*=-1;
      fPSPos	      *=-1;

      fLgRot = new G4RotationMatrix;
      fLgRot->rotateY(90*deg);

    }

  }else if(fPlacedAxis =="z" || fPlacedAxis == "-z")
  {    
    fUnitSizeX 			= fDetConstruction->GetUnitSizeX(); //Pmt ağzı ayarı!!!
    fUnitSizeY 			= fDetConstruction->GetUnitSizeY(); //Pmt ağzı ayarı!!!
    fUnitSizeZ 			= unitLength;
         
    fLgBinderSizeX  = fDetConstruction->GetNuScntSizeX(); //binder scin'e göre ayarlanıyor!
    fLgBinderSizeY  = fDetConstruction->GetNuScntSizeY(); 
    fLgBinderSizeZ  = lgBinderLength;
    
    fLgSizeX 				= fDetConstruction->GetNuScntSizeX(); 
    fLgSizeY 				= fDetConstruction->GetNuScntSizeY();
    fLgSizeZ        = lgLength;  
          
    //Binder, Lg and ps position wrt to Pmt PhysicalVolume 
    G4ThreeVector unitStartingPos(0,0,fUnitSizeZ*0.5); //Wrt pmtunit center
    fLgBinderPos 		= unitStartingPos - G4ThreeVector(0,0,lgBinderLength*0.5);
    fLgPos 					= unitStartingPos - G4ThreeVector(0,0,lgBinderLength+lgLength*0.5);
    fPSBinderPos 	  = unitStartingPos - G4ThreeVector(0,0,lgBinderLength+lgLength+psBinderLength*0.5);
    fPSPos          = unitStartingPos - G4ThreeVector(0,0,lgBinderLength+lgLength+psBinderLength+   
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

void NuSDPmtReadoutUnit::ConstructComponents()
{   
  //----------------------------------PMTREADOUTUNIT---------------------------------------
  G4Box  *unitSolid           = new G4Box ("PmtReadoutUnitSolid", 0.5*fUnitSizeX, 0.5*fUnitSizeY, 0.5*fUnitSizeZ);
  G4LogicalVolume *unitLogic  = new G4LogicalVolume(unitSolid, GetMaterial("G4_Galactic"), "PmtReadOutUnitLogic",0,0,0);

  SetLogicalVolume(unitLogic); //this method inherited from G4PVPlacement->G4VPhysicalVolume
  
  if(fDetConstruction->GetReflectorSurface())
    new G4LogicalSkinSurface("PmtTubeSkinSurface", unitLogic , fDetConstruction->GetReflectorSurface() );  
   
  //----------------------------------LgBinder---------------------------------------
  if(fPmtProperties->IsLgBinderOn())
  {
    G4Box *lgBinderSolid           = new G4Box ("LgBinderSolid", 0.5*fLgBinderSizeX, 0.5*fLgBinderSizeY, 0.5*fLgBinderSizeZ);
    G4LogicalVolume* lgBinderLogic = new G4LogicalVolume(lgBinderSolid, GetMaterial("Cement",true), "LgBinderLogic",0,0,0);
    
    fPhysLgBinder                  = new G4PVPlacement(0,                     //no rotation
                                         fLgBinderPos,      
                                         lgBinderLogic,         //its logical volume
                                         "LgBinderPV",               //its name
                                         unitLogic,                     //its mother  volume
                                         false,                 //no boolean operation
                                         fCopyNo,                     //copy number
                                         fCheckOverlaps);        //overlaps checking      
  }
   
  
  //----------------------------------LightGuide---------------------------------------
  if(fPmtProperties->IsLgOn())
  {
  
    G4double lgPmtEndSizeX = fPmtProperties->GetLgPmtEndSizeX();
    G4double lgPmtEndSizeY = fPmtProperties->GetLgPmtEndSizeY();
    G4double lgPmtEndSizeZ = fPmtProperties->GetLgPmtEndSizeZ();
    G4double lgLength      = fPmtProperties->GetLgLength();
    
    G4Trd   *lgSolid{nullptr};
    if(fPlacedAxis == "-x" || fPlacedAxis == "x")
    {
      lgSolid = new G4Trd ("LightGuideSolid", 0.5*lgPmtEndSizeZ, 0.5*fLgSizeZ, 0.5*lgPmtEndSizeY, 
                            0.5*fLgSizeY, 0.5*lgLength); 
    }else if(fPlacedAxis == "-y" || fPlacedAxis == "y")
    {
      lgSolid = new G4Trd ("LightGuideSolid", 0.5*lgPmtEndSizeX, 0.5*fLgSizeX, 0.5*lgPmtEndSizeZ, 
                            0.5*fLgSizeZ, 0.5*lgLength); 
    }else
    { 
      lgSolid = new G4Trd ("LightGuideSolid", 0.5*lgPmtEndSizeX, 0.5*fLgSizeX, 0.5*lgPmtEndSizeY, 
                            0.5*fLgSizeY, 0.5*lgLength); 
    }
          
    G4LogicalVolume *lgLogic = new G4LogicalVolume(lgSolid, GetMaterial("Pmma",true), "LightGuideLogic",0,0,0);

    fPhysLg                  = new G4PVPlacement(fLgRot, 
                                   fLgPos,      //at (0,0,0)
                                   lgLogic,         //its logical volume
                                   "LightGuidePV",               //its name
                                   unitLogic,                     //its mother  volume
                                   false,                 //no boolean operation
                                   fCopyNo,                     //copy number
                                   fCheckOverlaps);        //overlaps checking 
          
  }
     
  
  
  
  //----------------------------------(PSBinder)---------------------------------------
  G4double psRadius = fPmtProperties->GetPSRadius();
  
  if(fPmtProperties->IsPSBinderOn())
  {
    G4double psBinderLength         = fPmtProperties->GetPSBinderLength();
	  	  
    G4Tubs *psBinderSolid           = new G4Tubs("PSBinderSolid", 0, psRadius, 0.5*psBinderLength, 0*deg, 360*deg);
	  G4LogicalVolume *psBinderLogic  = new G4LogicalVolume (psBinderSolid, GetMaterial("Cement",true), "PSBinderLogic",0,0,0); 
    
    fPhysPSBinder                   = new  G4PVPlacement(fRot,                     
                                           fPSBinderPos,    
                                           psBinderLogic,         //its logical volume
                                           "PSBinderPV",               //its name
                                           unitLogic,                     //its mother  volume
                                           false,                 //no boolean operation
                                           fCopyNo,                     //copy number
                                           fCheckOverlaps);        //overlaps checking
	
  }
  
  //----------------------------------Photosensor(Photocatode)---------------------------------------
  G4double psLength   = fPmtProperties->GetPSLength(); //not importatnt just visualization
  
  G4Tubs *psSolid     = new G4Tubs("PhotonDetSolid", 0, psRadius, psLength*0.5, 0*deg, 360*deg); 
  fPSLogic            = new G4LogicalVolume(psSolid, GetMaterial("G4_Al"), "PhotonDetLogic",0,0,0);
  
  
  fPhysPS             = new G4PVPlacement(fRot, 
                            fPSPos,                     //no rotation
                            fPSLogic,         //its logical volume
                            "PhotonSDPV", // dont change this! checking in stepping action
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
				             fPmtProperties
				             );  
 	}		                          
    
  new G4LogicalSkinSurface("PSSentitiveSurface", fPSLogic, fPSSurface );                                
                         	
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPmtReadoutUnit::SetVisProp()
{

	NuSDVOptReadoutUnit::SetVisProp(fPmtProperties);

	G4VisAttributes * visAttributes{nullptr};

  if(fPmtProperties->IsLgOn())
  {

    if(fPhysLg)
    {
      visAttributes = new G4VisAttributes(G4Colour::Cyan());      
      visAttributes->SetForceSolid(true);
      fPhysLg->GetLogicalVolume()->SetVisAttributes(visAttributes);
      fDetConstruction->GetVisVec().push_back(visAttributes);
    }

  }
  
  if(fPmtProperties->IsLgBinderOn())
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

#endif

