cmake_minimum_required (VERSION 2.6)
project (EventServer)
SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive -g -ggdb -fPIC")

include_directories(${PROJECT_SOURCE_DIR}/src/base/codec ${PROJECT_SOURCE_DIR}/src/Terminus ${CMAKE_BINARY_DIR}/src/base/protobuf_message)
include_directories(${PROJECT_SOURCE_DIR}/src/tcp/inc 
					${PROJECT_SOURCE_DIR}/src/http/inc 
					${PROJECT_SOURCE_DIR}/src/utils/inc)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/tcp/src DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils/src DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/http/src DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/test DIR_SRCS)

add_executable(eventserver ${DIR_SRCS})

target_link_libraries(eventserver event log4cplus libuuid.so pthread config++ crypto)

