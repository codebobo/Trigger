cmake_minimum_required (VERSION 2.6)
project (AlphaNet)
SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fpermissive -g -ggdb -fPIC")

include_directories(${PROJECT_SOURCE_DIR}/src/tcp/inc 
					${PROJECT_SOURCE_DIR}/src/http/framework/inc
                    ${PROJECT_SOURCE_DIR}/src/http/filters/inc
                    ${PROJECT_SOURCE_DIR}/src/http/controllers/inc
                    ${PROJECT_SOURCE_DIR}/src/http/views/inc                    
					${PROJECT_SOURCE_DIR}/src/utils/inc)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/tcp/src DIR_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/utils/src DIR_SRCS)
add_library(alphanet ${DIR_SRCS})

aux_source_directory(${PROJECT_SOURCE_DIR}/src/test/tcpserver TCP_SRCS)
add_executable(tcpserver ${TCP_SRCS})
target_link_libraries(tcpserver alphanet event log4cplus libuuid.so pthread config++ crypto)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/http/framework/src HTTP_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/http/filters/src HTTP_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/http/views/src HTTP_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/http/controllers/src HTTP_SRCS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/test/httpserver HTTP_SRCS)
add_executable(httpserver ${HTTP_SRCS})
target_link_libraries(httpserver alphanet event log4cplus libuuid.so pthread config++ crypto jsoncpp)

