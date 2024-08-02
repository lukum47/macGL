cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
set(PROJECT_NAME 3DProject)
project(${PROJECT_NAME})

 set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(3DProject src/main.cpp 		
				src/resources/stb_image.h
				src/resources/camera.h
				src/resources/camera.cpp
				src/render/shaderProgram.h 
				src/render/shaderProgram.cpp
				src/render/texture2D.h 
				src/render/texture2D.cpp)
				
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)
include_directories(external/stb)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)