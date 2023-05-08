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

#include "CustomEJ309MatBuilder.hh"

#include "G4Material.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"
#include "G4NistManager.hh"

CustomEJ309MatBuilder::CustomEJ309MatBuilder(G4bool enableOpticalProperty):
VMaterialBuilder("CustomEJ-309", enableOpticalProperty) 
{
	if(IsOpticalPropertyEnabled())
	{
		pMatPropTable = new MaterialPropertiesTable(pName); 

		G4UImanager* uImanager = G4UImanager::GetUIpointer();
		uImanager->ApplyCommand("/control/execute NuSDMaterials/macros/customEJ-309.mac");  
	}
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CustomEJ309MatBuilder::~CustomEJ309MatBuilder()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* CustomEJ309MatBuilder::Build()          
{ 
  G4double density,a;
  G4int ncomponents,Z,A;
  
  //EnrichedLi  
  G4Isotope *Li6  = new G4Isotope("Li6", Z=3, A=6, a= 6.015122795*g/mole);
  G4Isotope *Li7  = new G4Isotope("Li7", Z=3, A=7, a= 7.01600455*g/mole);

  G4Element* enrichedLi_el  = new G4Element("EnrichedLithium", "Li", ncomponents=2);
  enrichedLi_el->AddIsotope(Li6, 95.*perCent);
  enrichedLi_el->AddIsotope(Li7, 5.*perCent);
    
  //Mass fraction
  G4double fracOfHydrogen     = 9.52*perCent;
  G4double fracOfCarbon       = 84.14*perCent;
  G4double fracOfLi6          = 0.1*perCent;  
  
  //calculate fraction of Enriched Lithium 
  G4double li6MassFraction    = 0.95*6.02/(0.95*6.02+0.05*7.02); //atom ratio = 95/5=15
  G4double fracOfEnrichedLi   = fracOfLi6/li6MassFraction;
  G4double fracOfOxygen       = 1-(fracOfCarbon+fracOfHydrogen+fracOfEnrichedLi);
   
  G4Material *scin_mat = new G4Material(pName, density=0.979*g/cm3, ncomponents=4);
  scin_mat->AddElement(pNistManager->FindOrBuildElement("H"), fracOfHydrogen);
  scin_mat->AddElement(pNistManager->FindOrBuildElement("C"), fracOfCarbon);
  scin_mat->AddElement(pNistManager->FindOrBuildElement("O"), fracOfOxygen);
  scin_mat->AddElement(enrichedLi_el, fracOfEnrichedLi);
   
  return scin_mat;  
}

