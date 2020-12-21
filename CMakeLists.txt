cmake_minimum_required(VERSION 3.6)

project(imac3_dg)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/cmake)

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

# eigen
set(EIGEN_INCLUDE_DIRS "third_party/eigen")

set(${PROJECT_NAME}_LIBRARIES
       DGtal
        )

set(${PROJECT_NAME}_UTIL_HEADERS
        ${${PROJECT_NAME}_INCLUDE_DIR}/util/common.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/util/eigen.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/util/DigitalComponent.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/util/CompositeDigitalObject.hpp

        )

set(${PROJECT_NAME}_INCLUDE_DIRS
        ${${PROJECT_NAME}_INCLUDE_DIR}
        ${${PROJECT_NAME}}_UTIL_HEADERS
        ${DGTAL_INCLUDE_DIRS}
        ${EIGEN_INCLUDE_DIRS}
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

set(${PROJECT_NAME}_TD2_FILES
        ## source
        ${${PROJECT_NAME}_SOURCE_DIR}/td2/main.cpp
        )

set(${PROJECT_NAME}_TD3_FILES
        ## source
        ${${PROJECT_NAME}_SOURCE_DIR}/td3/main.cpp
        )

add_executable(${PROJECT_NAME}_td1 ${${PROJECT_NAME}_TD1_FILES})
add_executable(${PROJECT_NAME}_td2 ${${PROJECT_NAME}_TD2_FILES})
add_executable(${PROJECT_NAME}_td3 ${${PROJECT_NAME}_TD3_FILES})

target_link_libraries(${PROJECT_NAME}_td1 ${${PROJECT_NAME}_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_td2 ${${PROJECT_NAME}_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_td3 ${${PROJECT_NAME}_LIBRARIES})

