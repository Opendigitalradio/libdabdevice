cmake_minimum_required(VERSION 3.1)
project("DABDevice"
  LANGUAGES CXX
  VERSION 1.0.1)

string(TOUPPER ${PROJECT_NAME} ${PROJECT_NAME}_UPPER)
string(TOLOWER ${PROJECT_NAME} ${PROJECT_NAME}_LOWER)

option(${${PROJECT_NAME}_UPPER}_ENABLE_TESTS
  "Build and run the ${PROJECT_NAME} unit tests."
  OFF
  )

if(${${${PROJECT_NAME}_UPPER}_ENABLE_TESTS})
  set(CONAN_OPTIONS libdabdevice:test=True)
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")
set(CXX_VERSION "c++11")

include("CompilerSetup")
include("ConanPackages")

install_conan_packages(SYSTEM_HEADERS
  PKGOPTS ${CONAN_OPTIONS}
  )

find_package(Boost REQUIRED)

add_subdirectory("common")
add_subdirectory("include")

if(${${PROJECT_NAME}_UPPER}_ENABLE_TESTS)
  include("CUTE")
  add_subdirectory(test)
endif()
