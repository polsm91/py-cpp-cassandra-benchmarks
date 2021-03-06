project(cppinterf)
cmake_minimum_required(VERSION 2.8)

# Turn on C++11 support (for threading)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")

SET(SOURCES
  ${PROJECT_NAME}module.cpp
)

SET(HEADERS
  ${PROJECT_NAME}.h
)
FIND_LIBRARY(CASS NAMES cassandra  PATHS /home/polsm/local/lib /usr/local/lib)
FIND_LIBRARY(UV NAMES libuv uv PATHS /home/polsm/local/lib /usr/local/lib)
#set( ALL_LIBS "-lcassandra" ${ALL_LIBS} )
set(ALL_LIBS ${CASS} ${UV} ${ALL_LIBS})
SET_SOURCE_FILES_PROPERTIES(${PROJECT_NAME}.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(${PROJECT_NAME}.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(${PROJECT_NAME} python ${PROJECT_NAME}.i ${SOURCES} ${HEADERS}) # I need to link to an external library
SWIG_LINK_LIBRARIES(${PROJECT_NAME} ${ALL_LIBS} ${PYTHON_LIBRARIES})
