cmake_minimum_required(VERSION 3.28)

project(Multithreading_exec)

file(GLOB_RECURSE SOURCE
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        Multithreading::library  
        ThomasEngine::Engine 
        ThomasEngine::Tools 
        ThomasEngine::Render
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res/obj"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res/textures"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/textures"
)