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

#ifndef NuSDPmtProperties_H
#define NuSDPmtProperties_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NuSDPhotoSensorProperties.hh"
#include "globals.hh"

class NuSDPmtPropertiesMessenger;

class NuSDPmtProperties: public NuSDPhotoSensorProperties 
{

  public:
    NuSDPmtProperties(G4bool isAttachedBothEnd, 
                      G4bool isPSBinderOn,
                      G4double psBinderLength,
                      G4double psQE,
                      G4bool isQEDependsEnergy,
                      G4double psRadius,
                      G4bool isLgOn,
                      G4double lgPmtEndSizeX,
                      G4double lgPmtEndSizeY,
                      G4double lgPmtEndSizeZ,
                      G4double lgLength,
                      G4bool isLgBinderOn,
                      G4double lgBinderLength
                     );
    
    ~NuSDPmtProperties();
  
  public:
    G4double GetPmtReadoutUnitLength(); 
  
  public:
    //UI commands   
   //PS
    void SetPSRadius(G4double pcr);
    //Lg
    void SetLightGuide(G4bool bl );
    void SetLgPmtEndSizeX(G4double x);
    void SetLgPmtEndSizeY(G4double y);
    void SetLgPmtEndSizeZ(G4double z);
    void SetLgLength(G4double length);
    //Lg binder --Optical cement------
    void SetLgBinder(G4bool bl );
    void SetLgBinderLength(G4double length);
        
  //Gettters      
  public:
    G4double GetPSRadius()        const { return fPSRadius; }
    G4bool   IsLgOn ()            const { return fIsLgOn; }
    G4double GetLgPmtEndSizeX()   const { return fLgPmtEndSizeX; }
    G4double GetLgPmtEndSizeY()   const { return fLgPmtEndSizeY; }
    G4double GetLgPmtEndSizeZ()   const { return fLgPmtEndSizeZ; }
    G4double GetLgLength()        const;   
    //Lg binder --Optical cement------
    G4bool   IsLgBinderOn()       const { return fIsLgBinderOn; }
    G4double GetLgBinderLength()  const;
           
  
   private:
    NuSDPmtPropertiesMessenger* fPmtPropertiesMessenger;      
         
  private:              
    G4double  fPSRadius;
    G4bool    fIsLgOn;
    G4double  fLgPmtEndSizeX;
    G4double  fLgPmtEndSizeY;
    G4double  fLgPmtEndSizeZ;
    G4double  fLgLength;
    G4bool    fIsLgBinderOn;
    G4double  fLgBinderLength;
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
