#include "AluminizedMylarMatBuilder.hh"
#include "G4Material.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"
#include "G4NistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AluminizedMylarMatBuilder::AluminizedMylarMatBuilder():
VMaterialBuilder("AluminizedMylar") 
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AluminizedMylarMatBuilder::~AluminizedMylarMatBuilder()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* AluminizedMylarMatBuilder::Build()          
{ 
  G4double density = 1.40*g/cm3;
  G4int nElements  = 4;
  
  G4Material *mat = new G4Material("AluminizedMylar", density, nElements);
  mat->AddMaterial(pNistManager->FindOrBuildMaterial("G4_H"),  0.03);
  mat->AddMaterial(pNistManager->FindOrBuildMaterial("G4_C"),  0.63);
  mat->AddMaterial(pNistManager->FindOrBuildMaterial("G4_O"),  0.32);
  mat->AddMaterial(pNistManager->FindOrBuildMaterial("G4_Al"), 0.02);
  
  return mat;
}

