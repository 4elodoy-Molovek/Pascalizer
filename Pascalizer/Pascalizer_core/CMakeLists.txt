file(GLOB CORE_SOURCES 
    "Sources/*.cpp"
    "Sources/*.ui"
    "Sources/*.qrc"
)

file(GLOB CORE_HEADERS 
    "Headers/*.h"
)

# Исполняемый файл с экспортом символов
add_executable(Pascalizer_core
    ${CORE_SOURCES}
    ${CORE_HEADERS}
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
endif()