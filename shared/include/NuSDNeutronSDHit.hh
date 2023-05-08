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

#ifndef NuSDNeutronSDHit_h
#define NuSDNeutronSDHit_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

class NuSDUserEventInformation;

class NuSDNeutronSDHit : public G4VHit
{
  public:
    NuSDNeutronSDHit();
    NuSDNeutronSDHit(const NuSDNeutronSDHit&);
    virtual ~NuSDNeutronSDHit();

    // operators
    const NuSDNeutronSDHit& operator=(const NuSDNeutronSDHit&);
    G4bool operator==(const NuSDNeutronSDHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print() {};

    G4int GetCopyNo()       const {return fCopyNo; }
    G4int GetParticlePDG()  const {return fParticlePDG;}
    G4int GetTrackID()   const {return fTrackID;} 
    G4double GetEdep()      const {return fEdep; }
    G4double GetTime()      const {return fTime; }
    
    void SetCopyNo(G4int cpyNo) {fCopyNo = cpyNo;}
    void SetParticlePDG(G4int pPDG) {fParticlePDG = pPDG;}
    void SetTrackID(G4int pID) {fTrackID = pID;}
    void SetTime(G4double time) {fTime = time;}
    void SetEdep(G4double edep) {fEdep = edep;}
  
  private:
    G4int fCopyNo;
    G4int fParticlePDG;
    G4int fTrackID;
    G4double fEdep;
    G4double fTime;
        
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using NuSDNeutronSDHitsCollection = G4THitsCollection<NuSDNeutronSDHit>;
//typedef G4THitsCollection<NuSDNeutronSDHit> NuSDNeutronSDHitsCollection;

extern G4ThreadLocal G4Allocator<NuSDNeutronSDHit>* NuSDNeutronSDHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* NuSDNeutronSDHit::operator new(size_t)
{
  if (!NuSDNeutronSDHitAllocator) 
  {
    NuSDNeutronSDHitAllocator = new G4Allocator<NuSDNeutronSDHit>;
  }
  void *hit;
  hit = (void *) NuSDNeutronSDHitAllocator->MallocSingle();
  return hit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDNeutronSDHit::operator delete(void *hit)
{
  if (!NuSDNeutronSDHitAllocator) 
  {
    NuSDNeutronSDHitAllocator = new G4Allocator<NuSDNeutronSDHit>;
  }
  NuSDNeutronSDHitAllocator->FreeSingle((NuSDNeutronSDHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
