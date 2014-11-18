cmake_minimum_required(VERSION 2.8.9)
set(LsdSlam_PROJECT_NAME lsd_slam_and_apps)
project(${LsdSlam_PROJECT_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)  # for organizing code to folders

# Look for our CMake modules paths
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake
                              ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
                              ${CMAKE_CURRENT_BINARY_DIR})
include(LsdSlamUtil)  # include useful macros
lsd_slam_print_status("Closure cmake modules: ${Closure_COLOR_BLUE}${CMAKE_MODULE_PATH}${Closure_COLOR_RESET}")

set(LsdSlam_WORLD_VERSION 1)
set(LsdSlam_MAJOR_VERSION 0)
set(LsdSlam_MINOR_VERSION 0)
lsd_slam_print_status("Welcome! Closure version ${Closure_COLOR_BLUE}${Closure_WORLD_VERSION}.${Closure_MAJOR_VERSION}.${Closure_MINOR_VERSION}${Closure_COLOR_RESET}")

# common settings
set(LsdSlam_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LsdSlam_LIB_PREFIX closure_)
set(LsdSlam_CXX_FLAGS)

add_definitions(-DLsdSlam_DIR="${Closure_ROOT_DIR}")

# common settings ==============================================================

# common settings
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(LSD_SLAM_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${LSD_SLAM_DIR} ${LSD_SLAM_DIR}/lsd_slam)

set(EXECUTABLE_OUTPUT_PATH ${LSD_SLAM_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${LSD_SLAM_DIR}/lib)
set(CMAKE_MODULE_PATH   ${LSD_SLAM_DIR}/cmake ${CMAKE_MODULE_PATH})

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

# External dependencies ========================================================
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/LsdSlamDependencies_Config.cmake")
  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LsdSlamDependencies_Config.cmake.in"
       DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
  file(RENAME "${CMAKE_CURRENT_BINARY_DIR}/LsdSlamDependencies_Config.cmake.in"
              "${CMAKE_CURRENT_BINARY_DIR}/LsdSlamDependencies_Config.cmake")
endif()
include(LsdSlamDependencies_Config)
include(LsdSlamDependencies)

# Thirdparty libs ==============================================================
include_directories(${LSD_SLAM_DIR}/thirdparty/eigen-3.2.1)
include_directories(${LSD_SLAM_DIR}/thirdparty/Sophus)

# FabMap
# uncomment this part to enable fabmap
#add_subdirectory(${LSD_SLAM_DIR}/thirdparty/openFabMap)
#include_directories(${LSD_SLAM_DIR}/thirdparty/openFabMap/include)
#add_definitions("-DHAVE_FABMAP")
#set(FABMAP_LIB openFABMAP )

# Direct API
include_directories("${LSD_SLAM_DIR}/thirdparty/dirent-1.20.1/include")

set(LsdSlam_ALL_LIBRARIES ${FABMAP_LIB} ${LsdSlam_EXTERNAL_LIBS})

if(ANDROID)
  add_definitions(-DENABLE_NEON)
else()
  add_definitions(-DENABLE_SSE)
  set(CMAKE_CXX_RELEASE_FLAGS ${CMAKE_CXX_RELEASE_FLAGS} ${SSE_FLAGS})
endif()

add_subdirectory(thirdparty)

# The Lsd Slam library =========================================================
add_subdirectory(lsd_slam)

# Applications
add_subdirectory(apps)

# Installation =================================================================
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/LsdSlamConfig.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/LsdSlamConfig.cmake"
               IMMEDIATE @ONLY)

# install FindClosure.cmake to common destination
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LsdSlamConfig.cmake
              ${CMAKE_CURRENT_SOURCE_DIR}/cmake/LsdSlamUtil.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

# Uninstallation ===============================================================
# uninstall target
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
               IMMEDIATE @ONLY)

add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)


# Useful paths configuration for windows 
if(WIN32)
  if(MSVC)
    if(CMAKE_CL_64)
      set(LsdSlam_ARCH x64)
    else()
      set(LsdSlam_ARCH x86)
    endif()
    if(MSVC_VERSION EQUAL 1400)
      set(LsdSlam_RUNTIME vc8)
    elseif(MSVC_VERSION EQUAL 1500)
      set(LsdSlam_RUNTIME vc9)
    elseif(MSVC_VERSION EQUAL 1600)
      set(LsdSlam_RUNTIME vc10)
      get_filename_component(VC_IDE_PATH $ENV{VS100COMNTOOLS}/../IDE ABSOLUTE)
    elseif(MSVC_VERSION EQUAL 1700)
      set(LsdSlam_RUNTIME vc11)
      get_filename_component(VC_IDE_PATH $ENV{VS110COMNTOOLS}/../IDE ABSOLUTE)
    elseif(MSVC_VERSION EQUAL 1800)
      set(LsdSlam_RUNTIME vc12)
      get_filename_component(VC_IDE_PATH $ENV{VS120COMNTOOLS}/../IDE ABSOLUTE)
    endif()
  endif()
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/set_paths.bat.in" 
                 "${CMAKE_CURRENT_BINARY_DIR}/set_paths.bat")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/set_paths.bat.in" 
                 "${Closure_RUNTIME_OUTPUT_DIRECTORY}/set_paths.bat")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/set_paths_and_run_vc.bat.in"
                 "${CMAKE_CURRENT_BINARY_DIR}/set_paths_and_run_vc.bat")
endif()
