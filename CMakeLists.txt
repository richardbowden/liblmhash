cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(lm_hash VERSION 0.1 LANGUAGES C)



# set(CMAKE_EXPORT_COMPILE_COMMANDS true)
# set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# include_directories("${PROJECT_SOURCE_DIR}/include")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)


INCLUDE(CheckCCompilerFlag)
message (STATUS "build type: " ${CMAKE_BUILD_TYPE})
message(STATUS "c bin dir" ${CMAKE_BINARY_DIR})
message(STATUS "c exec dir" ${EXECUTABLE_OUTPUT_PATH})
message(STATUS "c libout dir" ${LIBRARY_OUTPUT_PATH})
message(STATUS "cmake src " ${CMAKE_SOURCE_DIR})
message(STATUS "xxxxx " ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(lmhash ${PROJECT_SOURCE_DIR}/src/main.c ${PROJECT_SOURCE_DIR}/src/lm_hash.c)

set_property(TARGET lmhash PROPERTY C_STANDARD 11)

target_include_directories(lmhash
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        # ${CMAKE_CURRENT_SOURCE_DIR}/src

)
set(PROJECT_HEADERS ${CMAKE_SOURCE_DIR}/include)
target_compile_options(lmhash
    PRIVATE -g -Wall -Wstrict-prototypes -ansi -pedantic
)
