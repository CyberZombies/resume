cmake_minimum_required(VERSION 3.28)
project(GTKTutorial)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK4 REQUIRED gtk+-3.0)
include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(TutorialApp interface.c)
set_target_properties(TutorialApp PROPERTIES WIN32_EXECUTABLE true)
target_link_libraries(TutorialApp PRIVATE ${GTK4_LIBRARIES})

