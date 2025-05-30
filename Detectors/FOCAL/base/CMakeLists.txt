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

o2_add_library(FOCALBase
    SOURCES src/Composition.cxx
    src/EventReader.cxx
    src/Geometry.cxx
    src/Hit.cxx
    src/TestbeamAnalysis.cxx
    PUBLIC_LINK_LIBRARIES O2::DataFormatsFOCAL O2::SimulationDataFormat O2::CommonUtils ROOT::RIO ROOT::Tree ROOT::TreePlayer
    Microsoft.GSL::GSL)

o2_target_root_dictionary(
    FOCALBase
    HEADERS include/FOCALBase/Composition.h
    include/FOCALBase/EventReader.h
    include/FOCALBase/Geometry.h
    include/FOCALBase/Hit.h
    include/FOCALBase/TestbeamAnalysis.h
)