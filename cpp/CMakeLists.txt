cmake_minimum_required(VERSION 3.3)
project(cassandraBenchmark)

set(CMAKE_CXX_STANDARD 11)

find_package( Threads )

set(SOURCE_FILES
        sync.cpp
	single_callback.cpp)


add_executable(sequential sync.cpp)
target_link_libraries( sequential  /usr/local/lib64/libcassandra.so )
set_property(
        TARGET sequential
        APPEND PROPERTY COMPILE_FLAGS ${CASS_EXAMPLE_C_FLAGS})


add_executable(single_callback  single_callback.cpp )
target_link_libraries( single_callback  /usr/local/lib64/libcassandra.so ${CMAKE_THREAD_LIBS_INIT} )
set_property(
        TARGET single_callback
        APPEND PROPERTY COMPILE_FLAGS ${CASS_EXAMPLE_C_FLAGS})


add_executable(sync_threads sync_threads.cpp)
target_link_libraries( sync_threads  /usr/local/lib64/libcassandra.so )
set_property(
	        TARGET sync_threads
		        APPEND PROPERTY COMPILE_FLAGS ${CASS_EXAMPLE_C_FLAGS})


add_executable(multi_callback multi_callback.cpp)
target_link_libraries( multi_callback  /usr/local/lib64/libcassandra.so )
set_property(
	        TARGET multi_callback
		        APPEND PROPERTY COMPILE_FLAGS ${CASS_EXAMPLE_C_FLAGS})


