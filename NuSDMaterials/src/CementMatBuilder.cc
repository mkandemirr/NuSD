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

#include "CementMatBuilder.hh"

#include "G4Material.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"
#include "G4NistManager.hh"


CementMatBuilder::CementMatBuilder(G4bool enableOpticalProperty):
VMaterialBuilder("Cement", enableOpticalProperty) 
{
	if(IsOpticalPropertyEnabled())
	{
		pMatPropTable = new MaterialPropertiesTable(pName); 

		G4UImanager* uImanager = G4UImanager::GetUIpointer();
		uImanager->ApplyCommand("/control/execute NuSDMaterials/macros/cement.mac");  
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CementMatBuilder::~CementMatBuilder()
{ 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* CementMatBuilder::Build()          
{ 
  G4double density = 1.032*g/cm3;
  G4int nelements  = 2;
  
  G4Material *cement_mat = new G4Material(pName, density, nelements);
  cement_mat->AddElement(pNistManager->FindOrBuildElement("C"), 9);
  cement_mat->AddElement(pNistManager->FindOrBuildElement("H"), 10); 
  
  return cement_mat;  
}

