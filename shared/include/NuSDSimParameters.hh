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

#ifndef NuSDSimParameters_h
#define NuSDSimParameters_h 1

#include "globals.hh"

class NuSDSimParametersMessenger;

class NuSDSimParameters
{
  public:
    static NuSDSimParameters* GetInstance();
    virtual ~NuSDSimParameters();  
  
  private:
    NuSDSimParameters();
  
  //UI commands
  public: 
    void SetOpticalPhysicsFlag(G4bool bl) {fIsOpticalPhysicsOn = bl;}
  
  //Getters
  public:
    G4bool IsOpticalPhysicsOn() const {return fIsOpticalPhysicsOn; }
   
  private:
    static NuSDSimParameters *fInstance;  //singleton class
    NuSDSimParametersMessenger *fMessenger;
 
    G4bool fIsOpticalPhysicsOn;
}; 

#endif
