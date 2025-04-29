file(GLOB TEST_SOURCES "Sources/*.cpp")

add_executable(base_test ${TEST_SOURCES})

# Линкуемся с исполняемым файлом Pascalizer_core
target_link_libraries(base_test PRIVATE
    Pascalizer_core
    GTest::gtest_main
)

# Добавляем определение для импорта символов на Windows
target_compile_definitions(base_test PRIVATE
    $<$<PLATFORM_ID:Windows>:PASCALIZER_CORE_EXPORT=__declspec(dllimport)>
)

target_include_directories(base_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Pascalizer_core/Headers
)

include(GoogleTest)
gtest_discover_tests(base_test)