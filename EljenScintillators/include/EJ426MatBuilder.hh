#ifndef EJ426MatBuilder_h
#define EJ426MatBuilder_h 1

#include "VMaterialBuilder.hh"

class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EJ426MatBuilder: public VMaterialBuilder{

  public:
    EJ426MatBuilder(G4bool enableOpticalProperty=false); 
    virtual ~EJ426MatBuilder();
	
	protected:
    virtual G4Material* Build() override;
       
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

