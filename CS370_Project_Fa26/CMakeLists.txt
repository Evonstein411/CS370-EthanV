cmake_minimum_required(VERSION 3.17)

#-------------------------------------------------------------
# Include Assignment Information
#-------------------------------------------------------------

# Single assignment_info configuration file (assignments with no milestones)
file(GLOB ASSIGNMENT_INFO_FILE
        "${CMAKE_CURRENT_LIST_DIR}/assignment_info.cmake"
)

foreach(FILE ${ASSIGNMENT_INFO_FILE})
    #    message(STATUS "Including assignment file: ${FILE}")
    include(${FILE})
endforeach()

# Multiple assignment_info_ms# configuration files (assignments with multiple milestones)
file(GLOB ASSIGNMENT_INFO_MS_FILES
        "${CMAKE_CURRENT_LIST_DIR}/assignment_info_ms*.cmake"
)

list(SORT ASSIGNMENT_INFO_MS_FILES)

foreach(FILE ${ASSIGNMENT_INFO_MS_FILES})
    #    message(STATUS "Including milestone file: ${FILE}")
    include(${FILE})
endforeach()

# Resubmit configuration
file(GLOB ASSIGNMENT_INFO_RESUBMIT_FILE
    "${CMAKE_CURRENT_LIST_DIR}/assignment_info_resubmit.cmake"
)

foreach(FILE ${ASSIGNMENT_INFO_RESUBMIT_FILE})
    include(${FILE})
endforeach()

#-------------------------------------------------------------
# Project
#-------------------------------------------------------------
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_SOURCE_DIR}/${SRC_FOLDER}/bin)

#-------------------------------------------------------------
# OpenGL
#-------------------------------------------------------------
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

#-------------------------------------------------------------
# Platform-specific libraries
#-------------------------------------------------------------
if(APPLE)
    find_library(cf_lib CoreFoundation)
    find_library(cg_lib CoreGraphics)
    find_library(cocoa_lib Cocoa)
    find_library(io_lib IOKit)

    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        link_directories(${CMAKE_SOURCE_DIR}/lib/MacOSX_Silicon)
    else()
        link_directories(${CMAKE_SOURCE_DIR}/lib/MacOSX_x86_64)
    endif()

elseif(WIN32)
    file(COPY ${CMAKE_SOURCE_DIR}/lib/Win/glew32.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(COPY ${CMAKE_SOURCE_DIR}/lib/Win/glfw3.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    link_directories(${CMAKE_SOURCE_DIR}/lib/Win)
endif()

link_directories(${CMAKE_SOURCE_DIR}/common)

#-------------------------------------------------------------
# Source Files
#-------------------------------------------------------------
set(SOURCE_FILES
        ${PROJECT_NAME}.cpp
)

set(COMMON_FILES
        ${CMAKE_SOURCE_DIR}/common/GLFWutils.cpp
        ${CMAKE_SOURCE_DIR}/common/shaderutils.cpp
        ${CMAKE_SOURCE_DIR}/common/textureutils.cpp
        ${CMAKE_SOURCE_DIR}/common/objloader.cpp
        ${CMAKE_SOURCE_DIR}/common/tangentspace.cpp
        hud.h
)

#-------------------------------------------------------------
# Configure Target Function
#-------------------------------------------------------------
function(configure_target TARGET)

    target_compile_features(${TARGET} PRIVATE cxx_std_11)
    target_compile_definitions(${TARGET} PRIVATE TARGET_NAME="${TARGET}")

    if(APPLE)
        target_link_libraries(${TARGET}
            ${cf_lib}
            ${cg_lib}
            ${cocoa_lib}
            ${io_lib}
            ${OPENGL_gl_LIBRARY}
            glfw3
            glew
        )
    elseif(WIN32)
        target_link_libraries(${TARGET}
            ${OPENGL_gl_LIBRARY}
            glfw3
            glew32
        )
    else()
        target_link_libraries(${TARGET}
            OpenGL::GL
            glfw
            GLEW
        )
    endif()

endfunction()

#-------------------------------------------------------------
# Determine Executable Targets
#-------------------------------------------------------------

list(LENGTH ASSIGNMENT_INFO_FILE SINGLE_COUNT)
list(LENGTH ASSIGNMENT_INFO_MS_FILES MS_COUNT)
list(LENGTH ASSIGNMENT_INFO_RESUBMIT_FILE RESUBMIT_COUNT)

# Validate configuration
if((SINGLE_COUNT GREATER 0) AND (MS_COUNT GREATER 0))
    message(FATAL_ERROR
        "Cannot mix assignment_info.cmake "
        "with assignment_info_ms*.cmake."
        "A project can use only one of these configuration styles."
    )
endif()

if((SINGLE_COUNT EQUAL 0) AND (MS_COUNT EQUAL 0))
    message(FATAL_ERROR
        "No assignment information files were found."
    )
endif()

if(RESUBMIT_COUNT GREATER 1)
    message(FATAL_ERROR
        "Only one assignment_info_resubmit.cmake is permitted."
    )
endif()

# Build the list of executable targets
set(TARGET_LIST)

# Standard executables (single and multi-milestone)
if(SINGLE_COUNT EQUAL 1)
    list(APPEND TARGET_LIST ${PROJECT_NAME})
else()
    foreach(FILE ${ASSIGNMENT_INFO_MS_FILES})
        get_filename_component(FILE_NAME ${FILE} NAME)
        string(REGEX MATCH "ms([0-9]+)" MATCH "${FILE_NAME}")
        if(NOT MATCH)
            message(FATAL_ERROR
                    "Unable to determine milestone from ${FILE_NAME}"
            )
        endif()

        string(TOUPPER "${MATCH}" TARGET_SUFFIX)
        list(APPEND TARGET_LIST
                ${PROJECT_NAME}_${TARGET_SUFFIX}
        )
    endforeach()
endif()

# resubmit executables (useful for exams)
if(RESUBMIT_COUNT EQUAL 1)
    list(APPEND TARGET_LIST ${PROJECT_NAME}_RESUBMIT)
endif()

message(STATUS "")
message(STATUS "Executable Targets:")

foreach(TARGET ${TARGET_LIST})
    message(STATUS "    ${TARGET}")
endforeach()

#-------------------------------------------------------------
# Create Executables
#-------------------------------------------------------------

foreach(TARGET ${TARGET_LIST})
    add_executable(${TARGET}
            ${SOURCE_FILES}
            ${COMMON_FILES}
    )
    configure_target(${TARGET})
endforeach()
