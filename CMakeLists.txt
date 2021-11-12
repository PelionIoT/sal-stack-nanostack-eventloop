#################################################################################
## Copyright 2020-2021 Pelion.
##
## SPDX-License-Identifier: Apache-2.0
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
#################################################################################

if ((NOT DEFINED OS_BRAND))
target_include_directories(mbed-nanostack-sal_stack-event_loop
    INTERFACE
        .
        ./nanostack-event-loop
        ./nanostack-event-loop/platform
        ./source
)

target_sources(mbed-nanostack-sal_stack-event_loop
    INTERFACE
        source/event.c
        source/ns_timeout.c
        source/ns_timer.c
        source/system_timer.c
)
else()

cmake_minimum_required (VERSION 3.14)
SET(CMAKE_SYSTEM_NAME Generic)

include(FindUnixCommands)

project(nanostackeventloop)

option(enable_coverage_data "Enable Coverage data" OFF)

include(repositories.cmake)

# libservice is needed for ns-list and nsdynmem, so add dependencies
add_library(nanostackeventloop STATIC)
add_library(nanostackeventloopInterface INTERFACE)
target_include_directories(nanostackeventloopInterface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop)

target_sources(nanostackeventloop PRIVATE
    source/event.c
    source/nanostack_eventloop.c
    source/ns_timer.c
    source/system_timer.c
)

target_link_libraries(nanostackeventloop nanostack-libservice)

target_include_directories(nanostackeventloop PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(nanostackeventloop PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop)
target_include_directories(nanostackeventloop PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/platform)
target_include_directories(nanostackeventloop PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)

# Tests after this line
enable_testing()

if (enable_coverage_data)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif ()

add_executable(ns_timer_test EXCLUDE_FROM_ALL
    source/ns_timer.c
    test/ns_timer/ns_timer_test.cpp
    test/stubs/hal_stubs.c
    test/stubs/nsdynmemLIB_stub.c
    test/stubs/ns_list_stub.c
)
# make check, this must be after add_executable!
add_test(ns_timer_test ns_timer_test)
if (TARGET check)
    add_dependencies(check ns_timer_test)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS ns_timer_test)
endif()

target_include_directories(ns_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop)
target_include_directories(ns_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/platform)
target_include_directories(ns_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_include_directories(ns_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/stubs)
target_include_directories(ns_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
    ns_timer_test
    gtest_main
    nanostack-libserviceInterface
)

add_executable(nanostack_eventloop_test EXCLUDE_FROM_ALL
    source/nanostack_eventloop.c
    test/nanostack_eventloop/nanostack_eventloop_test.cpp
    test/stubs/hal_stubs.c
    test/stubs/event_stub.c
    test/stubs/system_timer_stub.c
)
# make check, this must be after add_executable!
add_test(nanostack_eventloop_test nanostack_eventloop_test)
if (TARGET check)
    add_dependencies(check nanostack_eventloop_test)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS nanostack_eventloop_test)
endif()

target_include_directories(nanostack_eventloop_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop)
target_include_directories(nanostack_eventloop_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/platform)
target_include_directories(nanostack_eventloop_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_include_directories(nanostack_eventloop_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/stubs)

target_link_libraries(
    nanostack_eventloop_test
    gtest_main
    nanostack-libserviceInterface
)

# We need to have a test also with NANOSTACK_USE_PAL_FUNCTION_POINTERS defined
add_executable(nanostack_eventloop_test2 EXCLUDE_FROM_ALL
    source/nanostack_eventloop.c
    test/nanostack_eventloop/nanostack_eventloop_test.cpp
    test/stubs/hal_stubs.c
    test/stubs/event_stub.c
    test/stubs/system_timer_stub.c
)
# make check, this must be after add_executable!
add_test(nanostack_eventloop_test2 nanostack_eventloop_test2)
if (TARGET check)
    add_dependencies(check nanostack_eventloop_test2)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS nanostack_eventloop_test2)
endif()

target_compile_options(nanostack_eventloop_test2 PRIVATE -DNANOSTACK_USE_PAL_FUNCTION_POINTERS)

target_include_directories(nanostack_eventloop_test2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop)
target_include_directories(nanostack_eventloop_test2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/platform)
target_include_directories(nanostack_eventloop_test2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_include_directories(nanostack_eventloop_test2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/stubs)

target_link_libraries(
    nanostack_eventloop_test2
    gtest_main
    nanostack-libserviceInterface
)

add_executable(event_test EXCLUDE_FROM_ALL
    source/event.c
    test/event/event_test.cpp
    test/stubs/hal_stubs.c
    test/stubs/system_timer_stub.c
    test/stubs/nanostack_eventloop_stub.c
    test/stubs/event_os_stub.c
    test/stubs/ns_timer_stub.c
    test/stubs/nsdynmemLIB_stub.c
    test/stubs/ns_list_stub.c
)
# make check, this must be after add_executable!
add_test(event_test event_test)
if (TARGET check)
    add_dependencies(check event_test)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS event_test)
endif()

target_include_directories(event_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop)
target_include_directories(event_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/platform)
target_include_directories(event_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_include_directories(event_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/stubs)

target_link_libraries(
    event_test
    gtest_main
    nanostack-libserviceInterface
)

add_executable(ns_timeout_test EXCLUDE_FROM_ALL
    source/ns_timeout.c
    test/ns_timeout/ns_timeout_test.cpp
    test/stubs/hal_stubs.c
    test/stubs/event_stub.c
    test/stubs/system_timer_stub.c
    test/stubs/nsdynmemLIB_stub.c
)
# make check, this must be after add_executable!
add_test(ns_timeout_test ns_timeout_test)
if (TARGET check)
    add_dependencies(check ns_timeout_test)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS ns_timeout_test)
endif()

target_include_directories(ns_timeout_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop)
target_include_directories(ns_timeout_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/platform)
target_include_directories(ns_timeout_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_include_directories(ns_timeout_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/stubs)

target_link_libraries(
    ns_timeout_test
    gtest_main
    nanostack-libserviceInterface
)

add_executable(system_timer_test EXCLUDE_FROM_ALL
    source/system_timer.c
    test/system_timer/system_timer_test.cpp
    test/stubs/hal_stubs.c
    test/stubs/event_stub.c
    test/stubs/ns_timer_stub.c
    test/stubs/nanostack_eventloop_stub.c
    test/stubs/ns_list_stub.c
    test/stubs/nsdynmemLIB_stub.c
)
# make check, this must be after add_executable!
add_test(system_timer_test system_timer_test)
if (TARGET check)
    add_dependencies(check system_timer_test)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
                  DEPENDS system_timer_test)
endif()

target_include_directories(system_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop)
target_include_directories(system_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/platform)
target_include_directories(system_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)
target_include_directories(system_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/stubs)
target_include_directories(ns_timer_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
    system_timer_test
    gtest_main
    nanostack-libserviceInterface
)

# GTest framework requires C++ version 11
set_target_properties(ns_timer_test nanostack_eventloop_test nanostack_eventloop_test2 event_test ns_timeout_test system_timer_test
PROPERTIES
    CXX_STANDARD 11
)

include(GoogleTest)
gtest_discover_tests(ns_timer_test EXTRA_ARGS --gtest_output=xml: XML_OUTPUT_DIR nstimer_xml)
gtest_discover_tests(nanostack_eventloop_test EXTRA_ARGS --gtest_output=xml: XML_OUTPUT_DIR nanostack_eventloop_xml)
gtest_discover_tests(nanostack_eventloop_test2 EXTRA_ARGS --gtest_output=xml: XML_OUTPUT_DIR nanostack_eventloop2_xml)
gtest_discover_tests(event_test EXTRA_ARGS --gtest_output=xml: XML_OUTPUT_DIR event_xml)
gtest_discover_tests(ns_timeout_test EXTRA_ARGS --gtest_output=xml: XML_OUTPUT_DIR ns_timeout_xml)
gtest_discover_tests(system_timer_test EXTRA_ARGS --gtest_output=xml: XML_OUTPUT_DIR system_timer_xml)

if (enable_coverage_data AND ${CMAKE_PROJECT_NAME} STREQUAL "nanostackeventloop")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html")

    add_test(NAME coap_cov WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${BASH} -c "gcovr -r . -e ${CMAKE_CURRENT_SOURCE_DIR}/build -e '.*test.*' --html --html-details -o build/html/coverage_index.html"
    )
endif ()
endif() # (NOT DEFINED OS_BRAND)
