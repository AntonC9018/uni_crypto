cmake_minimum_required(VERSION 3.21)

project(crypto VERSION 1.0.0 LANGUAGES CXX)

if(POLICY CMP0207)
    cmake_policy(SET CMP0207 NEW)
endif()

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
    get_filename_component(MINGW_RUNTIME_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(MINGW_PREFIX_DIR "${MINGW_RUNTIME_DIR}" DIRECTORY)
    set(GDK_PIXBUF_RUNTIME_DIR
        "${MINGW_PREFIX_DIR}/lib/gdk-pixbuf-2.0/2.10.0"
    )

    if(NOT EXISTS "${GDK_PIXBUF_RUNTIME_DIR}/loaders.cache")
        message(FATAL_ERROR
            "Could not find the MinGW GDK-Pixbuf runtime data under "
            "${GDK_PIXBUF_RUNTIME_DIR}"
        )
    endif()

    install(TARGETS crypto
        RUNTIME_DEPENDENCY_SET crypto_runtime_dependencies
        RUNTIME DESTINATION .
    )

    file(GLOB GDK_PIXBUF_LOADERS
        "${GDK_PIXBUF_RUNTIME_DIR}/loaders/*.dll"
    )
    foreach(GDK_PIXBUF_LOADER IN LISTS GDK_PIXBUF_LOADERS)
        get_filename_component(GDK_PIXBUF_LOADER_NAME
            "${GDK_PIXBUF_LOADER}" NAME_WE
        )
        string(MAKE_C_IDENTIFIER
            "gdk_pixbuf_${GDK_PIXBUF_LOADER_NAME}"
            GDK_PIXBUF_LOADER_TARGET
        )
        add_library("${GDK_PIXBUF_LOADER_TARGET}" MODULE IMPORTED)
        set_target_properties("${GDK_PIXBUF_LOADER_TARGET}" PROPERTIES
            IMPORTED_LOCATION "${GDK_PIXBUF_LOADER}"
        )
        install(IMPORTED_RUNTIME_ARTIFACTS "${GDK_PIXBUF_LOADER_TARGET}"
            RUNTIME_DEPENDENCY_SET crypto_runtime_dependencies
            LIBRARY DESTINATION lib/gdk-pixbuf-2.0/2.10.0/loaders
            RUNTIME DESTINATION lib/gdk-pixbuf-2.0/2.10.0/loaders
        )
    endforeach()

    install(RUNTIME_DEPENDENCY_SET crypto_runtime_dependencies
        DESTINATION .
        DIRECTORIES "${MINGW_RUNTIME_DIR}"
        PRE_EXCLUDE_REGEXES
            "^api-ms-.*"
            "^ext-ms-.*"
        POST_EXCLUDE_REGEXES
            ".*[\\\\/][Ww]indows[\\\\/][Ss]ystem32[\\\\/].*"
    )

    install(FILES "${GDK_PIXBUF_RUNTIME_DIR}/loaders.cache"
        DESTINATION lib/gdk-pixbuf-2.0/2.10.0
    )
    install(DIRECTORY
        "${MINGW_PREFIX_DIR}/share/glib-2.0/schemas"
        DESTINATION share/glib-2.0
    )
    install(DIRECTORY
        "${MINGW_PREFIX_DIR}/share/icons/Adwaita"
        "${MINGW_PREFIX_DIR}/share/icons/hicolor"
        DESTINATION share/icons
    )
    install(DIRECTORY "${MINGW_PREFIX_DIR}/share/themes/"
        DESTINATION share/themes
    )
    install(DIRECTORY "${MINGW_PREFIX_DIR}/share/mime"
        DESTINATION share
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
