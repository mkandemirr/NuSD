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

#ifndef NuSDPhotonSDHit_h
#define NuSDPhotonSDHit_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

class NuSDPhotonSDHit : public G4VHit
{
  public:
    NuSDPhotonSDHit();
    NuSDPhotonSDHit(const NuSDPhotonSDHit&);
    virtual ~NuSDPhotonSDHit();

    // operators
    const NuSDPhotonSDHit& operator=(const NuSDPhotonSDHit&);
    G4bool operator==(const NuSDPhotonSDHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print() {};
 
    G4int GetCopyNo() const { return fCopyNo; }
    G4double GetArrivalTime() const { return fArrivalTime; }

    void SetArrivalTime(G4double time) { fArrivalTime = time; }
    void SetCopyNo(G4int cpyNo) { fCopyNo = cpyNo; }
  
  private:
    G4int fCopyNo;
    G4double  fArrivalTime; 
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using NuSDPhotonSDHitsCollection = G4THitsCollection<NuSDPhotonSDHit>;
//typedef G4THitsCollection<NuSDPhotonSDHit> NuSDPhotonSDHitsCollection;

extern G4ThreadLocal G4Allocator<NuSDPhotonSDHit>* NuSDPhotonSDHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* NuSDPhotonSDHit::operator new(size_t)
{
  if (!NuSDPhotonSDHitAllocator) 
  {
    NuSDPhotonSDHitAllocator = new G4Allocator<NuSDPhotonSDHit>;
  }
  void *hit;
  hit = (void *) NuSDPhotonSDHitAllocator->MallocSingle();
  return hit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDPhotonSDHit::operator delete(void *hit)
{
  if (!NuSDPhotonSDHitAllocator) 
  {
    NuSDPhotonSDHitAllocator = new G4Allocator<NuSDPhotonSDHit>;
  }
  NuSDPhotonSDHitAllocator->FreeSingle((NuSDPhotonSDHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
