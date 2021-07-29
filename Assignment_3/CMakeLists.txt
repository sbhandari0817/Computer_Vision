project (cv_background)
cmake_minimum_required(VERSION 3.15)

# configure OpenCV
find_package(OpenCV REQUIRED)

# create create individual projects
add_executable(cv_assignment3 cv_assignment3.cpp)
target_link_libraries(cv_assignment3 ${OpenCV_LIBS})

