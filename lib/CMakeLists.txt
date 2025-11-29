cmake_minimum_required(VERSION 3.28)

project(Multithreading_lib)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

add_library(Multithreading::library ALIAS ${PROJECT_NAME})
add_library(ThomasEngine::Render STATIC IMPORTED GLOBAL)
add_library(ThomasEngine::Engine STATIC IMPORTED GLOBAL)
add_library(ThomasEngine::Tools STATIC IMPORTED GLOBAL)

set_target_properties(ThomasEngine::Render PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/include/Dep/Render.lib)
set_target_properties(ThomasEngine::Engine PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/include/Dep/Engine.lib)
set_target_properties(ThomasEngine::Engine PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include/Dep)
set_target_properties(ThomasEngine::Tools PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/include/Dep/Tools.lib)
set_target_properties(ThomasEngine::Tools PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/include/Dep)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        ThomasEngine::Engine 
        ThomasEngine::Tools 
        ThomasEngine::Render
)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)



