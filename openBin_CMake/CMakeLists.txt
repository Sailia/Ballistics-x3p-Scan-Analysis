cmake_minimum_required(VERSION 2.8.9)
set(EXECUTABLE_NAME openBin)
project(${EXECUTABLE_NAME})

#Designate a variable to store the location of header files
set(HEADER_LOCATION include)

#Designate a variable to store the location of source files
set(SOURCES 
	src/elzip.cpp
	src/openBin.cpp
	src/stdafx.cpp
	src/unzipper.cpp
	src/zipper.cpp
	src/pch.cpp
	)
	
#Specify static libraries used for the project
find_library(ZLIB_LIB
	NAMES zlibstat zlibstatd
	)
#set(PROJECT_LINK_LIBS 


#Include header files
include_directories(include)

add_executable(${EXECUTABLE_NAME} ${SOURCES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EXECUTABLE_NAME})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${ZLIB_LIB} "$<$<CXX_COMPILER_ID:MSVC>:-SAFESEH:NO>")

