file(GLOB TEST_SOURCES 
    "Sources/*.cpp"
)

add_executable(base_test ${TEST_SOURCES})

# Подключение зависимостей
target_link_libraries(base_test PRIVATE
    Pascalizer_core_lib
    GTest::gtest_main
)

# Настройки для импорта символов (Windows)
if(WIN32)
    target_compile_definitions(base_test PRIVATE
        PASCALIZER_CORE_BUILD
    )
endif()

# Относительные пути к заголовкам
target_include_directories(base_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Pascalizer_core/Headers
)

include(GoogleTest)
gtest_discover_tests(base_test)