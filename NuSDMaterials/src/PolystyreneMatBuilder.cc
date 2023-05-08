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

#include "PolystyreneMatBuilder.hh"

#include "G4Material.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"
#include "G4NistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PolystyreneMatBuilder::PolystyreneMatBuilder(G4bool enableOpticalProperty):
VMaterialBuilder("Polystyrene",enableOpticalProperty) 
{
  if(IsOpticalPropertyEnabled())
	{
		pMatPropTable = new MaterialPropertiesTable(pName); 

		G4UImanager* uImanager = G4UImanager::GetUIpointer();
		uImanager->ApplyCommand("/control/execute NuSDMaterials/macros/polystyrene.mac");  
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PolystyreneMatBuilder::~PolystyreneMatBuilder()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* PolystyreneMatBuilder::Build()          
{ 
  // BC91a: Our standard cores consist of a polystyrene-based core and a PMMA cladding
  
  G4double hydrogenAtomDensity = 4.82e+22*(1./cm3);
  G4double carbonAtomDensity   = 4.85e+22*(1./cm3);

  G4double hydrogenMolarMass   = 1.00794*(g/mole);
  G4double carbonMolarMass     = 12.0107*(g/mole);
  
	G4double hydrogenMassDensity = (hydrogenAtomDensity/CLHEP::Avogadro)*hydrogenMolarMass;
	G4double carbonMassDensity   = (carbonAtomDensity/CLHEP::Avogadro)*carbonMolarMass;
	
	G4double density 						 = hydrogenMassDensity+carbonMassDensity;
	
	G4double hydrogenMassFraction = hydrogenMassDensity/density;
	G4double carbonMassFraction 	= carbonMassDensity/density;
  
  // G4double density = 1.05*g/cm3;
  
	G4Material *mat = new G4Material(pName, density, 2);
  mat->AddElement(pNistManager->FindOrBuildElement("H"), hydrogenMassFraction);
  mat->AddElement(pNistManager->FindOrBuildElement("C"), carbonMassFraction);
   
  return mat;  

}

