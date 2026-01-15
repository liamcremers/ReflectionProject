cmake_minimum_required(VERSION 3.28)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/p2996-install")
    message(STATUS "Downloading P2996 clang toolchain...")
    file(DOWNLOAD "https://download.liamcremers.com/p2996-install.zip" "${CMAKE_SOURCE_DIR}/p2996-install.zip" SHOW_PROGRESS)

    message(STATUS "Extracting P2996 clang toolchain...")
    file(ARCHIVE_EXTRACT INPUT "${CMAKE_SOURCE_DIR}/p2996-install.zip" DESTINATION "${CMAKE_SOURCE_DIR}")

    file(GLOB P2996_BINARIES "${CMAKE_SOURCE_DIR}/p2996-install/bin/*")
    foreach(binary ${P2996_BINARIES})
        if(NOT IS_DIRECTORY "${binary}")
            execute_process(COMMAND chmod +x "${binary}")
        endif()
    endforeach()

    file(REMOVE "${CMAKE_SOURCE_DIR}/p2996-install.zip")
    message(STATUS "P2996 toolchain ready")
endif()

include(${CMAKE_SOURCE_DIR}/clang-toolchain.cmake)
project(Reflection CXX)

# Ensure compile_commands.json is generated for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(${PROJECT_NAME} PRIVATE
    "${nlohmann_json_SOURCE_DIR}/include"
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    "${CMAKE_SOURCE_DIR}/p2996-install/include/c++/v1"
    "${CMAKE_SOURCE_DIR}/p2996-install/include/x86_64-unknown-linux-gnu/c++/v1"
    "${CMAKE_SOURCE_DIR}/p2996-install/lib/clang/21/include"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -std=c++26
    -freflection-latest
    -fexpansion-statements
    -stdlib=libc++
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${CMAKE_SOURCE_DIR}/p2996-install/lib/x86_64-unknown-linux-gnu"
)