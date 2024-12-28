cmake_minimum_required(3.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra}")
set(CMAKE_CXX_DEBUG_FLAGS "{CMAKE_CXX_DEBUG_FLAGS} -g -O0}")
set(CMAKE_CXX_RELEASE_FLAGS "{CMAKE_CXX_RELEASE_FLAGS} -O3}")

project(Template)


file(GLOB_RECURSE PROJECT_FILES 
    "src/*.cpp",
    "src/*.h",
)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)