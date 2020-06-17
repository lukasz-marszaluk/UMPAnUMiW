# = MAIN PROJECT ========================================= #
cmake_minimum_required(VERSION 3.0)

project(document_scanner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# ======================================================== #
set(INCLUDE inc) # directories
set(SOURCE src/image.cpp src/common.cpp src/document.cpp) # files

# ======================================================== #
add_subdirectory(api)
add_executable(document_scanner main.cpp ${INCLUDE} ${SOURCE})

target_link_libraries(document_scanner PRIVATE camera_api)
add_dependencies(document_scanner camera_api)

# = END ================================================== #
