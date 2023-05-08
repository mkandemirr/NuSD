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

#include "NuSDVOptReadoutUnit.hh"

#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "NuSDMaterialFactory.hh"
#include "NuSDPhotoSensorProperties.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int NuSDVOptReadoutUnit::fObjectCount = 0;
G4OpticalSurface* NuSDVOptReadoutUnit::fPSSurface = nullptr;
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
NuSDVOptReadoutUnit::NuSDVOptReadoutUnit(G4RotationMatrix *pRot,
                                         const G4ThreeVector &tlate,
                                         G4LogicalVolume *pCurrentLogical,
                                         const G4String& pName, 
                                         G4LogicalVolume *pMotherLogical,
                                         G4bool pMany,
                                         G4int pCopyNo,
                                         const G4String& placedAxis 
                                         )
: G4PVPlacement(pRot,
  tlate, 
  pCurrentLogical,
  pName,
  pMotherLogical,
  pMany,
  pCopyNo
  ),
 
fPSLogic{nullptr},			                                                
fPhysPSBinder{nullptr},
fPhysPS{nullptr},
fPSBinderPos(),
fPSPos(),
fCopyNo{pCopyNo},
fPlacedAxis{placedAxis},
fRot{pRot},
fCheckOverlaps{true},
fDetConstruction{nullptr}
{  
      
  ++NuSDVOptReadoutUnit::fObjectCount;
  
  const G4VUserDetectorConstruction* vdc = G4RunManager::GetRunManager()->GetUserDetectorConstruction();
  
#if DETECTOR_NAME == CHANDLER
    fDetConstruction  = static_cast<const NuSDChandlerDetConstruction*>(vdc);
#elif DETECTOR_NAME == SWEANY
    fDetConstruction  = static_cast<const NuSDSweanyDetConstruction*>(vdc);
#elif DETECTOR_NAME == SOLID
    fDetConstruction  = static_cast<const NuSDSolidDetConstruction*>(vdc);  
#elif DETECTOR_NAME == NULAT
    fDetConstruction  = static_cast<const NuSDNulatDetConstruction*>(vdc);
#elif DETECTOR_NAME == PANDA
    fDetConstruction  = static_cast<const NuSDPandaDetConstruction*>(vdc);
#elif DETECTOR_NAME == PROSPECT
    fDetConstruction  = static_cast<const NuSDProspectDetConstruction*>(vdc);
#elif DETECTOR_NAME == HSP
    fDetConstruction  = static_cast<const NuSDHspDetConstruction*>(vdc);  
#endif  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVOptReadoutUnit::~NuSDVOptReadoutUnit()
{     
  --NuSDVOptReadoutUnit::fObjectCount;	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* NuSDVOptReadoutUnit::GetMaterial(const G4String& name, G4bool enableOpticalProperty) 
{
  G4Material* material = NuSDMaterialFactory::GetInstance()->GetMaterial(name,enableOpticalProperty);
  return material;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDVOptReadoutUnit::SetVisProp(NuSDPhotoSensorProperties* psProp)
{

	G4VisAttributes * visAttributes{nullptr};
	
	if(psProp->IsPSBinderOn())
	{
	  if(fPhysPSBinder)
	  {
		  visAttributes = new G4VisAttributes(G4Colour::Green());  // magenta ));
		  visAttributes->SetForceSolid(true);
		  fPhysPSBinder->GetLogicalVolume()->SetVisAttributes(visAttributes);
		  fDetConstruction->GetVisVec().push_back(visAttributes);
	  }

  }
	
	if(fPhysPS)
	{
		visAttributes = new G4VisAttributes(G4Colour::Yellow());      
		visAttributes->SetForceSolid(true);
		fPhysPS->GetLogicalVolume()->SetVisAttributes(visAttributes);
		fDetConstruction->GetVisVec().push_back(visAttributes);

	}


}




