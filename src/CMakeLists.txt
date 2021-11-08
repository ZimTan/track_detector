CMAKE_MINIMUM_REQUIRED(VERSION 3.18.4)

project(Track_Detector)

add_executable(track_detector track_detector.cpp)
find_package(Threads)
target_include_directories(track_detector PRIVATE ${CMAKE_BINARY_DIR}/submodules/cnpy)
target_link_libraries(track_detector ${CMAKE_BINARY_DIR}/lib/libcnpy.so ${CMAKE_THREAD_LIBS_INIT})
