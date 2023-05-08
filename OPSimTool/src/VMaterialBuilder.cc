#include "VMaterialBuilder.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "MaterialPropertiesTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

VMaterialBuilder::VMaterialBuilder(const G4String& name, G4bool enableOpticalProperty) : 
pName{name},
pNistManager{G4NistManager::Instance()},
bEnableOpticalProperty{enableOpticalProperty},
pMatPropTable{nullptr}
{
  G4cout<<"Material construction <<"<<this->GetName()<<">> has been created."<<G4endl; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

VMaterialBuilder::~VMaterialBuilder()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Templaate method.
G4Material* VMaterialBuilder::GetProduct()
{
  G4Material* mat = Build();
  
  if(IsOpticalPropertyEnabled())
  {
    pMatPropTable->AddPropertiesToMaterial(mat);
  }
    
  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

