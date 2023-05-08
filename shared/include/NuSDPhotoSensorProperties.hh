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

#ifndef NuSDPhotoSensorProperties_H
#define NuSDPhotoSensorProperties_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class NuSDPhotoSensorPropertiesMessenger;

class NuSDPhotoSensorProperties
{

  public:
    NuSDPhotoSensorProperties(G4bool isAttachedBothEnd, 
                              G4bool isPSBinderOn,
                              G4double psBinderLength,
                              G4double psQE,
                              G4bool isQEDependsEnergy
                             ); 
    virtual ~NuSDPhotoSensorProperties();
  
    
  public:
  //PhotoSensor UI comand
    void SetBothEnd(G4bool bl); 
    void SetPSBinder(G4bool bl );
    void SetPSBinderLength(G4double length); 
    
    void SetPSQE(G4double eff);
    void SetQEDependsEnergy(G4bool b);
    
  //Getters  
  public: 
    G4bool   IsAttachedBothEnd()     const { return fIsAttachedBothEnd; } 
    G4bool   IsPSBinderOn()          const { return fIsPSBinderOn; }
    G4double GetPSBinderLength()     const;
    G4double GetPSQE()               const { return fQE; }
    G4bool   IsPSQEDependsEnergy()   const { return fIsQEDependsEnergy; } 
    
    //not important just for visualization!
    G4double GetPSLength()           const  { return fPSLength; } 
    
  private:
    NuSDPhotoSensorPropertiesMessenger  *fPSPropertiesMessenger; 
     
  protected:          
    G4bool    fIsAttachedBothEnd;
    G4bool    fIsPSBinderOn;
    G4double  fPSBinderLength;
    G4double  fQE;  //teoric  
    G4bool    fIsQEDependsEnergy; 
   
    G4double  fPSLength; //not UI commands, just for visualization
    
        
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
