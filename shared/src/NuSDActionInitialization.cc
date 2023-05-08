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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDActionInitialization.hh"
#include "NuSDRunAction.hh"
#include "NuSDSteppingAction.hh"
#include "NuSDStackingAction.hh"
#include "NuSDEventAction.hh"

#ifdef GENERIC_PRIMARY_GENERATOR
#include "NuSDGenericPrimaryGeneratorAction.hh"
#else 
#include "NuSDPrimaryGeneratorAction.hh"
#endif 
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDActionInitialization::NuSDActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDActionInitialization::~NuSDActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDActionInitialization::BuildForMaster() const
{
 	SetUserAction( new NuSDRunAction() );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDActionInitialization::Build() const
{

#ifdef GENERIC_PRIMARY_GENERATOR
  SetUserAction( new NuSDGenericPrimaryGeneratorAction() ); //for background particles
#else 
  SetUserAction( new NuSDPrimaryGeneratorAction() ); //for IBD event
#endif  
		
	SetUserAction( new NuSDRunAction() );
  SetUserAction( new NuSDEventAction() );
	SetUserAction( new NuSDSteppingAction() );
  SetUserAction( new NuSDStackingAction() );

}


