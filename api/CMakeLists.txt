# = CAMERA API =========================================== #
cmake_minimum_required(VERSION 3.0)

project(camera_api)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# ======================================================== #
set(INCLUDE ../inc) # directories
set(SOURCE ../src/image.cpp) # files

# ======================================================== #
include(GenerateExportHeader)
if(UNIX)
    add_library(camera_api SHARED rpi_raspistill.cpp ${INCLUDE} ${SOURCE})
else()
    add_library(camera_api SHARED win_fake.cpp ${INCLUDE} ${SOURCE})
endif()
generate_export_header(camera_api)

set_target_properties(camera_api PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ../
    ARCHIVE_OUTPUT_DIRECTORY ../
    RUNTIME_OUTPUT_DIRECTORY ../)

# = END ================================================== #
