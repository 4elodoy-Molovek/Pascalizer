file(GLOB CORE_SOURCES "Sources/*.cpp" "Sources/*.ui" "Sources/*.qrc")
file(GLOB CORE_HEADERS "Headers/*.h")

# Создаем исполняемый файл с экспортом символов
add_executable(Pascalizer_core
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

# Включаем экспорт символов для Windows
set_target_properties(Pascalizer_core PROPERTIES
    ENABLE_EXPORTS ON
    WIN32_EXECUTABLE ON
)

# Настройки для экспорта символов
target_compile_definitions(Pascalizer_core PRIVATE
    $<$<BOOL:${CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS}>:PASCALIZER_CORE_EXPORT=__declspec(dllexport)>
    $<$<NOT:$<BOOL:${CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS}>>:PASCALIZER_CORE_EXPORT=>
)

target_link_libraries(Pascalizer_core PRIVATE
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(Pascalizer_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Headers"
)