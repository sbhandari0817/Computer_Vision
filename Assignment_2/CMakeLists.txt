project (cv_annotation)
cmake_minimum_required(VERSION 3.15)

# configure OpenCV
find_package(OpenCV REQUIRED)

# create create individual projects
add_executable(cv_assignment2 cv_assignment2.cpp)
target_link_libraries(cv_assignment2 ${OpenCV_LIBS})


