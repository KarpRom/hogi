cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project("hogi")

###============= C++11 support====================================
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else ()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif ()

###============= Build type ======================================

set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)

###=============== Find Packages =================================

find_package(OpenCV REQUIRED)

###=============== Source files ==================================
file(GLOB project_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

###=============== Include files =================================
add_library(${PROJECT_NAME}
            ${project_sources}
            ${CMAKE_CURRENT_SOURCE_DIR}/include/*
            )

target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        ${Boost_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${OpenCV_LIBRARIES}
        )

add_executable(${PROJECT_NAME} src/hogi.cpp)
