cmake_minimum_required(VERSION 3.22)
project(Colormap)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Colormap
    main.cpp
        header/ColorMap.hpp
        header/Color.hpp
        header/ImageHandler.hpp
        
        src/ColorMap.cpp
        src/Color.cpp
        src/ImageHandler.cpp)

target_link_libraries(Colormap ${OpenCV_LIBS})
