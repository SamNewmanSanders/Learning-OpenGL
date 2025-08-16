cmake_minimum_required(VERSION 3.14)
project(BlankWindow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(SummerProj
    src/main.cpp
    src/glad.c

    src/Application.cpp
    src/Application_setupScene.cpp
    src/Application_update.cpp
    src/Entity.cpp
    src/uiHandler.cpp

    src/Utils/Helpers.cpp
    src/Utils/fpsTracker.cpp
    src/Utils/performanceLogger.cpp

    src/Rendering/Wrappers/Shader.cpp
    src/Rendering/Wrappers/VAO.cpp
    src/Rendering/Wrappers/VBO.cpp
    src/Rendering/Wrappers/EBO.cpp

    src/Rendering/Camera.cpp
    src/Rendering/Renderer.cpp
    src/Rendering/Mesh.cpp
    src/Entity.cpp

    external/imGui/src/imgui.cpp
    external/imGui/src/imgui_demo.cpp
    external/imGui/src/imgui_draw.cpp
    external/imGui/src/imgui_tables.cpp
    external/imGui/src/imgui_widgets.cpp
    external/imGui/src/imgui_impl_glfw.cpp
    external/imGui/src/imgui_impl_opengl3.cpp
)

# after add_executable(MyApp ${SRC_FILES})
add_custom_command(TARGET SummerProj POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/shaders"
            "$<TARGET_FILE_DIR:SummerProj>/shaders"
)

# This copies the entire shaders directory to the output directory after building

# Add external include path
target_include_directories(SummerProj PRIVATE
    ${CMAKE_SOURCE_DIR}/external/include # external headers
    ${CMAKE_SOURCE_DIR}/external/imGui/include # imGui headers (treat like my own code)
    ${CMAKE_SOURCE_DIR}/include          # my own headers
)

# Link GLFW (prebuilt)
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    # IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/lib/glfw3.lib"   # Windows
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/lib/libglfw3.a" # Linux
)
target_include_directories(glfw INTERFACE "${CMAKE_SOURCE_DIR}/external/include")

# Find OpenGL
find_package(OpenGL REQUIRED)

# Link all dependencies
target_link_libraries(SummerProj PRIVATE glfw OpenGL::GL)

if(WIN32)
    target_link_libraries(SummerProj PRIVATE opengl32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(SummerProj PRIVATE dl pthread)
endif()
