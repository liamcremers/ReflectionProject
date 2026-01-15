cmake_minimum_required(VERSION 3.28)
project(Reflection CXX)

# Ensure compile_commands.json is generated for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include your clang-toolchain.cmake contents here
include(${CMAKE_SOURCE_DIR}/clang-toolchain.cmake)

# Fetch nlohmann_json before defining targets so include paths are known
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# Your executable setup
add_executable(${PROJECT_NAME} main.cpp)

# Link nlohmann_json
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Explicitly add nlohmann include dir for IDEs (helps clangd)
# FetchContent provides nlohmann_json in build/_deps/nlohmann_json-src/include
target_include_directories(${PROJECT_NAME} PRIVATE
    "${nlohmann_json_SOURCE_DIR}/include"
)

# libc++ and Clang resource dirs (reflection build)
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

set(rpath_dir "${CMAKE_SOURCE_DIR}/p2996-install/lib/x86_64-unknown-linux-gnu")

set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${rpath_dir}"
)

set(watch_sources input.json)
# Mark as GENERATED so it need not exist at configure time
set_source_files_properties(${watch_sources} PROPERTIES GENERATED TRUE)
add_custom_target(watch_input SOURCES ${watch_sources})
add_dependencies(${PROJECT_NAME} watch_input)