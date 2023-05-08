
#ifndef EJ200MatBuilder_h
#define EJ200MatBuilder_h 1

#include "VMaterialBuilder.hh"

class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EJ200MatBuilder: public VMaterialBuilder{

  public:
    EJ200MatBuilder(G4bool enableOpticalProperty=false); 
    virtual ~EJ200MatBuilder();

	protected:
    virtual G4Material* Build() override;
       
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

