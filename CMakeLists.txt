cmake_minimum_required(VERSION 2.8)
project(wavtomp3 CXX)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/build/lib")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-deprecated")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# build lame from sources
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lame)

INCLUDE_DIRECTORIES(
        src
)

if(UNIX OR APPLE)
    add_executable(
            ${PROJECT_NAME}

            src/main.cpp
            src/nix/filesystem.cpp
            src/encoder.cpp
            src/wavfile.cpp
            src/mp3file.cpp
    )
endif()

if(WIN32)
    add_executable(
            ${PROJECT_NAME}

            src/main.cpp
            src/win/filesystem.cpp
            src/encoder.cpp
            src/wavfile.cpp
            src/mp3file.cpp
    )
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif()

target_link_libraries(
        ${PROJECT_NAME}

        mp3lame
        pthread
        )

install(TARGETS ${PROJECT_NAME} DESTINATION "bin")
