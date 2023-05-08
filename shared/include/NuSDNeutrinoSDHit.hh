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

#ifndef NuSDNeutrinoSDHit_h
#define NuSDNeutrinoSDHit_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

class NuSDNeutrinoSDHit : public G4VHit
{
  public:
    NuSDNeutrinoSDHit();
    NuSDNeutrinoSDHit(const NuSDNeutrinoSDHit&);
    virtual ~NuSDNeutrinoSDHit();

    // operators
    const NuSDNeutrinoSDHit& operator=(const NuSDNeutrinoSDHit&);
    G4bool operator==(const NuSDNeutrinoSDHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print() {};

    G4int GetCopyNo() const {return fCopyNo; }
    G4int GetParticlePDG() const {return fParticlePDG;}
    G4int GetTrackID() const {return fTrackID;} 
    G4double GetEdep() const {return fEdep; }
    G4double GetTime() const {return fTime; }
    
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

using NuSDNeutrinoSDHitsCollection = G4THitsCollection<NuSDNeutrinoSDHit>;
//typedef G4THitsCollection<NuSDNeutrinoSDHit> NuSDNeutrinoSDHitsCollection;

extern G4ThreadLocal G4Allocator<NuSDNeutrinoSDHit>* NuSDNeutrinoSDHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* NuSDNeutrinoSDHit::operator new(size_t)
{
  if (!NuSDNeutrinoSDHitAllocator) {
    NuSDNeutrinoSDHitAllocator = new G4Allocator<NuSDNeutrinoSDHit>;
  }
  void *hit;
  hit = (void *) NuSDNeutrinoSDHitAllocator->MallocSingle();
  return hit;
}

inline void NuSDNeutrinoSDHit::operator delete(void *hit)
{
  if (!NuSDNeutrinoSDHitAllocator) {
    NuSDNeutrinoSDHitAllocator = new G4Allocator<NuSDNeutrinoSDHit>;
  }
  NuSDNeutrinoSDHitAllocator->FreeSingle((NuSDNeutrinoSDHit*) hit);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
