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

#if DETECTOR_NAME == SOLID
#include "NuSDGroove.hh"
#include "NuSDFiberProperties.hh"
#endif

#include "NuSDVPrimaryGeneratorAction.hh"
#include "NuSDVPrimaryGeneratorMessenger.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalVolumeStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDVPrimaryGeneratorAction::NuSDVPrimaryGeneratorAction(): 
G4VUserPrimaryGeneratorAction(),
fMessenger{nullptr},
fPosInNuScnt(),
fRndmPosFlag(true)
{
  
  fMessenger = new NuSDVPrimaryGeneratorMessenger(this);
  
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

NuSDVPrimaryGeneratorAction::~NuSDVPrimaryGeneratorAction()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector NuSDVPrimaryGeneratorAction::GetEventInitPos()
{
  G4ThreeVector selectedDetPos    = fDetConstruction->GetPV()->GetTranslation(); //wrt world volume 
	G4ThreeVector unitPos           = SelectARndmUnitAndGetPos();
	G4ThreeVector nuScntPos         = GetNuScntPos();
	
	if(fRndmPosFlag) { fPosInNuScnt = GetRandomPosInNuScnt(); } //default
	
	G4ThreeVector eventInitPos      = fPosInNuScnt+nuScntPos+unitPos+selectedDetPos;
	
	return eventInitPos; //wrt global frame
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector NuSDVPrimaryGeneratorAction::SelectARndmUnitAndGetPos()
{
  std::vector<G4VPhysicalVolume*> physUnitVector { fDetConstruction->GetUnitPVVector() };
  long unsigned int unitNumber {physUnitVector.size() };
  G4int selectedUnitCopyNo { (G4int)(G4UniformRand()*unitNumber) };
  G4ThreeVector unitPos { physUnitVector[selectedUnitCopyNo]->GetTranslation() }; //unit center pos wrt selected det center

  return unitPos;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector NuSDVPrimaryGeneratorAction::GetNuScntPos()
{
  //returns (0,0,0) for all detector types except Solid. Because neutrino scnt and unit center 
  //are coincides all detector types except Solid
  return G4PhysicalVolumeStore::GetInstance()->GetVolume("NuScntPV")->GetTranslation();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector NuSDVPrimaryGeneratorAction::GetRandomPosInNuScnt()
{

  G4ThreeVector randomPosInNuScnt;
           
#if DETECTOR_NAME == SOLID
  
  //Since there is a groove in nuScnt, we should'nt generate events inside groove!! 
  //nuScnt is inside alu. since alu and unit has the same dimension and their center are coincide, 
  //position wrt alu = position wrt unit
  
  G4double grooveSizeX      = fDetConstruction->GetFiberProperties()->GetGrooveSizeX();
  G4double grooveSizeZ      = fDetConstruction->GetFiberProperties()->GetGrooveSizeZ();
  G4double grooveDistToWall = fDetConstruction->GetFiberProperties()->GetGrooveDistanceToWall();
  
  G4double p_nuScntSizeX = fDetConstruction->GetNuScntSizeX()-2*(grooveSizeX-grooveDistToWall);
  G4double p_nuScntSizeY = fDetConstruction->GetNuScntSizeY();
  G4double p_nuScntSizeZ = fDetConstruction->GetNuScntSizeZ()-2*(grooveSizeZ-grooveDistToWall);

  G4double x = -p_nuScntSizeX/2. + p_nuScntSizeX*G4UniformRand();
  G4double y = -p_nuScntSizeY/2. + p_nuScntSizeY*G4UniformRand();
  G4double z = -p_nuScntSizeZ/2. + p_nuScntSizeZ*G4UniformRand();

  randomPosInNuScnt = G4ThreeVector(x,y,z); 
  return randomPosInNuScnt;
  
#elif DETECTOR_NAME == HSP 
  
  G4double x{}, y{}, z{};

  G4double nuScntTanDisToOuterSur  = fDetConstruction->GetNuScntTanDisToOuterSur();

  G4double nuScntSizeZ             = fDetConstruction->GetNuScntSizeZ();
  z                                = -nuScntSizeZ/2. + nuScntSizeZ*G4UniformRand(); 

  const G4int numberOfCorners      = 6;
  G4double theta                   = 0*deg;

  G4int selectedCorner             = (G4int)(G4UniformRand()*numberOfCorners);
  G4double sideLength              = (2*nuScntTanDisToOuterSur)/sqrt(3); 

  G4double X[numberOfCorners];
  G4double Y[numberOfCorners];

  for(unsigned int i = 0; i<6;i++)
  {

  X[i]=sideLength*cos(theta);
  Y[i]=sideLength*sin(theta);

  theta += 60*deg;

  }

  G4double r1 { G4UniformRand() };
  G4double r2 { G4UniformRand() };

  if(r1+r2>=1)
  {
  r1 = 1-r1;
  r2 = 1-r2;
  }

  if( selectedCorner == 5 )
  {
  x = r1*X[selectedCorner] + r2*X[0];
  y = r1*Y[selectedCorner] + r2*Y[0];
  }else
  {
  x = r1*X[selectedCorner] + r2*X[selectedCorner+1];
  y = r1*Y[selectedCorner] + r2*Y[selectedCorner+1];
  }

  randomPosInNuScnt = G4ThreeVector(x,y,z);
  return randomPosInNuScnt;
  
#else  
           
  G4double nuScntSizeX = fDetConstruction->GetNuScntSizeX();
  G4double nuScntSizeY = fDetConstruction->GetNuScntSizeY();
  G4double nuScntSizeZ = fDetConstruction->GetNuScntSizeZ();

  G4double x = -nuScntSizeX/2. + nuScntSizeX*G4UniformRand();
  G4double y = -nuScntSizeY/2. + nuScntSizeY*G4UniformRand();
  G4double z = -nuScntSizeZ/2. + nuScntSizeZ*G4UniformRand();

  randomPosInNuScnt = G4ThreeVector(x,y,z);
  return randomPosInNuScnt;
  
#endif
  
}
   
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//---------------------UI commands-------------------------
void NuSDVPrimaryGeneratorAction::SetPosInNuScnt(G4ThreeVector pos)
{
  fPosInNuScnt = pos;  
}
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......    
void NuSDVPrimaryGeneratorAction::SetRndmFlag(G4bool flag)
{
  fRndmPosFlag = flag;   
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
