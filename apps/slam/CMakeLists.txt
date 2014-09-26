cmake_minimum_required(VERSION 2.8.7)
project(slam_app)

## demos
add_executable(main_on_images main_on_images.cc)
set_property(TARGET main_on_images PROPERTY FOLDER "lsd_slam/apps")
target_link_libraries(main_on_images ${ALL_LIBRARIES})

add_executable(main_live_odometry main_live_odometry.cc)
set_property(TARGET main_live_odometry PROPERTY FOLDER "lsd_slam/apps")
target_link_libraries(main_live_odometry ${ALL_LIBRARIES})


