cmake_minimum_required(VERSION 3.15)
project(MyOpenCVProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# find OpenCV (core modules we usually need)
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui imgcodecs videoio)

# add include path for our headers
include_directories(${PROJECT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})

# library from our source files
add_library(mycvlib
        src/image_utils.cpp
        src/processing.cpp
)

# public include dir for users of the lib
target_include_directories(mycvlib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# link OpenCV to the library
target_link_libraries(mycvlib PUBLIC ${OpenCV_LIBS})

# main executable
add_executable(app src/main.cpp)

# link to our library (and transitively OpenCV)
target_link_libraries(app PRIVATE mycvlib)

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")