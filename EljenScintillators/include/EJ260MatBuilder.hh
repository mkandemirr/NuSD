#ifndef EJ260MatBuilder_h
#define EJ260MatBuilder_h 1

#include "VMaterialBuilder.hh"

class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EJ260MatBuilder: public VMaterialBuilder{

  public:
    EJ260MatBuilder(G4bool enableOpticalProperty=false); 
    virtual ~EJ260MatBuilder();
    
	protected:
    virtual G4Material* Build() override;
       
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

