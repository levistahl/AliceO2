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

o2_add_library(DPLUtils
               SOURCES src/Utils.cxx
                       src/RawParser.cxx
                       test/RawPageTestData.cxx
               PUBLIC_LINK_LIBRARIES O2::Framework ROOT::Tree ROOT::TreePlayer O2::CommonUtils)

o2_add_executable(raw-proxy
                  COMPONENT_NAME dpl
                  SOURCES src/raw-proxy.cxx
                  PUBLIC_LINK_LIBRARIES O2::Framework O2::DPLUtils)

o2_add_executable(raw-parser
                  COMPONENT_NAME dpl
                  SOURCES src/raw-parser.cxx
                  PUBLIC_LINK_LIBRARIES O2::Framework O2::DPLUtils)

o2_add_executable(output-proxy
                  COMPONENT_NAME dpl
                  SOURCES src/dpl-output-proxy.cxx
                  PUBLIC_LINK_LIBRARIES O2::Framework O2::DPLUtils)


o2_add_test(RootTreeWriterWorkflow
            NO_BOOST_TEST
            SOURCES test/test_RootTreeWriterWorkflow.cxx
            PUBLIC_LINK_LIBRARIES O2::DPLUtils
            COMPONENT_NAME DPLUtils
            LABELS dplutils
            COMMAND_LINE_ARGS ${DPL_WORKFLOW_TESTS_EXTRA_OPTIONS} --run)

o2_add_test(RootTreeReader
            NO_BOOST_TEST
            SOURCES test/test_RootTreeReader.cxx
            PUBLIC_LINK_LIBRARIES O2::DPLUtils
            COMPONENT_NAME DPLUtils
            LABELS dplutils
            COMMAND_LINE_ARGS ${DPL_WORKFLOW_TESTS_EXTRA_OPTIONS} --run)

add_executable(o2-test-framework-utils
  test/test_RootTreeWriter.cxx
  test/test_RawParser.cxx
  test/test_DPLRawParser.cxx
  test/test_DPLRawPageSequencer.cxx
)
target_link_libraries(o2-test-framework-utils PRIVATE O2::Framework O2::DPLUtils O2::DetectorsRaw)
target_link_libraries(o2-test-framework-utils PRIVATE O2::Catch2)

get_filename_component(outdir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../tests ABSOLUTE)
set_property(TARGET o2-test-framework-utils PROPERTY RUNTIME_OUTPUT_DIRECTORY ${outdir})

add_test(NAME framework:utils COMMAND o2-test-framework-utils)

if (TARGET benchmark::benchmark)
foreach(b
        RawParser
        DPLRawPageSequencer
        )
  o2_add_test(benchmark_${b} NAME test_Framework_benchmark_${b}
              SOURCES test/benchmark_${b}.cxx
              COMPONENT_NAME DPLUtils
              LABELS dplutils benchmark
              PUBLIC_LINK_LIBRARIES O2::DPLUtils benchmark::benchmark O2::DetectorsRaw)
endforeach()
endif()
