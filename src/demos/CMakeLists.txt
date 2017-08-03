if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${${PROJECT_NAME}_LOWER}_demos")

add_executable(rtl_dump
  rtl_dump.cpp
  )

target_link_libraries(rtl_dump
  dabdevice
  rtlsdr
  )
