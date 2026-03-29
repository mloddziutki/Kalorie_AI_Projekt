cmake_minimum_required(VERSION 3.16)
project(CalorieAI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(WIN32)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/mingw_64") 
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(CalorieAI ${SOURCES})

target_link_libraries(CalorieAI PRIVATE Qt6::Widgets)

if(WIN32)
    target_link_libraries(CalorieAI PRIVATE ws2_32 crypt32)
endif()