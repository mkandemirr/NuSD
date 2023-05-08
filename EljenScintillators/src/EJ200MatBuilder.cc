#include "EJ200MatBuilder.hh"
#include "G4Material.hh"
#include "MaterialPropertiesTable.hh"
#include "G4UImanager.hh"
#include "G4NistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EJ200MatBuilder::EJ200MatBuilder(G4bool enableOpticalProperty):
VMaterialBuilder("EJ-200", enableOpticalProperty) 
{
  if(IsOpticalPropertyEnabled())
  {
    pMatPropTable = new MaterialPropertiesTable(pName); 
    
    G4UImanager* uImanager = G4UImanager::GetUIpointer();
    uImanager->ApplyCommand("/control/execute EljenScintillators/macros/ej-200.mac");  
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EJ200MatBuilder::~EJ200MatBuilder()
{
  if(pMatPropTable) delete pMatPropTable; 
  
  pMatPropTable=nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* EJ200MatBuilder::Build()          
{ 
  //Ref: https://eljentechnology.com/images/products/data_sheets/EJ-200_EJ-204_EJ-208_EJ-212.pdf
  
  G4double hydrogenAtomDensity  = 5.17e+22*(1./cm3);
  G4double carbonAtomDensity    = 4.69e+22*(1./cm3);
 
  G4double hydrogenMolarMass    = pNistManager->FindOrBuildElement("H")->GetAtomicMassAmu()*(g/mole);
  G4double carbonMolarMass      = pNistManager->FindOrBuildElement("C")->GetAtomicMassAmu()*(g/mole);
  
	G4double hydrogenMassDensity  = (hydrogenAtomDensity/CLHEP::Avogadro)*hydrogenMolarMass;
	G4double carbonMassDensity    = (carbonAtomDensity/CLHEP::Avogadro)*carbonMolarMass;
	
	G4double density              = hydrogenMassDensity+carbonMassDensity;

	G4double hydrogenMassFraction = hydrogenMassDensity/density;
	G4double carbonMassFraction 	= carbonMassDensity/density;
  
	G4Material *scin_mat = new G4Material(pName, density, 2);
  scin_mat->AddElement(pNistManager->FindOrBuildElement("H"), hydrogenMassFraction);
  scin_mat->AddElement(pNistManager->FindOrBuildElement("C"), carbonMassFraction);
   
  return scin_mat;  
}

