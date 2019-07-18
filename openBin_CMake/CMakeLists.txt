cmake_minimum_required(VERSION 2.8.9)
set(EXECUTABLE_NAME openBin)
project(${EXECUTABLE_NAME})

set(CMAKE_CXX_FLAGS " -lz ${CMAKE_CXX_FLAGS}")
if(UNIX AND APPLE)
    set(CMAKE_CXX_FLAGS " -stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

#Designate a variable to store the location of header files
set(HEADER_LOCATION include)

#Designate a variable to store the location of source files
set(SOURCES 
	src/elzip.cpp
	src/openBin.cpp
	src/stdafx.cpp
	src/pch.cpp
	)
	
#Specify static libraries used for the project
find_library(ZLIB_LIB
	NAMES zlibstat zlibstatd
	)

#set(PROJECT_LINK_LIBS 
link_libraries(stdc++fs)

# unzip
set(UNZIP_SOURCES minizip/src/unzip.c
minizip/src/zip.c
minizip/src/ioapi.c)
set(UNZIP_HEADERS minizip/src/unzip.h
minizip/src/zip.h
minizip/src/ioapi.h)

add_library(unzip STATIC 
"${UNZIP_SOURCES}"
"${UNZIP_HEADERS}")

install(FILES ${UNZIP_HEADERS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include")

find_library(UNZIP_LIB NAMES unzip)

# unzipper and zipper
set(ZIPUTILS_SOURCES 
	src/unzipper.cpp
	src/zipper.cpp)

set(ZIPUTILS_HEADER
	include/unzipper.h
	include/zipper.h)

add_library(ziputils STATIC ${ZIPUTILS_SOURCES} ${ZIPUTILS_HEADER})

install(FILES ${ZIPUTILS_HEADER} DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
find_library(ZIPUTILS_LIB NAMES ziputils)


# elzip
set(ELZIP_SOURCES 
	src/elzip.cpp)

set(ELZIP_HEADERS
	include/elzip.hpp)

add_library(elzip STATIC ${ELZIP_SOURCES} ${ELZIP_HEADERS})

install(FILES ${ELZIP_HEADERS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
find_library(ELZIP_LIB NAMES elzip)

# Other stuff...
target_link_libraries(unzip elzip ziputils zlibstatic)


#Include header files
include_directories(include)

add_executable(${EXECUTABLE_NAME} ${UNZIP_SOURCES} ${ELZIP_SOURCES} ${ZIPUTILS_SOURCES} ${SOURCES})
#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} ${EXECUTABLE_NAME})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${ZLIB_LIB} ${UNZIP_LIB} ${ZIPUTILS_LIB} ${ELZIP_LIB} "$<$<CXX_COMPILER_ID:MSVC>:-SAFESEH:NO>")

