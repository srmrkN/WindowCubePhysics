cmake_minimum_required(VERSION 3.15)
project(OpenGLTest)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH "C:/Users/User/.vcpkg-clion/vcpkg/installed/x64-mingw-dynamic")

# Находим все необходимые пакеты
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Добавляем файлы реализации бэкенда ImGui
add_executable(${PROJECT_NAME}
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
        types.h
        shaders.h
        shaders.cpp
        physics.h
        render.h
        render.cpp
        camera.h
        camera.cpp
        physics.cpp
        gui.cpp
        gui.h
)

# Добавляем пути включения
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Линкуем библиотеки
target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenGL::GL
        GLEW::GLEW
        glfw
        glm::glm
        ${BULLET_LIBRARIES}
        imgui::imgui
)

if(WIN32)
    # Копирование DLL из vcpkg
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:GLEW::GLEW>
            $<TARGET_FILE:glfw>
            # Добавьте другие DLL из vcpkg
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

    # Копирование системных DLL из MinGW
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CXX_COMPILER}/../libstdc++-6.dll"
            "${CMAKE_CXX_COMPILER}/../libgcc_s_seh-1.dll"
            "${CMAKE_CXX_COMPILER}/../libwinpthread-1.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()