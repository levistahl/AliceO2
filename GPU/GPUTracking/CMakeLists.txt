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

set(MODULE GPUTracking)
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# set(CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE_UPPER} "${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE_UPPER}} -O0") # to uncomment if needed, tired of typing this...

if(NOT "${GPUCA_NO_FAST_MATH}" AND NOT CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
endif()

include(cmake/helpers.cmake)

if(ALIGPU_BUILD_TYPE STREQUAL "ALIROOT")
  if(ENABLE_CUDA OR ENABLE_OPENCL1 OR ENABLE_OPENCL2 OR ENABLE_HIP)
    include(FeatureSummary)
    find_package(O2GPU)
  else()
    include("cmake/kernel_helpers.cmake")
  endif()
else()
  if((ALIGPU_BUILD_TYPE STREQUAL "Standalone" AND BUILD_EVENT_DISPLAY) OR (ALIGPU_BUILD_TYPE STREQUAL "O2" AND NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND TARGET AliceO2::DebugGUI AND OPENGL_FOUND AND GLFW_FOUND))
    set(GPUCA_EVENT_DISPLAY ON)
  endif()
  if(ROOT_FOUND)
    set(GPUCA_QA ON)
  endif()
endif()

# General sources
set(SRCS
    SliceTracker/GPUTPCTrack.cxx
    SliceTracker/GPUTPCTrackParam.cxx
    SliceTracker/GPUTPCStartHitsFinder.cxx
    SliceTracker/GPUTPCStartHitsSorter.cxx
    SliceTracker/GPUTPCNeighboursCleaner.cxx
    SliceTracker/GPUTPCTracker.cxx
    SliceTracker/GPUTPCSliceData.cxx
    SliceTracker/GPUTPCSliceOutput.cxx
    SliceTracker/GPUTPCTrackletConstructor.cxx
    SliceTracker/GPUTPCSectorDebugSortKernels.cxx
    SliceTracker/GPUTPCCreateOccupancyMap.cxx
    SliceTracker/GPUTPCNeighboursFinder.cxx
    SliceTracker/GPUTPCGrid.cxx
    SliceTracker/GPUTPCTrackletSelector.cxx
    SliceTracker/GPUTPCRow.cxx
    SliceTracker/GPUTPCGlobalTracking.cxx
    SliceTracker/GPUTPCCreateSliceData.cxx
    Merger/GPUTPCGMMerger.cxx
    Merger/GPUTPCGMSliceTrack.cxx
    Merger/GPUTPCGMTrackParam.cxx
    Merger/GPUTPCGMPropagator.cxx
    Merger/GPUTPCGlobalDebugSortKernels.cxx
    Merger/GPUTPCGMPhysicalTrackModel.cxx
    Merger/GPUTPCGMPolynomialFieldManager.cxx
    DataTypes/GPUTRDTrack.cxx
    TRDTracking/GPUTRDTracker.cxx
    TRDTracking/GPUTRDTrackletWord.cxx
    TRDTracking/GPUTRDTrackerKernels.cxx
    Base/GPUParam.cxx)

set(SRCS_DATATYPES DataTypes/GPUDataTypes.cxx DataTypes/GPUConfigDump.cxx)
set(SRCS_DATATYPE_HEADERS DataTypes/GPUTPCGMPolynomialField.cxx)

set(HDRS_CINT_O2 Merger/GPUTPCGMMergedTrack.h Merger/GPUTPCGMSliceTrack.h Merger/GPUTPCGMBorderTrack.h)
set(HDRS_CINT_DATATYPES DataTypes/GPUTPCGMMergedTrackHit.h)
set(HDRS_CINT_O2_ADDITIONAL DataTypes/GPUSettings.h Definitions/GPUSettingsList.h DataTypes/GPUDataTypes.h DataTypes/GPUTRDTrack.h DataTypes/CalibdEdxTrackTopologyPol.h DataTypes/CalibdEdxTrackTopologySpline.h) # Manual dependencies for ROOT dictionary generation

set(SRCS_NO_CINT
    DataTypes/GPUMemorySizeScalers.cxx
    DataTypes/GPUNewCalibValues.cxx
    DataTypes/GPUTPCClusterOccupancyMap.cxx
    Base/GPUReconstruction.cxx
    Base/GPUReconstructionCPU.cxx
    Base/GPUProcessor.cxx
    Base/GPUMemoryResource.cxx
    Base/GPUGeneralKernels.cxx
    Base/GPUReconstructionDeviceBase.cxx
    Base/GPUReconstructionConvert.cxx
    Base/GPUKernelDebugOutput.cxx
    Global/GPUChain.cxx
    Global/GPUChainTracking.cxx
    Global/GPUTrackingInputProvider.cxx
    Global/GPUErrors.cxx
    Merger/GPUTPCGMMergerGPU.cxx
    Debug/GPUROOTDumpCore.cxx
    utils/timer.cxx)

set(SRCS_NO_H SliceTracker/GPUTPCTrackerDump.cxx
              Merger/GPUTPCGMMergerDump.cxx
              Base/GPUReconstructionLibrary.cxx
              Global/GPUChainTrackingClusterizer.cxx
              Global/GPUChainTrackingTransformation.cxx
              Global/GPUChainTrackingTRD.cxx
              Global/GPUChainTrackingRefit.cxx
              Global/GPUChainTrackingSliceTracker.cxx
              Global/GPUChainTrackingMerger.cxx
              Global/GPUChainTrackingCompression.cxx
              Global/GPUChainTrackingDebugAndProfiling.cxx
              Global/GPUChainTrackingIO.cxx)

set(HDRS_INSTALL
    Base/GPUConstantMem.h
    Base/GPUParam.inc
    Base/GPUParamRTC.h
    Base/GPUReconstructionHelpers.h
    Base/GPUReconstructionIncludes.h
    Base/GPUReconstructionIncludesITS.h
    Base/GPUReconstructionKernelMacros.h
    Base/GPUReconstructionKernels.h
    DataCompression/GPUTPCClusterRejection.h
    DataCompression/GPUTPCCompressionKernels.inc
    DataCompression/TPCClusterDecompressor.inc
    DataTypes/GPUdEdxInfo.h
    DataTypes/GPUHostDataTypes.h
    DataTypes/GPUO2DataTypes.h
    DataTypes/GPUO2FakeClasses.h
    DataTypes/GPUOutputControl.h
    DataTypes/GPUSettings.h
    DataTypes/GPUTPCGeometry.h
    DataTypes/GPUTPCGMMergedTrackHit.h
    DataTypes/GPUTRDDef.h
    DataTypes/GPUTRDInterfaceO2Track.h
    DataTypes/GPUTriggerOutputs.h
    Debug/GPUROOTDump.h
    Definitions/GPUDefConstantsAndSettings.h
    Definitions/GPUDefGPUParameters.h
    Definitions/GPUDef.h
    Definitions/GPUDefMacros.h
    Definitions/GPUDefOpenCL12Templates.h
    Definitions/GPULogging.h
    Definitions/GPUSettingsList.h
    Global/GPUChainTrackingDefs.h
    Global/GPUErrorCodes.h
    Merger/GPUTPCGMBorderTrack.h
    Merger/GPUTPCGMMergedTrack.h
    Merger/GPUTPCGMMergerTypes.h
    qa/GPUQAHelper.h
    qconfigoptions.h
    Refit/GPUTrackParamConvert.h
    SliceTracker/GPUTPCBaseTrackParam.h
    SliceTracker/GPUTPCClusterData.h
    SliceTracker/GPUTPCDef.h
    SliceTracker/GPUTPCHit.h
    SliceTracker/GPUTPCHitId.h
    SliceTracker/GPUTPCMCInfo.h
    SliceTracker/GPUTPCSliceOutCluster.h
    SliceTracker/GPUTPCTracklet.h
    SliceTracker/GPUTPCTrackLinearisation.h
    TPCConvert/GPUTPCConvertImpl.h
    TRDTracking/GPUTRDGeometry.h
    TRDTracking/GPUTRDInterfaces.h
    TRDTracking/GPUTRDSpacePoint.h
    TRDTracking/GPUTRDTrackData.h
    TRDTracking/GPUTRDTrackerDebug.h
    TRDTracking/GPUTRDTrackletLabels.h
    TRDTracking/GPUTRDTrackPoint.h
    TRDTracking/GPUTRDTrackPoint.h
)

# Sources for O2 and for Standalone if requested in config file
if(ALIGPU_BUILD_TYPE STREQUAL "O2" OR ALIGPU_BUILD_TYPE STREQUAL "Standalone")
  set(SRCS_NO_CINT ${SRCS_NO_CINT} display/GPUDisplayInterface.cxx)
endif()
if(ALIGPU_BUILD_TYPE STREQUAL "O2" OR CONFIG_O2_EXTENSIONS)
    set(SRCS_NO_CINT
        ${SRCS_NO_CINT}
        Global/GPUChainITS.cxx
        ITS/GPUITSFitter.cxx
        ITS/GPUITSFitterKernels.cxx
        dEdx/GPUdEdx.cxx
        TPCConvert/GPUTPCConvert.cxx
        TPCConvert/GPUTPCConvertKernel.cxx
        DataCompression/GPUTPCCompression.cxx
        DataCompression/GPUTPCCompressionTrackModel.cxx
        DataCompression/GPUTPCCompressionKernels.cxx
        DataCompression/GPUTPCDecompression.cxx
        DataCompression/GPUTPCDecompressionKernels.cxx
        DataCompression/TPCClusterDecompressor.cxx
        DataCompression/GPUTPCClusterStatistics.cxx
        TPCClusterFinder/GPUTPCClusterFinder.cxx
        TPCClusterFinder/ClusterAccumulator.cxx
        TPCClusterFinder/MCLabelAccumulator.cxx
        TPCClusterFinder/GPUTPCCFCheckPadBaseline.cxx
        TPCClusterFinder/GPUTPCCFStreamCompaction.cxx
        TPCClusterFinder/GPUTPCCFChargeMapFiller.cxx
        TPCClusterFinder/GPUTPCCFPeakFinder.cxx
        TPCClusterFinder/GPUTPCCFNoiseSuppression.cxx
        TPCClusterFinder/GPUTPCCFClusterizer.cxx
        TPCClusterFinder/GPUTPCCFDeconvolution.cxx
        TPCClusterFinder/GPUTPCCFMCLabelFlattener.cxx
        TPCClusterFinder/GPUTPCCFDecodeZS.cxx
        TPCClusterFinder/GPUTPCCFGather.cxx
        Refit/GPUTrackingRefit.cxx
        Refit/GPUTrackingRefitKernel.cxx
        Merger/GPUTPCGMO2Output.cxx)

    set(SRCS_DATATYPES
        ${SRCS_DATATYPES}
        DataTypes/TPCPadGainCalib.cxx
        DataTypes/TPCPadBitMap.cxx
        DataTypes/TPCZSLinkMapping.cxx
        DataTypes/CalibdEdxContainer.cxx
        DataTypes/CalibdEdxTrackTopologyPol.cxx
        DataTypes/CalibdEdxTrackTopologySpline.cxx)

    set(SRCS_DATATYPE_HEADERS
        ${SRCS_DATATYPE_HEADERS}
        DataTypes/GPUTRDTrackO2.cxx)

    set(SRCS_NO_H ${SRCS_NO_H}
        TPCClusterFinder/GPUTPCClusterFinderDump.cxx)

    set(HDRS_INSTALL ${HDRS_INSTALL}
                     ITS/GPUITSTrack.h
                     TPCClusterFinder/Array2D.h
                     TPCClusterFinder/CfConsts.h
                     TPCClusterFinder/CfFragment.h
                     TPCClusterFinder/CfUtils.h
                     TPCClusterFinder/ChargePos.h
                     Definitions/clusterFinderDefs.h
                     TPCClusterFinder/PackedCharge.h
                     TPCClusterFinder/GPUTPCCFChainContext.h)
endif()

if(ALIGPU_BUILD_TYPE STREQUAL "O2")
  set(SRCS_DATATYPES
      ${SRCS_DATATYPES}
      Interface/GPUO2InterfaceConfigurableParam.cxx)
endif()

# Sources only for AliRoot
if(ALIGPU_BUILD_TYPE STREQUAL "ALIROOT")
  set(SRCS
      ${SRCS}
      Merger/GPUTPCGlobalMergerComponent.cxx
      SliceTracker/GPUTPCTrackerComponent.cxx
      Merger/GPUTPCGMTracksToTPCSeeds.cxx
      DataCompression/AliHLTTPCClusterStatComponent.cxx
      TRDTracking/GPUTRDTrackerComponent.cxx
      TRDTracking/GPUTRDTrackletReaderComponent.cxx
      Global/AliHLTGPUDumpComponent.cxx)

  set(SRCS_NO_CINT ${SRCS_NO_CINT}
                   ${CMAKE_SOURCE_DIR}/HLT/TPCLib/AliHLTTPCGeometry.cxx
                   ${CMAKE_SOURCE_DIR}/HLT/TPCLib/AliHLTTPCLog.cxx
                   ${CMAKE_SOURCE_DIR}/HLT/TPCLib/AliHLTTPCDefinitions.cxx
                   ${CMAKE_SOURCE_DIR}/HLT/TRD/AliHLTTRDDefinitions.cxx)

  set(HDRS_INSTALL ${HDRS_INSTALL} SliceTracker/GPUTPCDefinitions.h)
endif()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include_gpu_onthefly)
file(GENERATE
     OUTPUT include_gpu_onthefly/GPUReconstructionKernelList.h
     INPUT Base/GPUReconstructionKernels.template.h
)
file(GENERATE
     OUTPUT include_gpu_onthefly/GPUReconstructionKernelIncludes.h
     INPUT Base/GPUReconstructionKernelIncludes.template.h
)
file(GENERATE
     OUTPUT include_gpu_onthefly/GPUReconstructionIncludesDeviceAll.h
     INPUT Base/GPUReconstructionIncludesDeviceAll.template.h
)
if(NOT ALIGPU_BUILD_TYPE STREQUAL "O2")
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/include_gpu_onthefly)
endif()
set(HDRS_INSTALL ${HDRS_INSTALL} ${CMAKE_CURRENT_BINARY_DIR}/include_gpu_onthefly/GPUReconstructionKernelList.h)
include(kernels.cmake)

# Optional sources depending on optional dependencies
if(GPUCA_EVENT_DISPLAY)
  add_subdirectory(display)
endif()

if(GPUCA_QA)
  set(SRCS_NO_CINT ${SRCS_NO_CINT} qa/GPUQA.cxx)
else()
  set(HDRS_INSTALL ${HDRS_INSTALL} qa/GPUQA.h)
endif()

# Update HDR variables with files derrived from SRC variables
string(REPLACE ".cxx" ".h" HDRS_SRCS "${SRCS}")
string(REPLACE ".cxx" ".h" HDRS_TMP "${SRCS_NO_CINT}")
set(HDRS_INSTALL ${HDRS_INSTALL} ${HDRS_TMP})
string(REPLACE ".cxx" ".h" HDRS_TMP "${SRCS_DATATYPES}")
set(HDRS_CINT_DATATYPES ${HDRS_CINT_DATATYPES} ${HDRS_TMP})
unset(HDRS_TMP)

# Main CMake part for O2
if(ALIGPU_BUILD_TYPE STREQUAL "O2")
  o2_add_library(GPUDataTypeHeaders
                 TARGETVARNAME targetName
                 PUBLIC_INCLUDE_DIRECTORIES .
                                            Definitions
                                            DataTypes
                 PUBLIC_LINK_LIBRARIES O2::GPUCommon
                                       O2::ReconstructionDataFormats
                                       O2::TPCFastTransformation
                 PRIVATE_LINK_LIBRARIES O2::DataFormatsTPC
                 SOURCES ${SRCS_DATATYPE_HEADERS})
  target_compile_definitions(${targetName} PRIVATE GPUCA_O2_LIB GPUCA_TPC_GEOMETRY_O2 GPUCA_HAVE_O2HEADERS)

  o2_add_library(GPUDataTypes
                 TARGETVARNAME targetName
                 PUBLIC_LINK_LIBRARIES O2::GPUDataTypeHeaders O2::GPUUtils
                 PRIVATE_LINK_LIBRARIES O2::DataFormatsTPC
                 SOURCES ${SRCS_DATATYPES})
  target_compile_definitions(${targetName} PRIVATE GPUCA_O2_LIB GPUCA_TPC_GEOMETRY_O2 GPUCA_HAVE_O2HEADERS)
  o2_target_root_dictionary(GPUDataTypes
                            HEADERS ${HDRS_CINT_DATATYPES} ${HDRS_CINT_O2_ADDITIONAL}
                            LINKDEF GPUTrackingLinkDef_O2_DataTypes.h)

  o2_add_library(${MODULE}
                 TARGETVARNAME targetName
                 PUBLIC_LINK_LIBRARIES O2::GPUCommon
                                       O2::GPUUtils
                                       O2::GPUDataTypes
                                       O2::DataFormatsTPC
                                       O2::DataFormatsTOF
                                       O2::TPCBase
                                       O2::TRDBase
                                       O2::TOFBase
                                       O2::ITSBase
                                       O2::ITStracking
                                       O2::TPCFastTransformation
                                       O2::DetectorsRaw
                                       O2::Steer
                 PUBLIC_INCLUDE_DIRECTORIES .
                                            Definitions
                                            DataTypes
                                            Base
                                            SliceTracker
                                            TPCConvert
                                            dEdx
                                            ITS
                                            TRDTracking
                                            qa
                                            Global
                                            HLTHeaders
                                            Interface
                                            Merger
                                            Refit
                                            Debug
                                            DataCompression
                                            TPCClusterFinder
                                            ${CMAKE_CURRENT_BINARY_DIR}/include_gpu_onthefly
                 SOURCES ${SRCS} ${SRCS_NO_CINT} ${SRCS_NO_H})

  target_include_directories(
    ${targetName}
    PRIVATE $<TARGET_PROPERTY:O2::Framework,INTERFACE_INCLUDE_DIRECTORIES>)

  target_compile_definitions(${targetName} PRIVATE GPUCA_O2_LIB
                             GPUCA_TPC_GEOMETRY_O2 GPUCA_HAVE_O2HEADERS)

  o2_target_root_dictionary(${MODULE}
                            HEADERS ${HDRS_CINT_O2} ${HDRS_CINT_O2_ADDITIONAL}
                            LINKDEF GPUTrackingLinkDef_O2.h)

  install(FILES ${HDRS_SRCS} ${HDRS_CINT_O2} ${HDRS_CINT_DATATYPES} ${HDRS_INSTALL}
          DESTINATION include/GPU)
  install(DIRECTORY utils
          DESTINATION include/GPU
          FILES_MATCHING PATTERN *.h)

  o2_add_test_root_macro(Standalone/tools/createGeo.C
                         PUBLIC_LINK_LIBRARIES O2::GPUTracking
                         LABELS its COMPILE_ONLY)
  o2_add_test_root_macro(Standalone/tools/createLUT.C
                         PUBLIC_LINK_LIBRARIES O2::GPUTracking
                         LABELS its COMPILE_ONLY)

  add_subdirectory(Interface)
endif()

# Main CMake part for AliRoot
if(ALIGPU_BUILD_TYPE STREQUAL "ALIROOT")
  set(HDRS_SRCS ${HDRS_SRCS}
                        DataTypes/GPUO2DataTypes.h)
  add_definitions(-DGPUCA_ALIROOT_LIB)
  include_directories(qa)
  include_directories(SYSTEM ${ROOT_INCLUDE_DIR})
  include_directories(${CMAKE_SOURCE_DIR}/HLT/BASE
                      ${CMAKE_SOURCE_DIR}/HLT/BASE/util
                      ${CMAKE_SOURCE_DIR}/HLT/TPCLib
                      ${CMAKE_SOURCE_DIR}/HLT/TPCLib/transform
                      ${CMAKE_SOURCE_DIR}/HLT/TPCLib/comp
                      ${CMAKE_SOURCE_DIR}/HLT/TRD
                      ${CMAKE_SOURCE_DIR}/TRD/TRDbase
                      ${CMAKE_SOURCE_DIR}/STEER/STEERBase
                      ${CMAKE_SOURCE_DIR}/STEER/STEER
                      ${CMAKE_SOURCE_DIR}/STEER/ESD
                      ${CMAKE_SOURCE_DIR}/STEER/CDB
                      ${CMAKE_SOURCE_DIR}/TPC/TPCbase
                      ${CMAKE_SOURCE_DIR}/TPC/TPCcalib
                      ${CMAKE_SOURCE_DIR}/TPC/TPCrec
                      ${CMAKE_SOURCE_DIR}/GPU/Common
                      ${CMAKE_SOURCE_DIR}/GPU/Utils
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Debug
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Definitions
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/DataTypes
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Base
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/SliceTracker
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Merger
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Global
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/TPCConvert
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/dEdx
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/TRDTracking
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/utils
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Base/cuda
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Base/hip
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Base/opencl-common
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Base/opencl
                      ${CMAKE_SOURCE_DIR}/GPU/GPUTracking/Base/opencl2
                      ${CMAKE_SOURCE_DIR}/GPU/TPCFastTransformation)
  alice_usevc()

  # Generate the dictionary
  get_directory_property(incdirs INCLUDE_DIRECTORIES)
  generate_dictionary_flat("Ali${MODULE}" "GPUTrackingLinkDef_AliRoot.h"
                           "${HDRS_SRCS}" "${incdirs}")

  # Generate the ROOT map Dependecies
  set(LIBDEPS
      STEERBase
      STEER
      ESD
      CDB
      HLTbase
      TPCbase
      TPCrec
      TPCcalib
      TRDbase
      AliTPCFastTransformation)
  generate_rootmap("Ali${MODULE}" "${LIBDEPS}"
                   "${CMAKE_CURRENT_SOURCE_DIR}/GPUTrackingLinkDef_AliRoot.h")
  # Don't pass Vc to root
  set(LIBDEPS ${LIBDEPS} Vc)

  # Add a library to the project using the specified source files
  add_library_tested(Ali${MODULE}
                     SHARED
                     ${SRCS}
                     ${SRCS_NO_CINT}
                     ${SRCS_NO_H}
                     ${SRCS_DATATYPES}
                     ${SRCS_DATATYPE_HEADERS}
                     G__Ali${MODULE}.cxx)
  target_link_libraries(Ali${MODULE} ${LIBDEPS})

  # Additional compilation flags
  set_target_properties(Ali${MODULE} PROPERTIES COMPILE_FLAGS "")

  # System dependent: Modify the way the library is build
  if(${CMAKE_SYSTEM} MATCHES Darwin)
    set_target_properties(Ali${MODULE}
                          PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
  endif(${CMAKE_SYSTEM} MATCHES Darwin)

  # Installation
  install(TARGETS Ali${MODULE} ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)

  install(FILES ${HDRS_SRCS} ${HDRS_INSTALL} DESTINATION include)
  install(DIRECTORY utils
          DESTINATION include
          FILES_MATCHING PATTERN *.h)

  set(targetName Ali${MODULE})
  add_library(O2::${MODULE} ALIAS Ali${MODULE})
endif()

# Main CMake part for Standalone
if(ALIGPU_BUILD_TYPE STREQUAL "Standalone")
  add_library(${MODULE} SHARED ${SRCS} ${SRCS_NO_CINT} ${SRCS_NO_H} ${SRCS_DATATYPES} ${SRCS_DATATYPE_HEADERS})
  set(targetName ${MODULE})
  add_library(O2::${MODULE} ALIAS ${MODULE})
  install(TARGETS ${MODULE})

  if(CONFIG_ROOT)
    ROOT_GENERATE_DICTIONARY(G__${MODULE} ${HDRS_CINT_O2} ${HDRS_CINT_DATATYPES} ${HDRS_CINT_O2_ADDITIONAL} GPUTrackingLinkDef_Standalone.h)
    target_sources(${MODULE} PRIVATE G__${MODULE})
    INSTALL(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/lib${MODULE}_rdict.pcm
      ${CMAKE_CURRENT_BINARY_DIR}/lib${MODULE}.rootmap
    DESTINATION .)
  endif()
endif()

# Disable all non-deterministic floating point to make TPC track model encoding / decoding precise
set_source_files_properties(DataCompression/GPUTPCCompressionTrackModel.cxx
                            DataCompression/GPUTPCCompressionKernels.cxx
                            DataCompression/TPCClusterDecompressor.cxx
                            DataCompression/GPUTPCDecompressionKernels.cxx
                            TARGET_DIRECTORY ${targetName}
                            PROPERTIES
                            COMPILE_FLAGS "${GPUCA_CXX_NO_FAST_MATH_FLAGS}"
                            COMPILE_DEFINITIONS "GPUCA_NO_FAST_MATH")

# GPUReconstructionLibrary needs to know which GPU backends are enabled for proper error messages
configure_file(Base/GPUReconstructionAvailableBackends.template.h ${CMAKE_CURRENT_BINARY_DIR}/GPUReconstructionAvailableBackends.h)
set_source_files_properties(Base/GPUReconstructionLibrary.cxx
                            PROPERTIES
                            INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}")


# Add compile definitions and libraries depending on available optional dependencies
if(GPUCA_QA)
  message(STATUS "Building GPU QA")
  target_compile_definitions(${targetName} PRIVATE GPUCA_BUILD_QA)
endif()

if(OpenMP_CXX_FOUND)
  message(STATUS "GPU: Using OpenMP: ${OpenMP_CXX_SPEC_DATE}")
  # Must be private, depending libraries might be compiled by compiler not understanding -fopenmp
  target_compile_definitions(${targetName} PRIVATE WITH_OPENMP)
  target_link_libraries(${targetName} PRIVATE OpenMP::OpenMP_CXX)
endif()

target_compile_options(${targetName} PRIVATE -Wno-instantiation-after-specialization)

# Add CMake recipes for GPU Tracking librararies
if(CUDA_ENABLED OR OPENCL1_ENABLED OR OPENCL2_ENABLED OR HIP_ENABLED)
  if(CMAKE_SYSTEM_NAME MATCHES Darwin)
    message(WARNING "GPU Tracking disabled on MacOS")
  else()
    if(CUDA_ENABLED)
      add_subdirectory(Base/cuda)
    endif()
    if(OPENCL1_ENABLED OR OPENCL2_ENABLED)
      add_subdirectory(Base/opencl-common)
    endif()
    if(OPENCL1_ENABLED)
      add_subdirectory(Base/opencl)
    endif()
    if(OPENCL2_ENABLED)
      add_subdirectory(Base/opencl2)
    endif()
    if(HIP_ENABLED)
      add_subdirectory(Base/hip)
    endif()
  endif()
endif()

if(ALIGPU_BUILD_TYPE STREQUAL "O2" OR ALIGPU_BUILD_TYPE STREQUAL "Standalone")
  if(NOT ${CMAKE_SYSTEM} MATCHES Darwin)
    add_subdirectory(Standalone/Benchmark)
  endif()
endif()

if(${GPUCA_NO_FAST_MATH})
  target_compile_definitions(${targetName} PUBLIC GPUCA_NO_FAST_MATH)
endif()
