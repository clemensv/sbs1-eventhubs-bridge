cmake_minimum_required(VERSION 2.6)

# create_source_group(relativeSourcePath sourceGroupName files)
#
# Creates a source group with the specified name relative to the relative path
# specified.
#
# Parameters:
#    - sourceGroupName: Name of the source group to create.
#    - relativeSourcePath: Relative path to the files.
#    - sourceFiles: Files to add to the source group.
#
# For example if you have the following directory structure:
#
#    - ExampleApplication
#        - include
#            - Main.h
#                - Window
#                    Window.h
#        - source
#            - Main.cpp
#                - Window
#                    Window.cpp
#
# You can get your list of files and call create_source_group the following way
#
#    file(GLOB_RECURSE my_source_files ${CMAKE_CURRENT_SOURCE_DIR}/source/*)
#    create_source_group("Source Files" "${CMAKE_CURRENT_SOURCE_DIR}/source" ${my_source_files})
#    file(GLOB_RECURSE my_header_files ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
#    create_source_group("Header Files" "${CMAKE_CURRENT_SOURCE_DIR}/include" ${my_header_files})
#    add_executable(ExampleApplication ${my_source_files} ${my_header_files})
#
# Then the generated solution would look like this
#
#    - ExampleApplication (project)
#        - Header Files
#            - Main.h
#                - Window
#                    Window.h
#        - Source Files
#            - Main.cpp
#                - Window
#                    Window.cpp
#
function(create_source_group sourceGroupName relativeSourcePath)
    FOREACH(currentSourceFile ${ARGN})
        FILE(RELATIVE_PATH folder ${relativeSourcePath} ${currentSourceFile})
        get_filename_component(filename ${folder} NAME)
        string(REPLACE ${filename} "" folder ${folder})
        if(NOT folder STREQUAL "")
            string(REGEX REPLACE "/+$" "" folderlast ${folder})
            string(REPLACE "/" "\\" folderlast ${folderlast})
            SOURCE_GROUP("${sourceGroupName}\\${folderlast}" FILES ${currentSourceFile})
        endif(NOT folder STREQUAL "")
    ENDFOREACH(currentSourceFile ${ARGN})
endfunction(create_source_group)

# ----------------------------------------
# SBS-1 Eventhubs Bridge
# ----------------------------------------
project(sbs1-eventhubs-bridge C)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#include(version)
#include(compiler_options)
#include(dependencies)

find_package(EVENTHUB_CLIENT REQUIRED)
if(EVENTHUB_CLIENT_FOUND)
    include_directories(${EVENTHUB_CLIENT_INCLUDE_DIR})
    list(APPEND ${PROJECT_NAME}_EXTERNAL_LIBRARIES "${EVENTHUB_CLIENT_LIBRARY}")
endif()

find_package(EVENTHUB_COMMON REQUIRED)
if(EVENTHUB_COMMON_FOUND)
    include_directories(${EVENTHUB_COMMON_INCLUDE_DIR})
    list(APPEND ${PROJECT_NAME}_EXTERNAL_LIBRARIES "${EVENTHUB_COMMON_LIBRARY}")
endif()

find_package(QPID_PROTON REQUIRED)
if(QPID_PROTON_FOUND)
    include_directories(${QPID_PROTON_INCLUDE_DIR})
    list(APPEND ${PROJECT_NAME}_EXTERNAL_LIBRARIES "${QPID_PROTON_LIBRARY}")
endif()

find_package(Threads REQUIRED)


set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_BUILD_TYPE Debug)

add_definitions(-std=gnu99)
add_definitions(-pedantic)
add_definitions(-Wall)
add_definitions(-Wextra)
add_definitions(-Wcast-align)
add_definitions(-w)

file(GLOB_RECURSE BRIDGE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bridge/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bridge/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bridge/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bridge/*.cpp)

list(SORT BRIDGE_SOURCES)
create_source_group("Source Files" "${CMAKE_CURRENT_SOURCE_DIR}/src" ${BRIDGE_SOURCES})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_INSTALL_PREFIX}/include)
include_directories(${CMAKE_INSTALL_PREFIX}/include/eventhubs)

add_executable(sbs1-eventhub-bridge ${BRIDGE_SOURCES})

target_link_libraries (sbs1-eventhub-bridge 
     ${CMAKE_THREAD_LIBS_INIT}
     ${${PROJECT_NAME}_EXTERNAL_LIBRARIES})

GET_PROPERTY(bridge_location TARGET sbs1-eventhub-bridge PROPERTY LOCATION)

