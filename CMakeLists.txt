# = COMMON_SETTINGS ====================================== #
cmake_minimum_required(VERSION 3.0.0)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

# = LIB_IMAGE ============================================ #
PROJECT(_image)
add_library(_image SHARED src/image.cpp)
target_include_directories(_image PRIVATE inc)

# = LIB_API ============================================== #
project(_api)
if(UNIX)
    add_library(_api SHARED api/rpi_raspistill.cpp)
else()
    add_library(_api SHARED api/win_fake.cpp)
endif()
target_include_directories(_api PRIVATE api)
target_link_libraries(_api _image)

# = MAIN PROJECT ========================================= #
project(document_scanner)
add_executable(document_scanner main.cpp src/image.cpp)
target_include_directories(document_scanner PRIVATE inc)
target_link_libraries(document_scanner _api _image)

# = END ================================================== #
