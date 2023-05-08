# NuSD
Neutrino Segmented Detector is a Geant4-based user application that simulates inverse beta decay event in a variety of segmented scintillation detectors developed by different international collaborations. It currently includes seven different detector concepts:

1. SWEANY
2. CHANDLER
3. SOLID 
4. NULAT
5. PROSPECT
6. HSP
7. PANDA 

## The package structure:
_shared/include/_ : It includes common header files for all detector types

_shared/src/_  : It includes common source files for all detector types

_detectors/_  : it includes detector construction files for each detector type.

_data/_  : It includes the reflectivity spectrum of the reflector and the quantum efficiency spectrum of the photosensor to be used in the construction of detectors. Users can change them with their own data.

_OPSimTool/_  : OPSimTool is a custom library that significantly simplifies the implementation process of optical components in Geant4. It allows users to create portable, reusable materials build code while working on their own projects. The directory named EljenScintillators in this package is created based on this library. This library is independent of NuSD and anyone performing optical photon simulations with Geant4 can utilize it.

_EljenScintillators/_ : This directory includes the implementation of some commercially available Eljen scintillators. This library is independent of NuSD. It uses OPSimTool library. Those who wish to use these scintillators in their projects should copy the OPSimTool directory together with this directory into their own project source directory. 

_NuSDMaterials/_ : This includes some special materials used in NuSD. It utilizes the OPSimTool library for the creation of material build classes. 

_macros/_ : It includes a macro file for each detector setup and several macros for simulation settings. A macro file is a collection of user interface commands. 

_neutrinoSpectrum/_  : It contains a root file in which two histograms are written: reactor anti-neutrino emission spectrum and positron scattering angle. These two histograms are used to determine the initial values of the primary particles in inverse beta decay events. 

_analysis/_  : It includes many Root macro files for the analysis of the simulation data. For example, neutron capture efficiency, anti-neutrino detection efficiency, neutron capture time, and light collection efficiency. 

_output/_ : If you activate the CREATE_ROOT_FILE preprocessor macro in NuSD_config.h file, the root files generated from the simulation are located here.                                                

_testRun/_ : It includes two sample runs of NuSD.                        

_NuSD.cc_    : It contains main() for the application.

_NuSD_config.h.in_ : It includes several preprocessor macros to conditionally compile certain parts of the program. This is the main configuration file of NuSD. 

## System requirements:
Tested on:
* Os: Linux (Ubuntu 22.04.2 LTS )
* Compiler: gcc 11.3.0
* CLHEP 2.4.6.4 
* Geant4 11.1.1 
* ROOT 6.28.02 
* CMake 3.26.0 

## How to run the NuSD?   
 1. Go in your home directory and create a subdirectory named NuSD
  ```bash
  cd ~
  mkdir NuSD
  ```
2. Copy the NuSD.zip archive into the NuSD folder you created and extract it in this location.
  ```bash
  cp ~/Downloads/NuSD.zip ./ (assuming NuSD.tar.gz is in the ~/Downloads folder)
  unzip NuSD.zip
  ```
3. Create a NuSD-build folder in the same directory with NuSD (in this example it is your home directory).
  ```bash
  mkdir NuSD-build
  ```
4. Now go inside this build directory and run CMake to generate the Makefiles needed to build the NuSD application. Select the detector type you want to study using the flag "DETECTOR_NAME". Suppose you want to study with CHANDLER:  
  ```bash
  cd NuSD-build
  cmake -DDETECTOR_NAME=CHANDLER -S ../NuSD -B . 
  ```
  (available detector name = SWEANY, CHANDLER, SOLID, NULAT, PROSPECT, HSP, PANDA)

5. Step 4 generates a configuration file in the build directory named "NuSD_config.h". Open this file and configure some initial simulation settings (for detail read section 2.1 in the respective articles). With the Makefile available, we can now build by simply running make:
  ```bash
  make -jN
  ```
6. The application is now ready to run. If you now list the contents of your build directory you will see the NuSD application executable has been created. The program can be run in two modes: interactive mode and batch mode. The first is for visualization and the second is for collecting statistics. \

* "-b": run in batch mode,  need an argument to specify the macro file corresponding to the selected detector. 
* "-i": run in interactive, need an argument to specify the macro file corresponding to the selected detector.  
* "-r": Set the seed 
* "-t": specify the number of threads (note: -t option is available only for multi-threaded mode.) 

Following are some examples. Let's assume that chandler is selected. The associated macro file is chandler.mac, which is located in the macros/ under the build directory. \  
```bash
./NuSD -i chandler.mac 
./NuSD -b chandler.mac  
./NuSD -b chandler.mac -t 2 
./NuSD -b chandler.mac -t 3 -r 345354 
./NuSD 
```
7. If you want to change the selected detector type, return to step 4 and follows the next steps. 

##  Contacts 
If you have any questions or wish to notify of updates and/or modification please contact: \
Mustafa Kandemir (mustafa.kandemir@erdogan.edu.tr)

## CITATION

@article{KANDEMIR2022108387, \
title = {NuSD: A Geant4 based simulation framework for segmented anti-neutrino detectors}, \
journal = {Computer Physics Communications}, \
volume = {277}, \
pages = {108387}, \
year = {2022}, \
issn = {0010-4655}, \
doi = {https://doi.org/10.1016/j.cpc.2022.108387 }, \
url = {https://www.sciencedirect.com/science/article/pii/S0010465522001060 }, \
author = {Mustafa Kandemir and Emrah Tiras and Vincent Fischer}, \
keywords = {NuSD, Geant4 based program, Segmented scintillation detector, Reactor anti-neutrino, Inverse beta decay simulation, Neutrino interaction, Near-field reactor monitoring} \
}
