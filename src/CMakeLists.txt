CMAKE_MINIMUM_REQUIRED(VERSION 3.18.4)

project(Track_Detector)

add_executable(track_detector track_detector.cpp)

target_link_libraries(track_detector ${CMAKE_BINARY_DIR}/lib/libcnpy.so)
