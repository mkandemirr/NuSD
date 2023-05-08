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

#ifndef NuSDHexPmtProperties_H
#define NuSDHexPmtProperties_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDPhotoSensorProperties.hh"
#include "globals.hh"

class NuSDHexPmtPropertiesMessenger;

class NuSDHexPmtProperties: public NuSDPhotoSensorProperties 
{

  public:
    NuSDHexPmtProperties(G4bool isAttachedBothEnd, 
                          G4bool isPSBinderOn,
                          G4double psBinderLength,
                          G4double psQE,
                          G4bool isQEDependsEnergy,
                          G4double psTanDisToOuterSur,
                          G4bool isLgOn,
                          G4double lgLength,
                          G4bool isLgBinderOn,
                          G4double lgBinderLength
                         );
    
    ~NuSDHexPmtProperties();
  
  public:
    G4double GetHexPmtReadoutUnitLength(); 
  
  public:
    //UI commands   
   //PS
    void SetPSTanDisToOuterSur(G4double td);
    //Lg
    void SetLightGuide(G4bool bl );
    void SetLgLength(G4double length);
    //Lg binder --Optical cement------
    void SetLgBinder(G4bool bl );
    void SetLgBinderLength(G4double length);
             
  //Gettters      
  public:
    G4double GetPSTanDisToOuterSur()  const { return fPSTanDisToOuterSur; }
    G4bool   IsLgOn ()                const { return fIsLgOn; }
    G4double GetLgLength()            const { return fLgLength; }   
    //Lg binder --Optical cement------
    G4bool   IsLgBinderOn()           const { return fIsLgBinderOn; }
    G4double GetLgBinderLength()      const { return fLgBinderLength; }
           
  
  private:
    NuSDHexPmtPropertiesMessenger* fHexPmtPropertiesMessenger;      
           
  private:              
    G4double fPSTanDisToOuterSur;
    G4bool   fIsLgOn;
    G4double fLgLength;
    G4bool   fIsLgBinderOn;
    G4double fLgBinderLength;
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
