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
#include "NuSDGroove.hh"
#include "NuSDSolidDetConstruction.hh"
#include "NuSDFiberProperties.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SystemOfUnits.hh"
#include "NuSDMaterialFactory.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDGroove::NuSDGroove(G4RotationMatrix *pRot,
                       const G4ThreeVector &tlate,
                       G4LogicalVolume *pMotherLogical,
                       G4bool pMany,
                       G4int pCopyNo,
                       char axis,
                       NuSDSolidDetConstruction* dc
                       )
                             
: G4PVPlacement(pRot,
  tlate,
  new G4LogicalVolume(new G4Box("temp",1,1,1),GetMaterial("Air",true), "temp",0,0,0),
  "GroovePV",
  pMotherLogical,
  pMany,
  pCopyNo
  ), 
fGroovePosVec{tlate},				       
fPlacedAxis{axis},
fCoreLogic{nullptr}, fClad1Logic{nullptr}, fClad2Logic{nullptr}, fGrooveLogic(pMotherLogical), 
fSolidDet(dc)

{
  Construct();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDGroove::Construct()
{
  DefineComponentsDimensions();
  ConstructComponens();
  SetVisProp();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDGroove::DefineComponentsDimensions()
{
   
	if(fPlacedAxis =='x')
	{ //fiber extends along x direction. put center of fiber on the z axis.
	  
	  fCoreSizeX = fSolidDet->GetUnitSizeX(); //or AluSizX = UnitSizeX
	  fCoreSizeY = fSolidDet->GetFiberProperties()->GetCoreSizeY();
	  fCoreSizeZ = fSolidDet->GetFiberProperties()->GetCoreSizeZ();

		fClad1SizeX = fCoreSizeX;
		fClad1SizeY = fCoreSizeY  + 2*fSolidDet->GetFiberProperties()->GetClad1Thickness();
		fClad1SizeZ = fCoreSizeZ  + 2*fSolidDet->GetFiberProperties()->GetClad1Thickness();

		fClad2SizeX = fCoreSizeX;
		fClad2SizeY = fClad1SizeY + 2*fSolidDet->GetFiberProperties()->GetClad2Thickness();
		fClad2SizeZ = fClad1SizeZ + 2*fSolidDet->GetFiberProperties()->GetClad2Thickness();
		    
		fGrooveSizeX = fCoreSizeX;
		fGrooveSizeY = fClad2SizeY+ 2*fSolidDet->GetFiberProperties()->GetGapThickness();
		fGrooveSizeZ = fClad2SizeZ+ 2*fSolidDet->GetFiberProperties()->GetGapThickness();

    G4double distGrooveToWall = fSolidDet->GetFiberProperties()->GetGrooveDistanceToWall();
		G4double distAluCenterToGrooveCenterAlongZ = -fSolidDet->GetNuScntSizeZ()*0.5 + fGrooveSizeZ*0.5 + distGrooveToWall;
		fGroovePosVec = G4ThreeVector (0, 0, distAluCenterToGrooveCenterAlongZ );

		this->SetTranslation(fGroovePosVec);

	}else if(fPlacedAxis == 'z')
	{ //fiber extends along z direction, put center of fiber on the z axis.

		fCoreSizeX = fSolidDet->GetFiberProperties()->GetCoreSizeX();
		fCoreSizeY = fSolidDet->GetFiberProperties()->GetCoreSizeY();
		fCoreSizeZ = fSolidDet->GetUnitSizeZ(); //RefSizeX = UnitSizeX 

		fClad1SizeX = fCoreSizeX + 2*fSolidDet->GetFiberProperties()->GetClad1Thickness();
		fClad1SizeY = fCoreSizeY + 2*fSolidDet->GetFiberProperties()->GetClad1Thickness();
		fClad1SizeZ = fCoreSizeZ;

		fClad2SizeX = fClad1SizeX + 2*fSolidDet->GetFiberProperties()->GetClad2Thickness();
		fClad2SizeY = fClad1SizeY + 2*fSolidDet->GetFiberProperties()->GetClad2Thickness();
		fClad2SizeZ = fCoreSizeZ;
		  
		fGrooveSizeX = fClad2SizeX+ 2*fSolidDet->GetFiberProperties()->GetGapThickness(); 
		fGrooveSizeY = fClad2SizeY+ 2*fSolidDet->GetFiberProperties()->GetGapThickness(); 
		fGrooveSizeZ = fCoreSizeZ;

		G4double distBetweenGroovesAlongY = fSolidDet->GetFiberProperties()->GetDistanceBetweenGrooves();
    G4double distGrooveToWall = fSolidDet->GetFiberProperties()->GetGrooveDistanceToWall();
    
		G4double distAluCenterToGrooveCenterAlongY = -(fGrooveSizeY + distBetweenGroovesAlongY);
		G4double distAluCenterToGrooveCenterAlongX = -fSolidDet->GetNuScntSizeX()*0.5 + fGrooveSizeX*0.5 + distGrooveToWall;
		fGroovePosVec = G4ThreeVector(distAluCenterToGrooveCenterAlongX, distAluCenterToGrooveCenterAlongY , 0);  

		this->SetTranslation(fGroovePosVec);
	}
	  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDGroove::ConstructComponens()
{
  //Groove
  G4Box* grooveSolid  = new G4Box("GrooveSolid", 0.5*fGrooveSizeX, 0.5*fGrooveSizeY, 0.5*fGrooveSizeZ);
  fGrooveLogic        = new G4LogicalVolume(grooveSolid, GetMaterial("Air",true), "GrooveLogic",0,0,0);
 
  SetLogicalVolume(fGrooveLogic);
  
  // The Fiber
  G4Box* coreSolid    = new G4Box("CoreSolid", 0.5*fCoreSizeX, 0.5*fCoreSizeY, 0.5*fCoreSizeZ);
  fCoreLogic          = new G4LogicalVolume(coreSolid, GetMaterial("Polystyrene",true), "CoreLogic",0,0,0);
 
  // Cladding (first layer)
  G4Box* clad1Solid   = new G4Box("Clad1Solid", 0.5*fClad1SizeX, 0.5*fClad1SizeY, 0.5*fClad1SizeZ);
  fClad1Logic         = new G4LogicalVolume(clad1Solid, GetMaterial("Pmma",true), "Clad1Logic",0,0,0);
 
  // Cladding (second layer)
  G4Box* clad2Solid   = new G4Box("Clad2Solid", 0.5*fClad2SizeX, 0.5*fClad2SizeY, 0.5*fClad2SizeZ);
  fClad2Logic         = new G4LogicalVolume(clad2Solid, GetMaterial("FluorinatedAcrylic",true), "Clad2Logic",0,0,0);
   
 //-------------------PLACEMENT---------------------------
   new G4PVPlacement(0,
       G4ThreeVector(0.,0.,0.),
       fCoreLogic,
       "CorePV", 
       fClad1Logic,
       false,
       0,
       true);
  
   new G4PVPlacement(0,
       G4ThreeVector(0.,0.,0.),
       fClad1Logic,
      "Clad1PV",
       fClad2Logic,
       false,
       0,
       true);
                   
  new G4PVPlacement(0,
      G4ThreeVector(0.,0.,0.),
      fClad2Logic,
      "Clad2PV",
      fGrooveLogic,
      false,
      0,
      true);                  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDGroove::SetVisProp()
{	
  G4VisAttributes* visAttributes{nullptr};

	if(fCoreLogic)
	{
		visAttributes = new G4VisAttributes(G4Colour::Brown() );
		visAttributes->SetForceSolid(true);
		fCoreLogic->SetVisAttributes(visAttributes);
		fSolidDet->GetVisVec().push_back(visAttributes);
	}

	if(fClad1Logic)
	{
		visAttributes = new G4VisAttributes(G4Colour::Grey() );
		visAttributes->SetForceSolid(true);
		fClad1Logic->SetVisAttributes(visAttributes);
		fSolidDet->GetVisVec().push_back(visAttributes);
	}

	if(fClad2Logic)
	{
		visAttributes = new G4VisAttributes(G4Colour::Cyan() );
		visAttributes->SetForceSolid(true);
		fClad2Logic->SetVisAttributes(visAttributes);
		fSolidDet->GetVisVec().push_back(visAttributes);
	}

	 
	if(fGrooveLogic)
	{
		visAttributes = new G4VisAttributes(G4Colour::White() );
		visAttributes->SetForceSolid(true);
		fGrooveLogic->SetVisAttributes(visAttributes);
		fSolidDet->GetVisVec().push_back(visAttributes);
	}


}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* NuSDGroove::GetMaterial(const G4String& name, G4bool enableOpticalProperty) 
{
  G4Material* material = NuSDMaterialFactory::GetInstance()->GetMaterial(name,enableOpticalProperty);
  return material;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

