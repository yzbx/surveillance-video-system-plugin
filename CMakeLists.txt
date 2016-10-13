project(surveillance-video-system-plugin)
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0037 OLD)
cmake_policy(SET CMP0016 OLD)

find_package(OpenCV REQUIRED)

aux_source_directory(. SRC_LIST)
aux_source_directory(./src SRC_LIST)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
# Find all the cpp files
FILE(GLOB cppfiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


FOREACH(cppfile ${cppfiles})

IF(NOT (cppfile MATCHES "test_.*"))

MESSAGE(STATUS "cpp file: ${cppfile}")
STRING(REGEX REPLACE ".cpp\$" "" basename "${cppfile}")
SET(libfile "${basename}")
MESSAGE(STATUS "lib file: ${libfile}")
add_library(${libfile} SHARED src/${cppfile})
target_link_libraries(${libfile} ${OpenCV_LIBS})
SET(hfile "${basename}.h")
set_property(TARGET ${libfile} PROPERTY PUBLIC_HEADER src/${hfile})
set_target_properties(${libfile}
    PROPERTIES OUTPUT_NAME ${libfile})

SET(testfile "test_${basename}.cpp")
SET(binfile "test_${basename}")
add_executable(${binfile} src/${testfile})
target_link_libraries(${binfile} ${libfile} ${OpenCV_LIBS})
set_target_properties(${binfile}
    PROPERTIES OUTPUT_NAME ${binfile})

install (TARGETS ${libfile} DESTINATION lib)
install (FILES src/${hfile} DESTINATION include/SVS-plugin)

ENDIF(NOT (cppfile MATCHES "test_.*"))

ENDFOREACH(cppfile)

add_executable(main main.cpp)
