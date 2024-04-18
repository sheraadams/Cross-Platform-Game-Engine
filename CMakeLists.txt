cmake_minimum_required (VERSION 3.0)
cmake_policy(VERSION 3.0)

project(SheraAdamsProject330)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/lib)

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

find_package(GLFW3 REQUIRED)

# Explicitly set the context API based on the platform
if(WIN32)
    set(GLFW_EXPOSE_NATIVE_WIN32 ON CACHE BOOL "GLFW expose native Win32" FORCE)
elseif(APPLE)
    set(GLFW_EXPOSE_NATIVE_COCOA ON CACHE BOOL "GLFW expose native Cocoa" FORCE)
elseif(UNIX AND NOT APPLE)
    set(GLFW_EXPOSE_NATIVE_X11 ON CACHE BOOL "GLFW expose native X11" FORCE)
endif()

# Include the relevant directories
include_directories(${GLFW3_INCLUDE_DIR})

message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
# find_package(SOIL REQUIRED)
# message(STATUS "Found SOIL in ${SOIL_INCLUDE_DIR}")
# find_package(GLEW REQUIRED)
# message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")
if(APPLE)
    find_package(Freetype REQUIRED)
    message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

if(WIN32)
    set(LIBS glfw3 opengl32 assimp freetype)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    find_package(OpenGL REQUIRED)
    add_definitions(${OPENGL_DEFINITIONS})
    find_package(X11 REQUIRED)
    # note that the order is important for setting the libs
    # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
    set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
    set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
    INCLUDE_DIRECTORIES(/System/Library/Frameworks)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
    FIND_LIBRARY(IOKit_LIBRARY IOKit)
    FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
    MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
    SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
    SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
    set(LIBS ${LIBS} ${APPLE_LIBS})
else()
    set(LIBS )
endif(WIN32)

set(projectS
        sa
        )

set(sa
        app
        )

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# first create relevant static libraries required for other projects
add_library(STB_IMAGE "src/stb_image.cpp" includes/imgui/backends/imgui_impl_glfw.cpp)
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c" includes/imgui/backends/imgui_impl_glfw.cpp)
set(LIBS ${LIBS} GLAD)

# Add ImGui source files to the imgui library
add_library(IMGUI
        "src/imgui.cpp"
        "src/imgui_version.cpp"
        "src/imgui_draw.cpp"
        "src/imgui_tables.cpp"
        "src/imgui_widgets.cpp"
        "src/backends/imgui_impl_glfw.cpp"
        "src/backends/imgui_impl_opengl3.cpp"
        includes/imgui/backends/imgui_impl_glfw.cpp)
set(LIBS ${LIBS} IMGUI)

macro(makeLink src dest target)
    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest}  DEPENDS  ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

function(create_project_from_sources project version)
    file(GLOB SOURCE
            "src/${project}/${version}/*.h"
            "src/${project}/${version}/*.cpp"
            "src/${project}/${version}/*.vs"
            "src/${project}/${version}/*.fs"
            "src/${project}/${version}/*.tcs"
            "src/${project}/${version}/*.tes"
            "src/${project}/${version}/*.gs"
            "src/${project}/${version}/*.cs"
            )
    if (version STREQUAL "")
        SET(replaced "")
        string(REPLACE "/" "_" replaced ${project})
        set(NAME ${replaced})
    else()
        set(NAME "${project}__${version}")
    endif()
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} ${LIBS})
    if(MSVC)
        target_compile_options(${NAME} PRIVATE /std:c++17 /MP)
        target_link_options(${NAME} PUBLIC /ignore:4099)
    endif(MSVC)
    if(WIN32)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${project}")
        set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${project}/Debug")
    elseif(UNIX AND NOT APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${project}")
    elseif(APPLE)
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${project}")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${project}")
        set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${project}")
    endif(WIN32)
    # copy shader files to build directory
    file(GLOB SHADERS
            "src/${project}/${version}/*.vs"
            # "src/${project}/${version}/*.frag"
            "src/${project}/${version}/*.fs"
            "src/${project}/${version}/*.tcs"
            "src/${project}/${version}/*.tes"
            "src/${project}/${version}/*.gs"
            "src/${project}/${version}/*.cs"
            )
    # copy dlls
    file(GLOB DLLS "dlls/*.dll")
    foreach(SHADER ${SHADERS})
        if(WIN32)
            # configure_file(${SHADER} "test")
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
            add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
        elseif(UNIX AND NOT APPLE)
            file(COPY ${SHADER} DESTINATION ${CMAKE_SOURCE_DIR}/bin/${project})
        elseif(APPLE)
            # create symbolic link for *.vs *.fs *.gs
            get_filename_component(SHADERNAME ${SHADER} NAME)
            makeLink(${SHADER} ${CMAKE_SOURCE_DIR}/bin/${project}/${SHADERNAME} ${NAME})
        endif(WIN32)
    endforeach(SHADER)
    # if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
    if(MSVC)
        configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
    endif(MSVC)
    add_custom_command(TARGET ${NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/resources/fonts/OCRAEXT.TTF
            $<TARGET_FILE_DIR:${NAME}>)

endfunction()

# then create a project file per tutorial
foreach(project ${projectS})
    foreach(version ${${project}})
        create_project_from_sources(${project} ${version})
    endforeach(version)
endforeach(project)


include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/includes/imgui)

include_directories(${CMAKE_SOURCE_DIR}/includes/imgui/backends)
include_directories(${CMAKE_SOURCE_DIR}/includes/imgui/misc)
