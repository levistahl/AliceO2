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

o2_add_library(ITS3Base
               SOURCES src/SegmentationSuperAlpide.cxx
                       src/ITS3Params.cxx
               PUBLIC_LINK_LIBRARIES O2::CommonConstants O2::MathUtils O2::DetectorsBase)

o2_target_root_dictionary(ITS3Base
                          HEADERS include/ITS3Base/SegmentationSuperAlpide.h
                                  include/ITS3Base/ITS3Params.h)
