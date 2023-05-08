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

#include "NuSDSiPMReadoutUnit.hh"
#include "NuSDSiPMProperties.hh"
#include "NuSDFiberProperties.hh"
#include "NuSDPhotoSensorSurface.hh"
#include "NuSDGroove.hh"

#include "globals.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSiPMReadoutUnit::NuSDSiPMReadoutUnit(G4RotationMatrix *pRot,
                                         const G4ThreeVector &tlate,
                                         G4LogicalVolume *pMotherLogic,
                                         G4bool pMany,
                                         G4int pCopyNo,
                                         const G4String& placedAxis,
                                         NuSDSiPMProperties* siPMProperties
                                         )
                     
: NuSDVOptReadoutUnit(pRot,
  tlate, 
  new G4LogicalVolume(new G4Box("temp",1,1,1), G4Material::GetMaterial("G4_Galactic"), "temp",0,0,0),
  "SiPMPV",
  pMotherLogic,
  pMany,
  pCopyNo,
  placedAxis
  ),
fSiPMProperties{siPMProperties},    
fPSBinderSizeX{0}, fPSBinderSizeY{0.}, fPSBinderSizeZ{0.},
fPSSizeX{0.}, fPSSizeY{0.}, fPSSizeZ{0.},
fUnitSizeX{0.}, fUnitSizeY{0.}, fUnitSizeZ{0.}
  
{
	Construct();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDSiPMReadoutUnit::~NuSDSiPMReadoutUnit()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSiPMReadoutUnit::Construct()
{ 
	DefineComponentsDimensions(); 
  ConstructComponents();
  SetVisProp();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSiPMReadoutUnit::DefineComponentsDimensions()
{
  G4double psBinderLength = fSiPMProperties->GetPSBinderLength();
  G4double psLength       = fSiPMProperties->GetPSLength();
  G4double unitLength     = fSiPMProperties->GetSiPMReadoutUnitLength();
      
  if(fPlacedAxis == "x" || fPlacedAxis == "-x")  
  {
    fUnitSizeX      = unitLength;
    fUnitSizeY      = fDetConstruction->GetFiberProperties()->GetGrooveSizeY();
    fUnitSizeZ      = fDetConstruction->GetFiberProperties()->GetGrooveSizeZ();

    fPSBinderSizeX  = psBinderLength; 
    fPSBinderSizeY  = fSiPMProperties->GetPSSizeY();
    fPSBinderSizeZ  = fSiPMProperties->GetPSSizeZ();

    fPSSizeX        = psLength;
    fPSSizeY        = fPSBinderSizeY;
    fPSSizeZ        = fPSBinderSizeZ;

    //Binder and PS Postion wrt to SiPM PhysicalVolume
    G4ThreeVector siPMStartPos(fUnitSizeX*0.5,0,0); 
    fPSBinderPos = siPMStartPos - G4ThreeVector(psBinderLength*0.5,0,0);
    fPSPos       = siPMStartPos - G4ThreeVector(psBinderLength+psLength*0.5,0,0);

    if(fPlacedAxis == "x") 
    {
      fPSBinderPos *=-1;
      fPSPos       *=-1;
    }


  }else if(fPlacedAxis =="z" || fPlacedAxis == "-z")
  {

    fUnitSizeX      = fDetConstruction->GetFiberProperties()->GetGrooveSizeX();
    fUnitSizeY      = fDetConstruction->GetFiberProperties()->GetGrooveSizeY();
    fUnitSizeZ      = unitLength;

    fPSBinderSizeX  = fSiPMProperties->GetPSSizeX();
    fPSBinderSizeY  = fSiPMProperties->GetPSSizeY();
    fPSBinderSizeZ  = psBinderLength;

    fPSSizeX        = fPSBinderSizeX;
    fPSSizeY        = fPSBinderSizeY;
    fPSSizeZ        = psLength;

    //Binder and PS Postion wrt to SiPM PhysicalVolume
    G4ThreeVector siPMStartPos(0,0,fUnitSizeZ*0.5); //Wrt pmt center
    fPSBinderPos = siPMStartPos - G4ThreeVector(0,0,psBinderLength*0.5);
    fPSPos       = siPMStartPos - G4ThreeVector(0,0,psBinderLength+psLength*0.5);

    if( fPlacedAxis == "z" ) 
    {
      fPSBinderPos *=-1;
      fPSPos       *=-1;
    }
  
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSiPMReadoutUnit::ConstructComponents()
{
  //----------------------------------SiPMREADOUTUNIT------------------------------
	G4Box *siPMSolid            = new G4Box ("SiPMSolid", 0.5*fUnitSizeX, 0.5*fUnitSizeY, 0.5*fUnitSizeZ);
	G4LogicalVolume* unitLogic  = new G4LogicalVolume(siPMSolid, GetMaterial("Air",true), "SiPMLogic",0,0,0);

  SetLogicalVolume(unitLogic);  //this method inherited from G4PVPlacement->G4VPhysicalVolume

  new G4LogicalSkinSurface("SiPMTubeSkinSurface", unitLogic , fDetConstruction->GetReflectorSurface() );
  
  //----------------------------------(PSBinder)---------------------------------------
  if(fSiPMProperties->IsPSBinderOn())
  {
    G4Box *psBinderSolid            = new G4Box ("BinderSolid", 0.5*fPSBinderSizeX, 0.5*fPSBinderSizeY, 0.5*fPSBinderSizeZ); 
	  G4LogicalVolume *psBinderLogic  = new G4LogicalVolume(psBinderSolid, GetMaterial("Grease",true), "BinderLogic",0,0,0);
	  
	  fPhysPSBinder                   = new G4PVPlacement(0,   //no rotation
		                                      fPSBinderPos,      
		                                      psBinderLogic,     //its logical volume
		                                      "PSBinderPV",      //its name
		                                      unitLogic,        //its mother  volume
		                                      false,             //no boolean operation
		                                      fCopyNo,           //copy number
		                                      fCheckOverlaps);   //overlaps checking  

  }
  
	//----------------------------------Photosensor(MPPC)-------------------------------------
	G4Box *psSolid            = new G4Box ("PhotonDetSolid", 0.5*fPSSizeX, 0.5*fPSSizeY, 0.5*fPSSizeZ); 
	fPSLogic                  = new G4LogicalVolume(psSolid, GetMaterial("G4_Al"), "PhotonDetLogic",0,0,0);

	fPhysPS                   = new G4PVPlacement(fRot, 
		                              fPSPos,        //no rotation
		                              fPSLogic,          //its logical volume
		                              "PhotonSDPV",     //its name
		                              unitLogic,       //its mother  volume
		                              false,            //no boolean operation
		                              fCopyNo,          //copy number
		                              fCheckOverlaps);  //overlaps checking 
  
  //----------------------SETPSSurface-------------------------------------- 
  if(!fPSSurface)
  { 
		fPSSurface = new NuSDPhotoSensorSurface("Mppc", //do not change name!!
							       glisur,   //model
		                 polished, // surfaceFinish 
			               dielectric_metal, //type
				             fSiPMProperties
				             );  
	}				             
	            
  new G4LogicalSkinSurface("PSSentitiveSurface", fPSLogic, fPSSurface );  		                              

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDSiPMReadoutUnit::SetVisProp()
{
	NuSDVOptReadoutUnit::SetVisProp(fSiPMProperties);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


