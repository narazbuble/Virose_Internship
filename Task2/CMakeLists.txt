cmake_minimum_required(VERSION 3.15)
# project(HelloCMake)
# project(BelajarJSON)

# set(CMAKE_CXX_STANDARD 17)

# add_executable(hello Bcm.cpp)
# add_executable(belajarJSON belajarJSON.cpp)


# Project Name
project("RobotConfig")
project("Task2Rev")

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(rapidjson_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include/")

# include_directories()
include_directories(${rapidjson_include_dir})

add_executable(test Test.cpp)
add_executable(MotionMovie Task2Rev.cpp)

# target_link_libraries()
