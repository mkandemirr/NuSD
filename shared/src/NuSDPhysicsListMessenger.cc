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

#include "NuSDPhysicsListMessenger.hh"

#include "NuSDPhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhysicsListMessenger::NuSDPhysicsListMessenger(NuSDPhysicsList* physList)
:fPhysicsList(physList)
{
  fPhysDir = new G4UIdirectory("/NuSD/physics/");
  fPhysDir->SetGuidance("Commands to activate physics models and set cuts");
 
  fPackageListCmd = new G4UIcmdWithAString("/NuSD/physics/addPackage",this);
  fPackageListCmd->SetGuidance("Add physics package.");
  fPackageListCmd->SetParameterName("package",false);
  fPackageListCmd->AvailableForStates(G4State_PreInit);
  fPackageListCmd->SetCandidates("QGSP_BIC_HP QGSP_BERT_HP FTFP_BERT_HP");
  
  fOpticalPhysicsCmd = new G4UIcmdWithABool("/NuSD/physics/isOpticalPhysicsOn",this);
  fOpticalPhysicsCmd->SetGuidance("Add optical physics process");
  fOpticalPhysicsCmd->SetParameterName("OpticalPhysicsFlag",false);
  fOpticalPhysicsCmd->AvailableForStates(G4State_PreInit);
  fOpticalPhysicsCmd->SetDefaultValue(false); 
  fOpticalPhysicsCmd->SetToBeBroadcasted(false);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhysicsListMessenger::~NuSDPhysicsListMessenger()
{
  delete fPhysDir;    
  delete fPackageListCmd;
  delete fOpticalPhysicsCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == fPackageListCmd )
  { 
    fPhysicsList->AddPackage(newValue);
  }
  else if ( command == fOpticalPhysicsCmd )
  {
    fPhysicsList->SetOpticalPhysicsOn(fOpticalPhysicsCmd->GetNewBoolValue(newValue));
       
  } 
      
}
