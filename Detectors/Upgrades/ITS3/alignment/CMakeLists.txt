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

o2_add_library(ITS3Align
               SOURCES src/MisalignmentParameters.cxx
                       src/MisalignmentHits.cxx
                       src/MisalignmentManager.cxx
                       src/Deformations.cxx
               PUBLIC_LINK_LIBRARIES O2::MathUtils
                                     O2::Steer
                                     O2::ITSBase
                                     O2::ITSMFTSimulation)

o2_target_root_dictionary(ITS3Align
                          HEADERS include/ITS3Align/MisalignmentParameters.h
                                  include/ITS3Align/MisalignmentHits.h
                                  include/ITS3Align/MisalignmentHits.h
                                  include/ITS3Align/Deformations.h)
