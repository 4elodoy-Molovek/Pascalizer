cmake_minimum_required(VERSION 3.20)
project(Pascalizer LANGUAGES CXX)

# Настройка Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Общие настройки
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Подпроекты
add_subdirectory(Pascalizer_core)
add_subdirectory(gtest)
add_subdirectory(base_test)