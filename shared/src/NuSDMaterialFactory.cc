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


#include "NuSDMaterialFactory.hh"
#include "ScintillatorFactory.hh"
#include "G4NistManager.hh"

#include "EJ200MatBuilder.hh"
#include "EJ260MatBuilder.hh"
#include "EJ426MatBuilder.hh"
#include "EJ335MatBuilder.hh"
#include "CustomEJ254MatBuilder.hh"
#include "CustomEJ309MatBuilder.hh"
#include "AirMatBuilder.hh"
#include "PmmaMatBuilder.hh"
#include "PolystyreneMatBuilder.hh"
#include "CementMatBuilder.hh"
#include "GreaseMatBuilder.hh"
#include "GdCoatingMatBuilder.hh"
#include "FepMatBuilder.hh"
#include "AluminizedMylarMatBuilder.hh"
#include "FluorinatedAcrylicMatBuilder.hh"
//#include "GdDopedWaterMatBuilder.hh"
//#include "WbLSMatBuilder.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDMaterialFactory* NuSDMaterialFactory::fInstance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDMaterialFactory::NuSDMaterialFactory():
VMaterialFactory(),
fEJ200MatBuilder{nullptr},
fEJ260MatBuilder{nullptr},
fEJ426MatBuilder{nullptr},
fEJ335MatBuilder{nullptr},
fEJ254MatBuilder{nullptr},
fEJ309MatBuilder{nullptr},
fAirMatBuilder{nullptr},
fCementMatBuilder{nullptr},
fGreaseMatBuilder{nullptr},
fPolytreneMatBuilder{nullptr},
fPmmaMatBuilder{nullptr},
fFluorinatedAcrylicMatBuilder{nullptr},
fAluminizedMylarMatBuilder{nullptr},
fFepMatBuilder{nullptr},
fGdCoatingMatBuilder{nullptr}
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDMaterialFactory::~NuSDMaterialFactory()
{	
  //Since we delete base pointer, the derived classes' destructors 
  //will be called automatically.
  
  if(fEJ200MatBuilder) delete fEJ200MatBuilder;
  if(fAirMatBuilder)   delete fAirMatBuilder;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* NuSDMaterialFactory::CreateMaterial(const G4String& name, G4bool enableOpticalProperty)
{

  
  G4Material* mat = G4NistManager::Instance()->FindOrBuildMaterial(name);
  if(mat) return mat;

  










  /*
  if(name=="EJ-200")
  {
    if(!fEJ200MatBuilder){
      fEJ200MatBuilder = new EJ200MatBuilder(enableOpticalProperty);
      return fEJ200MatBuilder->GetProduct();
    }
    else{
      return fEJ200MatBuilder->GetProduct();
    }
      
  }else if(name=="Air")
  {
    if(!fAirMatBuilder){
      fAirMatBuilder = new AirMatBuilder(enableOpticalProperty);
      return fAirMatBuilder->GetProduct();
    }
    else{
      return fAirMatBuilder->GetProduct();
    }
      
  }if(name=="AluminizedMylar")
  {
    if(!fAluminizedMylarMatBuilder){
      fAluminizedMylarMatBuilder = new AluminizedMylarMatBuilder(enableOpticalProperty);
      return fAluminizedMylarMatBuilder->GetProduct();
    }
    else{
      return fAluminizedMylarMatBuilder->GetProduct();
    }
      
  }else if(name=="GdCoating")
  {
    if(!fGdCoatingMatBuilder){
      fGdCoatingMatBuilder = new GdCoatingMatBuilder();
      return fGdCoatingMatBuilder->GetProduct();
    }
    else{
      return fGdCoatingMatBuilder->GetProduct();
    }
      
  }
  else
  {
    return nullptr;
  }
  
  
  */
  
  /*
  else if(name=="EJ-260")
  {
    if(!fEJ260MatBuilder){
      fEJ260MatBuilder = new EJ260MatBuilder(enableOpticalProperty);
      return fEJ260MatBuilder->GetProduct();
    }
    else{
      return fEJ260MatBuilder->GetProduct();
    }
      
  }if(name=="EJ-426")
  {
    if(!fEJ426MatBuilder){
      fEJ426MatBuilder = new EJ426MatBuilder(enableOpticalProperty);
      return fEJ426MatBuilder->GetProduct();
    }
    else{
      return fEJ426MatBuilder->GetProduct();
    }
      
  }if(name=="EJ-335")
  {
    if(!fEJ335MatBuilder){
      fEJ335MatBuilder = new EJ335MatBuilder(enableOpticalProperty);
      return fEJ335MatBuilder->GetProduct();
    }
    else{
      return fEJ335MatBuilder->GetProduct();
    }
      
  }if(name=="CustomEJ-254")
  {
    if(!fEJ254MatBuilder){
      fEJ254MatBuilder = new CustomEJ254MatBuilder(enableOpticalProperty);
      return fEJ254MatBuilder->GetProduct();
    }
    else{
      return fEJ254MatBuilder->GetProduct();
    }
      
  }if(name=="CustomEJ-309")
  {
    if(!fEJ309MatBuilder){
      fEJ309MatBuilder = new CustomEJ309MatBuilder(enableOpticalProperty);
      return fEJ309MatBuilder->GetProduct();
    }
    else{
      return fEJ309MatBuilder->GetProduct();
    }
      
  }
  
  */
  
  
  
  

 

  if(name=="EJ-200" && !fEJ200MatBuilder)
  {
  // fEJ200MatBuilder = new EJ200MatBuilder(enableOpticalProperty);
   //return fEJ200MatBuilder->GetProduct();
   return ScintillatorFactory::GetInstance()->GetMaterial("EJ-200",enableOpticalProperty);
    
  }else if(name=="EJ-260" && !fEJ260MatBuilder)
  {
    fEJ260MatBuilder = new EJ260MatBuilder(name);
    return fEJ260MatBuilder->GetProduct();
  }else if(name=="EJ-426" && !fEJ426MatBuilder)
  {
    fEJ426MatBuilder = new EJ426MatBuilder(enableOpticalProperty);
    return fEJ426MatBuilder->GetProduct();
  }else if(name=="EJ-335" && !fEJ335MatBuilder)
  {
    fEJ335MatBuilder = new EJ335MatBuilder(enableOpticalProperty);
    return fEJ335MatBuilder->GetProduct();
  }else if(name=="CustomEJ-254" && !fEJ254MatBuilder)
  {
    fEJ254MatBuilder = new CustomEJ254MatBuilder(enableOpticalProperty);
    return fEJ254MatBuilder->GetProduct();
  }else if(name=="CustomEJ-309" && !fEJ309MatBuilder)
  {
    fEJ309MatBuilder = new CustomEJ309MatBuilder(enableOpticalProperty);
    return fEJ309MatBuilder->GetProduct();
  }else if(name=="Air" && !fAirMatBuilder)
  {
    fAirMatBuilder = new AirMatBuilder(enableOpticalProperty);
    return fAirMatBuilder->GetProduct(); 
  }else if(name=="Polystyrene" && !fPolytreneMatBuilder)
  {
    fPolytreneMatBuilder = new PolystyreneMatBuilder(enableOpticalProperty);
    return fPolytreneMatBuilder->GetProduct();  
  }else if(name=="Pmma" && !fPmmaMatBuilder)
  {
    fPmmaMatBuilder = new PmmaMatBuilder(enableOpticalProperty);
    return fPmmaMatBuilder->GetProduct();  
  }else if(name=="GdCoating" && !fGdCoatingMatBuilder)
  {
    fGdCoatingMatBuilder = new GdCoatingMatBuilder(); 
    return fGdCoatingMatBuilder->GetProduct();
  }else if(name=="Cement" && !fCementMatBuilder)
  {
    fCementMatBuilder = new CementMatBuilder(name);
    return fCementMatBuilder->GetProduct();
  }else if(name =="Grease" && !fGreaseMatBuilder)
  {
    fGreaseMatBuilder = new GreaseMatBuilder(name);
    return fGreaseMatBuilder->GetProduct();
  }else if(name =="Fep" && !fFepMatBuilder)
  {
    fFepMatBuilder = new FepMatBuilder(enableOpticalProperty);
    return fFepMatBuilder->GetProduct();
  }else if(name =="AluminizedMylar" && !fAluminizedMylarMatBuilder)
  {
  	return CreateAluminizedMylar();
    //fAluminizedMylarMatBuilder = new AluminizedMylarMatBuilder();
    //return fAluminizedMylarMatBuilder->GetProduct();
  }else if(name =="3MSpecularReflector")
  {
  	return Create3MSpecularReflector();

  }else if(name =="Quartz")
  {
  	return CreateQuartz(enableOpticalProperty);

  }else if(name =="FluorinatedAcrylic" && !fFluorinatedAcrylicMatBuilder)
  {
    fFluorinatedAcrylicMatBuilder = new FluorinatedAcrylicMatBuilder(enableOpticalProperty);
    return fFluorinatedAcrylicMatBuilder->GetProduct();
  }
  else
  {
    return nullptr;
  }
  
  
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* NuSDMaterialFactory::Create3MSpecularReflector()
{
	G4double density = 1.06*g/cm3;
	G4int nElements  = 2;
	
	G4Material *mat = new G4Material("3MSpecularReflector", density, nElements);
  mat->AddElement(G4NistManager::Instance()->FindOrBuildElement("C"), 8);
  mat->AddElement(G4NistManager::Instance()->FindOrBuildElement("H"), 8);
	return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* NuSDMaterialFactory::CreateAluminizedMylar()
{
	G4double density   = 1.40*g/cm3;
	G4double nElements = 4;
	
	G4Material *mat = new G4Material("AluminizedMylar", density, nElements=4);
  mat->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_H"),  0.03);
  mat->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_C"),  0.63);
  mat->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_O"),  0.32);
  mat->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_Al"), 0.02);
	
	return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* NuSDMaterialFactory::CreateQuartz(G4bool enableOpticalProperty)
{
	/*
    PMT window quartz
    density = 2.200*g/cm3; // fused quartz 
    density = 2.64*g/cm3;  // crystalline quartz (c.f. PDG) 

  */
  
  G4double density   = 2.64*g/cm3;
  G4double nElements = 2;

  G4Material *quartz_mat = new G4Material("Quartz", density, nElements);
  quartz_mat->AddElement(G4NistManager::Instance()->FindOrBuildElement("Si"), 1);
  quartz_mat->AddElement(G4NistManager::Instance()->FindOrBuildElement("O"), 2);
  
  if( enableOpticalProperty )
  {
  
    std::vector<double> quartzPe = {2.067*eV, 3.263*eV};
    std::vector<double> quartzRefVal(quartzPe.size(), 1.54); //at 400nm
    std::vector<double> quartzAbsVal(quartzPe.size(), 300*cm); //not-known.find from literature

    G4MaterialPropertiesTable* optical_quartz_mat_prop = new G4MaterialPropertiesTable();
    optical_quartz_mat_prop->AddProperty("RINDEX", quartzPe, quartzRefVal);
    optical_quartz_mat_prop->AddProperty("ABSLENGTH", quartzPe, quartzAbsVal);
    G4cout << "-------OptQuartz G4MaterialPropertiesTable--------" << G4endl;
    optical_quartz_mat_prop->DumpTable();

    quartz_mat->SetMaterialPropertiesTable( optical_quartz_mat_prop );
  
  }
  
  return quartz_mat;
	

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDMaterialFactory* NuSDMaterialFactory::GetInstance()
{
  static NuSDMaterialFactory instance;
  
  if(fInstance == nullptr)
  {
    fInstance = &instance;
  }
    
  return fInstance;
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


