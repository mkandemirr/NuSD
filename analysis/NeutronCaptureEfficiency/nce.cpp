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

/*
>>Possible detector name:sweany, chandler, solid, hsp, prospect, panda, nulat.
>>if you activate the USEDEBUG macro, event summary will be printed for a few
events.
*/

#define USEDEBUG
#define fileName "nulat"

#include <iostream>

void nce()
{
  gROOT->Reset();
  
	//boşlukara dikkat et yoksa macro hata veriyor!!!
  TChain * myChain1 = new TChain("ntuple/Ntuple1");
	myChain1->Add(" ../rootFiles/" fileName"/NuSD.root");
	myChain1->Add(" ../rootFiles/" fileName"/NuSD_t0.root");
	myChain1->Add(" ../rootFiles/" fileName"/NuSD_t1.root");
	/*
	myChain1->Add(" ../rootFiles/" fileName"/NuSD_t2.root");
	myChain1->Add(" ../rootFiles/" fileName"/NuSD_t3.root");
	myChain1->Add(" ../rootFiles/" fileName"/NuSD_t4.root");
	myChain1->Add(" ../rootFiles/" fileName"/NuSD_t5.root");
  */

  double  neutronCaptureTime;
  int     eventID{};
	double  neutronEnergy{}; //KeV
	int     neutronAbsNucleusA{};

  myChain1->SetBranchAddress("NeutronCaptureTime", &neutronCaptureTime);
  myChain1->SetBranchAddress("EventID",            &eventID);
	myChain1->SetBranchAddress("NeutronEnergy",      &neutronEnergy);
	myChain1->SetBranchAddress("NeutronAbsNucleusA", &neutronAbsNucleusA);

  //Gadolinium--------------------------   
  int count152  = 0;
  int count154  = 0;
  int count155  = 0;
  int count156  = 0;
  int count157  = 0;
  int count158  = 0;
  int count160  = 0;
  
  int countGd   = 0; //for all Gd nuclei
  
  //Counts neutrons captured by nuclei within a given time interval.
	int countGd_t0    = 0;
	int countGd_t1    = 0;
	int countGd_t2    = 0;
	int countGd_t3    = 0;
	int countGd_t4    = 0;
	int countGd_t5    = 0;
     
  //---------Lithium---------------------
  int countLi6    = 0;
	int countLi7    = 0;
	
	//Counts neutrons captured by nuclei within a given time interval.
	int countLi6_t0  = 0;
	int countLi6_t1  = 0;
	int countLi6_t2  = 0;
	int countLi6_t3  = 0;
	int countLi6_t4  = 0;
	int countLi6_t5  = 0;
	
	
	//--------Carbon--------
  int countC      = 0;
  //--------Hydrogen--------
  int countH      = 0;
 
  //-------Escape--------
	int countEscape = 0;
 
  //--------Total------------
	int countTotal  = 0;
	
#ifdef USEDEBUG
  std::size_t nentries = 100;
#else
  std::size_t nentries =  myChain1->GetEntries();
#endif 

	for (std::size_t i=0; i < nentries; i++) 
	{
   	myChain1->GetEntry(i);
   	
#ifdef USEDEBUG
    if(i==0)
    {
      cout<<std::left<<std::setw(7+5)<<"EventID"<<std::setw(18+5)<<"NeutronCaptureTime"<<std::setw(18+5)<<"NeutronAbsNucleusA"<<endl;
    }
    
    cout<<std::left<<std::setw(7+5)<<eventID<<std::setw(18+5)<<neutronCaptureTime<<std::setw(18+5)<<neutronAbsNucleusA<<endl;
#endif
   	  	
   	//----------------------ANALYSIS-----------------------------
   	   	
#ifndef USEDEBUG
     	
    switch(neutronAbsNucleusA)
    {
      case 1:
        countH++;
      break;
      case 6:
        countLi6++;    
        if(neutronCaptureTime>0 && neutronCaptureTime<=50)
          countLi6_t0++;
        else if(neutronCaptureTime>50 && neutronCaptureTime<=100)
          countLi6_t1++;
        else if(neutronCaptureTime>100 && neutronCaptureTime<=150)
          countLi6_t2++;
        else if(neutronCaptureTime>150 && neutronCaptureTime<=200)
          countLi6_t3++;
        else if(neutronCaptureTime>200 && neutronCaptureTime<=250)
          countLi6_t4++;
        else if(neutronCaptureTime>250 && neutronCaptureTime<=500)
          countLi6_t5++;
		  break;
      case 7:
      countLi7++;
      break;
      case 12:
      countC++;
      break;
      case 152:
      count152++;
      break;
      case 154:
      count154++;
      break;
      case 155:
      count155++;
      break;
      case 156:
      count156++;
      break;
      case 157:
      count157++;
      break;
      case 158:
      count158++;
      break;
      case 160:
      count160++;
      break; 
      case -1:
      countEscape++;
      break;

      default:
      break;

	  } //switch loop
	  
	  
	  if(neutronAbsNucleusA ==152 || neutronAbsNucleusA ==154 || neutronAbsNucleusA == 155 || neutronAbsNucleusA ==156 ||   
      neutronAbsNucleusA ==157 || neutronAbsNucleusA == 158 || neutronAbsNucleusA == 160)
	  {
	    countGd++;
	    
	    if(neutronCaptureTime>0 && neutronCaptureTime<=50)
        countGd_t0++;
      else if(neutronCaptureTime>50 && neutronCaptureTime<=100)
        countGd_t1++;
      else if(neutronCaptureTime>100 && neutronCaptureTime<=150)
        countGd_t2++;
      else if(neutronCaptureTime>150 && neutronCaptureTime<=200)
        countGd_t3++;
      else if(neutronCaptureTime>200 && neutronCaptureTime<=250)
        countGd_t4++;
      else if(neutronCaptureTime>250 && neutronCaptureTime<=500)
        countGd_t5++;
	    
	  }
	  
    if(neutronAbsNucleusA != -1) //not excape
	    countTotal++;
	       	
#endif
   	
    //------------------END---------------------------

	} //event loop

#ifndef USEDEBUG
	  
    //Hydrogen
    double hydrogenFrac = (double)countH/nentries;
    //-------Carbon---------------
	  double carbonFrac 	= (double)countC/nentries;
	  //------Escape-------------------
	  double escapeFrac 	=  (double)countEscape/nentries;
	  
	  //----Lithium-------------
	  double li6Frac 			= (double)countLi6/nentries;
	  double li7Frac 			= (double)countLi7/nentries;
	  
	   //Efficiency depends on neutron capture time.
	  double li60Frac 		= (double)countLi6_t0/nentries;
	  double li61Frac 		= (double)countLi6_t1/nentries;
	  double li62Frac 		= (double)countLi6_t2/nentries;
	  double li63Frac 		= (double)countLi6_t3/nentries;
	  double li64Frac 		= (double)countLi6_t4/nentries;
	  double li65Frac 		= (double)countLi6_t5/nentries;
	  	  
	  //---------Gadolinium------------------
	  double gd152Frac   = (double)count152/nentries;
    double gd154Frac   = (double)count154/nentries;
    double gd155Frac   = (double)count155/nentries;
    double gd156Frac   = (double)count156/nentries;
    double gd157Frac   = (double)count157/nentries;
    double gd158Frac   = (double)count158/nentries;
    double gd160Frac   = (double)count160/nentries;
    double totalGdFrac = (double)countGd/nentries; 
    
    //Efficiency depends on neutron capture time.
    double gd0Frac  = (double)countGd_t0/nentries;
	  double gd1Frac  = (double)countGd_t1/nentries;
	  double gd2Frac  = (double)countGd_t2/nentries;
	  double gd3Frac  = (double)countGd_t3/nentries;
	  double gd4Frac  = (double)countGd_t4/nentries;
	  double gd5Frac  = (double)countGd_t5/nentries;
      
	  //--------Total------------------------------------
	  double totalEfficiency 		= (double)countTotal/nentries; 
	  	    
	  cout<<"----------------RESULTS--------------------------"<<endl;
	  
	  cout<<"Detector name         : "<<fileName<<endl;
    cout<<"Total number of events: "<<nentries<<endl;
	  
	  cout<<std::setprecision(0)<<std::fixed<<endl;
    
    cout<<" Hydrogen  fraction   : "<<100*hydrogenFrac<<endl;
	  cout<<" Carbon  fraction     : "<<100*carbonFrac<<endl;
	  cout<<" Escape fraction      : "<<100*escapeFrac<<endl;
	  
	  cout<<"-----------Li-6 capture efficiency versus time-----"<<endl;	  
    cout<<" 0-50(um)             : "<<100*li60Frac<<endl;
    cout<<" 50-100(um)           : "<<100*li61Frac<<endl;
    cout<<" 100-150(um)          : "<<100*li62Frac<<endl;
    cout<<" 150-200(um)          : "<<100*li63Frac<<endl;
    cout<<" 200-250(um)          : "<<100*li64Frac<<endl;
    cout<<" 250-500(um)          : "<<100*li65Frac<<endl; 
	  
	  cout<<"--------Time independent-------"<<endl;   
	  cout<<" Li6 fraction         : "<<100*li6Frac<<endl;
	  cout<<" Li7 fraction         : "<<100*li7Frac<<endl; 
	   
    cout<<"-----------Gd capture efficiency versus time -----"<<endl;	
    
    cout<<" 0-50(um)             : "<<100*gd0Frac<<endl;
    cout<<" 50-100(um)           : "<<100*gd1Frac<<endl;
    cout<<" 100-150(um)          : "<<100*gd2Frac<<endl;
    cout<<" 150-200(um)          : "<<100*gd3Frac<<endl;
    cout<<" 200-250(um)          : "<<100*gd4Frac<<endl;
    cout<<" 250-500(um)          : "<<100*gd5Frac<<endl; 
	  
	  cout<<"--------Time independent-------"<<endl;   
    cout<<" Gd152 fraction       : "<<100*gd152Frac<<endl;
    cout<<" Gd154 fraction       : "<<100*gd154Frac<<endl;
    cout<<" Gd155 fraction       : "<<100*gd155Frac<<endl;
    cout<<" Gd156 fraction       : "<<100*gd156Frac<<endl;
    cout<<" Gd157 fraction       : "<<100*gd157Frac<<endl;
    cout<<" Gd158 fraction       : "<<100*gd158Frac<<endl;
    cout<<" Gd160 fraction       : "<<100*gd160Frac<<endl;
    cout<<" Total Gd fraction    : " <<100*totalGdFrac<<endl;

	  
    cout<<"----------Total------------------------------"<<endl;
	  cout<<" Total efficiency     : "<<totalEfficiency*100<<endl;
	    
#endif
  
  //cout<<"----------------------------------------------------------"<<endl;
    
}

