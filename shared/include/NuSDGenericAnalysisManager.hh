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

#ifndef NuSDGenericAnalysisManager_h
#define NuSDGenericAnalysisManager_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "globals.hh"
#include "G4ThreadLocalSingleton.hh"

class NuSDGenericAnalysisManager
{
  
  friend class G4ThreadLocalSingleton<NuSDGenericAnalysisManager>;  
  
  public:
    static NuSDGenericAnalysisManager* GetInstance(); 
	  ~NuSDGenericAnalysisManager();

	  void Book();
	  void Save();
	  void EndOfEvent();
	
	private:
	  void FillNtuple(); 
	  void Reset();
	  void PrintEventResults() const;
	  void PrintPhotonSDOutput() const;
	  void PrintNeutrinoSDOutput() const;
	  void PrintNeutronSDOutput() const;
 
  public:
	  std::vector<G4double>& GetEventInitPosVec() { return fEventInitPosVec; }
	  
	  std::vector<G4int>&    GetNuScntParticlePDGVec() { return fNuScntParticlePDGVec; }
	  std::vector<G4int>&    GetNuScntTrackIDVec() { return fNuScntTrackIDVec; }
	  std::vector<G4int>&    GetNuScntCopyNoVec() { return fNuScntCopyNoVec; }
	  std::vector<G4double>& GetNuScntEdepVec() { return fNuScntEdepVec; }
	  std::vector<G4double>& GetNuScntTimeVec() { return fNuScntTimeVec; }
	  	  
    std::vector<G4int>&    GetNdParticlePDGVec() { return fNeutronScntParticlePDGVec; }
    std::vector<G4int>&    GetNdTrackIDVec() { return fNeutronScntTrackIDVec; }
    std::vector<G4int>&    GetNdCopyNoVec() { return fNeutronScntCopyNoVec; }
    std::vector<G4double>& GetNdEdepVec() { return fNeutronScntEdepVec; }
    std::vector<G4double>& GetNdTimeVec() { return fNeutronScntTimeVec; }
	  
	  std::vector<G4int>&    GetPhotonDetCopyNoVec() { return fPhotonDetCopyNoVec; }
	  std::vector<G4double>& GetPhotonDetTimeVec()  { return fPhotonDetTimeVec; }
	  
	  void SetEventID(G4int id);
	  void SetParticleEnergy(G4double pe);
	    
	  void IncEmittedPhotonNum();
  
	private:
	  NuSDGenericAnalysisManager();

	  G4bool fFactoryOn;  

	  G4int 	 fEventID;
    G4double fParticleEnergy;
	  G4int 	 fEmittedScntPhotonNum;
	  
	  //3d vector
	  std::vector<G4double> fEventInitPosVec; 
	  
	  std::vector<G4int>    fNuScntParticlePDGVec; 
	  std::vector<G4int>    fNuScntTrackIDVec; 
	  std::vector<G4int>    fNuScntCopyNoVec; 
    std::vector<G4double> fNuScntEdepVec; 
    std::vector<G4double> fNuScntTimeVec;  
       
    std::vector<G4int>    fNeutronScntParticlePDGVec; 
	  std::vector<G4int>    fNeutronScntTrackIDVec; 
	  std::vector<G4int>    fNeutronScntCopyNoVec; 
    std::vector<G4double> fNeutronScntEdepVec; 
    std::vector<G4double> fNeutronScntTimeVec;   
    
    std::vector<G4int>    fPhotonDetCopyNoVec;
    std::vector<G4double> fPhotonDetTimeVec;  //arrival time
         
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDGenericAnalysisManager::SetEventID(G4int id)
{
  fEventID = id;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDGenericAnalysisManager::SetParticleEnergy(G4double pe)
{
  fParticleEnergy = pe;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDGenericAnalysisManager::IncEmittedPhotonNum()
{
	fEmittedScntPhotonNum++;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

