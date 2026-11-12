cmake_minimum_required(VERSION 3.16)

project(libthing)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(libthing "${MY_SOURCES}")

add_custom_target(run
    COMMAND $<TARGET_FILE:libthing>
    DEPENDS libthing
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    USES_TERMINAL
    VERBATIM
)

target_compile_options(libthing PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
        -Wall
        -Wextra
        -Wno-unused-parameter
    >

    # MSVC equivalents
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4         # Comparable to -Wall -Wextra
        /wd4100     # Equivelant to -Wno-unused-parameter
    >
)

target_include_directories(libthing PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/include/")
