cmake_minimum_required(VERSION 3.3)
project(cassandraBenchmark)

set(CMAKE_CXX_STANDARD 11)

find_package( Threads )

set(SOURCE_FILES
        sync.cpp
        CMakeLists.txt)


add_executable(sequential sync.cpp)
target_link_libraries( sequential  /usr/local/lib64/libcassandra.so )
set_property(
        TARGET sequential
        APPEND PROPERTY COMPILE_FLAGS ${CASS_EXAMPLE_C_FLAGS})


add_executable(callback  callback.cpp )
target_link_libraries( callback  /usr/local/lib64/libcassandra.so ${CMAKE_THREAD_LIBS_INIT} )
set_property(
        TARGET callback
        APPEND PROPERTY COMPILE_FLAGS ${CASS_EXAMPLE_C_FLAGS})
