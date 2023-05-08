#ifndef ScintillatorFactory_h
#define ScintillatorFactory_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "VMaterialFactory.hh"

class VMaterialBuilder;
class G4Material;

//singleton class
class ScintillatorFactory: public VMaterialFactory
{
  public:
    static ScintillatorFactory* GetInstance();
    
    //Singletons should not be cloneable.
    ScintillatorFactory(const ScintillatorFactory&) = delete;
    
    //Singletons should not be assignable.
    ScintillatorFactory& operator=(const ScintillatorFactory&) = delete;
       
  protected:  
    ScintillatorFactory();
    virtual ~ScintillatorFactory();
      
    virtual G4Material* CreateMaterial(const G4String& name, G4bool enableOpticalProperty) override;
      
  private:
    static ScintillatorFactory* fInstance;  
 
    VMaterialBuilder* fEJ200MatBuilder; 
    VMaterialBuilder* fEJ260MatBuilder;
    VMaterialBuilder* fEJ426MatBuilder;
    VMaterialBuilder* fEJ335MatBuilder;                  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
