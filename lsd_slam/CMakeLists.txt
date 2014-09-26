cmake_minimum_required(VERSION 2.8.7)
project(lsd_slam)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH   ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})


# External libs ================================================================
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR} ${LSD_SLAM_DIR}/thirdparty/Sophus)

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


# Sources & headers ============================================================
set(modules depth_estimation global_mapping io_wrapper model tracking util)
set(lsd_slam_srcs)
foreach(module ${modules})
  file(GLOB ${module}_srcs ${CURRENT_CMAKE_SOURCE_DIR}/${module}/*.h
                           ${CURRENT_CMAKE_SOURCE_DIR}/${module}/*.h)
  include_directories(${CURRENT_CMAKE_SOURCE_DIR}/${module})
  list(APPEND lsd_slam_srcs ${${module}_src})
endforeach(module ${modules})

file(GLOB other_srcs ${CURRENT_CMAKE_SOURCE_DIR}/*.h
                         ${CURRENT_CMAKE_SOURCE_DIR}/*.h)
list(APPEND lsd_slam_srcs other_srcs)

## the library
set(lib_target lsd_slam)
add_library(${lib_target} ${lsd_slam_srcs})
target_link_libraries(${lib_target} ${ALL_LIBRARIES})
list(APPEND ALL_LIBRARIES ${lib_target})

## demos
set(demo_srcs live_slam_wrapper.cc slam_system.cc)
add_executable(main_on_images main_on_images.cc ${demo_srcs})
target_link_libraries(main_on_images ${ALL_LIBRARIES})


