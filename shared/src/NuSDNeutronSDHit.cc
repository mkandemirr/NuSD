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

//compile this file only composite detectors!
#if (DETECTOR_NAME == CHANDLER || DETECTOR_NAME == SWEANY || DETECTOR_NAME == SOLID )

#include "NuSDNeutronSDHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<NuSDNeutronSDHit>* NuSDNeutronSDHitAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDNeutronSDHit::NuSDNeutronSDHit()
: G4VHit(),
  fCopyNo{-1},
  fParticlePDG{-1},
  fTrackID{-1},
  fEdep{0.},
  fTime{0.}

{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDNeutronSDHit::~NuSDNeutronSDHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDNeutronSDHit::NuSDNeutronSDHit(const NuSDNeutronSDHit& right)
  : G4VHit()
{
  fCopyNo              = right.fCopyNo;
  fParticlePDG         = right.fParticlePDG;
  fTrackID          = right.fTrackID;
  fEdep                = right.fEdep;
  fTime                = right.fTime; 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const NuSDNeutronSDHit& NuSDNeutronSDHit::operator=(const NuSDNeutronSDHit& right)
{
  fCopyNo              = right.fCopyNo;
  fParticlePDG         = right.fParticlePDG;
  fTrackID          = right.fTrackID;
  fEdep                = right.fEdep;
  fTime                = right.fTime; 
  
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NuSDNeutronSDHit::operator==(const NuSDNeutronSDHit& right) const
{
  return ( this == &right ) ? true : false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



#endif
