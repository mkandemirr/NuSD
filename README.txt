                         NuSD 
=========================*****================================
NuSD: Neutrino Segmented Detector is a Geant4-based user application that simulates inverse beta decay event in a variety of
segmented scintillation detectors developed by different international collaborations. It currently includes seven different detector concepts:

I.    SWEANY
II.   CHANDLER
III.  SOLID 
IV.   NULAT
V.    PROSPECT
VI.   HSP
VII.  PANDA                 
 
                  The package structure:
=========================*****================================

include/               : header files

src/                   : source files

data/                  : It includes experimental input data for optical photon simulation. For example, scintillator      emissionspectrum,refractive index, absorption length, photosensor quantum efficiency, reflector reflectivity. For detailed information, read section 2.3 of the corresponding article

macros/                : It includes a macro file for each detector setup and several macros for simulation settings. A macro file is a collection of user interface commands. 

neutrinoSpectrum/      : It contains a root file in which two histograms are written: reactor anti-neutrino emission spectrum and positron scattering angle. These two histograms are used to determine the initial values of the primary particles in inverse beta decay events. 

analysis/              : It includes many Root macro files for the analysis of the simulation data. For example, neutron capture efficiency, anti-neutrino detection efficiency, neutron capture time, and light collection efficiency. 

output/                : If you activate the CREATE_ROOT_FILE preprocessor macro in NuSD_config.h file, the root files generated from the simulation are located here.                                                

testRun/               : It includes two sample runs of NuSD.                        

CMakeList              : It contains commands which describe how to build the NuSD.

NuSD.cc                : It contains main() for the application.

NuSD_config.h.in       : It includes several preprocessor macros to conditionally compile certain parts of the program. This is the main configuration file of NuSD. 


                  System requirements:
=========================*****================================

Tested on:
    Os: Linux (Ubuntu 18.04.5 LTS )
    Architecture: x86_64
    Compiler: gcc 7.5.0
   
Mandatory:
   - CLHEP (recommended version 2.4.1.3 or greater)
   - Geant4.10.0.6 or greater 
   - ROOT 6.20.0.0 or greater
   - CMake version 3.10.2 or greater

Optional: 
   - OpenGL / QT (for interactive visualiser)
   
   
                  How to run the NuSD application?   
=========================*****================================   
Step 1 
  Go in your home directory and create a subdirectory named NuSD
  $ cd ~
  $ mkdir NuSD

Step 2 
  Copy the NuSD.zip archive into the NuSD folder you created and extract it in this location.
  $ cp ~/Downloads/NuSD.zip ./ (assuming NuSD.tar.gz is in the ~/Downloads folder)
  $ unzip NuSD.zip

Step 3
  Create a NuSD-build folder in the same directory with NuSD (in this example it is your home directory).
  $ mkdir NuSD-build

Step 4
  Now go inside this build directory and run CMake to generate the Makefiles needed to build the NuSD application. Two arguments are passed to CMake. The first argument points CMake to our install of Geant4. Specifically, it is the directory holding the   
Geant4Config.cmake file that Geant4 installs to help CMake find and use Geant4. It is assumed that you have already installed Geant4 in your home area under, for illustration only, /home/you/geant4-install. The second argument to CMake is the path to the source directory of the application we want to build. Here it’s just the NuSD directory as indicated earlier. CMake will now run to configure the build and generate Makefiles.
  $ cd NuSD-build
  $ cmake -DGeant4_DIR=/home/you/geant4-install/lib64/Geant4-G4VERSION $HOME/NuSD 

Step 5
 Step 4 generates a configuration file in the build directory named "NuSD_config.h". Open this file and select the desired detector type from the list you want to study, and configure some initial simulation settings (for detail read section 2.1 in the respective articles).

Step 6
  With the Makefile available, we can now build by simply running make: 
  $ make -jN
  CMake generated Makefiles support parallel builds, so N can be set to the number of cores on your machine (e.g. if you have four core processor, you could set N to 4).

Step 7
  The application is now ready to run. If you now list the contents of your build directory you will see the NuSD application executable has been created. The program can be run in two modes: interactive mode and batch mode. The first is for visualization and the second is for collecting statistics.

    "-b": run in batch mode,  need an argument to specify the macro file corresponding to the selected detector.
    "-i": run in interactive, need an argument to specify the macro file corresponding to the selected detector.
    "-r": Set the seed
    "-t": specify the number of threads (note: -t option is available only for multi-threaded mode.) 

   Following are some examples. Let's assume that chandler is selected. The associated macro file is chandler.mac, which is located in the macros/ under the build directory.  
    ./NuSD -i chandler.mac
    ./NuSD -b chandler.mac 
    ./NuSD -b chandler.mac -t 2
    ./NuSD -b chandler.mac -t 3 -r 345354
    ./NuSD   

Step 8
  If you want to change the selected detector type or change any settings in the NuSD_config.h file, return to step 5 and 
  follows the next steps. 
   
                        Contacts 
=========================*****================================
If you have any questions or wish to notify of updates and/or modification please contact:
 	
Mustafa Kandemir (mustafa.kandemir@erdogan.edu.tr)

  
