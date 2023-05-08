#include "ScintillatorFactory.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"

#include "EJ200MatBuilder.hh"
#include "EJ260MatBuilder.hh"
#include "EJ426MatBuilder.hh"
#include "EJ335MatBuilder.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorFactory* ScintillatorFactory::fInstance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorFactory::ScintillatorFactory():
VMaterialFactory(),
fEJ200MatBuilder{nullptr},
fEJ260MatBuilder{nullptr},
fEJ426MatBuilder{nullptr},
fEJ335MatBuilder{nullptr}
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorFactory::~ScintillatorFactory()
{	
  if(fEJ200MatBuilder)   delete fEJ200MatBuilder;
  if(fEJ260MatBuilder)   delete fEJ260MatBuilder;
  if(fEJ426MatBuilder)   delete fEJ426MatBuilder;
  if(fEJ335MatBuilder)   delete fEJ335MatBuilder;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ScintillatorFactory::CreateMaterial(const G4String& name, G4bool enableOpticalProperty)
{
  G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(name);
  if(mat) return mat;

  if(name=="EJ-200" && !fEJ200MatBuilder)
  {
    fEJ200MatBuilder = new EJ200MatBuilder(enableOpticalProperty);
    return fEJ200MatBuilder->GetProduct();
  }else if(name=="EJ-260" && !fEJ260MatBuilder)
  {
    fEJ260MatBuilder = new EJ260MatBuilder(name);
    return fEJ260MatBuilder->GetProduct();
  }else if(name=="EJ-426" && !fEJ426MatBuilder)
  {
    fEJ426MatBuilder = new EJ426MatBuilder(enableOpticalProperty);
    return fEJ426MatBuilder->GetProduct();
  }else if(name=="EJ-335" && !fEJ335MatBuilder)
  {
    fEJ335MatBuilder = new EJ335MatBuilder(enableOpticalProperty);
    return fEJ335MatBuilder->GetProduct();
  }else
  {
    return nullptr;
  }
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorFactory* ScintillatorFactory::GetInstance()
{
  static ScintillatorFactory instance;
  
  if(fInstance == nullptr)
  {
    fInstance = &instance;
  }
    
  return fInstance;
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

