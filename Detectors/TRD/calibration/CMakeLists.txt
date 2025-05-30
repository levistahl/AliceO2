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

add_subdirectory(macros)

o2_add_library(TRDCalibration
               SOURCES src/TrackBasedCalib.cxx
                       src/CalibratorVdExB.cxx
                       src/CalibratorGain.cxx
                       src/CalibratorNoise.cxx
                       src/T0Fit.cxx
                       src/PadCalibCCDBBuilder.cxx
                       src/KrClusterFinder.cxx
                       src/DCSProcessor.cxx
                       src/CalibrationParams.cxx
                       src/PulseHeight.cxx
               PUBLIC_LINK_LIBRARIES O2::TRDBase
                                     O2::DataFormatsTRD
                                     O2::DataFormatsGlobalTracking
                                     O2::DetectorsBase
                                     O2::DetectorsCalibration
                                     O2::MathUtils
                                     O2::DetectorsDCS)

 o2_target_root_dictionary(TRDCalibration
                           HEADERS include/TRDCalibration/TrackBasedCalib.h
                                   include/TRDCalibration/CalibratorVdExB.h
                                   include/TRDCalibration/CalibratorGain.h
                                   include/TRDCalibration/CalibratorNoise.h
                                   include/TRDCalibration/T0Fit.h
                                   include/TRDCalibration/CalibrationParams.h
                                   include/TRDCalibration/PadCalibCCDBBuilder.h
                                   include/TRDCalibration/KrClusterFinder.h
                                   include/TRDCalibration/PulseHeight.h
                                   include/TRDCalibration/DCSProcessor.h)

o2_add_executable(trd-dcs-sim-workflow
                 COMPONENT_NAME calibration
                 SOURCES workflow/trd-dcs-sim-workflow.cxx
                 PUBLIC_LINK_LIBRARIES O2::DCStestWorkflow)

o2_add_executable(trd-dcs-workflow
                 COMPONENT_NAME calibration
                 SOURCES workflow/trd-dcs-data-workflow.cxx
                 PUBLIC_LINK_LIBRARIES O2::Framework
                                      O2::TRDCalibration
                                      O2::DetectorsDCS)
