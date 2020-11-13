cmake_minimum_required(VERSION 3.6)

project(imac3_td_dg)

set(${PROJECT_NAME}_CXX_STANDARD 20)

set(IS_GNU OFF)
set(IS_MSVC OFF)
set(IS_CLANG OFF)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(IS_GNU ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(IS_MSVC ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(IS_CLANG ON)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL 'Release')
    set(${PROJECT_NAME}_OPTIMISATION ON)
else ()
    set(${PROJECT_NAME}_OPTIMISATION OFF)
endif ()


if (${IS_GNU} OR ${IS_CLANG})
    if (${${PROJECT_NAME}_OPTIMISATION})
        set(${PROJECT_NAME}_OPTIMISATION_LEVEL "-O3")
    else ()
        set(${PROJECT_NAME}_OPTIMISATION_LEVEL "")
    endif ()
    ## -Warray-bounds is clang-only I think?
    set(${PROJECT_NAME}_CXX_FLAGS
            "-fPIC \
                -Werror \
                -Wall -pedantic \
                -Wextra -Wshadow -Wnon-virtual-dtor  -Warray-bounds \
                -Wold-style-cast \
                -Wimplicit-fallthrough=5 -Wconversion \
                ${${PROJECT_NAME}_OPTIMISATION_LEVEL} \
                "
            )
elseif (${IS_MSVC})
    if (${${PROJECT_NAME}_OPTIMISATION})
        set(${PROJECT_NAME}_OPTIMISATION_LEVEL "/O2")
    else ()
        set(${PROJECT_NAME}_OPTIMISATION_LEVEL "")
    endif ()
    set(${PROJECT_NAME}_CXX_FLAGS
            " \
                /EHsc \
                /WX \
                /W4 /w14640 /permissive \
                /w14242 /w14254 /w14263 /w14265 /w14287 \
                ${${PROJECT_NAME}_OPTIMISATION_LEVEL} \
                "
            )
else ()
    message(ERROR "Nope!")
endif ()

set(CMAKE_CXX_FLAGS ${${PROJECT_NAME}_MAIN_CXX_FLAGS})

set(${PROJECT_NAME}_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(${PROJECT_NAME}_INLINE_DIR ${PROJECT_SOURCE_DIR}/inline)

set(${PROJECT_NAME}_INCLUDE_DIRS
        ${CANTINA_COMMON_INCLUDE_DIR}
        )

find_library(DGtal REQUIRED)

include_directories(
        ${${PROJECT_NAME}_INCLUDE_DIRS}
)


set(${PROJECT_NAME}_TD1_FILES
        ## headers
        # ${${PROJECT_NAME}_SOURCE_DIR}/...

        #source
        ${${PROJECT_NAME}_INCLUDE_DIR}/td1/main.cpp
        )

add_executable(${PROJECT_NAME}_TD1 ${${PROJECT_NAME}_TD1_FILES})

target_link_libraries(${PROJECT_NAME}_TD1 DGTal)
# add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_FILES})

