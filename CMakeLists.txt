cmake_minimum_required(VERSION 3.21)

project(crypto VERSION 1.0.0 LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0)

file(GLOB_RECURSE CRYPTO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(crypto
    ${CRYPTO_SOURCES}
    third_party/strlib/strlib.cpp
)

target_compile_features(crypto PRIVATE cxx_std_17)
target_compile_options(crypto PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra>
)
target_include_directories(crypto PRIVATE
    src
    third_party/mydefines
    third_party/strlib
)
target_link_libraries(crypto PRIVATE PkgConfig::GTKMM)

if(MINGW)
    target_link_options(crypto PRIVATE -static-libgcc -static-libstdc++)
endif()

add_custom_target(run
    COMMAND $<TARGET_FILE:crypto>
    DEPENDS crypto
    USES_TERMINAL
)

if(WIN32)
    install(TARGETS crypto
        RUNTIME_DEPENDENCY_SET crypto_runtime_dependencies
        RUNTIME DESTINATION .
    )
    install(RUNTIME_DEPENDENCY_SET crypto_runtime_dependencies
        DESTINATION .
        PRE_EXCLUDE_REGEXES
            "api-ms-win-.*"
            "ext-ms-win-.*"
        POST_EXCLUDE_REGEXES
            ".*[\\\\/]Windows[\\\\/]System32[\\\\/].*"
    )
    set(CPACK_PACKAGE_FILE_NAME "crypto-windows")
else()
    install(TARGETS crypto RUNTIME DESTINATION .)
    set(CPACK_PACKAGE_FILE_NAME "crypto-${CMAKE_SYSTEM_NAME}")
endif()

install(FILES LICENSE DESTINATION .)

set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_PACKAGE_NAME "crypto")
set(CPACK_PACKAGE_VENDOR "Anton Curmanschii")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

include(CPack)
