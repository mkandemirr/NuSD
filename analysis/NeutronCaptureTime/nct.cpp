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

//#define USEDEBUG
#define fileName "prospect"
void nct()
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
  int     eventID{};
	double  neutronCaptureTime{};
	int     neutronAbsNucleusA{};
	double  neutronEnergy{};

  myChain1->SetBranchAddress("EventID",             &eventID);
	myChain1->SetBranchAddress("NeutronCaptureTime",  &neutronCaptureTime); //us
	myChain1->SetBranchAddress("NeutronAbsNucleusA",  &neutronAbsNucleusA);
	myChain1->SetBranchAddress("NeutronEnergy",       &neutronEnergy);

#ifdef USEDEBUG
  std::size_t nentries =  100;
#else
  std::size_t nentries =  myChain1->GetEntries();
#endif  

#ifndef USEDEBUG
  double  minTimeCut = 0.;
  TH1D *hNct = new TH1D("Neutron capture time dist","BB",500,0.,500.);
#endif 

	for (std::size_t i=0; i<nentries; ++i) 
	{
		myChain1->GetEntry(i);
		
#ifdef USEDEBUG
    if(i==0)
    {
      cout<<std::left<<std::setw(7+5)<<"EventID"<<std::setw(18+5)<<"NeutronCaptureTime"<<std::setw(18+5)<<"NeutronAbsNucleusA"<<endl;
    }

    cout<<std::left<<std::setw(7+5)<<eventID<<std::setw(18+5)<<neutronCaptureTime<<std::setw(18+5)<<neutronAbsNucleusA<<endl;
#endif
   	
   	//----------------------ANALYSIS----------------
   	 	
#ifndef USEDEBUG
    if( neutronCaptureTime>minTimeCut)
      hNct->Fill(neutronCaptureTime);
#endif
   	 	
   	//------------------ANALYSIS END----------------
   	
	}//event loop

#ifndef USEDEBUG
  hNct->Draw();

  cout<<std::setprecision(0)<<std::fixed<<endl;
  cout<<"Mean neutron capture time: "<<hNct->GetMean()<<endl;
#endif

  cout<<"Detector name: "<<fileName<<endl;
  cout<<"Total number of events: "<<nentries<<endl;
  
} 
