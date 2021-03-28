cmake_minimum_required(VERSION 3.14)

project(crypto LANGUAGES CXX)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
# Now the variables GTKMM_INCLUDE_DIRS, GTKMM_LIBRARY_DIRS and GTKMM_LIBRARIES 
# contain what you expect 

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(
    crypto PUBLIC
    ${GTKMM_INCLUDE_DIRS} 
    third_party/mydefines
    third_party/strlib
    src
)

set(SOURCES

    src/main.cpp
    src/shared.cpp
    src/abc.cpp

    src/gui/bazeries.cpp 
    src/gui/common.cpp 
    src/gui/shift.cpp 
    src/gui/straddling.cpp 
    src/gui/vigenere.cpp 
    src/gui/window.cpp 

    src/algos/bazeries.cpp 
    src/algos/affine.cpp 
    src/algos/polybios.cpp 
    src/algos/shift.cpp 
    src/algos/straddling.cpp 
    src/algos/vigenere.cpp
    
    third_party/strlib/strlib.cpp)

add_executable(crypto ${SOURCES})
target_link_libraries(crypto ${GTKMM_LIBRARIES})