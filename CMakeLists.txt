cmake_minimum_required(VERSION 2.8.7)
project(lsd_slam_and_apps)

set(LSD_SLAM_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH   ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})


# External libs ================================================================
include_directories(${LSD_SLAM_DIR}/thirdparty/eigen-3.2.1)
include_directories(${LSD_SLAM_DIR}/thirdparty/Sophus)

# SuiteParse
find_package(SuiteParse REQUIRED) # Apparently needed by g2o
include_directories(
  ${CSPARSE_INCLUDE_DIR} #Has been set by SuiteParse
  ${CHOLMOD_INCLUDE_DIR} #Has been set by SuiteParse
  )

# G2O
find_package(G2O REQUIRED)
include_directories(${G2O_INCLUDE_DIR})


# FabMap
# uncomment this part to enable fabmap
#add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/openFabMap)
#include_directories(${PROJECT_SOURCE_DIR}/thirdparty/openFabMap/include)
#add_definitions("-DHAVE_FABMAP")
#set(FABMAP_LIB openFABMAP )

set(ALL_LIBRARIES ${G2O_LIBRARIES} ${FABMAP_LIB})

if(ANDROID)
  add_definitions(-DENABLE_NEON)
else()
  add_definitions(-DENABLE_SSE)
  set(CMAKE_CXX_RELEASE_FLAGS ${CMAKE_CXX_RELEASE_FLAGS} ${SSE_FLAGS})
endif()

add_subdirectory(thirdparty)

add_subdirectory(lsd_slam)

add_subdirectory(apps)
