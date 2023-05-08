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

#ifndef NuSDMaterialFactory_h
#define NuSDMaterialFactory_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "VMaterialFactory.hh"

class G4Element;
class VMaterialBuilder;
class G4UImanager;

//singleton class
class NuSDMaterialFactory: public VMaterialFactory
{
  public:
    static NuSDMaterialFactory* GetInstance();
    
    //Singletons should not be cloneable.
    NuSDMaterialFactory(const NuSDMaterialFactory&) = delete;
    
    //Singletons should not be assignable.
    NuSDMaterialFactory& operator=(const NuSDMaterialFactory&) = delete;
  
  private:
  	G4Material* Create3MSpecularReflector();
  	G4Material* CreateAluminizedMylar();
  	G4Material* CreateQuartz(G4bool enableOpticalProperty);
  	     
  protected:  
    NuSDMaterialFactory();
    virtual ~NuSDMaterialFactory();
      
    virtual G4Material* CreateMaterial(const G4String& name, G4bool enableOpticalProperty) override;
      
  private:
    static NuSDMaterialFactory* fInstance;  //singleton class
 
    VMaterialBuilder* fEJ200MatBuilder; 
    VMaterialBuilder* fEJ260MatBuilder;
    VMaterialBuilder* fEJ426MatBuilder;
    VMaterialBuilder* fEJ335MatBuilder;
    VMaterialBuilder* fEJ254MatBuilder;
    VMaterialBuilder* fEJ309MatBuilder;
    
    VMaterialBuilder* fAirMatBuilder;
    VMaterialBuilder* fCementMatBuilder;
    VMaterialBuilder* fGreaseMatBuilder;
    
    VMaterialBuilder* fPolytreneMatBuilder;
    VMaterialBuilder* fPmmaMatBuilder;
    VMaterialBuilder* fFluorinatedAcrylicMatBuilder;
    
    VMaterialBuilder* fAluminizedMylarMatBuilder;
    VMaterialBuilder* fFepMatBuilder;
    VMaterialBuilder* fGdCoatingMatBuilder;
                
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
