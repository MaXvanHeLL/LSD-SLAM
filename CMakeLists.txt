cmake_minimum_required(VERSION 2.8.7)
project(lsd_slam_and_apps)

# common settings ==============================================================

# common settings
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(LSD_SLAM_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${LSD_SLAM_DIR} ${LSD_SLAM_DIR}/lsd_slam)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if(WIN32)
  add_definitions(-DNOMINMAX) # resolve conflicts of std::min()/std::max() on Windows MSVC
  add_definitions(-DWIN32_LEAN_AND_MEAN) # speed up compiler by omitting some headers
  add_definitions(-DM_SQRT1_2=0.707106781186547524401) # missing in MSVC2013
  add_definitions(-DM_PI=3.14159265358979323846) # missing in MSVC2013
  add_definitions("/MP")  # multiple processes compilation
endif(WIN32)

set(EXECUTABLE_OUTPUT_PATH ${LSD_SLAM_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${LSD_SLAM_DIR}/lib)
set(CMAKE_MODULE_PATH   ${LSD_SLAM_DIR}/cmake ${CMAKE_MODULE_PATH})


set(CMAKE_MODULE_PATH "${LSD_SLAM_DIR}/cmake")

# output paths settings ========================================================
# postfix, based on type
set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "postfix applied to debug build of libraries")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "postfix applied to release build of libraries")
set(CMAKE_RELWITHDEBINFO_POSTFIX "_rd" CACHE STRING "postfix applied to release-with-debug-information libraries")
set(CMAKE_MINSIZEREL_POSTFIX "_s" CACHE STRING "postfix applied to minimium-size-build libraries")

# work out the postfix; required where we use OUTPUT_NAME
if(CMAKE_BUILD_TYPE MATCHES Release)
  set(EXE_POSTFIX)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
  set(EXE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  set(EXE_POSTFIX ${CMAKE_RELWITHDEBINFO_POSTFIX})
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
  set(EXE_POSTFIX ${CMAKE_MINSIZEREL_POSTFIX})
endif(CMAKE_BUILD_TYPE MATCHES Release)
  
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
  string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_CURRENT_SOURCE_DIR}/bin")
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_CURRENT_SOURCE_DIR}/lib")
  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_CURRENT_SOURCE_DIR}/lib")
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


# External libs ================================================================
include_directories(${LSD_SLAM_DIR}/thirdparty/eigen-3.2.1)
include_directories(${LSD_SLAM_DIR}/thirdparty/Sophus)

## Boost
if(WIN32)
  set(Boost_USE_STATIC_LIBS  ON)
endif(WIN32)

find_package(Boost REQUIRED COMPONENTS thread system)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  set(Look3D_EXTERNAL_LIBS ${Look3D_EXTERNAL_LIBS} ${Boost_LIBRARIES})
  link_directories(${Boost_LIBRARY_DIR})
  message(STATUS "Boost libs: ${Boost_LIBRARIES}")
endif(Boost_FOUND)

##  OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
list(APPEND EXTERNAL_LIBRARIES ${OpenCV_LIBRARIES})

# # SuiteSparse
# find_package(SuiteSparse REQUIRED) # Apparently needed by g2o
# include_directories(
#   ${CSPARSE_INCLUDE_DIR} #Has been set by SuiteParse
#   ${CHOLMOD_INCLUDE_DIR} #Has been set by SuiteParse
#   )

# G2O
find_package(G2O REQUIRED)
set(G2O_BINARY_DIR ${G2O_ROOT}/bin)
set(G2O_LIBRARIES ${G2O_CORE_LIBRARY} ${G2O_STUFF_LIBRARY}
                  ${G2O_SOLVER_DENSE} ${G2O_TYPES_SLAM3D}
                  ${G2O_SIMULATOR}
                  )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(G2O_LIBRARIES ${G2O_CORE_LIBRARY_DEBUG} ${G2O_STUFF_LIBRARY_DEBUG}
                      ${G2O_SOLVER_DENSE_DEBUG} ${G2O_TYPES_SLAM3D_DEBUG}
                      ${G2O_SIMULATOR_DEBUG}
                      )
endif()
if(G2O_SOLVER_CSPARSE AND NOT APPLE)
  if(WIN32)
    list(APPEND G2O_INCLUDE_DIR "${G2O_ROOT}/include/EXTERNAL/csparse")
  else()
    list(APPEND G2O_INCLUDE_DIR "/usr/include/suitesparse")
  endif()
  list(APPEND G2O_LIBRARIES ${G2O_SOLVER_CSPARSE}
                            ${G2O_SOLVER_CSPARSE_EXTENSION}
                            ${G2O_EXT_CSPARSE})
  add_definitions(-DHAVE_SOLVER_CSPARSE)
endif()
if(G2O_SOLVER_CHOLMOD)
  list(APPEND G2O_LIBRARIES ${G2O_SOLVER_CHOLMOD})
  add_definitions(-DHAVE_SOLVER_CHOLMOD)
endif()
if(G2O_SOLVER_EIGEN)
  list(APPEND G2O_LIBRARIES ${G2O_SOLVER_EIGEN})
  add_definitions(-DHAVE_SOLVER_EIGEN)
endif()

if(APPLE)
  find_package(CSparse REQUIRED)
  list(APPEND G2O_LIBRARIES ${CSPARSE_LIBRARY})
  list(APPEND EXTRA_INC_DIRS ${CSPARSE_INCLUDE_DIR})
else()
  link_directories(${G2O_ROOT}/lib)
endif()
include_directories(${G2O_INCLUDE_DIR})
list(APPEND EXTERNAL_LIBRARIES ${G2O_LIBRARIES})

# FabMap
# uncomment this part to enable fabmap
#add_subdirectory(${LSD_SLAM_DIR}/thirdparty/openFabMap)
#include_directories(${LSD_SLAM_DIR}/thirdparty/openFabMap/include)
#add_definitions("-DHAVE_FABMAP")
#set(FABMAP_LIB openFABMAP )

# Direct API
include_directories("${LSD_SLAM_DIR}/thirdparty/dirent-1.20.1/include")

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
