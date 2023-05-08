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

#include "NuSDPhotoSensorSurface.hh"
#include "NuSDPhotoSensorProperties.hh"
#include "NuSDFileManager.hh"

#include "G4MaterialPropertiesTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*

G4OpticalSurface (const G4String &name, G4OpticalSurfaceModel model=glisur, G4OpticalSurfaceFinish finish=polished, G4SurfaceType type=dielectric_dielectric, G4double value=1.0)


*/


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhotoSensorSurface::NuSDPhotoSensorSurface(const G4String &name, 
                                               G4OpticalSurfaceModel model, 
                                               G4OpticalSurfaceFinish finish, 
                                               G4SurfaceType type,  
                                               NuSDPhotoSensorProperties* psProp 
                                               )
                                       
: G4OpticalSurface(name, 
  model, 
  finish,
  type, 
  1.0
  ),
fPSProperties{psProp}  
                                                                          
{  
  G4cout<<"PhotoSensorSurface is created"<<G4endl;
  G4cout<<"Quantum eff: "<<fPSProperties->GetPSQE()<<G4endl;
  
  SetPSSurface(name);
				
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDPhotoSensorSurface::~NuSDPhotoSensorSurface()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void NuSDPhotoSensorSurface::SetPSSurface(G4String name)
{
  G4String path;
  if(name=="PhotoCathode")
  {
    path = "data/photoSensors/PhotoCathodeQE_spect.txt";
  }else if(name=="Mppc")
  {
    path = "data/photoSensors/MppcQE_spect.txt";
  }
  
	//---if a photon is absorbed but not detected it is counted for absorption process. if a photon absorbed and detected it is counted  
	//for detection process--- 

	std::vector<double> photonEnergy;
	std::vector<double> quantumEff;
	std::vector<double> reflectivity;

	if(!fPSProperties->IsPSQEDependsEnergy())
	{
		NuSDFileManager::GetInstance()->ReadSpectrumFromFile(path, photonEnergy, 1*eV); 
		quantumEff.insert(quantumEff.end(), photonEnergy.size(), fPSProperties->GetPSQE() );
	}else //Read QE spectrum from file and get the efficiency value for each photon energy
	{ 
		NuSDFileManager::GetInstance()->ReadSpectrumFromFile(path, photonEnergy, quantumEff, 1*eV); //
	}

  reflectivity.insert(reflectivity.end(), photonEnergy.size(), 0.); // No reflection on ps surface.

	G4MaterialPropertiesTable *ps_mat_prop = new G4MaterialPropertiesTable();
	ps_mat_prop->AddProperty("REFLECTIVITY", photonEnergy, reflectivity);
	ps_mat_prop->AddProperty("EFFICIENCY", photonEnergy, quantumEff);
	G4cout << "---------PS surface material property--------- " << G4endl;
	ps_mat_prop->DumpTable();
	SetMaterialPropertiesTable(ps_mat_prop);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
