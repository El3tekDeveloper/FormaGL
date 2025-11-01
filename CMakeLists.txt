cmake_minimum_required(VERSION 3.20)
project(App)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Froma
    ${PROJECT_SOURCE_DIR}/thirdparty
)

file(GLOB_RECURSE SOURCES
    Froma/**/*.cpp
    thirdparty/glad/glad.c
    thirdparty/miniz/miniz.c
    thirdparty/tinyexr/tinyexr.cc
    thirdparty/stb/stb.cpp
    thirdparty/imgui/*.cpp
    main.cpp
)

add_executable(App ${SOURCES})

target_link_libraries(App
    ${PROJECT_SOURCE_DIR}/lib/libglfw3dll.a
)

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_custom_command(TARGET App POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${PROJECT_SOURCE_DIR}/glfw3.dll 
        $<TARGET_FILE_DIR:App>/glfw3.dll
    COMMENT "Copying glfw3.dll..."
)

add_custom_command(TARGET App POST_BUILD
    COMMAND cmd /c "start \"App\" /wait $<TARGET_FILE:App> || echo App crashed but build succeeded"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Running App.exe..."
)
