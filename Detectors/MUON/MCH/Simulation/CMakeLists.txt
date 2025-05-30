# Copyright 2019-2020 CERN and copyright holders of ALICE O2.
# See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
# All rights not expressly granted are reserved.
#
# This software is distributed under the terms of the GNU General Public
# License v3 (GPL Version 3), copied verbatim in the file "COPYING".
#
# In applying this license CERN does not waive the privileges and immunities
# granted to it by virtue of its status as an Intergovernmental Organization
# or submit itself to any jurisdiction.

o2_add_library(MCHSimulation
               SOURCES src/Detector.cxx
                       src/DEDigitizer.cxx
                       src/Digitizer.cxx
                       src/DigitizerParam.cxx
                       src/Hit.cxx
                       src/Stepper.cxx
                       src/Stepper.h
                       src/Response.cxx
                PUBLIC_LINK_LIBRARIES O2::DataFormatsMCH
                                      O2::DetectorsBase
                                      O2::DetectorsPassive
                                      O2::MCHBase
                                      O2::MCHGeometryCreator
                                      O2::MCHMappingInterface
                                      O2::SimulationDataFormat)

o2_target_root_dictionary(MCHSimulation
                          HEADERS include/MCHSimulation/Detector.h
                                  include/MCHSimulation/Digitizer.h
                                  include/MCHSimulation/DigitizerParam.h
                                  include/MCHSimulation/Hit.h
                                  include/MCHSimulation/Response.h)

o2_add_executable(inspect-collision-context
                  COMPONENT_NAME mch
                  SOURCES src/inspect-collision-context.cxx
                  PUBLIC_LINK_LIBRARIES O2::SimulationDataFormat)

o2_data_file(COPY data DESTINATION Detectors/MCH/simulation)

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
