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

# Preamble

cmake_minimum_required(VERSION 3.27.1 FATAL_ERROR)

# it's important to specify accurately the list of languages. for instance C and
# C++ as we _do_ have some C files to compile explicitely as C (e.g. gl3w.c)
project(O2 LANGUAGES C CXX VERSION 1.2.0)

include(CTest)

# Project wide setup

# Would better fit inside GPU/CMakeLists.txt, but include GPU/Common directly
set(ALIGPU_BUILD_TYPE "O2")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
set_property(GLOBAL PROPERTY REPORT_UNDEFINED_PROPERTIES)

cmake_host_system_information(RESULT _totalmem QUERY TOTAL_PHYSICAL_MEMORY)
math(EXPR _total_analysis_jobs "(${_totalmem}-4096)/10240")
if(_total_analysis_jobs LESS_EQUAL 0)
  set(_total_analysis_jobs 1)
endif()
set(ANALYSIS_COMPILE_POOL ${_total_analysis_jobs} CACHE STRING "How many parallel analysis compilation jobs")
set_property(GLOBAL PROPERTY JOB_POOLS analysis=${ANALYSIS_COMPILE_POOL})

include(O2BuildSanityChecks)
o2_build_sanity_checks()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(dependencies/FindONNXRuntime.cmake)

include(O2CheckCXXFeatures)
o2_check_cxx_features()

include(O2DefineOptions)
o2_define_options()
# see if upgrade is enabled
if (ENABLE_UPGRADES)
  add_definitions(-DENABLE_UPGRADES)
endif()

add_subdirectory(version)

include(O2DefineOutputPaths)
o2_define_output_paths()

include(O2DefineRPATH)
o2_define_rpath()

# External dependencies
include(dependencies/CMakeLists.txt)

# This is needed because CI might not have X11 or Cocoa available
if (DPL_TESTS_BATCH_MODE)
  set(DPL_WORKFLOW_TESTS_EXTRA_OPTIONS -b)
endif()

if (DPL_ENABLE_TRACING)
  add_definitions(-DDPL_ENABLE_TRACING)
endif()

if(DEFINED ENV{ENABLE_DEBUG_STREAMER})
  add_definitions(-DDEBUG_STREAMER)
  MESSAGE(STATUS "Enable debug streamer")
endif()

# include macros and functions that are used in the following subdirectories'
# CMakeLists.txt
include(O2AddExecutable)
include(O2AddHeaderOnlyLibrary)
include(O2AddLibrary)
include(O2AddTest)
include(O2AddTestRootMacro)
include(O2ReportNonTestedMacros)
include(O2TargetRootDictionary)
include(O2DataFile)
include(O2TargetManPage)
include(O2AddWorkflow)
include(O2SetROOTPCMDependencies)
include(O2AddHipifiedExecutable)
include(O2AddHipifiedLibrary)

# Main targets of the project in various subdirectories. Order matters.
add_subdirectory(Common)
add_subdirectory(Framework)
add_subdirectory(GPU)
add_subdirectory(DataFormats)
add_subdirectory(Utilities)
add_subdirectory(CCDB)
add_subdirectory(Algorithm)
add_subdirectory(Detectors)
add_subdirectory(EventVisualisation)
add_subdirectory(Generators)
add_subdirectory(Steer) # consider building this only for simulation ?
add_subdirectory(prodtests)
add_subdirectory(scripts)

if(BUILD_EXAMPLES)
  add_subdirectory(Examples)
endif()

if(BUILD_SIMULATION)
  add_subdirectory(run)
endif()

add_subdirectory(config)

add_custom_target(man ALL)
o2_target_man_page(man NAME o2)
o2_target_man_page(man NAME FairMQDevice)

# Testing and packaging only needed if we are the top level directory
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  # Documentation
  add_subdirectory(doc)
  if(BUILD_TESTING)
    add_subdirectory(tests)
  endif()
  if(BUILD_TEST_ROOT_MACROS)
    add_subdirectory(macro)
    o2_report_non_tested_macros()
  endif()
  add_subdirectory(packaging)
endif()

set_root_pcm_dependencies()
