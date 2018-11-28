cmake_minimum_required (VERSION 2.8.12)
project (ConfigIPCAM)
find_package(OpenCV REQUIRED)

include_directories(include)
add_definitions("-std=c++11")


add_executable(depth test.cpp)
target_link_libraries(depth ${OpenCV_LIBS} pthread)


add_executable(lenght lenght.cpp)
target_link_libraries(lenght ${OpenCV_LIBS} pthread)

add_executable(mouse mouse.cpp)
target_link_libraries(mouse ${OpenCV_LIBS} )
