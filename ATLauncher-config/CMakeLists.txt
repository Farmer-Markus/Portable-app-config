cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
project(config)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(GRESOURCE_C   gresource.c)
set(GRESOURCE_XML gresource.xml)

add_custom_command(
    OUTPUT ${GRESOURCE_C}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${GLIB_COMPILE_RESOURCES}
    ARGS
        --target=${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
        --generate-source
        ${GRESOURCE_XML}
    VERBATIM
    MAIN_DEPENDENCY ${GRESOURCE_XML}
)

add_custom_target(
    config-resource
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
)


find_package(PkgConfig REQUIRED)
find_package(cJSON REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
link_directories(${GTK3_LIBRARY_DIRS})
set(LIBRARIES ${LIBRARIES} ${GTK3_LIBRARIES} ${CJSON_LIBRARIES})

set(FLAGS "-I${GTK3_INCLUDE_DIRS}")
string(REPLACE ";" " -I" FLAGS "${FLAGS}")
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${GTK3_FLAGS} ${FLAGS})

add_executable(config main.c ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C})

set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${GRESOURCE_C}
    PROPERTIES GENERATED TRUE
)

add_dependencies(${PROJECT_NAME} config-resource)

target_link_libraries(config ${LIBRARIES})
