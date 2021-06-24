project (cv_hw_1)
cmake_minimum_required(VERSION 2.8)

# configure OpenCV
find_package(OpenCV REQUIRED)

# create create individual projects
add_executable(cv_hw_1 cv_hw_1.cpp)
target_link_libraries(cv_hw_1 ${OpenCV_LIBS})



