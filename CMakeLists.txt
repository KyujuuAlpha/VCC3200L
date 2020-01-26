cmake_minimum_required(VERSION 3.0)

project(vcc3200l)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(PkgConfig REQUIRED)
find_package (Threads REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

file(GLOB src_files ${PROJECT_SOURCE_DIR}/src/*.c)

add_library(vcc3200l STATIC ${src_files})

target_link_libraries(vcc3200l ${GTK3_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(vcc3200l PUBLIC ${PROJECT_SOURCE_DIR}/inc)
install(TARGETS vcc3200l DESTINATION ${PROJECT_SOURCE_DIR}/bin)