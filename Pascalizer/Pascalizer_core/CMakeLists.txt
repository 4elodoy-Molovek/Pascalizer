cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/UIFiles")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

file(GLOB CORE_HEADERS 
    "Headers/*.h"
)

file(GLOB CORE_SOURCES_NO_MAIN
    "Sources/*.cpp"
)
list(FILTER CORE_SOURCES_NO_MAIN EXCLUDE REGEX ".*main\\.cpp$")
list(FILTER CORE_SOURCES_NO_MAIN EXCLUDE REGEX ".*Pascalizer_MainWindow\\.cpp$")

file(GLOB UI_FILES 
    "UIFiles/*.ui"
)

file(GLOB QRC_FILES 
    "UIFiles/*.qrc"
)

add_library(Pascalizer_core_lib STATIC
    ${CORE_HEADERS}
    ${CORE_SOURCES_NO_MAIN}
)

target_include_directories(Pascalizer_core_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Headers"
)

target_link_libraries(Pascalizer_core_lib PUBLIC
    Qt6::Core
    Qt6::Widgets
)

# Исполняемый файл с экспортом символов
add_executable(Pascalizer_core
    ${UI_FILES}
    ${QRC_FILES}
    Sources/main.cpp
    Sources/Pascalizer_MainWindow.cpp
)

target_link_libraries(Pascalizer_core PRIVATE Pascalizer_core_lib)

# Настройки для Windows
if(WIN32)
    set_target_properties(Pascalizer_core PROPERTIES
        ENABLE_EXPORTS ON
        WIN32_EXECUTABLE ON
    )
    target_compile_definitions(Pascalizer_core PRIVATE
        PASCALIZER_CORE_BUILD
    )
endif()

# Относительные пути к заголовкам
target_include_directories(Pascalizer_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Headers"
)

# Автоматическое копирование Qt DLL (Windows)
if(WIN32)
    add_custom_command(TARGET Pascalizer_core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_RUNTIME_DLLS:Pascalizer_core>"
        "$<TARGET_FILE_DIR:Pascalizer_core>"
        COMMAND_EXPAND_LISTS
    )

    # NEW: Also copy platform plugin (qwindows.dll)
    set(QT_PLUGINS_DIR "${Qt6_DIR}/../../../plugins/platforms")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(QT_PLATFORM_PLUGIN "${QT_PLUGINS_DIR}/qwindowsd.dll")
    else()
        set(QT_PLATFORM_PLUGIN "${QT_PLUGINS_DIR}/qwindows.dll")
    endif()

    add_custom_command(TARGET Pascalizer_core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:Pascalizer_core>/platforms"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_PLATFORM_PLUGIN}"
            "$<TARGET_FILE_DIR:Pascalizer_core>/platforms/"
    )

    # Получить путь к Qt бинарникам
    get_filename_component(QT_BIN_DIR "${Qt6_DIR}/../../../bin" ABSOLUTE)

    add_custom_target(deploy_qt
        COMMAND "${QT_BIN_DIR}/windeployqt.exe"
            --${CMAKE_BUILD_TYPE}
            "$<TARGET_FILE:Pascalizer_core>"
        COMMENT "Deploying Qt runtime with windeployqt..."
)

add_dependencies(deploy_qt Pascalizer_core)
endif()