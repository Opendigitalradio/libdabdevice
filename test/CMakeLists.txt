find_package(Threads)

cute_test(rtl_file)
target_link_libraries(rtl_file_test ${CMAKE_THREAD_LIBS_INIT})
