# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(lr_two)

set(CMAKE_CXX_STANDARD 11)

# Добавим опцию для сборки с отладочной информацией
set(CMAKE_BUILD_TYPE Debug)

# Исходные файлы сервера и клиента
set(SERVER_SOURCE_FILES lr_two.cpp)
set(CLIENT_SOURCE_FILES client.cpp)

# Выходные файлы сервера и клиента
set(SERVER_OUTPUT_NAME server)
set(CLIENT_OUTPUT_NAME client)

# Пути для сохранения файлов
set(SERVER_SAVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/save")

# Добавим исполняемый файл сервера
add_executable(${SERVER_OUTPUT_NAME} ${SERVER_SOURCE_FILES})
target_include_directories(${SERVER_OUTPUT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(${SERVER_OUTPUT_NAME} PRIVATE -pthread)

# Добавим исполняемый файл клиента
add_executable(${CLIENT_OUTPUT_NAME} ${CLIENT_SOURCE_FILES})
target_include_directories(${CLIENT_OUTPUT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Добавим настройки для сохранения файлов на сервере
target_compile_definitions(${SERVER_OUTPUT_NAME} PRIVATE SERVER_SAVE_PATH="${SERVER_SAVE_PATH}")

# Если требуется сборка с использованием потоков, добавим флаг -pthread
find_package(Threads REQUIRED)
target_link_libraries(${SERVER_OUTPUT_NAME} PRIVATE Threads::Threads)

# Установка правил для make install (если необходимо)
install(TARGETS ${SERVER_OUTPUT_NAME} ${CLIENT_OUTPUT_NAME} DESTINATION bin)
