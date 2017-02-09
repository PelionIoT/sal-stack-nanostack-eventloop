INCLUDE(CMakeForceCompiler)
# CROSS COMPILER SETTING
cmake_minimum_required (VERSION 2.8)
SET(CMAKE_SYSTEM_NAME Generic)

project(salNanoStackLibServic)



include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/nanostack-event-loop/platform/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/libNsdl/src/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/libCoap/src/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/libCoap/src/include/)

set(LINUX_PORT_LIBS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../Linux_port_libs/)

message(STATUS ************22222*****${LINUX_PORT_LIBS_PATH})

include_directories(${LINUX_PORT_LIBS_PATH}/)
include_directories(${LINUX_PORT_LIBS_PATH}/mbed-client-c/)
include_directories(${LINUX_PORT_LIBS_PATH}/mbed-client-c/nsdl-c/)
include_directories(${LINUX_PORT_LIBS_PATH}/mbed-trace/)
include_directories(${LINUX_PORT_LIBS_PATH}/nanostack-libservice/)
include_directories(${LINUX_PORT_LIBS_PATH}/nanostack-libservice/mbed-client-libservice/)



FILE(GLOB SAL_NANO_STACK_LIB_SERVICE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/source/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"

	)

CREATE_LIBRARY(salNanoStackLibService "${SAL_NANO_STACK_LIB_SERVICE_SRC}" "") 


