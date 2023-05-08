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

#ifndef NuSDAnalysisManager_h
#define NuSDAnalysisManager_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
//#include "NuSDAnalysis.hh" //geant4 11 de kaldırıldı
#include "G4AnalysisManager.hh"
#include "G4ThreadLocalSingleton.hh"

class NuSDAnalysisManager
{  
  friend class G4ThreadLocalSingleton<NuSDAnalysisManager>;  
    
  public:
    static NuSDAnalysisManager* GetInstance(); 
	  ~NuSDAnalysisManager();
	  
	  void Book();
	  void Save();
	  void EndOfEvent();
	
	private:
	  void FillNtuple(); 
	  void Reset();
	  void PrintEventResults()     const;
	  void PrintPhotonSDOutput()   const;
	  void PrintNeutrinoSDOutput() const;
	  void PrintNeutronSDOutput()  const;
 
  public:
	  //İnitial particles.3d vector
	  std::vector<G4double>& GetNeutrinoMomentumDirectionVec() { return fNeutrinoMomentumDirectionVec;}
	  std::vector<G4double>& GetPositronMomentumDirectionVec() { return fPositronMomentumDirectionVec;}
	  std::vector<G4double>& GetNeutronMomentumDirectionVec()  { return fNeutronMomentumDirectionVec;}
	  std::vector<G4double>& GetEventInitPosVec()              { return fEventInitPosVec; }
	  std::vector<G4double>& GetNeutronCapturePosVec()         { return fNeutronCapturePosVec; }
	  
	  std::vector<G4int>&    GetNuScntParticlePDGVec()  { return fNuScntParticlePDGVec; }
	  std::vector<G4int>&    GetNuScntTrackIDVec()   { return fNuScntTrackIDVec; }
	  std::vector<G4int>&    GetNuScntCopyNoVec()       { return fNuScntCopyNoVec; }
	  std::vector<G4double>& GetNuScntEdepVec()         { return fNuScntEdepVec; }
	  std::vector<G4double>& GetNuScntTimeVec()         { return fNuScntTimeVec; }
	    
    std::vector<G4int>&    GetNdParticlePDGVec() { return fNeutronScntParticlePDGVec; }
    std::vector<G4int>&    GetNdTrackIDVec()  { return fNeutronScntTrackIDVec; }
    std::vector<G4int>&    GetNdCopyNoVec()      { return fNeutronScntCopyNoVec; }
    std::vector<G4double>& GetNdEdepVec()        { return fNeutronScntEdepVec; }
    std::vector<G4double>& GetNdTimeVec()        { return fNeutronScntTimeVec; }
	  
	  std::vector<G4int>&    GetPhotonDetCopyNoVec() { return fPhotonDetCopyNoVec; }
	  std::vector<G4double>& GetPhotonDetTimeVec()   { return fPhotonDetTimeVec; }
	  
	  void SetEventID(G4int id);
	  void SetNeutrinoEnergy(G4double v_e);
	  void SetPositronEnergy(G4double pe);
	  void SetNeutronEnergy(G4double ne);
	  void SetNeutronAbsNucleusA(G4int massNo);
	  void SetNeutronCaptureTime(G4double nct);
	  
	  void IncEmittedPhotonNum();
  
	private:
	  NuSDAnalysisManager();
	  
	  G4bool fFactoryOn;  

	  G4int 	 fEventID;
    G4double fNeutrinoEnergy;
	  G4double fPositronEnergy;
	  G4double fNeutronEnergy;
	  G4int 	 fNeutronAbsNucleusA;
	  G4double fNeutronCaptureTime;
	  G4int 	 fEmittedScntPhotonNum;
	  
	  //3d vector
	  std::vector<G4double> fNeutrinoMomentumDirectionVec;
	  std::vector<G4double> fPositronMomentumDirectionVec;
	  std::vector<G4double> fNeutronMomentumDirectionVec;
	  std::vector<G4double> fEventInitPosVec; 
	  std::vector<G4double> fNeutronCapturePosVec;
		   
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

inline void NuSDAnalysisManager::SetEventID(G4int id)
{
  fEventID = id;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDAnalysisManager::SetNeutrinoEnergy(G4double v_e)
{
  fNeutrinoEnergy = v_e;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDAnalysisManager::SetNeutronEnergy(G4double ne)
{
  fNeutronEnergy = ne;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDAnalysisManager::SetPositronEnergy(G4double pe)
{
  fPositronEnergy = pe;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDAnalysisManager::SetNeutronAbsNucleusA(int massNo)
{
	fNeutronAbsNucleusA = massNo;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDAnalysisManager::SetNeutronCaptureTime(double nct)
{
	fNeutronCaptureTime =nct;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NuSDAnalysisManager::IncEmittedPhotonNum()
{
	fEmittedScntPhotonNum++;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

