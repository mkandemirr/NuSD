#ifndef EJ335MatBuilder_h
#define EJ335MatBuilder_h 1

#include "VMaterialBuilder.hh"

class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EJ335MatBuilder: public VMaterialBuilder{

  public:
    EJ335MatBuilder(G4bool enableOpticalProperty=false); 
    virtual ~EJ335MatBuilder();

	protected:
    virtual G4Material* Build() override;
       
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

