#include "AirMatBuilder.hh"
#include "G4Material.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"
#include "G4NistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AirMatBuilder::AirMatBuilder(G4bool enableOpticalProperty):
VMaterialBuilder("Air", enableOpticalProperty) 
{ 
  if(IsOpticalPropertyEnabled())
	{
		pMatPropTable = new MaterialPropertiesTable(pName); 

		G4UImanager* uImanager = G4UImanager::GetUIpointer();
		uImanager->ApplyCommand("/control/execute NuSDMaterials/macros/air.mac");  
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AirMatBuilder::~AirMatBuilder()
{ 
  if(pMatPropTable) delete pMatPropTable;
  
  pMatPropTable=nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* AirMatBuilder::Build()          
{ 
  G4double density         = 1.290*mg/cm3;
  G4int numberOfComponents = 2;
  
  G4Material* mat = new G4Material(pName, density, numberOfComponents);
  mat->AddElement(pNistManager->FindOrBuildElement("N"), 70.*perCent);
  mat->AddElement(pNistManager->FindOrBuildElement("O"), 30.*perCent);
    
  return mat;  
}

