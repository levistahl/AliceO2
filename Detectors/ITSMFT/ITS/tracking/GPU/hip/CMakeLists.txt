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

if(HIP_ENABLED)
  message(STATUS "Building ITS HIP tracker")
  set(CMAKE_HIP_FLAGS "${CMAKE_HIP_FLAGS} -fgpu-rdc")
  o2_add_hipified_library(ITStrackingHIP
                 SOURCES ../cuda/ClusterLinesGPU.cu
                         ../cuda/Context.cu
                         ../cuda/TimeFrameGPU.cu
                         ../cuda/Stream.cu
                         ../cuda/TrackerTraitsGPU.cxx
                         ../cuda/TracerGPU.cu
                         ../cuda/TrackingKernels.cu
                         ../cuda/VertexingKernels.cu
                         ../cuda/VertexerTraitsGPU.cxx
                         ../cuda/Utils.cu
                 PUBLIC_INCLUDE_DIRECTORIES ../
                 PUBLIC_LINK_LIBRARIES O2::ITStracking
                                       O2::GPUTracking
                                       O2::SimulationDataFormat
                                       O2::ReconstructionDataFormats
                                       hip::host
                 PRIVATE_LINK_LIBRARIES O2::GPUTrackingHIPExternalProvider
                 TARGETVARNAME targetName)
endif()
