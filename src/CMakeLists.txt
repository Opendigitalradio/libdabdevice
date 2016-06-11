add_executable(demo demo.cpp)
target_link_libraries(demo ${CMAKE_THREAD_LIBS_INIT} rtlsdr)
