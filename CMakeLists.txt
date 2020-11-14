cmake_minimum_required(VERSION 3.6)

project(imac3_td_dg)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/cmake)
set(PROJECT_NAME  imac2_td_dg)

include(SetProjectFlags)
include(SetProjectDirectories)


# apply Project flags and c++ standard defined in included cmake files.
set(CMAKE_CXX_STANDARD "${${PROJECT_NAME}_CXX_STANDARD}")
set(CMAKE_CXX_FLAGS    "${${PROJECT_NAME}_CXX_FLAGS}")

message(STATUS "C++ Standard: " ${CMAKE_CXX_STANDARD})
message(STATUS "C++ Flags:    " ${CMAKE_CXX_FLAGS})

find_package(DGtal REQUIRED)
message(STATUS "Found DGtal:")
message(STATUS "     Include directory: " ${DGTAL_INCLUDE_DIRS})

set(${PROJECT_NAME}_LIBRARIES
       DGtal
        )

set(${PROJECT_NAME}_INCLUDE_DIRS
        ${${PROJECT_NAME}_INCLUDE_DIR}
        ${DGTAL_INCLUDE_DIRS}
        )


include_directories(
        ${${PROJECT_NAME}_INCLUDE_DIRS}
)


set(${PROJECT_NAME}_TD1_FILES
        ## source
        ${${PROJECT_NAME}_SOURCE_DIR}/td1/main.cpp

        ## headers
        # ${${PROJECT_NAME}_INCLUDE_DIR}/...
        )

add_executable(${PROJECT_NAME}_TD1 ${${PROJECT_NAME}_TD1_FILES})

target_link_libraries(${PROJECT_NAME}_TD1 ${${PROJECT_NAME}_LIBRARIES})
# add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_FILES})

