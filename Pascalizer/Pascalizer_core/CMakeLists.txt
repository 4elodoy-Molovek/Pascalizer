file(GLOB CORE_SOURCES 
    "Sources/*.cpp"
)

file(GLOB UI_FILES 
    "UIFiles/*.ui"
)

file(GLOB QRC_FILES 
    "UIFiles/*.qrc"
)

file(GLOB CORE_HEADERS 
    "Headers/*.h"
)

# Исполняемый файл с экспортом символов
add_executable(Pascalizer_core
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${UI_FILES}
    ${QRC_FILES}
)

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

# Подключение Qt
target_link_libraries(Pascalizer_core PRIVATE
    Qt6::Core
    Qt6::Widgets
)

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
endif()