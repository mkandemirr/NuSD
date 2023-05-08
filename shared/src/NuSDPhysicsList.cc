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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "NuSDPhysicsList.hh"
#include "NuSDPhysicsListMessenger.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"

#include "NuSDSimParameters.hh"
#include "G4RunManager.hh"
#include "G4StateManager.hh"
#include "G4OpticalPhysics.hh"

#include "G4OpticalPhoton.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
NuSDPhysicsList::NuSDPhysicsList() : G4VModularPhysicsList(),
fMessenger{nullptr},
fOptPhysicsList{nullptr},
fIsOpticalPhysicsOn{true}
{

  fMessenger = new NuSDPhysicsListMessenger(this);

  SetVerboseLevel(1);
  AddPackage("QGSP_BERT_HP");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhysicsList::~NuSDPhysicsList()
{
  delete fMessenger;
  delete fOptPhysicsList;
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPhysicsList::ConstructParticle()
{
  //Pre-Init state 
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  

  //cant be controlled by user interface command since ConsructParticle() 
  //called at pre-init state
  G4OpticalPhoton::OpticalPhotonDefinition();
  
  //the commands below dont work since the processes havent been instantiated yet!  
  //G4VModularPhysicsList::ConstructParticle();
  //if(fOptPhysicsList)
  //fOptPhysicsList->ConstructParticle();
  //if(!fOptPhysicsList) throw std::runtime_error("Something Bad happened here");
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPhysicsList::ConstructProcess()
{
  //calld at Init state
 
  G4VModularPhysicsList::ConstructProcess();
  
  if(fIsOpticalPhysicsOn)
  {
    fOptPhysicsList = new G4OpticalPhysics();
    fOptPhysicsList->ConstructProcess();
    G4cout<<"Optical process are added"<<G4endl;
  }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPhysicsList::AddPackage(const G4String& name)
{
  //called at Pre-init state
  
  //G4StateManager* stateManager    = G4StateManager::GetStateManager();
  //G4ApplicationState currentState = stateManager->GetCurrentState();
  //G4cout<<"surgustate: "<<currentState<<G4endl;  
  
  G4PhysListFactory factory;
  G4VModularPhysicsList* phys =factory.GetReferencePhysList(name);
  G4int i=0;
  const G4VPhysicsConstructor* elem= phys->GetPhysics(i);
  G4VPhysicsConstructor* tmp = const_cast<G4VPhysicsConstructor*> (elem);
  while (elem !=0)
	{
	  G4cout << "RegisterPhysics: " << tmp->GetPhysicsName() << G4endl;
	  RegisterPhysics(tmp);	  
	  elem= phys->GetPhysics(++i) ;
	  tmp = const_cast<G4VPhysicsConstructor*> (elem);
	}
		
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDPhysicsList::SetOpticalPhysicsOn(G4bool bl)
{
  //called at Pre-init state
  
  fIsOpticalPhysicsOn = bl;
  NuSDSimParameters::GetInstance()->SetOpticalPhysicsFlag(fIsOpticalPhysicsOn);
  
  //G4cout<<"neden: "<<fIsOpticalPhysicsOn<<G4endl;

  if(G4StateManager::GetStateManager()->GetCurrentState() != G4State_PreInit )
  {
    G4RunManager::GetRunManager()->ReinitializeGeometry();
  }
  
}

