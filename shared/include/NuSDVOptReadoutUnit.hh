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

#ifndef NuSDVOptReadoutUnit_H
#define NuSDVOptReadoutUnit_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4PVPlacement.hh"

#include "NuSD_config.h"

#if DETECTOR_NAME == CHANDLER
#include "NuSDChandlerDetConstruction.hh"
#elif DETECTOR_NAME == SWEANY
#include "NuSDSweanyDetConstruction.hh"
#elif DETECTOR_NAME == SOLID
#include "NuSDSolidDetConstruction.hh"  
#elif DETECTOR_NAME == NULAT
#include "NuSDNulatDetConstruction.hh"
#elif DETECTOR_NAME == PANDA
#include "NuSDPandaDetConstruction.hh"
#elif DETECTOR_NAME == PROSPECT
#include "NuSDProspectDetConstruction.hh"
#elif DETECTOR_NAME == HSP
#include "NuSDHspDetConstruction.hh"  
#endif  

class NuSDPhotoSensorProperties;
class G4OpticalSurface;
class G4Material;

class NuSDVOptReadoutUnit: public G4PVPlacement 
{
  public:
    NuSDVOptReadoutUnit(G4RotationMatrix *pRot,
                        const G4ThreeVector &tlate,
                        G4LogicalVolume *pCurrentLogical,
                        const G4String& pName, 
                        G4LogicalVolume *pMotherLogical,
                        G4bool pMany,
                        G4int pCopyNo,
                        const G4String& placedAxis
                        );
                    
    virtual ~NuSDVOptReadoutUnit();   
  
  protected:
    virtual void Construct() = 0;
     
    G4Material* GetMaterial(const G4String& name, G4bool enableOpticalProperty=false);  
    void SetVisProp(NuSDPhotoSensorProperties* psProp);
    
  protected:
    static G4int fObjectCount; //how many object of this class are created      
    static G4OpticalSurface* fPSSurface;  
  protected:
    G4LogicalVolume*   fPSLogic;
    G4VPhysicalVolume* fPhysPSBinder;
    G4VPhysicalVolume* fPhysPS;
    
    G4ThreeVector      fPSBinderPos;
    G4ThreeVector      fPSPos;
    
    G4int              fCopyNo;
    G4String           fPlacedAxis;
    G4RotationMatrix*  fRot;
    G4bool             fCheckOverlaps;
    
  protected:
  
#if DETECTOR_NAME == CHANDLER
    const NuSDChandlerDetConstruction *fDetConstruction;
#elif DETECTOR_NAME == SWEANY
    const NuSDSweanyDetConstruction   *fDetConstruction;
#elif DETECTOR_NAME == SOLID
    const NuSDSolidDetConstruction    *fDetConstruction;
#elif DETECTOR_NAME == NULAT
    const NuSDNulatDetConstruction    *fDetConstruction;
#elif DETECTOR_NAME == PANDA
    const NuSDPandaDetConstruction    *fDetConstruction;
#elif DETECTOR_NAME == PROSPECT
    const NuSDProspectDetConstruction *fDetConstruction;
#elif DETECTOR_NAME == HSP
    const NuSDHspDetConstruction      *fDetConstruction;  
#endif  
          
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
