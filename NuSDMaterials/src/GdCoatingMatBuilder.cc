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

#include "GdCoatingMatBuilder.hh"

#include "G4Material.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"
#include "G4NistManager.hh"
//#include "MaterialConstructionStore.hh"
//#include "MaterialManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GdCoatingMatBuilder::GdCoatingMatBuilder():
VMaterialBuilder("GdCoating") 
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GdCoatingMatBuilder::~GdCoatingMatBuilder()
{ 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* GdCoatingMatBuilder::Build()          
{
   
//From PANDA experiment(Oguri 2014): "The polyester sheet is obtained from Ask Sanshin Engineering Corp., Ltd. 
//The sheet is made of 50 μm thick polyester film sandwiched in two layers of 25-μm thick Gd2O3 coating. 
//The sheet contains 4.9 mg/cm2 of gadolinium."
//1.125*g/cm3 *50*um*10^-4cm/1um

  G4double  density        = 1.125*g/cm3;
  G4int numberOfComponents = 2; 
  
  G4Material *mat = new G4Material(pName, density, numberOfComponents);
  mat->AddElement(pNistManager->FindOrBuildElement("Gd"), 2);
  mat->AddElement(pNistManager->FindOrBuildElement("O"), 3);
  
  return mat;
}

