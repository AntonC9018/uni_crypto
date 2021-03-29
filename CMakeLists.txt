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
    src/shared/util.cpp
    src/shared/abc.cpp

    src/classic/gui/bazeries.cpp 
    src/classic/gui/common.cpp 
    src/classic/gui/shift.cpp 
    src/classic/gui/straddling.cpp 
    src/classic/gui/vigenere.cpp 
    src/classic/gui/window.cpp 

    src/classic/algos/bazeries.cpp 
    src/classic/algos/affine.cpp 
    src/classic/algos/polybios.cpp 
    src/classic/algos/shift.cpp 
    src/classic/algos/straddling.cpp 
    src/classic/algos/vigenere.cpp
    
    third_party/strlib/strlib.cpp)

add_executable(crypto ${SOURCES})
target_link_libraries(crypto ${GTKMM_LIBRARIES})