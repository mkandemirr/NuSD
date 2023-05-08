#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.21)
project(NuSD)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
set(DETECTOR_NAME PANDA CACHE STRING "")
option(NuSD_DEBUG "Debug mode" ON)
option(CREATE_ROOT_FILE "Save root file or not" ON)
option(GENERIC_PRIMARY_GENERATOR "Custom Primary Generator" OFF)
configure_file(NuSD_config.h.in NuSD_config.h) #copy

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Locate sources and headers for this project, include them compiler search path

include_directories(${PROJECT_SOURCE_DIR}/shared/include 
                    ${PROJECT_SOURCE_DIR}/detectors/${DETECTOR_NAME}/include
                    ${PROJECT_SOURCE_DIR}/OPSimTool/include
                    ${PROJECT_SOURCE_DIR}/EljenScintillators/include
                    ${PROJECT_SOURCE_DIR}/NuSDMaterials/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIRS}
                   )
#message("Geant4_INCLUDE_DIR: ${Geant4_INCLUDE_DIR}")
#message("ROOT_INCLUDE_DIR: ${ROOT_INCLUDE_DIRS}")

file(GLOB headers ${PROJECT_SOURCE_DIR}/shared/include/*.hh
                  ${PROJECT_SOURCE_DIR}/detectors/${DETECTOR_NAME}/include/*.hh
                  ${PROJECT_SOURCE_DIR}/OPSimTool/include/*.hh 
                  ${PROJECT_SOURCE_DIR}/EljenScintillators/include/*.hh
                  ${PROJECT_SOURCE_DIR}/NuSDMaterials/include/*.hh
    )

file(GLOB sources ${PROJECT_SOURCE_DIR}/shared/src/*.cc 
                  ${PROJECT_SOURCE_DIR}/detectors/${DETECTOR_NAME}/src/*.cc
                  ${PROJECT_SOURCE_DIR}/OPSimTool/src/*.cc 
                  ${PROJECT_SOURCE_DIR}/EljenScintillators/src/*.cc  
                  ${PROJECT_SOURCE_DIR}/NuSDMaterials/src/*.cc
    )
                  
#message("headers: ${headers} ")
#message("sources: ${sources} ")

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(NuSD NuSD.cc ${sources} ${headers}) 
target_link_libraries(NuSD ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#Bunu kapatınca NuSD_config.h görünmüyor
target_include_directories(NuSD PUBLIC "${PROJECT_BINARY_DIR}")

#message("Geant libraries: ${Geant4_LIBRARIES}")
#message("Root libraries: ${ROOT_LIBRARIES} ")

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build NuSD. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.

file(GLOB macro RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac  )
file(GLOB macro1 RELATIVE ${PROJECT_SOURCE_DIR} EljenScintillators/macros/*.mac  )
file(GLOB macro2 RELATIVE ${PROJECT_SOURCE_DIR} NuSDMaterials/macros/*.mac  )

file(GLOB_RECURSE data RELATIVE ${PROJECT_SOURCE_DIR} data/*)
file(GLOB_RECURSE data1 RELATIVE ${PROJECT_SOURCE_DIR} EljenScintillators/data/*)
file(GLOB_RECURSE data2 RELATIVE ${PROJECT_SOURCE_DIR} NuSDMaterials/data/*)


foreach(_file ${macro} ${macro1} ${macro2}  ${data} ${data1} ${data2} ${other_files} )
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
    )
endforeach()

#copy output directory
file(COPY ${PROJECT_SOURCE_DIR}/output DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/neutrinoSpectrum DESTINATION ${PROJECT_BINARY_DIR})


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS NuSD DESTINATION bin)

