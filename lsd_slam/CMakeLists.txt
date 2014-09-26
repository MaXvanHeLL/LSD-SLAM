cmake_minimum_required(VERSION 2.8.7)
project(lsd_slam)


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


