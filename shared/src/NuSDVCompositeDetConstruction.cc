//***********************************************************************
//* Neutrino Segmented Detector (NuSD) Copyright 2022 Mustafa Kandemir  *
//*                                                                     *
//* This file is part of NuSD.                                          *
//*                                                                     *
//* NuSD is free software: you can redistribute it and/or modify        * 
//* it under the terms of the GNU General Public License as published   *
//* by the Free Software Foundation, either version 3 of the License,   *
//* or any later version.                                               *
//*                                                                     *
//* NuSD is distributed in the hope that it will be useful, but         *
//* WITHOUT ANY WARRANTY; without even the implied warranty of          *
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        * 
//* GNU General Public License for more details.                        *
//*                                                                     *
//* You should have received a copy of the GNU General Public License   *
//* along with NuSD. If not, see <https://www.gnu.org/licenses/>.       *
//*                                                                     *
//***********************************************************************

#include "NuSD_config.h"

#if DETECTOR_TYPE == INHOMOGENEOUSCOMPOSITE

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDVCompositeDetConstruction.hh"
#include "NuSDVCompositeDetMessenger.hh"


#include "NuSDNeutronSD.hh"

#include "G4VisAttributes.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4ApplicationState.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVCompositeDetConstruction::NuSDVCompositeDetConstruction(const G4String& detName, NuSDDetType detType): 
NuSDVDetConstruction(detName,detType),
fDetMessenger{nullptr},
fPhysNdVector(), 
fNeutronScntLogic{nullptr}, 
fNeutronScntSizeX{0.}, fNeutronScntSizeY{0.}, fNeutronScntSizeZ{0.}, fNeutronScntThick{0.}

{
  fDetMessenger = new NuSDVCompositeDetMessenger(this);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVCompositeDetConstruction::~NuSDVCompositeDetConstruction()
{
  delete fDetMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVCompositeDetConstruction::CleanUpOldGeometry()
{ 
  NuSDVDetConstruction::CleanUpOldGeometry();
  fPhysNdVector.clear();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVCompositeDetConstruction::ConstructSDs() 
{   
  NuSDVDetConstruction::ConstructSDs();
  
  if(!fNeutronSD.Get())
  {

    NuSDNeutronSD *neutronSD = new NuSDNeutronSD("NeutronSD","NeutronSD_hit_col");
    fNeutronSD.Put(neutronSD);

    G4SDManager::GetSDMpointer()->AddNewDetector(fNeutronSD.Get());

  }

  SetSensitiveDetector("NeutronScntLogic", fNeutronSD.Get(), false);
     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVCompositeDetConstruction::DefineSolidsAndLogics()
{
  NuSDVDetConstruction::DefineSolidsAndLogics();

  //-------------------------Solid-Logic--------------------------------
  G4Box* nScntSolid       = new G4Box("NeutronScntSolid", 0.5*fNeutronScntSizeX , 0.5*fNeutronScntSizeY, 0.5*fNeutronScntSizeZ);
  fNeutronScntLogic       = new G4LogicalVolume(nScntSolid, GetMaterial("EJ-426",true),"NeutronScntLogic",0,0,0);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVCompositeDetConstruction::SetVisProp()
{
  NuSDVDetConstruction::SetVisProp();
  
  G4VisAttributes* visAttributes{nullptr};
  
  //Neutron detector
  visAttributes = new G4VisAttributes( G4Colour::Gray());
  visAttributes->SetForceSolid(false);
  fNeutronScntLogic->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//invoked by EndOfRunAction method of NuSDRunAction class
void NuSDVCompositeDetConstruction::Print() const 
{ 
  NuSDVDetConstruction::Print();
  
  G4cout<<"Neutron detector thickness (mm): "<<fNeutronScntThick*(1/mm)<<G4endl;
  if(fDetName=="Chandler")
    G4cout<<"Number of Nd : "<<GetNumberOfNeutronScnt()<<G4endl;
  else
    G4cout<<"Number of Neutron screen : "<<GetNumberOfUnit()<<G4endl; 
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//--------------------USERINTERFACE COMMAND------------------------
void NuSDVCompositeDetConstruction::SetNeutronScntThickness(G4double thickness)
{
  fNeutronScntThick = thickness;
  
  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
    G4RunManager::GetRunManager()->ReinitializeGeometry();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

