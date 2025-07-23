cmake_minimum_required(VERSION 3.22)
project(Colormap)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(external/crow/include)

include_directories(header)

add_executable(Colormap
    main.cpp
    header/ColorMap.hpp
    header/Color.hpp
    header/ImageHandler.hpp
    header/Server.hpp
    src/ColorMap.cpp
    src/Color.cpp
    src/ImageHandler.cpp
    src/Server.cpp
)

target_link_libraries(Colormap
    ${OpenCV_LIBS}
    nlohmann_json::nlohmann_json
)
