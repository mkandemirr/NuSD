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

#include "NuSDReflectorSurface.hh"
#include "NuSDReflectorProperties.hh"
#include "NuSDFileManager.hh"

#include "G4MaterialPropertiesTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*

G4OpticalSurface (const G4String &name, G4OpticalSurfaceModel model=glisur, G4OpticalSurfaceFinish finish=polished, G4SurfaceType type=dielectric_dielectric, G4double value=1.0)

*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDReflectorSurface::NuSDReflectorSurface(const G4String &name, 
                                           G4OpticalSurfaceModel model, 
                                           G4OpticalSurfaceFinish finish, 
                                           G4SurfaceType type,  
                                           NuSDReflectorProperties* refProp 
                                          )
                                       
: G4OpticalSurface(name, 
  model, 
  finish,
  type,
  1.0
  ), 
fReflectorProperties{refProp}
                                                                         
{  
  G4cout<<"ReflectorSurface is created"<<G4endl;
  G4cout<<"Reflectivity: "<<fReflectorProperties->GetReflectivity()<<G4endl;
  
  if(finish == polishedfrontpainted )
    SetReflectorSurface();
  else if(finish == polishedbackpainted)
    SetReflectorAndGapSurface(); //not used for now!
       
  		
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDReflectorSurface::~NuSDReflectorSurface()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDReflectorSurface::SetReflectorSurface() //for PFP surface finish type
{
  /*

  The finish PFP (GFP) defines a volume with a painted surface, e.g. a scintillator coated with a reflecting material, 
  where R is the  probability for reflection by the paint. The GEANT4 code decides, with respect to R, if the optical photon 
  is absorbed by the paint. If not absorbed the optical photon is SS (L) reflected. No refraction occurs and 
  Snell’s law is not applied. Note: the paint is defined by the surface parameters; it is not required to define it as an 
  additional geometrical volume.
  it is perfectly coated scintillator.

  GFP-------Only reflection or absorption; No refraction; Reflection probability set by Reflectivity. Only lambertian reflectin 

  PFP-------Only reflection or absorption; No refraction; Reflection probability set by Reflectivity.
  if reflected only Specular Spike  reflection

 */
  G4String path = "data/reflectors/Reflector_spect.txt";

	std::vector<double> photonEnergy;
	std::vector<double> efficiency;
	std::vector<double> reflectivity;

	if( !fReflectorProperties->IsReflectivityDependsEnergy() )
	{
		NuSDFileManager::GetInstance()->ReadSpectrumFromFile(path, photonEnergy, 1*eV); //fill only one column! 
		reflectivity.insert(reflectivity.end(), photonEnergy.size(), fReflectorProperties->GetReflectivity() );
	}else //Read experimental reflectivity value as a function of photon energy
	{ 
		NuSDFileManager::GetInstance()->ReadSpectrumFromFile(path, photonEnergy, reflectivity, 1*eV); //fill two columnn!!
	}
	
	efficiency.insert(efficiency.end(), photonEnergy.size(), 0.); //No detection for reflector surface

	G4MaterialPropertiesTable *reflectorSurfaceMatProp = new G4MaterialPropertiesTable();
	reflectorSurfaceMatProp->AddProperty("REFLECTIVITY", photonEnergy, reflectivity);
	reflectorSurfaceMatProp->AddProperty("EFFICIENCY", photonEnergy, efficiency);

	G4cout << "-------Reflector Material(FrontPainted) surface material property--------" << G4endl;
	reflectorSurfaceMatProp->DumpTable();

	SetMaterialPropertiesTable(reflectorSurfaceMatProp); // this function inherited from G4OpticalSurface

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDReflectorSurface::SetReflectorAndGapSurface() //for PBP surface finish type
{

  /*

  Diffrence:
  PBP----The Polish refers to wrapping. it implies wrapping is a perfectly smooth mirror with only specular spike reflection 
  taking place.
  GBP----The Ground refers to wrapping. it implies wrapping is a ground mirror with only lambertian reflection taking place.

  Common for GBP and PBP:

  The fSigmaAlpha value specified refers to the scintillator-air gap interface. Snell laws is apllied after sampling 
  the facet normal, and if reflection takes place, one of the four (specularlobe,specularspike,backscatter,lambertian) takes place 
  with respect to facet normal according to assigned probabilities.

 */
  G4String path = "data/reflectors/Reflector_spect.txt";

	std::vector<G4double> photonEnergy;
	std::vector<G4double> efficiency;
	std::vector<G4double> reflectivity;

	if( !fReflectorProperties->IsReflectivityDependsEnergy() )
	{
		NuSDFileManager::GetInstance()->ReadSpectrumFromFile(path, photonEnergy, 1*eV); //read only one column! 
		reflectivity.insert(reflectivity.end(), photonEnergy.size(), fReflectorProperties->GetReflectivity() );
	}else //Read experimental reflectivity value as a function of photon energy
	{ 
		NuSDFileManager::GetInstance()->ReadSpectrumFromFile(path, photonEnergy, reflectivity, 1*eV); //read two columns!
	}
	
	efficiency.insert(efficiency.end(), photonEnergy.size(), 0.); //No detection for reflector surface
		  
  // Reflectivity types refer to scintillator-gap interface--not wrapping!!
  SetSigmaAlpha(0.01); 

  std::vector<G4double> specularSpike;
  std::vector<G4double> specularLobe;
  std::vector<G4double> backScatter;
  //G4double wrap_Lambertian[photonNum]; 
   
  for(std::size_t i=0; i<photonEnergy.size(); ++i)
  {
    specularSpike.at(i)  = 0.;
    backScatter.at(i)    = 0.;
    specularLobe.at(i)   = 1.;
    //lambertian->at(i)   = 0.; //0+0+1+x=1 --> x=0
    
  }
  
  G4String airSpectPath = "data/materials/Air_refSpect.txt";

  std::vector<G4double> gapRefIPe;
  std::vector<G4double> gapRefIVal;
  
  G4double gapRefIndex = 1.0; //air
  G4bool isGapRefIDependsEnergy = false;
  //GapRefIndex_spect.txt file is not ready!!
  if( !isGapRefIDependsEnergy )
	{
		NuSDFileManager::GetInstance()->ReadSpectrumFromFile(airSpectPath, gapRefIPe, 1*eV); //read only one column!
		gapRefIVal.insert(gapRefIVal.end(), gapRefIPe.size(), gapRefIndex );
	}else //Read experimental reflectivity value as a function of photon energy
	{ 

		NuSDFileManager::GetInstance()->ReadSpectrumFromFile(airSpectPath, gapRefIPe, gapRefIVal, 1*eV); //read two columns!

	}

	G4MaterialPropertiesTable *reflectorSurfaceMatProp = new G4MaterialPropertiesTable();
	reflectorSurfaceMatProp->AddProperty("RINDEX", gapRefIPe, gapRefIVal);
	reflectorSurfaceMatProp->AddProperty("REFLECTIVITY", photonEnergy, reflectivity);
	reflectorSurfaceMatProp->AddProperty("EFFICIENCY", photonEnergy, efficiency);

	reflectorSurfaceMatProp->AddProperty("SPECULARLOBECONSTANT", photonEnergy, specularLobe);
  reflectorSurfaceMatProp->AddProperty("SPECULARSPIKECONSTANT", photonEnergy, specularSpike);
  reflectorSurfaceMatProp->AddProperty("BACKSCATTERCONSTANT", photonEnergy, backScatter);
	

	G4cout << "-------Reflector Material(BackPainted) surface material property--------" << G4endl;
	reflectorSurfaceMatProp->DumpTable();

	SetMaterialPropertiesTable(reflectorSurfaceMatProp); // this function inherited from G4OpticalSurface

}


