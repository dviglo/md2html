cmake_minimum_required(VERSION 3.15.5)


# Название решения (solution)
set(SOLUTION_NAME Solution)
project(${SOLUTION_NAME})


# Имя проекта и исполняемого файла (переменная PROJECT_NAME уже занята)
set(PROJ_NAME md2html)


# Оставляем только две конфигурации
set(CMAKE_CONFIGURATION_TYPES Debug Release)


# Создаём проекты libcmark-gfm_static и libcmark-gfm-extensions_static
option(CMARK_TESTS "" OFF)
option(CMARK_STATIC "" ON)
option(CMARK_SHARED "" OFF)
option(CMARK_LIB_FUZZER "" OFF)
add_subdirectory(ThirdParty/cmark-gfm)


# Куда будет помещён скомпилированный исполняемый файл
set(OUT_DIR ${CMAKE_BINARY_DIR}/Result)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUT_DIR})


# Создаём проект приложения
file(GLOB_RECURSE SRC_FILES "Application/*.h" "Application/*.cpp")
add_executable(${PROJ_NAME} ${SRC_FILES})


# Линкуем и инклудим cmark
target_link_libraries(${PROJ_NAME} libcmark-gfm-extensions_static libcmark-gfm_static)
include_directories(
    ${CMAKE_SOURCE_DIR}/ThirdParty/cmark-gfm/src
    ${CMAKE_BINARY_DIR}/ThirdParty/cmark-gfm/src
    ${CMAKE_BINARY_DIR}/ThirdParty/cmark-gfm/extensions
)
set_property(TARGET libcmark-gfm_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # Используем статический /MT
set_property(TARGET libcmark-gfm-extensions_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # / MT


# Назначаем проект приложения стартовым
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})


# Рабочая папка при отладке
set_property(TARGET ${PROJ_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${OUT_DIR})


# Запрещаем Студии помещать *.cpp и *.h в разные группы
source_group (TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${SRC_FILES})


# Активируем стандарт C++17
set_property(TARGET ${PROJ_NAME} PROPERTY CXX_STANDARD 17)
