CMAKE_MINIMUM_REQUIRED(VERSION 3.18.4)

project(Track_Detector)

#Set according to the number of cpu cores in your machine
set(NCPU 4)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_CNPY "Enable building cnpy" ON)
if (BUILD_CNPY)
    add_subdirectory("submodules/cnpy")
endif()

option(BUILD_PROJECT "Enable building track detector" ON)
if (BUILD_PROJECT)
    add_subdirectory("src")
endif()
