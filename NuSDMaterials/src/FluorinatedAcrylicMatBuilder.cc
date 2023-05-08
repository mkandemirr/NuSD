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

#include "FluorinatedAcrylicMatBuilder.hh"

#include "G4Material.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"
#include "G4NistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FluorinatedAcrylicMatBuilder::FluorinatedAcrylicMatBuilder(G4bool enableOpticalProperty):
VMaterialBuilder("FluorinatedAcrylic", enableOpticalProperty) 
{
  if(IsOpticalPropertyEnabled())
	{
		pMatPropTable = new MaterialPropertiesTable(pName); 

		G4UImanager* uImanager = G4UImanager::GetUIpointer();
		uImanager->ApplyCommand("/control/execute NuSDMaterials/macros/fluorinatedAcrylic.mac");  
	}
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FluorinatedAcrylicMatBuilder::~FluorinatedAcrylicMatBuilder()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* FluorinatedAcrylicMatBuilder::Build()          
{ 
  G4double density  = 1.2*g/cm3;
  G4int nComponents = 3;
  
  G4Material *mat = new G4Material(pName, density, nComponents); 
  mat->AddElement(pNistManager->FindOrBuildElement("C"), 5);
  mat->AddElement(pNistManager->FindOrBuildElement("H"), 8);
  mat->AddElement(pNistManager->FindOrBuildElement("O"), 2);
  return mat;
}

