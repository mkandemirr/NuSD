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

#include "NuSDFileManager.hh"
#include <fstream>
#include "G4SystemOfUnits.hh"

#undef NDEBUG
#include <assert.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDFileManager* NuSDFileManager::fInstance = 0;

NuSDFileManager::NuSDFileManager()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDFileManager::~NuSDFileManager()
{	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NuSDFileManager* NuSDFileManager::GetInstance()
{
  if (!fInstance)
    {
      fInstance = new NuSDFileManager();
    }
  return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFileManager::ReadSpectrumFromFile(const G4String &filename, std::vector<G4double>& quantity)
{
  G4String line;
  G4double something;

  std::ifstream myfile (filename.c_str());

  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      std::istringstream iss(line);

      if ( (iss >> something  ) ) 
      {
        quantity.push_back(something);
      } 

    }

    myfile.close();
  }
  else 
  {
    G4Exception("NuSDFileManager", "FileManagerErr0", FatalException, "Unable to open file");  
  } 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFileManager::ReadSpectrumFromFile(const G4String &filename, std::vector<G4double> &quantity, G4double unit)
{

  G4String line;
  G4double something;
  
  std::ifstream myfile (filename.c_str());

  if (myfile.is_open())
  {
  
    while ( getline (myfile,line) )
    {
    
      std::istringstream iss(line);
       
      if ( (iss >> something ) ) 
       {       
         quantity.push_back(something*unit);
       } 
               
    }
        
    myfile.close();
  }

  else 
  {
    G4Exception("NuSDFileManager", "FileManagerErr1", FatalException, "Unable to open file");  
  } 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NuSDFileManager::ReadSpectrumFromFile(const G4String &filename, std::vector<G4double> &photonEnergy, 
                                           std::vector<G4double> &value, G4double unit1, G4double unit2
                                           )
{
  G4String line;
  
  G4double column1;
  G4double column2;
   
  std::ifstream myfile (filename.c_str() );


  if (myfile.is_open())
  {
  
    while ( getline (myfile,line) )
    {
      std::istringstream iss(line);
         
      if ( (iss >> column1 >> column2 ) ) 
      {           
        photonEnergy.push_back(column1*unit1);
        value.push_back(column2*unit2); 
      } 
         
    }
       
    myfile.close();
  }

  else 
  { 
    G4Exception("NuSDFileManager", "FileManagerErr3", FatalException, "Unable to open file ");  
  } 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/*
//used for PrimaryGeneratorAction, not used for now..
void NuSDFileManager::ReadSpectrumFromFile(const G4String &filename, std::vector<G4double> &fX, std::vector<G4double> &fY)
{

  G4String line;

  G4double posEnergy;
  G4double posWeight;

  std::ifstream myfile (filename.c_str());

  if (myfile.is_open())
  {

    while ( getline (myfile,line) )
    {

      std::istringstream iss(line);
      
      if ( (iss >> posEnergy >> posWeight) ) 
      {
        fX.push_back(posEnergy*MeV);
        fY.push_back(posWeight);
      } 

    }

    myfile.close();
  }
  else 
  {
    G4Exception("NuSDFileManager", "FileManagerErr4", FatalException, "Unable to open file");  
  } 


}

*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
